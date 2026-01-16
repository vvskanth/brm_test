/*
 *      @(#) % %
 *    
* Copyright (c) 1996, 2023, Oracle and/or its affiliates. 
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _DM_SM_H
#define _DM_SM_H

#include "dm_credentials.h"
#include "dm_malloc.h"

/*
 * defines for the DM Back End <-> Storage Manager interface.
 */
#define DM_SM_GLOBAL_V1	0x01020304

struct dm_sm_global {
	u_int32		reserved;	/* do NOT use */
	u_int32		vers;		/* version: 0x01020304 */
	size_t		heap[8];	/* opaque global heap descriptor */
	u_int32		v1;		/* word values to use */
	u_int32		v2;
	void		*p1;		/* pointers to use */
	void		*p2;
	void		*p3;
	void		*p4;
};

/*
 * a structure for passing in config info to the SM during dm_if_init_process().
 */
struct dm_sm_config {
	u_int32	be_id;		/* id # of this BE, starting from 0 */
	u_int32	sm_shm_size;	/* 0 = no shm for SM, otherwise */
				/* the size of the shm allocated for SM */
	void	*sm_shm_base;	/* if sm_shm_size != 0, then base ... */
	u_int32	sm_conf_flags;	/* see below */
	struct dm_sm_global *sm_global_p;	/* ptr to global area */
};

#define DM_SM_CONFIG_INIT_SCHEMA	0x0001	/* (re)load schema from db */

/*
 * the structure that has the SM visible info for dm_if_process_op().
 */
struct dm_sm_info {
	/*
	 * there are two sections of interest to the underlying code:
	 *	the "public" part with the poid, who, pcp_flags; and
	 * a private (to the SM) area with two private things for SM use.
	 */
	poid_t	*poidp;		/* poid of the input flist */
	poid_t	*who;		/* id of sender */
	u_int32	trans_flags;		/* transaction flags */
	u_int32	pad;			/* reserved for now */
	u_int32	client_flags;		/* for client type, CM or TIMOS */
#ifndef WINDOWS
	int32 mem_used;
#endif
		/* private to SM */
	int	pvti;			/* int for private use */
	caddr_t pvtp;			/* ptr to private area */
	dm_credentials_t *credp;	/* ptr to DM credentials */
};

#define DM_SM_TRANS_RW		0x01	/* trans is RW (else RO) */
#define DM_SM_TRANS_LOCK	0x02	/* lock the poid for this trans. */
#define DM_SM_TRANS_DONE	0x04	/* set by SM to tell DM trans is done */

/*
 * some functions to reset the current heap
 */
#if defined(PURIFY)
static void* dm_malloc_get_current_heap() { return 0; }
static int dm_malloc_set_current_heap(struct dm_malloc_heap *hp) { return 0; }
static int dm_malloc_release_current_heap(struct dm_malloc_heap *hp) { return 0; }
static int dm_malloc_force_release_current_heap(struct dm_malloc_heap *hp,
        int old_pid){ return 0; }
static void dm_malloc_init(void *start, size_t hb_size, size_t big_size,
        int32 *errp){return;}
static void dm_malloc_used_free(int32 *usedp, int32 *freep,
        int64 *big_usedp, int64 *big_chunksp, int32 *heap_hwm,
        int32 *heap_max_used, int64 *big_hwm, int64 *big_max_used){ return;}
static int dm_malloc_get_heap(struct dm_malloc_heap *hp) { return 0; }
static void dm_malloc_put_heap(struct dm_malloc_heap *hp) { return; }
#else /* PURIFY */
#ifndef INTERNAL_DM_MALLOC
extern void *dm_malloc_get_current_heap();
extern int dm_malloc_set_current_heap(struct dm_malloc_heap *hp);
extern int dm_malloc_release_current_heap(struct dm_malloc_heap *hp);
extern int dm_malloc_force_release_current_heap(struct dm_malloc_heap *,
        int old_pid);
extern void dm_malloc_init(void *start, size_t hb_size, size_t big_size,
        int32 *errp);
extern void dm_malloc_used_free(int32 *usedp, int32 *freep,
        int64 *big_usedp, int64 *big_chunksp, int32 *heap_hwm,
        int32 *heap_max_used, int64 *big_hwm, int64 *big_max_used);
extern int dm_malloc_get_heap(struct dm_malloc_heap *);
extern void dm_malloc_put_heap(struct dm_malloc_heap *);
extern size_t dm_malloc_get_used_mem();
#endif /*INTERNAL_DM_MALLOC*/
#endif /* PURIFY */


#endif /*_DM_SM_H*/
