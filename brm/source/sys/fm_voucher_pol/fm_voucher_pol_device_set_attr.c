/*
 * @(#)% %
 *
* Copyright (c) 2002, 2019, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)% %";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#include "cm_fm.h"
#include "ops/voucher.h"
#include "pin_errs.h"
#include "pin_voucher.h"
#include "pin_device.h"
#include "ops/device.h"

#define FILE_LOGNAME "fm_voucher_pol_device_set_attr.c(1)"

EXPORT_OP void
op_voucher_pol_device_set_attr(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp);

extern void
fm_voucher_pol_utils_read_device_state(
        pcm_context_t   *ctxp,
        poid_t          *poidp,
        pin_flist_t     **r_flistpp,
        pin_errbuf_t    *ebufp);

extern void
fm_voucher_pol_utils_verify_deal(
        pcm_context_t   *ctxp,
        poid_t          *poidp,
        poid_t          *deal_poidp,
        pin_errbuf_t    *ebufp);


/**********************************************************************
 * Routines contained within
 **********************************************************************/
static void
fm_voucher_pol_device_set_attr(
	pcm_context_t   	*ctxp,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistp,
	pin_errbuf_t		*ebufp);

/**********************************************************************
 * Main routine for the PCM_OP_VOUCHER_POL_DEVICE_SET_ATTR operation.
 **********************************************************************/
void
op_voucher_pol_device_set_attr(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flp,
	pin_flist_t		**o_flpp,
	pin_errbuf_t		*ebufp) {

	
	pcm_context_t	*ctxp = connp->dm_ctx;
	pin_flist_t	*r_flp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/*********/
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_VOUCHER_POL_DEVICE_SET_ATTR) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
		PIN_ERRCLASS_SYSTEM_DETERMINATE,
		PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"bad opcode in op_voucher_pol_device_set_attr", ebufp);
		return;
	}

	/*
	 * DEBUG: What did we get?
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_voucher_pol_device_set_attr input flist", i_flp);

	/*
	 * do the real work
	 */
	fm_voucher_pol_device_set_attr(ctxp, i_flp, &r_flp, ebufp);

	/*
	 * Results.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		*o_flpp = (pin_flist_t *)NULL;
		PIN_FLIST_DESTROY_EX(&r_flp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_device_set_attr error", ebufp);
	}else {
		*o_flpp = r_flp;
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_voucher_pol_device_set_attr return flist", r_flp);
	}
	
	return;

}

/********************************************************************
 * fm_voucher_pol_device_set_attr():
 * This policy opcode will validate any attributes of a voucher device
 * that the user wants to modify. Currently the only validation is 
 * that the device id, voucher pin and voucher amount cannot be 
 * modified and if the user tries to do that the ebuf gets set
 * input : i_flistp	: the input flist contains the attributes that
 * 			are being modified
 * output: o_flistpp	: copy of the input flist. ebuf gets set in
 * 			case of an error
 ********************************************************************/
static void
fm_voucher_pol_device_set_attr(
	pcm_context_t   *ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	**o_flistpp,
	pin_errbuf_t	*ebufp) 
{
	void		*vp = NULL;	  /* void ptr	*/
	pin_flist_t	*sub_flistp;	  /* flist ptr	*/
	pin_flist_t	*r_flistp;	  /* flist ptr	*/
	poid_t		*dev_poidp;
	int32		*dev_state;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/********/
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * By default, we copy the input to the output..
	 */
	
	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);

	/*
	 * check if someone is trying to change the device id 
	 * if so, throw an error
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_DEVICE_ID, 1, ebufp);
	if(vp != (void *)NULL) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			 PIN_ERRCLASS_SYSTEM_DETERMINATE, PIN_ERR_BAD_ARG,
			 PIN_FLD_DEVICE_ID, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			" fm_voucher_pol_device_set_attr -cannot change device id",
			 ebufp);
	}
	
	/*
	 * check if someone is trying to change the voucher - if yes, set error
	 */
	sub_flistp = PIN_FLIST_SUBSTR_GET(i_flistp, PIN_FLD_DEVICE_VOUCHER, 1, 
		ebufp);
	if(sub_flistp != (void *)NULL) {

		/*
		 * Check for the voucher pin
		 */
		vp = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_VOUCHER_PIN, 1, ebufp);
		if (vp != (void *)NULL ) {
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_SYSTEM_DETERMINATE, PIN_ERR_BAD_ARG,
				PIN_FLD_VOUCHER_PIN, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_device_set_attr "
					"-cannot change voucher pin",ebufp);
		}

		/*
		 * Check for the voucher pin
		 */
		vp = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_DEAL_OBJ, 1, ebufp);
		if (vp != (void *)NULL ) {
			dev_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 1, ebufp);
			fm_voucher_pol_utils_read_device_state(ctxp, dev_poidp, &r_flistp, ebufp);	

			if (r_flistp != (pin_flist_t*)NULL) { 
				dev_state = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATE_ID, 1, ebufp);
				if ((dev_state != NULL) && (*dev_state != PIN_VOUCHER_NEW_STATE)) {
					pin_set_err(ebufp, PIN_ERRLOC_FM,
						PIN_ERRCLASS_SYSTEM_DETERMINATE, PIN_ERR_BAD_ARG,
						PIN_FLD_STATE_ID, 0, 0);
					PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						"fm_voucher_pol_device_set_attr "
					"device not new - cannot change voucher deal",ebufp);
					PIN_FLIST_DESTROY_EX(&r_flistp, NULL);	
					return;
		        	     /*************/
				}
			}
			if (vp != NULL) {
				fm_voucher_pol_utils_verify_deal(ctxp, dev_poidp, (poid_t*)vp, ebufp);
			}
		}	
	}
}
