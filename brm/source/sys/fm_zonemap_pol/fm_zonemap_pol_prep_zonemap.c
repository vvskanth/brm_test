/**************************************************************************
*
*	fm_zonemap_pol_prep_zonemap.c
*
* Copyright (c) 2000, 2023,  2024, Oracle and/or its affiliates. 
*
*      This material is the confidential property of Oracle Corporation
*      or licensors and may be used, reproduced, stored or transmitted only 
*      in accordance with a valid Oracle license or sublicense agreement.
*
**************************************************************************/
#ifndef lint
static char *_csrc = "@(#)$Id: fm_zonemap_pol_prep_zonemap.c /cgbubrm_7.5.0.portalbase/1 2023/07/18 03:57:49 visheora Exp $";
#endif

#include <sys/types.h>
#include <netinet/in.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "pcm.h"
#include "ops/zonemap.h"
#include "pinlog.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_zonemap.h"
#include "fm_zonemap_pol.h"

#ifdef _DEBUG
#define IPT_DEBUG
#endif

/* global values */
/* base address of the blob */
telephony_offset_t blob_base_addr;
/* current offset of the trie node block */
char *trie_vp;
/* current offset of the data node block */
char *data_vp;
/* current offset of the zones block */
char *zone_vp;
/* current offset of the zones data block */
char *zone_data_vp;

/*****************************************************************
 * Temporary table data structure
 *
 *	| phone number/IP address	| ancestor element id array	|
 *	---------------------------------------------------------
 *	|		415		|	{0, 0, 0, 1}		|
 *****************************************************************/
typedef struct TAG_TmpTableRowData_t {
	char			*content_p;
	uintptr_t		*ancestors_a;
} tmp_table_row_data_t;

typedef tmp_table_row_data_t *table_row_t;

/********************************************************************
 * Rountines used within
 ********************************************************************/
static int32 
fm_zonemap_pol_get_num_of_nodes(
	table_row_t		*tablep,
	int32			table_size,
	int32			*node_data_size,
	pin_errbuf_t		*ebufp);

static void 
fm_zonemap_pol_fill_blob(
	table_row_t		*tablep,
	int32			table_size,
	Blob_t			*blobpp,
	pin_flist_t		*error_flistp,
	pin_errbuf_t		*ebufp);

static void 
fm_zonemap_pol_insert_path_to_trie(
	uintptr_t		path_a[],
	char			*str,
	telephony_offset_t	*nodep,
	table_row_t             *tablep,
	int32			row_id,
	int32			table_size,
	int32			depth,
	pin_flist_t		*error_flistp,
	pin_errbuf_t		*ebufp);

static void 
fm_zonemap_pol_insert_path(
	uintptr_t		path_a[],
	TrieNode_t		*nodep,
	pin_flist_t		*error_flistp,
	pin_errbuf_t		*ebufp);

static int32
fm_util_ipt_canon_phone_number(
	char			*pszIn, 
	char			*pszOut);

static void 
fm_zonemap_pol_create_tmp_table(
	pin_flist_t		*zone_flistp,
	table_row_t		*tablep,
	uintptr_t		path_a[],
	int32			size,
	uintptr_t		*cur,
	pin_errbuf_t		*ebufp);

static int32	
fm_zonemap_pol_get_tmp_table_size(
	pin_flist_t		*zone_flistp,
	pin_errbuf_t		*ebufp) ;

static int32
fm_util_zonemap_compare_row(
	const void		*arg1_p,
	const void		*arg2_p);

static void
dealloc_tmp_table(table_row_t *tablep);


static int32
fm_zonemap_pol_get_num_child_nodes(
	table_row_t		*tablep,
	int32			row,
	int32			size,
	int32			len,
	const char		*source);

/*******************************************************************
 * Function bodies.
 *******************************************************************/

/*******************************************************************
 * fm_zonemap_pol_construct_blob
 * save the flist and the trie in a single block of memory
 *******************************************************************/

void  
fm_zonemap_pol_construct_blob(
	pin_flist_t		*zonemap_flistp,
	pin_flist_t		*return_flistp,
	pin_errbuf_t		*ebufp)
{
	Blob_t			*blob_base_p = NULL;
	pin_buf_t		*bufp = NULL;
	tree_node_data_t	*zonetreep = NULL;	
	pin_flist_t		*e_flistp = NULL;
	pin_errbuf_t		local_ebuf;

	/* a temporary table created in order to figure out */
	/* the size of trie and to help construct the trie. */
	table_row_t		*tablep = (table_row_t *)NULL;
	int32			node_count = 0;
	int32			zone_count = 0;
	int32			blob_size = 0;
	int32			node_size = 0;
	int32			data_size = 0;
	int32			table_size = 0;
	int32			zone_node_size = 0;
	int32			zone_data_size = 0;
	uintptr_t		path_a[PIN_MAX_PATH];
	int32			result = PIN_RESULT_PASS;
	pin_zonemap_err_t	res_code = PIN_ZONEMAP_ERR_NONE;
	int32			i = 0;
	uintptr_t		cur_row = 0;
	int32			child_count;
	int32			child_size;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);	

	/*
	 * Check to see if zonemap_flistp is null or not,
	 * and if it is null, exit without further 
	 * processing. However, it is not considered 
	 * as an error condition.
	 */	
	if (zonemap_flistp == NULL) {
		bufp = (pin_buf_t *)malloc(sizeof(pin_buf_t));
		if (!bufp) {
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_NO_MEM, 0, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Memory allocation error in constructing "
				"zonemap data.", ebufp);
			return;
		}
		bufp->flag = 0;
		bufp->size = 0;
		bufp->offset = 0;
		bufp->data = (char *)NULL;
		bufp->xbuf_file = (char *)NULL;
		PIN_FLIST_FLD_PUT(return_flistp,
			PIN_FLD_ZONEMAP_DATA_DERIVED, bufp, ebufp);
		return;
	}

	/* Create a tmp table for easy traverse */
	for (i = 0; i < PIN_MAX_PATH; i++) {
		path_a[i] = 0;
	}

	table_size = fm_zonemap_pol_get_tmp_table_size(zonemap_flistp, ebufp);
	if (table_size != 0) {
		tablep = (table_row_t *)malloc(sizeof(table_row_t) * (table_size + 1));
		if (tablep != NULL) {
			for (i = 0; i < table_size; i++) {
				tablep[i] = (tmp_table_row_data_t *)NULL;
			}
			fm_zonemap_pol_create_tmp_table(zonemap_flistp,
				tablep, path_a, 0, &cur_row, ebufp);
			*(tablep + table_size) = (tmp_table_row_data_t *)NULL;
		} else {
			pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
				PIN_ERR_NO_MEM, 0, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Memory allocation error in constructing blob.", ebufp);
			return;
		}
	} else {
		tablep = (table_row_t *)NULL;
	}

	/*
	 * Get number of trie nodes, child space, and the data size. For a trie, the number
	 * of children is always one fewer than the number of nodes (nothing points to root).
	 */
	node_count = fm_zonemap_pol_get_num_of_nodes(tablep, table_size, &data_size, ebufp);
	child_count = node_count - 1;
	child_size = child_count * sizeof(child_index_t); /* aligned as 2 * 32-bit numbers */

	/*
	 * Get the number of zones and the size of serialized zone tree.
	 */
	zone_count = fm_zonemap_pol_serialize_get_size(zonemap_flistp, &zone_data_size, ebufp);
	zone_node_size = ((int)sizeof(tree_node_data_t))*zone_count;

	/***************************************************
	* The format of the blob is:
	* | blob header | trie nodes | data nodes | zones |
	*       
	*	blob header 
	*		|
	*		+------ | version		|
	*				+ blob size	|
	*				+ node size	|
	*				+ data size	|
	*				+ zone size	|
	*				+ trie start	|
	*				+ data start	|
	*				+ zone start	|
	*	trie node
	*		child_entry
	*		child_entry
	*			|
	*			+ ------ | child node
	*					+ terminal flag
	*					+ leaf addr
	*					+ nChildern
	*					child_entry
	*					child_entry
	*	trie node
	*		child_entry
	*			|
	*			+------...
	*			...
	*
	*	data node
	*		|
	*		+------	| ancestor array  |
	*	data node
	*		|
	*		+------ ...
	*		...
	*	zone node
	*		|
	*		+------ ...
	*	zone node
	*		|
	*		+------ ...
	*	zone data node
	*		|
	*		+------ ...
	****************************************************/
			
	node_size = node_count * ((int)sizeof(TrieNode_t));
	blob_size = ((int)sizeof(Blob_t)) + 
		node_size +
	        child_size +	
		data_size +
		zone_node_size +
		zone_data_size;

	ROUNDUP(blob_size, (int)sizeof(intptr_t));

	/* allocate memory for the block */
	blob_base_p = (Blob_t *)malloc(blob_size + 1);
	
	/* check whether the memory is allocated successfully */
	if (blob_base_p == NULL) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION,
			PIN_ERR_NO_MEM, 0, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_zonemap_pol_construct_trie: \
			failed to allocate memory for blob.", ebufp);
	
		goto cleanup;
	}
	
	/* initialize the blob */
	pin_memset(blob_base_p, (size_t)blob_size + 1, 0, (size_t)blob_size + 1);

	/* fill in the blob data */
	blob_base_addr = (telephony_offset_t)blob_base_p;

	trie_vp = (char *)(blob_base_p);
	data_vp = (char *)(blob_base_p);
	
	/* fill header of this blob */
	/* version number of this blob */
	blob_base_p->version = htonl(ZONEMAP_CURRENT_VERSION);
	blob_base_p->blob_size = blob_size;
	trie_vp += (sizeof(Blob_t));
	MEM_ALIGN(trie_vp, sizeof(uintptr_t));
		
	/* debug */
	blob_base_p->trie_start = (telephony_offset_t)trie_vp;
	blob_base_p->node_size = node_size + child_size;

	data_vp = trie_vp + blob_base_p->node_size;
	MEM_ALIGN(data_vp, sizeof(uintptr_t));
	
	/* debug */
	blob_base_p->data_start = (telephony_offset_t)data_vp;
	blob_base_p->data_size = data_size;

	zone_vp = data_vp + data_size;
	MEM_ALIGN(zone_vp, sizeof(uintptr_t));

	/* debug */
	blob_base_p->zone_start = (telephony_offset_t)zone_vp;
	blob_base_p->zone_size = zone_node_size;

	zone_data_vp = zone_vp + zone_node_size;
	MEM_ALIGN(zone_data_vp, sizeof(uintptr_t));

	blob_base_p->zone_data_start = (telephony_offset_t)zone_data_vp;
	blob_base_p->zone_data_size = zone_data_size;

	/*
	 * Create a result flist that stores the error
	 * information when constructing the trie.
	 * For now, the error is none.
	 */
	e_flistp = PIN_FLIST_CREATE(ebufp);
	PIN_FLIST_FLD_SET(e_flistp, PIN_FLD_RESULT, (void *)&result, ebufp);
	PIN_FLIST_FLD_SET(e_flistp, PIN_FLD_RESULT_FORMAT, (void *)&res_code, ebufp);

	/*
	 * Now fill the trie part of the blob. At the same time the error flist will be set
	 * if anything goes wrong.
	 */ 
	fm_zonemap_pol_fill_blob(tablep,
		table_size,
		blob_base_p,
		e_flistp,
		ebufp);
	
#ifdef	IPT_DEBUG
	PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_DEBUG,
			"fm_zonemap_pol_fill_blob: Returned", ebufp);
#endif

	/* Serialize the zonemap tree. */
	zonetreep = (tree_node_data_t *)zone_vp;
	fm_zonemap_pol_serialize_zone_tree(zonemap_flistp, zonetreep, 0, ebufp);

	/* if successful */
	if (!PIN_ERR_IS_ERR(ebufp)) {
		/**************************************************
		 * if successful, construct the buffer and put 
		 * it onto return flist.
		 **************************************************/

		/* append null character */
		*((char *)(blob_base_p) + blob_size) = '\0';
		
		/* change the actual address to offset */
		/* and change the byte order from host to network */
		fm_zonemap_pol_post_process_blob(blob_base_p);

		/* No error! Now construct return buffer and return flist */
		bufp = (pin_buf_t *)malloc(sizeof(pin_buf_t));
		if (bufp == NULL) {
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_NO_MEM,
				PIN_FLD_ZONEMAP_DATA_DERIVED, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_zonemap_pol_construct_blob:\
				failed to allocate memory", ebufp);
			free((char *)blob_base_p);

			goto cleanup;
		}
	
		bufp->data = (caddr_t)blob_base_p;
		bufp->size = blob_size + 1;

		bufp->flag = 0;
		bufp->offset = 0;
		bufp->xbuf_file = (char *)NULL;

		 /* Plob buf into flist */
		PIN_FLIST_FLD_PUT(return_flistp, PIN_FLD_ZONEMAP_DATA_DERIVED, bufp, ebufp);
	} else {
		free((char *)blob_base_p);
		free((void *)bufp);

		/* Attach the result flist to the output flist. */
		PIN_ERR_CLEAR_ERR(&local_ebuf);
                PIN_FLIST_CONCAT(return_flistp, e_flistp, &local_ebuf);
	}
cleanup:
	/* Clean up. */
	dealloc_tmp_table(tablep);
	if (e_flistp) {
		PIN_FLIST_DESTROY_EX(&e_flistp, NULL);
	}
	return;
}

/*******************************************************************
 * dealloc_tmp_table
 * Deallocates all memory in temporary table.
 *******************************************************************/
static void
dealloc_tmp_table(table_row_t *tablep)
{
	table_row_t		*row;

	if (tablep == (table_row_t *)NULL) {
		return;
	}

	for (row = tablep; *row != (table_row_t)NULL; row++) {
		tmp_table_row_data_t *row_data = *row;
		free(row_data->ancestors_a);
		free(row_data);
	}
	free(tablep);
}
	
/*******************************************************************
 * fm_zonemap_pol_fill_blob
 * converts the zonemap into a trie
 *******************************************************************/
static void 
fm_zonemap_pol_fill_blob(
	table_row_t		*tablep,
	int32			table_size,
	Blob_t			*blobp,
	pin_flist_t		*error_flistp,
	pin_errbuf_t		*ebufp)
{
	tmp_table_row_data_t	*datap = NULL;
	table_row_t		*cur_rowp = NULL;
	char			*bufp = NULL;
	char			canon_bufp[PIN_MAX_PATH];
	telephony_offset_t	offset = 0;
	int32			row_id = 0;
	int32			depth = 0;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	if (tablep == NULL) {
		return;
	}
	
	cur_rowp = tablep;
	while ((*cur_rowp) != NULL) {
		datap = *cur_rowp;
		bufp = datap->content_p;
		/* Canonicalize numbers */
		fm_util_ipt_canon_phone_number(bufp, canon_bufp);
		fm_zonemap_pol_insert_path_to_trie(
			datap->ancestors_a, 
			canon_bufp,
			&offset,
			tablep,
			row_id,
			table_size,
			depth,	
			error_flistp,
			ebufp);  

		cur_rowp++;
		row_id++;
	}
}

/*******************************************************************
 * fm_zonemap_pol_insert_path_to_trie
 * insert the zone to the right place of the trie
 *******************************************************************/
static void 
fm_zonemap_pol_insert_path_to_trie(
	uintptr_t		path_a[],
	char			*str,
	telephony_offset_t	*triep,
	table_row_t             *tablep,
	int32			row_id,
	int32			table_size,
	int32			depth,
	pin_flist_t		*error_flistp,
	pin_errbuf_t		*ebufp)
{
	char			current;
	int32			index = -1;
	TrieNode_t		*nodep = NULL;
	int32			result = PIN_RESULT_PASS;
	pin_zonemap_err_t	res_code = PIN_ZONEMAP_ERR_NONE;   
	child_index_t		*pChildIndex = NULL;
	int32			child_count = 0;
	telephony_offset_t	offset = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	current = *str;

	/*
 	 * If we are encountering a new node, we need to allocate it... Otherwise, just cast
 	 * the telephony_offset_t into the TrieNode_t * pointer.
 	 */

	if (*triep == 0) {
		/*
		 * Allocate a new node (actual memory was allocated as single blob earlier, so 'allocation' is
		 * just finding a new location in the blob. We need to determine how many child nodes this will
		 * have so that we can create space after the TrieNode_t for the child pointer structure. The
		 * ordered table of 'terminal' strings is used to calculate this.
		 */
		child_count = fm_zonemap_pol_get_num_child_nodes(tablep, row_id, table_size, depth, "TRIE");
		*triep = (telephony_offset_t)trie_vp;
		trie_vp += sizeof(TrieNode_t);
		trie_vp += (sizeof(child_index_t) * child_count);
	}
	nodep = (TrieNode_t *)*triep;

	/* insert current character */
	if (current != '\0') {
		/* there is more input in the string */
		index = EXTENDED_CHARSET_INDEX(current);
		
		if (index >= EXTENDED_CHARSETSIZE || 
			index < 0) {
			result = PIN_RESULT_FAIL;
			res_code = PIN_ZONEMAP_ERR_ILLEGAL_DATA;
			PIN_FLIST_FLD_SET(error_flistp,
				PIN_FLD_RESULT,
				(void *)&result, ebufp);
			PIN_FLIST_FLD_SET(error_flistp,
				PIN_FLD_RESULT_FORMAT,
				(void *)&res_code, ebufp);
			PIN_FLIST_FLD_SET(error_flistp,
				PIN_FLD_DESCR,
				(void *)"Ilegal data in zonemap.", ebufp);
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_BAD_VALUE, 
				PIN_FLD_ZONEMAP_DATA_DATA, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Zonemap data contains illegal character.",
				ebufp);
			return;
		}
		/*
		 * Find the character in the existing offsets or add a new one. We then proceed
		 * with inserting (the offset is set by the call to fm_zonemap_pol_insert_path_to_trie
		 * (see pass-by-reference of pChildIndex->offset below).
		 */ 
		pChildIndex = (child_index_t *)(nodep + 1);
		for (index = 0; index < nodep->nChildren; index++, pChildIndex++) {
			if (pChildIndex->character == (u_int32)current) {
				break;
			}
                }
		/* 
		 * If not found, assign a new child - pChildIndex should now be pointing to the first unused 'slot'.
		 * The trie offset will be updated by the recursive call to 'fm_zonemap_pol_insert_path_to_trie'.
		 */
		if (index >= nodep->nChildren) {
			nodep->nChildren += 1;
		        pChildIndex->character = (u_int32)current;
		        pChildIndex->offset = (u_int32)0;
		}
		/* Recurse to next level ... */
		fm_zonemap_pol_insert_path_to_trie(path_a, ++str, &pChildIndex->offset,
			tablep, row_id, table_size, depth + 1, error_flistp, ebufp);
	} else {
		/* End of the string, initialize data node */	
		nodep->isTerminal = PIN_BOOLEAN_TRUE;
		fm_zonemap_pol_insert_path(path_a, nodep, error_flistp, ebufp);
	}
}

/*******************************************************************
 * fm_zonemap_pol_insert_path
 * insert the ancestors to the leaf data of the trie node
 *******************************************************************/
static void 
fm_zonemap_pol_insert_path(
	uintptr_t		path_a[],
	TrieNode_t		*nodep,
	pin_flist_t		*error_flistp,
	pin_errbuf_t		*ebufp) 
{
	LeafData_t		*leafp = NULL;
	int32			cur = 0;
	telephony_offset_t	*ances_listp = NULL;
	int32			result = PIN_RESULT_PASS;
	pin_zonemap_err_t	res_code = PIN_ZONEMAP_ERR_NONE;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return; 
	}
	PIN_ERR_CLEAR_ERR(ebufp);
	
	if (nodep->leafdata_p != 0) {
		/* duplicate happens! Quit immediately!!! */
		result = PIN_RESULT_FAIL;
		res_code = PIN_ZONEMAP_ERR_DUPLICATE;
		PIN_FLIST_FLD_SET(error_flistp,
			PIN_FLD_RESULT,
			(void *)&result, ebufp);
		PIN_FLIST_FLD_SET(error_flistp,
			PIN_FLD_RESULT_FORMAT,
			(void *)&res_code, ebufp);
		PIN_FLIST_FLD_SET(error_flistp,
			PIN_FLD_DESCR,
			(void *)"Duplicate data in zonemap.", ebufp);
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE, 
			PIN_FLD_ZONEMAP_DATA_DATA, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"Zonemap data contains duplicates.", ebufp);
		return;
	}

	leafp = (LeafData_t *)data_vp;
	data_vp += sizeof(LeafData_t);

	leafp->ancestors_p = (telephony_offset_t)data_vp;
	ances_listp = (telephony_offset_t *)leafp->ancestors_p;
	while (path_a[cur] != -1) {
		*(ances_listp++) = path_a[cur++];
		data_vp += sizeof(uintptr_t);
	}
	*((uintptr_t *)data_vp) = -1;
	data_vp += sizeof(uintptr_t);
	MEM_ALIGN(data_vp, sizeof(uintptr_t));
	
	nodep->leafdata_p = (telephony_offset_t)leafp;
}

/***********************************************************************
 * fm_zonemap_pol_get_tmp_table_size
 * returns the size of the temporary table that needs to be created
 ***********************************************************************/
static int32	
fm_zonemap_pol_get_tmp_table_size(
	pin_flist_t		*zone_flistp,
	pin_errbuf_t		*ebufp) 
{
	pin_flist_t		*subzone_flistp = NULL;
	int32			count = 0;
	int32			subzone_elem = 0;
	pin_cookie_t		z_cookie = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return 0;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	while((subzone_flistp = PIN_FLIST_ELEM_GET_NEXT(zone_flistp,
			PIN_FLD_ZONEMAP_ZONES, &subzone_elem, 1, &z_cookie,
			ebufp)) != NULL) {
		count += fm_zonemap_pol_get_tmp_table_size(subzone_flistp,
			ebufp);
	}
	
	count += PIN_FLIST_ELEM_COUNT(zone_flistp,
			PIN_FLD_ZONEMAP_DATA_SET, ebufp);

	return count;
}
/***********************************************************************
 * fm_zonemap_pol_create_tmp_tables
 * creates a temporary table to store phone numbers/IP address
 * and their ancestors for easier construction of the trie
 ***********************************************************************/
static void 
fm_zonemap_pol_create_tmp_table(
	pin_flist_t		*zone_flistp,
	table_row_t		*tablep,
	uintptr_t		path_a[],
	int32			size,
	uintptr_t		*cur,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*subzone_flistp = NULL;
	pin_flist_t		*data_flistp = NULL;
	void			*vp = NULL;
	int32			data_elem = 0;
	int32			subzone_elem = 0;
	int32			elem = 0;
	pin_cookie_t		z_cookie = NULL;
	pin_cookie_t		d_cookie = NULL;
	tmp_table_row_data_t	*row_p = NULL;
	int32			i = 0;
	
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/************************************************************
	 * Traverse zones array, for each zone, do such:
	 *	while (zone still has data) {
	 *		put data in the temporary table;
	 *	};
	 *	if (zone has children) {
	 *		create temp table for all its children;
	 *	};
	 * Repeat until the zone array is empty.
	 ************************************************************/

	/* Is there data? */
	while ((data_flistp = PIN_FLIST_ELEM_GET_NEXT(zone_flistp, PIN_FLD_ZONEMAP_DATA_SET, &data_elem, 1, &d_cookie,
			ebufp)) != NULL) {
		vp = PIN_FLIST_FLD_GET(data_flistp, PIN_FLD_ZONEMAP_DATA_DATA, 1, ebufp);
		if (vp) {
			/* There is data, create a row in the table! */
			row_p = (tmp_table_row_data_t *)malloc(sizeof(tmp_table_row_data_t));
			if (NULL == row_p) {
				pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
					PIN_ERR_NO_MEM, PIN_FLD_ZONEMAP_DATA_SET, 0, 0);
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_zonemap_pol_create_tmp_table: failed to allocate memory", ebufp);
				return;
			}

			row_p->content_p = strdup((char *)vp);
			row_p->ancestors_a = (uintptr_t *)malloc(sizeof(uintptr_t)*(size+1));
			if (row_p->ancestors_a == NULL) {
				pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
					PIN_ERR_NO_MEM, PIN_FLD_ZONEMAP_DATA_SET, 0, 0);
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_zonemap_pol_create_tmp_table: failed to allocate memory", ebufp);
                                return;
                        }

			i = 0;
			while (i < size) {
				row_p->ancestors_a[i] = path_a[i];
				i++;
			}
			row_p->ancestors_a[i] = -1;
		}
		*(tablep + *cur) = row_p;
		(*cur)++;
	}
	elem = 0;
	while((subzone_flistp = PIN_FLIST_ELEM_GET_NEXT(zone_flistp, PIN_FLD_ZONEMAP_ZONES, &subzone_elem, 1, &z_cookie,
			ebufp)) != NULL) {
		/* Append this element id to the end of the current path */
		path_a[size] = elem;
		fm_zonemap_pol_create_tmp_table(subzone_flistp, tablep, path_a, size+1, cur, ebufp);
		elem++;
	}
}

/*******************************************************************
 * fm_zonemap_pol_get_num_child_nodes
 *
 * This function determines, for a given location in a leaf string, how may child nodes
 * this nodes will  have. This works by finding all strings in the ordered map that have the
 * common sub-string and a unique character following that sub-string.
 *
 * For example, imagine the following ordered set of leaf strings:
 *
 *     00254   (row = 0, pos = 2 (pointing to character '2')
 *     00267
 *     00269
 *     00274
 *     0033
 *     0039
 *     0049
 *
 * If we are at position 002 on the first row then we simply compare all the strings that have
 * '002' as a matching prefix, and then check each subsequent character in turn: '5', '6', '6',
 * '7', before failing to match substring '003' from row '0033'. So we determine we will have
 * 3 child nodes ('5', '6', '7');
 *
 * @param tablep  The table of rows
 * @param row     The row we're currently looking at
 * @param size    The table size
 * @param len     The prefix length to match
 * @param source  The source message (for debug)
 *
 * @return        The number of children (0 - for terminals, > 0 for others)
 *******************************************************************/
static int32
fm_zonemap_pol_get_num_child_nodes (
	table_row_t 	*tablep,
	int32 		row,
	int32 		size,
	int32 		len,
	const char 	*source)
{
	table_row_t 	*rowp = tablep + row;
	char 		*this = (*rowp)->content_p;
	int32 		children = 0;
	int 		i = 0;
	char 		last_child_char = '\0';
	char 		this_child_char;

	for (i = row; i < size; i++, rowp++) {
		if (strncmp((*rowp)->content_p, this, len) == 0) {
			this_child_char = (*rowp)->content_p[len];
			if (this_child_char != last_child_char) {
				last_child_char = this_child_char;
				++children;
			}
		} else {
			break;
		}
        }
	return children;
}

/*******************************************************************
 * fm_zonemap_pol_get_num_of_nodes
 * Get the total number of trie nodes and the total number of child_nodes
 *******************************************************************/	
static int32
fm_zonemap_pol_get_num_of_nodes(
	table_row_t		*tablep,
	int32			table_size,
	int32			*node_data_size,
	pin_errbuf_t		*ebufp)
{
	table_row_t		*cur_rowp = NULL;
	char			*content = NULL;
	char			*previous = NULL;
	char			*cur = NULL;
	uintptr_t		*path_a = NULL;
	int32			total = 1;
	int32			sz = 0;
	int32			count;
	int32			i_row = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return 0;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/* 
	 * If the table is null, number of nodes is zero.
	 */
	if (tablep == NULL) {
		return 0;
	}

	/**********************************************************
	 * qsort the table based on the phone number/IP address
	 **********************************************************/
	qsort((void *)tablep, (size_t)table_size, sizeof(table_row_t), fm_util_zonemap_compare_row);
	
	cur_rowp = tablep;

	for (i_row = 0; i_row < table_size; i_row++) {
		previous = content;
		content = (char *)((*cur_rowp)->content_p);
		cur = content;
		/* look for the common substring, then start counting */
		while (cur != NULL &&
			*cur != '\0' &&
			previous != NULL &&
			*previous != '\0') {
			if (*cur == *previous) {
				cur++;
				previous++;
			} else {
				break;
			}
		}
		while (cur != NULL && *cur != '\0') {
			cur++;
			total++;
		}

		path_a = (*cur_rowp)->ancestors_a;
		count = 0;
		while (path_a[count++] != -1);
		
		sz += (int)sizeof(LeafData_t);
		sz += count * (int)sizeof(intptr_t);
		ROUNDUP(sz, (int)sizeof(intptr_t));

		cur_rowp++;
	}
	*node_data_size = sz;

	return total;
}
		
static int32
fm_util_ipt_canon_phone_number(
	char			*pszIn,
	char			*pszOut)
{

	char			*pszCurChar;

	/* For each character in the input... */
	for ( ; *pszIn != '\0'; pszIn++) {

		/* Find current input char in valid charset */
		for (pszCurChar = EXTENDED_VALID_CHARS; 
				*pszCurChar != '\0'; pszCurChar++) {
			if (*pszCurChar == *pszIn) {
				/* Found it, copy to output */
				*pszOut = *pszIn;
				pszOut++;
				break;
			}
		}
	}

	/* Add terminating NULL */
	*pszOut = '\0';

	/* Return success! */
	return 1;
}

static int32
fm_util_zonemap_compare_row(
	const void		*arg1_p,
	const void		*arg2_p) 
{
	return strcmp((*(table_row_t *)arg1_p)->content_p, (*(table_row_t *)arg2_p)->content_p);
}
