/*
 * @(#)%Portal Version: fm_sim_pol_device_set_attr.c:WirelessVelocityInt:1:2006-Sep-14 11:24:57 %
 *
 * Copyright (c) 2001 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#) %full_filespec: fm_sim_pol_device_set_attr.c~4:csrc:1 %";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#include "cm_fm.h"
#include "ops/sim.h"
#include "pin_errs.h"
#include "pin_sim.h"
#include "pin_device.h"
#include "ops/device.h"

#define FILE_LOGNAME "fm_sim_pol_device_set_attr.c(1)"

EXPORT_OP void
op_sim_pol_device_set_attr(
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
fm_sim_pol_device_set_attr(
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistp,
	pin_errbuf_t		*ebufp);

/**********************************************************************
 * Main routine for the PCM_OP_SIM_POL_DEVICE_SET_ATTR operation.
 **********************************************************************/
void
op_sim_pol_device_set_attr(
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
	if (opcode != PCM_OP_SIM_POL_DEVICE_SET_ATTR) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
		PIN_ERRCLASS_SYSTEM_DETERMINATE,
		PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"bad opcode in op_sim_pol_device_set_attr", ebufp);
		return;
	}

	/*
	 * DEBUG: What did we get?
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_sim_pol_device_set_attr input flist", i_flp);

	/*
	 * do the real work
	 */
	fm_sim_pol_device_set_attr(i_flp, &r_flp, ebufp);

	/*
	 * Results.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		*o_flpp = (pin_flist_t *)NULL;
		PIN_FLIST_DESTROY_EX(&r_flp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_sim_pol_device_set_attr error", ebufp);
	}else {
		*o_flpp = r_flp;
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_sim_pol_device_set_attr return flist", r_flp);
	}
	
	return;

}

/********************************************************************
 * fm_sim_pol_device_set_attr():
 * This policy opcode will validate any attributes of a sim device
 * that the user wants to modify. Currently the only validation is 
 * that the device id cannot be modified and if the user tries to do
 * that the ebuf gets set
 * input : i_flistp	: the input flist contains the attributes that
 * 			are being modified
 * output: o_flistpp	: copy of the input flist. ebuf gets set in
 * 			case of an error
 ********************************************************************/
static void
fm_sim_pol_device_set_attr(
	pin_flist_t	*i_flistp,
	pin_flist_t	**o_flistpp,
	pin_errbuf_t	*ebufp) {



	void		*vp = NULL;	  /* void ptr	*/


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/********/
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * By default, we simply copy the input to the output..
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
			" fm_sim_pol_device_set_attr -cannot change device id",
			 ebufp);
	}

}
