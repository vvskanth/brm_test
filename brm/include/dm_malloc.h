/*
 *      @(#) % %
 *    
* Copyright (c) 1996, 2011, Oracle and/or its affiliates. All rights reserved. 
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _DM_MALLOC_H
#define _DM_MALLOC_H

#include "pin_type.h"

/*
 * big things are always kept in a linked list - so they
 * can be freed by put_heap().
 * flags is separate, so size is always clear...
 */
struct dm_malloc_big {
#ifdef PIN_64_BIT
        u_int64          size;
#else
        u_int32          size;
#endif /* PIN_64_BIT */
        u_int32         flags;  /* same as regular, just here */
        struct dm_malloc_big *fwd;
        struct dm_malloc_big *bak;
};


/*
 * XXX HARDCODE: keep this and opaque version in dm.h same size!
 */
struct dm_malloc_heap {
/*
XXX - are all heaps tied together?
*/
	u_int32		magic;		/* XXX ? */
	u_int32		used;		/* total size of heap blocks used */
	struct dm_malloc_hblk *hb_head;	/* list of heap blocks I've got */
					/* keep for dm_malloc_put_heap() */
		/* for optimum searching free list, bidirectional list */
	struct dm_malloc_free	*low;		/* smallest */
	u_int32			low_val;
	struct dm_malloc_free	*high;		/* biggest */
	u_int32			high_val;

	struct dm_malloc_big	*big_things;	/* big things */
				/* keep used big chunks here, for put_heap() */
};

/*
 * keep size constant 32 bytes
 */
struct dm_malloc_dump_hdr {
	int32	version;	/* MUST SET: for now: 1 */
	int32	cmd;		/* MUST SET: cmd to _dump_heap */
	void	*heap_p;	/* MUST SET: pointer to heap to dump */
	int32	time;		/* really: time_t */
	int32	pid;		/* for pid */
	int32	pad[3];		/* 3 long words/12 bytes to do whatever */
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * declare the public interfaces
 */
#ifndef PURIFY
/* init dm_malloc - call just once by head process */
extern void dm_malloc_init(void *start, size_t hb_size, size_t big_size,
	int32 *errp);

/* create/destroy a heap */
extern int dm_malloc_get_heap(struct dm_malloc_heap *);
extern void dm_malloc_put_heap(struct dm_malloc_heap *);

/* set/release current heap */
extern int dm_malloc_set_current_heap(struct dm_malloc_heap *);
extern int dm_malloc_release_current_heap(struct dm_malloc_heap *);

/* ONLY USE in main() to cleanup dead kids */
extern int dm_malloc_force_release_current_heap(struct dm_malloc_heap *,
	int old_pid);

/* return pointer to current heap (as void *) */
extern void *dm_malloc_get_current_heap();

/* N.B. actual memory allocation funcs (pin_malloc(), ...) in pin_type.h */

/* for reporting usage */
extern void dm_malloc_used_free(int32 *usedp, int32 *freep,
	int64 *big_usedp, int64 *big_chunksp, int32 *heap_hwm,
	int32 *heap_max_used, int64 *big_hwm, int64 *big_max_used);

/* Used by memleak detector coded in dm_front */
extern size_t dm_malloc_get_used_mem();
/* return the total free shm heap blocks*/
extern int32 dm_malloc_get_free_blocks();
/* return the total big memory used */
extern int64 dm_malloc_get_used_big();

#endif


/* allowable values for lock_cmd */
#define DM_MALLOC_DUMP_HEAP_JUST_LOCK	1
#define DM_MALLOC_DUMP_HEAP_JUST_UNLOCK	2
#define DM_MALLOC_DUMP_HEAP_LOCK	3
#define DM_MALLOC_DUMP_HEAP_SET_LOCK	4
#define DM_MALLOC_DUMP_HEAP_NO_LOCK	5
#define DM_MALLOC_DUMP_HEAP_CLR_LOCK	6

/* cmd bits to _dump_heap */
#define DM_MALLOC_DUMP_HEAP_HEAP	0x01	/* do regular heap stuff */
#define DM_MALLOC_DUMP_HEAP_BIG		0x02	/* do "big" stuff */

extern int dm_malloc_dump_heap(struct dm_malloc_dump_hdr *header_p,
	int32 lock_cmd, int fd);

#ifdef __cplusplus
}
#endif

/*
 * a macro for aligning things
 *	p (pointer) is bumped up to next boundary of size "sz".
 */
#define	DM_SHM_ALIGN(p, sz) \
	((p) = (char *)(((size_t)(p) + (sz) - 1) & ~((sz) - 1))) 
#define DM_SHM_ALIGN_SZ sizeof(size_t)

#ifdef PIN_64_BIT
  #define DMO_INT_PRINTF_PATTERN          I64_PRINTF_PATTERN
#else
  #define DMO_INT_PRINTF_PATTERN
#endif

#endif /*_DM_MALLOC_H*/
