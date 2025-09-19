/*
 * @(#)%Portal Version: fm_ip_pol_device_set_brand.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:22 %
 *
 * Copyright (c) 2005 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_ip_pol_device_set_brand.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:22 %";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/apn.h"
#include "ops/ip.h"
#include "pin_apn.h"
#include "pin_ip.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "fm_utils.h"

#define FILE_LOGNAME "fm_ip_pol_device_set_brand.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_ip_pol_device_set_brand(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_ip_pol_validate_account_obj(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);

/*******************************************************************************
 * Functions provided outside of this source file
 ******************************************************************************/

	
/*******************************************************************************
 * Entry routine for the PCM_OP_IP_POL_DEVICE_SET_BRAND opcode
 ******************************************************************************/
void
op_ip_pol_device_set_brand(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*dn_flistp = NULL;
	void			*vp = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);


        /*
         * If opcode is not correct 
         */
        if (opcode != PCM_OP_IP_POL_DEVICE_SET_BRAND ) {

                pin_set_err(ebufp,
                                PIN_ERRLOC_FM,
                                PIN_ERRCLASS_APPLICATION,
                                PIN_ERR_BAD_OPCODE,
                                0,
                                0,
                                opcode);

                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "op_ip_pol_device_set_brand opcode error",
                                ebufp);

                *o_flistpp = NULL;
                return;
        }

	/*
	 * Check if PCM_OP_APN_POL_DEVICE_SET_BRAND is calling this policy 
	 * opcode. If ancestor is not PCM_OP_APN_POL_DEVICE_SET_BRAND then 
	 * movement of IP not possible
	 */
	if (! fm_utils_op_is_ancestor(connp->coip,
			PCM_OP_APN_POL_DEVICE_SET_BRAND)) {

		pin_set_err(ebufp, 
				PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_PERMISSION_DENIED, 
				0, 
				0, 
				opcode);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"op_ip_pol_device_set_brand opcode error", 
				ebufp);

		*o_flistpp = NULL;
		return;
	}
	
	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"op_ip_pol_device_set_brand input flist", 
				i_flistp);

	/*
	 * call fm_ip_pol_validate_account_obj
	 */
	fm_ip_pol_validate_account_obj(
				ctxp,
				i_flistp,
				ebufp);
	

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR,
				"op_ip_pol_device_set_brand error", 
				ebufp);

		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_ip_pol_device_set_brand output flist", 
			*o_flistpp);
	}

	return;
}


/*******************************************************************************
 * fm_ip_pol_validate_account_obj()
 *
 * Inputs: flist with account_obj, and (optionally) an flist with db values 
 *	for the /device/ip
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function examines the account_obj value against the current account_obj
 * of the /device/ip.  If they differ, the state of the device is checked to
 * enforce that only devices in the "NEW" or "UNALLOCATED" state can have their 
 * brand changed.
 * 
 * Possible Customizations:
 * This function can be customized to support new states, or to loosen the
 * state-specific constraints applied within.
 ******************************************************************************/

static void
fm_ip_pol_validate_account_obj(
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
               "fm_ip_pol_validate_account_obj : "
		"read fields input flist", 
		flistp);

        PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, flistp, &r_flistp,
		ebufp);

	if( PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_validate_account_obj : "
			"Error in reading fields");
		goto Cleanup;
	}
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        	"fm_ip_pol_validate_account_obj : "
		"read fields return flist", 
		r_flistp);

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
		 *   PIN_IP_NEW_STATE
		 *
		 * States outside of this list will generate an error.
		 */
		vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATE_ID, 0, ebufp);
		if(vp != NULL){
			state_id = *(int32 *)vp;

			if(state_id != PIN_IP_NEW_STATE &&
				state_id != PIN_IP_UNALLOCATED_STATE)
			{
				/*
				 * validation fails 
				 */
				pin_set_err(ebufp, PIN_ERRLOC_FM,
					PIN_ERRCLASS_APPLICATION,
					PIN_ERR_PROHIBITED_ARG, 
					PIN_FLD_STATE_ID,
					0, 
					0);
				PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR, 
					"cannot change device ownership when "
					"device is not in new or unallocated "
					"state ",
					 ebufp);
			}
		}
	}
	else {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
			"Brands are same, returning");
	}
Cleanup:
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	return;
}

