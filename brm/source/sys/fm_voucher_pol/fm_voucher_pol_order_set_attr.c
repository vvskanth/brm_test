/*
 * (#)$Id: fm_voucher_pol_order_set_attr.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:15:44 nishahan Exp $
 *
* Copyright (c) 2002, 2010, Oracle and/or its affiliates. All rights reserved. 
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_voucher_pol_order_set_attr.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:15:44 nishahan Exp $";
#endif

/*******************************************************************
 * Contains the PCM_OP_VOUCHER_POL_ORDER_SET_ATTR operation.
 *******************************************************************/

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#define FILE_LOGNAME "fm_voucher_pol_order_set_attr.c(1)"
#define ONEDAYINSECS 86400 

#include "ops/voucher.h"
#include "pin_voucher.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_channel.h"
#include "fm_utils.h"

extern time_t fm_utils_time_round_to_midnight(time_t);


/*******************************************************************
 * External Routines Refered.
 *******************************************************************/
extern void
fm_voucher_pol_utils_read_order(
        pcm_context_t   *ctxp,
        poid_t          *poidp,
        pin_flist_t     **r_flistpp,
        pin_errbuf_t    *ebufp);

extern void
fm_voucher_pol_utils_verify_deal(
        pcm_context_t   *ctxp,
        poid_t          *poidp,
        poid_t          *deal_poidp,
	pin_flist_t	**deal_flistpp,
        pin_errbuf_t    *ebufp);

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


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_voucher_pol_order_set_attr(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_voucher_pol_order_set_attr(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);


static void 
fm_voucher_validate_restricted_modified_flds(
	pin_flist_t     *i_flistp,
	pin_errbuf_t    *ebufp);

static void 
fm_voucher_validate_all_modified_flds(
	pcm_context_t   *ctxp,
	pin_flist_t     *i_flistp,
	poid_t          *poidp,
	pin_flist_t     *res_flistp,
	pin_errbuf_t    *ebufp);

static void
fm_voucher_pol_validate_modified_exp_time (
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_flist_t             *deal_flistp,
        pin_flist_t             *flistp,
        pin_errbuf_t            *ebufp) ;



/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_VOUCHER_POL_ORDER_SET_ATTR operation.
 *******************************************************************/
void
op_voucher_pol_order_set_attr(
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

	/***********************************************************
	 * Null out results
	 ***********************************************************/
	*r_flistpp = NULL;

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_VOUCHER_POL_ORDER_SET_ATTR) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_set_attr opcode error", ebufp);

		return;
	}
	
	/***********************************************************
	 * Debut what we got.
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_voucher_pol_order_set_attr input flist", i_flistp);

	/***********************************************************
	 * Main rountine for this opcode
	 ***********************************************************/
	fm_voucher_pol_order_set_attr(ctxp, flags, i_flistp, r_flistpp, ebufp);
	
	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(r_flistpp, ebufp);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_set_attr error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_voucher_pol_order_set_attr output flist", *r_flistpp);
	}

	return;
}

/*******************************************************************
 * fm_voucher_pol_order_set_attr:
 *
 * Validate the voucher start serial num to make sure that they 
 * are not duplicates. Also validate the voucher batch, pack
 * numbers and total batches are matched 
 *
 *******************************************************************/
static void
fm_voucher_pol_order_set_attr(
	pcm_context_t           *ctxp,
	int32                   flags,
	pin_flist_t             *i_flistp,
	pin_flist_t             **r_flistpp,
	pin_errbuf_t            *ebufp)
{
	poid_t		*poidp = NULL;          /* poid from input flist */	
	poid_t		*deal_poidp = NULL;
	pin_flist_t     *flistp = NULL;
	pin_flist_t     *file_flistp = NULL;
	pin_flist_t     *res_flistp = NULL;
	pin_flist_t	*sub_flistp = NULL;
	pin_flist_t	*deal_flistp = NULL;
	void		*vp = NULL;
	int32		status = 0; 
	pin_cookie_t    cookie = NULL; 
	int32           eid = 0;
	time_t		now;

	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, "INSIDE ORDER_SET_ATTR");

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * By default we copy the input to the output flist.
	 */
	flistp = PIN_FLIST_COPY(i_flistp, ebufp);

	/*
	 * Get the input poid from input flist
	 */
	poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);


	/*
	 * Read the voucher info from the data base
	 */
	fm_voucher_pol_utils_read_order(ctxp, poidp, &res_flistp, ebufp);

	vp = PIN_FLIST_FLD_GET(res_flistp, PIN_FLD_STATUS, 0, ebufp);
	status =  *(int32 *)vp;

	switch(status){
	case PIN_VOUCHER_ORDER_NEW:
		/*
		 * anything can be modified
		 * res_file_info array does not exist
		 */
			
		fm_voucher_validate_all_modified_flds(ctxp, i_flistp,
			poidp, res_flistp, ebufp);
		
		/* Check if the Deal is available in DB.*/

		sub_flistp = PIN_FLIST_ELEM_GET (i_flistp, PIN_FLD_VOUCHER_ORDERS,
                        PIN_ELEMID_ANY, 1, ebufp);

		if (sub_flistp != NULL) {

			deal_poidp = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_DEAL_OBJ, 1, ebufp);	
			fm_voucher_pol_utils_verify_deal(ctxp, poidp, deal_poidp, &deal_flistp,ebufp);	

			if (PIN_ERR_IS_ERR(ebufp)) {
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_validate_modified_exp_time error", ebufp);
				PIN_FLIST_DESTROY_EX(&deal_flistp, NULL);
				PIN_FLIST_DESTROY_EX(&res_flistp, NULL);

				return;
			}
		
			fm_voucher_pol_validate_modified_exp_time(ctxp, i_flistp, deal_flistp,flistp ,ebufp);
		}
		


		/*
		 * PIN_FLD_FILES
		 */
		while ((file_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
			PIN_FLD_FILES, &eid, 1, &cookie, ebufp))
				!= (pin_flist_t *)NULL) {

			/*
			 * Get the creation date of the request file
			 * from the input file. Set the creation
			 * date to the current date if it is not
			 * passed
			 */
			vp = PIN_FLIST_FLD_GET(file_flistp,
				PIN_FLD_CREATED_T, 1, ebufp);
			if (vp == (void *)NULL) {
				now = pin_virtual_time((time_t *)0);
				PIN_FLIST_FLD_SET(file_flistp,
					PIN_FLD_CREATED_T, (void *)&now,
						ebufp);
			}
			PIN_FLIST_ELEM_SET(flistp, file_flistp, PIN_FLD_FILES,
				eid, ebufp);
		}

		break;

	case PIN_VOUCHER_ORDER_REQUEST:
	case PIN_VOUCHER_ORDER_PARTIAL_RECEIVE:
		/*
		 * res_file_info can be changed
		 */
		fm_voucher_validate_restricted_modified_flds(i_flistp,
			ebufp);
		break;

	case PIN_VOUCHER_ORDER_RECEIVED:
	case PIN_VOUCHER_ORDER_CANCELLED:
		/*
		 * no modification allowed
		 */
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_PERMISSION_DENIED, PIN_FLD_STATUS, 0, 
				status);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_order_set_attr - no update allowed ", 
					ebufp);
	         break;
	default:
		/*
		 * set an error with an unknown status
		 */
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
				PIN_ERR_PERMISSION_DENIED, PIN_FLD_STATUS, 0, 
					status);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_order_set_attr - unknown status value ", 
					ebufp);

	}

	/*
	 * Clean up
	 */

	PIN_FLIST_DESTROY_EX(&res_flistp, NULL);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_voucher_pol_order_set_attr error", ebufp);
	}
	
	*r_flistpp = flistp;

	return;
}


/******************************************************************************
 * fm_voucher_validate_all_modified_flds()
 *
 * validate all the flds being updated. Ensure update to the start serial number 
 * and the quantity matches. Also ensure the order quantity matches the batch
 * and pack numbers 
 *
 ******************************************************************************/
static void 
fm_voucher_validate_all_modified_flds(
	pcm_context_t   *ctxp,
	pin_flist_t     *i_flistp,
	poid_t          *poidp,
	pin_flist_t     *res_flistp,
	pin_errbuf_t    *ebufp)
{

	pin_flist_t     *sub_flistp = NULL; 
	pin_flist_t     *tmp_flistp = NULL;
	pin_flist_t     *deal_flistp = NULL;

	char		*cur_voucher = NULL;
	pin_decimal_t   *cur_qty = NULL;  
	char		*new_voucher = 0;
	pin_decimal_t   *new_qty = 0;   
	pin_decimal_t   *cur_batch_qty = NULL; 
	pin_decimal_t   *cur_batch_total = NULL; 
	pin_decimal_t   *cur_pack_qty = NULL; 
	pin_decimal_t   *new_batch_qty = NULL; 
	pin_decimal_t   *new_pack_qty = NULL; 
	pin_decimal_t   *new_batch_total = NULL; 
	void            *vp = NULL;          

	pin_cookie_t    cookie = NULL;
	int32           eid = 0;           

	pin_cookie_t    cookie2 = NULL;
	int32           eid2 = 0;           

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Get the validation fields from the input flist
	 */
	while ((sub_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
		PIN_FLD_VOUCHER_ORDERS, &eid, 1, &cookie, ebufp))
		!= (pin_flist_t *)NULL) {

		vp = (poid_t *) PIN_FLIST_FLD_GET (sub_flistp,
			PIN_FLD_DEAL_OBJ, 1, ebufp);

		/*
		 * Check deal object available in db.
		 */
		if (vp != (poid_t *)NULL) {
			fm_voucher_pol_utils_verify_deal(ctxp, poidp, 
				vp, &deal_flistp,ebufp);
		}
		PIN_FLIST_DESTROY_EX(&deal_flistp, NULL);

		vp = (pin_decimal_t *) PIN_FLIST_FLD_GET (sub_flistp,
			PIN_FLD_BATCH_QUANTITY, 1, ebufp);
		if (vp != (void *)NULL) {
			new_batch_qty = (pin_decimal_t *) vp;
		}
		vp = (pin_decimal_t *) PIN_FLIST_FLD_GET (sub_flistp,
			PIN_FLD_PACK_QUANTITY, 1, ebufp);
		if (vp != (void *)NULL) {
			new_pack_qty = (pin_decimal_t *) vp;
		}
		vp = (pin_decimal_t *) PIN_FLIST_FLD_GET (sub_flistp,
			PIN_FLD_BATCH_TOTAL, 1, ebufp);
		if (vp != (void *)NULL) {
			new_batch_total = (pin_decimal_t *) vp;
		}
		vp = PIN_FLIST_FLD_GET (sub_flistp,
			PIN_FLD_START_SERIAL_NO, 1, ebufp);
		if (vp != (void *)NULL) {
			new_voucher =  vp;
		}
		vp = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_QUANTITY, 1, ebufp);
		if (vp != (void *)NULL){
			new_qty = (pin_decimal_t *)vp;
		}

		/*
		 * Get the current validation fields from the read flist
		 */
		tmp_flistp = (pin_flist_t *) NULL;
		if((tmp_flistp = PIN_FLIST_ELEM_GET_NEXT(res_flistp,
			PIN_FLD_VOUCHER_ORDERS, &eid, 1, &cookie, ebufp)) != NULL) {

			vp = (pin_decimal_t *)PIN_FLIST_FLD_GET(tmp_flistp,
						PIN_FLD_BATCH_QUANTITY, 1, ebufp);
			if (vp != (void *)NULL){
				cur_batch_qty = (pin_decimal_t *)vp;
			}
			vp = (pin_decimal_t *)PIN_FLIST_FLD_GET(tmp_flistp,
						PIN_FLD_PACK_QUANTITY, 1, ebufp);
			if (vp != (void *)NULL){
				cur_pack_qty = (pin_decimal_t *)vp;
			}
			vp = (pin_decimal_t *) PIN_FLIST_FLD_GET (tmp_flistp,
				PIN_FLD_BATCH_TOTAL, 1, ebufp);
			if (vp != (void *)NULL) {
			cur_batch_total = (pin_decimal_t *) vp;
			}
			vp = PIN_FLIST_FLD_GET (tmp_flistp,
				PIN_FLD_START_SERIAL_NO, 1, ebufp);
			if (vp != (void *)NULL) {
				cur_voucher =  vp;
			}
			vp = PIN_FLIST_FLD_GET(tmp_flistp, PIN_FLD_QUANTITY, 1, ebufp);
			if (vp != (void *)NULL){
				cur_qty = (pin_decimal_t *)vp;
			}
		}

		/*
		 * validate the quantity with start serial num
	 	*/


		if(new_qty != 0){

			/*
			 * quantity is being updated
			 */
			if(new_voucher != NULL){

				/*
				 * the voucher start value is being updated
				 */
				fm_voucher_pol_utils_is_valid_fld(ctxp, poidp,
					 PIN_FLD_START_SERIAL_NO, new_voucher, new_qty, ebufp);

			}
			else {

				/*
				 * validate with the old voucher and new qty
				 */
					fm_voucher_pol_utils_is_valid_fld(ctxp, poidp,
					PIN_FLD_START_SERIAL_NO, cur_voucher, new_qty, ebufp);
	
			}

		}/* qty being updated */
		else {
			/*
			 * quantity is not being updated, check voucher
			 */
			if(new_voucher != NULL){
	
				/*
				 * the voucher start value is being updated
				 */
				fm_voucher_pol_utils_is_valid_fld(ctxp, poidp,
					 PIN_FLD_START_SERIAL_NO, new_voucher, cur_qty, ebufp);
	
			}
	
			/*
			 * don't need to check for else condition since qty is old
			 */
		}/*qty is not being updated */
	
		/*
		 * Validate quantity with batch, pack and batch total
		 */
		if (new_qty != 0 ) {
	
			if ( new_batch_qty != NULL && new_pack_qty != NULL
					&& new_batch_total != NULL){
				fm_voucher_pol_utils_is_valid_quantity(ctxp, new_qty, new_batch_total,
					new_batch_qty, new_pack_qty, ebufp);
			}
			else if ( new_batch_qty != NULL && cur_pack_qty != NULL
					&& new_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, new_qty, new_batch_total,
					new_batch_qty, cur_pack_qty, ebufp);
			}
			else if ( new_pack_qty != NULL && cur_batch_qty != NULL
					&& new_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, new_qty, new_batch_total,
					cur_batch_qty, new_pack_qty, ebufp);
			}
			else if (cur_pack_qty != NULL && cur_batch_qty != NULL
					&& new_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, new_qty, new_batch_total,
					cur_batch_qty, cur_pack_qty, ebufp);
			}
			else if ( new_batch_qty != NULL && new_pack_qty != NULL
					&& cur_batch_total != NULL){
				fm_voucher_pol_utils_is_valid_quantity(ctxp, new_qty, cur_batch_total,
					new_batch_qty, new_pack_qty, ebufp);
			}
			else if ( new_batch_qty != NULL && cur_pack_qty != NULL
					&& cur_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, new_qty, cur_batch_total,
					new_batch_qty, cur_pack_qty, ebufp);
			}
			else if ( new_pack_qty != NULL && cur_batch_qty != NULL
					&& cur_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, new_qty, cur_batch_total,
					cur_batch_qty, new_pack_qty, ebufp);
			}
			else if (cur_pack_qty != NULL && cur_batch_qty != NULL
					&& cur_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, new_qty, cur_batch_total,
					cur_batch_qty, cur_pack_qty, ebufp);
			}
		}
		else {
			if ( new_batch_qty != NULL && new_pack_qty != NULL
					&& new_batch_total != NULL){
				fm_voucher_pol_utils_is_valid_quantity(ctxp, cur_qty, new_batch_total,
					new_batch_qty, new_pack_qty, ebufp);
			}
			else if ( new_batch_qty != NULL && cur_pack_qty != NULL
					&& new_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, cur_qty, new_batch_total,
					new_batch_qty, cur_pack_qty, ebufp);
			}
			else if ( new_pack_qty != NULL && cur_batch_qty != NULL
					&& new_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, cur_qty, new_batch_total,
					cur_batch_qty, new_pack_qty, ebufp);
			}
			else if (cur_pack_qty != NULL && cur_batch_qty != NULL
					&& new_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, cur_qty, new_batch_total,
					cur_batch_qty, cur_pack_qty, ebufp);
			}
			else if ( new_batch_qty != NULL && new_pack_qty != NULL
					&& cur_batch_total != NULL){
				fm_voucher_pol_utils_is_valid_quantity(ctxp, cur_qty, cur_batch_total,
					new_batch_qty, new_pack_qty, ebufp);
			}
			else if ( new_batch_qty != NULL && cur_pack_qty != NULL
					&& cur_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, cur_qty, cur_batch_total,
					new_batch_qty, cur_pack_qty, ebufp);
			}
			else if ( new_pack_qty != NULL && cur_batch_qty != NULL
					&& cur_batch_total != NULL) {
				fm_voucher_pol_utils_is_valid_quantity(ctxp, cur_qty, cur_batch_total,
					cur_batch_qty, new_pack_qty, ebufp);
			}
			else {
			/*      fm_voucher_utils_is_valid_quantity(ctxp, cur_batch_qty,
			 *              cur_pack_qty, cur_qty, ebufp);
			 * No need to validate
			*/
			}
		}
	
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
	PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"fm_voucher_validate_all_modified_flds error", ebufp);
	}

	return ;

}



/******************************************************************************
 * fm_voucher_validate_restricted_modified_flds()
 *
 * This function will ensure that results array should be updated
 * if the current order state is request or partial receive
 *
 ******************************************************************************/
static void 
fm_voucher_validate_restricted_modified_flds(
	pin_flist_t     *i_flistp,
	pin_errbuf_t    *ebufp)
{
	int32           count = 0;
        pin_flist_t     *sub_flistp = NULL;
        void            *vp = NULL;
        pin_cookie_t    cookie = NULL;
        int32           eid = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 *  PIN_FLD_RESULTS_DATA - even though the field is optional, it is
	 * mandatory here since someone cannot change the status of an
	 * order to PARTIAL_RECEIVE or RECEIVE without updating the
	 * response file info.
	 */

	/*
	 * first check if there are any result elements. set an error if
	 * there are none
	 */


	count = PIN_FLIST_ELEM_COUNT(i_flistp, PIN_FLD_RESULTS_DATA, ebufp);
	if(count == 0){
		/*
		 * set an error
		 */
		pin_set_err(ebufp, PIN_ERRLOC_FM,
		PIN_ERRCLASS_APPLICATION, PIN_ERR_NOT_FOUND,
		PIN_FLD_RESULTS_DATA, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_voucher_validate_restricted_mdified_flds - "
			"status  cannot be changed to PARTIAL_RECEIVE "
			"or RECEIVED without adding PIN_FLD_RESULTS_DATA",
			ebufp);
	}


        while ((sub_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
                PIN_FLD_VOUCHER_ORDERS, &eid, 1, &cookie, ebufp))
                != (pin_flist_t *)NULL) {

                vp = (poid_t *) PIN_FLIST_FLD_GET (sub_flistp,
                        PIN_FLD_DEAL_OBJ, 1, ebufp);
		
		if( vp != NULL ){
	                pin_set_err(ebufp, PIN_ERRLOC_FM,
       			        PIN_ERRCLASS_APPLICATION, 
				PIN_ERR_PROHIBITED_ARG,
                		PIN_FLD_DEAL_OBJ, 0, 0);
                	PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_voucher_validate_restricted_mdified_flds - "
			"deal object cannot be changed when in request"
			"/partial receive state",
                        ebufp);
		}
		
	}
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_voucher_validate_restricted_modified_flds error",
			ebufp);
	}

	return ;
}

/*******************************************************************
 * fm_voucher_pol_validate_modified_exp_time:
 *
 * Validate if the Voucher expiration time is less then or equal to
 * Deal expiration Time and handle any updates to the voucher
 * expiration.
 *
 *******************************************************************/
static void
fm_voucher_pol_validate_modified_exp_time (pcm_context_t *ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	*deal_flistp,
	pin_flist_t	*flistp,
	pin_errbuf_t	*ebufp) {

	int		*card_expirationp 	= NULL;
	pin_flist_t	*voucher_order_flistp   = NULL;
	pin_flist_t	*deal_result_flistp     = NULL;
	pin_flist_t	*voucher_order_flist1p  = NULL;
	time_t		*voucher_exptp		= NULL;
	time_t		*deal_exptp		= NULL;
	time_t		expt;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}

	deal_result_flistp = PIN_FLIST_FLD_GET(deal_flistp,
		PIN_FLD_RESULTS, 0, ebufp);

	deal_exptp = PIN_FLIST_FLD_GET(deal_result_flistp,
		PIN_FLD_END_T, 1, ebufp);

	if (deal_exptp == NULL || *deal_exptp == 0)  {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
		"fm__voucher_pol_validate_modified_exp_time - "
		"Deal expires never ");
	}

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_voucher_pol_validate_modified_exp_time input flist", i_flistp);	

	voucher_order_flistp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_VOUCHER_ORDERS, 
		0, ebufp);


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}

	voucher_exptp = PIN_FLIST_FLD_GET( voucher_order_flistp,
		PIN_FLD_EXPIRATION_T, 1, ebufp);
	card_expirationp = PIN_FLIST_FLD_GET( voucher_order_flistp,
		PIN_FLD_CARD_EXPIRATION, 1, ebufp);

	if (card_expirationp != NULL && *card_expirationp !=0 ) {
		expt = pin_virtual_time((time_t *)NULL);
		expt =  expt + (*card_expirationp * ONEDAYINSECS);

		/*
		* Round the time to Midnight.
		*/
		expt = fm_utils_time_round_to_midnight(expt);

		voucher_order_flist1p = PIN_FLIST_FLD_GET(flistp,
			PIN_FLD_VOUCHER_ORDERS, 0, ebufp);

		PIN_FLIST_FLD_SET( voucher_order_flist1p,
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
				"fm_voucher_pol_validate_modified_exp_time - "
					"PIN_FLD_CARD_EXPIRATION  is greater than deal Expiration time", ebufp);
			return;
		}
	}

	
	else if((card_expirationp == NULL || *card_expirationp == 0) && voucher_exptp == NULL) {

		if (PIN_ERR_IS_ERR(ebufp)) {
			return;
		}
		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE, PIN_FLD_EXPIRATION_T, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_voucher_pol_validate_modified_exp_time - "
			" either PIN_FLD_EXPIRATION_T or PIN_FLD_CARD_EXPIRATION "
			" needs to be passed.", ebufp);
		return;
	}

	if (!(deal_exptp == NULL || *deal_exptp == 0) && ((voucher_exptp != NULL) && (*voucher_exptp > *deal_exptp)) ){

		if (PIN_ERR_IS_ERR(ebufp)) {
			return;
		}
		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
		PIN_ERR_BAD_VALUE, PIN_FLD_EXPIRATION_T, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"fm_voucher_pol_validate_modified_exp_time - "
		"PIN_FLD_EXPIRATION_T  is greater than deal Expiration time", ebufp);
	}

	return;
	
}
