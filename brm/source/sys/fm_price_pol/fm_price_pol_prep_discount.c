/* continuus file information --- %full_filespec: fm_price_pol_prep_discount.c~7:csrc:1 % */
/*******************************************************************
 *
 *  @(#) %full_filespec: fm_price_pol_prep_discount.c~7:csrc:1 %
 *
 *      Copyright (c) 2003 - 2024  Oracle.
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
static  char    Sccs_id[] = "@(#)%Portal Version: fm_price_pol_prep_discount.c:ServerIDCVelocityInt:1:2006-Sep-06 16:35:26 %";
#endif

#include <stdio.h>
#include "pcm.h"
#include "ops/price.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "pin_bill.h"
#include "pin_price.h"

#define FILE_SOURCE_ID  "fm_price_pol_prep_discount.c (7)"

/*******************************************************************
 * Forward declarations
 *******************************************************************/
void fm_price_pol_prep_discount(
	pcm_context_t		*ctxp,
	u_int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistp,
        pin_errbuf_t		*ebufp);
 
/*******************************************************************
 * Main routine for the PCM_OP_PRICE_POL_PREP_DISCOUNT  command
 *******************************************************************/
EXPORT_OP void
op_price_pol_prep_discount(
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
	if (opcode != PCM_OP_PRICE_POL_PREP_DISCOUNT) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_price_pol_prep_discount", ebufp);
		return;
	}

	/*
	 * Do the actual prep in a lower routine
	 */
	fm_price_pol_prep_discount(ctxp, flags, i_flistp, &r_flistp, ebufp);

	/*
	 * Results.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		*o_flistpp = (pin_flist_t *)NULL;
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_price_pol_prep_discount error", ebufp);
	} else {
		*o_flistpp = r_flistp;
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_price_pol_prep_discount return flist", r_flistp);
	}

	return;
}

/*******************************************************************
 * fm_price_pol_prep_discount():
 *
 *	Prepare the given discount flist for database insertion.
 *
 *	XXX NOOP - STUBBED PROTOTYPE ONLY XXX
 *
 *******************************************************************/
void
fm_price_pol_prep_discount(
	pcm_context_t		*ctxp,
	u_int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	pin_flist_t		*d_flistp = NULL;
	void                    *vp = NULL;

	double			num_min = PIN_NUM_MIN;
	double			num_max = PIN_NUM_MAX;
        time_t			zero_t = 0;
	u_int32			dummy = 0;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * copy input flist to output
	 */
	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_price_pol_prep_discount error", ebufp);
	}

	return;
}
