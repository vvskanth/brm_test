/* continuus file information --- %full_filespec: psiu_list.h~5:incl:3 % */
/*
 *  @(#) %full_filespec: psiu_list.h~5:incl:3 %
 *
 *      Copyright (c) 1998 - 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PSIU_LIST_H_
#define _PSIU_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pcm.h"

typedef struct psiu_node {
	struct psiu_node	* next;
	void			*elem;
} psiu_node_t;

typedef psiu_node_t	*psiu_list_t;

typedef void		*psiu_listcookie_t;

/*
 * Initialize a list
 * Parameters:
 *	listp		pointer to the list
 */
extern void
psiu_list_init(
	psiu_list_t	*listp,
	pin_errbuf_t	*ebufp);

/*
 * Insert an element at the beginning of the list
 * Parameters:
 *	listp		pointer to the list
 *	elem		pointer to the element
 */
extern void
psiu_list_insert(
	psiu_list_t	*listp,
	void		*elem,
	pin_errbuf_t	*ebufp);

/*
 * Update an element in the list
 * Parameters:
 *	listp		pointer to the list
 *	newelem		pointer to the new element
 *	oldelem		pointer to the old element
 *	b_deleted	Should the elem be deleted before updating. 
 *			If the valid is 0, do not delete the elem.
 *			Otherwise delete the elem.
 */
extern void
psiu_list_update(
	psiu_list_t	*listp,
	void		*newelem,
	void		*oldelem,
	int32		b_deleted,
	pin_errbuf_t	*ebufp);

/*
 * Insert an element at the beginning of the list
 * Parameters:
 *	listp		pointer to the list
 *	elem		pointer to the element
 */
extern void
psiu_list_insert(
	psiu_list_t	*listp,
	void		*elem,
	pin_errbuf_t	*ebufp);


/*
 * Append an element at the end of the list
 * Parameters:
 *	listp		pointer to the list
 *	elem		pointer to the element
 */
void
psiu_list_append(
	psiu_list_t	*listp,
	void		*elem,
	pin_errbuf_t	*ebufp);


/*
 * Return the next element from the list
 * Parameters:
 *	cookiep		pointer to a list cookie
 */
extern void *
psiu_list_get_next(
	psiu_list_t	list,
	psiu_listcookie_t *cookiep,
	pin_errbuf_t	*ebufp);

/*
 * Delete the first node from the list
 * Parameters:
 *	listp		pointer to the list
 */
extern void
psiu_list_delete_first(
	psiu_list_t	*listp,
	pin_errbuf_t	*ebufp);

/*
 * Delete the first node from the list
 * Parameters:
 *	listp		pointer to the list
 *	ientry		if ientry is 1, destroy the entry also.
 *			otherwise, do not destroy the entry
 */
extern void
psiu_list_delete_first_ex(
	psiu_list_t	*listp,
	int32		ientry,
	pin_errbuf_t	*ebufp);

/*
 * Delete the last node from the list
 * Parameters:
 *	listp		pointer to the list
 */
extern void
psiu_list_delete_last(
	psiu_list_t	*listp,
	pin_errbuf_t	*ebufp);

/*
 * Delete the node containing the element
 * Parameters:
 *	listp		pointer to the list
 *	elem		pointer to an element
 */
extern void
psiu_list_delete_elem(
	psiu_list_t	*listp,
	void		*elem,
	pin_errbuf_t	*ebufp);

/*
 * Delete the node containing the element
 * Parameters:
 *	listp		pointer to the list
 *	elem		pointer to an element
 *  ientry      if ientry is 0, do not destroy the elem.
 */
extern void
psiu_list_delete_elem_ex(
	psiu_list_t	*listp,
	void		*elem,
	int32		ientry,
	pin_errbuf_t	*ebufp);

/*
 * Destroy a list, (*listp) will be set to NULL after call
 * Parameters:
 *	listp		pointer to the list
 */
extern void
psiu_list_destroy(
	psiu_list_t	*listp,
	pin_errbuf_t	*ebufp);

/*
 * Destroy a list, (*listp) will be set to NULL after call
 * Parameters:
 *	listp		pointer to the list
 *	ientry		if ientry is not 0, destroy the entry also.
 *			otherwise, do not destroy the entry
 */
extern void
psiu_list_destroy_ex(
	psiu_list_t	*listp,
	int32		ientry,
	pin_errbuf_t	*ebufp);

#ifdef __cplusplus
}
#endif

#endif /*_PSIU_LIST_H_*/
