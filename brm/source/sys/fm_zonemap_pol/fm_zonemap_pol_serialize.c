/* continuus file information --- %full_filespec: fm_zonemap_pol_serialize.c~9:csrc:1 % */
/**********************************************************************
*
*	C Source:		fm_zonemap_pol_serialize.c
*	Instance:		1
*	Description:	
*	%created_by:	gmartin %
*	%date_created:	Fri Jul 21 17:07:32 2000 %
*
*      Copyright (c) 2000 - 2023  Oracle.
*
*      This material is the confidential property of Oracle Corporation
*      or its subsidiaries or licensors and may be used, reproduced, stored
*      or transmitted only in accordance with a valid Oracle license or
*      sublicense agreement.
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#)$Id: fm_zonemap_pol_serialize.c /cgbubrm_7.5.0.portalbase/1 2023/07/18 03:57:44 visheora Exp $";
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
#include "fm_zonemap_pol.h"

/******************************************************************
 * cursor in the zone data block
 ******************************************************************/
extern char	*	zone_data_vp;
/******************************************************************
 * cursor in the zone nodes block
 ******************************************************************/
extern char *	zone_vp;

static void 
fm_zonemap_pol_serialize_insert_node(
	pin_flist_t		*zone_flistp,
	int32			elem,
	pin_errbuf_t		*ebufp);

/*******************************************************************
 * Function bodies.
 *******************************************************************/

/******************************************************************
 * fm_zonemap_pol_serialize_zone_tree
 * saves the zone trees in a flat data block for 
 * easier retrieval based on element id.
 * The algorithem is a breadth-first iteration.
 ******************************************************************/
/******************************************************************
 * Structure of the data block is as such:
 *
 *	---------------------------------------------------------
 *	| data | elem_id | &children | &sibling | ....
 *	---------------------------------------------------------
 ******************************************************************/

void
fm_zonemap_pol_serialize_zone_tree(
	pin_flist_t		*zones_flistp,
	tree_node_data_t	*nodep,
	int32			elem,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*subzone_flistp = NULL;
	pin_cookie_t		cookie = NULL;
	int32			size = 0;
	int32			count = 0;
	tree_node_data_t	*childp;
	int32			elem_num;
	
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);	

	if (nodep->data_addr == 0) {
		fm_zonemap_pol_serialize_insert_node(zones_flistp, 
			elem, ebufp);
	}

	/* Process each subzone */
	elem = 0;
	elem_num = 0;
	while ((subzone_flistp = PIN_FLIST_ELEM_GET_NEXT(zones_flistp,
			PIN_FLD_ZONEMAP_ZONES, &elem, 1, &cookie, ebufp)) !=
			NULL) {
		if (nodep->firstchild_addr == 0) {
			nodep->firstchild_addr = (telephony_offset_t)(zone_vp + 
				sizeof(tree_node_data_t));
		}
		zone_vp = (char *)(nodep->firstchild_addr + 
			elem_num*sizeof(tree_node_data_t));
		fm_zonemap_pol_serialize_insert_node(subzone_flistp, 
			elem_num, ebufp);
		size ++;
		elem_num++;
	}

	childp = (tree_node_data_t *)(nodep->firstchild_addr);

	count = 0;
	elem = 0;
	elem_num = 0;
	cookie = NULL;

	while (count < size) {
		/* Fill in children of the current node */
		childp->parent_addr = (telephony_offset_t)nodep;
		subzone_flistp = PIN_FLIST_ELEM_GET_NEXT(zones_flistp,
			PIN_FLD_ZONEMAP_ZONES, &elem, 1, &cookie, ebufp);
		fm_zonemap_pol_serialize_zone_tree(subzone_flistp,
			childp, elem_num, ebufp);
		/* Move to the next child! */
		childp++;
		count++;
		elem_num++;
	}

}


static void 
fm_zonemap_pol_serialize_insert_node(
	pin_flist_t		*zone_flistp,
	int32			elem,
	pin_errbuf_t		*ebufp)
{
	char			*name;
	char			*bufp = (char *)NULL;
	tree_node_data_t	*nodep = NULL;
	telephony_offset_t	node_datap;
	
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);	

	nodep = (tree_node_data_t *)zone_vp;
	node_datap = (telephony_offset_t)zone_data_vp;

	nodep->data_addr = node_datap;
	nodep->elem_id = elem;
	nodep->firstchild_addr = 0;
		
	bufp = zone_data_vp;
	
	name = (char *)PIN_FLIST_FLD_GET(zone_flistp, PIN_FLD_NAME, 1, ebufp);
	if (name != NULL) {
		strcpy(bufp, name);
		bufp += strlen(name);
	}
	
	*bufp++ = '\0';
				
	zone_data_vp = bufp;
	MEM_ALIGN(zone_data_vp, sizeof(uintptr_t));
	
}
	
/**********************************************************************
 * Function to get size of the serialized zonemap.
 * Return the number of nodes in the serialized zonemap, 
 * and set the zone_data_size to be the size of serialized
 * zonemap data.
 **********************************************************************/
extern int32
fm_zonemap_pol_serialize_get_size(
	pin_flist_t		*zone_flistp,
	int32			*zone_data_sizep,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*subzone_flistp;
	int32			elem = 0;
	pin_cookie_t		cookie = NULL;
	char 			*name = NULL;
	int32			count = 1;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return 0;
	}
	PIN_ERR_CLEAR_ERR(ebufp);	
	
	count = 1;
	name = (char *)PIN_FLIST_FLD_GET(zone_flistp,
				PIN_FLD_NAME, 1, ebufp);
	if (name != NULL) {
		int name_len = pin_convert_sizet_to_int(strlen(name) + 1, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				"fm_zonemap_pol_serialize_get_size  name len is truncated", ebufp);
			return 0;
		}

		*zone_data_sizep += name_len;
	}
	else {
		/* There is a null terminator. */
		*zone_data_sizep += 1;
	}
	ROUNDUP(*zone_data_sizep, (int)sizeof(intptr_t));

	while ((subzone_flistp = PIN_FLIST_ELEM_GET_NEXT(zone_flistp,
			PIN_FLD_ZONEMAP_ZONES, &elem, 1, &cookie, ebufp)) !=
			NULL) {
		count += fm_zonemap_pol_serialize_get_size(subzone_flistp,
					zone_data_sizep, ebufp);
	}

	return count;

}

