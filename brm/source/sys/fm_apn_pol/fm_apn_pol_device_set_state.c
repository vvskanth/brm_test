/*
 * @(#)%Portal Version: fm_apn_pol_device_set_state.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:17 %
 *
 * Copyright (c) 2005, 2023, Oracle and/or its affiliates.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_apn_pol_device_set_state.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:17 %";
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

#define FILE_LOGNAME "fm_apn_pol_device_set_state.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_apn_pol_device_set_state(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_apn_pol_validate_permission(
	pcm_context_t		*ctxp,
	int32           	flags,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);

static void
fm_apn_pol_validate_state(
        pcm_context_t           *ctxp,
        int32                   flags,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp);

static void 
get_device_state(
        pcm_context_t   	*ctxp,
        int32           	flags,
        pin_flist_t     	*i_flistp,
        pin_flist_t     	**dev_state_info_flistp,
        pin_errbuf_t    	*ebufp);

/*******************************************************************************
 * Entry routine for the PCM_OP_IP_POL_DEVICE_SET_STATE opcode
 ******************************************************************************/
void
op_apn_pol_device_set_state(
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
         * If opcode is not correct 
         */
        if (opcode != PCM_OP_APN_POL_DEVICE_SET_STATE ) {

                pin_set_err(ebufp,
                                PIN_ERRLOC_FM,
                                PIN_ERRCLASS_SYSTEM_DETERMINATE,
                                PIN_ERR_BAD_OPCODE,
                                0,
                                0,
                                opcode);

                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "op_apn_pol_device_set_state opcode error",
                                ebufp);

                return;
        }

	/*
         * Log the input flist
         */
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                                "op_apn_pol_device_set_state input flist",
                                i_flistp);

	/*
         * if a direct call is made to PCM_OP_DEVICE_SET_STATE
         * call fm_apn_pol_validate_permission to check the permission
         */

        if ((! fm_utils_op_is_ancestor(connp->coip,
                                     PCM_OP_IP_POL_DEVICE_CREATE)) &&
        	(! fm_utils_op_is_ancestor(connp->coip,
                                     PCM_OP_IP_POL_DEVICE_SET_ATTR))) {

                fm_apn_pol_validate_permission( ctxp, flags, i_flistp, ebufp);

        }

	/*
         * Validate that user cannot shange the apn state,
         * if any of the ip corresponding to the apn is in "Allocated" state.
         */
        fm_apn_pol_validate_state(ctxp, flags, i_flistp, ebufp);

        /*
         * Results.
         */
	
	if (PIN_ERR_IS_ERR(ebufp)) {
		/*
                 * If validation fails Set the ebuf, and log the problem
                 */
		PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR,
				"op_apn_pol_device_set_state error", 
				ebufp);
		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_apn_pol_device_set_state output flist", 
			*o_flistpp);
	}

	return;
}


/*******************************************************************************
 * fm_apn_pol_validate_permission()
 *
 * Inputs: flist with an apn poid 
 *
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function doesnot permit APN state change from "NEW" to "USABLE" 
 * when none of the IP devices are associated with that APN .
 * 
 * Possible Customizations:
 * This function can be customized to support new states, or to loosen the
 * state-specific constraints applied within.
 ******************************************************************************/

static void
fm_apn_pol_validate_permission(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp)
{

	int32                   *old_statep = NULL;
        int32                   *new_statep = NULL;
	pin_flist_t     	*dev_ret_state_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                                "op_apn_pol_device_set_state input flist",
                                i_flistp);

	/*
         * Get the new and old states from the input flist.
         */
        new_statep = (int32 *) PIN_FLIST_FLD_GET(i_flistp,
                PIN_FLD_NEW_STATE, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE, 0, 0, 0);

		PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR, 
			"APN device NEW_STATE is missing",
			 ebufp);
		return;
	}

	old_statep = (int32 *) PIN_FLIST_FLD_GET(i_flistp,
                PIN_FLD_OLD_STATE, 1, ebufp);

	if (old_statep ==  (int32 *) NULL){
		get_device_state(ctxp,flags,i_flistp,&dev_ret_state_flistp,ebufp);

		 if (PIN_ERR_IS_ERR(ebufp)) {
			return;
		}
		old_statep = (int32 *) PIN_FLIST_FLD_GET(dev_ret_state_flistp,
			PIN_FLD_STATE_ID, 1, ebufp);
	}


	/*
         * Changing the state of the device is restricted to objects
         * with old_state as "NEW" and new_state as "USABLE" when
         * a direct call is made to DEVICE_SET_STATE
         */

	if (old_statep &&  (*old_statep == PIN_APN_NEW_STATE) &&
                	new_statep && (*new_statep == PIN_APN_USABLE_STATE) ) 
	{
		/*
		 * validation fails 
		 */
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION,
			PIN_ERR_PERMISSION_DENIED, 
			0,
			0, 0);
		PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR, 
			"APN state change NEW to USABLE is not valid "
			"with out calling PCM_OP_IP_POL_DEVICE_CREATE", 
			 ebufp);
	}

	return;
}


/*******************************************************************************
 * fm_apn_pol_validate_state()
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
fm_apn_pol_validate_state(
        pcm_context_t           *ctxp,
        int32                   flags,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp) {

	pin_flist_t     *srch_flistp = NULL;    /* Search flist         */
        pin_flist_t     *sr_flistp = NULL;      /* Return flistp search */
        pin_flist_t     *sub_flistp = NULL;     /* Flist for substruct  */
        pin_flist_t     *s_flistp = NULL;       /* Flist for device_ip  */
        poid_t          *pdp = NULL;            /* Routing poid         */
        poid_t          *srch_poidp = NULL;     /* Search poid          */
        int32           flag = 0;               /* Search flags         */
        int64           dbno = 0;               /* Database no          */
        int32           cred = 0;               /* Scoping credentials  */
	int32           count=0;                /* Count from search    */	
	int32           state_id = PIN_IP_ALLOCATED_STATE;       /*IP State*/
        pin_cookie_t    cookie = NULL;          /* cookie to recurse array */

	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_apn_pol_validate_state entry point", ebufp);
                goto cleanup;
        }

        PIN_ERR_CLEAR_ERR(ebufp);

        srch_flistp = PIN_FLIST_CREATE(ebufp);

        pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

        if (PIN_ERR_IS_ERR(ebufp)) {

                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                        "fm_apn_pol_validate_state - error getting poid ");
                goto cleanup;
        }

        dbno = PIN_POID_GET_DB(pdp);

        srch_poidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

        PIN_FLIST_FLD_PUT(srch_flistp, PIN_FLD_POID, srch_poidp, ebufp);
        PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);

        sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_ARGS, 1, ebufp);
        s_flistp = PIN_FLIST_SUBSTR_ADD(sub_flistp,PIN_FLD_DEVICE_IP,ebufp);
        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_APN_OBJ, pdp, ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_ARGS, 2, ebufp);
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_STATE_ID, &state_id, ebufp);

        PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_TEMPLATE,
                (void *)"select X from /device/ip where  ( F1 = V1  and F2 = V2 ) ", ebufp);

	PIN_FLIST_ELEM_SET(srch_flistp, NULL, PIN_FLD_RESULTS, 0, ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_apn_pol_validate_state srch input flist", srch_flistp);

        PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, flags | PCM_OPFLG_COUNT_ONLY, srch_flistp, &sr_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "fm_apn_pol_validate_state "
                                "PCM_OP_GLOBAL_SEARCH error ",
                                        ebufp);
                goto cleanup;
        }

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_apn_pol_validate_state srch output flist", sr_flistp);

        PIN_FLIST_ELEM_GET_NEXT(sr_flistp, PIN_FLD_RESULTS,
                &count, 1, &cookie, ebufp);

        if (count > 0) {

                        pin_set_err(ebufp, PIN_ERRLOC_FM,
                                PIN_ERRCLASS_APPLICATION,
                                PIN_ERR_INVALID_STATE, PIN_FLD_STATE_ID, 0,
                                0);

                        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "fm_apn_pol_validate_state "
                                "set state failed for APN as one or more "
                                "IP devices were in the Allocated state ",
                                ebufp);
        }

cleanup:

        PIN_FLIST_DESTROY_EX(&srch_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);

        return;

}

/*
 * Read the device and get the old state if not populated
 */

static void get_device_state(
        pcm_context_t   *ctxp,
        int32           flags,
        pin_flist_t     *i_flistp,
        pin_flist_t     **dev_state_flistpp,
        pin_errbuf_t    *ebufp)
{
        pin_flist_t     *flistp = NULL;
        pin_flist_t     *readflds_ret_flistp = NULL;
        poid_t          *pdp = NULL;


        if (PIN_ERR_IS_ERR(ebufp))
                return;
        PIN_ERR_CLEAR_ERR(ebufp);

        pdp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp,PIN_FLD_POID, 0, ebufp);

        flistp = PIN_FLIST_CREATE(ebufp);
        PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, (void *)pdp, ebufp);

        PIN_FLIST_FLD_SET(flistp, PIN_FLD_STATE_ID, (void *)NULL,ebufp);

        PCM_OP(ctxp, PCM_OP_READ_FLDS, flags, flistp, &readflds_ret_flistp, ebufp);

        if (PIN_ERR_IS_ERR(ebufp)){
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, "PCM_OP_READ_FLDS error ", ebufp);

                return;
        }

        *dev_state_flistpp = PIN_FLIST_COPY(readflds_ret_flistp,ebufp);

        PIN_FLIST_DESTROY_EX(&flistp, NULL);

	return;
}
