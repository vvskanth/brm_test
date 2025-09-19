/*******************************************************************
 *
 *  fm_price_pol_delete_transition.c
 *
 *      Copyright (c) 2003 - 2006  Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its subsidiaries or licensors and may be used, reproduced, stored
 *      or transmitted only in accordance with a valid Oracle license or
 *      sublicense agreement.
 *
 *******************************************************************/

/******************************************************************
 ******************************************************************
 **
 ** This policy opcode can be used to perform validations over and
 ** above those which will be performed by fm_price
 **
 ** By default it just passes 
 **
 ******************************************************************
 ******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_price_pol_delete_transition.c:ServerIDCVelocityInt:1:2006-Sep-06 16:37:26 %";
#endif

/************************************************************************
 * This policy determines whether or not the requested transition
 * object may be safely deleted from the database or not.
 *
 * XXX NOOP - STUBBED PROTOTYPE ONLY - ALWAYS PASSES XXX
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "pcm.h"
#include "ops/price.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"

#include "pin_price.h"

#define FILE_SOURCE_ID  "fm_price_pol_delete_transition.c (1.7)"
 
/*******************************************************************
 * Main routine for the PCM_OP_PRICE_POL_DELETE_TRANSITION operation.
 *******************************************************************/
EXPORT_OP void
op_price_pol_delete_transition(
        cm_nap_connection_t	*connp,
	u_int32			opcode,
        u_int32			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**r_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	u_int32			result;
	void			*vp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Null out results until we have some.
	 */
	*r_flistpp = NULL;

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_PRICE_POL_DELETE_TRANSITION) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_price_pol_delete_transition", ebufp);
		return;
	}

	/*
	 * Debug - What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_price_pol_delete_transition input flist", i_flistp);

	/*
	 * Setup the result flist
	 */
	*r_flistpp = PIN_FLIST_CREATE(ebufp);

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, vp, ebufp);

	result = PIN_PRICE_VERIFY_PASSED;
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_RESULT, (void *)&result, ebufp);

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_DEBUG,
			"op_price_pol_delete_transition error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_price_pol_delete_transition return flist", *r_flistpp);
	}

	return;
}

