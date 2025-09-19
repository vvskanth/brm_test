/*******************************************************************
 *
 *  @(#) %full_filespec: fm_repl_pol_push.c~2.1.7:csrc:1 %
 *
* Copyright (c) 1999, 2015, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or
 *      its licensors and may be used, reproduced, stored or transmitted only
 *      in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static	char	Sccs_id[] = "@(#)%Portal Version: fm_repl_pol_push.c:PortalBase7.3.1Int:1:2007-Aug-28 22:57:42 %";
#endif

#define FILE_SOURCE_ID "fm_repl_pol_push.c(2)"

/*******************************************************************
 * This file contains the PCM_OP_REPL_POL_PUSH operation
 * Also included are subroutines specific to the operation.
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "pcm.h"
#include "ops/repl.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_channel.h"
#include "pinlog.h"
#include "fm_utils.h"
#include "pin_cust.h"
#include "fm_repl_pol.h"
#include "string.h"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
static void
fm_repl_pol_push_account(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_repl_pol_push_service(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static int
impacts_account(
	poid_t *a_pdp);
static int
impacts_service(
	poid_t *a_pdp);

static void
fm_repl_pol_push_log_error(
	char		*desc,
	poid_t		*poid,
	pin_errbuf_t	*ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_REPL_POL_DELETE_OBJ operation.
 *******************************************************************/
PIN_EXPORT void
op_repl_pol_push(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t	*ctxp 		= connp->dm_ctx;
	poid_t		*consumer_pdp 	= NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Null out results
	 */
	*r_flistpp = NULL;

	/*
	 * Insanity check
	 */
	if (opcode != PCM_OP_REPL_POL_PUSH) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			    PIN_ERRCLASS_SYSTEM_DETERMINATE,
			    PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "incorrect opcode in op_repl_pol_push",
				 ebufp);
		return;
	}

	/*
	 * Debug: What did we get?
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			  "op_repl_pol_push input flist",
			  i_flistp);

	/*
	 * Get the channel consumer poid. In other words, figure out which
	 * remote object needs to be updated.
	 */
	consumer_pdp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID,
						   0, ebufp);
	if (PIN_ERR_IS_ERR(ebufp)) {
		goto cleanup;
	}

	if (fm_utils_type_equal(consumer_pdp, "/r_account") == 1) {
		fm_repl_pol_push_account(ctxp, flags, i_flistp, r_flistpp,
					 ebufp);

	} else if (fm_utils_type_equal(consumer_pdp, "/r_service") == 1) {
		fm_repl_pol_push_service(ctxp, flags, i_flistp, r_flistpp,
					 ebufp);

	} else {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			    PIN_ERRCLASS_SYSTEM_DETERMINATE,
			    PIN_ERR_BAD_ARG, PIN_FLD_POID, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "incorrect consumer object type in "
				 "op_repl_pol_push",
				 ebufp);
		goto cleanup;
	}

	if (*r_flistpp == NULL) {
		*r_flistpp = PIN_FLIST_CREATE(ebufp);
		PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, consumer_pdp,
				  ebufp);
	}

cleanup:
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "op_repl_pol_object_create flistp error",
				 ebufp);
	}

	return;
}

/*
 * fm_repl_pol_push_account():
 * push new and modified account info to the DS
 */
static void
fm_repl_pol_push_account(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t	*s_flistp 	= NULL;
	pin_flist_t	*res_flistp	= NULL;
	poid_t		*a_pdp		= NULL;
	poid_t		*cons_pdp	= NULL;
	poid_t		*supplier_pdp 	= NULL;
	int			element_id	= 0;
	int64		ch_id		= 0;
	pin_cookie_t	cookie		= NULL;
	int32		acct_creation	= 0;
	int32		acct_modification = 0;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	a_pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_SOURCE_OBJ, 0, ebufp);
        cons_pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_CONSUMER_OBJ, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/*****/
	}


	/*
	 * Push out the information from this new/modofied account to the
	 * LDAP Directory Server.
	 */

	/*
	 * Go through the list of suppliers, and check if "/account" exist.
	 */
	cookie = NULL;
	element_id = 0;
	while ((s_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
		   PIN_FLD_SUPPLIERS, &element_id, 1, &cookie,
		   ebufp)) != NULL) {

		supplier_pdp = (poid_t *)PIN_FLIST_FLD_GET(s_flistp,
					PIN_FLD_SUPPLIER_OBJ, 0, ebufp);
		if (fm_utils_type_equal(supplier_pdp, "/account") == 1) {
			acct_creation = 1;
			break;
		}
		else  {
			/*
			 * Make sure the supplier is of valid type
			 */
			if (impacts_account(supplier_pdp) != 1) {
				continue;
			}

			/*
			 * The supplier has to be a subtype of /event
			 * since we are search in the event table
			 */
			if (fm_utils_is_subtype(supplier_pdp, "/event") == 0) {
				continue;
			}

			/*
			 * Found a valid supplier type
			 */
			acct_modification = 1;
			break;
		}
	}
	/*
	 * Just ignore errors
	 * and continue with the rest.
	 */
	if (acct_creation) {
		fm_repl_pol_create_obj(ctxp, flags, a_pdp, cons_pdp, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			fm_repl_pol_push_log_error(
				"Error pushing new account",
				a_pdp,ebufp);

		}
	}
	else if (acct_modification) {
		fm_repl_pol_write_flds(ctxp, flags, a_pdp, cons_pdp, ebufp);

		/*
		 * Just ignore errors
		 * and continue with the rest.
		 */
		if (PIN_ERR_IS_ERR(ebufp)) {
			fm_repl_pol_push_log_error(
				"Error pushing account",
				a_pdp,ebufp);

		}
	}
	else {
		/* no work to do */
		return;
	}

	return;
}

/*
 * fm_repl_pol_push_service():
 * Push new and modified service info to the DS
 */
static void
fm_repl_pol_push_service(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t	*s_flistp 	= NULL;
	pin_flist_t	*res_flistp	= NULL;
	poid_t      *cons_pdp = NULL;
	poid_t		*s_pdp		= NULL;
	poid_t		*supplier_pdp 	= NULL;
	int			element_id	= 0;
	int64		ch_id	= 0;
	pin_cookie_t	cookie		= NULL;
	int32		srvc_created 	= 0;
	int32		srvc_modified 	= 0;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	*r_flistpp = NULL;

	s_pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_SOURCE_OBJ, 0, ebufp);
        cons_pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_CONSUMER_OBJ, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/*****/
	}


	/*
	 * Push out the information from this new service to the
	 * LDAP Directory Server.
	 */

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "Push Service:",
			  i_flistp);
	/*
	 * Go through the list of suppliers, and check if "/service" exist.
	 */
	cookie = NULL;
	element_id = 0;
	while ((s_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_SUPPLIERS,
					      &element_id, 1, &cookie,
					      ebufp)) != NULL) {
		supplier_pdp = (poid_t *)PIN_FLIST_FLD_GET(s_flistp,
					PIN_FLD_SUPPLIER_OBJ, 0, ebufp);

		if (fm_utils_type_equal(supplier_pdp, "/service") == 1) {
			srvc_created = 1;
			break;
		}
		else {
			/*
			 * Make sure the supplier is of valid type
			 */
			if (impacts_service(supplier_pdp)) {
				srvc_modified = 1;
				break;
			}
		}
	}
	if (srvc_created) {
		if (fm_repl_pol_get_user_scheme() == 0) {
			fm_repl_pol_create_obj(ctxp, flags, s_pdp, cons_pdp, ebufp);
		} else {
			fm_repl_pol_write_flds(ctxp, flags, s_pdp, cons_pdp, ebufp);
		}
		/*
	 	* Just ignore errors from fm_repl_pol_{create_obj,write_flds}
		 * and continue with the rest.
		 */
		if (PIN_ERR_IS_ERR(ebufp)) {
			fm_repl_pol_push_log_error(
				"Error pushing new service",
				s_pdp,ebufp);

		}
	}
	else if (srvc_modified) {

		fm_repl_pol_write_flds(ctxp, flags, s_pdp, cons_pdp, ebufp);

		/*
		 * Just ignore errors from fm_repl_pol_write_flds()
		 * and continue with the rest.
		 */
		if (PIN_ERR_IS_ERR(ebufp)) {
			fm_repl_pol_push_log_error(
				"Error pushing service",
				s_pdp,ebufp);

		}
	}
	else {
		/* No work to do */
		return;
	}


	return;
}



/*
 * Does the supplier type indicate that we need to
 * update the account fields?
 */
static int
impacts_account(
	poid_t *a_pdp)
{
	return (fm_utils_is_subtype(a_pdp, "/event/customer/billinfo")	||
		fm_utils_is_subtype(a_pdp, "/event/customer/nameinfo")	||
		fm_utils_is_subtype(a_pdp, "/event/customer/status")	||
		fm_utils_is_subtype(a_pdp, "/event/customer/product_status"));
}

/*
 * Does the supplier type indicate that we need to
 * update the service fields?
 */
static int
impacts_service(
	poid_t *a_pdp)
{
	return (fm_utils_is_subtype(a_pdp, "/event/customer/password")	||
		fm_utils_is_subtype(a_pdp, "/event/customer/login") 	||
		fm_utils_is_subtype(a_pdp, "/event/customer/status") 	||
                fm_utils_is_subtype(a_pdp, "/event/billing/product")    ||
		fm_utils_is_subtype(a_pdp, "/event/notification/service"));
}

static void
fm_repl_pol_push_log_error(
	char		*desc,
	poid_t		*poid,
	pin_errbuf_t	*ebufp)
{
	char	msg[1024];
	char	poidstr[512];
	char	*strp = poidstr;
	int32	len = sizeof(poidstr);
	pin_errbuf_t lebuf;

	PIN_ERR_CLEAR_ERR(&lebuf);
	poidstr[0] = '\0';
	PIN_POID_TO_STR(poid, &strp, &len, &lebuf);

	pin_snprintf(msg, sizeof(msg) , "%s, poid=\"%s\"",
		desc, poidstr);

	PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, msg, ebufp);
}

