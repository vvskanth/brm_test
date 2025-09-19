/*
 * @(#)%Portal Version: fm_ip_pol_device_set_state.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:13 %
 *
* Copyright (c) 2005, 2019, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_ip_pol_device_set_state.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:13 %";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_device.h"
#include "ops/device.h"
#include "pin_ip.h"
#include "ops/ip.h"

#define FILE_LOGNAME "fm_ip_pol_device_set_state.c(1)" 

/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_ip_pol_device_set_state(
        cm_nap_connection_t     *connp,
        int32                   opcode,
        int32                   flags,
        pin_flist_t             *i_flistp,
        pin_flist_t             **o_flistpp,
        pin_errbuf_t            *ebufp);

static void
fm_ip_pol_validate_permission(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp);

/*******************************************************************************
 * Main routine for the PCM_OP_IP_POL_DEVICE_SET_STATE opcode
 ******************************************************************************/
void
op_ip_pol_device_set_state(
        cm_nap_connection_t     *connp,
        int32                   opcode,
        int32                   flags,
        pin_flist_t             *i_flistp,
        pin_flist_t             **o_flistpp,
        pin_errbuf_t            *ebufp)
{
	pcm_context_t           *ctxp = connp->dm_ctx;


       if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
        PIN_ERR_CLEAR_ERR(ebufp);


        /*
         * If opcode is not correct
         */
        if (opcode != PCM_OP_IP_POL_DEVICE_SET_STATE ) {

                pin_set_err(ebufp,
                                PIN_ERRLOC_FM,
                                PIN_ERRCLASS_SYSTEM_DETERMINATE,
                                PIN_ERR_BAD_OPCODE,
                                0,
                                0,
                                opcode);


                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "op_ip_pol_device_set_state opcode error",
                                ebufp);

                return;
        }


        /*
         * Log the input flist
         */
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                                "op_ip_pol_device_set_state input flist",
                                i_flistp);

        /*
         * if a direct call is made to DEVICE_SET_STATE
         * call fm_ip_pol_validate_permission to check the permission
         */

	 if (! fm_utils_op_is_ancestor(connp->coip,
                                     PCM_OP_IP_POL_DEVICE_ASSOCIATE)) {

                fm_ip_pol_validate_permission( ctxp, i_flistp, ebufp);

        }

        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR,
                                "op_ip_pol_device_set_state error",
                                ebufp);

                PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
                *o_flistpp = NULL;

        } else {
                *o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
                PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                        "op_ip_pol_device_set_state output flist",
                        *o_flistpp);
        }

        return;
}


/*******************************************************************************
 * fm_ip_pol_validate_permission()
 *
 * Inputs: flist with ip poid
 * 
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function doesnot permit IP state change from any state to "ALLOCATED"
 * or "ALLOCATED" state to any other state when the ancestor opcode is not  
 * "PCM_OP_IP_POL_DEVICE_ASSOCIATE".
 *
 * Possible Customizations:
 * This function can be customized to support new states, or to loosen the
 * state-specific constraints applied within.
 ******************************************************************************/
static void
fm_ip_pol_validate_permission(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp)
{

        int32                   *old_statep = NULL;
        int32                   *new_statep = NULL;


        if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
        PIN_ERR_CLEAR_ERR(ebufp);

        /*
         * Get the new and old state from the input flist.
         */
        new_statep = (int32 *) PIN_FLIST_FLD_GET(i_flistp,
                PIN_FLD_NEW_STATE, 0, ebufp);

        old_statep = (int32 *) PIN_FLIST_FLD_GET(i_flistp,
                PIN_FLD_OLD_STATE, 0, ebufp);

	/*
         * Changing the state of the device is restricted to objects,
         * if old_state/new_state is PIN_IP_ALLOCATED_STATE when
         * if a direct call is made to DEVICE_SET_STATE
         */

        if( ((old_statep != NULL ? *old_statep : 0) == PIN_IP_ALLOCATED_STATE) ||
                ((new_statep != NULL ? *new_statep : 0) == PIN_IP_ALLOCATED_STATE) )
        {
                /*
                 * validation fails
                 */
                pin_set_err(ebufp, PIN_ERRLOC_FM,
                        PIN_ERRCLASS_APPLICATION,
                        PIN_ERR_PERMISSION_DENIED,
                        PIN_FLD_STATE_ID,
                        0, 0);
                PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR,
                        "cannot change IP device state when "
                        "old_state/new_state is PIN_IP_ALLOCATED_STATE "
                        "with out calling PCM_OP_IP_POL_DEVICE_ASSOCIATE ",
                         ebufp);
        }

        return;
}
