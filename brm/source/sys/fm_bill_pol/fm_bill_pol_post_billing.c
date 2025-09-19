/*
 *
* Copyright (c) 2002, 2016, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */
#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_bill_pol_post_billing.c:BillingVelocityInt:3:2006-Sep-05 21:55:03 %";
#endif

#pragma GCC diagnostic ignored "-Wunused-variable"

#include <stdio.h> 
#include <string.h> 
#include <time.h> 
#include "pcm.h"
#include "ops/bill.h"
#include "ops/cust.h"
#include "pin_bill.h"
#include "pin_pymt.h"
#include "pin_cust.h"
#include "fm_utils.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "fm_bill_utils.h"
#include "psiu_business_params.h"
#include "fm_utils_bparams_cache.h"

#define FILE_LOGNAME "fm_bill_pol_post_billing.c(3)"

/*******************************************************************
 * Contains the PCM_OP_BILL_POL_POST_BILLING operation. 
 *
 * This policy provides a hook at the end of the billing process for 
 * the single account. The default implementation contains the call
 * to the PCM_OP_BILL_SUSPEND_BILLING opcode in order to stop billing 
 * for the closed account.
 *******************************************************************/

EXPORT_OP void
op_bill_pol_post_billing(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_bill_pol_stop_billing();

/*******************************************************************
 * Main routine for the PCM_OP_BILL_POL_POST_BILLING
 *******************************************************************/
void
op_bill_pol_post_billing(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t           *ctxp = connp->dm_ctx;


	*r_flistpp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*******************************************************************
	 * Insanity check.
	 *******************************************************************/
	if (opcode != PCM_OP_BILL_POL_POST_BILLING) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_bill_pol_post_billing bad opcode error", 
			ebufp);
		return;
	}

	/*******************************************************************
	 * Debug: What we got.
	 *******************************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_bill_pol_post_billing input flist", i_flistp);


	/*******************************************************************
	 * Call the default implementation in order to stop billing of the 
	 * closed accounts
	 *******************************************************************/
	fm_bill_pol_stop_billing(ctxp, flags, i_flistp, r_flistpp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"op_bill_pol_post_billing error", ebufp);
	} else {

		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_bill_pol_post_billing output flist", *r_flistpp);
	}
	return;
}

/*******************************************************************
 * fm_bill_pol_stop_billing():
 *
 * This checks the following conditions:
 * 1) The stop_bill_closed_accounts stanza from the pin.conf file (for CM)
 *    should not be zero
 * 2) Account has a closed status
 * 3) The billing is provided for the last (final) billing state
 * 4) Total due for open and pending items is zero
 * 5) This is a last accounting cycle in the multi-month billing cycle
 *
 * If all these conditions are satisfied then the function calls the
 * PCM_OP_BILL_SUSPEND_BILLING opcode to stop billing.
 *******************************************************************/
static void
fm_bill_pol_stop_billing(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*i_pol_flistp = NULL;
	pin_flist_t		*o_pol_flistp = NULL;
	void			*vp = NULL;
	pin_status_t		*statusp = NULL;
	int32			*cycles_leftp = NULL;
        int32			*bill_whenp = NULL;
        int32			*bill_typep = NULL;
        int32			f = PIN_BILL_FLAGS_DUE_TO_STOP_BILL_CLOSE_ACCT;
	int32		        config_stop_bill_closed_accounts = 0;
	pin_bill_billing_state_t *billing_statep = NULL;
	pin_decimal_t		*due = NULL;
	pin_decimal_t		*due_tot = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	*r_flistpp = PIN_FLIST_CREATE(ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, vp, ebufp);

	/***********************************************************
	 * Check the conditions to stop billing
	************************************************************/
	config_stop_bill_closed_accounts = fm_utils_bparams_cache_get_int(ctxp,
		PSIU_BPARAMS_BILLING_PARAMS, PSIU_BPARAMS_STOP_BILL_CLOSED_ACCOUNTS,
			0, ebufp);

	if (!config_stop_bill_closed_accounts) {
		goto cleanup;
	}

	billing_statep = (pin_bill_billing_state_t *)PIN_FLIST_FLD_GET(i_flistp,
					PIN_FLD_BILLING_STATE, 0, ebufp);
	if (billing_statep && (*billing_statep != PIN_ACTG_CYCLE_OPEN)) {
		goto cleanup;
	}

	statusp = (pin_status_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_STATUS,
							0, ebufp);
	if (statusp && (*statusp != PIN_STATUS_CLOSED)) {
		goto cleanup;
	}

	due_tot = pin_decimal ("0.0", ebufp);
	due = (pin_decimal_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_OPEN_BAL,
							1, ebufp);
	if (due) {
		pin_decimal_add_assign(due_tot, due, ebufp);
	}
	due = (pin_decimal_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_PENDING_BAL,
							1, ebufp);
	if (due) {
		pin_decimal_add_assign(due_tot, due, ebufp);
	}
	if (!pbo_decimal_is_zero(due_tot, ebufp)) {
		goto cleanup;
	}

	bill_whenp = (int32 *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_BILL_WHEN,
					0, ebufp);
	cycles_leftp = (int32 *)PIN_FLIST_FLD_GET(i_flistp, 
					PIN_FLD_BILL_ACTGCYCLES_LEFT, 0, ebufp);
	if (!bill_whenp || !cycles_leftp) {
			goto cleanup;
	}
	if (*bill_whenp != *cycles_leftp) {
			goto cleanup;
	}

	bill_typep = (int32 *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_PAY_TYPE,
		 1, ebufp);

	if (bill_typep && *bill_typep == PIN_PAY_TYPE_SUBORD) {
		f = PIN_BILL_FLAGS_DUE_TO_STOP_BILL_CLOSE_ACCT_SUBORD;
	} 	

	/***********************************************************
	 * Call the PCM_OP_BILL_SUSPEND_BILLING opcode to stop billing
	************************************************************/
	i_pol_flistp = PIN_FLIST_CREATE(ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(i_pol_flistp, PIN_FLD_POID, vp, ebufp);
	PIN_FLIST_FLD_SET(i_pol_flistp, PIN_FLD_BILLING_STATUS_FLAGS, 
				(void *)&f, ebufp);
	PCM_OP(ctxp, PCM_OP_BILL_SUSPEND_BILLING, flags, i_pol_flistp, 
				&o_pol_flistp, ebufp);

cleanup:

	PIN_FLIST_DESTROY_EX(&i_pol_flistp, ebufp);
	PIN_FLIST_DESTROY_EX(&o_pol_flistp, ebufp);
	PIN_DECIMAL_DESTROY_EX(&due_tot);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
		"fm_bill_pol_stop_billing", ebufp);
        }

	return;
}
