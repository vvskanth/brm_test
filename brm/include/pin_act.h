/*******************************************************************
 *
 *      
* Copyright (c) 1996, 2023, Oracle and/or its affiliates. 
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef _PIN_ACT_H
#define	_PIN_ACT_H


/*******************************************************************
 * Activity Tracking FM Definitions.
 *******************************************************************/

/*******************************************************************
 * Object Name Strings
 *******************************************************************/
#define PIN_ACTION_DESCR_RESCHEDULE     "Action rescheduled"
#define PIN_ACTION_DESCR_ASSIGN_AGENT   "Agent assigned"
#define PIN_TRANS_NAME_COLL_ACTION_INFO "coll_action_info"
#define PIN_OBJ_NAME_EVENT_ACT 		"Activity Tracking Event Log"
#define PIN_OBJ_NAME_EVENT_SESSION	"Activity Session Log"
#define PIN_OBJ_NAME_EVENT_VERIFY	"Activity Verification Log"
#define PIN_OBJ_NAME_EVENT_CREDIT_THRESHOLD "Credit Threshold Notification Log"
#define PIN_OBJ_NAME_EVENT_CREDIT_LIMIT	"Credit Limit Notification Log"
#define PIN_OBJ_NAME_EVENT_DATE_THRESHOLD "Date Threshold Notification Log"
#define PIN_OBJ_NAME_EVENT_DATE_EXPIRE	"Date Expiration Notification Log"
#define PIN_OBJ_NAME_EVENT_DELETE_ACCT	"Account Object Deletion Log"
#define PIN_OBJ_NAME_EVENT_PRE_DELETE_ACCT   "Account Object Pre Deletion Log"
#define PIN_OBJ_NAME_EVENT_DELETE_SRVC	"Service Object Deletion Log"
#define PIN_OBJ_NAME_EVENT_SERVICE	"Service Object Update Log"
#define PIN_OBJ_NAME_EVENT_SCHEDULE_CREATE "Schedule Create Event Log"
#define PIN_OBJ_NAME_EVENT_SCHEDULE_MODIFY "Schedule Modify Event Log"
#define PIN_OBJ_NAME_EVENT_SCHEDULE_EXECUTE "Schedule Execute Event Log"
#define PIN_OBJ_NAME_EVENT_SCHEDULE_DELETE "Schedule Delete Event Log"
#define PIN_OBJ_NAME_SCHEDULE		"Schedule Object"
#define PIN_OBJ_NAME_EVENT_PROFILE "Profile Object Update Log"
#define PIN_OBJ_NAME_EVENT_CUSTOMER        "Customer Modification Event Log"
#define PIN_OBJ_NAME_EVENT_REQUEST_CREATE  "Request Create Notification Log"

/*******************************************************************
 * Object Type Strings
 *******************************************************************/
#define	PIN_OBJ_TYPE_EVENT_ACT		"/event/activity"
#define	PIN_OBJ_TYPE_EVENT_ADMIN	"/event/activity/admin"
#define	PIN_OBJ_TYPE_EVENT_SESSION	"/event/session"
#define PIN_OBJ_TYPE_EVENT_VERIFY	"/event/activity/verify"
#define PIN_OBJ_TYPE_EVENT_CREDIT_THRESHOLD	"/event/notification/threshold"
#define PIN_OBJ_TYPE_EVENT_CREDIT_THRESHOLD_BELOW	"/event/notification/threshold_below"
#define PIN_OBJ_TYPE_EVENT_CREDIT_LIMIT	"/event/notification/limit"
#define PIN_OBJ_TYPE_EVENT_DATE_THRESHOLD "/event/notification/datethreshold"
#define PIN_OBJ_TYPE_EVENT_DATE_EXPIRE	"/event/notification/datexpire"
#define PIN_OBJ_TYPE_EVENT_DELETE_ACCT	"/event/notification/account/delete"
#define PIN_OBJ_TYPE_EVENT_PRE_DELETE_ACCT   "/event/notification/account/pre_delete"
#define PIN_OBJ_TYPE_EVENT_DELETE_SRVC	"/event/notification/service/delete"
#define PIN_OBJ_TYPE_EVENT_SERVICE	"/event/notification/service/modify"
#define PIN_OBJ_TYPE_EVENT_SCHEDULE_CREATE "/event/schedule/create"
#define PIN_OBJ_TYPE_EVENT_SCHEDULE_MODIFY "/event/schedule/modify"
#define PIN_OBJ_TYPE_EVENT_SCHEDULE_EXECUTE "/event/schedule/execute"
#define PIN_OBJ_TYPE_EVENT_SCHEDULE_DELETE "/event/schedule/delete"
#define PIN_OBJ_TYPE_EVENT_ACCOUNT_CREATE "/event/notification/account/create"
#define PIN_OBJ_TYPE_EVENT_REG_COMPLETE "/event/notification/customer/reg_complete"
#define PIN_OBJ_TYPE_EVENT_UNIQUENESS_CONFIRMED "/event/notification/customer/uniqueness_confirmed"
#define PIN_OBJ_TYPE_EVENT_DEAL_CHANGE "/event/notification/deal/change"
#define PIN_OBJ_TYPE_EVENT_DEAL_CHANGE_COMPLETE "/event/notification/deal/change_complete"
#define PIN_OBJ_TYPE_EVENT_CUSTOMER_PRE_MODIFY "/event/notification/customer/pre_modify"
#define PIN_OBJ_TYPE_EVENT_CUSTOMER_MODIFY "/event/notification/customer/modify"
#define PIN_OBJ_TYPE_EVENT_SERVICE_PRE_UPDATE "/event/notification/service/pre_change"
#define PIN_OBJ_TYPE_EVENT_SERVICE_POST_UPDATE "/event/notification/service/post_change"
#define PIN_OBJ_TYPE_EVENT_SERVICE_PRE_CREATE "/event/notification/service/pre_create"
#define PIN_OBJ_TYPE_EVENT_SERVICE_PRE_PURCHASE "/event/notification/service/pre_purchase"
#define PIN_OBJ_TYPE_EVENT_SERVICE_CREATE "/event/notification/service/create"
#define PIN_OBJ_TYPE_EVENT_PROFILE_CREATE "/event/notification/profile/create"
#define PIN_OBJ_TYPE_EVENT_PROFILE_MODIFY "/event/notification/profile/modify"
#define PIN_OBJ_TYPE_EVENT_PROFILE_PRE_MODIFY "/event/notification/profile/pre_modify"
#define PIN_OBJ_TYPE_EVENT_PROFILE_DELETE "/event/notification/profile/delete"
#define PIN_OBJ_TYPE_EVENT_OFFER_PROFILE_CREATE "/event/notification/offer_profile/create"
#define PIN_OBJ_TYPE_EVENT_OFFER_PROFILE_UPDATE "/event/notification/offer_profile/update"
#define PIN_OBJ_TYPE_EVENT_OFFER_PROFILE_DELETE "/event/notification/offer_profile/delete"      
#define PIN_OBJ_TYPE_EVENT_OFFER_PROFILE_THRESHOLD_BREACH "/event/notification/offer_profile/ThresholdBreach"
#define PIN_OBJ_TYPE_EVENT_BILL_NOW "/event/notification/service_item/bill_now"
#define PIN_OBJ_TYPE_EVENT_MAKE_BILL "/event/notification/service_item/make_bill"
#define PIN_OBJ_TYPE_EVENT_MAKE_BILL_START  "/event/notification/service_item/make_bill_start"
#define PIN_OBJ_TYPE_EVENT_BILL_DELAY "/event/notification/service_item/bill_delay"
#define PIN_OBJ_TYPE_EVENT_DEAL_TRANSITION "/event/notification/deal/transition"
#define PIN_OBJ_TYPE_EVENT_PLAN_TRANSITION_POST_DEAL_CANCEL "/event/notification/plan/transition/post_deal_cancel"
#define PIN_OBJ_TYPE_EVENT_DEAL_TRANSITION_COMPLETE "/event/notification/deal/transition_complete"
#define PIN_OBJ_TYPE_EVENT_PLAN_TRANSITION "/event/notification/plan/transition"
#define PIN_OBJ_TYPE_EVENT_PLAN_TRANSITION_COMPLETE "/event/notification/plan/transition_complete"
#define PIN_OBJ_TYPE_EVENT_BILL_START "/event/notification/billing/start"
#define PIN_OBJ_TYPE_EVENT_BILL_END "/event/notification/billing/end"
#define PIN_OBJ_TYPE_EVENT_BILL_START_PARTIAL  "/event/notification/billing/start_partial"
#define PIN_OBJ_TYPE_EVENT_BILL_END_PARTIAL  "/event/notification/billing/end_partial"
#define PIN_OBJ_TYPE_EVENT_BILL_NOW_START "/event/notification/bill_now/start"
#define PIN_OBJ_TYPE_EVENT_BILL_NOW_END "/event/notification/bill_now/end"
#define PIN_OBJ_TYPE_EVENT_ROLLOVER_START "/event/notification/rollover/start"
#define PIN_OBJ_TYPE_EVENT_ROLLOVER_END "/event/notification/rollover/end"
#define PIN_OBJ_TYPE_EVENT_CYCLE_START "/event/notification/cycle/start"
#define PIN_OBJ_TYPE_EVENT_CYCLE_END "/event/notification/cycle/end"
#define PIN_OBJ_TYPE_EVENT_COLL_INFO_CHANGE "/event/notification/collections/info_change"
#define PIN_OBJ_TYPE_EVENT_COLLECTIONS "/event/audit/collections/action"
#define PIN_OBJ_TYPE_EVENT_GROUP_COLLECTIONS "/event/group/collections_targets"
#define PIN_OBJ_TYPE_EVENT_COLL_EXEMPTION "/event/activity/collections/exemption"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_REQUEST_CREATE "/event/notification/request/create"

/*******************************************************************
 * Collections object type Strings
 *******************************************************************/
#define PIN_OBJ_TYPE_PROMISE_TO_PAY			"/collections_action/promise_to_pay"

/*******************************************************************
 * Event Description Strings
 *******************************************************************/
#define PIN_EVENT_DESCR_ACT		"Activity"
#define PIN_EVENT_DESCR_SESSION		"Session"
#define PIN_EVENT_DESCR_CREDIT_THRESHOLD "Credit Threshold Exceeded"
#define PIN_EVENT_DESCR_CREDIT_LIMIT	"Credit Limit Exceeded"
#define PIN_EVENT_DESCR_DATE_THRESHOLD	"Date Threshold Exceeded"
#define PIN_EVENT_DESCR_DATE_EXPIRE	"Date Expire"
#define PIN_EVENT_DESCR_DELETE_ACCT	"Account Object Deleted"
#define PIN_EVENT_DESCR_PRE_DELETE_ACCT	"Account Object Delete Begin"
#define PIN_EVENT_DESCR_DELETE_SRVC	"Service Object Deleted"
#define PIN_EVENT_DESCR_SERVICE		"Service Object Updated"
#define PIN_EVENT_DESCR_SERVICE_PRE_MODIFY "Service Object Update begin"
#define PIN_EVENT_DESCR_SERVICE_POST_MODIFY "Service Object Update complete"
#define PIN_EVENT_DESCR_SERVICE_CREATE "Service Object Created"
#define PIN_EVENT_DESCR_SCHEDULE_CREATE "Schedule Object Created"
#define PIN_EVENT_DESCR_SCHEDULE_MODIFY "Schedule Object Modified"
#define PIN_EVENT_DESCR_SCHEDULE_EXECUTE "Schedule Object Executed"
#define PIN_EVENT_DESCR_SCHEDULE_DELETE "Schedule Object Deleted"
#define PIN_EVENT_DESCR_CUSTOMER_PRE_MODIFY "Customer Object pre Modification"
#define PIN_EVENT_DESCR_CUSTOMER_MODIFY "Customer Object Modified"
#define PIN_EVENT_DESCR_PROFILE_CREATE "Profile Object Created"
#define PIN_EVENT_DESCR_PROFILE_MODIFY "Profile Object Modified"
#define PIN_EVENT_DESCR_PROFILE_PRE_MODIFY "Profile Object about to be modified"
#define PIN_EVENT_DESCR_PROFILE_DELETE "Profile Event Deleted"
#define PIN_EVENT_DESCR_OFFER_PROFILE_CREATE "Creating: Offer_profile"
#define PIN_EVENT_DESCR_OFFER_PROFILE_DELETE "Deleting: Offer_profile"
#define PIN_EVENT_DESCR_OFFER_PROFILE_UPDATE "Updating: Offer_profile"
#define PIN_EVENT_DESCR_OFFER_PROFILE_THRESHOLD_BREACH "Offer Profile Threshold Breached"
#define PIN_EVENT_DESCR_COLL_INFO_CHANGE "CollectionsInfoChange"
#define PIN_EVENT_DESCR_COLLECTIONS_GROUP_MEMBER_ADD "Collections Group Member Added"
#define PIN_EVENT_DESCR_COLLECTIONS_GROUP_MEMBER_DELETE "Collections Group Member Deleted"
#define PIN_EVENT_DESCR_REQUEST_CREATE "Request record created"

/*******************************************************************
 * PCM_OP_POL_ACT_SPEC_VERIFY fields
 *******************************************************************/
/*
 * PIN_FLD_TYPE - type of check to perform
 */
typedef enum pin_act_check {
	PIN_ACT_CHECK_UNDEFINED =	0,
	PIN_ACT_CHECK_ACCT_TYPE =	1,
	PIN_ACT_CHECK_ACCT_STATUS =	2,
        PIN_ACT_CHECK_ACCT_PASSWD =	3,
        PIN_ACT_CHECK_SRVC_TYPE =	4,
        PIN_ACT_CHECK_SRVC_STATUS =	5,
        PIN_ACT_CHECK_SRVC_PASSWD =	6,
        PIN_ACT_CHECK_CREDIT_AVAIL =	7,
        PIN_ACT_CHECK_DUPE_SESSION =	8
} pin_act_check_t;

/*
 * PIN_FLD_SCOPE - Scope of check to be performed.
 */
typedef enum pin_act_verify_scope {
        PIN_ACT_VERIFY_SCOPE_SNAPSHOT = 1,
        PIN_ACT_VERIFY_SCOPE_MASTER =   2
} pin_act_verify_scope_t;

/*
 * PIN_FLD_SUBTYPE - scope of check to perform
 */
typedef enum pin_act_scope {
	PIN_ACT_SCOPE_UNDEFINED =	0,
	PIN_ACT_SCOPE_SRVC_OBJ =	1,
	PIN_ACT_SCOPE_SRVC_TYPE =	2
} pin_act_scope_t;

/*
 * PIN_FLD_STATUS - Status of a schedule object
 */
typedef enum pin_schedule_status {
        PIN_SCHEDULE_STATUS_PENDING = 0,
        PIN_SCHEDULE_STATUS_DONE = 1,
        PIN_SCHEDULE_STATUS_ERROR = 2,
	PIN_SCHEDULE_STATUS_PENDING_UNDELETABLE = 3
} pin_schedule_status_t;

/*
 * PIN_FLD_PASSWD_STATUS - Status of passwd
 */
typedef enum pin_passwd_status {
	PIN_PASSWDSTATUS_NORMAL = 0,
	PIN_PASSWDSTATUS_TEMPORARY = 1,
	PIN_PASSWDSTATUS_EXPIRES = 2,
	PIN_PASSWDSTATUS_INVALID = 3
} pin_passwd_status_t;

/*
 * PIN_FLD_RESOURCE_STATUS, PIN_FLD_RESULT - 
 * for PCM_OP_ACT_CHEKC_RESOURCE_THRESHOLD - 
 * Status of the Resource Threshold status
 */
typedef enum pin_resource_status {
        PIN_RESOURCE_STATUS_RED = 0,
        PIN_RESOURCE_STATUS_GREEN = 1,
        PIN_RESOURCE_STATUS_YELLOW =  2
} pin_resource_status_t;

/*
 * PIN_FLD_TYPE 
 * for PCM_OP_ACT_CHEKC_RESOURCE_THRESHOLD - 
 * Authorization types
 */
typedef enum pin_authorization_types {
        PIN_TYPE_AUTH  = 0,
        PIN_TYPE_REAUTH = 1
} pin_authorization_types_t;

/*******************************************************************
 * PCM_OP_ACT_VERIFY fields.
 *******************************************************************/
/*
 * PIN_FLD_RESULT - result of a act_verify operation.
 */

#define	PIN_ACT_VERIFY_PASSED		PIN_BOOLEAN_TRUE
#define	PIN_ACT_VERIFY_FAILED		PIN_BOOLEAN_FALSE

/*
 * Payment flow
 */
#define PYMT_MOVED_TO_SUSP_FLOW		0
#define PYMT_SUSP_TO_REG_ACCT_FLOW	1

/*
 * Deletion flags
 */
#define PIN_ACT_DELETE_RESERVATION_OBJ		1
#define PIN_ACT_DELETE_ASO			2 

/*
 * PIN_FLD_REASON - reason for result act_verify operation. 
 */

#define	PIN_ACT_VERIFY_PASSED_LOGIN		0
#define	PIN_ACT_VERIFY_FAILED_LOGIN		2
#define	PIN_ACT_VERIFY_PASSED_TEMPPASSWD	4
#define	PIN_ACT_VERIFY_FAILED_EXPPASSWD		5
#define	PIN_ACT_VERIFY_FAILED_INVALIDPASSWD	6
#define PIN_ACT_VERIFY_SERVICE_LOCKED		7

/* Lock status for pcm_client and admin_client services */
#define PIN_SERVICE_LOCKED			PIN_BOOLEAN_TRUE	

/* pin_notify mode for act_usage*/
#define PIN_ACT_NOTIFY_EXACT_MATCH_EVENT            0
#define PIN_ACT_NOTIFY_REGEX_MATCH_EVENT            1

/*******************************************************************
 * PCM_OP_ACT_MULTI_AUTHORIZE fields.
 *******************************************************************/
/*
 * modes of multi_authorize call
 */
#define MULTI_AUTH_MODE_INTERN_BAL_AND_RR 0
#define MULTI_AUTH_MODE_INTERN_BAL_AND_NO_RR 1
#define MULTI_AUTH_MODE_EXTERN_MONETARY_BAL_AND_NO_RR 2

/*
 * PIN_FLD_RESULT - result of an act_multi_authorize operation.
 */

#define PIN_ACT_MULTI_AUTH_PASSED       PIN_BOOLEAN_TRUE
#define PIN_ACT_MULTI_AUTH_FAILED       PIN_BOOLEAN_FALSE

/*
 * PIN_FLD_REASON - reason for the result of act_multi_auth operation.
 * PCM_OP_ACT_MULTI_AUTHORIZE uses codes between 101-120
 */

#define PIN_ACT_MULTI_AUTH_LOGIN_NOT_FOUND         101
#define PIN_ACT_MULTI_AUTH_RATING_FAILURE          102
#define PIN_ACT_MULTI_AUTH_RESERVATION_FAILURE     103
#define PIN_ACT_MULTI_AUTH_INPUT_ERR               104

/* Introduced new MODE for PCM_OP_ACT_UPDATE_SESSION to skip calling ACT USAGE */
#define PIN_ACT_UPDATE_SESSION_SKIP_RATING 0x08000

/*******************************************************************
 * PCM_OP_ACT_EVENT defines.
 *******************************************************************/
typedef enum pin_act_item_type {
	PIN_ACT_ITEM_INDIVIDUAL =	1,
	PIN_ACT_ITEM_CUMULATIVE =	2,
	PIN_ACT_ITEM_PRE_CREATE =	3
} pin_act_item_type_t;

/*
 * PIN_FLD_TYPE - type of newsfeed
 */
typedef enum pin_newsfeed_type {
    PIN_FLD_TYPE_UNDEFINED            =  0,

    // AR and Payment Types Reserve 1 - 20
    PIN_FLD_TYPE_ADJUSTMENT           =  1,
    PIN_FLD_TYPE_NCR_ADJUSTMENT       =  2,
    PIN_FLD_TYPE_OPEN_DISPUTE         =  3,
    PIN_FLD_TYPE_CLOSED_DISPUTE       =  4,
    PIN_FLD_TYPE_WRITEOFF             =  5,
    PIN_FLD_TYPE_REFUND               =  6,
    PIN_FLD_TYPE_PAYMENT              =  7,
    PIN_FLD_TYPE_PAYMENT_REVERSAL     =  8,
    PIN_FLD_TYPE_ALLOCATED            =  9,
    PIN_FLD_TYPE_PYMT_CHANGE          =  10,
    PIN_FLD_TYPE_LIMIT_CHANGE         =  11,
    PIN_FLD_TYPE_WRITEOFF_REVERSAL    =  12,

    // Subscription Types Reserve 21 - 40
    PIN_FLD_TYPE_PURCHASE             =  21,
    PIN_FLD_TYPE_CANCEL               =  22,
    PIN_FLD_TYPE_OFF_STATUS           =  23,
    PIN_FLD_TYPE_TRANSITION           =  24,

    // Collections Types Reserve 41 - 45
    PIN_FLD_TYPE_COLLECTIONS          =  41,

    // Account Change Types Reserve 46 - 65
    PIN_FLD_TYPE_NAMEINFO             =  46,
    PIN_FLD_TYPE_ACCT_STATUS          =  47,
    PIN_FLD_TYPE_DEFERRED             =  48,
    PIN_FLD_TYPE_PAYINFO              =  49,
    PIN_FLD_TYPE_BILLINFO             =  50,
    PIN_FLD_TYPE_BILLUNIT_CREATED     =  51,
    PIN_FLD_TYPE_BILLUNIT_DELETED     =  52,
    PIN_FLD_TYPE_BALGRP_TRANSFER      =  53,
	 
    // Service Change Types Reserve 66 - 75
    PIN_FLD_TYPE_SRVC_STATUS          =  66,
    PIN_FLD_TYPE_SRV_TO_DEV           =  67,

    // Charges Types Reserved 76 - 90
    PIN_FLD_TYPE_CORRECTIVE_BILL      =  76,
    PIN_FLD_TYPE_RECURRING_CHRG       =  77,
    PIN_FLD_TYPE_BILL_ISSUED          =  78,
    PIN_FLD_TYPE_BILL_ISSUED_MIDCYCLE =  79,
    PIN_FLD_TYPE_ONE_TIME_CHARGE      =  80,
	
    // Sharing Group Types Reserved 91 - 105
    PIN_FLD_TYPE_SHARING_GROUP        =  91,

    //Custom Types Reserved from 10000 onwards. 
} pin_act_newsfeed_type_t;

#ifndef _COLLECTIONS_ACTION_STATUS
#define _COLLECTIONS_ACTION_STATUS
/* collections action status taken from fm_collections.h */
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

/*******************************************************************
 * Data structures used with fm_act module.
 *******************************************************************/
typedef struct notify_entry {
	u_int		opcode;
	u_int		opflag;
	char		expr[256];
#if defined(_REGEX_H) || defined(_pin_os_regex_h_)
        regex_t         compiled_expr;
#else
        char            compiled_expr[256];
#endif
} notify_t;

typedef struct notify_table {
	u_int		howmany;
	notify_t 	*table;
} notify_table_t;

typedef struct spec_rates_entry {
	u_int		opcode;
	u_int		opflag;
	char		expr[256];
#if defined(_REGEX_H) || defined(_pin_os_regex_h_)
	regex_t		compiled_expr;
#else
	char		compiled_expr[256];
#endif
} spec_rates_t;

typedef struct spec_rates_table {
	u_int		howmany;
	spec_rates_t 	*table;
} spec_rates_table_t;

typedef struct verify_entry {
	u_int		type;
	u_int		flags;
	char		descr[255];
} verify_t;

typedef struct verify_table {
	u_int		howmany;
	verify_t 	*table;
} verify_table_t;


/*******************************************************************
 * Data structures used for FLIST trimming before sending to RTP
 *******************************************************************/
#define MAX_TRIM_DEPTH 5

struct FIELDS_INFO {
        int                 level;
        int                 pin_fld_id;
        int                 fld_type;
        int                 record_id;
        int                 next_idx;
};

struct EVENT_FIELDS_INFO {
        char                event_type[192];
        int                 flags;
        struct FIELDS_INFO  *fields_info;
        int                 size_evt_match;
        int                 size_fields_info;
};

struct EVENT_ACCOUNT_INFO {
        char                event_type[192];
        int                 flags;
        pin_flist_t         *flistp;
        int                 size_evt_match;
};

struct EVENT_SERVICE_INFO {
        char                event_type[192];
        int                 flags;
        pin_flist_t         *flistp;
        int                 size_evt_match;
};

extern int                             fm_act_trim_add_req;
extern struct EVENT_ACCOUNT_INFO       *fm_act_trim_evt_acc_info;
extern int                             fm_act_trim_evt_acc_info_cnt;
extern struct EVENT_SERVICE_INFO       *fm_act_trim_evt_ser_info;
extern int                             fm_act_trim_evt_ser_info_cnt;

extern struct EVENT_FIELDS_INFO        *fm_act_trim_info;
extern int                             fm_act_trim_info_cnt;
extern int                             fm_act_trim_using_drop;
extern int                             bill_time_discount_when;

/*******************************************************************
 * Collections definitions taken from fm_collections.h and others
 *******************************************************************/
#define PIN_EVENT_DESCR_COLLECTIONS_ENTER           "Entered collections"
#define PIN_EVENT_DESCR_COLLECTIONS_EXIT            "Exited collections"
#define PIN_EVENT_DESCR_COLLECTIONS_ACTION_PENDING  "Status: Pending"
#define PIN_EVENT_DESCR_COLLECTIONS_UPDATING_ACTION "Updating: Action"
#define PIN_EVENT_DESCR_COLLECTIONS_CREATING_ACTION "Creating: Action"
#define PIN_EVENT_DESCR_COLLECTIONS_P2P		    "Promise to Pay"	 
#define PIN_EVENT_DESCR_COLLECTIONS_REPLACEMENT	    "Scenario Replacement"
#define PIN_ACTION_DESCR_CREATE		    	    "Action created"
#define PIN_ACTION_DESCR_CANCELED		    "Action canceled"
#define PIN_EVENT_DESCR_BILLINFO_EXEMPTED_COLLECTIONS "Billinfo Exempted From Collections"

#define PIN_ACTION_DESCR_RESCHEDULE                 "Action rescheduled"
/*******************************************************************
 * NewsFeed Reason Ids for Localization
 *******************************************************************/
#define PIN_NEWSFEED_REASON_DOMAIN_ID               60

/* Newsfeed Types/Sub-Types */
#define PIN_REASON_ID_TYPE_PAYMENT                  100
#define PIN_REASON_ID_TYPE_ADJUSTMENT               101
#define PIN_REASON_ID_TYPE_WRITEOFF                 102
#define PIN_REASON_ID_TYPE_NCR_ADJUSTMENT           103
#define PIN_REASON_ID_TYPE_PURCHASE                 104
#define PIN_REASON_ID_TYPE_CANCEL                   105
#define PIN_REASON_ID_TYPE_REFUND                   106
#define PIN_REASON_ID_TYPE_DISPUTE_OPEN             107
#define PIN_REASON_ID_TYPE_DISPUTE_CLOSED           108
#define PIN_REASON_ID_TYPE_COLLECTIONS              109
#define PIN_REASON_ID_TYPE_PYMT_REVERSAL            131
#define PIN_REASON_ID_TYPE_ALLOCATION               134
#define PIN_REASON_ID_TYPE_PYMT_MOVED_TO_SUSP       150
#define PIN_REASON_ID_TYPE_PYMT_SUSP_TO_THIS_ACC    151
#define PIN_REASON_ID_TYPE_WRITE_OFF_REVERSAL       160

#define PIN_REASON_ID_TYPE_NAMEINFO                 171
#define PIN_REASON_ID_TYPE_ACCOUNT_STATUS           172
#define PIN_REASON_ID_TYPE_DEFERRED_CREATE          177
#define PIN_REASON_ID_TYPE_DEFERRED_EXECUTE         178
#define PIN_REASON_ID_TYPE_PYMT_METHOD              179
#define PIN_REASON_ID_TYPE_FREQUENCY_CHANGE         188
#define PIN_REASON_ID_TYPE_DOM_CHANGE               189
#define PIN_REASON_ID_TYPE_ACTGTYPE_CHANGE          190
#define PIN_REASON_ID_TYPE_BILLUNIT_CREATED         192
#define PIN_REASON_ID_TYPE_BILLUNIT_DELETED         193
#define PIN_REASON_ID_TYPE_PYMT_CHANGE              194
#define PIN_REASON_ID_TYPE_BALGRP_TRANSFER          195

#define PIN_REASON_ID_TYPE_SERVICE_STATUS           211
#define PIN_REASON_ID_TYPE_SERVICE_DEVICE           212

#define PIN_REASON_ID_TYPE_CORRECTIVE_BILL          231
#define PIN_REASON_ID_TYPE_RECURRING_CHRG           232
#define PIN_REASON_ID_TYPE_BILL_ISSUED              233
#define PIN_REASON_ID_TYPE_BILL_ISSUED_MIDCYCLE     234
#define PIN_REASON_ID_TYPE_ONE_TIME_CHARGE          235

#define PIN_REASON_ID_TYPE_ITEM_ACTIVATED           241
#define PIN_REASON_ID_TYPE_ITEM_INACTIVATED         242
#define PIN_REASON_ID_TYPE_ITEM_TERMINATED          243
#define PIN_REASON_ID_TYPE_PACKAGE_TRANSITION       246
#define PIN_REASON_ID_TYPE_BUNDLE_TRANSITION        247

#define PIN_REASON_ID_TYPE_CREDIT_LIMIT_UPDATED     255
#define PIN_REASON_ID_TYPE_THRESHOLD_UPDATED        256

#define PIN_REASON_ID_TYPE_GROUP_CREATE             275 
#define PIN_REASON_ID_TYPE_GROUP_MODIFY             276
#define PIN_REASON_ID_TYPE_GROUP_DELETE             277
#define PIN_REASON_ID_TYPE_GROUP_DELETE_MEMBER      278
#define PIN_REASON_ID_TYPE_GROUP_ADD_MEMBER         279
#define PIN_REASON_ID_TYPE_ADDED_TO_GROUP           280
#define PIN_REASON_ID_TYPE_DELETED_FROM_GROUP       281

/* NewsFeed Details */
#define PIN_REASON_ID_FULLY_ALLOCATED               110
#define PIN_REASON_ID_UNALLOCATED                   111
#define PIN_REASON_ID_PARTIALLY_ALLOCATED           112
#define PIN_REASON_ID_WRITEOFF_BILL                 113
#define PIN_REASON_ID_WRITEOFF_ACCOUNT              114
#define PIN_REASON_ID_NCR_MULTI_BAL_IMPACT          115
#define PIN_REASON_ID_REFUND_ISSUED                 116
#define PIN_REASON_ID_REFUND_INITIATED              117
#define PIN_REASON_ID_DISPUTE_OPEN_BILL             118
#define PIN_REASON_ID_DISPUTE_OPEN_ITEM             119
#define PIN_REASON_ID_DISPUTE_OPEN_EVENT            120
#define PIN_REASON_ID_DISPUTE_SETTLED_BILL          121
#define PIN_REASON_ID_DISPUTE_SETTLED_ITEM          122
#define PIN_REASON_ID_DISPUTE_SETTLED_EVENT         123
#define PIN_REASON_ID_FULLY_GRANTED                 124
#define PIN_REASON_ID_PARTIALLY_GRANTED             125
#define PIN_REASON_ID_DENIED                        126
#define PIN_REASON_ID_COLLECTIONS_ENTER             127
#define PIN_REASON_ID_COLLECTIONS_OVERDUE           128
#define PIN_REASON_ID_COLLECTIONS_EXIT              129
#define PIN_REASON_ID_MULTI_BAL_IMPACT              130
#define PIN_REASON_ID_PYMT_REVERSAL                 132
#define PIN_REASON_ID_ORIG_PYMT                     133
#define PIN_REASON_ID_ALLOCATED                     135
#define PIN_REASON_ID_FULLY_ALLOCATED_TO_BILL       136
#define PIN_REASON_ID_FULLY_ALLOCATED_TO_ITEM       137
#define PIN_REASON_ID_FULLY_ALLOCATED_TO_EVENT      138
#define PIN_REASON_ID_PARTIALLY_ALLOCATED_TO_ITEM   139
#define PIN_REASON_ID_PARTIALLY_ALLOCATED_TO_EVENT  140
#define PIN_REASON_ID_PARTIALLY_ALLOCATED_TO_BILL   141
#define PIN_REASON_ID_WRITEOFF_ITEM                 142
#define PIN_REASON_ID_SETTLED_PARTIAL               143
#define PIN_REASON_ID_COLLECTIONS_ACTION_COMPLETED  144
#define PIN_REASON_ID_COLLECTIONS_ACTION_CANCELLED  145
#define PIN_REASON_ID_COLLECTIONS_ACTION_ERROR      146
#define PIN_REASON_ID_COLLECTIONS_ACTION_SKIPPED    147
#define PIN_REASON_ID_COLLECTIONS_TARGET_ALL        148
#define PIN_REASON_ID_COLLECTIONS_TARGET_CHILDREN   149
#define PIN_REASON_ID_PYMT_MOVED_FROM_SUSP          152
#define PIN_REASON_ID_PYMT_MOVED_TO_THIS            153 
#define PIN_REASON_ID_SHOW_PYMT_IN_SUSP             154
#define PIN_REASON_ID_PROMISE_TO_PAY_ADDED          155
#define PIN_REASON_ID_PROMISE_TO_PAY_REVOKED        156
#define PIN_REASON_ID_COLL_SCENARIO_REPLACED        157
#define PIN_REASON_ID_SCHEDULED               	    158
#define PIN_REASON_ID_PROMISE_TO_PAY_BREACHED       159
#define PIN_REASON_ID_WRITEOFF_REVERSED             161
#define PIN_REASON_ID_ORIG_WRITEOFF                 162
#define PIN_REASON_ID_COLLECTIONS_AGENT_ASSIGNED    163
#define PIN_REASON_ID_COLLECTIONS_ACTION_CREATED    164
#define PIN_REASON_ID_COLLECTIONS_ACTION_UPDATED    165
#define PIN_REASON_ID_COLL_BILLINFO_EXEMPTED        166
#define PIN_REASON_ID_COLL_BILLINFO_UNEXEMPTED      167
#define PIN_REASON_ID_FROM_UNASSIGNED               168
#define PIN_REASON_ID_TO_UNASSIGNED                 169
#define PIN_REASON_ID_ALLOCATED_TO_TMP_ITEM	    170

#define PIN_REASON_ID_DETAIL_CHANGE                 175
#define PIN_REASON_ID_ACCOUNT_STATUS                176
#define PIN_REASON_ID_INACTIVE_ACTIVE               180
#define PIN_REASON_ID_INACTIVE_CLOSED               181
#define PIN_REASON_ID_ACTIVE_INACTIVE               182
#define PIN_REASON_ID_ACTIVE_CLOSED                 183
#define PIN_REASON_ID_CLOSED_INACTIVE               184
#define PIN_REASON_ID_CLOSED_ACTIVE                 185
#define PIN_REASON_ID_UPDATED_PAYMENT               186
#define PIN_REASON_ID_NEW_PAYMENT                   187
#define PIN_REASON_ID_DELETE_PAYMENT                191
#define PIN_REASON_ID_TRANSITION_TYPE               196


#define PIN_REASON_ID_SERVICE_STATUS                213
#define PIN_REASON_ID_SERVICE_ATTACHED              214
#define PIN_REASON_ID_SERVICE_DETACHED              215

#define PIN_REASON_ID_ASSC_SRVC          	    244
#define PIN_REASON_ID_ASSC_PLAN          	    245
#define PIN_REASON_ID_LIMIT                         251
#define PIN_REASON_ID_FLOOR                         252
#define PIN_REASON_ID_THRESHOLD                     253
#define PIN_REASON_ID_THRESHOLDS_FIXED              254

#define PIN_REASON_ID_CHARGE_SHARE          	    271
#define PIN_REASON_ID_DISCOUNT_SHARE          	    272
#define PIN_REASON_ID_PROFILE_SHARE          	    273
#define PIN_REASON_ID_COLLECTIONS_GROUP             274

#define PIN_REASON_ID_INSTALLMENT_NUMBER            301
#define PIN_REASON_ID_INSTALLMENT_INTERVAL          302

#define PIN_REASON_ID_FROM_DETAILS                  992
#define PIN_REASON_ID_TO_DETAILS                    993
#define PIN_REASON_ID_MISC_TO		            994
#define PIN_REASON_ID_EXTERNAL_DOMAIN_STR           995
#define PIN_REASON_ID_MISC_FROM                     996
#define PIN_REASON_ID_MISC_BY                       997
#define PIN_REASON_ID_MISC_ON                       998
#define PIN_REASON_ID_MISC                          999

#define PIN_REASON_ID_PYMT_SUS_DOMAIN_STR "Reason codes-Payment Suspense Management"
#define PIN_REASON_ID_SLM_DOMAIN_STR "lifecycle_states"

#define OP_ACT_USAGE_LAST_SERVICE_OBJ "last_service"

//Device Objects
#define PIN_OBJ_TYPE_BASE_EVENT_DEVICE "/device/"

/******************************************************************
 * Definition for /request/ object types
 ******************************************************************/
#define PIN_OBJ_TYPE_REQUEST                   "/request"
#define PIN_OBJ_TYPE_REQUEST_FAILED            "/request/failed"
#define PIN_OBJ_TYPE_REQUEST_FAILED_OPCODE     "/request/failed/opcode"
#define PIN_OBJ_TYPE_REQUEST_FAILED_REST       "/request/failed/rest"

/*******************************************************************
 * Used to indicate the /request object status
 *******************************************************************/
typedef enum pin_record_request_status {
    PIN_RECORD_REQUEST_SUCCESS       = 0,
    PIN_RECORD_REQUEST_FAILED        = 1,
} pin_record_request_status_t;

/*******************************************************************
 * Used to indicate the completeness of paylod information in /request object
 *******************************************************************/
typedef enum pin_payload_request_status {
    PIN_PAYLOAD_REQUEST_COMPLETE     = 0,
    PIN_PAYLOAD_REQUEST_INCOMPLETE   = 1,
} pin_payload_request_status_t;

#endif   /*_PIN_ACT_H*/
