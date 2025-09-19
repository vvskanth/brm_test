/* Continuus file information --- %full_filespec: fm_collections_pol_exec_policy_action.c~5:csrc:1 % */
/*
 * @(#) %full_filespec: fm_collections_pol_exec_policy_action.c~5:csrc:1 %
 *
 *      Copyright (c) 2001-2008 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "@(#) %full_filespec: fm_collections_pol_exec_policy_action.c~5:csrc:1 %";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#include "cm_fm.h"
#include "cm_cache.h"
#include "ops/collections.h"
#include "fm_utils.h"
#include "fm_collections.h"



#define FILE_LOGNAME "fm_collections_pol_exec_policy_action.c(1)"

/*******************************************************************
 * Routines defined here.
 *******************************************************************/

EXPORT_OP void
op_collections_pol_exec_policy_action(
	cm_nap_connection_t	*connp, 
	int32			opcode, 
	int32			flags, 
	pin_flist_t		*in_flistp, 
	pin_flist_t		**ret_flistpp, 
	pin_errbuf_t		*ebufp);

static void
fm_collections_pol_exec_policy_action(
	pcm_context_t		*ctxp, 
	int32			flags, 
	pin_flist_t		*in_flistp, 
	pin_flist_t		**ret_flistpp, 
	pin_errbuf_t		*ebufp);

/*******************************************************************
 * Variables/functions defined elsewhere
 *******************************************************************/
/*******************************************************************
 * Main routine for the PCM_OP_COLLECTIONS_POL_EXEC_POLICY_ACTION 
 *******************************************************************/
void
op_collections_pol_exec_policy_action(
	cm_nap_connection_t	*connp, 
	int32			opcode, 
	int32			flags, 
	pin_flist_t		*in_flistp, 
	pin_flist_t		**ret_flistpp, 
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_COLLECTIONS_POL_EXEC_POLICY_ACTION) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_collections_pol_exec_policy_action opcode error", 
			ebufp);
		return;
	}

	/***********************************************************
	 * Debug: What we got.
	 ***********************************************************/

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_collections_pol_exec_policy_action input flist", in_flistp);

	/***********************************************************
	 * Do the actual op in a sub. Copy it since we may need to
	 * replace it later.
	 ***********************************************************/

	fm_collections_pol_exec_policy_action(ctxp, flags, in_flistp, 
			ret_flistpp, ebufp);

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_collections_pol_exec_policy_action error", ebufp);
	} else {
		/***************************************************
		 * Debug: What we're sending back.
		 ***************************************************/
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_collections_pol_exec_policy_action return flist", 
			*ret_flistpp);
	}
	return;
}

/*******************************************************************
 * fm_collections_pol_exec_policy_action()
 * Default behaviour of this function is to set the action status to
 * PENDING and return.
 * This function can be customized to perform custom collections 
 * actions and update the action status to COMPLETED. 
 *******************************************************************/
static void
fm_collections_pol_exec_policy_action(
	pcm_context_t		*ctxp, 
	int32			flags, 
	pin_flist_t		*in_flistp, 
	pin_flist_t		**ret_flistpp, 
	pin_errbuf_t		*ebufp)
{
	pin_action_status_t	action_status = PIN_ACTION_PENDING;
	void			*vp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/* Prepare the return flist */
	*ret_flistpp = PIN_FLIST_CREATE(ebufp);
	vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*ret_flistpp, PIN_FLD_POID, vp, ebufp);

	/* Default behavior */
	/* do nothing, simply set the status to pending */
	PIN_FLIST_FLD_SET(*ret_flistpp, PIN_FLD_STATUS, 
			(void *)&action_status, ebufp);
}
