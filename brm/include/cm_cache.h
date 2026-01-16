/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2020 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */


#ifndef CM_CACHE_H
#define CM_CACHE_H

#undef EXTERN
#ifdef __CMPIN__ 
#define EXTERN PIN_EXPORT 
#else
#define EXTERN PIN_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 * define cache key types
 */
#define CM_CACHE_KEY_STR	1
#define CM_CACHE_KEY_POID	2
#define CM_CACHE_KEY_IDDBSTR	3

/* for cache key of ..._STR, use null terminated char string as key. */
/* for cache key of ..._POID, only use id and db as key. */
typedef struct cm_cache_key_poid {
	int64	id;
	int64	db;
} cm_cache_key_poid_t;

/* for cache key of ..._IDDBSTR, only (poid) id and db + string as key. */
typedef struct cm_cache_key_iddbstr {
	int64	id;
	int64	db;
	char	*str;
} cm_cache_key_iddbstr_t;


typedef void *cm_cache_t;

/*
 * call at init time, specified via fm_module in CM pin.conf
 */
EXTERN cm_cache_t * 
cm_cache_init(char *name,	/* a name, for statistics print mostly */
	int32 n_ents,		/* number of entries to allow */
	int32 size,		/* size of data memory */
	int32 hash_modulus,	/* number of hash buckets */
	int32 keytype,		/* CM_CACHE_KEY_xxx the key type */
	int32 *errp);		/* PIN_ERR_xxx */

EXTERN cm_cache_t *
cm_cache_init_byname(char *name,   /* the name for the cache */
                     int32 keytype, /* CM_CACHE_KEY_xxx the key type */
                     int32 *errp); /* PIN_ERR_xxx */

EXTERN cm_cache_t *
cm_cache_init_by_flist(
                char            *name,		/* the name for the cache */
                pin_flist_t     *in_flistp,	/* The flist of data to cache */
                pin_fld_num_t   field,		/* name of field from which */
						/* to get the CM_CACHE_KEY_xx */
						/* the key type */
                int32           hash_modulus,	/* number of hash buckets */
						/* <= to zero means same as */
						/* number of PIN_FLD_RESULTS */
                int32           *errp );	/* PIN_ERR_xxx */
EXTERN void
cm_cache_re_init_by_flist(
		cm_cache_t      *cachep,
		pin_flist_t     *in_flistp,     /* The flist of data to cache */
		pin_fld_num_t   field,          /* name of field from which */
						/* to get the CM_CACHE_KEY_xx */
						/* the key type */
		int32           *errp );        /* PIN_ERR_xxx */

/* This function will initialize a CM cache and copy the past flist
 * into it.
 * Returns PIN_ERR_xxx
 */
EXTERN int32 cm_cache_init_and_copy_flist(
	pin_flist_t*      in_flistp,       /* input flist */
	char*             cache_name,      /* cache name */
	cm_cache_t**      cachepp,         /* return ptr to created cache */
	pin_flist_t**     out_flistpp);     /* return ptr to flist in the cache */

/*
 * call to find an entry.
 * returns ptr to flist (read only please!), else NULL if not found or error.
 * *errp set to PIN_ERR_NONE if found, else PIN_ERR_xxx
 */
EXTERN pin_flist_t *
cm_cache_find_entry(cm_cache_t *cachep,
	void *keyvalp,
	int32 *errp);

/*
 * cm_cache_add_entry - used to (try to) add an entry into the hash
 *	list.  checks to see if already there.
 */
EXTERN void
cm_cache_add_entry(cm_cache_t *cachep,
	void	*keyvalp,
	pin_flist_t	*in_flistp,
	int32	*errp);

/*
 * cm_cache_update_entry - used to update an existing entry
 *	in the cache.  If it doesn't exit, this function
 *	will call add_entry.
 */
EXTERN void
cm_cache_update_entry(
	cm_cache_t	*in_cachep,
	void		*keyvalp,
	pin_flist_t	*in_flistp,
	int32		*errp);

#ifdef __cplusplus
}
#endif

#undef EXTERN

#endif /*CM_CACHE_H*/
