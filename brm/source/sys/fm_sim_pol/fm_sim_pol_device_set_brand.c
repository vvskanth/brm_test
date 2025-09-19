/*
 * @(#)%Portal Version: fm_sim_pol_device_set_brand.c:WirelessVelocityInt:2:2006-Sep-14 11:25:02 %
 *
 * Copyright (c) 2001 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_sim_pol_device_set_brand.c:WirelessVelocityInt:2:2006-Sep-14 11:25:02 %";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/sim.h"
#include "pin_sim.h"
#include "cm_fm.h"
#include "pin_errs.h"

#define FILE_LOGNAME "fm_sim_pol_device_set_brand.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_sim_pol_device_set_brand(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_sim_pol_device_set_brand(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);

static void
fm_sim_pol_validate_account_obj(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);	


/*******************************************************************************
 * Entry routine for the PCM_OP_SIM_POL_DEVICE_SET_BRAND opcode
 ******************************************************************************/
void
op_sim_pol_device_set_brand(
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
	if (opcode != PCM_OP_SIM_POL_DEVICE_SET_BRAND) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_sim_pol_device_set_brand opcode error", ebufp);

		*o_flistpp = NULL;
		return;
	}

	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_sim_pol_device_set_brand input flist", i_flistp);

	/*
	 * Prepare the return flist - copy the input to the output 
	 */
	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);


	/*
	 * Main rountine for this opcode
	 */
	fm_sim_pol_device_set_brand(ctxp, i_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_sim_pol_device_set_brand error", ebufp);
		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_sim_pol_device_set_brand return flist", *o_flistpp);
	}

	return;
}


/*******************************************************************************
 * fm_sim_pol_device_set_brand()
 *
 * Inputs: flist with /device/sim poid and account_obj
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function validates the input flist has a poid of type '/device/sim',
 * then validates the account_obj.
 ******************************************************************************/
static void
fm_sim_pol_device_set_brand(
	pcm_context_t           *ctxp,
	pin_flist_t             *i_flistp,
	pin_errbuf_t            *ebufp)
{
	void			*vp = NULL;			/* flist gets */

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
		"fm_sim_pol_device_set_brand input flist", i_flistp);

	/*
	 * Validate the object is of type /device/sim
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	if(vp != NULL){
		if (strcmp((char *)PIN_POID_GET_TYPE((poid_t *)vp), 
			PIN_OBJ_TYPE_DEVICE_SIM) != 0) {

			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
			       "fm_sim_pol_device_set_brand invalid poid type");

			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_APPLICATION,
               		        PIN_ERR_BAD_POID_TYPE, PIN_FLD_POID, 0, 0);

		}
	}

	/*
	 * If the account_obj field is provided, validate it
	 */
	if(PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACCOUNT_OBJ, 1, ebufp) != NULL) {
		fm_sim_pol_validate_account_obj(ctxp, i_flistp, ebufp);
	}

}


/*******************************************************************************
 * fm_sim_pol_validate_account_obj()
 *
 * Inputs: flist with account_obj, and (optionally) an flist with db values 
 *	for the /device/sim
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function examines the account_obj value against the current account_obj
 * of the /device/sim.  If they differ, the state of the device is checked to
 * enforce that only  devices in the "RELEASE" state can have their brand 
 * changed.
 * 
 * Possible Customizations:
 * This function can be customized to support new states, or to loosen the
 * state-specific constraints applied within.
 ******************************************************************************/
static void
fm_sim_pol_validate_account_obj(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp)
{

	pin_flist_t		*flistp = NULL;		/* flist for rdflds */
	pin_flist_t		*r_flistp = NULL;	/* flist for rdflds */
	poid_t			*ia_poidp = NULL;	/* input act obj */
	poid_t			*dba_poidp = NULL;	/* act obj from db */
	int32			state_id = 0;		/* state_id from db */
	void			*vp = NULL;		/* void ptr	*/

	if (PIN_ERR_IS_ERR(ebufp)) {
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


        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
               "fm_sim_pol_validate_acct_obj rd flds input flist", flistp);
        PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, flistp, &r_flistp,
		ebufp);
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        	"fm_sim_pol_validate_acct_obj  rd flds return flist", r_flistp);

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
		 *   PIN_SIM_RELEASED_STATE
		 *
		 * States outside of this list will generate an error.
		 */
		vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATE_ID, 0, ebufp);
		if(vp != NULL){
			state_id = *(int32 *)vp;

			if(state_id != PIN_SIM_RELEASED_STATE)
			{
				/*
				 * validation fails 
				 */
				pin_set_err(ebufp, PIN_ERRLOC_FM,
					PIN_ERRCLASS_APPLICATION,
					PIN_ERR_PROHIBITED_ARG, PIN_FLD_ACCOUNT_OBJ,
					 0, 0);
				PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR, 
					"cannot change device ownership when "
					"device is not in release state ",
					 ebufp);
			}
		}
	}
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	return;
}
