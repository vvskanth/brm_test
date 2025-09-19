/*
 * @(#)%Portal Version: fm_num_pol_device_set_brand.c:WirelessVelocityInt:2:2006-Sep-14 11:27:42 %
 *
* Copyright (c) 2001, 2019, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_num_pol_device_set_brand.c:WirelessVelocityInt:2:2006-Sep-14 11:27:42 %";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/num.h"
#include "pin_num.h"
#include "cm_fm.h"
#include "pin_errs.h"

#define FILE_LOGNAME "fm_num_pol_device_set_brand.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_num_pol_device_set_brand(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_num_pol_validate_account_obj(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);	


/*******************************************************************************
 * Entry routine for the PCM_OP_NUM_POL_DEVICE_SET_BRAND opcode
 ******************************************************************************/
void
op_num_pol_device_set_brand(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	void			*vp = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_NUM_POL_DEVICE_SET_BRAND) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_num_pol_device_set_brand opcode error", ebufp);

		*o_flistpp = NULL;
		return;
	}

	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_num_pol_device_set_brand input flist", i_flistp);

	/*
	 * Validate the object is of type /device/num
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	if (strcmp((char *)PIN_POID_GET_TYPE((poid_t *)vp), 
		PIN_OBJ_TYPE_DEVICE_NUM) != 0) {

		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
			"fm_num_pol_device_set_brand invalid poid type");

		pin_set_err(ebufp, PIN_ERRLOC_FM, 
			PIN_ERRCLASS_APPLICATION,
                        PIN_ERR_BAD_POID_TYPE, PIN_FLD_POID, 0, 0);

		return;
	}

	/*
	 * If the account_obj field is provided, validate it
	 */
	fm_num_pol_validate_account_obj(ctxp, i_flistp, ebufp);

	/*
	 * Prepare the successful return flist
	 */
	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_num_pol_device_set_brand error", ebufp);

		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_num_pol_device_set_brand return flist", *o_flistpp);
	}

	return;
}


/*******************************************************************************
 * fm_num_pol_validate_account_obj()
 *
 * Inputs: flist with (optional) account_obj
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function examines the account_obj value against the current account_obj
 * of the /device/num.  If they differ, the state of the device is checked to
 * enforce that only 'New' and 'Unassigned' numbers can have their brand 
 * changed.
 * 
 * Possible Customizations:
 * This function can be customized to support new states, or to loosen the
 * state-specific constraints applied within.
 ******************************************************************************/
static void
fm_num_pol_validate_account_obj(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp)
{
	poid_t			*ia_poidp = NULL;	/* input act obj */
	poid_t			*dba_poidp = NULL;	/* act obj from db */
	pin_flist_t		*db_flistp = NULL;	/* db values */
	int32			*state_id = NULL;	/* state_id from db */


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * If the account_obj is not provided, return
	 */
	if (PIN_FLIST_FLD_GET(i_flistp, 
		PIN_FLD_ACCOUNT_OBJ, 1, ebufp) == NULL) {

		return;
	}

	/*
	 * Get the values of the number from the database
	 * Add the state_id field to the flist, so its
	 * value is also read from the database
	 */
	PIN_FLIST_FLD_SET(i_flistp, PIN_FLD_STATE_ID, NULL, ebufp);

        PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, 
		i_flistp, &db_flistp, ebufp);

	PIN_FLIST_FLD_DROP(i_flistp, PIN_FLD_STATE_ID, ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_num_pol_validate_account_obj return flist",	db_flistp);

	/*
	 * Compare existing account_obj with input value
	 */
	ia_poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, 
		PIN_FLD_ACCOUNT_OBJ, 0, ebufp);
	dba_poidp = (poid_t *)PIN_FLIST_FLD_GET(db_flistp, 
		PIN_FLD_ACCOUNT_OBJ, 0, ebufp);

	if (PIN_POID_COMPARE(ia_poidp, dba_poidp, 0, ebufp) != 0) {

		/*
		 * Changing the brand of the number is restricted to objects
		 * with a state_id of:
		 *
		 *   PIN_NUM_STATE_NEW
		 *   PIN_NUM_STATE_UNASSIGNED
		 *
		 * States outside of this list will generate an error.
		 */
		state_id = (int32 *)PIN_FLIST_FLD_GET(db_flistp,
			PIN_FLD_STATE_ID, 0, ebufp);

	   if(state_id != NULL) {
		switch (*state_id) {

		case PIN_NUM_STATE_NEW:
		case PIN_NUM_STATE_UNASSIGNED:

			/*
			 * These values support changing the account_obj,
			 * so do nothing
			 */
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
				"fm_num_pol_validate_account_obj change "
				"supported");

			break;

		case PIN_NUM_STATE_RAW:
		case PIN_NUM_STATE_ASSIGNED:
		case PIN_NUM_STATE_QUARANTINED:
		default:

			/*
			 * Set the ebuf, and log the problem
			 */
			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_APPLICATION,
                        	PIN_ERR_BAD_VALUE, PIN_FLD_STATE_ID, 0,
				*state_id);

			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_num_pol_validate_account_obj current "
				"state_id does not permit changing the "
				"account_obj value", ebufp);

			break;

		}
           }

	} /* else new and current account_obj values are the same, do nothing */ 

	PIN_FLIST_DESTROY_EX(&db_flistp, NULL);
	return;
}
