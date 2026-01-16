/* continuus file information --- %full_filespec: psiu_hashtable.h~6:incl:3 % */
/*
 *  @(#) %full_filespec: psiu_hashtable.h~6:incl:3 %
 *
 *      Copyright (c) 1998 - 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PSIU_HASHTABLE_H_
#define _PSIU_HASHTABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pcm.h"
#include "psiu_list.h"

typedef struct psiu_hashbucket {
	psiu_list_t		entries;	/* Entry list */
} psiu_hashbucket_t;

typedef struct psiu_hashtable {
	int32			nbuckets;	/* Number of hash buckets */
	psiu_hashbucket_t	*buckets;	/* Bucket array */
} psiu_hashtable_t;

typedef struct psiu_hashcookie {
	int32			bucketnum;
	psiu_listcookie_t	entry;
} psiu_hashcookie_t;

typedef int32	(*psiu_hash_func_t)(void *, int32);
typedef int32	(*psiu_compar_func_t)(void *, void *);
typedef void	(*psiu_delete_elem_func_t)(void *, void *   );
typedef void	(*psiu_print_func_t)(FILE *, void *);

/*
 * Initialize a hash cookie
 * Parameters:
 *	cookiep:	pointer to an existing hash cookie
 */
extern void
psiu_hash_init_cookie(
	psiu_hashcookie_t	*cookiep);
	
/*
 * Create a hash table
 * Parameters:
 *	nbuckets:	number of hash buckets to create
 */
extern psiu_hashtable_t *
psiu_hash_create(
	int32			nbuckets,
	pin_errbuf_t		*ebufp);

/*
 * Find an entry
 * Parameters:
 *	hashtablep	hash table pointer
 *	entry		pointer to entry to find
 *	hash		hash function to convert an entry to an int
 *	compare		compare function to compare if two entries are the same
 *			first arg of compare is incoming element
 *			second arg of compare is existing hash element
 */
extern void *
psiu_hash_find(
	psiu_hashtable_t	*hashtablep,
	void			*entryp,
	psiu_hash_func_t	hash,
	psiu_compar_func_t	compare,
	pin_errbuf_t		*ebufp);

/*
 * Add an entry
 * Parameters:
 *	hashtablep	hash table pointer
 *	entry		pointer to entry to find
 *	hash		hash function to convert an entry to an int
 */
extern void
psiu_hash_add(
	psiu_hashtable_t	*hashtablep,
	void			*entryp,
	psiu_hash_func_t		hash,
	pin_errbuf_t		*ebufp);

/*
 * Update an entry
 * Parameters:
 *	hashtablep	hash table pointer
 *	entry		pointer to entry to be update
 *	hash		hash function to convert an entry to an int
 *	compare		compare function to compare if two entries are the same
 *			first arg of compare is incoming element
 *			second arg of compare is existing hash element
 *	b_deleted	Should the elem be deleted before updating. If the valid
 *			is 0, do not delete the elem. Otherwise delete the elem.
 */
extern void
psiu_hash_update(
	psiu_hashtable_t	*hashtablep,
	void			*entryp,
	psiu_hash_func_t	hash,
	psiu_compar_func_t	compare,
	int32			b_deleted,
	pin_errbuf_t		*ebufp);

/*
 * Delete a element
 * Parameters:
 *	hashtablep	hash table pointer
 *      entry           pointer to entry to find
 *      hash            hash function to convert an entry to an int
 *      compare         compare function to compare if two entries are the same
 *                      first arg of compare is incoming element
 *                      second arg of compare is existing hash element
 *      delete_elem    function to delete the sub elements in the element
 */
extern void
psiu_hash_delete(
	psiu_hashtable_t	*hashtablep,
	void                    *entryp,
	psiu_hash_func_t        hash,
	psiu_compar_func_t      compare,
	psiu_delete_elem_func_t delete_elem,
	pin_errbuf_t		*ebufp);

/*
 * Delete a element
 * Parameters:
 *	hashtablep	hash table pointer
 *      entry           pointer to entry to find
 *      hash            hash function to convert an entry to an int
 *      compare         compare function to compare if two entries are the same
 *                      first arg of compare is incoming element
 *                      second arg of compare is existing hash element
 *      delete_elem    function to delete the sub elements in the element
 *      ientry         if ientry is 0, do not free the entry.
 */
extern void
psiu_hash_delete_ex(
	psiu_hashtable_t	*hashtablep,
	void                    *entryp,
	psiu_hash_func_t        hash,
	psiu_compar_func_t      compare,
	psiu_delete_elem_func_t delete_elem,
	int32					ientry,
	pin_errbuf_t		*ebufp);

/*
 * Destroy all elements
 * Parameters:
 *	hashtablep	hash table pointer
 *      delete_elem    function to delete the sub elements in the element
 *                     It should not delete the element itself
 */
extern void
psiu_hash_delete_all(

	psiu_hashtable_t	*hashtablep,
	psiu_delete_elem_func_t delete_elem,
	pin_errbuf_t		*ebufp);


/*
 * Destroy all elements
 * Parameters:
 *	hashtablep	hash table pointer
 *      delete_elem    function to delete the sub elements in the element
 *                     It should not delete the element itself
 *      ientry         if ientry is 0 do not free the entry.
 */
extern void
psiu_hash_delete_all_ex(

	psiu_hashtable_t	*hashtablep,
	psiu_delete_elem_func_t delete_elem,
	int32				ientry,
	pin_errbuf_t		*ebufp);

/*
 * Destroy a hashtable
 * Parameters:
 *	hashtablep	hash table pointer
 */
extern void
psiu_hash_destroy(
	psiu_hashtable_t	*hashtablep,
	pin_errbuf_t		*ebufp);

/*
 * Destroy a hashtable with/without entries
 * Parameters:
 *	hashtablep	hash table pointer
 *	ientry		if ientry is not 0, destroy the entry also.
 *			otherwise, do not destroy the entry
 */
extern void
psiu_hash_destroy_ex(
	psiu_hashtable_t	*hashtablep,
	int32			ientry,
	pin_errbuf_t		*ebufp);

/*
 * Get the next element from hash table
 * Parameters:
 *	hashtablep	hash table pointer
 *	cookiep		pointer to hash cookie
 *
 */
extern void *
psiu_hash_getnext(
	psiu_hashtable_t	*hashtablep,
	psiu_hashcookie_t	*cookiep,
	pin_errbuf_t		*ebufp);


/*
 * Print the elements in the hashtable
 * Parameters:
 *	hashtablep	hash table pointer
 *	print		print function to print an entry
 */
extern void
psiu_hash_print(
	FILE			*fp,
	psiu_hashtable_t	*hashtablep,
	psiu_print_func_t	print,
	pin_errbuf_t		*ebufp);

#ifdef __cplusplus
}
#endif

#endif /*_PSIU_HASHTABLE_H_*/
