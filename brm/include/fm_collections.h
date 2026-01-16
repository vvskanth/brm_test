/*
* Copyright (c) 2001, 2022, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _FM_COLLECTIONS_H_
#define _FM_COLLECTIONS_H_

#define EXTERN extern
#include "pin_act.h"
#include "pin_cust.h"
/* Default profile name string */
#define PIN_PROFILE_DEFAULT_USD		"Default (USD)"

/* Action type strings */
#define PIN_ACTION_DUNNING_LETTER	"dunning_letter"
#define PIN_ACTION_INVOICE_REMINDER	"invoice_reminder"
#define PIN_ACTION_POLICY_ACTION	"policy_action"
#define PIN_ACTION_LATE_FEES		"late_fee"
#define PIN_ACTION_FINANCE_CHARGES	"finance_charge"
#define PIN_ACTION_MANUAL_ACTION	"manual_action"
#define	PIN_ACTION_REFER_TO_OUTSIDE_AGENCY	"refer_to_outside_agency"
#define PIN_ACTION_CLOSE_BILLINFO       "close_billinfo"
#define PIN_ACTION_INACTIVATE_BILLINFO  "inactivate_billinfo"
#define PIN_ACTION_WRITEOFF_BILLINFO    "writeoff_billinfo"
#define PIN_ACTION_PROMISE_TO_PAY       "promise_to_pay"
#define PIN_ACTION_COLLECT_PAYMENT	"collect_payment"

/* To be removed in future */
#define PIN_ACTION_CLOSE_ACCOUNT       "close_billinfo"
#define PIN_ACTION_INACTIVATE_ACCOUNT  "inactivate_billinfo"
#define PIN_ACTION_WRITEOFF_ACCOUNT    "writeoff_billinfo"

/* Transaction flist */
#define PIN_TRANS_NAME_GRP_INFO "group_info"
#define PIN_TRANS_NAME_COLL_GRP_NAME "coll_group_name"
#define PIN_TRANS_NAME_COLL_ACTION_INFO "coll_action_info"

/* PCM_OP_COLLECTIONS_PROCESS_BILLINFO run status */
typedef enum pin_collections_status {
	PIN_COLLECTIONS_STATUS_REMAIN_OUT	=	0,
	PIN_COLLECTIONS_STATUS_ENTER		=	1,
	PIN_COLLECTIONS_STATUS_REMAIN_IN	=	2,
	PIN_COLLECTIONS_STATUS_EXIT		=	3
} pin_collections_status_t;

#ifndef _COLLECTIONS_ACTION_STATUS
#define _COLLECTIONS_ACTION_STATUS
/* collections action status */
typedef enum pin_action_status {
	PIN_ACTION_PENDING	=	0,
	PIN_ACTION_CANCELED	=	1,
	PIN_ACTION_COMPLETED	=	2,
	PIN_ACTION_ERROR	=	3,
	PIN_ACTION_NO_EXECUTE	=	4,
	PIN_ACTION_WAITING_FOR_DEPENDENT = 5,
	PIN_ACTION_BROKEN       =       6
} pin_action_status_t;
#endif

#define PIN_COLL_ACTION_STATUSES_COUNT 7

/* collections overall P2P status */
typedef enum pin_coll_overall_p2p_status {
        PIN_COLL_P2P_PENDING     =       0,
        PIN_COLL_P2P_CANCELLED   =       1,
        PIN_COLL_P2P_COMPLETED   =       2,
        PIN_COLL_P2P_KEPT        =       3,
        PIN_COLL_P2P_BROKEN      =       4
} pin_coll_overall_p2p_status_t;

/* collections scenario exit reason */
typedef enum pin_exit_reason {
	PIN_SCENARIO_EXIT_NORMAL	        =	0,
	PIN_SCNEARIO_EXIT_ENFORCED	        =	1,
	PIN_SCENARIO_EXIT_PROMISE_ATTAINED      =       2,
	PIN_SCENARIO_EXIT_REPLACED		=	3
} pin_exit_reason_t;


/* PIN_FLD_ACTION MODE action execute option */

typedef enum pin_action_execute_option {
	PIN_ACTION_MUST_EXECUTE		=	0,
	PIN_ACTION_OPTIONAL_EXECUTE	=	1
} pin_action_execute_option_t;

/*****flags for rescheduling actions.  ****/
#define PIN_ACTION_UPDATE_ONE_ACTION  0x0
#define PIN_ACTION_UPDATE_ALL_ACTIONS  0x1
#define PIN_SCHDL_NEXT_ACTIONS_NEXT_TO_CURR_DAY 0x2


/* PIN_FLD_SCHEDULE_MODE - collections action schedule mode
 * collections action scheduling modes during scenario replacement. */
typedef enum pin_action_schedule_mode {
         PIN_ACTION_SCHEDULE_BILL_DUE_DATE      =       0,
         PIN_ACTION_SCHEDULE_USER_DATE          =       1
} pin_action_schedule_mode_t;


/* Audit event type */
#define PIN_COLLECTIONS_AUDIT_ACTION		"/event/audit/collections/action"
#define PIN_COLLECTIONS_ACTIVITY_PROMISE_TO_PAY "/event/activity/collections/promise_to_pay"

/* Collections Activity event type */
#define PIN_COLLECTIONS_ACTIVITY_PROMISE_TO_PAY  	"/event/activity/collections/promise_to_pay"
#define PIN_COLLECTIONS_ACTIVITY_REPLACE_SCENARIO 	"/event/activity/collections/replace_scenario"
#define PIN_COLLECTIONS_ACTIVITY_EXEMPTION      	"/event/activity/collections/exemption"

#define PIN_EVENT_DESCR_BILLINFO_EXEMPTED_COLLECTIONS "Billinfo Exempted From Collections"
#define PIN_EVENT_DESCR_BILLINFO_UNEXEMPTED_COLLECTIONS "Billinfo Un-Exempted From Collections"
#define PIN_EVENT_SYS_DESCR_EXEMPTED_COLLECTIONS "Collections Exemption"


/* Collectiosn group related */
#define COLLECTIONS_GROUP_POID_TYPE           "/group/collections_targets"
#define COLLECTIONS_GROUP_TYPE_STR            "Collections Group"
#define COLLECTIONS_GROUP_PROGRAM_STR         "Collections group"

#define PIN_EVENT_DESCR_COLLECTIONS_GROUP_MEMBER_ADD    "Collections Group Member Added"
#define PIN_EVENT_DESCR_COLLECTIONS_GROUP_MEMBER_DELETE "Collections Group Member Deleted"
#define PIN_EVENT_DESCR_COLLECTIONS_GROUP_PARENT        "Collections Group Set Parent"
#define PIN_EVENT_DESCR_COLLECTIONS_GROUP_CREATE       "Collections Group Create"
#define PIN_EVENT_DESCR_COLLECTIONS_GROUP_DELETE       "Collections Group Delete"

#define PIN_OBJ_TYPE_EVENT_GROUP_COLLECTIONS_TARGETS "/event/group/collections_targets"

/* Collections group related */
#define PIN_COLL_INCLUDE_PENDING_RECEIVABLE  0x01

/* Object type string */
#define PIN_OBJ_TYPE_PROMISE_TO_PAY		"/collections_action/promise_to_pay"

/* Object name string */
#define PIN_OBJ_NAME_ACTION			"Collections Action Object"
#define PIN_OBJ_NAME_SCENARIO 			"Collections Scenario Object"
#define PIN_OBJ_NAME_PROMISE_TO_PAY		"Collections Promise to Pay Object"
#define PIN_OBJ_NAME_COLLECT_PAYMENT		"Collections Collect Payment Object"

#define PIN_OBJ_TYPE_PROMISE_TO_PAY             "/collections_action/promise_to_pay"
#define PIN_OBJ_TYPE_COLLECT_PAYMENT		"/collections_action/collect_payment"

/* Audit event description strings */
#define PIN_EVENT_DESCR_COLLECTIONS_CREATE_ACTION	"Creating: Action"
#define PIN_EVENT_DESCR_COLLECTIONS_UPDATE_ACTION	"Updating: Action"
#define PIN_EVENT_DESCR_COLLECTIONS_ENTER               "Entered collections"
#define PIN_EVENT_DESCR_COLLECTIONS_EXIT                "Exited collections"
#define PIN_EVENT_DESCR_PROMISE_ATTAINED		"Promise Attained"
#define PIN_EVENT_DESCR_PROMISE_BREACHED		"Promise Breached"
#define PIN_EVENT_DESCR_PROMISE_INVOKED			"Promise to Pay Invoked"
#define PIN_EVENT_DESCR_SCENARIO_REPLACEMENT		"Scenario Replacement"

/* Action update description strings */
#define PIN_ACTION_DESCR_CREATE				"Action created"
#define PIN_ACTION_DESCR_CANCEL				"Action canceled"
#define PIN_ACTION_DESCR_ASSIGN_AGENT			"Agent assigned"
#define PIN_ACTION_DESCR_TAKEN				"Action taken"
#define PIN_ACTION_DESCR_RESCHEDULE			"Action rescheduled"
#define PIN_ACTION_DESCR_SET_STATUS			"Action status set"
#define PIN_ACTION_DESCR_SCHEDULE			"Action scheduled"
#define PIN_ACTION_DESCR_UPDATE 			"Action updated"
#define PIN_ACTION_DESCR_PENDING                        "Action pending"

/* Collections Action Due Notifications */
#define PIN_OBJ_NAME_EVENT_COLL_ACTION_DUE              "Collections Action Due Notification Log"
#define PIN_OBJ_TYPE_EVENT_NOTIFY			"/event/notification"
#define PIN_EVENT_DESCR_COLL_ACTION_DUE                 "Collections Action Due Notification"

/* Late fee calculation options */
#define PIN_OPTION_EITHER				0
#define PIN_OPTION_MIN					1
#define PIN_OPTION_MAX					2
#define PIN_OPTION_BOTH					3

/* Event type for late fee */
#define PIN_COLLECTIONS_EVENT_LATEFEE		"/event/billing/late_fee"
#define PIN_OBJ_NAME_EVENT_LATEFEE		"Late fee event"
#define PIN_EVENT_DESCR_LATEFEE			"Charge for late fee event"

/* Event type for finance charge */
#define PIN_COLLECTIONS_EVENT_FINANCECHARGE	"/event/billing/finance_charge"
#define PIN_OBJ_NAME_EVENT_FINANCECHARGE	"Finance charge event"
#define PIN_EVENT_DESCR_FINANCECHARGE		"Finance charge"

/* Buffer size to store the description string */
#define COLL_ACTION_DESCR_LEN			1024

/**** Error codes for customer center display proper message in case of any error condition in collections *****/
#define PIN_ERR_COLLECTIONS_MANDATORY_ACTION_STATUS_ERROR  124
#define PIN_ERR_COLLECTIONS_ACTION_INSERT_ERROR  125

/* Error messsage when billinfo is already part of collections group */
#define PIN_ERR_COLLECTIONS_ALREADY_PART_OF_COLLECTIONS_GROUP  126



/**** Error codes for collections center to display proper message in case of any error condition in collections *****/
#define PIN_ERR_COLLECTIONS_ALREADY_UNDER_PROMISE_TO_PAY_OBLIGATION 127
#define PIN_ERR_COLLECTIONS_PROMISE_TO_PAY_INVOKE_IN_PAST 128
#define PIN_ERR_COLLECTIONS_ADD_ACTION_IN_PROMISE_TO_PAY_PERIOD 129
#define PIN_ERR_COLLECTIONS_NOT_UNDER_PROMISE_TO_PAY_OBLIGATION 130
#define PIN_ERR_COLLECTIONS_ADD_PROMISE_ACTION_OUTSIDE_PROMISE_TO_PAY_PERIOD 131
#define PIN_ERR_COLLECTIONS_UPDATE_PROMISE_ACTION_ERROR 132
#define PIN_ERR_COLLECTIONS_MILESTONE_AMOUNT_NOT_FOUND 133
#define PIN_ERR_COLLECTIONS_INVALID_MILESTONE_AMOUNT 134
#define PIN_ERR_COLLECTIONS_MILESTONE_INTERVAL_NOT_FOUND 135
#define PIN_ERR_COLLECTIONS_PARAMETER_IS_ZERO 136
#define PIN_ERR_COLLECTIONS_INVALID_PROMISE_AMOUNT 137
#define PIN_ERR_COLLECTIONS_MEMBER_BILLINFO_CANNNOT_BE_EXEMPTED 138
#define PIN_ERR_COLLECTIONS_BILLINFO_HAS_CYCLE_RELATION  139
#define PIN_ERR_COLLECTIONS_GROUP_NAME_ALREADY_EXISTS  140
#define PIN_ERR_COLLECTIONS_BILLINFO_IS_SUBORD  141
#define PIN_ERR_COLLECTIONS_BILLINFO_IS_ALREADY_EXEMPTED  142
#define PIN_ERR_COLLECTIONS_BILLINFO_IS_ALREADY_MEMBER  143
#define PIN_ERR_COLLECTIONS_BILLINFO_IS_ALREADY_PARENT  144
#define PIN_ERR_COLLECTIONS_BILLINFO_IS_IN_COLLECTIONS 145
#define PIN_ERR_COLLECTIONS_INVALID_CONFIG_COLLECTIONS_PROMISE_TO_PAY_SPEC 211
#define PIN_ERR_COLLECTIONS_INVALID_MILESTONE_PERCENTAGE 212
#define PIN_ERR_COLLECTIONS_INVALID_TOTAL_MILESTONE_AMOUNT 213
#define PIN_ERR_COLLECTIONS_INVALID_TOTAL_MILESTONE_PERCENTAGE 214
#define PIN_ERR_COLLECTIONS_MILESTONE_INTERVAL_EXCEEDED 215
#define PIN_ERR_COLLECTIONS_NUM_OF_MILESTONE_EXCEEDED 216

#define PIN_BILL_WRITEOFF_NO_DUE_ERR           "Write-off error : Cannot writeoff billinfo as billinfo due < 0"

#define PCM_OP_AR_BILLINFO_WRITEOFF     4512

/* fm_collections_cache.c */
EXTERN pin_flist_t *
fm_collections_cache_get_profile(
        pcm_context_t           *ctxp,
        poid_t                  *p_pdp,
        pin_errbuf_t            *ebufp);


EXTERN pin_flist_t *
fm_collections_cache_get_action(
        pcm_context_t           *ctxp,
        poid_t                  *a_pdp,
        pin_errbuf_t            *ebufp);

EXTERN void
fm_collections_cache_update(
        poid_t          *pdp,
        cm_cache_t      *cache_ptr,
        pin_flist_t     *cache_flistp,
        pin_errbuf_t    *ebufp);

EXTERN void
fm_collections_cache_get_template(
        pcm_context_t           *ctxp,
        pin_flist_t             *action_flistp,
        pin_errbuf_t            *ebufp);

/* fm_collections_utils.c */
EXTERN void
fm_collections_utils_get_description(
	char                    *action_str,
	char                    *action_name,
	pin_action_status_t     action_status,
	char 			*descrp,
	pin_errbuf_t            *ebufp);

#endif /* _FM_COLLECTIONS_H_ */
