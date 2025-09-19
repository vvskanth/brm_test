/*******************************************************************
 *
 * Copyright (c) 2001, 2024 Oracle.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

/*******************************************************************
 * This file contains the PCM_OP_PROV_POL_UPDATE_SVC_ORDER opcode. 
 *
 *******************************************************************/

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_prov_pol_update.c:PortalBase7.3.1Int:1:2007-Sep-03 03:02:16 %";
#endif

#include "pcm.h"
#include "cm_fm.h"
#include "ops/prov.h"
#include "pin_prov_wireless.h"
#include "pinlog.h"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void op_prov_pol_update_svc_order(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);
static void fm_prov_pol_update_svc_order(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	**r_flistpp,
	int32		flags,
	pin_errbuf_t	*ebufp);
static void fm_prov_pol_transform_gsm_svc_order(
	pcm_context_t       *ctx, 
	poid_t              *poidp, 
	pin_flist_t         *ei_flistp,
	pin_flist_t         **new_ei_flistpp,
	pin_errbuf_t        *ebufp);
static void fm_prov_pol_gsm_get_param_info(
	pcm_context_t   *ctxp,
	pin_flist_t     *so_info_flistp,
	pin_flist_t     *ei_flistp,
	pin_flist_t     **new_param_info_flistpp,
	pin_errbuf_t    *ebufp);

static void fm_prov_pol_transform_telco_svc_order(
	pcm_context_t       *ctx, 
	poid_t              *poidp, 
	pin_flist_t         *ei_flistp,
	pin_flist_t         **new_ei_flistpp,
	pin_errbuf_t        *ebufp);
static void fm_prov_pol_telco_get_param_info(
	pcm_context_t   *ctxp,
	pin_flist_t     *so_info_flistp,
	pin_flist_t     *ei_flistp,
	pin_flist_t     **new_param_info_flistpp,
	pin_errbuf_t    *ebufp);
/*******************************************************************
 * Main routine for the PCM_OP_PROV_UPDATE_SVC_ORDER operation.
 *******************************************************************/
void
op_prov_pol_update_svc_order(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t           *ctxp = connp->dm_ctx;
	pin_flist_t		*r_flistp = NULL;

	/***********************************************************
	 * Null our results until we have some.
	 ***********************************************************/
	*o_flistpp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_PROV_POL_UPDATE_SVC_ORDER) {
		pinlog(__FILE__, __LINE__, LOG_FLAG_ERROR,
			"bad opcode (%d) in op_prov_pol_update_svc_order", opcode);
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		return;
	}

	/***********************************************************
	 * Debug: What did we get?
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_prov_pol_update_svc_order input flist", i_flistp);

	/***********************************************************
	 * Call function that does the real work.
	 ***********************************************************/
	fm_prov_pol_update_svc_order(ctxp, i_flistp, &r_flistp, flags, ebufp);
	

	/***********************************************************
	 * Results.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {

		/***************************************************
		 * Log something and return nothing.
		 ***************************************************/
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_prov_pol_update_svc_order error", ebufp);
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		*o_flistpp = (pin_flist_t *)NULL;

	} else {

		/***************************************************
		 * Point the real return flist to the right thing.
		 ***************************************************/
		PIN_ERR_CLEAR_ERR(ebufp);
		*o_flistpp = r_flistp;

		/***************************************************
		 * Debug: What we're sending back.
		 ***************************************************/
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_prov_pol_update_svc_order return flist", r_flistp);

	}

}

/*******************************************************************
 * fm_prov_pol_update_svc_order():
 *
 * Transforms the extended_info passed in, as needed for
 * particular types of service orders.
 *
 *******************************************************************/
static void
fm_prov_pol_update_svc_order(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	**r_flistpp,
	int32		flags,
	pin_errbuf_t	*ebufp)
{
	poid_t		*poidp = NULL;
	char		*svc_order_type = NULL;
	pin_flist_t	*ei_flistp = NULL;
	pin_flist_t	*new_ei_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);
	
	*r_flistpp = PIN_FLIST_CREATE(ebufp);

	/*
	 * Copy PIN_FLD_POID from input to return flist.
	 */
	poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, (void *)poidp, ebufp);

	

	/*
	 * Get the extended_info passed in.  Note: this opcode is not
	 * called if there's no extended info.
	 */
	ei_flistp = PIN_FLIST_SUBSTR_GET(i_flistp,
			PIN_FLD_EXTENDED_INFO, 0, ebufp);
	if (PIN_ERR_IS_ERR(ebufp)) {
		goto done;
		/********/
	}

	/*
	 * Get the service order type.
	 */
	svc_order_type = (char *) PIN_POID_GET_TYPE(poidp);

	/*
	 * Transform the extended info.  Specific transformation depends
	 * on the service order type.
	 */

	if (svc_order_type != NULL &&
	    (strcmp(svc_order_type, PIN_OBJ_TYPE_EVENT_GSM_SVC_ORDER) == 0)) {
		
		fm_prov_pol_transform_gsm_svc_order(ctxp, poidp, ei_flistp, 
			&new_ei_flistp, ebufp);
		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Error fm_prov_transform_gsm_svc_order", ebufp);
			goto done;
		}
	
	} 
	else if (svc_order_type != NULL &&
	    (strstr(svc_order_type, "/event/provisioning/service_order/telco") != NULL)) {
		
		fm_prov_pol_transform_telco_svc_order(ctxp, poidp, ei_flistp, 
			&new_ei_flistp, ebufp);
		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Error fm_prov_transform_telco_svc_order", ebufp);
			goto done;
		}
	
	} else {

		/*
	 	* Default is to just copy the extended info passed in
	 	* (no transformation).
	 	*/
		new_ei_flistp = PIN_FLIST_COPY(ei_flistp, ebufp);
	}

	/*
	 * Put the transformed extended info onto the return flist.
	 */
	PIN_FLIST_SUBSTR_PUT(*r_flistpp, new_ei_flistp,
			PIN_FLD_EXTENDED_INFO, ebufp);

done:
	/* Error? */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_prov_pol_update_svc_order", ebufp);
	}
}


/*******************************************************************************
 * fm_prov_pol_transform_gsm_svc_order():
 *
 * Transforms the extended_info passed in for gsm service orders
 *
 ******************************************************************************/

static void
fm_prov_pol_transform_gsm_svc_order(
	pcm_context_t       *ctxp, 
	poid_t              *so_poidp, 
	pin_flist_t         *ei_flistp,
	pin_flist_t         **new_ei_flistpp,
	pin_errbuf_t        *ebufp)
{
	pin_flist_t         *so_flistp = NULL;
	pin_flist_t         *s_flistp = NULL;
	pin_flist_t         *svc_ord_flistp = NULL;
	pin_flist_t         *param_flistp = NULL;
	int                 elem_id = 0;
	int                 count = 0;
	pin_flist_t         *so_info_flistp = NULL;
	pin_cookie_t        cookie = NULL;
	poid_t              *prov_objp = NULL;
	char                *actionp = NULL;
	char                *namep = NULL;
	pin_flist_t         *new_param_info_flistp = NULL;
	pin_flist_t         *new_so_info_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/******/
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	s_flistp = PIN_FLIST_CREATE (ebufp);
	svc_ord_flistp = PIN_FLIST_CREATE (ebufp);
	param_flistp = PIN_FLIST_CREATE (ebufp);

	/**********************************************************************
	 * Create an flist to read the following fields in GSM Service 
	 * Order Event object using the service order poid
	 *
	 * PIN_FLD_POID           /event/provisioning/service_order/gsm
	 * PIN_FLD_SERVICE_ORDER_INFO  ARRAY [*] allocated 20, used 4
         *     PIN_FLD_POID           POID [0] NULL
         *     PIN_FLD_ACTION          STR [0] NULL
	 *     PIN_FLD_NAME            STR [0] NULL
	 *     PIN_FLD_PARAMS          ARRAY [*]
	 *         PIN_FLD_NAME        STR [0] NULL 
	 *         PIN_FLD_ACTION      STR [0] NULL 
	 **********************************************************************/
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_POID, (void *)so_poidp, ebufp);
	PIN_FLIST_FLD_SET(svc_ord_flistp, PIN_FLD_POID, NULL, ebufp);
	PIN_FLIST_FLD_SET(svc_ord_flistp, PIN_FLD_ACTION, NULL, ebufp);
	PIN_FLIST_FLD_SET(svc_ord_flistp, PIN_FLD_NAME, NULL, ebufp);
	PIN_FLIST_FLD_SET(param_flistp, PIN_FLD_NAME, NULL, ebufp);
	PIN_FLIST_FLD_SET(param_flistp, PIN_FLD_ACTION, NULL, ebufp);

	PIN_FLIST_ELEM_PUT(svc_ord_flistp, param_flistp, PIN_FLD_PARAMS, 
		PIN_ELEMID_ANY, ebufp);

	PIN_FLIST_ELEM_PUT(s_flistp, svc_ord_flistp, 
		PIN_FLD_SERVICE_ORDER_INFO, PIN_ELEMID_ANY, ebufp);
	/**********************************************************************
	 * Read the Service Order Object. 
	 *********************************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "Read input flist ", s_flistp);
	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, s_flistp, 
		&so_flistp, ebufp);
	PIN_FLIST_DESTROY_EX(&s_flistp, NULL);	
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"Error: Reading the Service Order Event Object", ebufp);
		 PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
		 PIN_FLIST_DESTROY_EX(&svc_ord_flistp, NULL);
		 PIN_FLIST_DESTROY_EX(&param_flistp, NULL);
		return;
		/******/
	}
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
		"Fields Read from the Service Order Event Object", so_flistp);

	*new_ei_flistpp  = PIN_FLIST_CREATE(ebufp); 
	/********************************************************************* 
	 * For Each Service order info element in the service order object
	 * get the provisioning object poid, action, and list of parameters
	 * that need status update. If the action is not ignore or if there
	 * parameters that need update to the status in the service order 
	 * object, append the response info to new_ei_flistpp
	 ********************************************************************/  
	while(( so_info_flistp = PIN_FLIST_ELEM_GET_NEXT(so_flistp, 
		PIN_FLD_SERVICE_ORDER_INFO, &elem_id, 1, &cookie, ebufp))
		!= NULL ) {

		/**************************************************************
		 *	Get the poid of the prov object to be updated 
		 *************************************************************/
		prov_objp = (poid_t *)PIN_FLIST_FLD_GET(so_info_flistp, 
			PIN_FLD_POID, 0, ebufp);
		actionp = (char *)PIN_FLIST_FLD_GET(so_info_flistp, 
			PIN_FLD_ACTION, 0, ebufp);
		namep = (char *)PIN_FLIST_FLD_GET(so_info_flistp, 
			PIN_FLD_NAME, 1, ebufp);
		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Error: Getting Mandatory fields ", ebufp);
			PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
			return;
			/******/
		}

		fm_prov_pol_gsm_get_param_info(ctxp, so_info_flistp, ei_flistp, 
			&new_param_info_flistp, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Error: In fm_prov_gsm_get_param_info", ebufp);
			PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
			PIN_FLIST_DESTROY_EX(&new_param_info_flistp, NULL);
			return;
			/******/
		}

		count = PIN_FLIST_ELEM_COUNT (new_param_info_flistp, 
			PIN_FLD_PARAMS, ebufp);

		if ( ( count > 0 ) ||
			(strcmp ( actionp, PIN_ACTION_IGNORE) != 0) ) {

			new_so_info_flistp = PIN_FLIST_ELEM_ADD(
				*new_ei_flistpp, PIN_FLD_SERVICE_ORDER_INFO,
				 elem_id, ebufp);

			PIN_FLIST_FLD_SET(new_so_info_flistp, PIN_FLD_POID,
				(void *)prov_objp, ebufp);
			PIN_FLIST_FLD_SET(new_so_info_flistp, PIN_FLD_ACTION,
				(void *)actionp, ebufp);
			if ( namep != NULL) {
				PIN_FLIST_FLD_SET(new_so_info_flistp,
					PIN_FLD_NAME, (void *)namep, ebufp);
			}

			if (count > 0 ) {
				PIN_FLIST_CONCAT(new_so_info_flistp, 
					new_param_info_flistp, ebufp);
			}
		}
		PIN_FLIST_DESTROY_EX(&new_param_info_flistp, NULL);
	}
	PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
	return;
	/******/
}

/*******************************************************************************
 * fm_prov_pol_transform_telco_svc_order():
 *
 * Transforms the extended_info passed in for gsm service orders
 *
 ******************************************************************************/

static void
fm_prov_pol_transform_telco_svc_order(
	pcm_context_t       *ctxp, 
	poid_t              *so_poidp, 
	pin_flist_t         *ei_flistp,
	pin_flist_t         **new_ei_flistpp,
	pin_errbuf_t        *ebufp)
{
	pin_flist_t         *so_flistp = NULL;
	pin_flist_t         *s_flistp = NULL;
	pin_flist_t         *svc_ord_flistp = NULL;
	pin_flist_t         *param_flistp = NULL;
	int                 elem_id = 0;
	int                 count = 0;
	pin_flist_t         *so_info_flistp = NULL;
	pin_cookie_t        cookie = NULL;
	poid_t              *prov_objp = NULL;
	char                *actionp = NULL;
	char                *namep = NULL;
	pin_flist_t         *new_param_info_flistp = NULL;
	pin_flist_t         *new_so_info_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/******/
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	s_flistp = PIN_FLIST_CREATE (ebufp);
	svc_ord_flistp = PIN_FLIST_CREATE (ebufp);
	param_flistp = PIN_FLIST_CREATE (ebufp);

	/**********************************************************************
	 * Create an flist to read the following fields in TELCO Service 
	 * Order Event object using the service order poid
	 *
	 * PIN_FLD_POID           /event/provisioning/service_order/telco
	 * PIN_FLD_SERVICE_ORDER_INFO  ARRAY [*] allocated 20, used 4
         *     PIN_FLD_POID           POID [0] NULL
         *     PIN_FLD_ACTION          STR [0] NULL
	 *     PIN_FLD_NAME            STR [0] NULL
	 *     PIN_FLD_PARAMS          ARRAY [*]
	 *         PIN_FLD_NAME        STR [0] NULL 
	 *         PIN_FLD_ACTION      STR [0] NULL 
	 **********************************************************************/
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_POID, (void *)so_poidp, ebufp);
	PIN_FLIST_FLD_SET(svc_ord_flistp, PIN_FLD_POID, NULL, ebufp);
	PIN_FLIST_FLD_SET(svc_ord_flistp, PIN_FLD_ACTION, NULL, ebufp);
	PIN_FLIST_FLD_SET(svc_ord_flistp, PIN_FLD_NAME, NULL, ebufp);
	PIN_FLIST_FLD_SET(param_flistp, PIN_FLD_NAME, NULL, ebufp);
	PIN_FLIST_FLD_SET(param_flistp, PIN_FLD_ACTION, NULL, ebufp);

	PIN_FLIST_ELEM_PUT(svc_ord_flistp, param_flistp, PIN_FLD_PARAMS, 
		PIN_ELEMID_ANY, ebufp);

	PIN_FLIST_ELEM_PUT(s_flistp, svc_ord_flistp, 
		PIN_FLD_SERVICE_ORDER_INFO, PIN_ELEMID_ANY, ebufp);
	/**********************************************************************
	 * Read the Service Order Object. 
	 *********************************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "Read input flist ", s_flistp);
	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, s_flistp, 
		&so_flistp, ebufp);
	PIN_FLIST_DESTROY_EX(&s_flistp, NULL);	
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"Error: Reading the Service Order Event Object", ebufp);
		 PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
		 PIN_FLIST_DESTROY_EX(&svc_ord_flistp, NULL);
		 PIN_FLIST_DESTROY_EX(&param_flistp, NULL);
		return;
		/******/
	}
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
		"Fields Read from the Service Order Event Object", so_flistp);

	*new_ei_flistpp  = PIN_FLIST_CREATE(ebufp); 
	/********************************************************************* 
	 * For Each Service order info element in the service order object
	 * get the provisioning object poid, action, and list of parameters
	 * that need status update. If the action is not ignore or if there
	 * parameters that need update to the status in the service order 
	 * object, append the response info to new_ei_flistpp
	 ********************************************************************/  
	while(( so_info_flistp = PIN_FLIST_ELEM_GET_NEXT(so_flistp, 
		PIN_FLD_SERVICE_ORDER_INFO, &elem_id, 1, &cookie, ebufp))
		!= NULL ) {

		/**************************************************************
		 *	Get the poid of the prov object to be updated 
		 *************************************************************/
		prov_objp = (poid_t *)PIN_FLIST_FLD_GET(so_info_flistp, 
			PIN_FLD_POID, 0, ebufp);
		actionp = (char *)PIN_FLIST_FLD_GET(so_info_flistp, 
			PIN_FLD_ACTION, 0, ebufp);
		namep = (char *)PIN_FLIST_FLD_GET(so_info_flistp, 
			PIN_FLD_NAME, 1, ebufp);
		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Error: Getting Mandatory fields ", ebufp);
			PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
			return;
			/******/
		}

		fm_prov_pol_telco_get_param_info(ctxp, so_info_flistp, ei_flistp, 
			&new_param_info_flistp, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Error: In fm_prov_telco_get_param_info", ebufp);
			PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
			PIN_FLIST_DESTROY_EX(&new_param_info_flistp, NULL);
			return;
			/******/
		}

		count = PIN_FLIST_ELEM_COUNT (new_param_info_flistp, 
			PIN_FLD_PARAMS, ebufp);

		if ( ( count > 0 ) ||
			(strcmp ( actionp, PIN_ACTION_IGNORE) != 0) ) {

			new_so_info_flistp = PIN_FLIST_ELEM_ADD(
				*new_ei_flistpp, PIN_FLD_SERVICE_ORDER_INFO,
				 elem_id, ebufp);

			PIN_FLIST_FLD_SET(new_so_info_flistp, PIN_FLD_POID,
				(void *)prov_objp, ebufp);
			PIN_FLIST_FLD_SET(new_so_info_flistp, PIN_FLD_ACTION,
				(void *)actionp, ebufp);
			if ( namep != NULL) {
				PIN_FLIST_FLD_SET(new_so_info_flistp,
					PIN_FLD_NAME, (void *)namep, ebufp);
			}

			if (count > 0 ) {
				PIN_FLIST_CONCAT(new_so_info_flistp, 
					new_param_info_flistp, ebufp);
			}
		}
		PIN_FLIST_DESTROY_EX(&new_param_info_flistp, NULL);
	}
	PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
	return;
	/******/
}

/******************************************************************************
 * fm_prov_pol_gsm_get_param_info():
 *
 * Get the parameters that needs update  to the status  based on the response
 * from the provisioning platform. 
 *
 ******************************************************************************/
static void 
fm_prov_pol_gsm_get_param_info(
	pcm_context_t   *ctxp,
	pin_flist_t     *so_info_flistp,
	pin_flist_t     *ei_flistp,
	pin_flist_t     **new_param_info_flistpp,
	pin_errbuf_t    *ebufp)
{
	pin_flist_t     *params_flistp = NULL;
	int             elem_id = 0;
	pin_cookie_t    cookie = NULL;
	char            *param_namep = NULL;
	char            *param_actionp = NULL;
	pin_flist_t     *resp_params_flistp = NULL;
	int             elem_id1 = 0;
	pin_cookie_t    cookie1 = NULL;
	char            *resp_namep = NULL;
	char            msg[80];
	int             param_found = 0;

	*new_param_info_flistpp = PIN_FLIST_CREATE(ebufp);

	/**********************************************************************
	 * For Each Parameter array in the Service Order object in the database
	 * get the param name and action. 
	 *********************************************************************/
	while (( params_flistp = PIN_FLIST_ELEM_GET_NEXT(
		so_info_flistp, PIN_FLD_PARAMS, &elem_id, 1, &cookie, 
		ebufp)) !=  NULL) {

		param_found = 0;
		param_namep = (char *)PIN_FLIST_FLD_GET(params_flistp, 
			PIN_FLD_NAME, 0, ebufp);
		param_actionp = (char *)PIN_FLIST_FLD_GET (params_flistp, 
			PIN_FLD_ACTION, 0, ebufp);
		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Err: Getting Mandatory param flds in SO", 
				ebufp);
			return;
			/******/
		}
		/**************************************************************
		 * If the action is ignore, the param need no update in DB     
		 *************************************************************/
		if ( strcmp(param_actionp, PIN_ACTION_IGNORE) == 0)  {
			param_found = 1;
		}
		/*************************************************************
		 * If the param action is not ignore, Loop the parameters 
		 * received from Provisioning System, and append the response
		 * parameter values to new_param_info_flistpp to update the
		 * the service order event with status recd from provisioning
		 * system. 
		 ************************************************************/
		elem_id1 = 0;
		cookie1 = NULL;
		while ( ( resp_params_flistp = PIN_FLIST_ELEM_GET_NEXT( 
			ei_flistp, PIN_FLD_PARAMS, &elem_id1, 1, &cookie1, 
			ebufp) ) != NULL && param_found == 0) {

			resp_namep = (char *)PIN_FLIST_FLD_GET( 
				resp_params_flistp, PIN_FLD_NAME, 0, ebufp);
			if (PIN_ERR_IS_ERR(ebufp)) {
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"Error: Getting Response Param Name", 
					ebufp);
				return;
				/******/
			}
			if (strcmp(param_namep, resp_namep) == 0) {
				param_found = 1;
				PIN_FLIST_ELEM_SET (*new_param_info_flistpp, 
					resp_params_flistp, PIN_FLD_PARAMS, 
					elem_id, ebufp);
			}
		}
		/*************************************************************
		 * If the parameter in the Service Order object with action
		 * other than ignore, is  not found in the response, return 
		 * with an error
		 ************************************************************/
		if ( param_found == 0) {
			pin_snprintf(msg, sizeof(msg), "Return: No Respones recd for Param : %s",
			  	param_namep);
			PIN_ERR_LOG_MSG( PIN_ERR_LEVEL_DEBUG, msg);
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_SYSTEM_DETERMINATE, 
				PIN_ERR_BAD_ARG, PIN_FLD_PARAMS, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				" Error : Response missing for a Parameter",
				 ebufp);
			return;
			/*******/
		}
	}
	return;
	/*******/
}
/******************************************************************************
 * fm_prov_pol_telco_get_param_info():
 *
 * Get the parameters that needs update  to the status  based on the response
 * from the provisioning platform. 
 *
 ******************************************************************************/
static void 
fm_prov_pol_telco_get_param_info(
	pcm_context_t   *ctxp,
	pin_flist_t     *so_info_flistp,
	pin_flist_t     *ei_flistp,
	pin_flist_t     **new_param_info_flistpp,
	pin_errbuf_t    *ebufp)
{
	pin_flist_t     *params_flistp = NULL;
	int             elem_id = 0;
	pin_cookie_t    cookie = NULL;
	char            *param_namep = NULL;
	char            *param_actionp = NULL;
	pin_flist_t     *resp_params_flistp = NULL;
	int             elem_id1 = 0;
	pin_cookie_t    cookie1 = NULL;
	char            *resp_namep = NULL;
	char            msg[80];
	int             param_found = 0;

	*new_param_info_flistpp = PIN_FLIST_CREATE(ebufp);

	/**********************************************************************
	 * For Each Parameter array in the Service Order object in the database
	 * get the param name and action. 
	 *********************************************************************/
	while (( params_flistp = PIN_FLIST_ELEM_GET_NEXT(
		so_info_flistp, PIN_FLD_PARAMS, &elem_id, 1, &cookie, 
		ebufp)) !=  NULL) {

		param_found = 0;
		param_namep = (char *)PIN_FLIST_FLD_GET(params_flistp, 
			PIN_FLD_NAME, 0, ebufp);
		param_actionp = (char *)PIN_FLIST_FLD_GET (params_flistp, 
			PIN_FLD_ACTION, 0, ebufp);
		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Err: Getting Mandatory param flds in SO", 
				ebufp);
			return;
			/******/
		}
		/**************************************************************
		 * If the action is ignore, the param need no update in DB     
		 *************************************************************/
		if ( strcmp(param_actionp, PIN_ACTION_IGNORE) == 0)  {
			param_found = 1;
		}
		/*************************************************************
		 * If the param action is not ignore, Loop the parameters 
		 * received from Provisioning System, and append the response
		 * parameter values to new_param_info_flistpp to update the
		 * the service order event with status recd from provisioning
		 * system. 
		 ************************************************************/
		elem_id1 = 0;
		cookie1 = NULL;
		while ( ( resp_params_flistp = PIN_FLIST_ELEM_GET_NEXT( 
			ei_flistp, PIN_FLD_PARAMS, &elem_id1, 1, &cookie1, 
			ebufp) ) != NULL && param_found == 0) {

			resp_namep = (char *)PIN_FLIST_FLD_GET( 
				resp_params_flistp, PIN_FLD_NAME, 0, ebufp);
			if (PIN_ERR_IS_ERR(ebufp)) {
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"Error: Getting Response Param Name", 
					ebufp);
				return;
				/******/
			}
			if (strcmp(param_namep, resp_namep) == 0) {
				param_found = 1;
				PIN_FLIST_ELEM_SET (*new_param_info_flistpp, 
					resp_params_flistp, PIN_FLD_PARAMS, 
					elem_id, ebufp);
			}
		}
		/*************************************************************
		 * If the parameter in the Service Order object with action
		 * other than ignore, is  not found in the response, return 
		 * with an error
		 ************************************************************/
		if ( param_found == 0) {
			pin_snprintf(msg, sizeof(msg), "Return: No Respones recd for Param : %s",
				param_namep);
			PIN_ERR_LOG_MSG( PIN_ERR_LEVEL_DEBUG, msg);
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_SYSTEM_DETERMINATE, 
				PIN_ERR_BAD_ARG, PIN_FLD_PARAMS, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				" Error : Response missing for a Parameter",
				 ebufp);
			return;
			/*******/
		}
	}
	return;
	/*******/
}
