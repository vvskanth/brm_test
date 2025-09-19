/*
 *      Copyright (c) 2001-2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "@(#) %full_filespec: fm_collections_pol_select_profile.c~2:csrc:1 %";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#include "cm_fm.h"
#include "cm_cache.h"
#include "ops/collections.h"
#include "fm_utils.h"
#include "pin_cust.h"
#include "fm_collections.h"


/*******************************************************************
 * Routines defined here.
 *******************************************************************/

EXPORT_OP void
op_collections_pol_select_profile(
	cm_nap_connection_t	*connp, 
	int32			opcode, 
	int32			flags, 
	pin_flist_t		*in_flistp, 
	pin_flist_t		**ret_flistpp, 
	pin_errbuf_t		*ebufp);

static void
fm_collections_pol_select_profile(
	pcm_context_t		*ctxp, 
	int32			flags, 
	pin_flist_t		*in_flistp, 
	pin_flist_t		**ret_flistpp, 
	pin_errbuf_t		*ebufp);
/*******************************************************************
 * Variables/functions defined elsewhere
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_COLLECTIONS_POL_SELECT_PROFILE 
 *******************************************************************/
void
op_collections_pol_select_profile(
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
	if (opcode != PCM_OP_COLLECTIONS_POL_SELECT_PROFILE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_collections_pol_select_profile opcode error", 
			ebufp);
		return;
	}

	/***********************************************************
	 * Debug: What we got.
	 ***********************************************************/

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_collections_pol_select_profile input flist", in_flistp);

	/***********************************************************
	 * Do the actual op in a sub. Copy it since we may need to
	 * replace it later.
	 ***********************************************************/

	fm_collections_pol_select_profile(ctxp, flags, in_flistp, 
			ret_flistpp, ebufp);

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_collections_pol_select_profile error", ebufp);
	} else {
		/***************************************************
		 * Debug: What we're sending back.
		 ***************************************************/
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_collections_pol_select_profile return flist", 
			*ret_flistpp);
	}
	return;
}

/*******************************************************************
 * fm_collections_pol_select_profile()
 *
 * This function calls corresponding action opcode and updates status.
 *
 *******************************************************************/
static void
fm_collections_pol_select_profile(
	pcm_context_t		*ctxp, 
	int32			flags, 
	pin_flist_t		*in_flistp, 
	pin_flist_t		**ret_flistpp, 
	pin_errbuf_t		*ebufp)
{
	poid_t			*a_pdp = NULL;
	poid_t			*b_pdp = NULL;
	poid_t			*p_pdp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/* Customers should change this policy to return a config profile */
	/* pointer for a given account.  One should define several profiles */
	/* from the configuration center and map the given account to */
	/* one of the profiles based on account related information. */
	/* By default, this policy returns the default profile. */
	*ret_flistpp = PIN_FLIST_CREATE(ebufp);

	a_pdp = (poid_t *)PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID, 0, ebufp);
	b_pdp = (poid_t *)PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_BILLINFO_OBJ, 0, ebufp);

	PIN_FLIST_FLD_SET(*ret_flistpp, 
				PIN_FLD_POID, a_pdp, ebufp);

	PIN_FLIST_FLD_SET(*ret_flistpp, 
				PIN_FLD_BILLINFO_OBJ, b_pdp, ebufp);

	p_pdp = PIN_POID_CREATE(PIN_POID_GET_DB(a_pdp), 
			"/config/collections/profile/default", 
			801, ebufp);

	PIN_FLIST_FLD_PUT(*ret_flistpp, 
				PIN_FLD_CONFIG_PROFILE_OBJ, p_pdp, ebufp);
}
