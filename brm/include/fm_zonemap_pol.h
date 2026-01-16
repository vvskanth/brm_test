/* continuus file information --- %full_filespec: fm_zonemap_pol.h~14:incl:1 % */
/**********************************************************************
*
*	C Header:		fm_zonemap_pol.h
*	Instance:		1
*	Description:	
*	%created_by:	jshi %
*	%date_created:	Mon Sep 17 11:16:33 2001 %
*
* Copyright (c) 2000, 2023, Oracle and/or its affiliates. 
*
*      This material is the confidential property of Oracle Corporation
*      or its subsidiaries or licensors and may be used, reproduced, stored
*      or transmitted only in accordance with a valid Oracle license or
*      sublicense agreement.
*
**********************************************************************/
#ifndef _1_fm_zonemap_pol_h_H
#define _1_fm_zonemap_pol_h_H

#ifndef lint
static char    *_1_fm_zonemap_pol_h = "@(#)$Id: fm_zonemap_pol.h /cgbubrm_7.5.0.portalbase/1 2015/11/27 03:58:33 nishahan Exp $";
#endif

#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_MAX_PATH	260

/* macro for alignment */
#define	MEM_ALIGN(p, sz) \
	((p) = (char *)(((uintptr_t)(p) + (sz) -1 ) & ~((sz) - 1))) 

#define ROUNDUP(s, mod) \
	((s) = (s + mod - 1) & ~(mod - 1))

#define ADDR(base, offset) \
	((uintptr_t)((uintptr_t)base + (uintptr_t)offset))

#define OFFSET(base, p) \
	(p = ((uintptr_t)p > (uintptr_t)base)? (uintptr_t)((uintptr_t)p - (uintptr_t)base) : 0)
 
#ifndef rs6000
#define NTOHL(l) \
	(l = (u_int32)ntohl((u_int32)l))

#define HTONL(l) \
	(l = (u_int32)htonl((u_int32)l))
#endif

#define NTOHLL(x) ((1 == ntohl(1)) ? (x) : \
	(((u_int64)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((u_int32)((x) >> 32)))

#define HTONLL(x) ((1 == htonl(1)) ? (x) : \
	(((u_int64)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((u_int32)((x) >> 32)))


typedef uintptr_t telephony_offset_t;

/*
 * The ZONEMAP_CURRENT_VERSION changes whenever the format of the
 * zonemap or any of its structures changes.  This must be expressed
 * as a hexadecimal number!
 */
#define ZONEMAP_CURRENT_VERSION   0x4
/*
 * The character set supported by the zonemap 
 * for Portal instance that requires 
 * more sophisticated data
 */
#define EXTENDED_CHARSETSIZE		83
static char	*EXTENDED_VALID_CHARS =  "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ;/?:@&=+$,-_.!~*'()%#"; 
				

#define EXTENDED_CHARSET_INDEX(c) \
	(int32)(strchr(EXTENDED_VALID_CHARS, c) - \
	EXTENDED_VALID_CHARS)

/* data structures */
/* structure of a leaf data node */
typedef struct TAG_LeafData_t {
	/* offset of an array that stores all its ancestors */
	telephony_offset_t	ancestors_p; 
} LeafData_t;

/* trie node */
typedef struct TAG_TrieNode_t {
	telephony_offset_t	children[EXTENDED_CHARSETSIZE];
	int32			isTerminal;
	telephony_offset_t	leafdata_p;
} TrieNode_t;

typedef struct TAG_Blob_t {
	int32			version;
	int32			blob_size;		/* size of the entire blob */
	int32			node_size;		/* size of the trie nodes */
	int32			data_size;		/* size of the trie node data */
	int32			zone_size;		/* size of the zone node */
	int32			zone_data_size;		/* size of the zone node data */
	telephony_offset_t	trie_start;		/* start of the trie nodes */
	telephony_offset_t	data_start;		/* start of the trie node data */
	telephony_offset_t	zone_start;		/* start of the zone node */
	telephony_offset_t	zone_data_start;	/* start of the zone node data */
} Blob_t;

/* data structure to save the zone and it's children */
typedef struct TAG_TreeNodeData_t {
	telephony_offset_t	data_addr;
	int32			elem_id;
	telephony_offset_t	firstchild_addr;
	telephony_offset_t	parent_addr;
} tree_node_data_t;

/********************************************************************
 * Functions from fm_zonemap_pol_prep_zonemap.c
 ********************************************************************/
extern void 
fm_zonemap_pol_construct_blob(
	pin_flist_t		*zonemap_flistp,
	pin_flist_t		*return_flistp,
	pin_errbuf_t		*ebufp);

/*******************************************************************
 * Functions from fm_zonemap_pol_serialize.c
 *******************************************************************/
extern void
fm_zonemap_pol_serialize_zone_tree(
	pin_flist_t		*zones_flistp,
	tree_node_data_t	*parentp,
	int32			elem,
	pin_errbuf_t		*ebufp);

extern int32
fm_zonemap_pol_serialize_get_size(
	pin_flist_t		*zone_flistp,
	int32			*zone_data_size,
	pin_errbuf_t		*ebufp);

/*******************************************************************
 * Functions from fm_zonemap_pol_zonemap_network.c
 *******************************************************************/
extern void 
fm_zonemap_pol_pre_process_blob(			
	Blob_t			*blobp);

extern void
fm_zonemap_pol_post_process_blob(
	Blob_t			*blobp);

/*******************************************************************
 * Functions from fm_zonemap_pol_util.c
 *******************************************************************/
extern void
fm_zonemap_pol_get_lineage_from_ancestors(
	telephony_offset_t	bufp, 
	telephony_offset_t	ancestor_p,
	char			**plineage_p,
	pin_errbuf_t		*ebufp);


#ifdef __cplusplus
}
#endif

#endif
