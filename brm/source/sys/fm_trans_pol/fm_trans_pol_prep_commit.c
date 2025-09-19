/* continuus file information --- %full_filespec: fm_trans_pol_prep_commit.c~8:csrc:1 % */
/*******************************************************************
 *
 *  @(#) %full_filespec: fm_trans_pol_prep_commit.c~8:csrc:1 %
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
static  char    Sccs_id[] = "@(#)%Portal Version: fm_trans_pol_prep_commit.c:BillingApolloInt:1:2004-Oct-05 10:44:39 %";
#endif

/*******************************************************************
 * Contains the PCM_OP_TRANS_POL_PREP_COMMIT operation. 
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

#define FILE_SOURCE_ID  "fm_trans_pol_prep_commit.c (1)"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_trans_pol_prep_commit(
	cm_nap_connection_t	*connp,
	u_int			opcode,
	u_int			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

void fm_trans_pol_create_uniquenss (
	pcm_context_t *ctxp,
	pin_errbuf_t *ebufp );

/*******************************************************************
 * Main routine for the PCM_OP_TRANS_POL_PREP_COMMIT operation.
 *******************************************************************/
void
op_trans_pol_prep_commit(
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
	if (opcode != PCM_OP_TRANS_POL_PREP_COMMIT) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_trans_pol_prep_commit opcode error", ebufp);
		return;
	}

	/*
	 * Debug: What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			  "op_trans_pol_prep_commit input flist", i_flistp);

	/*
	 * Do the actual op.
	 */

	/*
	 * NOTE: The following call is for EAI framework.  Don't delete.
	 * Send prep commit to publisher if publish is enabled.
	 */
	fm_utils_publish_transaction_state(connp, opcode, i_flistp, ebufp);

	/*
	 * NOTE: The following call is for service order provisioning.
	 * Don't delete. Send prep. commit to publisher.
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
			"op_trans_pol_prep_commit error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_trans_pol_prep_commit return flist", *o_flistpp);
	}

	fm_trans_pol_create_uniquenss(ctxp, ebufp);

	return;
}

void fm_trans_pol_create_uniquenss (
	pcm_context_t *ctxp,
	pin_errbuf_t *ebufp ) 
{
	pin_flist_t	*uniq_flistp = NULL;
	pin_flist_t	*uniq_flistp_copy = NULL;
	pin_flist_t	*r_flistp = NULL;
	pin_flist_t	*trans_flistp = NULL;
	pin_flist_t	*opcodes_flistp = NULL;
	int32		cred = 0;
	pin_rec_id_t	elem_id = 0;
	pin_cookie_t	cookie = NULL;
	void		*vp;
	int32		opcode = 0;
	int32		opcode_flag = 0;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	trans_flistp = CM_FM_GET_NAMED_TRANS_FLIST(ctxp,
					PIN_TRANS_NAME_PREP_COMMIT);
	if ( trans_flistp ) {

		while ( (uniq_flistp = PIN_FLIST_ELEM_GET_NEXT( trans_flistp, 
			PIN_FLD_ACTION_INFO, &elem_id, 1, &cookie, ebufp)) != NULL ) {

			uniq_flistp_copy = PIN_FLIST_COPY(uniq_flistp, ebufp);

			/*************************************************
			* Loop through the transition flist and fetch
			* uniqueness object set in PCM_OP_CUST_SET_LOGIN. 
			**************************************************/

			opcodes_flistp = PIN_FLIST_ELEM_GET(uniq_flistp_copy, PIN_FLD_OPCODES, 0, 0, ebufp);
			vp = PIN_FLIST_FLD_GET(opcodes_flistp, PIN_FLD_OPCODE, 0, ebufp);
			if(vp) {
				opcode = *(int32 *)vp;
			}

			vp = PIN_FLIST_FLD_GET(opcodes_flistp, PIN_FLD_FLAGS, 0, ebufp);
			if (vp) {
				opcode_flag = *(int32 *)vp;
			}

			PIN_FLIST_ELEM_DROP(uniq_flistp_copy, PIN_FLD_OPCODES, 0, ebufp);

			cred = CM_FM_BEGIN_OVERRIDE_SCOPE( CM_CRED_SCOPE_OVERRIDE_ROOT);
			PCM_OP(ctxp, opcode, opcode_flag, uniq_flistp_copy, &r_flistp, ebufp);
			CM_FM_END_OVERRIDE_SCOPE(cred);

			PIN_FLIST_DESTROY_EX( &r_flistp, NULL );
			PIN_FLIST_DESTROY_EX( &uniq_flistp_copy, NULL );
        		
    		}
		// Destroy the transition flist
		CM_FM_DROP_NAMED_TRANS_FLIST(ctxp, PIN_TRANS_NAME_PREP_COMMIT, ebufp);
	}
	
	if ( PIN_ERR_IS_ERR(ebufp) ) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_trans_pol_create_uniquenss error", ebufp);
	} 

	return;
}