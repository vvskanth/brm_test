/*
 * (#)$Id: fm_num_pol_device_associate.c /cgbubrm_mainbrm.portalbase/1 2019/03/11 00:02:48 mmaley Exp $ 
 *
* Copyright (c) 2001, 2023, Oracle and/or its affiliates.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "(#)$Id: fm_num_pol_device_associate.c /cgbubrm_mainbrm.portalbase/1 2019/03/11 00:02:48 mmaley Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/device.h"
#include "ops/num.h"
#include "pin_device.h"
#include "pin_num.h"
#include "pin_sim.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "fm_utils.h"
#include "pin_cust.h"

#define FILE_LOGNAME "fm_num_pol_device_associate.c"
#define PIN_PORT_OUT_REASON "Disconnect-Port-OUT"

/*******************************************************************************
 * Functions contained within this source file
 ******************************************************************************/
EXPORT_OP void
op_num_pol_device_associate(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_num_pol_device_associate(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_flist_t		*rfo_flistp,
	pin_errbuf_t		*ebufp);

static void
fm_num_pol_device_disassociate(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_flist_t		*rfo_flistp,
	pin_errbuf_t		*ebufp);

static int32
fm_num_pol_check_dup_service(
	poid_t			*i_poidp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);

static void 
fm_num_pol_check_network_element(
	pcm_context_t		*ctxp,
	poid_t			*i_poidp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t 		*ebufp);

static void
fm_num_pol_check_brand(
	pcm_context_t		*ctxp,
	poid_t			*dev_brand_poidp,
	poid_t			*acct_poidp,
	pin_errbuf_t 		*ebufp);

static void
fm_num_pol_alias_to_services(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        char                    *alias,
        int32                   is_assct,
        pin_errbuf_t            *ebufp);

static void 
fm_num_pol_check_service_status(
	pcm_context_t		*ctxp,
	poid_t			*i_poidp,
	pin_errbuf_t 		*ebufp);

/*******************************************************************************
 * Main routine for the PCM_OP_NUM_POL_DEVICE_ASSOCIATE operation
 ******************************************************************************/
void
op_num_pol_device_associate(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{	
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*rfi_flistp = NULL;
	pin_flist_t		*rfo_flistp = NULL;
	pin_flist_t		*sub_flistp = NULL;
	poid_t			*i_poidp = NULL;
	int32			*assoc_flag = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_NUM_POL_DEVICE_ASSOCIATE) {

		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_num_pol_device_associate", ebufp);

		*o_flistpp = NULL;
		return;
	}

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_num_pol_device_associate input flist", i_flistp);

	/*
	 * Validate the input flist has the correct poid type.
	 */
	i_poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	if (strcmp(PIN_POID_GET_TYPE(i_poidp), PIN_OBJ_TYPE_DEVICE_NUM) != 0) {

		pin_set_err(ebufp, PIN_ERRLOC_FM, 
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_POID_TYPE, PIN_FLD_POID, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
			"op_num_pol_device_associate error", ebufp);

		*o_flistpp = NULL;
		return;
	}

	/*
	 * Validate the input flist has at least one service element
	 */
	if (PIN_FLIST_ELEM_COUNT(i_flistp, PIN_FLD_SERVICES, ebufp) < 1) {

		pin_set_err(ebufp, PIN_ERRLOC_FLIST, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_MISSING_ARG, PIN_FLD_SERVICES, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
			"op_num_pol_device_associate error", ebufp);

		*o_flistpp = NULL;
		return;
	}

	/*
	 * Read the existing services array for this device along with its
	 * current state, and network_element values from the database.
	 * Also, get the brand of this device.
	 */
	rfi_flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET(rfi_flistp, PIN_FLD_POID, (void *)i_poidp, ebufp);

	PIN_FLIST_FLD_SET(rfi_flistp, PIN_FLD_STATE_ID, NULL, ebufp);
	PIN_FLIST_FLD_SET(rfi_flistp, PIN_FLD_ACCOUNT_OBJ, NULL, ebufp);

	/* 
	 *  need the msisdn for the service alias list which is 
	 *  in the device_id 
	 */
	PIN_FLIST_FLD_SET(rfi_flistp, PIN_FLD_DEVICE_ID, NULL, ebufp);

	PIN_FLIST_ELEM_SET(rfi_flistp, NULL, PIN_FLD_SERVICES, 
		PIN_ELEMID_ANY, ebufp);

	sub_flistp = PIN_FLIST_SUBSTR_ADD(rfi_flistp, 
		PIN_FLD_DEVICE_NUM, ebufp);
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_NETWORK_ELEMENT, NULL, ebufp);
	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, rfi_flistp, 
		&rfo_flistp, ebufp); 

	/*
	 * Perform the association or disassociation based on the input flag
	 */
	assoc_flag = (int32 *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_FLAGS,
		0, ebufp);

	if ((assoc_flag != NULL) && 
		((*assoc_flag & PIN_DEVICE_FLAG_ASSOCIATE) == 
		PIN_DEVICE_FLAG_ASSOCIATE)) {

		fm_num_pol_device_associate(ctxp, i_flistp, rfo_flistp, ebufp);

	} else {

		fm_num_pol_device_disassociate(ctxp, i_flistp, 
			rfo_flistp, ebufp);
	}

	if (PIN_ERR_IS_ERR(ebufp)) {

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_num_pol_device_associate error", ebufp);

		*o_flistpp = NULL;

	} else {

		/*
		 * Return a copy of the input flist
		 */
		*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);

		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_num_pol_device_assoicate return flist", 
			*o_flistpp);
	}

	PIN_FLIST_DESTROY_EX(&rfi_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&rfo_flistp, NULL);

	return;
}


/*******************************************************************************
 * fm_num_pol_device_associate()
 *
 * Inputs: flist with device poid and services array, each element including
 *         a service poid and an account poid; flist with device attributes
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function validates the input services against the given device, and
 * ensures the state of the device is Assigned if successful.
 ******************************************************************************/
static void
fm_num_pol_device_associate(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_flist_t		*rfo_flistp,
	pin_errbuf_t		*ebufp)
{
	pin_cookie_t		cookie = NULL;
	pin_flist_t		*i_serv_flistp = NULL;
	pin_flist_t		*db_serv_flistp = NULL;
	pin_flist_t		*dssi_flistp = NULL;
	pin_flist_t		*dsso_flistp = NULL;
	poid_t			*i_serv_acct_poidp = NULL;
	poid_t			*i_serv_serv_poidp = NULL;
	poid_t			*db_serv_acct_poidp = NULL;
	poid_t			*db_dev_brand_poidp = NULL;
	int32			elem_id = 0;
	int32			next_state = 0;
	int32			*db_state_id = NULL;
	int32			brand_checked = PIN_BOOLEAN_FALSE;
	void			*vp = NULL;
	char                    *msisdn = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Attempt to get an account_obj value from the existing services
	 * of the device.  It is possible that the device has no services
	 * at this time.
	 */
	if ((db_serv_flistp = (pin_flist_t *)PIN_FLIST_ELEM_GET(rfo_flistp,
		 PIN_FLD_SERVICES, PIN_ELEMID_ANY, 1, ebufp))
		 != NULL) {

		db_serv_acct_poidp = (poid_t *)PIN_FLIST_FLD_GET(
			db_serv_flistp, PIN_FLD_ACCOUNT_OBJ, 0, ebufp);
	}

	/*
	 * For each element of the input services array, validate the 
	 * account poid provided is the same as the account poid for the 
	 * existing services. In addition, ensure there are no duplicate 
	 * service types and that any /device/sim associated to these 
	 * services has the same network_element value.
	 */
	while(	(i_serv_flistp = (pin_flist_t *)
		PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_SERVICES,
		&elem_id, 1, &cookie, ebufp)) != NULL) {

		/*
		 * Get the account poid and service poid from this input
		 * service element
		 */
		i_serv_acct_poidp = (poid_t *)PIN_FLIST_FLD_GET(
			i_serv_flistp, PIN_FLD_ACCOUNT_OBJ, 1, ebufp);

		i_serv_serv_poidp = (poid_t *)PIN_FLIST_FLD_GET(
			i_serv_flistp, PIN_FLD_SERVICE_OBJ, 1, ebufp);

		/*
		 * Compare the brand of the device to one of the services.
		 */
		if (brand_checked == PIN_BOOLEAN_FALSE) {

			/*
			 * Get the brand of the device
			 */
			db_dev_brand_poidp = (poid_t *)PIN_FLIST_FLD_GET(
				rfo_flistp, PIN_FLD_ACCOUNT_OBJ, 0, ebufp);
			PIN_POID_SET_DB(db_dev_brand_poidp, cm_fm_get_current_db_no(ctxp));

			fm_num_pol_check_brand(ctxp, db_dev_brand_poidp,
				i_serv_acct_poidp, ebufp);

			if (PIN_ERR_IS_ERR(ebufp)) {
				return;
			}

			brand_checked = PIN_BOOLEAN_TRUE;
		}

		/*
		 * If the service poid is not null, (i.e) service level association
		 * Ensure their are no duplicate service types amongst the
		 * input services.  The function should find one match which
		 * which we acknowledge as the input poid from that flist.
		 */


		if (! PIN_POID_IS_NULL(i_serv_serv_poidp) ) {

			if (fm_num_pol_check_dup_service(i_serv_serv_poidp, i_flistp,
				ebufp) != 1) {

				pin_set_err(ebufp, PIN_ERRLOC_FM, 
					PIN_ERRCLASS_APPLICATION,
					PIN_ERR_DUPLICATE,
					PIN_FLD_SERVICE_OBJ, elem_id, 0);

				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_num_pol_device_associate duplicate "
					"service type found in input flist", ebufp);

				break;
			}

			/*
			 * Ensure their are no duplicate service types amongst the
			 * existing services for this device.  The function should
			 * should not find any matching service types.
			 */
			if (fm_num_pol_check_dup_service(i_serv_serv_poidp, rfo_flistp,
				ebufp) != 0) {

				pin_set_err(ebufp, PIN_ERRLOC_FM, 
					PIN_ERRCLASS_APPLICATION,
					PIN_ERR_DUPLICATE,
					PIN_FLD_SERVICE_OBJ, elem_id, 0);

				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_num_pol_device_associate duplicate "
					"service type found in existing services",
					ebufp);

				break;
			}
		}
		/*
		 * It is assumed that all current services in the database
		 * for this device have the same account_obj value.
		 * Compare the account_obj of the current input service element
		 * with an arbitrary account_obj from the device's existing
		 * services.
		 */
		if (db_serv_acct_poidp == NULL) {
			db_serv_acct_poidp = i_serv_acct_poidp;
		}

		if (PIN_POID_COMPARE(i_serv_acct_poidp, db_serv_acct_poidp,
			0, ebufp) != 0) {

			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_COMPARE_NOT_EQUAL, PIN_FLD_ACCOUNT_OBJ,
				elem_id, 0);

			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				"fm_num_pol_device_associate device has "
				"different account poid than provided as input",
				ebufp);

			break;
		}
	
		/*
		 * If the service poid is not null, (i.e) service level association
		 * If this service instance has an associated /device/sim(s),
		 * both of the devices should have the same network_element
		 * value.
		 */

		if (! PIN_POID_IS_NULL(i_serv_serv_poidp) ) {
			fm_num_pol_check_network_element(ctxp, i_serv_serv_poidp,
				rfo_flistp, ebufp);
			if (PIN_ERR_IS_ERR(ebufp)) {
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		                        "fm_num_pol_check_network_element Error", ebufp);
				return;
			}
			/*
			 * If the status of the service is closed throw error message
			 *
			 */
			fm_num_pol_check_service_status(ctxp, i_serv_serv_poidp,
				ebufp);
			if (PIN_ERR_IS_ERR(ebufp)) {
				PIN_ERR_LOG_POID(PIN_ERR_LEVEL_ERROR,
		                        "fm_num_pol_check_service_status Service poid status is closed", 
					i_serv_serv_poidp);

				return;
			}
		}
	}

	/* get the msisdn */
	msisdn = PIN_FLIST_FLD_GET(rfo_flistp, PIN_FLD_DEVICE_ID, 
				   0, ebufp);
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	
	/* 
	 * Add the msisdn to the services alias list in elem_id 1 
	 * PIN_BOOLEAN_TRUE for is_assct indicates this is 
	 * for association 
	 */
	fm_num_pol_alias_to_services(ctxp, i_flistp, msisdn, 
					    PIN_BOOLEAN_TRUE, ebufp);

	/*
	 * If the state of the device is New, Quarantined, or Unassigned
	 * set the state to Assigned.
	 *
	 * Note that additional constraints could be placed here to
	 * restrict a quarantined number from becoming assigned.
	 */
	db_state_id = (int32 *)PIN_FLIST_FLD_GET(rfo_flistp, PIN_FLD_STATE_ID,
		0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
     if(db_state_id != NULL) {
	switch (*db_state_id) {

	case PIN_NUM_STATE_NEW:
	case PIN_NUM_STATE_QUARANTINED:
	case PIN_NUM_STATE_UNASSIGNED:
	case PIN_NUM_STATE_PORT_IN_NEW:
	case PIN_NUM_STATE_PORT_OUT:
	case PIN_NUM_STATE_REPATRIATED:

		dssi_flistp = PIN_FLIST_CREATE(ebufp);

		vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
		PIN_FLIST_FLD_SET(dssi_flistp, PIN_FLD_POID, vp, ebufp);

		if ( (*db_state_id == PIN_NUM_STATE_PORT_IN_NEW) ||
		     (*db_state_id == PIN_NUM_STATE_PORT_OUT) ||
		     (*db_state_id == PIN_NUM_STATE_REPATRIATED) ) {
			next_state = PIN_NUM_STATE_TEMPORARY_ASSIGN;
		} else {
			next_state = PIN_NUM_STATE_ASSIGNED;
		}
	
		PIN_FLIST_FLD_SET(dssi_flistp, PIN_FLD_NEW_STATE, 
			(void *)&next_state, ebufp);

		PCM_OP(ctxp, PCM_OP_DEVICE_SET_STATE, 0, 
			dssi_flistp, &dsso_flistp, ebufp);

		PIN_FLIST_DESTROY_EX(&dssi_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&dsso_flistp, NULL);

		break;

	case PIN_NUM_STATE_ASSIGNED:

		/*
		 * The device is already in Assigned state; do nothing
		 */
		break;

	default:

		pin_set_err(ebufp, PIN_ERRLOC_FM, 
			PIN_ERRCLASS_SYSTEM_DETERMINATE, PIN_ERR_BAD_VALUE, 
			PIN_FLD_STATE_ID, 0, *db_state_id);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_num_pol_device_associate unknown state_id "
			"read for this device",	ebufp);
	}
      }
	return;
}


/*******************************************************************************
 * fm_num_pol_device_disassociate()
 *
 * Inputs: input flist with device poid and services, flist with device poid
 *         and services from database
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function changes the state of the device to Quarantined if it is
 * currently Assigned and all of the current services are being disassociated.
 ******************************************************************************/
static void
fm_num_pol_device_disassociate(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_flist_t		*rfo_flistp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*dssi_flistp = NULL;
	pin_flist_t		*dsso_flistp = NULL;
	int32			i_count = 0;
	int32			db_count = 0;
	int32			next_state = 0;
	int32			*db_state_id = NULL;
	int32			port_out_flag = 0;
	void			*vp = NULL;
	char 			*reason_code = NULL;
	char 			*port_out = PIN_PORT_OUT_REASON;
	char                    *msisdn = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/* 
	 * remove the msisdn from the alias list of the 
	 * disassociated services
	 */

	/* get the msisdn */
        msisdn = PIN_FLIST_FLD_GET(rfo_flistp, PIN_FLD_DEVICE_ID,
                                   0, ebufp);
        if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
	/* PIN_BOOLEAN_FALSE for is_assct indicates this is for disassociation */
	fm_num_pol_alias_to_services(ctxp, i_flistp, msisdn, PIN_BOOLEAN_FALSE, ebufp);

	/*
	 * Compare the number of services currently associated with the device
	 * against the number of services on the input flist.
	 */
	i_count = PIN_FLIST_ELEM_COUNT(i_flistp, PIN_FLD_SERVICES, ebufp);
	db_count = PIN_FLIST_ELEM_COUNT(rfo_flistp, PIN_FLD_SERVICES, ebufp);

	if (i_count < db_count) {

		/*
		 * Not all of the services are being disassociated; do nothing
		 */

	} else if (i_count == db_count) {

		/*
		 * If the state of the device is Assigned, set the state
		 * to Quarantined.  Else the current state does not support
		 * service associations; report an error.
		 */
		db_state_id = (int32 *)PIN_FLIST_FLD_GET(rfo_flistp, 
			PIN_FLD_STATE_ID, 0, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			return;
		}

		reason_code = (char *) PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_DESCR,1,ebufp);
		if (reason_code != NULL) {
			if ( strstr(reason_code,PIN_PORT_OUT_REASON) != NULL) {
				port_out_flag = 1;
				PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
					"found port_order  flist", i_flistp);
			}
		}
            if(db_state_id != NULL ) {
		 switch (*db_state_id) {

		case PIN_NUM_STATE_ASSIGNED:

			dssi_flistp = PIN_FLIST_CREATE(ebufp);

			vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID,
				0, ebufp);
			PIN_FLIST_FLD_SET(dssi_flistp, PIN_FLD_POID, vp, ebufp);

			if (port_out_flag ==  1) {
				next_state = PIN_NUM_STATE_QUARANTINED_PORT_OUT;
			} else {
				next_state = PIN_NUM_STATE_QUARANTINED;
			}

			PIN_FLIST_FLD_SET(dssi_flistp, PIN_FLD_NEW_STATE, 
				(void *)&next_state, ebufp);

			PCM_OP(ctxp, PCM_OP_DEVICE_SET_STATE, 0, 
				dssi_flistp, &dsso_flistp, ebufp);

			PIN_FLIST_DESTROY_EX(&dssi_flistp, NULL);
			PIN_FLIST_DESTROY_EX(&dsso_flistp, NULL);

			break;

		default:

			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_SYSTEM_DETERMINATE,
				PIN_ERR_BAD_VALUE, PIN_FLD_STATE_ID,
				0, *db_state_id);

			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_num_pol_device_disassociate invalid "
				"state_id for disassociation of services",
				ebufp);

		}
           }

	} else {  /* (i_count > db_count) */

		pin_set_err(ebufp, PIN_ERRLOC_APP, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE, PIN_FLD_SERVICES, 0, i_count);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_num_pol_device_disassociate more input services "
			"than device currently has", ebufp);
	}

	return;
}


/*******************************************************************************
 * fm_num_pol_check_dup_service()
 *
 * Inputs: service poid; flist with services array of service_obj values
 * Outputs: number of services with the same type as the input poid;
 *          ebuf set if errors encountered
 *
 * Description:
 * This function checks for duplicate service types in the services array.
 ******************************************************************************/
static int32
fm_num_pol_check_dup_service(
	poid_t			*i_poidp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp)
{
	pin_cookie_t		cookie = NULL;
	pin_flist_t		*i_serv_flistp = NULL;
	poid_t			*i_serv_serv_poidp = NULL;
	int32			elem_id = 0;
	int32			dup_count = 0;


	if (PIN_ERR_IS_ERR(ebufp)) {

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_num_pol_check_dup_service error", ebufp);

		return(-1);
	}
	PIN_ERR_CLEAR_ERR(ebufp);


	while(	(i_serv_flistp = (pin_flist_t *)
		PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_SERVICES,
		&elem_id, 1, &cookie, ebufp)) != NULL) {

		/*
		 * get the service poid from this input service element
		 */
		i_serv_serv_poidp = (poid_t *)PIN_FLIST_FLD_GET(
			i_serv_flistp, PIN_FLD_SERVICE_OBJ, 1, ebufp);

		if( ! PIN_POID_IS_NULL(i_serv_serv_poidp) ) {
			if ( strcmp(PIN_POID_GET_TYPE(i_poidp),
				PIN_POID_GET_TYPE(i_serv_serv_poidp)) == 0 ){
				dup_count++;
			}
		}

	}

	return(dup_count);
}


/*******************************************************************************
 * fm_num_pol_check_network_element()
 *
 * Inputs: service poid; flist of device values read from the database
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function compares the network_element of the /device/num with the
 * network element of a /device/sim for this service instance (if this service
 * has a SIM device).  The constraint is to only allow numbers and SIM devices
 * to share a service instance if they are associated with the same network
 * element.  So an error is returned if a SIM device is found for this service
 * but the network_element value does not match the number's value.
 ******************************************************************************/
static void
fm_num_pol_check_network_element(
	pcm_context_t		*ctxp,
	poid_t			*i_poidp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t 		*ebufp)
{
	pin_cookie_t		cookie = NULL;
	pin_flist_t		*si_flistp = NULL;
	pin_flist_t		*so_flistp = NULL;
	pin_flist_t		*sub_flistp = NULL;
	pin_flist_t		*sub_sub_flistp = NULL;
	pin_flist_t		*results_flistp = NULL;
	pin_flist_t     	*srch_inp_flistp = NULL; /* Search inp Flist */
	pin_flist_t     	*srch_sub_flistp = NULL; /* Search sub Flist */
	pin_flist_t     	*srch_out_flistp = NULL; /* Search out  Flist */
	poid_t			*pdp = NULL; /* Routing poid */
	char			*i_net_elem = NULL;
	char			*r_net_elem = NULL;
	char			*template = NULL;
	int64			database = 0;
	int32			s_flag = 0;
	int32			elem_id = 0;
	int32			rec_count = 0;
	void			*vp = NULL;
	static int32 		sim_device_exists = 0; /* To hold the sim device existance */ 
	 /*  As the sim_device_exists is  used as Read only type of object */
        /* it  will not give thread problem */

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	database = PIN_POID_GET_DB(i_poidp);

	if ( sim_device_exists == 0) {
		/*
		 * Create the Service Flist to Read the /device/sim
		 */
		srch_inp_flistp = PIN_FLIST_CREATE ( ebufp );

		pdp = PIN_POID_CREATE ( database, "/dd/objects", 0, ebufp );
		PIN_FLIST_FLD_PUT ( srch_inp_flistp, PIN_FLD_POID, pdp, ebufp );

		srch_sub_flistp = PIN_FLIST_ELEM_ADD (srch_inp_flistp, PIN_FLD_OBJ_DESC, 0, ebufp );
		PIN_FLIST_FLD_SET (srch_sub_flistp, PIN_FLD_NAME, (void *)"/device/sim", ebufp);

		PCM_OP (ctxp, PCM_OP_GET_DD, 0, srch_inp_flistp, 
				&srch_out_flistp, ebufp);

		if (PIN_ERR_IS_ERR (ebufp)) {
			PIN_ERR_LOG_EBUF ( PIN_ERR_LEVEL_ERROR,
				" fm_num_pol_check_network_element: "
				" error in PCM_OP_GET_DD", ebufp );
			 goto cleanup;
		}
		
		rec_count = PIN_FLIST_ELEM_COUNT( srch_out_flistp, PIN_FLD_OBJ_DESC, ebufp);

		if (rec_count <= 0) {
			/*
			 * /device/sim object Doesn't exist comparision of 
			 * PIN_FLD_NETWORK_ELEMENT is not required
			 */
			PIN_ERR_LOG_MSG ( PIN_ERR_LEVEL_DEBUG,
					 " /device/sim Object doesn't exist "
					 " PIN_FLD_NETWORK_ELEMENT comparision is not required");
			sim_device_exists = 1;
			goto cleanup;
		
		}
		sim_device_exists = 2;
	}

	if ( sim_device_exists == 1) {
		PIN_ERR_LOG_MSG ( PIN_ERR_LEVEL_DEBUG,
				 " /device/sim Object doesn't exist "
				 " PIN_FLD_NETWORK_ELEMENT comparision is not required");
		goto cleanup;
	}
	/*
	 * Get the input network element for comparing later.
	 */
	sub_flistp = (pin_flist_t *)PIN_FLIST_SUBSTR_GET(i_flistp,
		PIN_FLD_DEVICE_NUM, 0, ebufp);

	i_net_elem = (char *)PIN_FLIST_FLD_GET(sub_flistp,
		PIN_FLD_NETWORK_ELEMENT, 0, ebufp);

	/*
	 * Search for all /device/sim objects which are associated to the
	 * inut service object and retrieve their network element value.
	 */
	si_flistp = PIN_FLIST_CREATE(ebufp);


	vp = PIN_POID_CREATE(database, "/search", -1, ebufp);
	PIN_FLIST_FLD_PUT(si_flistp, PIN_FLD_POID, vp, ebufp);

	PIN_FLIST_FLD_SET(si_flistp, PIN_FLD_FLAGS, (void *)&s_flag, ebufp);

	
	
	template = "select X from /device/sim where ( F1 = V1  and F2 = V2 ) ";
	PIN_FLIST_FLD_SET(si_flistp, PIN_FLD_TEMPLATE, (void *)template, ebufp);

	/*
	 * Add the first arg - the SIM device poid (type only)
	 */
	sub_flistp = PIN_FLIST_ELEM_ADD(si_flistp, PIN_FLD_ARGS, 1, ebufp);

	vp = PIN_POID_CREATE(database, PIN_OBJ_TYPE_DEVICE_SIM, -1, ebufp);
	PIN_FLIST_FLD_PUT(sub_flistp, PIN_FLD_POID, vp, ebufp);
	
	/*
	 * Add the second arg - the input service poid
	 */
	sub_flistp = PIN_FLIST_ELEM_ADD(si_flistp, PIN_FLD_ARGS, 2, ebufp);

	sub_sub_flistp = PIN_FLIST_ELEM_ADD(sub_flistp, PIN_FLD_SERVICES, 0,
		 ebufp);
	PIN_FLIST_FLD_SET(sub_sub_flistp, PIN_FLD_SERVICE_OBJ,
		(void *)i_poidp, ebufp);

	/*
	 * Add the selection field - the network element,
	 * and only return a single record for which to check below.
	 */
	sub_flistp = PIN_FLIST_ELEM_ADD(si_flistp, PIN_FLD_RESULTS, 1, ebufp);

	sub_sub_flistp = PIN_FLIST_SUBSTR_ADD(sub_flistp, PIN_FLD_DEVICE_SIM,
		ebufp);
	PIN_FLIST_FLD_SET(sub_sub_flistp, PIN_FLD_NETWORK_ELEMENT, NULL, ebufp);

	PCM_OP(ctxp, PCM_OP_SEARCH, PCM_OPFLG_CACHEABLE, si_flistp, 
		&so_flistp, ebufp);

	/*
	 * The assumption is that only one of the SIM devices needs to
	 * be checked because SIM devices only share a common network element
	 * between sevices.
	 */
	if ((results_flistp = (pin_flist_t *)
		PIN_FLIST_ELEM_GET_NEXT(so_flistp, PIN_FLD_RESULTS,
		&elem_id, 1, &cookie, ebufp)) != NULL) {

		sub_flistp = (pin_flist_t *)PIN_FLIST_SUBSTR_GET(
			results_flistp, PIN_FLD_DEVICE_SIM, 0, ebufp);
			
		r_net_elem = (char *)PIN_FLIST_FLD_GET(sub_flistp,
			PIN_FLD_NETWORK_ELEMENT, 0, ebufp);

		if (	i_net_elem == NULL ||
			r_net_elem == NULL ||
			strcmp(i_net_elem, r_net_elem) != 0) {

			pin_set_err(ebufp, PIN_ERRLOC_FM,
			   	PIN_ERRCLASS_SYSTEM_DETERMINATE, 
				PIN_ERR_BAD_VALUE, 
			   	PIN_FLD_NETWORK_ELEMENT, 0, 0);

			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				"fm_num_pol_check_network_element "
				"existing SIM device for sevice has "
				"different network_element", ebufp);
		}
	}

	cleanup :
	PIN_FLIST_DESTROY_EX(&si_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&srch_inp_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&srch_out_flistp, NULL);

	return;
}


/*******************************************************************************
 * fm_num_pol_check_brand()
 *
 * Inputs: device brand poid; account poid
 * Outputs: void; ebuf set if errors encountered or brands do not match
 *
 * Description:
 * This function compares the brand of the device to the brand of the account
 * and returns an error if they are not equal.
 ******************************************************************************/
static void
fm_num_pol_check_brand(
	pcm_context_t		*ctxp,
	poid_t			*dev_brand_poidp,
	poid_t			*acct_poidp,
	pin_errbuf_t 		*ebufp)
{
	pin_flist_t		*ri_flistp = NULL;
	pin_flist_t		*ro_flistp = NULL;
	poid_t			*brand_poidp = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Get the brand of the account from the database.
	 */
        ri_flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET(ri_flistp, PIN_FLD_POID, acct_poidp, ebufp);
	PIN_FLIST_FLD_SET(ri_flistp, PIN_FLD_BRAND_OBJ, NULL, ebufp);

	PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, ri_flistp, &ro_flistp, ebufp);

	brand_poidp = (poid_t *)PIN_FLIST_FLD_GET(ro_flistp,
		PIN_FLD_BRAND_OBJ, 0, ebufp);

	if (PIN_POID_COMPARE(dev_brand_poidp, brand_poidp, 0, ebufp) != 0) {

		pin_set_err(ebufp, PIN_ERRLOC_FM, 
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_PERMISSION_DENIED,
			PIN_FLD_BRAND_OBJ, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_num_pol_check_brand non-matching brand "
			"for device and service", ebufp);
	}

	PIN_FLIST_DESTROY_EX(&ri_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&ro_flistp, NULL);

	return;
}



/*******************************************************************
 * Function used to add or delete an MSISDN alias to/from the service 
 * alias list that are passed into the i_flist
 * input:
 *  i_flist: an flist, where we get services poid or account poid 
 *           from the PIN_FLD_SERVICES array
 *           An assumption is that only the services of one account 
 *           are are listed here 
 *  is_assct : This flag indicates whether this function is called from
 * 	     associate or disassociate. (TRUE/FALSE)
 *  alias :  alias to add or delete.
 *
 * output:
 *  ebuf: set in case of error
 *******************************************************************/
static void
fm_num_pol_alias_to_services(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        char                    *alias,
        int32                   is_assct,
        pin_errbuf_t            *ebufp) 
{
        pin_flist_t             *s_flistp = NULL;      /* set flist     */
        pin_flist_t             *log_flistp = NULL;    /* login flist   */
        pin_flist_t             *al_flistp = NULL;     /* alias flist   */
        pin_flist_t             *r_flistp = NULL;      /* return flist  */
        pin_flist_t             *svc_flistp = NULL;    /* service list flist*/
        pin_flist_t             *alias_flistp = NULL;    /* alias list flist*/
        pin_flist_t             *alias_in_flistp = NULL;    /* alias list flist*/
        poid_t                  *svc_poid = NULL;      /* service poid */
        poid_t                  *acct_poid = NULL;     /* account poid */
        int32                   elem_id = 0;    /* array element        */
        int32                   elem_id1 = 0;    /* array element        */
        int32                   max_elem_id = -1;    /* count         */
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
                        "fm_num_pol_alias_to_services input flist ", i_flistp);

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

		PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_PROGRAM_NAME, FILE_LOGNAME,
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
                PIN_FLIST_DESTROY_EX(&alias_in_flistp, NULL);
                PIN_FLIST_DESTROY_EX(&alias_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
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
fm_num_pol_check_service_status(
	pcm_context_t		*ctxp,
	poid_t			*service_poidp,
	pin_errbuf_t 		*ebufp) {

	pin_flist_t             *service_inp_flistp = NULL;
	pin_flist_t             *service_out_flistp = NULL;
	pin_status_t		*service_statep = NULL;

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
			"fm_num_pol_check_service_status PCM_OP_READ_FLDS error", ebufp);

		goto cleanup;
	}

	service_statep = (pin_status_t *) PIN_FLIST_FLD_GET(service_out_flistp, 
			PIN_FLD_STATUS, 0, ebufp);


	if (service_statep == NULL) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
			"fm_num_pol_check_service_status service status is null", ebufp);

		goto cleanup;
	}

	if (*service_statep == PIN_STATUS_CLOSED) {
		pin_set_err(ebufp, PIN_ERRLOC_FM, 
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_VALUE, PIN_FLD_STATUS, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
			"fm_num_pol_check_service_status Service status is closed ", ebufp);

		goto cleanup;
	}

	cleanup:
		PIN_FLIST_DESTROY_EX(&service_inp_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&service_out_flistp, NULL);

	return;

}
