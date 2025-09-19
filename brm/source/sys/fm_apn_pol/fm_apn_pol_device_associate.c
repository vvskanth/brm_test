/*
 *  @(#)%Portal Version: fm_apn_pol_device_associate.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:44 %
 *
* Copyright (c) 2005, 2019, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_apn_pol_device_associate.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:44 %";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "cm_fm.h"
#include "ops/apn.h"
#include "ops/device.h"
#include "pin_errs.h"
#include "pin_apn.h"
#include "pin_device.h"
#include "fm_utils.h"

#define FILE_LOGNAME "fm_apn_pol_device_associate.c"

EXPORT_OP void
op_apn_pol_device_associate(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp);

#define FILE_SOURCE_ID	"fm_apn_pol_device_associate.c(1.2)"

/**********************************************************************
 * Routines contained within
 **********************************************************************/

static void
fm_apn_pol_device_associate(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	int32		flags,
	pin_errbuf_t	*ebufp);

static void
fm_apn_pol_device_check_brand(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	*r_flistp,
	pin_errbuf_t 	*ebufp);

/**********************************************************************
 * Main routine for the PCM_OP_APN_POL_DEVICE_ASSOCIATE operation.
 **********************************************************************/
void
op_apn_pol_device_associate(
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
			"op_apn_pol_device_associate Error ", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if(opcode != PCM_OP_APN_POL_DEVICE_ASSOCIATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_apn_pol_device_associate", ebufp);
		return;
	}

	/*
	 * DEBUG: What did we get?
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_apn_pol_device_associate input flist", i_flistp);

	/*
	 * check for association  of service
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_FLAGS, 0, ebufp);
	if(vp != (void *)NULL){
		assoc_flag = *(int32 *)vp;
	}
	else {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			" op_apn_pol_device_associate Associate flag"
			" error", ebufp);
		return;
	}

	if((assoc_flag & PIN_DEVICE_FLAG_ASSOCIATE) ==
		 PIN_DEVICE_FLAG_ASSOCIATE){
		/*
		 * association, do the required validation
		 */
		fm_apn_pol_device_associate(ctxp, i_flistp, 
				flags, ebufp);
	}

	/*
	 * Results.
	 */
	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_apn_pol_device_associate error", ebufp);
	}
	else {
		*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_apn_pol_device_assoicate return flist", *o_flistpp);
	}

	return;
}

/****************************************************************************
 * fm_apn_pol_device_associate():
 *
 * The validation specific to apn devices before associating an account to
 * apn device goes here. This function will associate the apn
 * with the account, the brand of the apn and the account is the same
 * input:
 *	i_flistp: the opcode input flist
 * output:
 *	ebuf is set if there is an error
 *****************************************************************************/
static void
fm_apn_pol_device_associate(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	int32		flags,
	pin_errbuf_t	*ebufp) {

	pin_flist_t	*flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	poid_t		*poidp = NULL;

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_device_associate Error ", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Get the device poid
	 */
	poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	/*
	 * do a read flds to get the device apn info
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
			"fm_apn_pol_device_associate PCM_OP_READ_FLDS Error ", ebufp);
		goto cleanup;
	}

	/*
	 * Validate the brand obj of the apn device and the account is the same
	 */
	fm_apn_pol_device_check_brand(ctxp, i_flistp, r_flistp, ebufp);

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
fm_apn_pol_device_check_brand(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	*r_flistp,
	pin_errbuf_t 	*ebufp){

	int32		*state  = NULL;    /* state ptr */
	poid_t		*cur_pdp = NULL;	/* cur poid */
	pin_flist_t	*ri_flistp = NULL;	/* read flist	*/
	pin_flist_t	*ro_flistp = NULL;	/* result flist	*/
	pin_flist_t	*nsvc_flistp = NULL;	/* service element */
	poid_t		*brand_pdp = NULL;	/* brand poid */
	poid_t		*acct_brand_pdp = NULL;	/* brand poid */
	int32		elem_id = 0;		/* element id */
	pin_cookie_t	cookie = NULL;		/* recurse array */

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_device_check_brand Error ", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	state = (int32 *)PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATE_ID, 0,
				ebufp);

	if (state && *state == PIN_APN_UNUSABLE_STATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE, PIN_FLD_STATE_ID, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_device_associate - "
			"device state is unusable", ebufp);
		return;
	}

	/*
	 * Get the device brand object which will be stored in the Account
	 */
	acct_brand_pdp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_ACCOUNT_OBJ,
						0, ebufp);

	while((nsvc_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_SERVICES,
		&elem_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {

		/*
		 * get the account poid from service if any
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


		if(!PIN_POID_IS_NULL(brand_pdp)) {
			if(PIN_POID_COMPARE(acct_brand_pdp, brand_pdp, 0, ebufp) != 0) {
				PIN_FLIST_DESTROY_EX(&ri_flistp, NULL);
				PIN_FLIST_DESTROY_EX(&ro_flistp, NULL);
				pin_set_err(ebufp, PIN_ERRLOC_FM,
					PIN_ERRCLASS_APPLICATION,
					PIN_ERR_PERMISSION_DENIED,
					PIN_FLD_BRAND_OBJ, 0, 0);
		
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_apn_pol_device_check_brand non-matching "
					"brand for device and account", ebufp);
				break;
			}
		}
		PIN_FLIST_DESTROY_EX(&ri_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&ro_flistp, NULL);
	}

	return;
}
