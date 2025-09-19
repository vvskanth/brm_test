/*
 *
 *      Copyright (c) 1999 - 2024 Oracle.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_pymt_pol_under_payment.c:BillingVelocityInt:2:2006-Sep-05 21:54:09 %";
#endif

/*******************************************************************
 * Contains the PCM_OP_PYMT_POL_UNDER_PAYMENT operation. 
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 
 
#include "pcm.h"
#include "ops/pymt.h"
#include "pin_pymt.h"
#include "pin_bill.h"
#include "pin_cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"

#define FILE_SOURCE_ID  "fm_pymt_pol_under_payment.c(1.9)"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_pymt_pol_under_payment(
        cm_nap_connection_t	*connp,
	u_int			opcode,
        u_int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp);

/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_PYMT_POL_UNDER_PAYMENT operation.
 * Just pick only the items that can be paid off with the given
 * PIN_FLD_AMOUNT and throw away rest of the items selected.
 *******************************************************************/
void
op_pymt_pol_under_payment(
        cm_nap_connection_t	*connp,
	u_int			opcode,
        u_int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_cookie_t		cookie = NULL;
	pin_flist_t		*r_flistp = NULL;
	pin_flist_t		*flistp = NULL;
	pin_decimal_t		*duep = NULL;
	pin_decimal_t		*amtp = NULL;

	int32			rec_id = 0;
	u_int			result = 0;
	pin_decimal_t		*total_allocated = NULL;
	pin_decimal_t		*total_recvd = NULL;
	pin_decimal_t		*item_amt = NULL;
	pin_decimal_t		*decp = NULL;
	u_int			status = PIN_SELECT_STATUS_UNDER_PAYMENT;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_PYMT_POL_UNDER_PAYMENT) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_pymt_pol_under_payment opcode error", ebufp);
		return;
	}

	/***********************************************************
	 * Debug: What we got.
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_pymt_pol_under_payment input flist", i_flistp);

	/***********************************************************
	 * Get the total money received from the input flist.
	 ***********************************************************/
	total_allocated	 = pbo_decimal_from_str("0.00", ebufp);
	amtp = (pin_decimal_t *)PIN_FLIST_FLD_GET(i_flistp, 
			PIN_FLD_AMOUNT, 0, ebufp);
	total_recvd = pbo_decimal_copy(amtp, ebufp);

	/***********************************************************
	 * Check for any credits with in the items selected and 
	 * include that money also in the toal received.
	 ***********************************************************/
	while ((flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_ITEMS,
		&rec_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {

		duep = (pin_decimal_t *)PIN_FLIST_FLD_GET(flistp,
			PIN_FLD_DUE, 0, ebufp);
		if (pbo_decimal_sign(duep, ebufp) < 0) {
			pbo_decimal_subtract_assign(total_recvd, duep, ebufp);
		}
	}

	cookie = NULL;
	r_flistp = PIN_FLIST_COPY(i_flistp, ebufp);
	/***********************************************************
	 * Go thru the input opem items and allocate the money.
	 ***********************************************************/
	while ((flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_ITEMS,
		&rec_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {

		/***************************************************
	 	 * Check to see, whether we can squeeze in this item
	 	 ****************************************************/
		duep = (pin_decimal_t *)PIN_FLIST_FLD_GET(flistp, 
				PIN_FLD_DUE, 0, ebufp);
		amtp = (pin_decimal_t *)PIN_FLIST_FLD_GET(flistp, 
				PIN_FLD_AMOUNT, 1, ebufp);

		if (pbo_decimal_is_null(duep, ebufp) ||
			(pbo_decimal_sign(duep, ebufp) < 0))
			continue;

		if (pbo_decimal_compare(total_allocated,
					 total_recvd, ebufp) >= 0 ){
			PIN_FLIST_ELEM_DROP(r_flistp, PIN_FLD_ITEMS, 
				rec_id, ebufp);
			continue;
		}

		if ((!pbo_decimal_is_null(amtp, ebufp)) ){
			if ((pbo_decimal_sign(duep, ebufp) > 0) && 
				(pbo_decimal_sign(amtp, ebufp) == 0)) {
				PIN_FLIST_ELEM_DROP(r_flistp, PIN_FLD_ITEMS, 
					rec_id, ebufp);
				continue;
			}
		}

		pbo_decimal_destroy(&item_amt);
		if (!pbo_decimal_is_null(amtp, ebufp)) {
			item_amt = pbo_decimal_negate(amtp, ebufp);
		} else {
			item_amt = pbo_decimal_copy(duep, ebufp);
		}

		pbo_decimal_destroy(&decp);
		decp = pbo_decimal_add(total_allocated, item_amt, ebufp);
		if (pbo_decimal_compare(decp, total_recvd, ebufp) > 0) {
			pbo_decimal_destroy(&item_amt);
			item_amt = pbo_decimal_subtract(total_recvd,
				  total_allocated, ebufp);
		} 

		pbo_decimal_add_assign(total_allocated, item_amt, ebufp);
		pbo_decimal_negate_assign(item_amt, ebufp);

		flistp = PIN_FLIST_ELEM_GET(r_flistp, 
				PIN_FLD_ITEMS, rec_id, 0, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_AMOUNT, 
				(void *)item_amt, ebufp);
	
	}

	if (flags & PIN_BILLFLG_SELECT_FINAL) {
		result = PIN_SELECT_RESULT_PASS;
	} else {
		result = PIN_SELECT_RESULT_FAIL;
		status |= PIN_SELECT_STATUS_MANUAL_ALLOC;
	}

	/***********************************************************
	 * Set the result & result status of select items.
	 ***********************************************************/
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_SELECT_RESULT, 
			(void *)&result, ebufp);
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_SELECT_STATUS, 
			(void *)&status, ebufp);
	/***********************************************************
	 * Cleanup.
	 ***********************************************************/
	pbo_decimal_destroy(&item_amt);
	pbo_decimal_destroy(&total_recvd);
	pbo_decimal_destroy(&decp);
	pbo_decimal_destroy(&total_allocated);

	/***********************************************************
	 * Results.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {

		/***************************************************
		 * Log something and return nothing.
		 ***************************************************/
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_pymt_pol_under_payment error", ebufp);
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		*o_flistpp = NULL;

	} else {

		/***************************************************
		 * Point the real return flist to the right thing.
		 ***************************************************/
		PIN_ERR_CLEAR_ERR(ebufp);
		*o_flistpp = r_flistp;

		/***************************************************
		 * Debug: What we're sending back.
		 ***************************************************/
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_pymt_pol_under_payment return flist", *o_flistpp);

	}
	return;
}
