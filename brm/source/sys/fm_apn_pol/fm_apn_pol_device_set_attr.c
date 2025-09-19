/*
 * @(#)%Portal Version: fm_apn_pol_device_set_attr.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:49 %
 *
 * Copyright (c) 2004 - 2023 Oracle.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_apn_pol_device_set_attr.c:ServerIDCVelocityInt:1:2006-Sep-14 11:30:49 %" ;
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#include "cm_fm.h"
#include "ops/apn.h" 
#include "pin_errs.h"
#include "pin_apn.h"
#include "pin_ip.h"
#include "pin_device.h"
#include "ops/device.h"

#define FILE_LOGNAME "fm_apn_pol_device_set_attr.c(1)" 

EXPORT_OP void
op_apn_pol_device_set_attr(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp);


/**********************************************************************
 * Routines contained within
 **********************************************************************/

/**********************************************************************
 * fm_apn_pol_validate_set_attr() validates ip_obj's state for given apn
 **********************************************************************/
static void
fm_apn_pol_validate_set_attr(
	pcm_context_t		*ctxp,
	int32                   flags,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);

/**********************************************************************
 * fm_apn_pol_validate_device_id() validates apn device id duplication
 **********************************************************************/
static void
fm_apn_pol_validate_device_id(
        pcm_context_t           *ctxp,
        int32                   flags,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp);


/**********************************************************************
 * Main routine for the PCM_OP_APN_POL_DEVICE_SET_ATTR operation.
 * op_apn_pol_device_set_attr():
 *
 * This opcode is called from the device policy framework.
 * Policy opcode will :
 * 1.   check if the opcode passed is PCM_OP_APN_POL_DEVICE_SET_ATTR .
 * 2.   check if the poid passed is of type APN
 * 3.   check if all IPs correspondig to this APN are in a valid state
 * 4.   copy the input flist to output flist
 **********************************************************************/
void
op_apn_pol_device_set_attr(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flpp,
	pin_errbuf_t		*ebufp) {

	pcm_context_t	*ctxp = connp->dm_ctx;
	poid_t		*dev_poidp = NULL; 
	char		*poid_type = NULL;
	void            *vp = NULL;       /* void ptr   */

	if (PIN_ERR_IS_ERR(ebufp)) {
               PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "op_apn_pol_device_set_attr entry point", ebufp);
                goto cleanup;
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check. Check if the opcode passed is not PCM_OP_APN_POL_DEVICE_SET_ATTR, 
	 * through "Bad Opcode"  error message.
	 */
	if (opcode != PCM_OP_APN_POL_DEVICE_SET_ATTR) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_apn_pol_device_set_attr", ebufp);
		goto cleanup;
	}

	/*
	 * DEBUG: What did we get? Log the input flist.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_apn_pol_device_set_attr input flist", i_flistp);

	/*
	 * Check if poid passed is not of type APN, through "Bad Poid Type" error message.
	 */

	/*
	 * Get the device poid
	 */
	dev_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                        "op_apn_pol_device_set_attr - error getting poid type ");
                goto cleanup;
        }

	/*
	 * Get the device poid type
	 */
	poid_type = (char *)PIN_POID_GET_TYPE(dev_poidp);

	/*
	 * Validate the object is of type /device/apn
	 */
	if (strcmp(poid_type, PIN_OBJ_TYPE_DEVICE_APN) != 0) {

		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION,
                        PIN_ERR_BAD_POID_TYPE, PIN_FLD_POID, 0, 0);

		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_device_set_attr invalid poid type");

		goto cleanup;
	}

	/*
         * Check the device id of the apn from the input flist,
         * if the device id already exisits, through an error.
         */

        vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_DEVICE_ID, 1, ebufp);
        if(vp != (void *)NULL) {

		/*
                 * if someone is trying to duplicate the device id,
                 * throw an error and log the problem
                 */

                 fm_apn_pol_validate_device_id(ctxp, flags, i_flistp, ebufp);

                if (PIN_ERR_IS_ERR(ebufp)) {

                        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                                "op_apn_pol_device_set_attr - "
                                "Device Id already exists");
                        goto cleanup;
                }
                else
                {
                         PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
                                "op_apn_pol_device_set_attr - New Device Id ");
                }
	}
        else {
                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
                        "op_apn_pol_device_set_attr - Device Id is not passed");
        }

	/*
	 * Validate to ensure that user cannot update the apn,
	 * if any of the ip corresponding to the apn is in "Allocated" state.
	 */
	fm_apn_pol_validate_set_attr(ctxp, flags, i_flistp, ebufp);

	
        if (PIN_ERR_IS_ERR(ebufp)) {

                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                        "op_apn_pol_device_set_attr error from fm_apn_pol_device_set_attr()");
                goto cleanup;
        }


cleanup:

	/*
         * Results.
         */
	if (PIN_ERR_IS_ERR(ebufp)) {

		/*
                 * If validation fails Set the ebuf, and log the problem
                 */			

		*o_flpp = (pin_flist_t *)NULL;
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_apn_pol_device_set_attr error", ebufp);
	}else {

		/*
         	 * By default, we simply copy the input to the output..
         	 */	
		
		*o_flpp = PIN_FLIST_COPY(i_flistp, ebufp);
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_apn_pol_device_set_attr return flist", *o_flpp);
	}
	
	return;

}

/*******************************************************************************
 * fm_apn_pol_validate_set_attr()
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
fm_apn_pol_validate_set_attr(
	pcm_context_t		*ctxp,
	int32                   flags,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp) {


	pin_flist_t     *srch_flistp = NULL;    /* Search flist         */
        pin_flist_t     *sr_flistp = NULL;      /* Return flistp search */
        pin_flist_t     *sub_flistp = NULL;     /* Flist for substruct  */
        pin_flist_t     *s_flistp = NULL;     	/* Flist for device_ip  */
        pin_flist_t     *res_flistp = NULL;     /* Flist for result substruct  */
        poid_t          *pdp = NULL;            /* Routing poid         */
        poid_t          *srch_poidp = NULL;     /* Search poid          */
        int32           flag = 0;   		/* Search flags         */
        int64           dbno = 0;               /* Database no          */
        int32           cred = 0;               /* Scoping credentials  */
	int32		state_id = PIN_IP_ALLOCATED_STATE;	 /*IP State*/
	int32           count=0;                /* Count from search    */
	pin_cookie_t	cookie = NULL;		/* cookie to recurse array */

	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_apn_pol_validate_set_attr entry point", ebufp);
                goto cleanup;
        }	

        PIN_ERR_CLEAR_ERR(ebufp);

        srch_flistp = PIN_FLIST_CREATE(ebufp);

        pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {

                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                        "fm_apn_pol_validate_set_attr - error getting poid ");
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
                (void *)"select X from /device/ip where  ( F1 = V1 and F2 = V2 ) ", ebufp);

	PIN_FLIST_ELEM_SET(srch_flistp, NULL, PIN_FLD_RESULTS, 0, ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
              	"fm_apn_pol_validate_set_attr srch input flist", srch_flistp);

        PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, flags | PCM_OPFLG_COUNT_ONLY, srch_flistp, &sr_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "fm_apn_pol_validate_set_attr"
                                "PCM_OP_GLOBAL_SEARCH error ",
                                        ebufp);
                goto cleanup;
        }

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_apn_pol_validate_set_attr srch output flist", sr_flistp);
	
	PIN_FLIST_ELEM_GET_NEXT(sr_flistp, PIN_FLD_RESULTS,
                &count, 1, &cookie, ebufp);

	if (count > 0) {	

			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
                       		PIN_ERR_BAD_VALUE, PIN_FLD_STATE_ID, 0,
				0);

                        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "fm_apn_pol_validate_set_attr "
                                "set attributes failed for APN as one or more " 
				"IP devices were in the Allocated state ",
                                ebufp);
        }


cleanup:  

        PIN_FLIST_DESTROY_EX(&srch_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);

        return;

}

/*******************************************************************************
 * fm_apn_pol_validate_device_id()
 *
 * Inputs: flist with apn poid
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This method will search the database to see if there is any apn device
 * that already exists in the db with the given device id. If there is
 * any, then an error is thrown and then the ebuf is set.
 *
 ******************************************************************************/
static void
fm_apn_pol_validate_device_id(
        pcm_context_t           *ctxp,
        int32                   flags,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp) {

	pin_flist_t     *srch_flistp = NULL;    /* Search flist         */
        pin_flist_t     *sr_flistp = NULL;      /* Return flistp search */
        pin_flist_t     *sub_flistp = NULL;     /* Flist for substruct  */
        pin_flist_t     *res_flistp = NULL;     /* Flist for result substruct  */
        poid_t          *pdp = NULL;            /* Routing poid         */
        poid_t          *srch_poidp = NULL;     /* Search poid          */
        char            *dev_id = NULL;         /* APN_device_ID        */
        int32           flag = 0;               /* Search flags         */
        int64           dbno = 0;               /* Database no          */
        int32           cred = 0;               /* Scoping credentials  */
        poid_t          *pdp_apn = NULL;         /* APN poid             */


	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_apn_pol_validate_device_id entry point", ebufp);
                goto cleanup;
        }

        PIN_ERR_CLEAR_ERR(ebufp);

	srch_flistp = PIN_FLIST_CREATE(ebufp);

        pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
        dbno = PIN_POID_GET_DB(pdp);

        srch_poidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

	/*
         * Get the Device id from the input Flist
         */
        dev_id = (char *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_DEVICE_ID,
                                                0, ebufp);

        PIN_FLIST_FLD_PUT(srch_flistp, PIN_FLD_POID, srch_poidp, ebufp);
        PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_ARGS, 1, ebufp);
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_DEVICE_ID, dev_id, ebufp);

        PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_TEMPLATE,
                (void *)"select X from /device/apn where  ( F1 = V1 ) ", ebufp);

        sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_RESULTS, 0, ebufp);
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_POID, NULL, ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
             	"fm_apn_pol_validate_device_id srch input flist", srch_flistp);

        PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, flags, srch_flistp, &sr_flistp, ebufp);

        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "fm_apn_pol_validate_device_id "
                                "PCM_OP_GLOBAL_SEARCH error ",
                                        ebufp);
                goto cleanup;
        }

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_apn_pol_validate_device_id srch output flist", sr_flistp);

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
                                PIN_ERRCLASS_SYSTEM_DETERMINATE,
                                PIN_ERR_DUPLICATE, PIN_FLD_DEVICE_ID, 0, 0);

                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "duplicate device id in fm_apn_pol_validate_device_id()", ebufp);

                PIN_ERR_LOG_POID(PIN_ERR_LEVEL_ERROR,
                                "fm_apn_pol_validate_device_id "
                                "device id already exists for the APN poid: ",
                                pdp_apn);
        }


cleanup:

        PIN_FLIST_DESTROY_EX(&srch_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);

        return;

}
