/*
 * @(#)%Portal Version: fm_ip_pol_device_set_attr.c:ServerIDCVelocityInt:1:2006-Sep-14 11:31:10 %
 *
* Copyright (c) 2004, 2023, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_ip_pol_device_set_attr.c:ServerIDCVelocityInt:1:2006-Sep-14 11:31:10 %";
#endif


#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#include "cm_fm.h"
#include "ops/ip.h" 
#include "pin_errs.h"
#include "pin_ip.h" 
#include "pin_apn.h" 
#include "pin_device.h"
#include "ops/device.h"

#define FILE_LOGNAME "fm_ip_pol_device_set_attr.c(1)" 

EXPORT_OP void
op_ip_pol_device_set_attr(
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
fm_ip_pol_device_set_attr(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistp,
	pin_errbuf_t		*ebufp);


static void
fm_ip_pol_validate_device_state(
	pcm_context_t		*ctxp,
	int32                   flags,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);


static void
fm_ip_pol_device_id_exists(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*apn_poidp,
	pin_flist_t		*in_flistp,
	pin_errbuf_t		*ebufp);


static void
fm_ip_pol_apn_change_state(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*apn_poidp,
	pin_flist_t		*in_flistp,
	pin_errbuf_t		*ebufp);


/**********************************************************************
 * Main routine for the PCM_OP_IP_POL_DEVICE_SET_ATTR operation.
 * op_ip_pol_device_set_attr():
 * 
 * This opcode is called from the device policy framework. 
 * Policy opcode will :
 * 1.   check if the opcode passed is PCM_OP_IP_POL_DEVICE_SET_ATTR .
 * 2.   check if the poid passed is of type IP
 * 3.   check if the state of device IP is a valid state
 * 4.   copy the input flist to output flist 
 **********************************************************************/
void
op_ip_pol_device_set_attr(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flpp,
	pin_errbuf_t		*ebufp) {

	pcm_context_t	*ctxp = connp->dm_ctx;
	poid_t		*dev_poidp = NULL; 
	char		*poid_type = NULL; 

	

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                	"op_ip_pol_device_set_attr entry point", ebufp);
		goto cleanup;
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check. Check if the opcode passed is not PCM_OP_IP_POL_DEVICE_SET_ATTR, 
	 * through "Bad Opcode"  error message.
	 */
	if (opcode != PCM_OP_IP_POL_DEVICE_SET_ATTR) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_ip_pol_device_set_attr", ebufp);
		goto cleanup;
	}

	/*
	 * DEBUG: What did we get? Log the input flist.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_ip_pol_device_set_attr input flist", i_flistp);

	/*
	 * Check if poid passed is not of type IP, through "Bad Poid Type" error message.
	 */

	/*
	 * Get the device poid
	 */
	dev_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {

                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                        "op_ip_pol_device_set_attr - error getting poid type ");
                goto cleanup;
        }

	/*
	 * Get the device poid type
	 */
	poid_type = (char *)PIN_POID_GET_TYPE(dev_poidp);

	/*
	 * Validate the object is of type /device/ip
	 */
	if (strcmp(poid_type, PIN_OBJ_TYPE_DEVICE_IP) != 0) {

		pin_set_err(ebufp, PIN_ERRLOC_FM, 
			PIN_ERRCLASS_APPLICATION,
                        PIN_ERR_BAD_POID_TYPE, PIN_FLD_POID, 0, 0);

		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
			"op_ip_pol_device_set_attr - invalid poid type");

		goto cleanup;
	}

	/*
	 * Validate to ensure that user cannot update the device if it is in,
	 * "Allocated" state or "Returned" state.
	 */
	fm_ip_pol_validate_device_state(ctxp, flags, i_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {

		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                        "op_ip_pol_device_set_attr error from fm_ip_pol_validate_device_state()");
                goto cleanup;
        }
	


	/*
	 * do the real work
	 */
	fm_ip_pol_device_set_attr(ctxp, flags, i_flistp, o_flpp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) { 

                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                        "op_ip_pol_device_set_attr error from fm_ip_pol_device_set_attr()");
                goto cleanup;	
	}

	
cleanup:

	/*
         * Results.
         */
        if (PIN_ERR_IS_ERR(ebufp)) {
                *o_flpp = (pin_flist_t *)NULL;
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "op_ip_pol_device_set_attr error", ebufp);
        }else {
                PIN_ERR_CLEAR_ERR(ebufp);
                PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                        "op_ip_pol_device_set_attr return flist", *o_flpp);
        }

	return;

}

/********************************************************************
 * fm_ip_pol_device_set_attr():
 *
 * input : i_flistp	: the input flist contains the attributes that
 * 			are being modified
 * output: o_flistpp	: copy of the input flist. ebuf gets set in
 * 			case of an error
 * Description:
 * This function validates the attributes of ip device that the user modifies.
 * The list of attributes that user is trying to modify is sent in input flist. 
 * Error buffer is set in case following conditions are not met.
 * 	1.  device_id cannot be modified.
 *
 ********************************************************************/
static void
fm_ip_pol_device_set_attr(
	pcm_context_t	*ctxp,
	int32		flags,
	pin_flist_t	*i_flistp,
	pin_flist_t	**o_flistpp,
	pin_errbuf_t	*ebufp) {

	void		*vp = NULL;	  /* void ptr	*/
	poid_t		*apn_poidp = NULL; 
	pin_flist_t	*sub_flistp = NULL;   /* flist for device_ip*/


	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_ip_pol_device_set_attr entry point", ebufp); 
                goto cleanup;
        }

	PIN_ERR_CLEAR_ERR(ebufp);

 	/*
         * Validate to ensure that user cannot update the device id.
         */

        vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_DEVICE_ID, 1, ebufp);
        if(vp != (void *)NULL) {

                /*
                 * if someone is trying to change the device id,
                 * throw an error and log the problem
                 */

                pin_set_err(ebufp, PIN_ERRLOC_FM,
                         PIN_ERRCLASS_APPLICATION, PIN_ERR_PERMISSION_DENIED,
                         PIN_FLD_DEVICE_ID, 0, 0);
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        " fm_ip_pol_device_set_attr -cannot change device id",
                         ebufp);
                goto cleanup;
        }

	/*
	 * Validate Canonical Device Id and the APN_OBJ
	 */

	sub_flistp = PIN_FLIST_SUBSTR_GET(i_flistp, PIN_FLD_DEVICE_IP, 1, ebufp);
	if (sub_flistp  != NULL) {

		/*
         	 * Validate to ensure that user cannot update the canonical device id.
         	 */   
	        vp = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_CANONICAL_DEVICE_ID, 1, ebufp);
                if(vp != (void *)NULL) {

                /*
                 * if someone is trying to change the canonical device id,
                 * throw an error and log the problem
                 */

                        pin_set_err(ebufp, PIN_ERRLOC_FM,
                                PIN_ERRCLASS_APPLICATION, PIN_ERR_PERMISSION_DENIED,
                                PIN_FLD_CANONICAL_DEVICE_ID, 0, 0);
                        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                " fm_ip_pol_device_set_attr "
                                " - cannot change canonical device id",
                                ebufp);
                        goto cleanup;
                }

		/*
         	 * if the APN OBJ is modified then Check for the uniqueness of IP APN
         	 * combination.
         	 * if the APN Device is in NEW change the state to USABLE
         	 */
		apn_poidp = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_APN_OBJ, 1, ebufp);
	
		if ( !PIN_POID_IS_NULL(apn_poidp)) {
			PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"CALLING fm_ip_pol_device_id_exists input flist", i_flistp);
	
			fm_ip_pol_device_id_exists(ctxp,flags, apn_poidp, i_flistp, ebufp);
				
			fm_ip_pol_apn_change_state(ctxp,flags, apn_poidp, i_flistp, ebufp);
	
			if (PIN_ERR_IS_ERR(ebufp)) {
		
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
				 "op_ip_pol_device_set_attr error" );
		
				  goto cleanup;
			}
		}
	}

	/*
       	 * By default, we simply copy the input to the output..
       	 */

	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
	PIN_ERR_CLEAR_ERR(ebufp);
       	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
       		"fm_ip_pol_device_set_attr return flist", *o_flistpp);


cleanup:
	return;

}


/*******************************************************************************
 * fm_ip_pol_validate_device_state()
 *
 * Inputs: flist with ip_obj
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function reads the PIN_FLD_STATE_ID of the device object passed in input flist. 
 * If the device is in "Allocated" state or "Returned" state, an error is thrown.
 * 
 * Possible Customizations:
 * This function can be customized to support new states, or to loosen the
 * state-specific constraints applied within.
 ******************************************************************************/
static void
fm_ip_pol_validate_device_state(
	pcm_context_t		*ctxp,
	int32                   flags,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp) {

	pin_flist_t		*flistp = NULL;		/* flist for rdflds */
	pin_flist_t		*device_o_flsitp = NULL;	/* db values */
	int32			*state_id = NULL;	/* state_id from db */
	void			*vp = NULL;	 


	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_ip_pol_validate_device_state entry point", ebufp);
		goto cleanup;
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	flistp = PIN_FLIST_CREATE(ebufp);
	
	/*
	 * Get the values of the device state id from the database
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, vp, ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_STATE_ID, NULL, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ip_pol_validate_device_state rd flds input flist", flistp);

    	PCM_OP(ctxp, PCM_OP_READ_FLDS, flags | PCM_OPFLG_CACHEABLE, 
				flistp, &device_o_flsitp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "fm_ip_pol_validate_device_state "
                                "PCM_OP_READ_FLDS error ",
                                        ebufp);
                goto cleanup;
        }

    	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ip_pol_validate_device_state return flist",	device_o_flsitp);

	/*
	 * Updating the attributes of the ip is restricted to objects
	 * with a state_id of:
	 *
	 *   PIN_IP_ALLOCATED_STATE
	 *   PIN_IP_RETURNED_STATE
	 */

	state_id = (int32 *)PIN_FLIST_FLD_GET(device_o_flsitp,
		PIN_FLD_STATE_ID, 0, ebufp);

	if( (state_id != NULL ? *state_id : 0) == PIN_IP_ALLOCATED_STATE || 
             (state_id != NULL ? *state_id : 0) == PIN_IP_RETURNED_STATE )
	{
		/*
		 * validation fails Set the ebuf, and log the problem
		 */
	
		pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_APPLICATION,
                       		PIN_ERR_BAD_VALUE, PIN_FLD_STATE_ID, 0,
				*state_id);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_ip_pol_validate_device_state"
				"state_id does not permit to set the attributes ",
					ebufp);
		goto cleanup;

	}


cleanup: 
	 PIN_FLIST_DESTROY_EX(&flistp, NULL);
	 if (device_o_flsitp) {
	 	PIN_FLIST_DESTROY_EX(&device_o_flsitp, NULL);
 	 }
	 return;

}

/*******************************************************************************
 * fm_ip_pol_device_id_exists()
 *
 * Inputs: flist with i_flistp
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function reads the PIN_FLD_DEVICE_ID and APN_OBJ if the combination exists
 * in the DB through error Message
 * 
 ******************************************************************************/
static void
fm_ip_pol_device_id_exists(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*apn_poidp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp) {
	
	int32		s_flags = 0;            /* for searching */
	int64 		database = 0;           /* route the search */
	pin_flist_t	*search_flistp = NULL;  /* search input */
	pin_flist_t	*args_flistp = NULL;    /* search args */
	pin_flist_t	*r_flistp = NULL;       /* search output */
	pin_flist_t	*dev_ip_flistp = NULL;  /* device_ip flist */
	pin_flist_t	*dev_read_flistp = NULL; /* Read flist */
	pin_flist_t	*dev_out_flistp = NULL; /* device_out flist */
	poid_t		*srchpp = NULL;         /* for routing */
	poid_t 		*dn_poidp = NULL;       /* for searching */
	char		*template = NULL;       /* search template */
	char		*poid_type = NULL;      /* for searches */
	void		*vp = NULL;             /* for flist gets */
	int32		elemid = 0;		/* for results check */
	pin_cookie_t	cookie = NULL; 		/* for results check */	
	
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ip_pol_device_id_exists input flist",i_flistp);


	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	/*Error message has to be added*/

	/*
	 * READ THE DEVICE ID from the IP POID
	 */	

	dev_read_flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET(dev_read_flistp, PIN_FLD_POID, vp, ebufp);

	PIN_FLIST_FLD_SET(dev_read_flistp, PIN_FLD_DEVICE_ID, NULL, ebufp);

	PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, dev_read_flistp, &dev_out_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_id_exists "
			" PCM_OP_READ_FLDS error", ebufp);

		goto cleanup;
	}

	/*
	 * Create the Search Flist
	 */
	search_flistp = PIN_FLIST_CREATE(ebufp);

	database = PIN_POID_GET_DB((poid_t *)vp);
	srchpp = PIN_POID_CREATE(database,"/search", -1 , ebufp);

	PIN_FLIST_FLD_PUT(search_flistp, PIN_FLD_POID, srchpp,
				ebufp);

	PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_FLAGS, &s_flags, ebufp);
			
	template = "select X from /device/ip where "
		   "  F1 = V1 and  F2 = V2 " ;

	PIN_FLIST_FLD_SET(search_flistp,
		 PIN_FLD_TEMPLATE, (void *)template, ebufp);

	/*
	 * Add the first argument to the flist - the device_id
	 */

	args_flistp = PIN_FLIST_ELEM_ADD(search_flistp,
			PIN_FLD_ARGS, 1, ebufp);
	
	/*
	 * Get the DEVICE ID
	 */
	vp = (char *)PIN_FLIST_FLD_GET(dev_out_flistp, PIN_FLD_DEVICE_ID, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_id_exists "
			" PIN_FLD_DEVICE_ID Read error ", ebufp);

		goto cleanup;
	}
		
	PIN_FLIST_FLD_SET(args_flistp,
				PIN_FLD_DEVICE_ID, vp , ebufp);
	
	/*
	 * Add the second argument to the flist - the APN poid
	 */
	args_flistp = PIN_FLIST_ELEM_ADD(search_flistp,
			PIN_FLD_ARGS, 2, ebufp);

	args_flistp = PIN_FLIST_SUBSTR_ADD(args_flistp,
				PIN_FLD_DEVICE_IP, ebufp);

	PIN_FLIST_FLD_SET(args_flistp, PIN_FLD_APN_OBJ, apn_poidp, ebufp);
	
	 PIN_FLIST_ELEM_SET(search_flistp,
		 		NULL, PIN_FLD_RESULTS, 0 , ebufp);

	PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, PCM_OPFLG_COUNT_ONLY | flags,
		search_flistp, &r_flistp, ebufp);

	/*
	 * If /device with given device_id already exists, set ebuf
	 */
	
	PIN_FLIST_ELEM_GET_NEXT(r_flistp,
		 PIN_FLD_RESULTS, &elemid, 0, &cookie, ebufp);

	if (elemid > 0) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_DUPLICATE, PIN_FLD_DEVICE_ID, 0, 0);		
		
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_device_id_exists device_id "
			"already exists");
	}	
	
	cleanup:
		PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&dev_read_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&dev_out_flistp, NULL);
	
	return;
}

/*******************************************************************************
 * fm_ip_pol_apn_change_state()
 *
 * Inputs: flist with i_flistp and APN POID
 *
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function reads APN_OBJ STATE if the state is NEW change it to USABLE
 *
 ******************************************************************************/
static void
fm_ip_pol_apn_change_state(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*apn_poidp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp) {
	
	pin_flist_t	*read_flistp = NULL;       /* flist for read obj */
	pin_flist_t	*set_state_flistp = NULL;  /* flist for set State */
	pin_flist_t	*out_flistp = NULL;	   /* flist for output */
	pin_flist_t	*set_state_out_flistp = NULL;/* flist for set state output */
	char 		*prog_name = NULL;	 /* Program Name */
	int32		new_state = PIN_APN_USABLE_STATE ; /*new Dev state */
	int32		*current_state = NULL;   /*Present state of Device */ 

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_apn_change_state entry error", ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);
	
	/*
	 * Read the APN OBJ for STATE 
	 */

	read_flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_POID,(void *)apn_poidp, ebufp);

	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_STATE_ID, (void *)NULL,
			 	ebufp);

	PCM_OP(ctxp, PCM_OP_READ_FLDS, flags ,		
		 read_flistp, &out_flistp, ebufp);

	 if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_apn_change_state READ FLDS ERROR", ebufp);

		goto cleanup;
	}
	
	current_state = (int32 *) PIN_FLIST_FLD_GET(out_flistp, PIN_FLD_STATE_ID,
					0, ebufp);
	 if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ip_pol_apn_change_state PIN_FLD_STATE_ID GET ERROR", ebufp);

		goto cleanup;
	}

	/*
	 * If the APN device is in NEW STATE change it to USABLE STATE
	 */

	if ( (current_state != NULL ? *current_state : 0) == PIN_APN_NEW_STATE ) {

		set_state_flistp = PIN_FLIST_CREATE(ebufp);

		PIN_FLIST_FLD_SET(set_state_flistp, PIN_FLD_POID,
					(void *)apn_poidp, ebufp);

		PIN_FLIST_FLD_SET(set_state_flistp, PIN_FLD_NEW_STATE,
					&new_state, ebufp);

		prog_name = (char *) PIN_FLIST_FLD_GET(i_flistp, 
					PIN_FLD_PROGRAM_NAME, 
					1, ebufp);

		if ( prog_name == NULL) {
		  prog_name = FILE_LOGNAME;
		}

		PIN_FLIST_FLD_SET(set_state_flistp, PIN_FLD_PROGRAM_NAME,
					(void *)prog_name, ebufp);

		PCM_OP(ctxp, PCM_OP_DEVICE_SET_STATE, flags,
			set_state_flistp, &set_state_out_flistp, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				" fm_ip_pol_apn_change_state : "
				" PCM_OP_DEVICE_SET_STATE error ", ebufp);

			goto cleanup;
		}
	}

	cleanup:
		PIN_FLIST_DESTROY_EX(&read_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&set_state_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&set_state_out_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&out_flistp, NULL);

	return;
}
