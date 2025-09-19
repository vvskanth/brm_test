/*
 *
 * Copyright (c) 2002, 2009, Oracle and/or its affiliates.All rights reserved. 
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_voucher_pol_order_create.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:15:37 nishahan Exp $";
#endif

/*******************************************************************
 * Contains the PCM_OP_VOUCHER_POL_ORDER_CREATE operation.
 *******************************************************************/

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#define FILE_LOGNAME "fm_voucher_pol_order_create.c(1)"

#include "ops/voucher.h"
#include "pin_voucher.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_channel.h"
#include "fm_utils.h"

/*******************************************************************
 * External Routines Refered.
 *******************************************************************/
extern time_t fm_utils_time_round_to_midnight(time_t);

extern void
fm_voucher_pol_utils_is_valid_fld (
        pcm_context_t   *ctxp,
        poid_t          *poidp,
        int32           fldnum,
        char            *fld_value,
        pin_decimal_t   *quantity,
        pin_errbuf_t    *ebufp);

extern void
fm_voucher_pol_utils_is_valid_quantity (
        pcm_context_t   *ctxp,
        pin_decimal_t   *quantity,
        pin_decimal_t   *batch_total,
        pin_decimal_t   *batch_qty,
        pin_decimal_t   *pack_qty,
        pin_errbuf_t    *ebufp);

extern void
fm_voucher_pol_utils_verify_deal(
        pcm_context_t   *ctxp,
        poid_t          *poidp,
        poid_t          *deal_poidp,
	pin_flist_t     **deal_flistp,
        pin_errbuf_t    *ebufp);


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_voucher_pol_order_create(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);


static void
fm_voucher_pol_order_create(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_voucher_pol_validate_voucher_exp_time (
	pcm_context_t           *ctxp,
	pin_flist_t             *i_flistp,
	pin_flist_t             *deal_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t            *ebufp) ;

/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_VOUCHER_POL_ORDER_CREATE operation.
 *******************************************************************/
void
op_voucher_pol_order_create(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t	*r_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Null out results
	 */
	*r_flistpp = NULL;

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_VOUCHER_POL_ORDER_CREATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_create opcode error", ebufp);

		return;
	}
	
	/*
	 * Debug what we got.
         */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_voucher_pol_order_create input flist", i_flistp);

	/*
	 * Main rountine for this opcode
	 */
	fm_voucher_pol_order_create(ctxp, flags, i_flistp, r_flistpp, ebufp);
	
	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(r_flistpp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_create error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_voucher_pol_order_create output flist", *r_flistpp);
	}

	return;
}

/*******************************************************************
 * fm_voucher_pol_order_create:
 *
 * Validate the voucher start serial num to make sure that they 
 * are not duplicates. Also validate the voucher batch, pack
 * numbers and batch total are matched 
 *
 *******************************************************************/
static void
fm_voucher_pol_order_create(
	pcm_context_t           *ctxp,
	int32                   flags,
	pin_flist_t             *i_flistp,
	pin_flist_t             **r_flistpp,
	pin_errbuf_t            *ebufp)
{
	poid_t		*poidp = NULL;          /* poid from input flist */	
	poid_t		*deal_poidp = NULL;          /* poid from input flist */	
	pin_flist_t     *sub_flistp = NULL;
	pin_flist_t     *deal_flistp = NULL;
	pin_decimal_t	*quantity = NULL;       /* quantity */
	pin_decimal_t   *batch_total = NULL; 
	pin_decimal_t   *batch_qty = NULL; 
	pin_decimal_t   *pack_qty = NULL; 
	char		*voucher_start = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * By default we copy the input to the output flist.
	 */
	*r_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);

	/*
	 * Get the input poid from input flist
	 */
	poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);



	/*
	 * retrieve the starting voucher serial number and make
	 * sure it is valid
	 */
	sub_flistp = PIN_FLIST_ELEM_GET (i_flistp, PIN_FLD_VOUCHER_ORDERS,
		PIN_ELEMID_ANY, 0, ebufp);
	voucher_start = PIN_FLIST_FLD_GET(sub_flistp,
		PIN_FLD_START_SERIAL_NO, 0, ebufp);

	/*
	 * PIN_FLD_QUANTITY
	 */
	quantity  = (pin_decimal_t *)PIN_FLIST_FLD_GET(sub_flistp,
		PIN_FLD_QUANTITY, 0, ebufp);

	/*
	 * retrieve the batch and pack numbers to validate the numbers
	 * with the total quantity and deal poid
	 */
	batch_total = (pin_decimal_t *)PIN_FLIST_FLD_GET(sub_flistp,
		PIN_FLD_BATCH_TOTAL, 1, ebufp);
	batch_qty = (pin_decimal_t *)PIN_FLIST_FLD_GET(sub_flistp,
		PIN_FLD_BATCH_QUANTITY, 1, ebufp);
	pack_qty = (pin_decimal_t *)PIN_FLIST_FLD_GET(sub_flistp,
		PIN_FLD_PACK_QUANTITY, 1, ebufp);


	/*
	 * validate start serial number
	 */
	fm_voucher_pol_utils_is_valid_fld(ctxp, poidp, PIN_FLD_START_SERIAL_NO,
		(void *)voucher_start, quantity, ebufp);

	/*
	 * Validate the order quantity with the batch, pack numbers
	 */ 
	fm_voucher_pol_utils_is_valid_quantity(ctxp, quantity, batch_total, 
		batch_qty, pack_qty, ebufp);


	deal_poidp = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_DEAL_OBJ, 1, ebufp);
	/*
	 * Check deal object available in db.
	 */ 
	fm_voucher_pol_utils_verify_deal(ctxp, poidp, deal_poidp, &deal_flistp,ebufp); 

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_create error", ebufp);
		goto cleanup;
	}
	/*
	 * Check voucher expiration should be less than deal Expiration and 
	 * if PIN_FLD_EXPIRATION_T  is not passed set the 
	 * PIN_FLD_EXPIRATION_T = PIN_FLD_CARD_EXPIRATION + pin_virtual_time
	 */ 
	fm_voucher_pol_validate_voucher_exp_time(ctxp, i_flistp, deal_flistp, r_flistpp,ebufp); 
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_voucher_pol_validate_voucher_exp_time error", ebufp);
	}

	cleanup :

		PIN_FLIST_DESTROY_EX(&deal_flistp, NULL);	

	return;
}

/*******************************************************************
 * fm_voucher_pol_validate_voucher_exp_time:
 *
 * validate the Voucher expiration time should be less then or equal to 
 * Deal expiration Time. if voucher expiration time is null 
 * PIN_FLD_EXPIRATION_T =  PIN_FLD_CARD_EXPIRATION + pin_virtual_time
 *
 *******************************************************************/
static void
fm_voucher_pol_validate_voucher_exp_time (pcm_context_t           *ctxp,
	pin_flist_t             *i_flistp,
	pin_flist_t             *deal_flistp,
	pin_flist_t             **r_flistpp,
	pin_errbuf_t            *ebufp) {
	
	int		*card_expirationp = NULL;
	pin_flist_t     *voucher_order_flistp	= NULL;
	pin_flist_t     *deal_result_flistp	= NULL;
	pin_flist_t     *voucher_order_flist1p	= NULL;
	time_t 		*voucher_exptp = NULL;
	time_t 		*deal_exptp = NULL;
	time_t 		expt;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}

	deal_result_flistp = PIN_FLIST_FLD_GET(deal_flistp,
                                        PIN_FLD_RESULTS, 0, ebufp);

	deal_exptp = PIN_FLIST_FLD_GET(deal_result_flistp,
                                        PIN_FLD_END_T, 1, ebufp);

	if (deal_exptp == NULL || *deal_exptp == 0)  {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
		"fm_voucher_pol_validate_voucher_exp_time - "
		"Deal expires never ");
	}

	voucher_order_flistp = PIN_FLIST_FLD_GET(i_flistp,
                                        PIN_FLD_VOUCHER_ORDERS, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	voucher_exptp = PIN_FLIST_FLD_GET( voucher_order_flistp,
                                       PIN_FLD_EXPIRATION_T, 1, ebufp);

	if (voucher_exptp == NULL) {
		card_expirationp = PIN_FLIST_FLD_GET( voucher_order_flistp,
					       PIN_FLD_CARD_EXPIRATION, 1, ebufp); 
		if (card_expirationp != NULL) {
			expt = pin_virtual_time((time_t *)NULL);

			expt =  expt + (*card_expirationp * 86400);

			/*
			 * Round the time to Midnight.
			 */
			expt = fm_utils_time_round_to_midnight(expt);

			voucher_order_flist1p = PIN_FLIST_FLD_GET(*r_flistpp,
				PIN_FLD_VOUCHER_ORDERS, 0, ebufp);

			PIN_FLIST_FLD_SET( voucher_order_flist1p ,
				       PIN_FLD_EXPIRATION_T, &expt, ebufp);

			if (deal_exptp == NULL || *deal_exptp == 0)  {
				return;
			}
			if (expt > *deal_exptp) {
				if (PIN_ERR_IS_ERR(ebufp)) {
					return;
				}
				pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
					PIN_ERR_BAD_VALUE, PIN_FLD_CARD_EXPIRATION, 0, 0);

				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_voucher_pol_validate_voucher_exp_time - "
					"PIN_FLD_CARD_EXPIRATION  is greater than deal Expiration time", ebufp);
				return;
			}
		}
		else {
			if (PIN_ERR_IS_ERR(ebufp)) {
				return;
			}
			pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
				PIN_ERR_BAD_VALUE, PIN_FLD_EXPIRATION_T, 0, 0);

			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_validate_voucher_exp_time - "
				" either PIN_FLD_EXPIRATION_T or PIN_FLD_CARD_EXPIRATION "
				" needs to be passed.", ebufp);
			return;
		}
	}
	else if (!(deal_exptp == NULL || *deal_exptp == 0) && (*voucher_exptp > *deal_exptp) ){
		
		if (PIN_ERR_IS_ERR(ebufp)) {
			return;
		}
		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE, PIN_FLD_EXPIRATION_T, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_voucher_pol_validate_voucher_exp_time - "
			"PIN_FLD_EXPIRATION_T  is greater than deal Expiration time", ebufp);
	}

	return;
}
