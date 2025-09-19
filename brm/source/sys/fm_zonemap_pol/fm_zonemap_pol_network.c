/* continuus file information --- %full_filespec: fm_zonemap_pol_network.c~10:csrc:1 % */
/**********************************************************************
*
*	C Source:		fm_zonemap_pol_network.c
*	Instance:		1
*	Description:	
*	%created_by:	gmartin %
*	%date_created:	Fri Jul 21 17:07:27 2000 %
*
* Copyright (c) 2000, 2023, 2024 Oracle and/or its affiliates. 
*
*      This material is the confidential property of Oracle Corporation
*      or its subsidiaries or licensors and may be used, reproduced, stored
*      or transmitted only in accordance with a valid Oracle license or
*      sublicense agreement.
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#)$Id: fm_zonemap_pol_network.c /cgbubrm_7.5.0.portalbase/1 2023/07/18 03:57:47 visheora Exp $";
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

#ifdef _DEBUG
#define IPT_DEBUG
#endif

extern telephony_offset_t		blob_base_addr;

/*******************************************************************
 * Function prototypes
 *******************************************************************/
void 
fm_zonemap_pol_pre_process_blob(			
	Blob_t			*blobp);

void
fm_zonemap_pol_post_process_blob(
	Blob_t			*blobp);


/*******************************************************************
 * fm_zonemap_pol_pre_process_blob
 * change the network byte order to host byte order
 *******************************************************************/
void 
fm_zonemap_pol_pre_process_blob(			
	Blob_t *blobp)
{
	TrieNode_t		*trie_vp = (TrieNode_t *)NULL;
	LeafData_t		*data_vp = (LeafData_t *)NULL;
	tree_node_data_t	*zone_vp = (tree_node_data_t *)NULL;
	char			*vp = NULL;
	uintptr_t		*pathp = NULL;
	int32			index = 0;
	u_int16			nChildren = 0;
	child_index_t           *pChildIndex = NULL;
	telephony_offset_t	end = 0;
	char			msg[512];
	
	NTOHL(blobp->blob_size);
	NTOHL(blobp->node_size);
	NTOHL(blobp->data_size);
	NTOHL(blobp->zone_size);
	NTOHL(blobp->zone_data_size);
	NTOHL(blobp->version);

	if (blobp->version != ZONEMAP_CURRENT_VERSION) {
		pin_snprintf(msg,sizeof(msg), "fm_zonemap_pol_pre_process_blob: "
			"Wrong version of Zonemap.  Found %i, "
			"expecting %i. Ignoring zonemap. Use Configuration "
			"Center IPT to fix this problem.", blobp->version,
			ZONEMAP_CURRENT_VERSION);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
		return;
	}
	
	NTOHLL(blobp->trie_start);
	NTOHLL(blobp->data_start);
	NTOHLL(blobp->zone_start);
	NTOHLL(blobp->zone_data_start);
			
	trie_vp = (TrieNode_t *)ADDR(blobp, blobp->trie_start);
	data_vp = (LeafData_t *)ADDR(blobp, blobp->data_start);
	zone_vp = (tree_node_data_t *)ADDR(blobp, blobp->zone_start);
	vp = (char *)ADDR(blobp, blobp->zone_data_start);

	while ((telephony_offset_t)trie_vp < (telephony_offset_t)data_vp) {
		NTOHS(trie_vp->nChildren);
		NTOHS(trie_vp->isTerminal);
		NTOHLL(trie_vp->leafdata_p);
		nChildren = trie_vp->nChildren;
		trie_vp++;
		pChildIndex = (child_index_t *)trie_vp;
		for (index = 0; index < nChildren; index++, pChildIndex++) {
			NTOHL(pChildIndex->character);
			NTOHL(pChildIndex->offset);
			trie_vp = (TrieNode_t *)((char *)trie_vp + sizeof(child_index_t));
		}
	}


	/* The pointers should be equal.  If not, log an error */
	if ((telephony_offset_t)trie_vp != (telephony_offset_t)data_vp) {
		pin_snprintf(msg,sizeof(msg), "fm_zonemap_pol_pre_process_blob: "
			"Bad zonemap data. TRIE ptr (%08x) is not "
			"equal to DATA ptr (%08x). Ignoring zonemap.", 
			(telephony_offset_t)trie_vp, (telephony_offset_t)data_vp);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
		return;
	}
	
	while ((telephony_offset_t)data_vp < (telephony_offset_t)zone_vp) {
		NTOHLL(data_vp->ancestors_p);
		pathp = (uintptr_t *)ADDR(blobp, data_vp->ancestors_p);
		while (*pathp != -1) {
			NTOHLL(*pathp);
			pathp++;
		}
		/* convert the last element -1 */
		NTOHLL(*pathp);
		pathp++;
		data_vp = (LeafData_t *)pathp;
	}

	/* The pointers should be equal.  If not, log an error */
	if ((telephony_offset_t)data_vp != (telephony_offset_t)zone_vp) {
		pin_snprintf(msg, sizeof(msg), "fm_zonemap_pol_pre_process_blob: "
			"Bad zonemap data. DATA ptr (%08x) is not "
			"equal to ZONE ptr (%08x). Ignoring zonemap.", 
			(telephony_offset_t)data_vp, (telephony_offset_t)zone_vp);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
		return;
	}

	/* convert the zone nodes */
	while ((telephony_offset_t)zone_vp < (telephony_offset_t)vp) {
		NTOHLL(zone_vp->data_addr);
		NTOHLL(zone_vp->firstchild_addr);
		NTOHLL(zone_vp->parent_addr);
		NTOHL(zone_vp->elem_id);
		zone_vp++;
	}

	/* The pointers should be equal.  If not, log an error */
	if ((telephony_offset_t)zone_vp != (telephony_offset_t)vp) {
		pin_snprintf(msg,sizeof(msg), "fm_zonemap_pol_pre_process_blob: "
			"Bad zonemap data. ZONE ptr (%08x) is not "
			"equal to VP ptr (%08x). Ignoring zonemap.", 
			(telephony_offset_t)zone_vp, (telephony_offset_t)vp);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
		return;
	}
	
	/* Don't need to convert zone node data since it is string data */
}

/*******************************************************************
 * fm_zonemap_pol_post_process_blob
 * change the actual address to offset
 * and change the byte order from host to network 
 *******************************************************************/
void
fm_zonemap_pol_post_process_blob(
	Blob_t			*blobp)
{
	TrieNode_t		*trie_vp = NULL;
	LeafData_t		*data_vp = NULL;
	tree_node_data_t	*zone_vp = NULL;
	uintptr_t		*pathp = NULL;
	char			*vp = NULL;
	int32			index = 0;
	char			msg[512];
	child_index_t		*pChildIndex = NULL;
	u_int16			nChildren = 0;

	trie_vp = (TrieNode_t *)(blobp->trie_start);
	data_vp = (LeafData_t *)(blobp->data_start);
	zone_vp = (tree_node_data_t *)(blobp->zone_start);
	
	while ((telephony_offset_t)trie_vp < (telephony_offset_t)data_vp) {
		/* convert the byte order one by one */
		OFFSET(blob_base_addr, trie_vp->leafdata_p);
		nChildren = trie_vp->nChildren;

		HTONLL(trie_vp->leafdata_p);
		HTONS(trie_vp->isTerminal);
		HTONS(trie_vp->nChildren);

		trie_vp++;
		/* walk the children list */
		pChildIndex = (child_index_t *)trie_vp;
		for (index = 0; index < nChildren; index++, pChildIndex++) {
			HTONL(pChildIndex->character);
			OFFSET(blob_base_addr, pChildIndex->offset);
			HTONL(pChildIndex->offset);
			trie_vp = (TrieNode_t *)((char *)trie_vp + sizeof(child_index_t));
		}	
	}

	/* The pointers should be equal.  If not, log an error */
	if ((telephony_offset_t)trie_vp != (telephony_offset_t)data_vp) {
		pin_snprintf(msg, sizeof(msg), "fm_zonemap_pol_post_process_blob: "
			"Bad zonemap data. TRIE ptr (%08x) is not "
			"equal to DATA ptr (%08x). Ignoring zonemap.", 
			(telephony_offset_t)trie_vp, (telephony_offset_t)data_vp);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
		return;
	}
	
	/* convert trie node data */
	while ((telephony_offset_t)data_vp < (telephony_offset_t)blobp->zone_start) {
		/* Get to first element of the ancestor array */
		pathp = (uintptr_t *)(data_vp->ancestors_p);

		while(*pathp != -1) {
			HTONLL(*pathp);
			pathp++;
		}
		/* convert the last element -1 */
		HTONLL(*pathp);
		pathp++;
		OFFSET(blob_base_addr, data_vp->ancestors_p);
		HTONLL(data_vp->ancestors_p);
		data_vp = (LeafData_t *)pathp;	
	}

	/* The pointers should be equal.  If not, log an error */
	if ((telephony_offset_t)data_vp != (telephony_offset_t)blobp->zone_start) {
		pin_snprintf(msg,sizeof(msg), "fm_zonemap_pol_post_process_blob: "
			"Bad zonemap data. DATA ptr (%08x) is not "
			"equal to ZONE ptr (%08x). Ignoring zonemap.", 
			(telephony_offset_t)data_vp, (telephony_offset_t)zone_vp);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
		return;
	}

	/* convert zone nodes */
	while ((telephony_offset_t)zone_vp < (telephony_offset_t)blobp->zone_data_start) {
		OFFSET(blob_base_addr, zone_vp->data_addr);
		HTONLL(zone_vp->data_addr);
		OFFSET(blob_base_addr, zone_vp->firstchild_addr);
		HTONLL(zone_vp->firstchild_addr);
		OFFSET(blob_base_addr, zone_vp->parent_addr);
		HTONLL(zone_vp->parent_addr);
		HTONL(zone_vp->elem_id);
		zone_vp++;
	}

	/* The pointers should be equal.  If not, log an error */
	if ((telephony_offset_t)zone_vp < (telephony_offset_t)blobp->zone_data_start) {
		pin_snprintf(msg,sizeof(msg), "fm_zonemap_pol_post_process_blob: "
			"Bad zonemap data. ZONE ptr (%08x) is not "
			"equal to ZONE DATA START ptr (%08x). Ignoring "
			"zonemap.", 
			(telephony_offset_t)zone_vp, blobp->zone_data_start);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
		return;
	}
	
	/* Don't need to convert zone node data since it is string data */
	HTONL(blobp->blob_size);
	OFFSET(blob_base_addr, blobp->trie_start);
	HTONLL(blobp->trie_start);
	HTONL(blobp->node_size);
	
	OFFSET(blob_base_addr, blobp->data_start);
	HTONLL(blobp->data_start);
	HTONL(blobp->data_size);

	OFFSET(blob_base_addr, blobp->zone_start);
	HTONLL(blobp->zone_start);
	HTONL(blobp->zone_size);

	OFFSET(blob_base_addr, blobp->zone_data_start);
	HTONLL(blobp->zone_data_start);
	HTONL(blobp->zone_data_size);

}

