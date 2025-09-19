/*
 *
 *      Copyright (c) 1999, 2024, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_bill_pol_valid_transfer.c:BillingVelocityInt:2:2006-Sep-05 21:55:44 %";
#endif

#pragma GCC diagnostic ignored "-Wunused-variable"

/*******************************************************************
 * Contains the PCM_OP_BILL_POL_VALID_TRANSFER operation. 
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 
 
#include "pcm.h"
#include "ops/bill.h"
#include "pin_bill.h"
#include "pin_cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"

#define FILE_SOURCE_ID  "fm_bill_pol_valid_transfer.c(1.7)"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_bill_pol_valid_transfer(
        cm_nap_connection_t	*connp,
	u_int			opcode,
        u_int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp);
void
fm_bill_pol_validate_src_item(
        cm_nap_connection_t	*connp,
        pin_flist_t		*i_flistp,
        u_int			*resultp,
        pin_errbuf_t		*ebufp);

/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/
extern void fm_bill_pol_validate_item_fields();

/*******************************************************************
 * Main routine for the PCM_OP_BILL_POL_VALID_TRANSFER operation.
 *******************************************************************/
void
op_bill_pol_valid_transfer(
        cm_nap_connection_t	*connp,
	u_int			opcode,
        u_int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	
	pin_flist_t		*r_flistp = NULL;
	poid_t 			*src_item_pdp = NULL;
	pin_cookie_t		cookie = NULL;
	int32			element_id = 0;
	pin_flist_t		*flistp;
	char			*descrp = NULL;
	void			*vp = NULL;

	u_int			status = 0;
	u_int			result = 0;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_BILL_POL_VALID_TRANSFER) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_bill_pol_valid_transfer opcode error", ebufp);
		return;
	}

	/***********************************************************
	 * Debug: What we got.
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_bill_pol_valid_transfer input flist", i_flistp);

	src_item_pdp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ITEM_OBJ, 0, ebufp);

	if ( strcmp(PIN_POID_GET_TYPE (src_item_pdp),
			PIN_OBJ_TYPE_ITEM_PAYMENT) == 0){
		fm_bill_pol_validate_src_item(connp, i_flistp,
						&status, ebufp);
		if( status != PIN_RESULT_FAIL ) {
			while( ( flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
					PIN_FLD_ITEMS, &element_id, 
						1, &cookie, ebufp)) != NULL) {

				/***************************************************
				 * Validate the item fields.
				 ***************************************************/
				fm_bill_pol_validate_item_fields(connp, 
						flistp, &result, ebufp);
			}
			result |= PIN_BOOLEAN_TRUE;
			descrp = (char *)"Succeeded";
		}
		else {
			descrp = (char *)"Failed";
		}
	}

	else{
		/************************************************************
		* Iterate the item list
		************************************************************/
		while( ( flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
				PIN_FLD_ITEMS, &element_id, 
					1, &cookie, ebufp)) != NULL) {

			/***************************************************
			 * Validate the item fields.
			 ***************************************************/
			fm_bill_pol_validate_item_fields(connp, 
					flistp, &result, ebufp);
		}
		result |= PIN_BOOLEAN_TRUE;
		descrp = (char *)"Succeeded";
	}

	/***********************************************************
	 * Construct the return flist.
	 ***********************************************************/
	r_flistp = PIN_FLIST_CREATE(ebufp);

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ITEM_OBJ, 0, ebufp);
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_POID, vp, ebufp);

	/***********************************************************
	 * Set the validation result field (pass or fail)
		and description about validation
	 ***********************************************************/
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_RESULT, (void *)&result, ebufp);
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_DESCR, (void *)descrp, ebufp);

	/***********************************************************
	 * Results.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {

		/***************************************************
		 * Log something and return nothing.
		 ***************************************************/
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_bill_pol_valid_transfer error", ebufp);
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
			"op_bill_pol_valid_transfer return flist", r_flistp);

	}

	return;
}
/*******************************************************************
 * fm_bill_pol_validate_src_item()
 *******************************************************************/
void
fm_bill_pol_validate_src_item(connp, i_flistp, resultp, ebufp)
        cm_nap_connection_t	*connp;
	pin_flist_t	*i_flistp;
	u_int		*resultp;
	pin_errbuf_t	*ebufp;
{
	pcm_context_t	*ctxp = connp->dm_ctx;
	void 		*vp = NULL;
	pin_flist_t	*robj_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	pin_flist_t	*flistp = NULL;
	pin_decimal_t   *item_total = NULL;
	pin_decimal_t   *due = NULL;
	pin_decimal_t   *amtp = NULL;
	pin_decimal_t   *abs_amtp = NULL;
	pin_decimal_t   *abs_due = NULL;
	pin_decimal_t   *abs_item_total = NULL;
	pin_decimal_t   *total_dst_amt = NULL;
	pin_cookie_t	cookie = NULL;
	int32           element_id = 0;


	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	robj_flistp = PIN_FLIST_CREATE(ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ITEM_OBJ, 0, ebufp);
	PIN_FLIST_FLD_SET(robj_flistp, PIN_FLD_POID, vp, ebufp);
	PIN_FLIST_FLD_SET(robj_flistp, PIN_FLD_ITEM_TOTAL, NULL, ebufp);
	PIN_FLIST_FLD_SET(robj_flistp, PIN_FLD_DUE, NULL, ebufp);

	PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, robj_flistp, &r_flistp, ebufp);
	PIN_FLIST_DESTROY_EX(&robj_flistp, NULL);

	item_total = (pin_decimal_t *)PIN_FLIST_FLD_GET(r_flistp,
			PIN_FLD_ITEM_TOTAL, 0, ebufp);

	due = (pin_decimal_t *)PIN_FLIST_FLD_GET(r_flistp,
			PIN_FLD_DUE, 0, ebufp);
	abs_due = pbo_decimal_abs(due, ebufp);
	abs_item_total = pbo_decimal_abs(item_total, ebufp);
	total_dst_amt = pin_decimal("0.0", ebufp);

	while( ( flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
			PIN_FLD_ITEMS, &element_id, 
				1, &cookie, ebufp)) != NULL) {

		amtp = (pin_decimal_t *)PIN_FLIST_FLD_GET(flistp,
			PIN_FLD_AMOUNT, 0, ebufp);
		if (pbo_decimal_is_zero(amtp, ebufp)) {
			continue;
		}
		abs_amtp = pbo_decimal_abs(amtp, ebufp);
		pbo_decimal_add_assign(total_dst_amt, abs_amtp, ebufp);

		/***************************************************
		 * when the item transfer will be from /item/payment 
		 * to bill items
		 ***************************************************/

		if (pbo_decimal_sign(amtp, ebufp) < 0) {
			if(pbo_decimal_compare(abs_amtp, abs_due, ebufp) > 0) {
				*resultp &= PIN_RESULT_FAIL;
				pbo_decimal_destroy(&abs_amtp);
				break;
			}
			else {
				pbo_decimal_subtract_assign(abs_due, abs_amtp, ebufp);
				*resultp |= PIN_RESULT_PASS;
			}
		}
		
		/***************************************************
		 * when the item transfer will be from bill item 
		 * to /item/payment
		 ***************************************************/

		if (pbo_decimal_sign(amtp, ebufp) > 0) {
			if(pbo_decimal_compare(amtp, abs_item_total, ebufp) > 0) {
				*resultp &= PIN_RESULT_FAIL;
				pbo_decimal_destroy(&abs_amtp);
				break;
			}
			else {
				pbo_decimal_subtract_assign(abs_item_total, amtp, ebufp);
				*resultp |= PIN_RESULT_PASS;
			}
		}		
		pbo_decimal_destroy(&abs_amtp);
	}
	if (pbo_decimal_is_zero(total_dst_amt, ebufp)){
		*resultp |= PIN_RESULT_PASS;
	}
	pbo_decimal_destroy(&total_dst_amt);	
	pbo_decimal_destroy(&abs_due);	
	pbo_decimal_destroy(&abs_item_total);	
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
}
