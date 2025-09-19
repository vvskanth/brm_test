/*
 * (#)$Id: fm_voucher_pol_device_associate.c /cgbubrm_mainbrm.portalbase/1 2019/03/06 06:47:06 bsindhuj Exp $
 *
* Copyright (c) 2002, 2019, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "(#)$Id: fm_voucher_pol_device_associate.c /cgbubrm_mainbrm.portalbase/1 2019/03/06 06:47:06 bsindhuj Exp $";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#include "cm_fm.h"
#include "ops/voucher.h"
#include "ops/bill.h"
#include "ops/device.h"
#include "pin_errs.h"
#include "pin_voucher.h"
#include "pin_bill.h"
#include "pin_rate.h"
#include "pin_device.h"
#include "pin_currency.h"
#include "fm_utils.h"

#define FILE_LOGNAME "fm_voucher_pol_device_associate.c"

EXPORT_OP void
op_voucher_pol_device_associate(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp);


/**********************************************************************
 * Routines contained within
 **********************************************************************/
static void
fm_voucher_pol_device_associate(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
        pin_flist_t     *o_flistp,
	int32		flags,
	pin_errbuf_t	*ebufp);

static void
fm_voucher_pol_device_check_and_associate(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	*r_flistp,
        pin_flist_t     *o_flistp,
	int32		flags,
	pin_errbuf_t	*ebufp);

static void 
fm_voucher_pol_device_check_brand(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	*r_flistp,
	pin_errbuf_t 	*ebufp);

static void 
fm_voucher_pol_device_authenticate(
	pin_flist_t	*i_flistp,
	pin_flist_t	*r_flistp,
	pin_errbuf_t 	*ebufp);

static void
fm_voucher_pol_populate_bal_impacts(
        pin_flist_t     *deal_flistp,
        pin_flist_t     *o_flistp,
        pin_errbuf_t    *ebufp);

/**********************************************************************
 * Main routine for the PCM_OP_VOUCHER_POL_DEVICE_ASSOCIATE operation.
 **********************************************************************/
void
op_voucher_pol_device_associate(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flp,
	pin_flist_t		**o_flpp,
	pin_errbuf_t		*ebufp) {

	
	pcm_context_t	*ctxp = connp->dm_ctx;
	int32           svc_count = 0;		/* no of srvc objs on flist */
	int32		assoc_flag = 0;	/* associate/ disassociate flag */
	void		*vp = NULL;		/* void ptr	*/	

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/*********/
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_VOUCHER_POL_DEVICE_ASSOCIATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_voucher_pol_device_associate", ebufp);
		return;
	}

	/*
	 * DEBUG: What did we get?
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_voucher_pol_device_associate input flist", i_flp);

	/*
	 * By default, we copy the input to the output..
	 */
	
	*o_flpp = PIN_FLIST_COPY(i_flp, ebufp);


	/*
	 * count the number of service objects passed in
	 */
	svc_count = PIN_FLIST_ELEM_COUNT(i_flp, PIN_FLD_SERVICES, ebufp);

	if(svc_count <= 0){
		/*
		 * no validation required
		 */
		return;
		/*********/
	}


	/*
	 * check for association or disassociation first
	 */
	vp = PIN_FLIST_FLD_GET(i_flp, PIN_FLD_FLAGS, 0, ebufp);
	if(vp != (void *)NULL){
		assoc_flag = *(int32 *)vp;
	}

	if((assoc_flag & PIN_DEVICE_FLAG_ASSOCIATE) ==
		 PIN_DEVICE_FLAG_ASSOCIATE){

		/* 
		 * must be association, do the required validation
		 */
		fm_voucher_pol_device_associate(ctxp, i_flp, *o_flpp, flags, ebufp);
	}

	/*
	 * Results.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(o_flpp, NULL);
		*o_flpp = (pin_flist_t *)NULL;
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_device_associate error", ebufp);
	}else {
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_voucher_pol_device_assoicate return flist", *o_flpp);
	}
	
	return;
}

/****************************************************************************
 * fm_voucher_pol_device_associate():
 *
 * The validation specific to voucher devices before associating an account to 
 * the voucher device goes here. This function will associate the voucher
 * with the pre paid account, transfer the amount to the account and ensure
 * the brand of the voucher and the pre paid account is the same 
 * input:
 *	i_flistp: the opcode input flist
 * output:
 *	ebuf is set if there is an error 
 *****************************************************************************/
static void
fm_voucher_pol_device_associate(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	*o_flistp,
	int32		flags,
	pin_errbuf_t	*ebufp) {

	pin_flist_t	*flistp = NULL;		
	pin_flist_t	*sub_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	poid_t		*poidp = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/********/
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Get the device poid
	 */
	poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	/*
	 * do a read flds to get the device voucher info
	 */
	flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, poidp, ebufp);
	
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_STATE_ID, (void *)NULL, ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_ACCOUNT_OBJ, (void *)NULL, ebufp);

	sub_flistp = PIN_FLIST_SUBSTR_ADD(flistp, PIN_FLD_DEVICE_VOUCHER,  
		 ebufp);
	
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_DEAL_OBJ, (void *)NULL,
		 ebufp);

	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_EXPIRATION_T, (void *)NULL,
		 ebufp);

	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_VOUCHER_PIN, (void *)NULL,
		 ebufp);

	/*
	 * call the read flds opcode
	 */
	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE|PCM_OPFLG_LOCK_OBJ, flistp, 
		&r_flistp, ebufp); 


	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&flistp, NULL);
		return;
	}
	
	/*
	 * Validate the brand obj of the voucher device and the pre paid
	 * account is the same
	 */ 
	fm_voucher_pol_device_check_brand (ctxp, i_flistp, r_flistp, ebufp);
	

	/*
	 * Validate the voucher pin matches the one defined with in the 
	 * device object 
	 */ 
	fm_voucher_pol_device_authenticate (i_flistp, r_flistp, ebufp);

	/*
	 * During the association we have to ensure the device is not expired,
	 * the state of the device is still new and change the device state 
	 * into assign state or expire if the device is already expired
	 * Also at this stage we need to transfer the voucher info into 
	 * the account
	 */
	fm_voucher_pol_device_check_and_associate (ctxp, i_flistp, r_flistp, 
		o_flistp, flags, ebufp); 

	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	
	return;
}

/********************************************************************
 * fm_voucher_pol_device_acct_asst():
 * This function will validate the voucher before associating the voucher
 * with the account, change the state of the voucher as well as calculate
 * and returns balance impacts using deal purchase. 
 ********************************************************************/
static void 
fm_voucher_pol_device_check_and_associate(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	*r_flistp,
	pin_flist_t	*o_flistp,
	int32		flags,
	pin_errbuf_t 	*ebufp)
{

	int32		cur_state = 0; 		/* cur state of device */
	int32		state = 0; 		/* next state of device */
	pin_flist_t	*flistp = NULL;		/* flist ptr	*/
	pin_flist_t	*rd_flistp = NULL;	/* flist ptr	*/
	poid_t		*poidp = NULL;		/* poid ptr	*/
	pin_flist_t	*sub_flistp = NULL;	/* flist ptr	*/
	pin_flist_t	*extnd_flistp  = NULL;	/* flist ptr	*/
	
	pin_decimal_t	*amnt = NULL;		/* voucher amount */
	time_t		*expt = NULL;		/* voucher expiration */
	time_t		now ;		/* current time */
	void		*vp = NULL;	/* void */
	int32		currency = 0;	/* currency pointer */
	int32		act_life = 0;	/* activation life */

	pin_flist_t     *i_deal_flistp = NULL;         /* input flist ptr */
	int32	        *state_id = NULL;         /* temporary  ptr */
	
	pin_flist_t     *i_sub_deal_flistp = NULL;         /* sub flist ptr */
	pin_flist_t     *o_deal_flistp = NULL;         /* output flist ptr */
	poid_t		*acct_poidp = NULL;
	poid_t		*deal_poidp = NULL;
	poid_t          *serv_objp = NULL;
	
	pin_flist_t     *serv_flistp = NULL;         /* service flist ptr */

	char		*prg_name = "Unknown";


	if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
        PIN_ERR_CLEAR_ERR(ebufp);

	flistp = PIN_FLIST_CREATE(ebufp);

	poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	
	/*
	 * Get input account obj and the service obj.
	 */
	
	serv_flistp = PIN_FLIST_ELEM_GET(i_flistp, PIN_FLD_SERVICES, 
				PIN_ELEMID_ANY, 1, ebufp);

	if (serv_flistp != NULL) {
		acct_poidp = PIN_FLIST_FLD_GET(serv_flistp, 
			PIN_FLD_ACCOUNT_OBJ, 1, ebufp);
		serv_objp = PIN_FLIST_FLD_GET(serv_flistp,
			PIN_FLD_SERVICE_OBJ, 1 ,ebufp);
	}

	/*
	 * Read acct poid only if there is no service
	 */
	if (acct_poidp == (poid_t*) NULL) {
		acct_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACCOUNT_OBJ, 0,
			ebufp);
	}

	/*
	 * Get device current state 
	 */

	state_id = (int32 *)PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATE_ID, 0,
		ebufp);

	if(state_id != 0) {
		cur_state = *state_id;
	}
	/*
	 * Get device info 
	 */
	 sub_flistp = PIN_FLIST_SUBSTR_GET(r_flistp, PIN_FLD_DEVICE_VOUCHER, 0, 
		ebufp);

	/*
	 * Get expiration date
	 */
	expt = PIN_FLIST_FLD_GET (sub_flistp, PIN_FLD_EXPIRATION_T, 0, ebufp);

	/*
	 * Get deal poid
	 */
       deal_poidp = PIN_FLIST_FLD_GET (sub_flistp, PIN_FLD_DEAL_OBJ, 0, ebufp);

	/*
	 * Get current date
	 */
	 now = pin_virtual_time((time_t *)NULL);

	if ((expt != NULL) && 
		(*expt >= now)  && 
		(cur_state == PIN_VOUCHER_NEW_STATE)) {

		/* 
 		 * Associate the device to the account (purchase the 
		 * deal in calc_only) 
		 */
	
        	PIN_ERR_CLEAR_ERR(ebufp);
		i_deal_flistp = PIN_FLIST_CREATE(ebufp);
		
		PIN_FLIST_FLD_SET(i_deal_flistp, PIN_FLD_POID, acct_poidp, 
			ebufp);

		PIN_FLIST_FLD_SET(i_deal_flistp, PIN_FLD_PROGRAM_NAME, 
			prg_name, ebufp);
		i_sub_deal_flistp = PIN_FLIST_SUBSTR_ADD(i_deal_flistp, 
			PIN_FLD_DEAL_INFO, ebufp);
	
		PIN_POID_SET_DB(deal_poidp, cm_fm_get_current_db_no(ctxp));
		PIN_FLIST_FLD_SET(i_sub_deal_flistp, PIN_FLD_DEAL_OBJ, 
			deal_poidp, ebufp);

		/* Here we pass the Service object to the purchase deal opcodes
		 * so that discounts  are picked up appropriately and the
		 * balances are intact. */
	

		if (serv_objp != NULL){
			PIN_FLIST_FLD_SET(i_deal_flistp, PIN_FLD_SERVICE_OBJ,
				serv_objp, ebufp);
		}
	

		PCM_OPREF(ctxp, PCM_OP_SUBSCRIPTION_PURCHASE_DEAL, PCM_OPFLG_CALC_ONLY,
                        i_deal_flistp, &o_deal_flistp, ebufp);


		if (o_deal_flistp != (pin_flist_t*)NULL) {
			fm_voucher_pol_populate_bal_impacts(o_deal_flistp, 
				o_flistp, ebufp);
		}

		PIN_FLIST_DESTROY_EX(&i_deal_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&o_deal_flistp, NULL);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_device_check_and_associate "
				"error - during purchase deal", ebufp);
			PIN_FLIST_DESTROY_EX(&flistp, NULL);
			return;
		}

		/*
		 * Populate the Expiration Time in Return Flist 
		 */

		extnd_flistp  = PIN_FLIST_SUBSTR_GET(o_flistp, PIN_FLD_EXTENDED_INFO, 0,
		                ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_device_check_and_associate "
				"error - Extended info not found", ebufp);
			PIN_FLIST_DESTROY_EX(&flistp, NULL);

			return;
		}
		sub_flistp = PIN_FLIST_SUBSTR_GET(extnd_flistp , PIN_FLD_DEVICE_VOUCHER, 0,
		       	         ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_device_check_and_associate "
				"error - DEVICE_VOUCHER substruct not found", ebufp);
			PIN_FLIST_DESTROY_EX(&flistp, NULL);

			return;
		}
		PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_EXPIRATION_T, expt,ebufp);

	        if (!(flags & PCM_OPFLG_CALC_ONLY)) {
			/*
			* Device has not expired yet set device state to used
			*/
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, poidp, ebufp); 
			state = PIN_VOUCHER_USED_STATE;
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_NEW_STATE,
				&state, ebufp);
			
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_OLD_STATE,
				&cur_state, ebufp);

			PCM_OP(ctxp, PCM_OP_DEVICE_SET_STATE,  0, flistp, 
				&rd_flistp, ebufp);

			PIN_FLIST_DESTROY_EX(&rd_flistp, NULL);
			if (PIN_ERR_IS_ERR(ebufp)) {
				PIN_FLIST_DESTROY_EX(&flistp, NULL);
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_device_acct_asst error", 
				ebufp);
       				return ;
			}
		}
	}
	else if ((expt != NULL) &&
                (*expt < now)  && 
		(cur_state == PIN_VOUCHER_NEW_STATE) &&
		(!(flags & PCM_OPFLG_CALC_ONLY)) ) {
		/*
	 	 * Device has expired 
	 	 * Set device state to expired - if not CALC_ONLY
	 	 * Log a warning indicating device expired 
	 	 */
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, poidp, ebufp); 
		
		state = PIN_VOUCHER_EXPIRED_STATE;
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_NEW_STATE,
			&state, ebufp);
	
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_OLD_STATE,
			&cur_state, ebufp);
	
		PCM_OP(ctxp, PCM_OP_DEVICE_SET_STATE,  0, flistp, &rd_flistp,
			ebufp);

		PIN_FLIST_DESTROY_EX(&rd_flistp, NULL);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_FLIST_DESTROY_EX(&flistp, NULL);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_device_acct_asst error", 
				ebufp);
                	return ;
		}

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_WARNING,
			"fm_voucher_pol_device_associate - "
				"device is expired ", ebufp);
	}
	else if ( cur_state != PIN_VOUCHER_NEW_STATE ) {

		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE, PIN_FLD_STATE_ID, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_voucher_pol_device_associate - "
			"device state is not new ", ebufp);
	}
	else if ((expt != NULL) &&
                 *expt < now ) {

		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE, PIN_FLD_EXPIRATION_T, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_voucher_pol_device_associate - "
			"device has already expired ", ebufp);
	}
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	return;
}


/********************************************************************
 * fm_voucher_pol_device_check_brand():
 * This method will validate that the brand of the device 
 * is the same as the brand of the account that will be 
 * associated with the device.
 * It compares the brand object of the device with the brand obj 
 * of the account  
 * input :
 *	r_flistp: the flist containing the brand obj of the device 
 *	cur_pdp: the brand poid of the account 
 * output :
 *	ebuf is set if there is an error
 *
 ********************************************************************/
static void 
fm_voucher_pol_device_check_brand(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	*r_flistp,
	pin_errbuf_t 	*ebufp){

	poid_t			*cur_pdp = NULL;
	pin_flist_t		*ri_flistp = NULL;	/* read flist	*/
	pin_flist_t		*ro_flistp = NULL;	/* result flist	*/
	pin_flist_t		*nsvc_flistp = NULL;    /* single new service element */
	poid_t			*brand_pdp = NULL;	/* brand poid */
	poid_t			*dev_brand_pdp = NULL;	/* brand poid */
	int32			elem_id = 0;            /* element id */
	pin_cookie_t		cookie = NULL;          /* cookie to recurse array */


	if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
        PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Get the device brand object 
	 */
	dev_brand_pdp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_ACCOUNT_OBJ,
		0, ebufp);

	while((nsvc_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_SERVICES,
		&elem_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {

		/*
		 * get the account poid from each service
		 * one service is expected
		 */
		cur_pdp = PIN_FLIST_FLD_GET(nsvc_flistp, PIN_FLD_ACCOUNT_OBJ, 0,
			ebufp);
	}
	
        /*
         * Get the brand of the account from the database.
         */

        ri_flistp = PIN_FLIST_CREATE(ebufp);

        PIN_FLIST_FLD_SET(ri_flistp, PIN_FLD_POID, cur_pdp, ebufp);
        PIN_FLIST_FLD_SET(ri_flistp, PIN_FLD_BRAND_OBJ, NULL, ebufp);

        PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, ri_flistp, &ro_flistp, ebufp);

        brand_pdp = (poid_t *)PIN_FLIST_FLD_GET(ro_flistp,
                PIN_FLD_BRAND_OBJ, 0, ebufp);
	PIN_POID_SET_DB(dev_brand_pdp, cm_fm_get_current_db_no(ctxp));
	PIN_POID_SET_DB(brand_pdp, cm_fm_get_current_db_no(ctxp));

        if (PIN_POID_COMPARE(dev_brand_pdp, brand_pdp, 0, ebufp) != 0) {

                pin_set_err(ebufp, PIN_ERRLOC_FM,
                        PIN_ERRCLASS_SYSTEM_DETERMINATE,
                        PIN_ERR_PERMISSION_DENIED,
                        PIN_FLD_BRAND_OBJ, 0, 0);

                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_voucher_pol_device_check_brand non-matching "
                        "brand for device and account", ebufp);
        }

        PIN_FLIST_DESTROY_EX(&ri_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&ro_flistp, NULL);

        return;
}
/********************************************************************
 * fm_voucher_pol_device_authenticate():
 * This method will validate that the pin of the voucher 
 * matches the pin defined in the device object 
 * input :
 *	i_flistp: the flist containing the passed in pin 
 *	r_flistp: the flist contain the actual pin of the device 
 *		as defined in the device object	
 * output :
 *	ebuf is set if there is an error
 *
 ********************************************************************/
static void 
fm_voucher_pol_device_authenticate(
	pin_flist_t	*i_flistp,
	pin_flist_t	*r_flistp,
	pin_errbuf_t 	*ebufp){

	pin_flist_t	*sub_flistp = NULL;
	pin_flist_t	*ext_flistp = NULL;
	pin_flist_t	*v_flistp = NULL;
	char		*dev_pin = NULL;
	char		*i_pin = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
        PIN_ERR_CLEAR_ERR(ebufp);
	/*
	 * Get the device pin 
	 */
	 sub_flistp = PIN_FLIST_SUBSTR_GET(r_flistp, PIN_FLD_DEVICE_VOUCHER, 0, 
		ebufp);
	dev_pin = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_VOUCHER_PIN,
		0, ebufp);

	/*
	 * Get the pass in pin
	 */
	ext_flistp = PIN_FLIST_SUBSTR_GET(i_flistp, PIN_FLD_EXTENDED_INFO, 1, 
		ebufp);
	if (ext_flistp != NULL) {
		/*
		 * Get the pin
		 */
		v_flistp = PIN_FLIST_SUBSTR_GET(ext_flistp, PIN_FLD_DEVICE_VOUCHER,
			0, ebufp);
		i_pin = PIN_FLIST_FLD_GET(v_flistp, PIN_FLD_VOUCHER_PIN,
			0, ebufp);

		if (i_pin != NULL) {

			if (strcmp(dev_pin,i_pin) != 0) {

				pin_set_err(ebufp, PIN_ERRLOC_FM,
					PIN_ERRCLASS_APPLICATION,
					PIN_ERR_BAD_ARG,
					PIN_FLD_VOUCHER_PIN, 0, 0);

				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_voucher_pol_device_authenticate "
					"non-matching for device pin", ebufp);
			}
        	}

	}
	return;
}
/********************************************************************
 * fm_voucher_pol_device_authenticate():
 * This method will validate that the pin of the voucher
 * matches the pin defined in the device object
 * input :
 *      i_flistp: the flist containing the passed in pin
 *      r_flistp: the flist contain the actual pin of the device
 *              as defined in the device object
 * output :
 *      ebuf is set if there is an error
 *
 ********************************************************************/
static void
fm_voucher_pol_populate_bal_impacts(
        pin_flist_t     *deal_flistp,
        pin_flist_t     *o_flistp,
        pin_errbuf_t    *ebufp)
{

	int32	        elem_id = 0;                /* element id */
	int32	        bal_elem_id = 0;            /* element id */
	int32	        sub_bal_elem_id = 0;        /* element id */
	int32	        sub_bal_elem_count = 0;     /* count of sub_bal */
	pin_cookie_t    cookie = NULL;              /* cookie to recurse array */
	pin_cookie_t    bal_cookie = NULL;          /* cookie to recurse array */
	pin_cookie_t    sub_bal_cookie = NULL;      /* cookie to recurse array */
	pin_flist_t    *sub_bal_impact_flistp = NULL;	/* flist ptr	*/
	pin_flist_t    *ext_flistp = NULL;	    
	pin_flist_t    *sub_bal_flistp = NULL;	    
	pin_flist_t    *bal_imp_flistp = NULL;	    
	pin_flist_t    *deal_results_flistp = NULL;
	char           *result_type = NULL;
	poid_t         *result_poid = NULL;
	int32          *resource_id = NULL;
	
	pin_flist_t    *bal_impact_flistp = NULL;  /* flist for bal_impact */
	pin_flist_t    *tax_jur_flistp    = NULL;  /* flist for TAX_JURISDICTIONS */
	int32          elem_id1 = 0;               /* element id */
	pin_cookie_t   cookie1 = NULL;             /* cookie to recurse array */
	int32          bal_elem_count = 0;         /* element count */
	int32          tax_jur_count = 0;          /* element count */
	pin_decimal_t  *amount=0;
	time_t         *valid_from;
	time_t         *valid_to;

	/*
	 * The ext flist info will be one and all the balance impacts will be added to only this occurence
	 * That is the reason it is now outside the while loop.
	 */
	ext_flistp = PIN_FLIST_SUBSTR_GET(o_flistp, 
				PIN_FLD_EXTENDED_INFO, 1, ebufp);

	while( (deal_results_flistp = PIN_FLIST_ELEM_TAKE_NEXT(deal_flistp, 
		PIN_FLD_RESULTS, &elem_id, 1, &cookie, ebufp) ) != 
		(pin_flist_t *)NULL ) {
		result_poid = PIN_FLIST_FLD_GET(deal_results_flistp, 
			PIN_FLD_POID, 0, ebufp);
		result_type = (char*) PIN_POID_GET_TYPE(result_poid);
	        
		/*
		 * We require only purchase event
		 */ 
		if( strcmp(result_type, 
			PIN_OBJ_TYPE_EVENT_PRODUCT_FEE_PURCHASE) != 0 ) {
			PIN_FLIST_DESTROY_EX(&deal_results_flistp, NULL);
			continue;
		}		

		bal_cookie = NULL;
		bal_elem_id = 0;
		
		while ( (sub_bal_impact_flistp = PIN_FLIST_ELEM_TAKE_NEXT(
			deal_results_flistp, PIN_FLD_SUB_BAL_IMPACTS,
			&bal_elem_id, 1, &bal_cookie, ebufp) ) !=
			(pin_flist_t *)NULL) {


		/* 
		 * Since there is no restriction in number of balance impact 
		 * now we can have multiple sub balances. For each sub 
		 * balances we need to have seperate Balance impacts data.
		 * Below code has been modified to work for multiple balances 
		 * using array field type.
		 */


			resource_id = (int32*)PIN_FLIST_FLD_GET(
			sub_bal_impact_flistp, PIN_FLD_RESOURCE_ID, 1, ebufp);


			sub_bal_cookie = NULL;
			sub_bal_elem_id = 0;
			while ((sub_bal_flistp = PIN_FLIST_ELEM_GET_NEXT(
					sub_bal_impact_flistp, PIN_FLD_SUB_BALANCES, 
					&sub_bal_elem_id, 1, 
					&sub_bal_cookie, ebufp)) != 
					(pin_flist_t*)NULL) {

				bal_imp_flistp = PIN_FLIST_ELEM_ADD(ext_flistp,
							PIN_FLD_BAL_IMPACTS, 
							sub_bal_elem_count, 
							ebufp);
				
				PIN_FLIST_FLD_SET(bal_imp_flistp, 
					PIN_FLD_RESOURCE_ID,
					resource_id, ebufp);				
				/*
				 * get the required infomation
				 */
				amount = (pin_decimal_t *) PIN_FLIST_FLD_GET( sub_bal_flistp,
					PIN_FLD_AMOUNT, 1, ebufp);
				valid_from = PIN_FLIST_FLD_GET( sub_bal_flistp, 
					PIN_FLD_VALID_FROM, 1, ebufp);
				valid_to = PIN_FLIST_FLD_GET( sub_bal_flistp, 
					PIN_FLD_VALID_TO, 1, ebufp);
				if ( amount != NULL) {
					/*
				 	* set it to the new flist
				 	*/
					PIN_FLIST_FLD_SET(bal_imp_flistp, PIN_FLD_AMOUNT, 
						amount, ebufp);	
				}
				PIN_FLIST_FLD_SET(bal_imp_flistp, PIN_FLD_VALID_FROM, 
					valid_from, ebufp);	
				PIN_FLIST_FLD_SET(bal_imp_flistp, PIN_FLD_VALID_TO, 
					valid_to, ebufp);	

				sub_bal_elem_count ++;
			}
			PIN_FLIST_DESTROY_EX(&sub_bal_impact_flistp, NULL);
		}
		/**************************************************************************************
		 * SUBSCRIPTION_PURCHASE_DEAL returns BAL_IMPACTS in output flist. This will 
		 * have the detail balance impact(purchase fee, tax etc. Voucher opcode has to send 
		 * this complete balance impact to payment opcodes. 
		 *
		 * But return flist of voucher opcode uses BAL_IMPACTS array to return total amount. 
		 * To maintain the backward compatibility, this opcode is using SUB_BAL_IMPACTS to send 
		 * the BAL_IMPACTS sent from purchase_deal. 
		 *
		 * Payment opcode should read this SUB_BAL_IMPACTS and change it to BAL_IMPACTS before 
		 * recording the event.   
		 * More detail is there in bug 8342939 
		 **************************************************************************************/ 
		elem_id1 = 0;
		cookie1 = NULL;
		while ( (bal_impact_flistp = PIN_FLIST_ELEM_TAKE_NEXT(
		  	deal_results_flistp, PIN_FLD_BAL_IMPACTS,
			&elem_id1, 1, &cookie1, ebufp) ) !=
			(pin_flist_t *)NULL) {

				/* Store the balance impact in SUB_BAL_IMPACTS */
				PIN_FLIST_ELEM_PUT(ext_flistp, bal_impact_flistp, 
						PIN_FLD_SUB_BAL_IMPACTS, bal_elem_count, ebufp);
				bal_elem_count++;
		}
		elem_id1 = 0;
		cookie1 = NULL;
		while ( (tax_jur_flistp = PIN_FLIST_ELEM_TAKE_NEXT(
			deal_results_flistp, PIN_FLD_TAX_JURISDICTIONS, 
			&elem_id1, 1, &cookie1, ebufp) ) != 
			(pin_flist_t *)NULL) {
				/* Pass TAX_JURISDICTIONS also in output flist */ 
				PIN_FLIST_ELEM_PUT(ext_flistp, tax_jur_flistp, 
						PIN_FLD_TAX_JURISDICTIONS, tax_jur_count, ebufp);
				/* Using separate counter for the usecase having multiple RESULTS array*/
				tax_jur_count++; 
		}
				
		PIN_FLIST_DESTROY_EX(&deal_results_flistp, NULL);
	}
	return;
        /**************/

}
