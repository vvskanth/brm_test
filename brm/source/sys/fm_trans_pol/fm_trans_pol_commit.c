/* continuus file information --- %full_filespec: fm_trans_pol_commit.c~8:csrc:1 % */
/*******************************************************************
 *
 *  @(#) %full_filespec: fm_trans_pol_commit.c~8:csrc:1 %
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
static  char    Sccs_id[] = "@(#)%Portal Version: fm_trans_pol_commit.c:BillingApolloInt:1:2004-Oct-05 10:44:45 %";
#endif

/*******************************************************************
 * Contains the PCM_OP_TRANS_POL_COMMIT operation. 
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
#include "pin_channel.h"
#include "fm_utils.h"

#define FILE_SOURCE_ID  "fm_trans_pol_commit.c (1.8)"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_trans_pol_commit(
	cm_nap_connection_t	*connp,
	u_int			opcode,
	u_int			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_TRANS_POL_COMMIT operation.
 *******************************************************************/
void
op_trans_pol_commit(
	cm_nap_connection_t	*connp,
	u_int			opcode,
	u_int			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;


	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_TRANS_POL_COMMIT) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_trans_pol_commit opcode error", ebufp);
		return;
	}

	/*
	 * Debug: What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_trans_pol_commit input flist", i_flistp);

	/*
	 * Do the actual op.
	 */

	/*
	 * NOTE: The following call is for EAI framework.  Don't delete.
	 * Send commit to publisher if publish is enabled.
	 */
	fm_utils_publish_transaction_state(connp, opcode, i_flistp, ebufp);

	/*
	 * NOTE: The following call is for service order provisioning.
	 * Don't delete. Send commit to publisher.
	 */
	fm_utils_prov_txn_op(connp, opcode, i_flistp, ebufp);


	/*
	 * For now, just pass along a copy of the input flist
	 * (ie, it will continue to be the current transaction flist).
	 */
	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_trans_pol_commit error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_trans_pol_commit return flist", *o_flistpp);
	}

}

