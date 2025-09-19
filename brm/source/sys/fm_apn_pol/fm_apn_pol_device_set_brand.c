/*
 * @(#)%Portal Version: fm_apn_pol_device_set_brand.c:ServerIDCVelocityInt:1:2006-Sep-14 11:31:19 %
 *
 * Copyright (c) 2004 - 2023 Oracle.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_apn_pol_device_set_brand.c:ServerIDCVelocityInt:1:2006-Sep-14 11:31:19 %";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/apn.h"
#include "ops/device.h"
#include "pin_apn.h"
#include "pin_ip.h"
#include "cm_fm.h"
#include "pin_errs.h"

#define FILE_LOGNAME		"fm_apn_pol_device_set_brand.c"
#define STEP_SEARCH_MAX		100


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_apn_pol_device_set_brand(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_apn_pol_device_set_brand(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);

static void
fm_apn_pol_set_ip_brand(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);	

static void
fm_apn_pol_validate_account_obj(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);	

static void
fm_apn_pol_validate_ip_dev_state(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp);

static void
fm_apn_pol_validate_duplicate_brand(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp);

/*******************************************************************************
 * Entry routine for the PCM_OP_APN_POL_DEVICE_SET_BRAND opcode
 ******************************************************************************/
void
op_apn_pol_device_set_brand(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp) {

	pcm_context_t		*ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_apn_pol_device_set_brand Error ", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_APN_POL_DEVICE_SET_BRAND) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_apn_pol_device_set_brand opcode error", ebufp);

		return;
	}

	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_apn_pol_device_set_brand input flist", i_flistp);

	/*
	 * Do Brand check for the Device
	 */
	fm_apn_pol_device_set_brand(ctxp, i_flistp, ebufp);

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_apn_pol_device_set_brand error", ebufp);
		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
	} 
	else {
		*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_apn_pol_device_set_brand return flist", *o_flistpp);
	}

	return;
}

/*******************************************************************************
 * fm_apn_pol_device_set_brand()
 *
 * Inputs: flist with /device/apn poid and account_obj
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function validates the input flist has a poid of type '/device/apn',
 * then validates the account_obj.
 ******************************************************************************/
static void
fm_apn_pol_device_set_brand(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp) {


	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_device_set_brand Error ", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * If the account_obj field is provided, validate it
	 */
	if(PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACCOUNT_OBJ, 1, ebufp) != NULL) {

		fm_apn_pol_validate_account_obj(ctxp, i_flistp, ebufp);

		if(PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_device_set_brand Validate Error", ebufp);
		}
		else
		{
			/*
                         * Check for possible duplicate of APN and Account combination
                         */
			fm_apn_pol_validate_duplicate_brand(ctxp, i_flistp, ebufp);
                        if(PIN_ERR_IS_ERR(ebufp)) {
                                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "fm_apn_pol_device_set_brand Duplicate APN", ebufp);
                        }

		 	/*
		  	 * Move the Brand of IP Devices belonging to APN to new Brand
		  	 * If the ip Device state is New
		  	 */
			fm_apn_pol_set_ip_brand(ctxp, i_flistp, ebufp);
			if(PIN_ERR_IS_ERR(ebufp)) {
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				" fm_apn_pol_device_set_brand ip "
				" set Brand error ", ebufp);
			}
		}
	}
	else
	{
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION, PIN_ERR_MISSING_ARG,
			PIN_FLD_ACCOUNT_OBJ, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_apn_pol_device_set_brand Account Obj not found", ebufp);
	}


	return;
}

/*******************************************************************************
 * fm_apn_pol_validate_account_obj()
 *
 * Inputs: flist with account_obj an flist with db values for the /device/apn
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This fuction compate the Brand of the device and the Account object
 * 
 ******************************************************************************/
static void
fm_apn_pol_validate_account_obj(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp) {

	pin_flist_t	*flistp = NULL;		/* flist for rdflds */
	pin_flist_t	*r_flistp = NULL;	/* flist for rdflds */
	poid_t		*ia_poidp = NULL;	/* input act obj */
	poid_t		*dba_poidp = NULL;	/* act obj from db */
	int32		state_id = 0;		/* state_id from db */
	void		*vp = NULL;		/* void ptr*/

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_validate_account_obj Error ", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);


	flistp = PIN_FLIST_CREATE(ebufp);
	/*
	 * Get the values of the device state id from the database
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, vp, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_STATE_ID, NULL, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_ACCOUNT_OBJ, NULL, ebufp);

	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, flistp, &r_flistp,
		ebufp);

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_validate_account_obj read fields error ", ebufp);
		PIN_FLIST_DESTROY_EX(&flistp, NULL);

		return;
	}
	

	/*
	 * Compare existing account_obj with input value
	 */
	ia_poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, 
		PIN_FLD_ACCOUNT_OBJ, 0, ebufp);
	dba_poidp = (poid_t *)PIN_FLIST_FLD_GET(r_flistp, 
		PIN_FLD_ACCOUNT_OBJ, 0, ebufp);

	if (PIN_POID_COMPARE(ia_poidp, dba_poidp, 0, ebufp) != 0) {
		/*
		 * Changing the brand of the number is restricted to objects
		 * with a state_id of:
		 *
		 *   PIN_APN_NEW_STATE
		 *   PIN_APN_USABLE_STATE
		 * States outside of this list will generate an error.
		 */
		vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATE_ID, 0, ebufp);
		if(vp != NULL){
			state_id = *(int32 *)vp;

			switch(state_id) { 
				case PIN_APN_NEW_STATE :
				case PIN_APN_USABLE_STATE :
					break;
				default:
					/*
					 * validation fails 
					 */
					pin_set_err(ebufp, PIN_ERRLOC_FM,
						PIN_ERRCLASS_APPLICATION,
						PIN_ERR_PROHIBITED_ARG, 
						PIN_FLD_ACCOUNT_OBJ,
						 0, 0);
					PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR, 
						"cannot change device "
						" ownership when "
						"device is not in new or usable state ",
						 ebufp);
					break;
			}
		}
	}
	else
	{
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_WARNING,
			"Brands are same, returning");
	}

	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	return;
}

/*******************************************************************************
 * fm_apn_pol_set_ip_brand()
 *
 * Inputs: flist with /device/apn poid and account_obj
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * this function will call the PCM_OP_DEVICE_SET_BRAND in loop if any ip device 
 * exist with the apn in a loop it will roll back if any of the ip device is in 
 * assigned state or returned state.
 *
 ******************************************************************************/
static void
fm_apn_pol_set_ip_brand(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp) {

	pin_flist_t	*args_flistp = NULL; /* Search args flistp */
	pin_flist_t	*sr_flistp = NULL;   /* Ret search flist*/
	pin_flist_t	*res_flistp = NULL;   /* Result flist*/
	pin_flist_t	*sub_flistp = NULL;  /* Flist for substruct  */
	pin_flist_t	*s_flistp = NULL;    /* search flist */
	pin_flist_t	*dev_ip_flistp = NULL; /* flist for ip */
	pin_flist_t	*dev_ip_res_flistp = NULL; /* ip result flist */
	pin_flist_t	*step_flistp = NULL;  /* step result flist */
	poid_t		*rout_poidp = NULL;         /* Routing poid */
	poid_t		*srch_poidp = NULL;  /* Search poid */
	poid_t		*acct_poidp = NULL;  /* Brand poid */
	poid_t		*dev_ip_poid = NULL;  /* device ip poid */
	int32		flag = 0;            /* Search flags */
	int32		count   = 0;         /* Record count */
	int32		*rec_id = 0;         /* Element Id */
	int64		dbno = 0;            /* Database no */
	char		*prog_name = NULL;   /* prog name*/
	pin_cookie_t	cookie = NULL;       /* cookie */


	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"fm_apn_pol_set_ip_brand Error ", ebufp);

		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * check the Status of the IP device if any of the Device
	 * is in Allocated State through error Message
	 */

	fm_apn_pol_validate_ip_dev_state(ctxp, i_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"fm_apn_pol_device_delete Invlid state", ebufp);

		return;
	}

	/*
	 * Create the Argument Flist for Step Search
	 */
	args_flistp = PIN_FLIST_CREATE(ebufp);

	rout_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	dbno = PIN_POID_GET_DB(rout_poidp);

	srch_poidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

	PIN_FLIST_FLD_PUT(args_flistp, PIN_FLD_POID, srch_poidp, ebufp);
	PIN_FLIST_FLD_SET(args_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD (args_flistp, PIN_FLD_ARGS, 1, ebufp);
	s_flistp = PIN_FLIST_SUBSTR_ADD (sub_flistp, PIN_FLD_DEVICE_IP, ebufp);

	PIN_FLIST_FLD_SET (s_flistp, PIN_FLD_APN_OBJ, rout_poidp, ebufp);

	/*
	 * Create the  Search Template
	 */

	PIN_FLIST_FLD_SET (args_flistp, PIN_FLD_TEMPLATE,
		(void *)"select X from /device/ip where  F1 = V1 ", ebufp);

	/*
	 * Set The number of record to be fetched at each search
	 */

	sub_flistp = PIN_FLIST_ELEM_ADD (args_flistp, PIN_FLD_RESULTS, 
			STEP_SEARCH_MAX, ebufp);

	PIN_FLIST_FLD_SET (sub_flistp, PIN_FLD_POID, (void *)NULL, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"fm_apn_pol_set_ip_brand: Search Flist "
			" for Global Search", args_flistp);

	PCM_OP(ctxp, PCM_OP_STEP_SEARCH, 0, args_flistp, &sr_flistp, ebufp);

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"fm_apn_pol_set_ip_brand: step search error", ebufp);

		goto cleanup;
	}

	res_flistp = PIN_FLIST_CREATE(ebufp);

	/*
	 * Collect the Step Search result
	 */

	while (1) {
		count = PIN_FLIST_ELEM_COUNT(sr_flistp, PIN_FLD_RESULTS, ebufp);

		if (count != 0 ) {
			PIN_FLIST_CONCAT(res_flistp, sr_flistp, ebufp);
		}

		PIN_FLIST_DESTROY_EX (&sr_flistp, NULL);

		if (count < STEP_SEARCH_MAX) {
			 break;
		}
		PCM_OP(ctxp, PCM_OP_STEP_NEXT, 0, args_flistp,
			 &sr_flistp, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_apn_pol_set_ip_brand Step Search Error", ebufp);
			goto cleanup;
		}
	}

	/*
	 * End the Step Search
	 */
	PCM_OP(ctxp, PCM_OP_STEP_END, 0, args_flistp, &sr_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_set_ip_brand Step Search End Error", ebufp);
		goto cleanup;
	}

	/*
	 * Set the program name
	 */

	prog_name = (char *) PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_PROGRAM_NAME, 1, ebufp);

	if ( prog_name == NULL ) {
		prog_name = FILE_LOGNAME;
	}

	dev_ip_flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET(dev_ip_flistp, PIN_FLD_PROGRAM_NAME, (void *)prog_name, ebufp);

	/*
	 * Get the Account object which will contain the Brand information
	 */

	acct_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACCOUNT_OBJ, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_set_ip_brand Missing Brand Information ", ebufp);
		goto cleanup;
	}

	PIN_FLIST_FLD_SET(dev_ip_flistp, PIN_FLD_ACCOUNT_OBJ, acct_poidp, ebufp);

	/*
	 * Get all the IP Device POID one by one and Call device framework to Set brand
	 */
	while((step_flistp = PIN_FLIST_ELEM_GET_NEXT (res_flistp, PIN_FLD_RESULTS,
				rec_id, 1, &cookie, ebufp)) != NULL) {
		
		dev_ip_poid = PIN_FLIST_FLD_GET(step_flistp, PIN_FLD_POID,
				0, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_apn_pol_set_ip_brand invalid IP poid ", ebufp);
				goto cleanup;
		}

		if (PIN_POID_IS_NULL(dev_ip_poid)) {
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION, PIN_ERR_BAD_VALUE,
				PIN_FLD_POID, 0, 0);

			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_apn_pol_set_ip_brand error : "
					"Invlid ip Poid ", ebufp);

				goto cleanup;
		}

		PIN_FLIST_FLD_SET(dev_ip_flistp, PIN_FLD_POID, dev_ip_poid, ebufp);

		/*
		 * Call The Device Framework to change brand of the IP Device
		 */

		PCM_OP(ctxp, PCM_OP_DEVICE_SET_BRAND, 0, dev_ip_flistp,
			&dev_ip_res_flistp, ebufp);

		PIN_FLIST_DESTROY_EX(&dev_ip_res_flistp, NULL);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_apn_pol_set_ip_brand error : "
				"in ip set brand", ebufp);

			goto cleanup;
		}
	}

	cleanup:
		PIN_FLIST_DESTROY_EX(&args_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&dev_ip_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&res_flistp, NULL);

	return;

}


/*******************************************************************************
 * fm_apn_pol_validate_ip_dev_state()
 *
 * Inputs: flist with apn poid
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function searches for all the ip_objs for the given apn poid
 * Validate to ensure that none of the ip_obj's state is in "Allocated" state.
 *
 * Possible Customizations:
 * This function can be customized to support new states, or to loosen the
 * state-specific constraints applied within.
 ******************************************************************************/
static void
fm_apn_pol_validate_ip_dev_state(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp) {

	pin_flist_t	*srch_flistp = NULL;    /* Search flist */
	pin_flist_t	*sr_flistp = NULL;      /* Return flistp search */
	pin_flist_t	*sub_flistp = NULL;     /* Flist for substruct */
	pin_flist_t	*s_flistp = NULL;       /* Flist for device_ip	*/
	pin_flist_t	*res_flistp = NULL;     /* Flist for result substruct */
	poid_t		*rout_poidp = NULL;            /* Routing poid */
	poid_t		*srch_poidp = NULL;     /* Search poid */
	int32		flag = 0;               /* Search flags */
	int64		dbno = 0;               /* Database no  */
	int32		count=0;                /* Count from search */
	int32		state_id = PIN_IP_ALLOCATED_STATE; /*IP State*/
	pin_cookie_t	cookie = NULL;		/* cookie to recurse array */

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_ip_dev_validate_state entry point", ebufp);
		goto cleanup;
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	srch_flistp = PIN_FLIST_CREATE(ebufp);

	rout_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
		"fm_apn_pol_validate_ip_dev_state - error getting poid ");

		goto cleanup;
	}

	dbno = PIN_POID_GET_DB(rout_poidp);
	srch_poidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

	PIN_FLIST_FLD_PUT(srch_flistp, PIN_FLD_POID, srch_poidp, ebufp);
	PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);

	/*
	 * Add the APN Obj
	 */

	sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_ARGS, 1, ebufp);
	s_flistp = PIN_FLIST_SUBSTR_ADD(sub_flistp, PIN_FLD_DEVICE_IP, ebufp);

	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_APN_OBJ, rout_poidp, ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_ARGS, 2, ebufp);
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_STATE_ID, &state_id, ebufp);

	PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_TEMPLATE,
		(void *)"select X from /device/ip where F1 = V1 and F2 = V2 ", ebufp);

	PIN_FLIST_ELEM_SET(srch_flistp, NULL, PIN_FLD_RESULTS, 0, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_apn_pol_validate_ip_dev_state srch input flist", srch_flistp);

	PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, PCM_OPFLG_COUNT_ONLY,
		srch_flistp, &sr_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_validate_ip_dev_state "
			"PCM_OP_GLOBAL_SEARCH error ",  ebufp);

		goto cleanup;
	}

	PIN_FLIST_ELEM_GET_NEXT(sr_flistp, PIN_FLD_RESULTS,
		&count, 1, &cookie, ebufp);

	 if (count > 0) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE, PIN_FLD_STATE_ID, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_validate_ip_dev_state "
			"Set brand failed for APN as one or more "
			"IP devices were in the Allocated state ",
			ebufp);
	}

	cleanup:
		PIN_FLIST_DESTROY_EX(&srch_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);
	return;
}

/*******************************************************************************
 * fm_apn_pol_validate_duplicate_brand()
 *
 * Inputs: flist with apn poid
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This method will search the database to check for the duplicate apn device id
 * with the target brand while moving an APN from one brand to another. If there is
 * any duplicate id, then an error is thrown and then the ebuf is set.
 *
 ******************************************************************************/
static void
fm_apn_pol_validate_duplicate_brand(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp) {

        pin_flist_t     *srch_flistp = NULL;    /* Search flist         */
        pin_flist_t     *sr_flistp = NULL;      /* Return flistp search */
        pin_flist_t     *sub_flistp = NULL;     /* Flist for substruct  */
        pin_flist_t     *res_flistp = NULL;     /* Flist for result substruct*/
        poid_t          *pdp = NULL;            /* Routing poid         */
        poid_t          *srch_poidp = NULL;     /* Search poid          */
        char            *dev_id = NULL;         /* APN_device_ID        */
        int32           flag = 0;               /* Search flags         */
        int64           dbno = 0;               /* Database no          */
        int32           cred = 0;               /* Scoping credentials  */
        poid_t          *pdp_apn = NULL;         /* APN poid             */

        poid_t          *ia_poidp = NULL;   /* input act obj */
        pin_flist_t     *flistp = NULL;     /* Flist for input to read flds  */
        pin_flist_t     *r_flistp = NULL;   /* Flist for output from read flds*/

        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_apn_pol_validate_duplicate_brand entry point", ebufp);
                goto cleanup;
        }

        PIN_ERR_CLEAR_ERR(ebufp);

        flistp = PIN_FLIST_CREATE(ebufp);
        /*
         * Get the values of the device id from the database
         */
        pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
        PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, pdp, ebufp);
        PIN_FLIST_FLD_SET(flistp, PIN_FLD_DEVICE_ID, NULL, ebufp);

        PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, flistp, &r_flistp,
                ebufp);
        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_apn_pol_validate_duplicate_brand "
                        "error from read flds "
                        , ebufp);
                goto cleanup;
        }

        /*
         * Get the Device id from the result flist
         */
        dev_id = (char *)PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_DEVICE_ID,
                                                0, ebufp);

        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "error in getting device id" , ebufp);
                goto cleanup;
        }

        srch_flistp = PIN_FLIST_CREATE(ebufp);

        /*
         * get account_obj from input flist
         */
        ia_poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACCOUNT_OBJ, 0, ebufp);
	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "error in getting account obj" , ebufp);
                goto cleanup;
        }

	dbno = PIN_POID_GET_DB(pdp);
        srch_poidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

        PIN_FLIST_FLD_PUT(srch_flistp, PIN_FLD_POID, srch_poidp, ebufp);
        PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);

        sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_ARGS, 1, ebufp);
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_ACCOUNT_OBJ, ia_poidp, ebufp);

        sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_ARGS, 2, ebufp);
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_DEVICE_ID, dev_id, ebufp);

        PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_TEMPLATE,
                (void *)"select X from /device/apn where F1 = V1 and F2 = V2 ", ebufp);

        sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_RESULTS, 0, ebufp);
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_POID, NULL, ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_apn_pol_validate_duplicate_brand srch input flist", srch_flistp);

        /*
         * turn off scoping just before the search and turn it on right after
         */
        cred = CM_FM_BEGIN_OVERRIDE_SCOPE(CM_CRED_SCOPE_OVERRIDE_ROOT);

        PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, 0, srch_flistp, &sr_flistp, ebufp);


        /* turn the scoping back on */
        CM_FM_END_OVERRIDE_SCOPE(cred);

        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "fm_apn_pol_validate_duplicate_brand "
                                "PCM_OP_GLOBAL_SEARCH error ",
                                        ebufp);
                goto cleanup;
        }

        if((res_flistp = PIN_FLIST_FLD_GET(sr_flistp, PIN_FLD_RESULTS,
                                                1, ebufp)) != NULL)
        {
                pdp_apn = PIN_FLIST_FLD_GET(res_flistp, PIN_FLD_POID, 0, ebufp);

                if (PIN_ERR_IS_ERR(ebufp)) {

                        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                                "fm_apn_pol_validate_set_attr - error getting poid ");
                        goto cleanup;
                }

                /* there are duplicates */
                pin_set_err(ebufp, PIN_ERRLOC_FM,
                                PIN_ERRCLASS_APPLICATION,
                                PIN_ERR_DUPLICATE, PIN_FLD_DEVICE_ID, 0, 0);

                PIN_ERR_LOG_POID(PIN_ERR_LEVEL_ERROR,
                                "fm_apn_pol_validate_duplicate_brand - "
                                "APN device already exists in target brand: ",
                                pdp_apn);
        }


cleanup:

        PIN_FLIST_DESTROY_EX(&srch_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&flistp, NULL);

        return;

}
