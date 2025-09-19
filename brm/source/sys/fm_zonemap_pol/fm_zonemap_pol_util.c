/* continuus file information --- %full_filespec: fm_zonemap_pol_util.c~6:csrc:2 % */
/*************************************************************************
 *  @(#) %full_filespec: fm_zonemap_pol_util.c~6:csrc:2 %
 *
* Copyright (c) 1999, 2024, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted 
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *************************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: fm_zonemap_pol_util.c /cgbubrm_7.5.0.portalbase/1 2023/06/14 03:57:44 sreejs Exp $";
#endif

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

#define FILE_LOGNAME "fm_zonemap_pol_util.c(4)"


/*****************************************************************************
 * Routine to retrieve zone lineage in concatenated string format
 * delimited by delimiters based on the ancestor array.
 * The storeage is handled within the function therefore it is
 * also repsponsible for releasing the memory.
 *****************************************************************************/
void
fm_zonemap_pol_get_lineage_from_ancestors(
	telephony_offset_t	bufp, 
	telephony_offset_t	ancestor_p,
	char			**lineage_pp,
	pin_errbuf_t		*ebufp)
{
	telephony_offset_t	zone_start = 0;
	tree_node_data_t	*zonetree_p = NULL;
	tree_node_data_t	*curzone_p = NULL;
	tree_node_data_t	*curparent_p = NULL;
	char			*zonename_p = NULL;
	char			*cur_p = NULL;
	uintptr_t		elem = -1;
	uintptr_t		*path_a = NULL;

	*lineage_pp = (char *)malloc(PIN_MAX_PATH);
	if (!(*lineage_pp)) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION, PIN_ERR_NO_MEM, 0,0,0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, "Memory allocation error", ebufp);
		return;
	}
	pin_memset(*lineage_pp, PIN_MAX_PATH, 0, PIN_MAX_PATH);
	cur_p = *lineage_pp;

	zone_start = ((Blob_t *)bufp)->zone_start;
	zonetree_p = (tree_node_data_t *)ADDR(bufp, zone_start);
	curzone_p = zonetree_p;
	
	path_a = (uintptr_t *)ADDR(bufp, ancestor_p);
	while (path_a != NULL &&
		(elem = *path_a) != -1) {

		/* Start walking the ancestore array */
		/* For each element id, figure out the zone name */
		curparent_p = curzone_p;
		curzone_p = (tree_node_data_t *)ADDR(bufp, (curparent_p->firstchild_addr));
		curzone_p += elem;
		/******************************************************
		 * Due to the data structure, the current zone node 
		 * might NOT be the one we want!!! Therefore, 
		 * check its parent.
		 ******************************************************/
		if ((telephony_offset_t)curparent_p != ADDR(bufp, curzone_p->parent_addr)) {
			/* this element id does not exist!!! */
			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_APPLICATION, 
				PIN_ERR_BAD_VALUE, 
				PIN_FLD_ZONEMAP_ZONES, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				"fm_zonemap_pol_get_lineage_from_ancestor_id: Ancestor does not exist!", ebufp);
			break;
		}

		zonename_p = (char *)ADDR(bufp, curzone_p->data_addr);

		if (zonename_p != NULL) {
			/* Copy the zonename to the current place in */
			/* plineage_p */
			pin_strlcat(cur_p, zonename_p, PIN_MAX_PATH);

			/******************************************************
			 * Add the predefined delimiter. It has to be
			 * the same delimiter used in rating engine.
		 	 ******************************************************/
			strncat(cur_p, PIN_LINEAGE_DELIMITER, (PIN_MAX_PATH - strlen(cur_p) - 1));
			pin_strlcat(cur_p, PIN_LINEAGE_DELIMITER, PIN_MAX_PATH);
		}
		else {
			/* Something is wrong, there is a null zone name */
			/* Exit with error code */
			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_APPLICATION, 
				PIN_ERR_BAD_VALUE, 
				PIN_FLD_ZONEMAP_ZONES, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				"fm_act_pol_telephony_get_matrix: empty zone name", ebufp);
			break;
		}

		/* Move to next one in the path */
		path_a++;		 
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		free(*lineage_pp);
		*lineage_pp = NULL;
		return;	
	}
}
/*
 * fm_zonemap_pol_debug_zonemap - runs though a zonemap and prints out the structure on
 * the provided output stream.
 *
 * @param fp      The file pointer for the desired output stream.
 * @param blobp   The base of the binary object.
 * @param msg     A message to act as a header to the debug (mandatory)
 *
 */
static void
__fm_zonemap_pol_debug_zonemap(FILE *fp, Blob_t *blobp, TrieNode_t *triep, int32 level, int32 *node_id, char *str)
{
char full_str[512];
child_index_t *cip = NULL;
int index = 0;

	fprintf(fp, "%8d  %6d  %6hd  %6s  %12p  %12u  %.64s\n",
		*node_id, level, triep->nChildren, triep->isTerminal ? "YES" : "NO",
		triep, (u_int32)((char *)triep - (char *)blobp), str);
	(*node_id)++;
        cip = (child_index_t *)(triep + 1);
	for (index = 0; index < triep->nChildren; index++, cip++) {
		pin_snprintf(full_str, sizeof(full_str), "%s%c", str, (char)cip->character);
		__fm_zonemap_pol_debug_zonemap(fp, blobp, (TrieNode_t *)cip->offset, level + 1, node_id, full_str);
	}
}

/*
 * Simple debug function to validate the zonemap contents.
 *
 * @param fp      The file stream to output to.
 * @param blobp   The blobp root pointer.
 * @param msg     Identification message for this log output.
 */
void
fm_zonemap_pol_debug_zonemap (FILE *fp, Blob_t *blobp, const char *msg)
{
	int32 node_id = 0;

	fprintf(fp, "ZM[%s]:\n\n", msg);
	if (!blobp) {
		fprintf(fp, "Empty BLOB\n");
		return;
	}
        fprintf(fp, "Version   : %x\n", blobp->version);
        fprintf(fp, "Size      : %d\n", blobp->blob_size);
        fprintf(fp, "Trie addr : %lu \n", blobp->trie_start);
        fprintf(fp, "\n");
	fprintf(fp, "%8s  %6s  %6s  %6s  %12s  %12s  %-64.64s\n",
		"NodeId", "Lvl", "#Chld", "Term?", "Address", "Offset", "String");

	__fm_zonemap_pol_debug_zonemap(fp, blobp, (TrieNode_t *)blobp->trie_start, 0, &node_id, "");
}
