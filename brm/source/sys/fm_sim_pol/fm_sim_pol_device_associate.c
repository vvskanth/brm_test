/*
 * (#)$Id: fm_sim_pol_device_associate.c /cgbubrm_mainbrm.portalbase/1 2019/03/06 06:47:06 bsindhuj Exp $
 *
 * Copyright (c) 2001, 2023, Oracle and/or its affiliates.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "(#)$Id: fm_sim_pol_device_associate.c /cgbubrm_mainbrm.portalbase/1 2019/03/06 06:47:06 bsindhuj Exp $";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#include "cm_fm.h"
#include "ops/sim.h"
#include "ops/device.h"
#include "pin_errs.h"
#include "pin_sim.h"
#include "pin_num.h"
#include "pin_device.h"
#include "pin_gsm.h"

#define FILE_LOGNAME "fm_sim_pol_device_associate.c(1)"

EXPORT_OP void
op_sim_pol_device_associate(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp);


#define FILE_SOURCE_ID  "fm_sim_pol_device_associate.c(1.6)"

/**********************************************************************
 * Routines contained within
 **********************************************************************/
static void
fm_sim_pol_device_associate(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp);

static void
fm_sim_pol_device_disassociate(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	int32			svc_count,
	pin_errbuf_t		*ebufp);

static void 
fm_sim_pol_device_srvc_exists(
	pin_flist_t	*i_flistp,
	pin_flist_t	*db_flistp,
	pin_errbuf_t	*ebufp);

static int 
fm_sim_pol_device_check_srvc_dup(
	pin_flist_t	*s_flistp,
	char 		*svc_type,
	pin_errbuf_t	*ebufp);

static void 
fm_sim_pol_device_check_network_elem(
	pcm_context_t	*ctxp,
	pin_flist_t	* i_flistp,
	poid_t		*svc_pdp,
	pin_errbuf_t 	*ebufp);

static void 
fm_sim_pol_device_check_brand(
	pcm_context_t	*ctxp,
	poid_t		*cur_pdp,
	pin_flist_t	*r_flistp,
	pin_errbuf_t 	*ebufp);

static void
fm_sim_pol_alias_to_services(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        char                    *alias,
        int32                   is_assct,
        pin_errbuf_t            *ebufp);

static void
fm_sim_pol_check_service_status(
        pcm_context_t           *ctxp,
        poid_t                  *service_poidp,
        pin_errbuf_t            *ebufp) ;

/**********************************************************************
 * Main routine for the PCM_OP_SIM_POL_DEVICE_ASSOCIATE operation.
 **********************************************************************/
void
op_sim_pol_device_associate(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flp,
	pin_flist_t		**o_flpp,
	pin_errbuf_t		*ebufp) {

	
	pcm_context_t	*ctxp = connp->dm_ctx;
	int32		svc_count = 0;		/* no of srvc objs on flist */
	int32		assoc_flag = 0;   /* associate/ disassociate flag */
	void		*vp = NULL;		/* void ptr	*/	

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/*********/
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_SIM_POL_DEVICE_ASSOCIATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_sim_pol_device_associate", ebufp);
		return;
	}

	/*
	 * DEBUG: What did we get?
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_sim_pol_device_associate input flist", i_flp);

	/*
	 * By default, we simply copy the input to the output..
	 */
	
	*o_flpp = PIN_FLIST_COPY(i_flp, ebufp);

	/*
	 * Check if the sim pre provisioning opcode is calling this 
	 * policy code. If that is true then we do not have to perform 
	 * any service associate validation as the device is in new
	 * state and no service is associated with the device yet
	 */
	 if (fm_utils_op_is_ancestor(connp->coip,
		PCM_OP_SIM_DEVICE_PROVISION)) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
		"Return, opcode is invoked by PCM_OP_SIM_DEVICE_PROVISION");
		return;
		}

	/*
	 * count the number of service objects passed in
	 */
	svc_count = PIN_FLIST_ELEM_COUNT(i_flp, PIN_FLD_SERVICES, ebufp);

	if(svc_count <= 0){

		/*
		 * no validation required
		 */
		return;
		/*********/
	}


	/*
	 * start the validation
	 */


	/*
	 * check for  association or disassociation first
	 */

	vp = PIN_FLIST_FLD_GET(i_flp, PIN_FLD_FLAGS, 0, ebufp);
	if(vp != (void *)NULL){
		assoc_flag = *(int32 *)vp;
	}

	if((assoc_flag & PIN_DEVICE_FLAG_ASSOCIATE) ==
		 PIN_DEVICE_FLAG_ASSOCIATE){

		/* 
		 * must be association, do the required validation
		 */
		fm_sim_pol_device_associate(ctxp, i_flp, ebufp);
	}
	else{
		/*
		 * call the function to change the state of the device
		 */
		fm_sim_pol_device_disassociate(ctxp, i_flp, svc_count, ebufp);
	}

	/*
	 * Results.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(o_flpp, NULL);
		*o_flpp = (pin_flist_t *)NULL;
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_sim_pol_device_associate error", ebufp);
	}else {
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_sim_pol_device_assoicate return flist", *o_flpp);
	}
	
	return;

}

/****************************************************************************
 * fm_sim_pol_device_associate():
 *
 * The validation specific to sim devices  before associating a service goes
 * here
 * assumption: All the services passed in are unique and valid
 * checks that are made: No duplicate services are being added
 *			 Network element of the device is the same as
 * 	the network element of other num devices associated with the service
 *			Account object of service is same as the account
 * 	object of existing services
 * Finally if device state is in RELEASE set it to ASSIGN
 * input:
 *	i_flistp: the opcode input flist
 * output:
 *	ebuf is set if there is an error 
 *****************************************************************************/
static void
fm_sim_pol_device_associate(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp) {

	pin_flist_t	*flistp = NULL;		/* flist  for read and set */
	pin_flist_t	*r_flistp = NULL;	/* return flist ptr	*/
	pin_flist_t	*r_flistp2 = NULL;	/* another return flist ptr*/
	pin_flist_t	*nsvc_flistp = NULL;	/* single new service element */
	pin_flist_t	*sub_flistp = NULL;	/* flist for network elem */
	pin_flist_t	*sv_flistp = NULL;	/* flist for existing service */
	
	poid_t		*acct_pdp = NULL;	/* acct poid of srvc obj */
	poid_t		*cur_pdp = NULL;     /* acct poid of cur srvc in loop */
	poid_t		*cur_spdp = NULL;     /* acct poid of cur srvc in loop*/
	poid_t		*poidp = NULL;		/* poid of the device	*/
	poid_t		*acc_pdp = NULL;	/* poid of existing acc	*/
	int		elem_id = 0;		/* element id		*/
	pin_cookie_t	cookie = NULL;		/* cookie to recurse array */
	int32		state = PIN_SIM_ASSIGNED_STATE; /* state of device */
	int32		old_state = 0; 		/* old state of device */
	void*		vp = NULL;		/*temporary pointer*/
	char*           imsi = NULL;            /* imsi of the device */


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/********/
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	/*
	 * start the validation
	 */

	
	/*
	 * do a read flds to get the services array from the device
	 */
	flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, poidp, ebufp);
	
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_STATE_ID, (void *)NULL, ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_ACCOUNT_OBJ, (void *)NULL, ebufp);

	PIN_FLIST_ELEM_SET(flistp, NULL, PIN_FLD_SERVICES, 
		PIN_ELEMID_ANY, ebufp);

	sub_flistp = PIN_FLIST_SUBSTR_ADD(flistp, PIN_FLD_DEVICE_SIM,  
		 ebufp);
	
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_NETWORK_ELEMENT, (void *)NULL,
		 ebufp);

	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_IMSI, (void *)NULL,
		 ebufp);

	/*
	 * call the read flds opcode
	 */
	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, flistp, 
		&r_flistp, ebufp); 

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&flistp, NULL);
		return;
	}


	/*
	 * Get device current state 
	 */
	vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATE_ID, 0, ebufp);
	if (vp != NULL){
		old_state = *(int32 *)vp ;
	}
	/*
	 * loop through the array and compare this poid with 
	 * the account poid of the other service objects and make
	 * sure they match. Else set an error.
	 * at the same time check if this service is already
	 * associated with the device.
	 */
	while((nsvc_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_SERVICES,
		&elem_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {

		/*
		 * get the account poid from each service
		 */
		cur_pdp = PIN_FLIST_FLD_GET(nsvc_flistp, PIN_FLD_ACCOUNT_OBJ, 0,
			ebufp);
		PIN_POID_SET_DB(cur_pdp, cm_fm_get_current_db_no(ctxp));
		if(acct_pdp == NULL){
			acct_pdp = cur_pdp;
			sv_flistp = PIN_FLIST_ELEM_GET(r_flistp,
				 PIN_FLD_SERVICES, PIN_ELEMID_ANY, 1, ebufp); 

			if (sv_flistp != NULL){
				acc_pdp = PIN_FLIST_FLD_GET (sv_flistp, 
					PIN_FLD_ACCOUNT_OBJ, 1, ebufp);

				if (acc_pdp != NULL) {

					/*
		 			 * compare the acct poid with the
					 * account poid of any existing service
					 *  already associated with the device.
		 			 */
					if(PIN_POID_COMPARE(acct_pdp, acc_pdp,
						0, ebufp) != 0){
						/*
			 			 * set error
			 			 */
						pin_set_err(ebufp,PIN_ERRLOC_FM,
						PIN_ERRCLASS_APPLICATION, 
						PIN_COMPARE_NOT_EQUAL,
						PIN_FLD_ACCOUNT_OBJ, 0, 0);
				           PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						"fm_sim_pol_device_associate - "
						"account poid does not match "
						"account  poid of the existing "
						"service ", ebufp);
						break;
					}
				}
			}
		}

		/*
		 * compare the acct poids between accounts 
		 */
		if(PIN_POID_COMPARE(acct_pdp, cur_pdp, 0, ebufp) != 0){
			/*
			 * set error
			 */
			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_APPLICATION, 
				PIN_COMPARE_NOT_EQUAL,
				PIN_FLD_ACCOUNT_OBJ, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				"fm_sim_pol_device_associate - "
				"account poids between services are "
				"not the same", ebufp);
			break;
		}
		/*
                 * get the service poid from each service
                 */	
		cur_spdp = PIN_FLIST_FLD_GET(nsvc_flistp,
			PIN_FLD_SERVICE_OBJ, 1, ebufp);

		/*
		 * If the service poid is not null (i.e) service level association
		 * check if devices with the same service 
		 *  have the same NETWORK_ELEMENT
		 */
		if (! PIN_POID_IS_NULL(cur_spdp) ) {
			fm_sim_pol_device_check_network_elem(ctxp, r_flistp,
				 cur_spdp, ebufp); 
			if (PIN_ERR_IS_ERR(ebufp)) {
                        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "fm_sim_pol_device_check_network_elem error ", ebufp);
				goto cleanup;
                        }
                        /*
			 * If the status of the service is closed throw error message
			 */

			fm_sim_pol_check_service_status(ctxp, cur_spdp,
				ebufp);
			if (PIN_ERR_IS_ERR(ebufp)) {
				PIN_ERR_LOG_POID(PIN_ERR_LEVEL_ERROR,
					"fm_sim_pol_check_service_status Service Poid status is closed",
					 cur_spdp);
				goto cleanup;
			}
		}

	}

	/*
	 * check for duplicate services 
	 */
	fm_sim_pol_device_srvc_exists( i_flistp, r_flistp, ebufp);

	/*
	 * Validate the services associated with the device has the
	 * same brand as the device brand 
	 */

	fm_sim_pol_device_check_brand (ctxp, cur_pdp, r_flistp, ebufp);

	/*
	 * at this point if there are no errors. then do the device state
	 */

	if(old_state != PIN_SIM_ASSIGNED_STATE){ 
	
		PIN_FLIST_DESTROY_EX(&flistp, NULL);

		flistp = PIN_FLIST_CREATE(ebufp);

		PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, poidp, ebufp);

		PIN_FLIST_FLD_SET(flistp, PIN_FLD_NEW_STATE, 
			&state, ebufp);

		PCM_OP(ctxp, PCM_OP_DEVICE_SET_STATE,  0, flistp, &r_flistp2,
			 ebufp);
                /*
                 * Results.
                 */
                if (PIN_ERR_IS_ERR(ebufp)) {
                        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                                "fm_sim_pol_device_associate error during changing state", ebufp);
                }else {
                        PIN_ERR_CLEAR_ERR(ebufp);
                        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                                "fm_sim_pol_device_associate after  changing state", r_flistp2);
                }
	}

	/* get the imsi */
	sub_flistp = PIN_FLIST_SUBSTR_GET(r_flistp, PIN_FLD_DEVICE_SIM,
					  0, ebufp);
	imsi = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_IMSI, 0, ebufp);	
	/*
	 * at this point if there are no errors. 
	 * Add the imsi in the service alias list. PIN_BOOLEAN_TRUE for is_assct is for association.
	 */

	fm_sim_pol_alias_to_services(ctxp, i_flistp, imsi, PIN_BOOLEAN_TRUE, ebufp);

	cleanup:
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&r_flistp2, NULL);
		PIN_FLIST_DESTROY_EX(&flistp, NULL);
}


/***********************************************************************
 * fm_sim_pol_device_disassociate():
 *
 *	The validation specific to sim devices goes here.
 * if flags = DISASSOCIATE, then this function will be called. will 
 * check if there are going to be no services associated with the 
 * device at the end of the call, the state of the device will be 
 * changed to unassigned
 * assumption: all services passed in are unique and have no duplicates
 * input:
 *	svc_count : the number of services being disassociated
 *	pdp : the device poid to read the number of existing services
 *	flags: DISASSOCIATE flag value
 * output:
 *	ebuf is set if there is any error
 ***********************************************************************/
static void
fm_sim_pol_device_disassociate(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	int32			svc_count,
	pin_errbuf_t		*ebufp){

	int32		count = 0;		/* count of service objects */
	pin_flist_t	*s_flistp = NULL;	/* search flist for counting */
	pin_flist_t	*r_flistp = NULL; 	/* return flist		    */
	pin_flist_t	*flistp = NULL;	    /* flist for setting state of dev */
	pin_flist_t	*sub_flistp = NULL;
	poid_t		*poidp = NULL;		/* device poid	*/
	void		*vp = NULL;		/* void ptr */
	int32		state = PIN_SIM_UNASSIGNED_STATE;  /* state of device*/
	int32		old_state = 0;  	/*  old state of device*/
	char		*imsi = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/*********/
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * check for the number of services currently associated with the 
	 * device.							 
	 */
	s_flistp = PIN_FLIST_CREATE(ebufp);
	
	
	poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_POID, poidp, ebufp);
	
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_STATE_ID, (void *)NULL, ebufp);

	PIN_FLIST_ELEM_SET(s_flistp, NULL, PIN_FLD_SERVICES, PIN_ELEMID_ANY,
		ebufp);

	sub_flistp = PIN_FLIST_SUBSTR_ADD(s_flistp, PIN_FLD_DEVICE_SIM,
                 ebufp);

	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_IMSI, (void *)NULL,
                 ebufp);

        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_NETWORK_ELEMENT, (void *)NULL,
                 ebufp);


	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, s_flistp, 
		&r_flistp, ebufp);

	sub_flistp = PIN_FLIST_SUBSTR_GET(r_flistp, PIN_FLD_DEVICE_SIM, 0, ebufp);

        imsi = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_IMSI, 0, ebufp);


	count = PIN_FLIST_ELEM_COUNT(r_flistp, PIN_FLD_SERVICES, ebufp);

	/*
	 * now compare this count with the number of service objects on the
	 * incoming flist. if they are equal then change the device state to
	 * unassigned. Else return.					   
	 */
	if(svc_count == count){

		vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATE_ID, 0, ebufp);
		if ( vp != NULL) {
			old_state = *(int32 *)vp;
		}
		/*
		 * Now move the device state to unassigned state 
		 */ 
		if(old_state != PIN_SIM_UNASSIGNED_STATE){ 
			
			flistp = PIN_FLIST_CREATE(ebufp);

			PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, poidp, ebufp);

			PIN_FLIST_FLD_SET(flistp, PIN_FLD_NEW_STATE, &state,
				ebufp);

			PIN_FLIST_DESTROY_EX(&s_flistp, NULL); 
		
			PCM_OP(ctxp, PCM_OP_DEVICE_SET_STATE, 0, flistp,
				 &s_flistp, ebufp);
		}
	}

	/*
	 * Get the IMSI.
	 * At this point if we have disassociate the device from the service
	 * we still need to delete the imsi from the service alias list
	 */
	
	
	fm_sim_pol_alias_to_services(ctxp, i_flistp, imsi, PIN_BOOLEAN_FALSE, ebufp);

		PIN_FLIST_DESTROY_EX(&flistp, NULL); 
		PIN_FLIST_DESTROY_EX(&s_flistp, NULL); 
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL); 
}

/********************************************************************
 * fm_sim_pol_device_check_srvc_dup():
 * This method will check if the passed in service type is already
 * associated with the given device. It will retreive all the existing
 * services and loop through them and compare the service type
 * This does not account for subclasses and may need to be addressed.
 * input :
 *	s_flistp: the array of all the services associated with the 
 *		current device
 *	svc_type: the service type that we are validating
 * output :
 *	the number of times the service type was found in the flist	
 *
 ********************************************************************/
static int 
fm_sim_pol_device_check_srvc_dup(
	pin_flist_t	*s_flistp,
	char 		*svc_type,
	pin_errbuf_t	*ebufp){


	char		*sp_type = NULL;	/* srvc type in input flist */
	poid_t		*s_poidp = NULL;	/* service poid in input flist*/
	pin_flist_t	*flistp = NULL;		/* individual service flists */
	int		elem_id = 0;		/* array element	*/
	pin_cookie_t	cookie = NULL;		/* to loop through array */
	int		srvc_exists = 0; 	/* counter for existing service */

	if (PIN_ERR_IS_ERR(ebufp)) {
		return 0;
		/*********/
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_sim_pol_device_check_srvc_dup input flist", s_flistp);


	/*
	 * loop through the array to check for duplicates
	 */
	while((flistp = PIN_FLIST_ELEM_GET_NEXT(s_flistp, PIN_FLD_SERVICES,
		&elem_id, 1, &cookie, ebufp) ) != (pin_flist_t *)NULL) { 

		s_poidp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_SERVICE_OBJ, 1,
			ebufp);
		if ( ! PIN_POID_IS_NULL(s_poidp) ) {
			sp_type = (char*)PIN_POID_GET_TYPE(s_poidp);

			/*
		 	* compare the two service types - if the service type 
		 	* is /service/telephony two instances are allowed.
		 	*/
			if(strcasecmp(sp_type, svc_type) == 0){
				srvc_exists++;
			}
		}

	}
 	return (srvc_exists);

}

/********************************************************************
 * fm_sim_pol_device_srvc_exists():
 * This method will check if the passed in service type is already
 * associated with the given device. It will retreive all the existing
 * services and loop through them and compare the service type
 * This does not account for subclasses and may need to be addressed.
 * input :
 *	i_flistp: the array of all the services associated with the 
 *		current device
 *	svc_pdp: the service poid that we are validating
 * output :
 *	ebuf is set if there is an error
 *
 ********************************************************************/
static void 
fm_sim_pol_device_srvc_exists(
	pin_flist_t	*is_flistp,
	pin_flist_t	*db_flistp,
	pin_errbuf_t	*ebufp){

	char		*sp_type = NULL;	/* srvc type in input flist */
	char		*rsp_type = NULL;	/* srvc type in incoming srvc */
	poid_t		*s_poidp = NULL;	/* service poid in input flist*/
	poid_t		*rs_poidp = NULL;	/* service poid in db flist*/
	poid_t		*tmp_pdp = NULL;	/*  temporary holder */
	char		*tmp_type = NULL;	/* type of temporary poid */
	pin_flist_t	*svc_flistp = NULL;	/* individual service flists */
	pin_flist_t	*rsvc_flistp = NULL;	/* individual service flists */
	int		elem_id = 0;		/* array element	*/
	int		elem_id2 = 0;		/* array element	*/
	pin_cookie_t	cookie = NULL;		/* to loop through array */
	pin_cookie_t	cookie2 = NULL;		/* to loop through array */
	int		srvc_exists = 0; /*  counter for services telephony  */
	int		srvc_telephony = 0; /*  counter for srvc telephony  */

	if (PIN_ERR_IS_ERR(ebufp)) {
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_sim_pol_device_srvc_exists input flist", is_flistp);

		return;
		/*********/
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_sim_pol_device_srvc_exists input flist", is_flistp);


	/*
	 * loop through the services and check for duplicates 
	 */
	while ( ( svc_flistp = PIN_FLIST_ELEM_GET_NEXT(is_flistp,
		PIN_FLD_SERVICES, &elem_id, 1, &cookie, ebufp) )
		!= (pin_flist_t *)NULL) {

		s_poidp = PIN_FLIST_FLD_GET(svc_flistp, PIN_FLD_SERVICE_OBJ, 1,
			ebufp);
		
		/*
		 * No need to check duplicates if poid is null
		 */
		if ( PIN_POID_IS_NULL(s_poidp) ) {
			continue;
		}	

		sp_type = (char *)PIN_POID_GET_TYPE(s_poidp);

		/*
		 * check if the service exists already in the list of
		 * services already associated with the device
		 */
		srvc_exists = fm_sim_pol_device_check_srvc_dup(db_flistp, 
			sp_type, ebufp);
		if(srvc_exists > 0){
			/*
			 * check for /service/gsm/telephony
			 */
			if(strcasecmp(sp_type,
					 PIN_OBJ_TYPE_SERVICE_GSM_TEL) == 0) {
				/* 
				 * its ok to have two instances of this
				 */
				if(srvc_exists > 1){
					pin_set_err(ebufp, PIN_ERRLOC_FM,
						PIN_ERRCLASS_APPLICATION,
						 PIN_ERR_DUPLICATE,
						PIN_FLD_SERVICE_OBJ, 0, 0);
					PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						"fm_sim_pol_device_associate - "
						" Duplicate service exists ",
						 ebufp);
					break;

				}
				else {
					srvc_telephony ++;
				}
			}
			else {
				pin_set_err(ebufp, PIN_ERRLOC_FM,
					PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE,
					PIN_FLD_SERVICE_OBJ, 0, 0);
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_sim_pol_device_associate - "
					" Duplicate service exists ", ebufp);
				break;
			}

		}

		/*
		 * now check if the service exists more than once in the
		 * input flist. /service/gsm/telephony is allowed to be there
		 * twice provided there are no instances of it already
		 * associated with the device.
		 */

		srvc_exists = fm_sim_pol_device_check_srvc_dup(is_flistp,
			 sp_type, ebufp);
		if(srvc_exists > 1) {
			if(strcasecmp(sp_type, PIN_OBJ_TYPE_SERVICE_GSM_TEL)
				== 0) {
				if((srvc_exists > 2) || (srvc_telephony > 0)) {
					pin_set_err(ebufp, PIN_ERRLOC_FM,
						PIN_ERRCLASS_APPLICATION,
						 PIN_ERR_DUPLICATE,
						PIN_FLD_SERVICE_OBJ, 0, 0);
					PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						"fm_sim_pol_device_associate - "
						" Duplicate service exists ",
						 ebufp);
					break;


				}
			}
			else {
				pin_set_err(ebufp, PIN_ERRLOC_FM,
					PIN_ERRCLASS_APPLICATION, PIN_ERR_DUPLICATE,
					PIN_FLD_SERVICE_OBJ, 0, 0);
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_sim_pol_device_associate - "
					" Duplicate service exists ", ebufp);
				break;
			}
		}

		
	}
}


/********************************************************************
 * fm_sim_pol_device_check_network_elem():
 * This method will validate that the network element of the device 
 * is the same as the network element of the num devices that the same
 * service is associated with.
 * It goes through and reads the network element of the sim device. it 
 * then reads the network element of any one num device that the 
 * service is already associated with. Compares the two network elems.
 * assumption: All the services passed in are unique and valid
 * input :
 *	i_flistp: the flist containing the network elem of the sim 
 *	svc_pdp: the service being associated with the device
 * output :
 *	ebuf is set if there is an error
 *
 ********************************************************************/
static void 
fm_sim_pol_device_check_network_elem(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	poid_t		*svc_pdp,
	pin_errbuf_t 	*ebufp){

	pin_flist_t		*s_flistp = NULL;	/* search flist	*/
	pin_flist_t		*sub_flistp = NULL;	/* flist for args */
	pin_flist_t		*sub_sflistp = NULL;	/* flist for srvc arg */
	pin_flist_t		*num_flistp = NULL;	/* flist for substr */
	pin_flist_t		*sr_flistp = NULL;	/* search return flist*/
	pin_flist_t		*r_flistp = NULL;	/* search return flist*/
        pin_flist_t		*srch_inp_flistp = NULL; /* Search inp Flist */
        pin_flist_t		*srch_sub_flistp = NULL; /* Search sub Flist */
	pin_flist_t		*srch_out_flistp = NULL; /* Search out Flist */
	poid_t			*pdp = NULL; /* Routing poid */
	poid_t			*spoidp = NULL;		/* search poid */
	poid_t			*num_pdp = NULL;	/* number poid */

	pin_cookie_t		cookie = NULL;		/* to iterate  array */
	char			*net_elem_old = NULL;	/*  new network elem */
	char			*net_elem = NULL;	/*  cur network elem */
	void			*vp = NULL;		/* void ptr */
	int32			flag = SRCH_DISTINCT;	/* search opcode flag */
	int64			dbno = 0;		/* db num for search */
	int32			count = 0;		/* count array elems */
	int32			rec_count = 0;		/* count array elems */
	static int32		num_device_exists = 0;  /* to hold the Num Device status*/

	/*  As the num_device_exists is  used as Read only type of object */
	/* it  will not give thread problem */

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/*********/
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * create the search poid
	 */
	dbno = PIN_POID_GET_DB(svc_pdp);

	if ( num_device_exists == 0) {

		/*
		 *Create the Service Flist to Read the /device/num
		 */
		srch_inp_flistp = PIN_FLIST_CREATE ( ebufp );

		pdp = PIN_POID_CREATE ( dbno, "/dd/objects", 0, ebufp );
		PIN_FLIST_FLD_PUT ( srch_inp_flistp, PIN_FLD_POID, pdp, ebufp );

		srch_sub_flistp = PIN_FLIST_ELEM_ADD (srch_inp_flistp, PIN_FLD_OBJ_DESC, 0, ebufp );
		PIN_FLIST_FLD_SET (srch_sub_flistp, PIN_FLD_NAME, (void *)"/device/num", ebufp);

		PCM_OP (ctxp, PCM_OP_GET_DD, 0, srch_inp_flistp, &srch_out_flistp, ebufp);

		if (PIN_ERR_IS_ERR (ebufp)) {
			PIN_ERR_LOG_EBUF ( PIN_ERR_LEVEL_ERROR,
				" fm_sim_pol_check_network_element: "
				" error in PCM_OP_GET_DD", ebufp );

			 goto cleanup;
		}

		rec_count = PIN_FLIST_ELEM_COUNT( srch_out_flistp, PIN_FLD_OBJ_DESC, ebufp);

		if (rec_count <= 0) {
			/*
			 * /device/num Object doesn't exist Comparision of PIN_FLD_NETWORK_ELEMENT
			 * is not required.
			 */
			PIN_ERR_LOG_MSG ( PIN_ERR_LEVEL_DEBUG,
					 " /device/num Object doesn't exist "
					 "PIN_FLD_NETWORK_ELEMENT comparision is not required");
			num_device_exists = 1;
			goto cleanup;
		}
		num_device_exists = 2;
	}

	if ( num_device_exists == 1) {
		PIN_ERR_LOG_MSG ( PIN_ERR_LEVEL_DEBUG,
				 " /device/num Object doesn't exist "
				 "PIN_FLD_NETWORK_ELEMENT comparision is not required");
		goto cleanup;
	}
	/*
	 * do a search on the db for all num devices which are associated 
	 * with the service. On the return flist request the network  	  
	 * element .
	 */
	s_flistp = PIN_FLIST_CREATE(ebufp);
	spoidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);
	num_pdp = PIN_POID_CREATE(dbno, PIN_OBJ_TYPE_DEVICE_NUM, -1, ebufp);

	/*
	 * populate the search flist
	 */
	PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, spoidp, ebufp);
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);



	/*
	 * template for search
	 * F1 = service associated with /device/num in db
	 * V1 = service being associated 
	 */
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_TEMPLATE, 
		(void *)"select X from /device/num where "
		"( F1 = V1  and F2 = V2 ) ", ebufp);

	/*
	 *  add the service poid as the argument for search
	 */
	sub_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);
	sub_sflistp = PIN_FLIST_ELEM_ADD(sub_flistp, PIN_FLD_SERVICES, 0,
		 ebufp);
	PIN_FLIST_FLD_SET(sub_sflistp, PIN_FLD_SERVICE_OBJ, svc_pdp, ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 2, ebufp);
	PIN_FLIST_FLD_PUT(sub_flistp, PIN_FLD_POID, num_pdp, ebufp);
	

	/*
	 * add the network element fld on  to the DEVICE_NUM substruct and
	 * tag that on to the results flist. That is the fld
	 * we are interested in retreiving.
	 */
	sub_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_RESULTS, 0, ebufp);

	num_flistp = PIN_FLIST_SUBSTR_ADD(sub_flistp, PIN_FLD_DEVICE_NUM,
		ebufp);	

	PIN_FLIST_FLD_SET(num_flistp, PIN_FLD_NETWORK_ELEMENT, (void *)NULL, 
		ebufp);	

	PCM_OP(ctxp, PCM_OP_SEARCH, PCM_OPFLG_CACHEABLE, s_flistp, 
		&sr_flistp, ebufp);

	if((r_flistp = PIN_FLIST_ELEM_GET_NEXT(sr_flistp, PIN_FLD_RESULTS, 
		&count, 1, &cookie, ebufp)) != NULL) {

	 	vp = PIN_FLIST_SUBSTR_GET(r_flistp, PIN_FLD_DEVICE_NUM, 0,
			ebufp);
		net_elem = (char *) PIN_FLIST_FLD_GET(vp, 
			PIN_FLD_NETWORK_ELEMENT, 0, ebufp); 
		if(net_elem != NULL){
		
			/*
			 * now retreive the nework element from the
			 *  existing device
			 */
			vp = PIN_FLIST_SUBSTR_GET(i_flistp, PIN_FLD_DEVICE_SIM,
				1, ebufp);
			
			net_elem_old =  (char *)PIN_FLIST_FLD_GET(vp, 
				PIN_FLD_NETWORK_ELEMENT, 1,ebufp);
			if(net_elem_old!= NULL){
			
				if(strcmp(net_elem_old,net_elem) != 0) {
				   /*
				    * there is a problem
				    */
				   pin_set_err(ebufp, PIN_ERRLOC_FM,
				   	PIN_ERRCLASS_APPLICATION,
			   	     	PIN_ERR_BAD_VALUE, 
				   	PIN_FLD_NETWORK_ELEMENT,
				   	0,0);
					PIN_ERR_LOG_EBUF(
				   	    PIN_ERR_LEVEL_ERROR,
			   		   "fm_sim_pol: network element"
					   " does not match", ebufp);
				}/*  strcmp */
			}/*  vp = network elem */

		}/*  vp = new network elem */
	}/*  results */
	/*
 	* clean up the mess
 	*/
	cleanup:
	PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&srch_inp_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&srch_out_flistp, NULL);
	return;
}


/********************************************************************
 * fm_sim_pol_device_check_brand():
 * This method will validate that the brand of the device 
 * is the same as the brand of the service that will be 
 * associated with the device.
 * It compares the brand object of the device with the brand obj 
 * of the account associated with the service 
 * input :
 *	r_flistp: the flist containing the brand obj of the device 
 *	cur_pdp: the account poid associated with the service
 * output :
 *	ebuf is set if there is an error
 *
 ********************************************************************/
static void 
fm_sim_pol_device_check_brand(
	pcm_context_t	*ctxp,
	poid_t		*cur_pdp,
	pin_flist_t	*r_flistp,
	pin_errbuf_t 	*ebufp){

	pin_flist_t		*ri_flistp = NULL;	/* read flist	*/
	pin_flist_t		*ro_flistp = NULL;	/* result flist	*/
	poid_t			*brand_pdp = NULL;	/* brand poid */
	poid_t			*dev_brand_pdp = NULL;	/* brand poid */

	if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
        PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Get the device brand object 
	 */
	 dev_brand_pdp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_ACCOUNT_OBJ,
		0, ebufp);
	PIN_POID_SET_DB(dev_brand_pdp, cm_fm_get_current_db_no(ctxp));


        /*
         * Get the brand of the account from the database.
         */
        ri_flistp = PIN_FLIST_CREATE(ebufp);

        PIN_FLIST_FLD_SET(ri_flistp, PIN_FLD_POID, cur_pdp, ebufp);
        PIN_FLIST_FLD_SET(ri_flistp, PIN_FLD_BRAND_OBJ, NULL, ebufp);

        PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, ri_flistp, &ro_flistp, ebufp);

        brand_pdp = (poid_t *)PIN_FLIST_FLD_GET(ro_flistp,
                PIN_FLD_BRAND_OBJ, 0, ebufp);
	PIN_POID_SET_DB(brand_pdp, cm_fm_get_current_db_no(ctxp));

        if (PIN_POID_COMPARE(dev_brand_pdp, brand_pdp, 0, ebufp) != 0) {

                pin_set_err(ebufp, PIN_ERRLOC_FM,
                        PIN_ERRCLASS_SYSTEM_DETERMINATE,
                        PIN_ERR_PERMISSION_DENIED,
                        PIN_FLD_BRAND_OBJ, 0, 0);

                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_sim_pol_check_brand non-matching brand "
                        "for device and service", ebufp);
        }

        PIN_FLIST_DESTROY_EX(&ri_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&ro_flistp, NULL);

        return;
}



/*******************************************************************
 * Function used to add or delete imsi alias to/from the service alias list
 * that are passed into the i_flist
 * input:
 *  i_flist: an flist, where we get services poid or account poid 
 *           from the PIN_FLD_SERVICES array
 *           An assumption is that only the services of one account 
 *           are are listed here 
 *  is_assct: This indicates whether this function is called from 
 *	      associate /disassociate(TRUE/FALSE) 
 *  alias : Add: alias to add
 *          Delete: NULL, indicates a delete
 *
 * output:
 *  ebuf: set in case of error
 *******************************************************************/
static void
fm_sim_pol_alias_to_services(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        char                    *alias,
        int32                   is_assct,
        pin_errbuf_t            *ebufp) 
{
        pin_flist_t             *s_flistp = NULL;      /* set flist     */
        pin_flist_t             *log_flistp = NULL;    /* login flist   */
        pin_flist_t             *al_flistp = NULL;     /* alias flist   */
        pin_flist_t            	*r_flistp = NULL;      /* return flist  */
        pin_flist_t             *svc_flistp = NULL;    /* service list flist*/
	pin_flist_t             *alias_flistp = NULL;	/* Alias list flist */
	pin_flist_t             *alias_in_flistp = NULL; /* Alias list flist */
        poid_t                  *svc_poid = NULL;      /* service poid */
        poid_t                  *acct_poid = NULL;     /* account poid */
        int32                   elem_id = 0;    /* array element        */
	int32			elem_id1 = 0;  /* array element */
	int32			max_elem_id = -1; /* count */
        pin_cookie_t            cookie = NULL;  /* to loop through array */
	pin_cookie_t            cookie1 = NULL;  /* to loop through array */
        void                    *vp=NULL;
	char			*al_name = NULL;

        if (PIN_ERR_IS_ERR(ebufp)) {
                return;
                /*********/
        }

        PIN_ERR_CLEAR_ERR(ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                        "fm_sim_pol_alias_to_services input flist ", i_flistp);

        /*
         * loop through the array of all services and 
         * build SET_LOGIN input flist and call the opcode
         */
        /* if END_T is passed by caller */
        vp=PIN_FLIST_FLD_GET(i_flistp,PIN_FLD_END_T,1,ebufp);

        while((svc_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_SERVICES,
                &elem_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {
                svc_poid = PIN_FLIST_FLD_GET(svc_flistp, PIN_FLD_SERVICE_OBJ,
                                             1, ebufp);

                /*
                 * No need to call SET_LOGIN opcode if service poid doesnt exist
                 */
                if ( PIN_POID_IS_NULL(svc_poid) ) {
                        continue;
                }
	
		/*
		* create and set the update_services input flist
		*/

		s_flistp = PIN_FLIST_CREATE(ebufp);

		PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_PROGRAM_NAME, FILE_SOURCE_ID,
			ebufp);

		/******************************************************************
		 * -> Read ALIAS_LIST from svc_obj.
		 *
		 * -> If this function is called because of associate then
		 *    find the max_elem_id and prepare the input to cust_set_login
		 *    to add the new element in the ALIAS_LIST.
		 *
		 * -> If this function is called because of disassociate then
		 *    loop through existing alias list and get the matching elem_id.
		 *    Use this elem id to delete the ALIAS_LIST.
		 *
		 * -> call cust_set_login.
		 *
		 * -> Above steps need to be done for each service which are associated
	   	 *    with this device.
		 *
		 * -> In case of same device associated then it is handled in
		 *    fm_device_associate(). So no need to handle in this case.
                 *******************************************************************/

		log_flistp = PIN_FLIST_CREATE(ebufp);	

		alias_in_flistp = PIN_FLIST_CREATE(ebufp);
		PIN_FLIST_FLD_SET(alias_in_flistp, PIN_FLD_POID, (void *)svc_poid, ebufp);

		PIN_FLIST_ELEM_SET(alias_in_flistp, NULL, PIN_FLD_ALIAS_LIST,
			PIN_ELEMID_ANY, ebufp);

		PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, alias_in_flistp,
			&alias_flistp, ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"Alias List in service object", alias_flistp);

		if (is_assct == PIN_BOOLEAN_TRUE) { /* Caller is pol_device_associate  */

			/*
                         * PIN_ELEMID_ASSIGN cannot be used here because CREATE_OBJ called
                         * during SET_LOGIN flow to recored the event is not supporting
		  	 * ELEMID_ASSIGN for the array element. Because of this limiation
			 * loop through ALIAS_LIST and get the max_elem_id.
			*/
			max_elem_id = -1;
			elem_id1 = 0;
			cookie1 = NULL;

			while ((PIN_FLIST_ELEM_GET_NEXT( alias_flistp, PIN_FLD_ALIAS_LIST,
				&elem_id1, 1, &cookie1, ebufp)) != (pin_flist_t *)NULL) {

				(max_elem_id < elem_id1)? (max_elem_id=elem_id1):0;
			}

			max_elem_id++; /* Get the next elem_id*/

			al_flistp = PIN_FLIST_ELEM_ADD(log_flistp, PIN_FLD_ALIAS_LIST,
					max_elem_id, ebufp);
			PIN_FLIST_FLD_SET(al_flistp, PIN_FLD_NAME, alias, ebufp);
		}
		else { /* Caller is pol_device_disassociate */
			elem_id1 = 0;
			cookie1 = NULL;

			while (( al_flistp = PIN_FLIST_ELEM_GET_NEXT( alias_flistp, PIN_FLD_ALIAS_LIST,
				&elem_id1, 1, &cookie1, ebufp)) != (pin_flist_t *)NULL) {

				al_name = PIN_FLIST_FLD_GET(al_flistp, PIN_FLD_NAME, 1, ebufp);

				if (strcmp(al_name, alias) == 0) {
					/*
					 * Remove the alias from the service alias list
					 * set the flist to NULL for the elem_id
					 */
					PIN_FLIST_ELEM_SET(log_flistp, (void*)NULL,
						PIN_FLD_ALIAS_LIST, elem_id1, ebufp);
					break;
				}
			}
		}


                acct_poid = PIN_FLIST_FLD_GET(svc_flistp, 
                                              PIN_FLD_ACCOUNT_OBJ,
                                              0, ebufp);
                PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_POID, 
                                  acct_poid, ebufp);
                PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_SERVICE_OBJ, 
                                  svc_poid, ebufp);
                PIN_FLIST_ELEM_PUT(s_flistp, log_flistp, 
				PIN_FLD_LOGINS, 0 , ebufp);
                /* if END_T is not NULL set it */
                if(vp)  {
                PIN_FLIST_FLD_SET(s_flistp,PIN_FLD_END_T,vp,ebufp);
                }

                PCM_OP(ctxp, PCM_OP_CUST_SET_LOGIN, 0, s_flistp, 
                       &r_flistp, ebufp);
                PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&alias_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&alias_in_flistp, NULL);
	}

}

/*******************************************************************
 * Function used to validate the Service status
 * input:
 *  service_poidp: service poid
 * output:
 *  ebuf: set in case of error
 *******************************************************************/
static void
fm_sim_pol_check_service_status(
        pcm_context_t           *ctxp,
        poid_t                  *service_poidp,
        pin_errbuf_t            *ebufp) {

	pin_flist_t             *service_inp_flistp = NULL;
	pin_flist_t             *service_out_flistp = NULL;
	pin_status_t            *service_statep = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	service_inp_flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET(service_inp_flistp, PIN_FLD_POID, (void *)service_poidp, ebufp);

	PIN_FLIST_FLD_SET(service_inp_flistp, PIN_FLD_STATUS, NULL, ebufp);

	/*
	 * Read the Service Poid and Get the service status.
	 */

	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, service_inp_flistp,
			&service_out_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_sim_pol_check_service_status PCM_OP_READ_FLDS error", ebufp);

		goto cleanup;
	}
	service_statep = (pin_status_t *) PIN_FLIST_FLD_GET(service_out_flistp,
		 PIN_FLD_STATUS, 0, ebufp);

	if (service_statep == NULL) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_sim_pol_check_service_status service status is null", ebufp);

                        goto cleanup;
        }

	if (*service_statep == PIN_STATUS_CLOSED) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_VALUE, PIN_FLD_STATUS, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_sim_pol_check_service_status Service status is closed ", ebufp);

		goto cleanup;
	}


	cleanup:
		PIN_FLIST_DESTROY_EX(&service_inp_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&service_out_flistp, NULL);

	return;

}
