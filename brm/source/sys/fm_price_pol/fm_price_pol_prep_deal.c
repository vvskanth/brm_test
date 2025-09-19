/* continuus file information --- %full_filespec: fm_price_pol_prep_deal.c~5:csrc:1 % */
/*******************************************************************
 *
 *  @(#) %full_filespec: fm_price_pol_prep_deal.c~5:csrc:1 %
 *
 *      Copyright (c) 1999 - 2024  Oracle.
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
 ** This policy opcode can be used to add additional data to price
 ** objects that is not provided by either the GUI application or
 ** or by fm_price.  This policy opcode is called BEFORE fm_price
 ** performs any built in validation checks.  Therefore, everything
 ** you do here will be validated.
 **
 ******************************************************************
 ******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_price_pol_prep_deal.c:ServerIDCVelocityInt:1:2006-Sep-06 16:46:45 %";
#endif

#include <stdio.h>
#include "pcm.h"
#include "ops/price.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "pin_price.h"
#include "pin_bill.h"

#define FILE_SOURCE_ID	"fm_price_pol_prep_deal.c (1.8)"

/*******************************************************************
 * Forward declarations
 *******************************************************************/
void fm_price_pol_prep_deal(
	pcm_context_t		*ctxp,
	u_int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistp,
        pin_errbuf_t		*ebufp);
 
/*******************************************************************
 * Main routine for the PCM_OP_PRICE_POL_PREP_DEAL  command
 *******************************************************************/
EXPORT_OP void
op_price_pol_prep_deal(
        cm_nap_connection_t	*connp,
	u_int32			opcode,
        u_int32			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*r_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Null out results until we have some.
	 */
	*o_flistpp = NULL;

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_PRICE_POL_PREP_DEAL) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_price_pol_prep_deal", ebufp);
		return;
	}

	/*
	 * Do the actual prep in a lower routine
	 */
	fm_price_pol_prep_deal(ctxp, flags, i_flistp, &r_flistp, ebufp);

	/*
	 * Results.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		*o_flistpp = (pin_flist_t *)NULL;
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_price_pol_prep_deal error", ebufp);
	} else {
		*o_flistpp = r_flistp;
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_price_pol_prep_deal return flist", r_flistp);
	}

	return;
}

/*******************************************************************
 * fm_price_pol_prep_deal():
 *
 *	Prepare the given deal flist for database insertion.
 *
 *	XXX NOOP - STUBBED PROTOTYPE ONLY XXX
 *
 *******************************************************************/
void
fm_price_pol_prep_deal(
	pcm_context_t		*ctxp,
	u_int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Copy the input to output
	 */

	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);


	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_price_pol_prep_deal error", ebufp);
	}

	return;
}
