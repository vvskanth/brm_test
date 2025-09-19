/*******************************************************************
 *
 * Copyright (c) 2000, 2009, Oracle and/or its affiliates. 
 * All rights reserved. 
 *
 * This material is the confidential property of Oracle Corporation. or its 
 * subsidiaries or licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_trans_pol_open.c:BillingVelocityInt:1:2006-Aug-29 18:37:12 %";
#endif

#define PIN_FILE_SOURCE_ID  "fm_trans_pol_open.c(2)"

/*******************************************************************
 * Contains the PCM_OP_TRANS_POL_OPEN operation. 
 *******************************************************************/

#include <stdio.h>
#include <string.h>
 
#include "pcm.h"
#include "ops/base.h"
#include "pin_bill.h"
#include "pin_cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "fm_utils.h"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_trans_pol_open(
	cm_nap_connection_t*    connp,
	u_int                   opcode,
	u_int                   flags,
	pin_flist_t*            i_flistp,
	pin_flist_t**           o_flistpp,
	pin_errbuf_t*           ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_TRANS_POL_OPEN operation.
 *******************************************************************/
void
op_trans_pol_open(
	cm_nap_connection_t*    connp,
	u_int                   opcode,
	u_int                   flags,
	pin_flist_t*            i_flistp,
	pin_flist_t**           o_flistpp,
	pin_errbuf_t*           ebufp)
{
	pcm_context_t*          ctxp = connp->dm_ctx;
	pin_errbuf_t            my_ebuf;
	int*                    int_p;
	int                     error_num;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_TRANS_POL_OPEN) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_trans_pol_open opcode error", ebufp);
		return;
	}

	/*
	 * Debug: What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_trans_pol_open input flist", i_flistp);

	/*
	 * Do the actual op.
	 */

	/*
	 * NOTE: The following call is for EAI framework.  Don't delete.
	 * Set the transaction flag if publish is enabled
	 */
	fm_utils_publish_transaction_state(connp, opcode, i_flistp, ebufp);

	/*
	 * NOTE: The following call is for service order provisioning.
	 * Don't delete. Set transaction flag.
	 */
	fm_utils_prov_txn_op(connp, opcode, i_flistp, ebufp);

	/*
	 * For now, just pass along a copy of the input flist
	 * (ie, it will continue to be the current transaction flist).
	 */
	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);

	/* See if the returned FLIST has PIN_FLD_ERROR_NUM in it*/
	PIN_ERR_CLEAR_ERR(&my_ebuf);

	int_p = (int*)PIN_FLIST_FLD_GET((pin_flist_t*)i_flistp,
		PIN_FLD_ERROR_NUM, 1, &my_ebuf);
	if ((my_ebuf.pin_err == PIN_ERR_NONE) && (int_p != NULL)) {
		error_num = *int_p;
	}
	else {
		error_num = 0;
	}
	if (error_num == PIN_ERR_PERF_LIMIT_REACHED) {
		/*
		 * Now decide if we are going to clear the error or not by
		 * checking the pin.conf options. If we want the error cleared
		 * then we just drop PIN_FLD_ERROR_NUM from the returned flist.
		 * Otherwise we leave it alone (which means it will be treated
		 * as an error). Then we let the PCM_OP_TRANS_OPEN code handle it.
		 */
		if (cm_reject_on_overload() == 0) {
			/* Clear it */
			PIN_FLIST_FLD_DROP(*o_flistpp, PIN_FLD_ERROR_NUM, &my_ebuf);
			/* Ignore an ebuf error from the PIN_FLIST_FLD_DROP */

			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
				"op_trans_pol_open: cleared perf_limit_reached_error");
		}
	}

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_trans_pol_open error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_trans_pol_open return flist", *o_flistpp);
	}

	return;
}

