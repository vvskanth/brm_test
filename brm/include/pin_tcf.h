/************************************************************************
 *  @(#)$Id: pin_tcf.h /cgbubrm_mainbrm.portalbase/1 2016/12/11 21:52:47 thakuppu Exp $
 *
* Copyright (c) 2004, 2022, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation 
 * or its licensors and may be used, reproduced, stored or transmitted 
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 **********************************************************************/

#ifndef _PIN_TCF_H
#define _PIN_TCF_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DEBUG
#define FM_TCF_DEBUG
#endif
#define PIN_TCF_MODULE_NAME                "fm_tcf"
#define PIN_TCF_MODULE_ARG_SIMULATE_AGENT  "simulate_agent"
#define PIN_TCF_MODULE_ARG_PROV_DB         "prov_db"
#define PIN_TCF_SIMULATE_AGENT_ON          1
#define PIN_TCF_SIMULATE_AGENT_OFF         0
#define PIN_TCF_MODULE_ARG_PROVISIONING_ENABLE "provisioning_enabled"

#define PIN_TCF_AAA_MODULE_NAME		   		  "fm_tcf_aaa"
#define PIN_TCF_AAA_MODULE_ARG_WAIT_FOR_ALL_INTERIM_STOP_REQUEST "wait_for_all_interim_stop_request"

#define READ_VALUE_FROM_CONFIG_FILE        1
#define DONT_READ_FROM_CONFIG_FILE         0
#define MAX_STRING_LENGTH		   540

#define ELEM_NOT_FOUND  -1
#define ELEM_FOUND      1

#define PIN_TCF_CONFIRMED_MODE	1
#define PIN_TCF_QUEUED_MODE	0

#define DEAL_CHANGE_TRANS_FLIST "deal change"

/*
 * pin_cust.h being included for the feature_service_era_status below.
 */
#include "pin_cust.h"
#include "cm_fm.h"

#define PIN_PROV_MSISDN_TAG                  "MSISDN"
#define PIN_PROV_IMSI_TAG                    "IMSI"
#define PIN_PROV_DEVICE_SIM_TAG              "SIM"
#define PIN_PROV_PIN1_TAG                    "PIN1"
#define PIN_PROV_PIN2_TAG                    "PIN2"
#define PIN_PROV_PUK1_TAG                    "PUK1"
#define PIN_PROV_PUK2_TAG                    "PUK2"
#define PIN_PROV_KI_TAG                      "KI"
#define PIN_PROV_ADM1_TAG                    "ADM1"
#define PIN_PROV_NETWORK_ELEMENT_TAG         "NET"

#define PIN_ACCT_ERA_FRIENDS_FAMILY 	"FRIENDS_FAMILY"
#define PIN_ACCT_ERA_CLOSED_USER_GROUP 	"CLOSEDUSERGROUP"

/* Service Order Status  */
typedef enum  pin_prov_tcf_so_status{

	PIN_SVC_ORDER_STATUS_SUCCESS             = 0,
	PIN_SVC_ORDER_STATUS_FAILED              = 1,
	PIN_SVC_ORDER_STATUS_PROVISIONING        = 2,
	PIN_SVC_ORDER_STATUS_PRE_PROVISIONING    = 3

} pin_prov_tcf_so_status_t;

#define PIN_TELCO_PROV_PROGRAM_NAME  "Telco Provisioning"

/* Service Order Actions     */
#define PIN_SVC_ORDER_ACTION_PRE_PROVISIONING   "P"
#define PIN_SVC_ORDER_ACTION_ACTIVATE           "A"
#define PIN_SVC_ORDER_ACTION_DEACTIVATE         "D"
#define PIN_SVC_ORDER_ACTION_SUSPEND            "S"
#define PIN_SVC_ORDER_ACTION_CHANGE             "C"
#define PIN_SVC_ORDER_ACTION_REACTIVATE         "R"
#define PIN_SVC_ORDER_ACTION_IGNORE             "I"

/*
 * String constants for config class/object.
 */
#define PIN_OBJ_TYPE_TCF_CONFIG                           "/config"
#define PIN_OBJ_TYPE_CONFIG_TELCO                         "/config/telco"
#define PIN_OBJ_TYPE_CONFIG_ACC_ERA                       "/config/account_era"
#define PIN_OBJ_TYPE_CONFIG_TELCO_PROVISIONING            "/config/telco/provisioning"
#define PIN_OBJ_TYPE_CONFIG_TELCO_PROVISIONING_FIELDLIST  "/config/telco/provisioning/fieldlist"
#define PIN_OBJ_TYPE_CONFIG_TELCO_SERVICE_ORDER_STATE     "/config/telco/service_order_state"
#define PIN_OBJ_TYPE_CONFIG_DEVICE_PERMIT_MAP		  "/config/device_permit_map"
#define PIN_OBJ_TYPE_CONFIG_PERMITTED_SERVICE_TYPES	  "/config/service_framework/permitted_service_types"
#define PIN_OBJ_TYPE_EVENT_TELCO_SVC_ORDER      "/event/provisioning/service_order/telco"
#define PIN_OBJ_TYPE_SERVICE_TELCO              "/service/telco"
#define PIN_OBJ_TYPE_PROFILE_SERV_EXTRA         "/profile/serv_extrating"

/*
 * String constants for caching config.
 */
#define TCF_CONFIG_CACHE_TTL       60*60
#define TCF_CONFIG_CACHE_NAME      "fm_tcf_utils_config_cache"


/*
 * Generic Event Name.
 */
#define TCF_EVENT_SESSION_TYPE     "/event/session/telco"
#define TCF_ACTIVITY_EVENT_TYPE    "/event/activity/telco"
#define TCF_ACTIVE_SESSION_TYPE    "/active_session/telco"

/* 
 * Generic opcode processing stage 
 */
#define PIN_TCF_AAA_PREP_INPUT    	  0
#define PIN_TCF_AAA_HANDLE_DUP_SESSIONS   1
#define PIN_TCF_AAA_POST_PROCESS          2
#define PIN_TCF_AAA_SEARCH_SESSION        3
#define PIN_TCF_AAA_ACC_ON_OFF_SEARCH     4
#define PIN_TCF_AAA_TAG_SESSION           5
#define PIN_TCF_AAA_VALIDATE_LIFECYCLE    7
/*
 * Processing stage specifc to Dropped Call
 */
#define PIN_TCF_AAA_MATCH_CONTINUOUS_CALL 6

/*
 * Values for SUCCESS and FAILURE to be returned from
 * accounting opcodes
 */
#define PIN_TCF_AAA_STATUS_FAILURE           PIN_BOOLEAN_FALSE 
#define PIN_TCF_AAA_STATUS_SUCCESS           PIN_BOOLEAN_TRUE 
#define PIN_TCF_AAA_STATUS_NOT_IMPLEMENTED   2 
#define PIN_TCF_AAA_STATUS_ABORT            -1 
#define PIN_TCF_AAA_STATUS_NOT_FOUND         PIN_BOOLEAN_FALSE 
#define PIN_TCF_AAA_STATUS_FOUND             PIN_BOOLEAN_TRUE 
#define PIN_TCF_AAA_FAILOVER_MODE	     PIN_BOOLEAN_FALSE
#define PIN_TCF_AAA_DUPLICATE_SESSION	     PIN_BOOLEAN_FALSE

/*
 * Values for TCF AAA opcodes 
 */
#define PIN_TCF_AAA_NO          PIN_BOOLEAN_FALSE 
#define PIN_TCF_AAA_YES         PIN_BOOLEAN_TRUE 
#define PIN_TCF_AAA_DONT_KNOW  -1 

/*
 * Index name for the Network session correlation id.
 */
#define PIN_TCF_INDEX_NAME_CORRELATION_ID "active_session_correlation_id_i"

/* 
 * typedef to contain the results of 
 * finding the config object 
 */
typedef struct {
	int32 status;
	poid_t          *config_poidp;
} config_info_struct;


typedef enum {
	PIN_ACTION_CANCEL       = 0,
	PIN_ACTION_ADD          = 1,
	PIN_ACTION_MODIFY       = 2
} tcf_provision_status_t;

typedef feature_service_era_status_t tcf_feature_service_era_status_t;

#define NEW_TELCO_SERVICE    0
#define OLD_TELCO_SERVICE    1
#define NO_PROV_REQUIRED    -1
#define PROV_REQUIRED        1
#define SUSPEND_PROV         2

#define SIM_PROV_AVAILABLE	0
#define SIM_PROV_REQUIRED	1
#define NUM_PROV_AVAILABLE	0
#define NUM_PROV_REQUIRED	2

#define PROVISIONING_NOOP         PIN_STATUS_FEA_ERA_PROVISIONING_NOOP
#define PROVISIONING              PIN_STATUS_FEA_ERA_PROVISIONING
#define PROVISIONING_FAILED       PIN_STATUS_FEA_ERA_PROVISIONING_FAILED
#define	ACTIVE                    PIN_STATUS_FEA_ERA_ACTIVE
#define	UNPROVISIONING            PIN_STATUS_FEA_ERA_UNPROVISIONING
#define	UNPROVISIONING_FAILED     PIN_STATUS_FEA_ERA_UNPROVISIONING_FAILED
#define	UNPROVISIONED             PIN_STATUS_FEA_ERA_UNPROVISIONED
#define	SUSPENDING                PIN_STATUS_FEA_ERA_SUSPENDING
#define	SUSPENDING_FAILED         PIN_STATUS_FEA_ERA_SUSPENDING_FAILED
#define	SUSPENDED                 PIN_STATUS_FEA_ERA_SUSPENDED

#define PIN_TELCO_SVC_SO_EVENT_NAME         "TELCO Service Order"
#define PIN_TELCO_PROF_SO_EVENT_NAME        "Profile Service Order"
#define PIN_TELCO_PRE_SIM_SO_EVENT_NAME     "Sim Pre Provisioning Order"
#define PIN_BEARER_SERVICE_TAG              "BEARER_SERVICE"

/***********************************************************************
 * Service Order state types.
 ***********************************************************************/
typedef enum pin_svc_order_state_type {
	PIN_SVC_ORDER_RAW_STATE		=	0,
	PIN_SVC_ORDER_INIT_STATE	=	1,
	PIN_SVC_ORDER_NORMAL_STATE	=	2,
	PIN_SVC_ORDER_TERM_STATE	=	3		
} pin_svc_order_state_type_t;

/*
 * Values for PIN_FLD_STATUS in service order events:
 * represents states in the service order state machine.
 */
#define PIN_TCF_SVC_ORDER_STATE_NEW            1
#define PIN_TCF_SVC_ORDER_STATE_READY          2
#define PIN_TCF_SVC_ORDER_STATE_PROCESSING     3
#define PIN_TCF_SVC_ORDER_STATE_COMPLETED      4
#define PIN_TCF_SVC_ORDER_STATE_FAILED         5
#define PIN_TCF_SVC_ORDER_STATE_CANCELLED      6

/************************************************************************
 * Object Name Strings
 ***********************************************************************/
#define PIN_OBJ_NAME_EVENT_SVC_ORDER_ATTR               "Service Order Attribute Change Event Log"
#define PIN_OBJ_NAME_EVENT_SVC_ORDER_STATE              "Service Order State Change Event Log"
#define PIN_OBJ_NAME_EVENT_SVC_ORDER_STATE_NOTIFY       "Service Order State Notification Event"
#define PIN_OBJ_NAME_EVENT_SVC_ORDER_TRANSITION_NOTIFY  "Service Order State Transition Notification Event" 


/************************************************************************
 * Service Order event type strings
 ***********************************************************************/
#define PIN_OBJ_TYPE_EVENT_SVC_ORDER_ATTRIBUTE          "/event/svc_order/attribute"
#define PIN_OBJ_TYPE_EVENT_SVC_ORDER_STATE              "/event/svc_order/state"
#define PIN_OBJ_TYPE_EVENT_SVC_ORDER_STATE_NOTIFY       "/event/notification/svc_order/state"
#define PIN_OBJ_TYPE_EVENT_SVC_ORDER_TRANSITION_NOTIFY  "/event/notification/svc_order/state/in_transition" 

#define PIN_EVENT_DESCR_SVC_ORDER_ATTRIBUTE             "Service Order Set Attribute"
#define PIN_EVENT_DESCR_SVC_ORDER_STATE                 "Service Order  Set State"

/************************************************************************
 * Called number modify mark
 ***********************************************************************/
#define NUM_MARK_UNDEFINED_NUMBER        0
#define NUM_MARK_NORMAL_NUMBER           0
#define NUM_MARK_SOCIAL_NUMBER           1
#define NUM_MARK_ANONYMIZE_NUMBER        2
#define NUM_MARK_SPECIAL_NUMBER          4
#define NUM_MARK_MODIFIED_NUMBER         8

/************************************************************************
 * Mode of input
 ***********************************************************************/
#define USED_AGGR_MODE          1
#define USED_INCR_MODE          2
#define REQ_AGGR_MODE           4
#define REQ_INCR_MODE           8

/************************************************************************
 * Requested mode
 ***********************************************************************/
#define PIN_TCF_AAA_REQ_MODE_AMOUNT                     1
#define PIN_TCF_AAA_REQ_MODE_DURATION                   2
#define PIN_TCF_AAA_REQ_MODE_VOLUME                     4
#define PIN_TCF_AAA_REQ_MODE_ACTIVITY                   8

/************************************************************************
 * Subsession mode
 ***********************************************************************/
#define PIN_TCF_AAA_SUBSESSION_AGGREGATE_MODE		1
#define PIN_TCF_AAA_SUBSESSION_RATE_IMMEDIATELY_MODE	2
#define PIN_TCF_AAA_SUBSESSION_DEFERRED_RATE_MODE	3

/************************************************************************
 * Session types.
 ***********************************************************************/
#define PIN_TCF_AAA_SESSION_TYPE_NORMAL			0
#define PIN_TCF_AAA_SESSION_TYPE_MASTER			1
#define PIN_TCF_AAA_SESSION_TYPE_SUBSESSION		2

/************************************************************************
 * Duplicate check type.
 ***********************************************************************/
#define PIN_TCF_AAA_DUPLICATE_CHECK_TYPE_ASO		1
#define PIN_TCF_AAA_DUPLICATE_CHECK_TYPE_EVENT		2

/************************************************************************
 * Rating Mode 
 ***********************************************************************/
#define NO_ADJUST        0
#define ADJUST           1

#define NORMAL_STOP_MODE 0
#define DIRECT_DEBIT_MODE 1

#define PIN_TCF_AAA_STOP_FAILED         0
#define PIN_TCF_AAA_STOP_SUCCESS        1
#define PIN_TCF_AAA_INSUFFICIENT_FUND   3
#define PIN_TCF_AAA_NO_FUND             4
#define PIN_TCF_AAA_VALIDATION_FAILED	7

/************************************************************************
 * Dropped Call Macros.
 ***********************************************************************/
#define PIN_TCF_AAA_SORT_DECENDING                       1
#define PIN_TCF_AAA_SORT_ASCENDING                       0
#define PIN_TCF_AAA_CONTINUATION_CALL                    2
#define PIN_TCF_AAA_NORMAL_CALL                          0
#define PIN_TCF_AAA_DROPPED_CALL                         1
#define PIN_TCF_AAA_SAME_NUMBER_CALLED_FLAG              "SAME_CALLED_PARTY"
#define PIN_TCF_AAA_DURATION_BETWEEN_CALLS               "MAX_TIME_TO_CONTINUATION_CALL"
#define PIN_TCF_AAA_NO_OF_INTERMEDIATE_CALLS             "MAX_INTERVENING_CALLS"
#define PIN_TCF_AAA_CONFIG_RANGE_CROSSED                 2
#define PIN_TCF_AAA_NO_PROFILE_FOUND                     PIN_TCF_AAA_CONFIG_RANGE_CROSSED  
#define PIN_TCF_AAA_BILLING_CYCLE_CROSSED                PIN_TCF_AAA_CONFIG_RANGE_CROSSED  
#define PIN_TCF_AAA_TIME_RANGE_CROSSED                   PIN_TCF_AAA_CONFIG_RANGE_CROSSED  
#define PIN_TCF_AAA_INTERMEDIATE_CALLS_RANGE_CROSSED     PIN_TCF_AAA_CONFIG_RANGE_CROSSED  
#define PIN_TCF_AAA_DROPPED_CALL_STR                     "DROPPED_CALL"
#define PIN_TCF_AAA_DROPPED_CALL_SUCCESS                 PIN_BOOLEAN_TRUE 
#define PIN_TCF_AAA_DROPPED_CALL_FAILURE                 PIN_BOOLEAN_FALSE

/************************************************************************
 * Subscriber Life Cycle validation Macros.
 ***********************************************************************/
#define PIN_TCF_AAA_TRANSITION_REQUIRED    2
#define PIN_TCF_AAA_VALIDATION_NOT_REQUIRED    3
/********************************************************************************
 * fm_telco_prov_utils_validate_event 
 *    Based on the event type and the provisioning object decides  if 
 *    the information is needed to create Telco Service order
 *******************************************************************************/
PIN_EXPORT int32
fm_telco_prov_utils_validate_event(         
    pcm_context_t               *ctxp, 
	pin_flist_t                 *i_flistp, 
	poid_t                      **event_objpp, 
	char                        **event_typepp, 
	poid_t                      **prov_objpp, 
	pin_errbuf_t                *ebufp);

/********************************************************************************
 * fm_telco_prov_utils_update_service
 *   Updates the service object with the provisioning status and reads
 *    after the update to check the  result provisioning status.
 *******************************************************************************/
PIN_EXPORT void
fm_telco_prov_utils_update_service(
    pcm_context_t               *ctxp, 
	poid_t                      *acct_objp, 
	poid_t                      *svc_objp, 
	pin_flist_t                 *tcf_upd_flistp, 
	int32                       *tcf_status_valuep, 
	pin_flist_t                 **result_flistpp, 
	pin_errbuf_t                *ebufp);

/********************************************************************************
 * fm_telco_prov_tcf_prov_info 
 *
 *  This does the following functions
 *  1.  Decides the Service Order action based on the Status and Status Flag of
 *      Service object
 *  2.  Create the flist to update the service object if provisioning is
 *      required
 *******************************************************************************/
PIN_EXPORT void
fm_telco_prov_tcf_prov_action(         
	pcm_context_t               *ctxp, 
	int                         svc_state,
	pin_flist_t                 *pre_svc_flistp, 
	pin_flist_t                 *post_svc_flistp, 
	pin_flist_t                 *tcf_upd_flistp, 
	char                        *tcf_actionp, 
	cm_nap_connection_t         *connp,
	int32						tcf_actionp_len,
	pin_errbuf_t                *ebufp);

/******************************************************************************
 * fm_telco_prov_tcf_prov_info
 *  This function is called when complete service information is required
 *      to be sent in the Service Order and assumes the provisioning flags
 *      are set.
 *  This does the following functions
 *  1.  Gets all the features that are set to provisioning Required
 *  2.  Ignore redundant features
 *  3.  Create the provisioning info array for the Telco service with 
 *      Features
 *
 *****************************************************************************/
PIN_EXPORT int32 
fm_telco_prov_tcf_prov_info(         
	pcm_context_t               *ctxp, 
	pin_flist_t                 *post_svc_flistp, 
	pin_flist_t                 *svc_order_arrp, 
	int32                       *so_elem_idp,
	char                        *tcf_actionp, 
	pin_errbuf_t                *ebufp);
/*******************************************************************************
 * fm_telco_prov_fields_prov_info
 *
 *  This does the following functions
 *  1.  Decides the Service Order action based on the Configured Field's value
 *      change in pre and post svc flist.
 *  2.  Create the flist to update the service object if provisioning is
 *      required
 *
 ******************************************************************************/
PIN_EXPORT void
fm_telco_prov_fields_prov_info(
        pcm_context_t               *ctxp,
        pin_flist_t                 *pre_svc_flistp,
        pin_flist_t                 *post_svc_flistp,
        pin_flist_t                 *svc_order_arrp,
        int32                       *so_elem_idp,
        int32                       *feat_prov_flagp,
        pin_errbuf_t                *ebufp);

/********************************************************************************
 * fm_telco_prov_feat_prov_info
 *
 *  This does the following functions
 *     Get Features with status PROVISIONING/UNPROVISIONING/UNPROVISIONING
 *     Identify the features that are already provisioned
 *     Update the status for features if required
 *     Create the flist with feature info that need provisioning
 *  
 *******************************************************************************/
PIN_EXPORT void
fm_telco_prov_feat_prov_info(         
	pcm_context_t               *ctxp, 
	pin_flist_t                 *pre_svc_flistp, 
	pin_flist_t                 *post_svc_flistp, 
	char                        *tcf_actionp, 
	pin_flist_t                 *tcf_prov_flistp,
	int32                       *feat_prov_flagp,
	cm_nap_connection_t         *connp,
	pin_errbuf_t                *ebufp);

/********************************************************************************
 * fm_telco_prov_profile_prov_info 
 * 
 *  This does the following functions
 *  1.  Decides the Service Order action based on the Status of
 *      Profile object
 *  2.  Create the Profile  provisioning info to be appended to the SO
 *
 *******************************************************************************/
PIN_EXPORT void
fm_telco_prov_profile_prov_info(         
	pcm_context_t               *ctxp, 
	pin_flist_t                 *pre_profile_flistp, 
	pin_flist_t                 *post_profile_flistp, 
	pin_flist_t                 *profile_prov_flistp, 
	int                         *profile_prov_flagp,
	pin_errbuf_t                *ebufp);

/********************************************************************************
 * fm_telco_prov_device_prov_actinfo 
 * 
 *  This does the following functions
 *  1.  Get the device events in transflist for a particular device
 *  2.  Based on the event type decide the dvice action.
 *  3.  Invoke fm_telco_prov_device_prov_info() to get the device info reqd
 *        to create the Service Order
 *******************************************************************************/
PIN_EXPORT void
fm_telco_prov_device_prov_actinfo(         
	pcm_context_t               *ctxp, 
	pin_flist_t                 *dev_evnt_flistp, 
	pin_flist_t                 *svc_order_arrp,
	int32                       *so_elem_idp,
	char                        *device_action,
	int32						device_action_len,
	pin_errbuf_t                *ebufp);

/********************************************************************************
 * fm_telco_prov_device_prov_info 
 * 
 *  This does the following functions
 *  1.  Based on the Event Type decide the device action.
 *  2.  Read the device objects using the device poid
 *  3.  Create the device info to be appended to the SO
 *******************************************************************************/
PIN_EXPORT void
fm_telco_prov_device_prov_info(         
	pcm_context_t               *ctxp, 
	char                        *event_type, 
	poid_t                      *new_device_objp, 
	poid_t                      *old_device_objp,
	pin_flist_t                 *svc_order_arrp,
	int32                       *so_elem_idp,
	char                        *device_action,
	int32						device_action_len,
	pin_errbuf_t                *ebufp);

/********************************************************************************
 * fm_telco_prov_device_prov_srch 
 * 
 *  Search the device object based on the Service associated and 
 *   retreive the paramaters required to create the device info needed
 *   to create the Service Order
 *
 *******************************************************************************/
PIN_EXPORT void
fm_telco_prov_device_prov_srch(         
	pcm_context_t               *ctxp, 
	poid_t                      *svc_objp, 
	pin_flist_t                 *svc_order_arrp, 
	int32                       *so_elem_idp, 
	pin_flist_t                 *dev_evnt_flistp, 
	char                        *tcf_actionp, /* 20-Feb-2003 PRSF00032319 */
	pin_errbuf_t                *ebufp);

/******************************************************************************
 * fm_telco_prov_handle_telco_features 
 *   
 *****************************************************************************/
PIN_EXPORT void
fm_telco_prov_handle_telco_features(         
	pcm_context_t               *ctxp, 
	char                        *field_namep,
	pin_flist_t                 *svc_flistp, 
	pin_flist_t                 *prov_flistp, 
	int32                       *param_elem_idp,
	char                        *tcf_actionp, 
	pin_errbuf_t                *ebufp);
 
/*******************************************************************************
 * fm_telco_prov_handle_other_service_device:
 ******************************************************************************/
void
fm_telco_prov_handle_other_service_device(
	pcm_context_t               *ctxp,
	poid_t                      *acct_objp,
	pin_flist_t                 device_flistp,	
	pin_flist_t                 dev_elem_flistp,	
	int                         field,
	pin_flist_t                 *prov_flistp,
	int32                       *param_elem_idp,
	pin_errbuf_t                *ebufp);

/*******************************************************************************
 * fm_telco_prov_add_service_order_params:
 ******************************************************************************/
PIN_EXPORT void
fm_telco_prov_add_service_order_params(
	pcm_context_t               *ctxp,
	int                         field_num,
	pin_flist_t                 *device_flistp,
	pin_flist_t                 *device_flist2p,
	pin_flist_t                 *configp_flistp,
	pin_flist_t                 *prov_flistp,
	int32                       *param_elem_idp,
	char                        *action,
	pin_errbuf_t                *ebufp);

/*******************************************************************************
 * fm_telco_prov_utils_add_service_order_params:
 ******************************************************************************/
void
fm_telco_prov_utils_add_service_order_params(
        pcm_context_t   *ctxp,
        int32           field_num,
        pin_flist_t     *device_flistp,
        pin_flist_t     *device_flist2p,
        pin_flist_t     *configp_flistp,
        pin_flist_t     *prov_flistp,
        int32           *param_elem_idp,
        char            *action,
        pin_errbuf_t    *ebufp);

/*******************************************************************************
 * fm_telco_prov_utils_handle_other_service_device:
 ******************************************************************************/
void
fm_telco_prov_utils_handle_other_service_device(
        pcm_context_t   *ctxp,
        poid_t          *acct_objp,
        pin_flist_t     *device_flistp,
        pin_flist_t     *dev_elem_flistp,
        int32           field,
        pin_flist_t     *prov_flistp,
        int32           *param_elem_idp,
        int32           elementIsConfiguredFlag,
        pin_errbuf_t    *ebufp);

PIN_EXPORT int
fm_telco_prov_append_feat(
        pcm_context_t   *ctxp,
        pin_flist_t     *svc_flistp,
        char            *param_namep,
        tcf_feature_service_era_status_t    *param_statusp,
        pin_flist_t     *inherited_flistp,
        int             *drop_elem_ids,
        int             *drop_count,
        pin_errbuf_t    *ebufp);

PIN_EXPORT void
fm_telco_prov_utils_cust_update_services(
        pcm_context_t   *ctxp,
        pin_flist_t     *i_flistp,
	int32           flags,
        pin_errbuf_t    *ebufp);

PIN_EXPORT void 
fm_telco_prov_utils_read_object(
        pcm_context_t           *ctxp,
        poid_t                  *poidOfObj,
        pin_flist_t             **resultFlistpp,
        pin_errbuf_t            *ebufp);

/*******************************************************************************
 * fm_telco_prov_utils_get_acct_products :
 *
 * This Function gets all the purchased offering objects of the input Account 
 * object by calling PCM_OP_SUBSCRIPTION_GET_PURCHASED_OFFERINGS opcode.
 *
*******************************************************************************/
PIN_EXPORT void 
fm_telco_prov_utils_get_acct_products(
                        pcm_context_t           *ctxp,
                        poid_t                  *poidOfAccount,
                        poid_t                  *scopePoidp,
                        pin_flist_t             **accountFlistpp,
                        pin_errbuf_t            *ebufp);


/*******************************************************************************
 * fm_telco_prov_utils_get_acct_discounts :
 *
 * This Function gets all the purchased offering objects of the input Account 
 * object by calling PCM_OP_SUBSCRIPTION_GET_PURCHASED_OFFERINGS opcode.
 *
*******************************************************************************/
PIN_EXPORT void 
fm_telco_prov_utils_get_acct_discounts(
                        pcm_context_t           *ctxp,
                        poid_t                  *poidOfAccount,
                        poid_t                  *scopePoidp,
                        pin_flist_t             **accountFlistpp,
                        pin_errbuf_t            *ebufp);


/******************************************************************************
 * Macros to access the flist fields.
 *****************************************************************************/
#define COPY_MANDATORY_FIELD(i_flist_sp, r_flist_sp, fld, tmp) {        \
        (tmp) = (i_flist_sp)->get((fld));       		        \
	(r_flist_sp)->set((fld), (tmp));                                \
}

#define COPY_OPTIONAL_FIELD(i_flist_sp, r_flist_sp, fld, tmp) {         \
        if (!i_flist_sp->isNull()) {                                    \
                (tmp) = (i_flist_sp)->get((fld), PIN_BOOLEAN_TRUE);     \
                if (!(tmp)->isNull()) {                                 \
	                 (r_flist_sp)->set((fld), (tmp));               \
                }                                                       \
        }                                                               \
        else  {                                                         \
                (tmp) = NULL;                                           \
        }                                                               \
}

#define COPY_OPTIONAL_AGGREGATE_FIELD(i_flist_sp, session_flist_sp, fld, r_flist_sp, tmp) {	\
	COPY_OPTIONAL_FIELD(i_flist_sp, r_flist_sp, fld, tmp);			\
	if ((tmp->isNull()) && !(session_flist_sp->isNull()))			\
		COPY_OPTIONAL_FIELD(session_flist_sp, r_flist_sp, fld, tmp);	\
}

#ifdef __cplusplus
}
#endif

#endif  /*_PIN_TCF_H*/

