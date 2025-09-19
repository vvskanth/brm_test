/*
 * @(#)%Portal Version: fm_apn_pol_device_delete.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:39 %
 *
 * Copyright (c) 2005 - 2023 Oracle.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_apn_pol_device_delete.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:39 %";
#endif

#include <pcm.h>
#include <pinlog.h>
#include "cm_fm.h"
#include "pin_errs.h"
#include "fm_utils.h"
#include "ops/apn.h"
#include "ops/ip.h"
#include "ops/device.h"
#include "pin_apn.h"
#include "pin_ip.h"
#include "pin_device.h"

#define FILE_LOGNAME 		"fm_apn_pol_device_delete.c"
#define STEP_SEARCH_MAX		100


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_apn_pol_device_delete(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_apn_pol_device_delete(
	pcm_context_t		*ctxp,
        pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);

static void
fm_apn_pol_validate_ip_dev_state(
	pcm_context_t		*ctxp,
        pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);


/*******************************************************************************
 * Entry routine for the PCM_OP_APN_POL_DEVICE_DELETE opcode
 ******************************************************************************/
void
op_apn_pol_device_delete(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp) {

	pcm_context_t		*ctxp = connp->dm_ctx;


	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_apn_pol_device_delete entry error", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_APN_POL_DEVICE_DELETE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_apn_pol_device_delete opcode error", ebufp);

		return;
	}

	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_apn_pol_device_delete input flist", i_flistp);

	/*
	 * state check 
	 */
	fm_apn_pol_device_delete(ctxp, i_flistp, ebufp);

        /*
         * Error check
         */

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_apn_pol_device_delete error", ebufp);
	}
	 else {
		*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_apn_pol_device_delete return flist", *o_flistpp);
	}

	return;
}

/***************************************************************************
 * fm_apn_pol_device_delete()
 *
 * This method will pool all the IP Device using STEP Search and Delete
 * one by one by calling DEVICE Framework
 *
 * input: i_flistp: the input flist sent in through the opcode call
 *
 * output: ebuf is set if any of the IP device is in ALLOCATED state
 ***************************************************************************/
static void
fm_apn_pol_device_delete(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp) {

	pin_flist_t	*args_flistp = NULL; /* Search args flistp */
	pin_flist_t	*sr_flistp = NULL;   /* Ret search flist*/
	pin_flist_t	*res_flistp = NULL;   /* Result flist*/
	pin_flist_t	*sub_flistp = NULL;  /* Flist for substruct  */
	pin_flist_t	*s_flistp = NULL;    /* search flist */
	pin_flist_t	*dev_ip_flistp = NULL; /* flist for ip */
	pin_flist_t	*dev_ip_res_flistp = NULL; /* ip result flist */
	pin_flist_t	*step_flistp = NULL;  /* step result flist */
	poid_t		*pdp = NULL;         /* Routing poid */
	poid_t		*srch_poidp = NULL;  /* Search poid */
	poid_t		*dev_ip_poid = NULL;  /* device ip poid */
	int32		flag = 0;            /* Search flags */
	int64		dbno = 0;            /* Database no */
	int32		cred = 0;            /* Scoping credentials */
	int32		*rec_id = 0;         /* Element Id */
	char		*prog_name = NULL;   /* prog name*/
	pin_cookie_t	cookie = NULL;       /* cookie */
	int32		count	= 0;	     /* Record count */


	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                "fm_apn_pol_device_delete Error ", ebufp);

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
	 * Create the Argument Flist
	 */
	args_flistp = PIN_FLIST_CREATE(ebufp);

	pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	dbno = PIN_POID_GET_DB(pdp);

	srch_poidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

	PIN_FLIST_FLD_PUT(args_flistp, PIN_FLD_POID, srch_poidp, ebufp);
	PIN_FLIST_FLD_SET(args_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD (args_flistp, PIN_FLD_ARGS, 1, ebufp);
	s_flistp = PIN_FLIST_SUBSTR_ADD (sub_flistp, PIN_FLD_DEVICE_IP, ebufp);

	PIN_FLIST_FLD_SET (s_flistp, PIN_FLD_APN_OBJ, pdp, ebufp);

	/*
	 * Create the  Search Template
	 */

	PIN_FLIST_FLD_SET (args_flistp, PIN_FLD_TEMPLATE,
		(void *)"select X from /device/ip where  F1 = V1 ", ebufp);

	/*
	 * Set The number of record to be fetched at each search
	 */

	sub_flistp = PIN_FLIST_ELEM_ADD (args_flistp, PIN_FLD_RESULTS, STEP_SEARCH_MAX, ebufp);

	PIN_FLIST_FLD_SET (sub_flistp, PIN_FLD_POID, (void *)NULL, ebufp);


	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
			"fm_apn_pol_device_delete: Search Flist "
			" for Global Search", args_flistp);

	PCM_OP(ctxp, PCM_OP_STEP_SEARCH, 0, args_flistp, 
		&sr_flistp, ebufp);

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"fm_apn_pol_device_delete: step search error", ebufp);

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
				"fm_apn_pol_device_delete Step Search Error", ebufp);
			goto cleanup;
		}

		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"fm_apn_pol_device_delete Step Search Result",sr_flistp);
	}
	/*
	 * End the Step Search
	 */
	PCM_OP(ctxp, PCM_OP_STEP_END, 0, args_flistp, &sr_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_ip_delete Step Search End Error", ebufp);
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
	 * Get all the IP Device POID one by one and Call device framework to Delete
	 */
	while((step_flistp = PIN_FLIST_ELEM_GET_NEXT (res_flistp, PIN_FLD_RESULTS,
				rec_id, 1, &cookie, ebufp)) != NULL) {

		dev_ip_poid = PIN_FLIST_FLD_GET(step_flistp, PIN_FLD_POID, 
				0, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_apn_pol_device_delete invalid IP poid ", ebufp);
				goto cleanup;
		}

		if (PIN_POID_IS_NULL(dev_ip_poid)) {
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION, PIN_ERR_BAD_VALUE, 
				PIN_FLD_POID, 0, 0);

			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_apn_pol_device_delete error : "
					"Invlid ip Poid ", ebufp);
	
				goto cleanup;
		}
	
		PIN_FLIST_FLD_SET(dev_ip_flistp, PIN_FLD_POID, dev_ip_poid, ebufp);
			
		/*
	   	 * Call The Device Framework to Delete the IP Device
		 */

		PCM_OP(ctxp, PCM_OP_DEVICE_DELETE, 0, dev_ip_flistp, 
			&dev_ip_res_flistp, ebufp);
	
		PIN_FLIST_DESTROY_EX(&dev_ip_res_flistp, NULL);
	
		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_apn_pol_device_delete error : "
				"in ip delete", ebufp);

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
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp) {

	pin_flist_t	*srch_flistp = NULL;    /* Search flist         */
	pin_flist_t	*sr_flistp = NULL;      /* Return flistp search */
	pin_flist_t	*sub_flistp = NULL;     /* Flist for substruct  */
	pin_flist_t	*s_flistp = NULL;       /* Flist for device_ip  */
	pin_flist_t	*res_flistp = NULL;     /* Flist for result substruct  */
	poid_t		*pdp = NULL;            /* Routing poid         */
	poid_t		*srch_poidp = NULL;     /* Search poid          */
	int32		flag = 0;               /* Search flags         */
	int64		dbno = 0;               /* Database no          */
	int32		cred = 0;               /* Scoping credentials  */
	int32		count=0;		/* Count from search    */
	int32		state_id = PIN_IP_ALLOCATED_STATE;       /*IP State*/
	pin_cookie_t	cookie = NULL;          /* cookie to recurse array */

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_ip_dev_validate_state entry point", ebufp);
		goto cleanup;
	}
	
	PIN_ERR_CLEAR_ERR(ebufp);
	
	srch_flistp = PIN_FLIST_CREATE(ebufp);
	
	pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
		"fm_apn_pol_validate_ip_dev_state - error getting poid ");

		goto cleanup;
	}

	dbno = PIN_POID_GET_DB(pdp);
		
	srch_poidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

	PIN_FLIST_FLD_PUT(srch_flistp, PIN_FLD_POID, srch_poidp, ebufp);
	PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);

	/*
	 * Add the APN Obj
	 */

	sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_ARGS, 1, ebufp);
	s_flistp = PIN_FLIST_SUBSTR_ADD(sub_flistp, PIN_FLD_DEVICE_IP, ebufp);

	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_APN_OBJ, pdp, ebufp);

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
			"delete failed for APN as one or more "
			"IP devices were in the Allocated state ",
			ebufp);
	}

	cleanup:
		PIN_FLIST_DESTROY_EX(&srch_flistp, NULL);
		 PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);
	return;
}
