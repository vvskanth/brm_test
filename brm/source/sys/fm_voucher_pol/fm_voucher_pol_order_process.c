/*
 *
 * Copyright (c) 2002, 2024, Oracle and/or its affiliates.All rights reserved. 
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_voucher_pol_order_process.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:15:41 nishahan Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/voucher.h"
#include "pin_voucher.h"
#include "cm_fm.h"
#include "pin_errs.h"

#define FILE_LOGNAME "fm_voucher_pol_order_process.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_voucher_pol_order_process(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_voucher_pol_order_process(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);


/*******************************************************************************
 * Entry routine for the PCM_OP_VOUCHER_POL_ORDER_PROCESS opcode
 ******************************************************************************/
void
op_voucher_pol_order_process(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_VOUCHER_POL_ORDER_PROCESS) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_process opcode error", ebufp);

		*o_flistpp = NULL;
		return;
	}

	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_voucher_pol_order_process input flist", i_flistp);

	/*
	 * Main rountine for this opcode
	 */
	fm_voucher_pol_order_process(ctxp, i_flistp, o_flistpp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_process error", ebufp);
		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_voucher_pol_order_process return flist", *o_flistpp);
	}

	return;
}


/*******************************************************************************
 * fm_voucher_pol_order_process()
 *
 * Inputs: flist with /order/voucher poid 
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function validates the input flist has a poid of type '/order/voucher',
 * then validates the order states 
 ******************************************************************************/
static void
fm_voucher_pol_order_process(
	pcm_context_t           *ctxp,
	pin_flist_t             *i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t            *ebufp)
{
	void			*vp = NULL;			/* flist gets */
	pin_flist_t             *flistp = NULL;			/* read flist */
	pin_flist_t             *tmp_flistp = NULL;			/* read flist */
	pin_flist_t             *r_flistp = NULL;		/* result flist */
	pin_flist_t		*sub_flistp = NULL;
	char			*poid_type = NULL;		/* poid type */
	int32			status = 0;
	pin_decimal_t		*quantity_req = NULL;
	pin_decimal_t		*quantity_sum = NULL;
	pin_decimal_t		*quant = NULL;
	pin_decimal_t		*proc_quant = NULL;
	char			quantity[15];
	int32			dev_count = 0;
	int32			dup_record = 0;
	int32			actual_dev = 0;
	int32			new_state = 0;
	int32			elem_id =0;
	pin_cookie_t		cookie = NULL;



	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	flistp = PIN_FLIST_CREATE(ebufp);

	tmp_flistp = PIN_FLIST_COPY(i_flistp, ebufp);

	/*
	 * Validate the object is of type /order/voucher
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	poid_type = (char *)PIN_POID_GET_TYPE((poid_t *)vp);

	if(vp != NULL){
		poid_type = (char *)PIN_POID_GET_TYPE((poid_t *)vp);
		if ((strncmp(poid_type, PIN_OBJ_TYPE_ORDER_VOUCHER,
			strlen(PIN_OBJ_TYPE_ORDER_VOUCHER)) != 0 ) ||
			(strcmp(poid_type, PIN_OBJ_TYPE_ORDER_VOUCHER)) != 0) {

			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
			"fm_voucher_pol_order_process invalid poidtype");

			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_APPLICATION,
               		        PIN_ERR_BAD_POID_TYPE, PIN_FLD_POID, 0, 0);

		}
	}

	/*
	 * Get the values of the order state id from the database
	 */
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, vp, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_STATUS, NULL, ebufp);
	PIN_FLIST_ELEM_SET(flistp,NULL,PIN_FLD_VOUCHER_ORDERS,
		PIN_ELEMID_ANY, ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
               "fm_voucher_pol_order_process rd flds input flist", flistp);
        PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, flistp, &r_flistp,
		ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_voucher_pol_order_process rd flds return flist", r_flistp);

	while((sub_flistp = PIN_FLIST_ELEM_GET_NEXT(r_flistp, PIN_FLD_VOUCHER_ORDERS,
		&elem_id, 1, &cookie, ebufp)) != (pin_flist_t*) NULL) {

		quantity_req = PIN_FLIST_FLD_GET (sub_flistp, PIN_FLD_QUANTITY, 0,
			ebufp);
	}

	/*
	 * Processing the voucher order is not allowed 
	 * if the status of:
	 *
	 *   PIN_VOUCHER_ORDER_CANCELLED
	 *
	 * States outside of this list will generate an error.
	 */
	vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATUS, 0, ebufp);
	if(vp != NULL){
		status = *(int32 *)vp;

		if(status == PIN_VOUCHER_ORDER_CANCELLED) 
		{
			/*
			 * validation fails 
			 */
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_PROHIBITED_ARG, PIN_FLD_POID,
				 0, 0);
			PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR, 
				"cannot process an order if the "
				"order is in cancel state ",
				 ebufp);
		}
	}

	/*
	 * Set the state id
	 */

	/*
	 * Get the duplicate records
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_DUPLICATE, 1,
		ebufp);
	if (vp != NULL ){ 
		dup_record = *(int32 *)vp;
	}

	/*
	 * Get the actual devices
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_COUNT, 1,
		ebufp);

	if (vp != NULL ){ 
		dev_count = *(int32 *)vp;
	}

	/*
	 * Get the order applied quantity
	 */
	quantity_sum = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_QUANTITY_APPLIED, 1,
		ebufp);


 	/* Subtract the duplicate records from the array */
	actual_dev = dev_count - dup_record;
	pin_snprintf(quantity, sizeof(quantity), "%d", actual_dev);

	quant = pin_decimal(quantity, ebufp);

	if(!pbo_decimal_is_null(quantity_sum, ebufp)){
		proc_quant = pbo_decimal_add(quant, quantity_sum, ebufp);
	}
	else {
		quantity_sum = pin_decimal("0.0", ebufp);
		proc_quant = pbo_decimal_add(quant, quantity_sum, ebufp);
	}

	if(!pbo_decimal_is_null(quantity_req, ebufp)) {

		if (pbo_decimal_compare(quantity_req, proc_quant, ebufp) == -1) {

			pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE , 0, 0, 0);

			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"Error in fm_voucher_pol_order_process. "
			"Received Quantity is greater than the requested quantity.",ebufp);

                }

		else if (pbo_decimal_compare(quantity_req, proc_quant, ebufp) == 1){ 

			new_state = PIN_VOUCHER_ORDER_PARTIAL_RECEIVE;
		}
		else {
			new_state = PIN_VOUCHER_ORDER_RECEIVED;
		}
	}

	/*
	 * Set the state id in the return flist
	 */
	PIN_FLIST_FLD_SET (tmp_flistp, PIN_FLD_STATUS, &new_state, ebufp);
	
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	PIN_DECIMAL_DESTROY_EX(&proc_quant);
	PIN_DECIMAL_DESTROY_EX(&quant);

	*o_flistpp = tmp_flistp;

	return;
}
