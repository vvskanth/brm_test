/*
 * @(#)%Portal Version: fm_ip_pol_device_delete.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:53 %
 *
* Copyright (c) 2005, 2019, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_ip_pol_device_delete.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:53 %";
#endif

#include <pcm.h>
#include <pinlog.h>
#include "cm_fm.h"
#include "pin_errs.h"
#include "fm_utils.h"
#include "ops/ip.h"
#include "ops/device.h"
#include "pin_ip.h"
#include "pin_device.h"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_ip_pol_device_delete(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_ip_pol_device_delete(
	pcm_context_t		*ctxp,
        pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);


/*******************************************************************************
 * Entry routine for the PCM_OP_IP_POL_DEVICE_DELETE opcode
 ******************************************************************************/
void
op_ip_pol_device_delete(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp) {

	pcm_context_t		*ctxp = connp->dm_ctx;


	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_ip_pol_device_delete entry error", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_IP_POL_DEVICE_DELETE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_ip_pol_device_delete opcode error", ebufp);

		return;
	}

	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_ip_pol_device_delete input flist", i_flistp);

	/*
	 * Check the Device State 
	 */
	fm_ip_pol_device_delete(ctxp, i_flistp, ebufp);

	/*
	 * Error check
	 */

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_ip_pol_device_delete error", ebufp);
	}
	else {
		*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_ip_pol_device_delete return flist", *o_flistpp);
	}

	return;
}

/***************************************************************************
 * fm_ip_pol_device_delete()
 *
 * check the Staus of the IP device if the Device is in ALLOCATED
 * State through error message
 *
 * input: i_flistp: the input flist sent in through the opcode call
 *
 * output: ebuf is set if the IP device is in ALLOCATED state
 ***************************************************************************/
static void
fm_ip_pol_device_delete(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp) {

	void 			*vp =	NULL;		/*Void pointer*/
	pin_flist_t		*srch_flistp = NULL; 	/* serach flist*/
	pin_flist_t		*ret_flistp = NULL; 	/* result flist*/
	int32			*state_id = 0 ; 	/* Device state*/

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			 "fm_ip_pol_ip_delete Error ", ebufp);
		 return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);
	
	srch_flistp = PIN_FLIST_CREATE(ebufp);
	
	/*
	 * Get the values of the device state id from the database
	 */
	
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	
	if(PIN_ERR_IS_ERR(ebufp)) {
		 PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_delete Bad poid", ebufp);

		goto cleanup;
	}
	PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_POID, vp, ebufp);

	PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_STATE_ID, NULL, ebufp);

	PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, 
		srch_flistp, &ret_flistp, ebufp);

	if(PIN_ERR_IS_ERR(ebufp)) {
		 PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_delete read fields error ", ebufp);

		goto cleanup;
	}

	state_id = (int *)PIN_FLIST_FLD_GET(ret_flistp, PIN_FLD_STATE_ID, 0, ebufp);

	if(PIN_ERR_IS_ERR(ebufp)) {
		 PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_delete State id Does not exist", ebufp);

		goto cleanup;
	}

	if ( (state_id != NULL ? *state_id : 0) == PIN_IP_ALLOCATED_STATE) {

		pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_INVALID_STATE, PIN_FLD_STATE_ID, 0,
				*state_id);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_ip_pol_ip_delete State id Does not permit "
				" to Delete the IP device",ebufp);

		goto cleanup;
	}

	cleanup:
		PIN_FLIST_DESTROY_EX(&srch_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&ret_flistp, NULL);

	return;
}
