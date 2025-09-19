/*
 * @(#)$Id: fm_ip_pol_device_associate.c /cgbubrm_mainbrm.portalbase/1 2019/03/11 00:02:48 mmaley Exp $
 *
* Copyright (c) 2004, 2019, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_ip_pol_device_associate.c /cgbubrm_mainbrm.portalbase/1 2019/03/11 00:02:48 mmaley Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "cm_fm.h"
#include "ops/ip.h"
#include "ops/device.h"
#include "pin_errs.h"
#include "pin_ip.h"
#include "pin_apn.h"
#include "pin_device.h"
#include "fm_utils.h"

#define FILE_LOGNAME "fm_ip_pol_device_associate.c"

EXPORT_OP void
op_ip_pol_device_associate(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp);

#define FILE_SOURCE_ID	"fm_ip_pol_device_associate.c(1.1)"

/**********************************************************************
 * Routines contained within
 **********************************************************************/

static void
fm_ip_pol_device_associate(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	int32		flags,
	pin_errbuf_t	*ebufp);

static void
fm_ip_pol_device_check_brand(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	*r_flistp,
	pin_errbuf_t 	*ebufp);

static void
fm_ip_apn_device_exist(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_errbuf_t 	*ebufp);

static void
fm_ip_pol_device_disassociate(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);

/**********************************************************************
 * Main routine for the PCM_OP_IP_POL_DEVICE_ASSOCIATE operation.
 **********************************************************************/
void
op_ip_pol_device_associate(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp) {

	pcm_context_t	*ctxp = connp->dm_ctx;
	int32		assoc_flag = 0;		/* associate flag */
	void		*vp = NULL;		/* void ptr */	

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_ip_pol_device_associate Error ", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if(opcode != PCM_OP_IP_POL_DEVICE_ASSOCIATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_ip_pol_device_associate", ebufp);
		return;
	}

	/*
	 * DEBUG: What did we get?
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_ip_pol_device_associate input flist", i_flistp);

	/*
	 * check for association  of service
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_FLAGS, 0, ebufp);
	if(vp != (void *)NULL){
		assoc_flag = *(int32 *)vp;
	}
	else
	{
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_ip_pol_device_associate Association Flag Error", ebufp);
		return;
	}

	if((assoc_flag & PIN_DEVICE_FLAG_ASSOCIATE) ==
		 PIN_DEVICE_FLAG_ASSOCIATE){
		/*
		 * association, do the required validation
		 */
		fm_ip_pol_device_associate(ctxp, i_flistp, flags, ebufp);
	}
	else {
		fm_ip_pol_device_disassociate(ctxp, i_flistp, ebufp);
	}	

	/*
	 * Results.
	 */
	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_ip_pol_device_associate error", ebufp);
	}
	else {
		*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);

		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_ip_pol_device_assoicate return flist", *o_flistpp);
	}

	return;
}

/****************************************************************************
 * fm_ip_pol_device_associate():
 *
 * The validation specific to ip devices before associating an account to
 * ip device goes here. This function will associate the ip
 * with the account, the brand of the ip and the account is the same
 * input:
 *	i_flistp: the opcode input flist
 * output:
 *	ebuf is set if there is an error
 *****************************************************************************/
static void
fm_ip_pol_device_associate(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	int32		flags,
	pin_errbuf_t	*ebufp) {

	pin_flist_t	*flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	poid_t		*poidp = NULL;

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_associate Error ", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ip_pol_device_associate input flist", i_flistp);

	/*
	 * Get the device poid
	 */
	poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	/*
	 * do a read flds to get the device ip info
	 */
	flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, poidp, ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_STATE_ID, (void *)NULL, ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_ACCOUNT_OBJ, (void *)NULL, ebufp);

	/*
	 * call the read flds opcode
	 */
	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, flistp,
		&r_flistp, ebufp);

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_associate PCM_OP_READ_FLDS Error", ebufp);
		goto cleanup;
	}

	/*
	 * Check whether IP belong to the APN Passed and state of the APN
	 */
	fm_ip_apn_device_exist(ctxp, i_flistp, ebufp);

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_associate fm_ip_apn_device_exist Error", ebufp);
		goto cleanup;
	}

	/*
	 * Validate the brand obj of the ip device and the account is the same
	 */
	fm_ip_pol_device_check_brand(ctxp, i_flistp, r_flistp, ebufp);

	cleanup:
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&flistp, NULL);
	
	return;
}

/********************************************************************
 * This method will validate that the brand of the device
 * is the same as the brand of the account that will be
 * associated with the device.
 * It compares the brand object of the device with the brand obj
 * of the account
 * it Also compare the State of the Device
 * input :
 *	r_flistp: the flist containing the brand obj of the device
 *	cur_pdp: the brand poid of the account
 * output :
 *	ebuf is set if there is an error
 *
 ********************************************************************/
static void
fm_ip_pol_device_check_brand(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	*r_flistp,
	pin_errbuf_t 	*ebufp){

	int32		cur_state  = 0;    /* current state */
	int32		next_state  = 0;    /* next state */
	poid_t		*cur_pdp = NULL;	/* Current poid */
	pin_flist_t	*ri_flistp = NULL;	/* read flist	*/
	pin_flist_t	*ro_flistp = NULL;	/* result flist	*/
	pin_flist_t	*nsvc_flistp = NULL;	/* service element */
	pin_flist_t	*dev_rflistp = NULL;	/* device element */
	pin_flist_t	*dev_roflistp = NULL;	/* device o/p element */
	poid_t		*brand_pdp = NULL;	/* brand poid */
	poid_t		*acct_brand_pdp = NULL;	/* brand poid */
	poid_t		*dev_poidp = NULL;	/* device poid */
	int32		elem_id = 0;		/* element id */
	pin_cookie_t	cookie = NULL;		/* recurse array */
        void            *vp = NULL;            

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_check_brand Error ", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATE_ID, 0,
                        ebufp);
        if(vp != (void *)NULL)
              cur_state = *(int32 *)vp;

	/*
	 * Change the State of the Device to ALLOCATED if the device 
	 * present status is PIN_IP_NEW_STATE or PIN_IP_UNALLOCATED_STATE
	 */ 

	 if(cur_state == PIN_IP_NEW_STATE || cur_state == PIN_IP_UNALLOCATED_STATE){
		dev_rflistp = PIN_FLIST_CREATE(ebufp);		
		dev_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

		PIN_FLIST_FLD_SET(dev_rflistp, PIN_FLD_POID, dev_poidp, ebufp);

		next_state = PIN_IP_ALLOCATED_STATE;

		PIN_FLIST_FLD_SET(dev_rflistp, PIN_FLD_NEW_STATE, &next_state,
				ebufp);
		PIN_FLIST_FLD_SET(dev_rflistp, PIN_FLD_OLD_STATE, &cur_state,
				ebufp);

		PCM_OP(ctxp, PCM_OP_DEVICE_SET_STATE, 0, dev_rflistp,
			&dev_roflistp, ebufp);
		PIN_FLIST_DESTROY_EX(&dev_roflistp, NULL);
		PIN_FLIST_DESTROY_EX(&dev_rflistp, NULL);

		if(PIN_ERR_IS_ERR(ebufp)){
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_acct_asst DEVICE STATE CHANGE error", ebufp);
				
			return;
		}
	}
	/*
	 * if the Device state is in ALLOCATED state then through error message
	 */
	else if (cur_state != PIN_IP_ALLOCATED_STATE){  	
		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
		PIN_ERR_BAD_VALUE, PIN_FLD_STATE_ID, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_associate - "
			"device state is not new or allocated or unallocated  ", ebufp);
		return;
	}

	/*
	 * Get the device brand object
	 */
	acct_brand_pdp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_ACCOUNT_OBJ,
						0, ebufp);
	PIN_POID_SET_DB(acct_brand_pdp, cm_fm_get_current_db_no(ctxp));

	while((nsvc_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_SERVICES,
		&elem_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {

		/*
		 * get the account poid from service if
		 */
		cur_pdp = PIN_FLIST_FLD_GET(nsvc_flistp, PIN_FLD_ACCOUNT_OBJ, 0,
			ebufp);
	
		/*
		 * Get the brand of the account from the database.
		 */

		ri_flistp = PIN_FLIST_CREATE(ebufp);

		PIN_FLIST_FLD_SET(ri_flistp, PIN_FLD_POID, cur_pdp, ebufp);
		PIN_FLIST_FLD_SET(ri_flistp, PIN_FLD_BRAND_OBJ, NULL, ebufp);

		PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, ri_flistp, &ro_flistp, ebufp);

		brand_pdp = (poid_t *)PIN_FLIST_FLD_GET(ro_flistp,
				PIN_FLD_BRAND_OBJ, 0, ebufp);
		PIN_POID_SET_DB(brand_pdp, cm_fm_get_current_db_no(ctxp));

		if(!PIN_POID_IS_NULL(brand_pdp)) {
			if(PIN_POID_COMPARE(acct_brand_pdp, brand_pdp, 0, ebufp) != 0) {
				pin_set_err(ebufp, PIN_ERRLOC_FM,
					PIN_ERRCLASS_APPLICATION,
					PIN_ERR_PERMISSION_DENIED,
					PIN_FLD_BRAND_OBJ, 0, 0);
		
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_ip_pol_device_check_brand non-matching "
					"brand for device and account", ebufp);
					PIN_FLIST_DESTROY_EX(&ri_flistp, NULL);
					PIN_FLIST_DESTROY_EX(&ro_flistp, NULL);
				break;
			}
		}
		PIN_FLIST_DESTROY_EX(&ri_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&ro_flistp, NULL);
	}

	return;
}

/***************************************************************************
 * fm_ip_apn_device_exist()
 *
 * This method will check the database to see if there is the APN device
 * exists in the db with the given APN device poid. If not
 * then the ebuf is set.
 *  input :
 *         the input flist sent in through the opcode call
 *  output:
 *   ebuf   is set if the APN device doesn't exists
 ***************************************************************************/
static void
fm_ip_apn_device_exist(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp) {

	pin_flist_t	*ipr_flistp = NULL;    /* flist for read   */
	pin_flist_t	*ipro_flistp = NULL;  /* flist for output */
	pin_flist_t	*apnr_flistp = NULL;  /* flist for read   */
	pin_flist_t	*apnro_flistp = NULL; /* flist for output */
	pin_flist_t	*sub_flistp = NULL;   /* flist for output */
	poid_t		*ip_poid = NULL;      /* IP Poid */
	poid_t		*apn_poid = NULL;     /* IP Poid */
	int32		*state = NULL;        /* APN Device state */

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_apn_device_exist Error ", ebufp);
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	ip_poid = (char *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID,
			0, ebufp);
	PIN_POID_SET_DB(ip_poid, cm_fm_get_current_db_no(ctxp));

	if(!PIN_POID_IS_TYPE_ONLY(ip_poid)){

		ipr_flistp = PIN_FLIST_CREATE(ebufp);
		PIN_FLIST_FLD_SET(ipr_flistp, PIN_FLD_POID, ip_poid, ebufp);
		sub_flistp = PIN_FLIST_SUBSTR_ADD(ipr_flistp, PIN_FLD_DEVICE_IP, ebufp);
		PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_APN_OBJ, (void *)NULL, ebufp);

		/*
		 * Read the IP object
		 */
		PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, ipr_flistp, &ipro_flistp, ebufp);

		if((PIN_ERR_IS_ERR(ebufp)) || ipro_flistp == NULL){
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_ip_apn_device_exist APN does not exist" , ebufp);
			goto cleanup;
		}
	}
	else {
		pin_set_err(ebufp, PIN_ERRLOC_FM,PIN_ERRCLASS_APPLICATION,
			PIN_ERR_NOT_FOUND, PIN_FLD_POID, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_apn_device_exist wrong IP poid" , ebufp);
		goto cleanup;
		
	}	

	sub_flistp = PIN_FLIST_SUBSTR_GET(ipro_flistp, PIN_FLD_DEVICE_IP,
			0, ebufp);
	apn_poid = PIN_FLIST_FLD_GET (sub_flistp, PIN_FLD_APN_OBJ, 0, ebufp);
	PIN_POID_SET_DB(apn_poid, cm_fm_get_current_db_no(ctxp));

        apnr_flistp = PIN_FLIST_CREATE(ebufp);
	PIN_FLIST_FLD_SET(apnr_flistp, PIN_FLD_POID, apn_poid, ebufp);

	PIN_FLIST_FLD_SET(apnr_flistp, PIN_FLD_STATE_ID, (void *) NULL,
				ebufp);
	PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, apnr_flistp,
                        &apnro_flistp, ebufp);

	if((PIN_ERR_IS_ERR(ebufp)) || apnro_flistp == NULL){
		pin_set_err(ebufp, PIN_ERRLOC_FM,PIN_ERRCLASS_APPLICATION,
			PIN_ERR_NOT_FOUND,PIN_FLD_APN_OBJ, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_apn_device_exist APN does not exist" , ebufp);
		
		goto cleanup;
	}

	state = (int32 *)PIN_FLIST_FLD_GET(apnro_flistp, PIN_FLD_STATE_ID,
			0, ebufp);
	if( state != NULL ) {
		if( *state == PIN_APN_UNUSABLE_STATE ) {
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_INVALID_STATE,
				PIN_FLD_APN_OBJ, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				" fm_ip_apn_device_exist "
				" invalid state id", ebufp);
		}
	}

	cleanup:
		PIN_FLIST_DESTROY_EX(&ipro_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&ipr_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&apnro_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&apnr_flistp, NULL);
	return;
}

/*******************************************************************************
 * fm_ip_pol_device_disassociate()
 *
 * Inputs: input flist with device poid and services, flist with device poid
 *         and services from database
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function changes the state of the device to Unallocated if it is
 * currently Allocated and all of the current services are being disassociated.
 ******************************************************************************/
static void
fm_ip_pol_device_disassociate(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp) {

	int		svc_count = 0;        /* no of srvc objs on flist */
	int32		db_count = 0;         /* count of service in db */
	pin_flist_t	*srch_flistp = NULL;  /* search flist for counting */
	pin_flist_t	*res_flistp = NULL;   /* return flist */
	pin_flist_t	*state_flistp = NULL; /* flist for dev state */
	poid_t		*dev_poidp = NULL;    /* device poid  */
	void		*vp = NULL;           /* void ptr */
	int32		new_state = 0 ;       /* state of device*/
	int32		*old_state = NULL;    /*  old state of device*/


	 if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_disassociate Error ", ebufp);
		
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * count the number of service objects passed in
	 */
	svc_count = PIN_FLIST_ELEM_COUNT(i_flistp, PIN_FLD_SERVICES, ebufp);
	if(svc_count <= 0){
		/*
		 * no validation required
		 */
		return;
	}
	
	/*
	 * check for the number of services currently associated with the
	 * device
	 */
	srch_flistp = PIN_FLIST_CREATE(ebufp);
	
	dev_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_POID, dev_poidp, ebufp);
	
	PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_STATE_ID, (void *)NULL, ebufp);

	PIN_FLIST_ELEM_SET(srch_flistp, NULL, PIN_FLD_SERVICES, PIN_ELEMID_ANY,
				ebufp);

	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, srch_flistp,
		&res_flistp, ebufp);
	 if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_disassociate read fields error ", ebufp);
		goto cleanup;
	}

	db_count = PIN_FLIST_ELEM_COUNT(res_flistp, PIN_FLD_SERVICES, ebufp);
	
	/*
	 * now compare this count with the number of service objects on the
	 * incoming flist. if they are equal then change the device state to
	 * unassigned. Else return.
	 */ 

	 if (svc_count < db_count) {
		/*
		 * Not all of the services are being disassociated; do nothing
		 */
	}
	 else if(svc_count == db_count){
		old_state  = (int32 *) PIN_FLIST_FLD_GET(res_flistp, PIN_FLD_STATE_ID, 0, ebufp);
		/*
		 * Now move the device state to unassigned state
		 */
		
		if ( (old_state != NULL ? *old_state : 0) == PIN_IP_ALLOCATED_STATE) { 
			state_flistp = PIN_FLIST_CREATE(ebufp);

			PIN_FLIST_FLD_SET(state_flistp, PIN_FLD_POID, dev_poidp, ebufp);
			
			new_state = PIN_IP_UNALLOCATED_STATE;
			PIN_FLIST_FLD_SET(state_flistp, PIN_FLD_NEW_STATE, &new_state,
					ebufp);

			 PIN_FLIST_DESTROY_EX(&res_flistp, NULL);
		
			PCM_OP(ctxp, PCM_OP_DEVICE_SET_STATE, 0, state_flistp,
				 &res_flistp, ebufp);
		}	
	}
	else {
		pin_set_err(ebufp, PIN_ERRLOC_APP, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE, PIN_FLD_SERVICES, 0, svc_count);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_disassociate more input services "
			"than device currently has", ebufp);
	}

	cleanup:
		PIN_FLIST_DESTROY_EX(&res_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&state_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&srch_flistp, NULL);

	return;
}
