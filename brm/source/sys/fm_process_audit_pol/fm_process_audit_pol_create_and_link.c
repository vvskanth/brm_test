/*
 * Copyright (c) 2003, 2023, Oracle and/or its affiliates.
 *	
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)% %";
#endif

/*******************************************************************
 * Contains the PCM_OP_PROCESS_AUDIT_POL_CREATE_AND_LINK operation.
 *******************************************************************/

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>

#define FILE_LOGNAME "fm_process_audit_pol_create_and_link.c(1)"

#include "ops/process_audit.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_channel.h"
#include "fm_utils.h"
#include "pin_process_audit.h"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/

EXPORT_OP void
op_process_audit_pol_create_and_link(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_process_audit_pol_create_and_link(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static int32
search_duplicate_process_audit_object(
	pcm_context_t           *ctxp,
	pin_flist_t             *i_flistp,
	char					*object_type,
	pin_flist_t             **result_flistpp,
	pin_errbuf_t            *ebufp);

static int32
search_duplicate_entry_in_process_audit_object(
	pin_flist_t             *i_flistp,
	pin_flist_t             *result_array_flistp,
	char					*object_type,
	pin_errbuf_t            *ebufp);

static void
check_process_audit_simple(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_status(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_svctype(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_revstream(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_svctype_status(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_revstream_status(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_svctype_revstream(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_svctype_revstream_status(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_suspense(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_svctype_revstream_outputstream(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_svctype_revstream_status_outputstream(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);

static void
check_process_audit_outputstream(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp);


/*******************************************************************
 * Main routine for the PCM_OP_PROCESS_AUDIT_POL_CREATE_AND_LINK operation.
 *******************************************************************/
void
op_process_audit_pol_create_and_link(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Null out results
	 ***********************************************************/
	*r_flistpp = NULL;

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_PROCESS_AUDIT_POL_CREATE_AND_LINK) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_process_audit_pol_create_and_link opcode error", ebufp);

		return;
	}
	
	/***********************************************************
	 * Debut what we got.
         ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_process_audit_pol_create_and_link input flist", i_flistp);

	/***********************************************************
	 * Main rountine for this opcode
	 ***********************************************************/
	fm_process_audit_pol_create_and_link(ctxp, flags, i_flistp, r_flistpp, ebufp);
	
	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(r_flistpp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_process_audit_pol_create_and_link error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_process_audit_pol_create_and_link output flist", *r_flistpp);
	}

	return;
}

/*******************************************************************
 * fm_process_audit_pol_create_and_link:
 *******************************************************************/
static void
fm_process_audit_pol_create_and_link(
	pcm_context_t           *ctxp,
	int32                   flags,
	pin_flist_t             *i_flistp,
	pin_flist_t             **r_flistpp,
	pin_errbuf_t            *ebufp)
{

	poid_t			*old_poid = NULL;

	pin_flist_t		*result_flistp = NULL;
	int32			result_count = 0;
	pin_flist_t		*result_array_flistp = NULL;

	pin_flist_t		*o_flistp = NULL;

	void			*vp = NULL;
	
	char			object_type[256] = "";
	char			control_point[256] = "";
	char			batch_id[256] = "";

	
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	if (PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_OBJECT_TYPE, 1, ebufp) != NULL)
		PIN_FLIST_FLD_DROP(i_flistp, PIN_FLD_OBJECT_TYPE, ebufp);

	if (PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_SESSION_OBJ, 1, ebufp) != NULL)
		PIN_FLIST_FLD_DROP(i_flistp, PIN_FLD_SESSION_OBJ, ebufp);

	if (PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_SERVICE_OBJ, 1, ebufp) != NULL)
		PIN_FLIST_FLD_DROP(i_flistp, PIN_FLD_SERVICE_OBJ, ebufp);

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	if (vp)	pin_strlcpy(object_type, (const char *)PIN_POID_GET_TYPE((poid_t *)(vp)), sizeof(object_type));

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_PROCESS_NAME, 0, ebufp);
	if (vp) pin_strlcpy(control_point, (const char *)vp, sizeof(control_point));

	vp = PIN_FLIST_SUBSTR_GET(i_flistp, PIN_FLD_BATCH_STAT, 0, ebufp);
	if (vp)
	{
		vp = PIN_FLIST_FLD_GET((pin_flist_t *)vp, PIN_FLD_BATCH_ID, 0, ebufp);
		if (vp) pin_strlcpy(batch_id, (const char *)vp, sizeof(batch_id));
	}

	if (strstr(object_type, "/process_audit/batchstat/") != object_type)
	{
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
			"fm_process_audit_pol_create_and_link: invalid object type, "
			"object type should be a subtype of /process_audit/batchstat");

		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
			PIN_ERR_BAD_VALUE, PIN_FLD_OBJECT_TYPE, 0, 0);

		return;
	}

	if (search_duplicate_process_audit_object(ctxp, i_flistp, object_type, 
		&result_flistp, ebufp) == PIN_BOOLEAN_FALSE)
	{
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
			"fm_process_audit_pol_create_and_link: error in "
			"search_duplicate_process_audit_object");

		return;
	}

	result_count = PIN_FLIST_ELEM_COUNT(result_flistp, PIN_FLD_RESULTS, ebufp);
	if (result_count <= 0)
	{
		PIN_FLIST_DESTROY_EX(&result_flistp, NULL);

		o_flistp = PIN_FLIST_COPY(i_flistp, ebufp);

		PIN_FLIST_FLD_SET(o_flistp, PIN_FLD_ACTION, 
			(void *)ACTION_CREATE_NEW_PROCESS_AUDIT_OBJECT, ebufp);

		*r_flistpp = o_flistp;
	}
	else
	{
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
			"fm_process_audit_pol_create_and_link: "
			"duplicate object found, checking for element uniqueness");

		result_array_flistp = PIN_FLIST_ELEM_GET(result_flistp, PIN_FLD_RESULTS, 
			0, 0, ebufp);

		if (search_duplicate_entry_in_process_audit_object(i_flistp, 
			result_array_flistp, object_type, ebufp) == PIN_BOOLEAN_FALSE)
		{
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
				"fm_process_audit_pol_create_and_link: "
				"error in search_duplicate_entry_in_process_audit_object");

			PIN_FLIST_DESTROY_EX(&result_flistp, NULL);
			return;
		}
		
		/**
		 * Getting the poid of the matched object from the result flistp, we need 
		 * this to be able to modify the correct object later down the code
		 **/
		old_poid = (poid_t *)PIN_FLIST_FLD_TAKE(result_array_flistp, 
			PIN_FLD_POID, 0, ebufp);

		PIN_FLIST_DESTROY_EX(&result_flistp, NULL);

		o_flistp = PIN_FLIST_COPY(i_flistp, ebufp);
		PIN_FLIST_FLD_PUT(o_flistp, PIN_FLD_POID, old_poid, ebufp);

		PIN_FLIST_FLD_SET(o_flistp, PIN_FLD_ACTION, 
			(void *)ACION_UPDATE_EXISTING_PROCESS_AUDIT_OBJECT, ebufp);

		/**
		 * Dropping fields that should not be updated
		 **/
		PIN_FLIST_FLD_DROP(o_flistp, PIN_FLD_PROCESS_NAME, ebufp);
		PIN_FLIST_SUBSTR_DROP(o_flistp, PIN_FLD_BATCH_STAT, ebufp);
		
		if (PIN_FLIST_FLD_GET(o_flistp, PIN_FLD_PROGRAM_NAME, 1, ebufp)
			!= NULL)

			PIN_FLIST_FLD_DROP(o_flistp, PIN_FLD_PROGRAM_NAME, ebufp);

		if (PIN_FLIST_FLD_GET(o_flistp, PIN_FLD_PROCESS_START_T, 1, ebufp)
			!= NULL)

			PIN_FLIST_FLD_DROP(o_flistp, PIN_FLD_PROCESS_START_T, ebufp);

		if (PIN_FLIST_FLD_GET(o_flistp, PIN_FLD_VERSION_ID, 1, ebufp)
			!= NULL)

			PIN_FLIST_FLD_DROP(o_flistp, PIN_FLD_VERSION_ID, ebufp);

		/**
		 * Setting the ElementId to Assign so that the DM will assign a unique 
		 * element id to this element
		 **/
		vp = PIN_FLIST_ELEM_TAKE(o_flistp, PIN_FLD_GROUP_DETAILS, 0, 0, ebufp);
		PIN_FLIST_ELEM_PUT(o_flistp, vp, PIN_FLD_GROUP_DETAILS, 
			PIN_ELEMID_ASSIGN, ebufp);

		*r_flistpp = o_flistp;
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_process_audit_pol_create_and_link error", ebufp);
	}

	return;
}


/*******************************************************************
 * search_duplicate_process_audit_object
 *******************************************************************/
static int32
search_duplicate_process_audit_object(
	pcm_context_t           *ctxp,
	pin_flist_t             *i_flistp,
	char					*object_type,
	pin_flist_t             **result_flistpp,
	pin_errbuf_t            *ebufp)
{

	poid_t			*old_poid = NULL;
	poid_t			*new_poid = NULL;

	pin_flist_t		*search_flistp = NULL;
	pin_flist_t		*search_args_flistp = NULL;
	char			search_template[255];
	poid_t			*search_poid = NULL;
	int				search_flags = 256;

	void			*vp = NULL;

	/**
	 * Creating search flist
	**/
	search_flistp = PIN_FLIST_CREATE(ebufp);

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	search_poid = PIN_POID_CREATE(PIN_POID_GET_DB((poid_t *)(vp)), "/search", 
		-1, ebufp);

	if (PIN_ERR_IS_ERR(ebufp))
	{
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
			"fm_process_audit_pol_create_and_link: error creating search poid");

		PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
		return PIN_BOOLEAN_FALSE;
	}

	PIN_FLIST_FLD_PUT(search_flistp, PIN_FLD_POID, search_poid, ebufp);
	PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_FLAGS, (void *)&search_flags, ebufp);
	PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_RESULTS, PIN_ELEMID_ANY, ebufp);
	
	search_args_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS, 1, ebufp);
	search_args_flistp = PIN_FLIST_SUBSTR_ADD(search_args_flistp, 
		PIN_FLD_BATCH_STAT, ebufp);

	vp = PIN_FLIST_SUBSTR_GET(i_flistp, PIN_FLD_BATCH_STAT, 0, ebufp);
	vp = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), PIN_FLD_BATCH_ID, 0, ebufp);
	PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_BATCH_ID, vp, ebufp);

	search_args_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS, 2, ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_PROCESS_NAME, 0, ebufp);
	PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_PROCESS_NAME, vp, ebufp);

	search_args_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS, 3, ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	
	search_poid = PIN_POID_CREATE(PIN_POID_GET_DB((poid_t *)(vp)), object_type, 
		-1, ebufp);

	if (PIN_ERR_IS_ERR(ebufp))
	{
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
			"fm_process_audit_pol_create_and_link: "
			"error creating search parameter poid");

		PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
		return PIN_BOOLEAN_FALSE;
	}

	PIN_FLIST_FLD_PUT(search_args_flistp, PIN_FLD_POID, search_poid, 
		ebufp);

	pin_snprintf(search_template, sizeof(search_template), 
		" select X from /process_audit/batchstat "
		"where F1 = V1 and F2 = V2 and F3 = V3 ");

	PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_TEMPLATE, (void *)search_template, 
		ebufp);

	if (PIN_ERR_IS_ERR(ebufp))
	{
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
			"fm_process_audit_pol_create_and_link: error creating search flist");

		PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
		return PIN_BOOLEAN_FALSE;
	}

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
		"fm_process_audit_pol_create_and_link: initial search flist is", 
		search_flistp);

	PCM_OP(ctxp, PCM_OP_SEARCH, 0, search_flistp, result_flistpp, ebufp);
	if (PIN_ERR_IS_ERR(ebufp))
	{
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
			"fm_process_audit_pol_create_and_link: error searching the objects");

		PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
		PIN_FLIST_DESTROY_EX(result_flistpp, NULL);

		return PIN_BOOLEAN_FALSE;
	}
	
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
		"fm_process_audit_pol_create_and_link: search results flist is", 
		*result_flistpp);
	
	PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
	return PIN_BOOLEAN_TRUE;
}

/*******************************************************************
 * search_duplicate_entry_in_process_audit_object
 *******************************************************************/
static int32
search_duplicate_entry_in_process_audit_object(
	pin_flist_t             *i_flistp,
	pin_flist_t             *result_array_flistp,
	char					*object_type,
	pin_errbuf_t            *ebufp)
{
	
	void			*vp = NULL;

	int32			batch_type = BATCH_TYPE_UNKNOWN;

	vp = PIN_FLIST_SUBSTR_GET(i_flistp, PIN_FLD_BATCH_STAT, 0, ebufp);
	vp = PIN_FLIST_FLD_GET((pin_flist_t *)vp, PIN_FLD_BATCH_TYPE, 0, ebufp);
	if (vp)
		batch_type = *(int32 *)vp;

	vp = PIN_FLIST_SUBSTR_GET(result_array_flistp, PIN_FLD_BATCH_STAT, 0, ebufp);
	vp = PIN_FLIST_FLD_GET((pin_flist_t *)vp, PIN_FLD_BATCH_TYPE, 0, ebufp);
	if (vp)
	{
		if (batch_type != *(int32 *)vp)
		{
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
				"fm_process_audit_pol_create_and_link: "
				"pin_fld_original_batch_id or pin_fld_suspended_from_batch_id is invalid");

			pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
				PIN_ERR_BAD_ARG, PIN_FLD_ORIGINAL_BATCH_ID, 0, 0);

			return PIN_BOOLEAN_FALSE;
		}
	}

	vp = PIN_FLIST_ELEM_GET(i_flistp, PIN_FLD_GROUP_DETAILS, 0, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp))
		return PIN_BOOLEAN_FALSE;

	if (strcmp(object_type, "/process_audit/batchstat/simple") == 0)

		check_process_audit_simple(vp, result_array_flistp, 
			batch_type, ebufp);

	else if (strcmp(object_type, "/process_audit/batchstat/status") == 0)

		check_process_audit_status(vp, result_array_flistp, 
			batch_type, ebufp);

	else if (strcmp(object_type, "/process_audit/batchstat/svctype") == 0)

		check_process_audit_svctype(vp, result_array_flistp, 
			batch_type, ebufp);

	else if (strcmp(object_type, "/process_audit/batchstat/revstream") == 0)

		check_process_audit_revstream(vp, result_array_flistp, 
			batch_type, ebufp);

	else if (strcmp(object_type, "/process_audit/batchstat/svctype_status") == 0)

		check_process_audit_svctype_status(vp, result_array_flistp, 
			batch_type, ebufp);

	else if (strcmp(object_type, "/process_audit/batchstat/revstream_status") == 0)

		check_process_audit_revstream_status(vp, result_array_flistp, 
			batch_type, ebufp);

	else if ((strcmp(object_type, 
		"/process_audit/batchstat/svctype_revstream") == 0) || 
		(strcmp(object_type, "/process_audit/batchstat/revstream_svctype") == 0))

		check_process_audit_svctype_revstream(vp, result_array_flistp, 
			batch_type, ebufp);

	else if ((strcmp(object_type, 
		"/process_audit/batchstat/svctype_revstream_status") == 0) || 
		(strcmp(object_type, 
		"/process_audit/batchstat/revstream_svctype_status") == 0))

		check_process_audit_svctype_revstream_status(vp, result_array_flistp, 
			batch_type, ebufp);

	else if (strcmp(object_type, "/process_audit/batchstat/suspense") == 0)

		check_process_audit_suspense(vp, result_array_flistp, 
			batch_type, ebufp);

	else if (strcmp(object_type, 
		"/process_audit/batchstat/svctype_revstream_outputstream") == 0)

		check_process_audit_svctype_revstream_outputstream(vp, 
			result_array_flistp, batch_type, ebufp);

	else if (strcmp(object_type, 
		"/process_audit/batchstat/svctype_revstream_status_outputstream") == 0)

		check_process_audit_svctype_revstream_status_outputstream(vp, 
			result_array_flistp, batch_type, ebufp);

	else if (strcmp(object_type, "/process_audit/batchstat/outputstream") == 0)

		check_process_audit_outputstream(vp, result_array_flistp, 
			batch_type, ebufp);

	else
	{
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
			"fm_process_audit_pol_create_and_link: "
			"invalid process_audit object, modify the policy opcode "
			"'pcm_op_process_audit_pol_and_link' "
			"to support this object");

		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
			PIN_ERR_BAD_VALUE, PIN_FLD_OBJECT_TYPE, 0, 0);
	}

	if (PIN_ERR_IS_ERR(ebufp))
		return PIN_BOOLEAN_FALSE;
	else
		return PIN_BOOLEAN_TRUE;
}

/*******************************************************************
 * check_process_audit_simple:
 *******************************************************************/
static void
check_process_audit_simple(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = PIN_FLIST_ELEM_GET_NEXT
		(result_array_flistp, PIN_FLD_GROUP_DETAILS, &elem_id, 1, &cookie, 
		ebufp)) != NULL)
	{

		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		if (batch_type == BATCH_TYPE_MEDIATION)
		{
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
				"fm_process_audit_pol_create_and_link: "
				"duplicate entry found");

			pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
				PIN_ERR_DUPLICATE, 0, 0, 0);

			break;
		}
		else
		{
			/**
			 * Check if an element with the same Original Batch Id already 
			 * exists
			 **/
			if (orig_batch_id1 && orig_batch_id2 && 
				strcmp(orig_batch_id1, orig_batch_id2) == 0)
			{
				if (batch_type == BATCH_TYPE_RERATING)
				{
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
						"fm_process_audit_pol_create_and_link: "
						"duplicate entry found");

					pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
						PIN_ERR_DUPLICATE, 0, 0, 0);

					break;
				}
				else
				{
					if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
						strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
					{
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
							"fm_process_audit_pol_create_and_link: "
							"duplicate entry found");

						pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
							PIN_ERR_DUPLICATE, 0, 0, 0);

						break;
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_status:
 *******************************************************************/
static void	
check_process_audit_status(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	char			*edr_status1 = NULL, *edr_status2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting EDR Status in the Input Flist
	 **/
	edr_status1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_EDR_STATUS, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = 
		PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, PIN_FLD_GROUP_DETAILS, 
		&elem_id, 1, &cookie, ebufp)) != NULL)
	{

		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		edr_status2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_EDR_STATUS, 0, ebufp);

		/**
		 * Check if an element with the same EDR Staus already exists
		 **/
		if (edr_status1 && edr_status2 && 
			strcmp(edr_status1, edr_status2) == 0)
		{
			if (batch_type == BATCH_TYPE_MEDIATION)
			{
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
					"fm_process_audit_pol_create_and_link: "
					"duplicate entry found");

				pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
					PIN_ERR_DUPLICATE, 0, 0, 0);

				break;
			}
			else
			{
				/**
				 * Check if an element with the same Original Batch Id 
				 * already exists
				 **/
				if (orig_batch_id1 && orig_batch_id2 && 
					strcmp(orig_batch_id1, orig_batch_id2) == 0)
				{
					if (batch_type == BATCH_TYPE_RERATING)
					{
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
							"fm_process_audit_pol_create_and_link: "
							"duplicate entry found");

						pin_set_err(ebufp, PIN_ERRLOC_FM, 
							PIN_ERRCLASS_APPLICATION, 
							PIN_ERR_DUPLICATE, 0, 0, 0);

						break;
					}
					else
					{
						if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
							strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
						{
							PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
								"fm_process_audit_pol_create_and_link: "
								"duplicate entry found");

							pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
								PIN_ERR_DUPLICATE, 0, 0, 0);

							break;
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_svctype:
 *******************************************************************/
static void
check_process_audit_svctype(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	char			*svc_type1 = NULL, *svc_type2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting Service Type in the Input Flist
	 **/
	svc_type1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SERVICE_TYPE, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, 
		PIN_FLD_GROUP_DETAILS, &elem_id, 1, &cookie, ebufp)) != NULL)
	{

		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		svc_type2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SERVICE_TYPE, 0, ebufp);

		/**
		 * Check if an element with the same Service Type already exists
		 **/
		if (svc_type1 && svc_type2 && strcmp(svc_type1, svc_type2) == 0)
		{
			if (batch_type == BATCH_TYPE_MEDIATION)
			{
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
					"fm_process_audit_pol_create_and_link: "
					"duplicate entry found");

				pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
					PIN_ERR_DUPLICATE, 0, 0, 0);

				break;
			}
			else
			{
				/**
				 * Check if an element with the same Original Batch Id 
				 * already exists
				 **/
				if (orig_batch_id1 && orig_batch_id2 && 
					strcmp(orig_batch_id1, orig_batch_id2) == 0)
				{
					if (batch_type == BATCH_TYPE_RERATING)
					{
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
							"fm_process_audit_pol_create_and_link: "
							"duplicate entry found");

						pin_set_err(ebufp, PIN_ERRLOC_FM, 
							PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 0, 0, 0);

						break;
					}
					else
					{
						if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
							strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
						{
							PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
								"fm_process_audit_pol_create_and_link: "
								"duplicate entry found");

							pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
								PIN_ERR_DUPLICATE, 0, 0, 0);

							break;
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_revstream:
 *******************************************************************/
static void
check_process_audit_revstream(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	char			*rev_stream1 = NULL, *rev_stream2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting Revenue Stream in the Input Flist
	 **/
	rev_stream1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_REVENUE_STREAM, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = 
		PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, PIN_FLD_GROUP_DETAILS, 
		&elem_id, 1, &cookie, ebufp)) != NULL)
	{

		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		rev_stream2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_REVENUE_STREAM, 0, ebufp);

		/**
		 * Check if an element with the same Revenue Stream already exists
		 **/
		if (rev_stream1 && rev_stream2 && 
			strcmp(rev_stream1, rev_stream2) == 0)
		{
			if (batch_type == BATCH_TYPE_MEDIATION)
			{
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
					"fm_process_audit_pol_create_and_link: "
					"duplicate entry found");

				pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
					PIN_ERR_DUPLICATE, 0, 0, 0);

				break;
			}
			else
			{
				/**
				 * Check if an element with the same Original Batch Id 
				 * already exists
				 **/
				if (orig_batch_id1 && orig_batch_id2 && 
					strcmp(orig_batch_id1, orig_batch_id2) == 0)
				{
					if (batch_type == BATCH_TYPE_RERATING)
					{
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
							"fm_process_audit_pol_create_and_link: "
							"duplicate entry found");

						pin_set_err(ebufp, PIN_ERRLOC_FM, 
							PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 0, 0, 0);

						break;
					}
					else
					{
						if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
							strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
						{
							PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
								"fm_process_audit_pol_create_and_link: "
								"duplicate entry found");

							pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
								PIN_ERR_DUPLICATE, 0, 0, 0);

							break;
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_svctype_status:
 *******************************************************************/
static void
check_process_audit_svctype_status(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	char			*edr_status1 = NULL, *edr_status2 = NULL;
	char			*svc_type1 = NULL, *svc_type2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting Service Type in the Input Flist
	 **/
	svc_type1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SERVICE_TYPE, 0, ebufp);

	/**
	 * Getting EDR Status in the Input Flist
	 **/
	edr_status1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_EDR_STATUS, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = 
		PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, PIN_FLD_GROUP_DETAILS, 
		&elem_id, 1, &cookie, ebufp)) != NULL)
	{

		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		svc_type2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SERVICE_TYPE, 0, ebufp);

		edr_status2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_EDR_STATUS, 0, ebufp);

		/**
		 * Check if an element with the same Service Type already exists
		 **/
		if (svc_type1 && svc_type2 && strcmp(svc_type1, svc_type2) == 0)
		{
			/**
			 * Check if an element with the same EDR Staus already exists
			 **/
			if (edr_status1 && edr_status2 && 
				strcmp(edr_status1, edr_status2) == 0)
			{
				if (batch_type == BATCH_TYPE_MEDIATION)
				{
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
						"fm_process_audit_pol_create_and_link: "
						"duplicate entry found");

					pin_set_err(ebufp, PIN_ERRLOC_FM, 
						PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 0, 0, 0);

					break;
				}
				else
				{
					/**
					 * Check if an element with the same Original Batch Id 
					 * already exists
					 **/
					if (orig_batch_id1 && orig_batch_id2 && 
						strcmp(orig_batch_id1, orig_batch_id2) == 0)
					{
						if (batch_type == BATCH_TYPE_RERATING)
						{
							PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
								"fm_process_audit_pol_create_and_link: "
								"duplicate entry found");

							pin_set_err(ebufp, PIN_ERRLOC_FM, 
								PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 
								0, 0, 0);

							break;
						}
						else
						{
							if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
								strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
							{
								PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
									"fm_process_audit_pol_create_and_link: "
									"duplicate entry found");

								pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
									PIN_ERR_DUPLICATE, 0, 0, 0);

								break;
							}
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_revstream_status:
 *******************************************************************/
static void
check_process_audit_revstream_status(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	char			*edr_status1 = NULL, *edr_status2 = NULL;
	char			*rev_stream1 = NULL, *rev_stream2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting Revenue Stream in the Input Flist
	 **/
	rev_stream1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_REVENUE_STREAM, 0, ebufp);

	/**
	 * Getting EDR Status in the Input Flist
	 **/
	edr_status1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_EDR_STATUS, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = 
		PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, PIN_FLD_GROUP_DETAILS, 
		&elem_id, 1, &cookie, ebufp)) != NULL)
	{
		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		rev_stream2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_REVENUE_STREAM, 0, ebufp);

		edr_status2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_EDR_STATUS, 0, ebufp);

		/**
		 * Check if an element with the same Revenue Stream already exists
		 **/
		if (rev_stream1 && rev_stream2 && 
			strcmp(rev_stream1, rev_stream2) == 0)
		{
			/**
			 * Check if an element with the same EDR Staus already exists
			 **/
			if (edr_status1 && edr_status2 && 
				strcmp(edr_status1, edr_status2) == 0)
			{
				if (batch_type == BATCH_TYPE_MEDIATION)
				{
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
						"fm_process_audit_pol_create_and_link: "
						"duplicate entry found");

					pin_set_err(ebufp, PIN_ERRLOC_FM, 
						PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 0, 0, 0);

					break;
				}
				else
				{
					/**
					 * Check if an element with the same Original Batch Id 
					 * already exists
					 **/
					if (orig_batch_id1 && orig_batch_id2 && 
						strcmp(orig_batch_id1, orig_batch_id2) == 0)
					{
						if (batch_type == BATCH_TYPE_RERATING)
						{
							PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
								"fm_process_audit_pol_create_and_link: "
								"duplicate entry found");

							pin_set_err(ebufp, PIN_ERRLOC_FM, 
								PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 
								0, 0, 0);

							break;
						}
						else
						{
							if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
								strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
							{
								PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
									"fm_process_audit_pol_create_and_link: "
									"duplicate entry found");

								pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
									PIN_ERR_DUPLICATE, 0, 0, 0);

								break;
							}
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_svctype_revstream:
 *******************************************************************/
static void
check_process_audit_svctype_revstream(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	char			*svc_type1 = NULL, *svc_type2 = NULL;
	char			*rev_stream1 = NULL, *rev_stream2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting Revenue Stream in the Input Flist
	 **/
	rev_stream1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_REVENUE_STREAM, 0, ebufp);

	/**
	 * Getting Service Type in the Input Flist
	 **/
	svc_type1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SERVICE_TYPE, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = 
		PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, 
		PIN_FLD_GROUP_DETAILS, &elem_id, 1, &cookie, ebufp)) != NULL)
	{
		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		rev_stream2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_REVENUE_STREAM, 0, ebufp);

		svc_type2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SERVICE_TYPE, 0, ebufp);

		/**
		 * Check if an element with the same Revenue Stream already exists
		 **/
		if (rev_stream1 && rev_stream2 && 
			strcmp(rev_stream1, rev_stream2) == 0)
		{
			/**
			 * Check if an element with the same Service Type already exists
			 **/
			if (svc_type1 && svc_type2 && strcmp(svc_type1, svc_type2) == 0)
			{
				if (batch_type == BATCH_TYPE_MEDIATION)
				{
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
						"fm_process_audit_pol_create_and_link: "
						"duplicate entry found");

					pin_set_err(ebufp, PIN_ERRLOC_FM, 
						PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 0, 0, 0);

					break;
				}
				else
				{
					/**
					 * Check if an element with the same Original Batch Id 
					 * already exists
					 **/
					if (orig_batch_id1 && orig_batch_id2 && 
						strcmp(orig_batch_id1, orig_batch_id2) == 0)
					{
						if (batch_type == BATCH_TYPE_RERATING)
						{
							PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
								"fm_process_audit_pol_create_and_link: "
								"duplicate entry found");

							pin_set_err(ebufp, PIN_ERRLOC_FM, 
								PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 
								0, 0, 0);

							break;
						}
						else
						{
							if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
								strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
							{
								PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
									"fm_process_audit_pol_create_and_link: "
									"duplicate entry found");

								pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
									PIN_ERR_DUPLICATE, 0, 0, 0);

								break;
							}
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_svctype_revstream_status:
 *******************************************************************/
static void
check_process_audit_svctype_revstream_status(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	char			*edr_status1 = NULL, *edr_status2 = NULL;
	char			*svc_type1 = NULL, *svc_type2 = NULL;
	char			*rev_stream1 = NULL, *rev_stream2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting Revenue Stream in the Input Flist
	 **/
	rev_stream1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_REVENUE_STREAM, 0, ebufp);

	/**
	 * Getting Service Type in the Input Flist
	 **/
	svc_type1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SERVICE_TYPE, 0, ebufp);

	/**
	 * Getting EDR Status in the Input Flist
	 **/
	edr_status1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_EDR_STATUS, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = 
		PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, PIN_FLD_GROUP_DETAILS, 
		&elem_id, 1, &cookie, ebufp)) != NULL)
	{

		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		rev_stream2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_REVENUE_STREAM, 0, ebufp);

		svc_type2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SERVICE_TYPE, 0, ebufp);

		edr_status2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_EDR_STATUS, 0, ebufp);

		/**
		 * Check if an element with the same Revenue Stream already exists
		 **/
		if (rev_stream1 && rev_stream2 && 
			strcmp(rev_stream1, rev_stream2) == 0)
		{
			/**
			 * Check if an element with the same Service Type already exists
			 **/
			if (svc_type1 && svc_type2 && strcmp(svc_type1, svc_type2) == 0)
			{
				/**
				 * Check if an element with the same EDR Staus already exists
				 **/
				if (edr_status1 && edr_status2 && 
					strcmp(edr_status1, edr_status2) == 0)
				{
					if (batch_type == BATCH_TYPE_MEDIATION)
					{
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
							"fm_process_audit_pol_create_and_link: "
							"duplicate entry found");

						pin_set_err(ebufp, PIN_ERRLOC_FM, 
							PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 
							0, 0, 0);

						break;
					}
					else
					{
						/**
						 * Check if an element with the same Original Batch 
						 * Id already exists
						 **/
						if (orig_batch_id1 && orig_batch_id2 && 
							strcmp(orig_batch_id1, orig_batch_id2) == 0)
						{
							if (batch_type == BATCH_TYPE_RERATING)
							{
								PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
									"fm_process_audit_pol_create_and_link: "
									"duplicate entry found");

								pin_set_err(ebufp, PIN_ERRLOC_FM, 
									PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 
									0, 0, 0);

								break;
							}
							else
							{
								if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
									strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
								{
									PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
										"fm_process_audit_pol_create_and_link: "
										"duplicate entry found");

									pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
										PIN_ERR_DUPLICATE, 0, 0, 0);

									break;
								}
							}
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_suspense:
 *******************************************************************/
static void
check_process_audit_suspense(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	int32			*suspense_reason1 = NULL, *suspense_reason2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspense Reason in the Input Flist
	 **/
	suspense_reason1 = (int32 *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENSE_REASON, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = 
		PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, PIN_FLD_GROUP_DETAILS, 
		&elem_id, 1, &cookie, ebufp)) != NULL)
	{

		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		suspense_reason2 = (int32 *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENSE_REASON, 0, ebufp);

		/**
		 * Check if an element with the same Suspense Reason already exists
		 **/
		if (suspense_reason1 && suspense_reason2 && 
			*suspense_reason1 == *suspense_reason2)
		{
			if (batch_type == BATCH_TYPE_MEDIATION)
			{
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
					"fm_process_audit_pol_create_and_link: "
					"duplicate entry found");

				pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
					PIN_ERR_DUPLICATE, 0, 0, 0);

				break;
			}
			else
			{
				/**
				 * Check if an element with the same Original Batch Id 
				 * already exists
				 **/
				if (orig_batch_id1 && orig_batch_id2 && 
					strcmp(orig_batch_id1, orig_batch_id2) == 0)
				{
					if (batch_type == BATCH_TYPE_RERATING)
					{
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
							"fm_process_audit_pol_create_and_link: "
							"duplicate entry found");

						pin_set_err(ebufp, PIN_ERRLOC_FM, 
							PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 0, 0, 0);

						break;
					}
					else
					{
						if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
							strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
						{
							PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
								"fm_process_audit_pol_create_and_link: "
								"duplicate entry found");

							pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
								PIN_ERR_DUPLICATE, 0, 0, 0);

							break;
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_svctype_revstream_outputstream:
 *******************************************************************/
static void
check_process_audit_svctype_revstream_outputstream(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	char			*svc_type1 = NULL, *svc_type2 = NULL;
	char			*rev_stream1 = NULL, *rev_stream2 = NULL;
	char			*out_stream1 = NULL, *out_stream2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting Revenue Stream in the Input Flist
	 **/
	rev_stream1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_REVENUE_STREAM, 0, ebufp);

	/**
	 * Getting Service Type in the Input Flist
	 **/
	svc_type1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SERVICE_TYPE, 0, ebufp);

	/**
	 * Getting Output Stream in the Input Flist
	 **/
	out_stream1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_OUTPUT_STREAM, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, 
		PIN_FLD_GROUP_DETAILS, &elem_id, 1, &cookie, ebufp)) != NULL)
	{

		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		rev_stream2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_REVENUE_STREAM, 0, ebufp);

		svc_type2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SERVICE_TYPE, 0, ebufp);

		out_stream2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_OUTPUT_STREAM, 0, ebufp);

		/**
		 * Check if an element with the same Revenue Stream already exists
		 **/
		if (rev_stream1 && rev_stream2 && 
			strcmp(rev_stream1, rev_stream2) == 0)
		{
			/**
			 * Check if an element with the same Service Type already exists
			 **/
			if (svc_type1 && svc_type2 && strcmp(svc_type1, svc_type2) == 0)
			{
				/**
				 * Check if an element with the same Output Stream already 
				 * exists
				 **/
				if (out_stream1 && out_stream2 && 
					strcmp(out_stream1, out_stream2) == 0)
				{
					if (batch_type == BATCH_TYPE_MEDIATION)
					{
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
							"fm_process_audit_pol_create_and_link: "
							"duplicate entry found");

						pin_set_err(ebufp, PIN_ERRLOC_FM, 
							PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 
							0, 0, 0);

						break;
					}
					else
					{
						/**
						 * Check if an element with the same Original Batch 
						 * Id already exists
						 **/
						if (orig_batch_id1 && orig_batch_id2 && 
							strcmp(orig_batch_id1, orig_batch_id2) == 0)
						{
							if (batch_type == BATCH_TYPE_RERATING)
							{
								PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
									"fm_process_audit_pol_create_and_link: "
									"duplicate entry found");

								pin_set_err(ebufp, PIN_ERRLOC_FM, 
									PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 
									0, 0, 0);

								break;
							}
							else
							{
								if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
									strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
								{
									PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
										"fm_process_audit_pol_create_and_link: "
										"duplicate entry found");

									pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
										PIN_ERR_DUPLICATE, 0, 0, 0);

									break;
								}
							}
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_svctype_revstream_status_outputstream:
 *******************************************************************/
static void
check_process_audit_svctype_revstream_status_outputstream(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	char			*edr_status1 = NULL, *edr_status2 = NULL;
	char			*svc_type1 = NULL, *svc_type2 = NULL;
	char			*rev_stream1 = NULL, *rev_stream2 = NULL;
	char			*out_stream1 = NULL, *out_stream2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting Revenue Stream in the Input Flist
	 **/
	rev_stream1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_REVENUE_STREAM, 0, ebufp);

	/**
	 * Getting Service Type in the Input Flist
	 **/
	svc_type1 = PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SERVICE_TYPE, 0, ebufp);

	/**
	 * Getting EDR Status in the Input Flist
	 **/
	edr_status1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_EDR_STATUS, 0, ebufp);

	/**
	 * Getting Output Stream in the Input Flist
	 **/
	out_stream1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_OUTPUT_STREAM, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, 
		PIN_FLD_GROUP_DETAILS, &elem_id, 1, &cookie, ebufp)) != NULL)
	{

		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		rev_stream2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_REVENUE_STREAM, 0, ebufp);

		svc_type2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SERVICE_TYPE, 0, ebufp);

		edr_status2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_EDR_STATUS, 0, ebufp);

		out_stream2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_OUTPUT_STREAM, 0, ebufp);

		/**
		 * Check if an element with the same Revenue Stream already exists
		 **/
		if (rev_stream1 && rev_stream2 && 
			strcmp(rev_stream1, rev_stream2) == 0)
		{
			/**
			 * Check if an element with the same Service Type already exists
			 **/
			if (svc_type1 && svc_type2 && strcmp(svc_type1, svc_type2) == 0)
			{
				/**
				 * Check if an element with the same EDR Staus already exists
				 **/
				if (edr_status1 && edr_status2 && 
					strcmp(edr_status1, edr_status2) == 0)
				{
					/**
					 * Check if an element with the same Output Stream already 
					 * exists
					 **/
					if (out_stream1 && out_stream2 && 
						strcmp(out_stream1, out_stream2) == 0)
					{
						if (batch_type == BATCH_TYPE_MEDIATION)
						{
							PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
								"fm_process_audit_pol_create_and_link: "
								"duplicate entry found");

							pin_set_err(ebufp, PIN_ERRLOC_FM, 
								PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 
								0, 0, 0);

							break;
						}
						else
						{
							/**
							 * Check if an element with the same Original Batch 
							 * Id already exists
							 **/
							if (orig_batch_id1 && orig_batch_id2 && 
								strcmp(orig_batch_id1, orig_batch_id2) == 0)
							{
								if (batch_type == BATCH_TYPE_RERATING)
								{
									PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
										"fm_process_audit_pol_create_and_link: "
										"duplicate entry found");

									pin_set_err(ebufp, PIN_ERRLOC_FM, 
										PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 
										0, 0, 0);

									break;
								}
								else
								{
									if (suspended_from_batch_id1 && 
										suspended_from_batch_id2 && 
										strcmp(suspended_from_batch_id1, 
										suspended_from_batch_id2) == 0)
									{
										PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
											"fm_process_audit_pol_create_and_link: "
											"duplicate entry found");

										pin_set_err(ebufp, PIN_ERRLOC_FM, 
											PIN_ERRCLASS_APPLICATION, 
											PIN_ERR_DUPLICATE, 0, 0, 0);

										break;
									}
								}
							}
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}

/*******************************************************************
 * check_process_audit_outputstream:
 *******************************************************************/
static void
check_process_audit_outputstream(
	void			*vp,
	pin_flist_t		*result_array_flistp,
	int32			batch_type,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	int32			elem_id = -1;
	pin_flist_t		*group_details_flistp = NULL;

	char			*orig_batch_id1= NULL, *orig_batch_id2 = NULL;
	char			*suspended_from_batch_id1= NULL, *suspended_from_batch_id2 = NULL;
	char			*out_stream1 = NULL, *out_stream2 = NULL;

	/**
	 * Getting Original Batch Id in the Input Flist
	 **/
	orig_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

	/**
	 * Getting Suspended From Batch Id in the Input Flist
	 **/
	suspended_from_batch_id1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

	/**
	 * Getting Output Stream in the Input Flist
	 **/
	out_stream1 = (char *)PIN_FLIST_FLD_GET((pin_flist_t *)(vp), 
		PIN_FLD_OUTPUT_STREAM, 0, ebufp);

	/**
	 * Loop through all the Group_Details elements and match with 
	 * corresponding fields in the Input Flist
	 **/
	while ((group_details_flistp = 
		PIN_FLIST_ELEM_GET_NEXT(result_array_flistp, PIN_FLD_GROUP_DETAILS, 
		&elem_id, 1, &cookie, ebufp)) != NULL)
	{

		orig_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_ORIGINAL_BATCH_ID, 0, ebufp);

		suspended_from_batch_id2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

		out_stream2 = (char *)PIN_FLIST_FLD_GET(group_details_flistp, 
			PIN_FLD_OUTPUT_STREAM, 0, ebufp);

		/**
		 * Check if an element with the same Output Stream already exists
		 **/
		if (out_stream1 && out_stream2 && 
			strcmp(out_stream1, out_stream2) == 0)
		{
			if (batch_type == BATCH_TYPE_MEDIATION)
			{
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
					"fm_process_audit_pol_create_and_link: "
					"duplicate entry found");

				pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
					PIN_ERR_DUPLICATE, 0, 0, 0);

				break;
			}
			else
			{
				/**
				 * Check if an element with the same Original Batch Id 
				 * already exists
				 **/
				if (orig_batch_id1 && orig_batch_id2 && 
					strcmp(orig_batch_id1, orig_batch_id2) == 0)
				{
					if (batch_type == BATCH_TYPE_RERATING)
					{
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
							"fm_process_audit_pol_create_and_link: "
							"duplicate entry found");

						pin_set_err(ebufp, PIN_ERRLOC_FM, 
							PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE, 0, 0, 0);

						break;
					}
					else
					{
						if (suspended_from_batch_id1 && suspended_from_batch_id2 && 
							strcmp(suspended_from_batch_id1, suspended_from_batch_id2) == 0)
						{
							PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
								"fm_process_audit_pol_create_and_link: "
								"duplicate entry found");

							pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
								PIN_ERR_DUPLICATE, 0, 0, 0);

							break;
						}
					}
				}
			}
		}

		if (PIN_ERR_IS_ERR(ebufp))
			break;
	}
}
