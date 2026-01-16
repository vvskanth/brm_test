/*******************************************************************
 *	
 * Copyright (c) 2000, 2023, Oracle and/or its affiliates.
 *
 *	This material is the confidential property of Oracle Corporation
 *	or its licensors and may be used, reproduced, stored or transmitted
 *	only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

/* 
 * NOTE: 
 *	If defining a new event type, also define it's corresponding
 *      event description.
 */


#ifndef _PIN_SUBSCRIPTION_H
#define	_PIN_SUBSCRIPTION_H

/*******************************************************************
 * Subscription FM Definitions.
 *
 * For the old data, refer pin_bill.h for other subscription mgmt
 * settings.
 *******************************************************************/

/*******************************************************************
 * PIN_FLD_FLAGS for PCM_OP_SUBSCRIPTION_TRANSITION_PLAN  
 *******************************************************************/
#define PIN_SUBS_TRANSITION_CONTROL_ROLLOVER	0x01
#define PIN_SUBS_TRANSITION_GEN_CHANGE		0x02

/*=================================================================*
 * Discount and discount instance related definitions.
 *=================================================================*/

/*******************************************************************
 * Object Type Strings (for discount action events)
 *******************************************************************/
#define	PIN_OBJ_TYPE_EVENT_DISCOUNT 		\
			"/event/billing/discount"
#define	PIN_OBJ_TYPE_EVENT_DISCOUNT_ACTION	\
			"/event/billing/discount/action"
#define	PIN_OBJ_TYPE_EVENT_DISCOUNT_ACTION_PURCHASE \
			"/event/billing/discount/action/purchase"
#define	PIN_OBJ_TYPE_EVENT_DISCOUNT_ACTION_CANCEL \
			"/event/billing/discount/action/cancel"
#define	PIN_OBJ_TYPE_EVENT_DISCOUNT_ACTION_MODIFY \
			"/event/billing/discount/action/modify"
#define	PIN_OBJ_TYPE_EVENT_DISCOUNT_ACTION_MODIFY_STATUS \
			"/event/billing/discount/action/modify/status"
/*******************************************************************
 * First usage Product/discount set validity events
 *******************************************************************/
#define PIN_OBJ_TYPE_EVENT_PRODUCT_SET_VALIDITY "/event/billing/product/action/set_validity"
#define PIN_OBJ_TYPE_EVENT_DISCOUNT_SET_VALIDITY "/event/billing/discount/action/set_validity"
#define PIN_EVENT_DESCR_SET_VALIDITY_SHADOW_EVENT     "set validity shadow event"
#define PIN_OBJ_TYPE_EVENT_SUB_BAL_SET_VALIDITY "/event/billing/sub_bal_validity"

/*******************************************************************
 * Events related to Promotion Name on Invoice 
 *******************************************************************/
#define PIN_OBJ_TYPE_EVENT_BUNDLE_CREATE "/event/billing/bundle/create"
#define PIN_OBJ_TYPE_EVENT_BUNDLE_MODIFY "/event/billing/bundle/modify"
#define PIN_EVENT_DESCR_SET_BUNDLE_INFO "Set Bundle Info"

/*******************************************************************
 * Discount event description strings.
 *******************************************************************/
#define PIN_EVENT_DESCR_PURCHASE_DISCOUNT	"Purchase Discount"
#define PIN_EVENT_DESCR_CANCEL_DISCOUNT		"Cancel Discount"
#define PIN_EVENT_DESCR_SET_DISCINFO		"Modified Discount"
#define PIN_EVENT_DESCR_SET_DISCOUNT_STATUS	"Modified Discount Status"

/***************************************************************************
 * First Usage product/Discount set validity Event descriptions
 ************************************************************************/
#define PIN_EVENT_DESCR_SET_FU_PRODUCT_VALIDITY	 	" First Usage Product Validity"
#define PIN_EVENT_DESCR_SET_FU_DISCOUNT_VALIDITY	" First Usage Discount Validity"

/*******************************************************************
 * Settings for billing time cycle discount and rollover events.
 *******************************************************************/
#define	PIN_OBJ_TYPE_EVENT_CYCLE_DISCOUNT	\
			"/event/billing/cycle/discount"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_DISC_MOSTCALLED	\
			"/event/billing/cycle/discount/mostcalled"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_ROLLOVER	\
			"/event/billing/cycle/rollover"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_ROLLOVER_TRANSFER	\
			"/event/billing/cycle/rollover_transfer"


#define PIN_EVENT_DESCR_CYCLE_DISCOUNT		"Cycle Discounts"
#define PIN_EVENT_DESCR_CYCLE_DISC_MOSTCALLED	"Most Called  Discounts"
#define PIN_EVENT_DESCR_CYCLE_ROLLOVER		"Cycle Rollover"
#define PIN_EVENT_DESCR_CYCLE_ROLLOVER_TRANSFER		"Rollover Transfer"

/*******************************************************************
 * Flags for status change of sharing group owner 
 *******************************************************************/
#define PIN_SUBS_STATUS_NOT_CHANGED		0
#define PIN_SUBS_STATUS_CHANGED			1

/*******************************************************************
 * Discount Validity Indices
 *******************************************************************/
#define PIN_DISCOUNT_INDEX_FROM_MIDDLE          0
#define PIN_DISCOUNT_INDEX_TO_MIDDLE            1
#define PIN_DISCOUNT_INDEX_ONLY_MIDDLE          2

/*******************************************************************
 * Discount Flags for Variable DiscountAction
 *******************************************************************/
#define PIN_DISCOUNT_PURCHASE_FLAG		1
#define PIN_DISCOUNT_CANCEL_FLAG		2
#define PIN_DISCOUNT_SET_INFO_FLAG		3
#define PIN_DISCOUNT_SET_INFO_CANCEL_FLAG	4


/*******************************************************************
 * PIN_FLD_TYPE - Type of discount
 *******************************************************************/
typedef enum pin_disc_type {
	PIN_DISC_TYPE_ITEM =	601,
	PIN_DISC_TYPE_ONGOING = 602,
	PIN_DISC_TYPE_SYSTEM =	603
} pin_disc_type_t;

/*******************************************************************
 * PIN_FLD_STATUS - Status of a discount
 *******************************************************************/
typedef enum pin_discount_status {
	PIN_DISCOUNT_STATUS_NOT_SET =	0,
	PIN_DISCOUNT_STATUS_ACTIVE =	1,
	PIN_DISCOUNT_STATUS_INACTIVE =	2,
	PIN_DISCOUNT_STATUS_CANCELLED =	3
} pin_discount_status_t;

/*******************************************************************
 * Flags for purchased bundle
 *******************************************************************/
typedef enum pin_bundle_status {
	PIN_BUNDLE_STATUS_NOT_SET =	0,
	PIN_BUNDLE_STATUS_ACTIVE =	1,
	PIN_BUNDLE_STATUS_INACTIVE =	2,
	PIN_BUNDLE_STATUS_CANCELLED =	3
} pin_bundle_status_t;

#define PIN_SUBS_DELINK_BUNDLE_OFFERINGS	0x1

/*******************************************************************
 * PIN_FLD_MODE - Mode of discount
 *******************************************************************/
typedef enum pin_disc_mode {
	PIN_DISC_MODE_PARALLEL	=	801,
	PIN_DISC_MODE_CASCADING	=	802,
	PIN_DISC_MODE_SEQUENTIAL =	803
} pin_disc_mode_t;

/*******************************************************************
 * PIN_FLD_APPLY_MODE - Operation on a resource 
 *******************************************************************/
typedef enum pin_apply_mode {
	PIN_APPLY_MODE_UNDEFINED =	0,
	PIN_APPLY_MODE_FOLD =		1
} pin_apply_mode_t;

/*******************************************************************
 * PIN_DISCOUNT_FLAGS - FLags of a discount rules 
 *******************************************************************/
typedef enum pin_discount_flags {
        PIN_DISCOUNT_FLAG_NA =         0,
        PIN_DISCOUNT_FLAG_FULL =       1,
        PIN_DISCOUNT_FLAG_PRORATE =    2,
        PIN_DISCOUNT_FLAG_NONE =       3
} pin_discount_flags_t;

/*******************************************************************
 * PCM_OP_BILL_CANCEL_DISCOUNT. ACTIONS for Cancel discount.
 *******************************************************************/
#define PIN_BILL_CANCEL_DISCOUNT_ACTION_CANCEL_ONLY     "cancel_only"
#define PIN_BILL_CANCEL_DISCOUNT_ACTION_CANCEL_DELETE   "cancel_delete"
#define PIN_BILL_CANCEL_DISCOUNT_ACTION_DONOT_CANCEL    "donot_cancel"

/*-----------------------------------------------------------------*
 * End Discount and Discount instance related definitions.
 *-----------------------------------------------------------------*/

/*******************************************************************
 * PIN_FLD_FLAGS - Indicates the validations to be performed by the  
 * PCM_OP_SUBSCRIPTION_VALIDATE_DISCOUNT_DEPENDENCY opcode. 
 *******************************************************************/
#define PIN_SUBS_FLG_DISC_DISC_DEP              0x1                                
#define PIN_SUBS_FLG_PLAN_DISC_DEP              0x2               
#define PIN_SUBS_FLG_DISABLE_PURCH_TIME         0x4                        
#define PIN_SUBS_FLG_RETURN_ON_FIRST_ERR        0x8


/*=================================================================*
 * Group Sharing, Ordered Balance group and Mapping Table related 
 * definitions.
 *=================================================================*/

/*******************************************************************
 * Mapping Table definitions.
 *******************************************************************/
/*        CALC_ONLY event types. not recorded */
#define	PIN_OBJ_TYPE_MAPPING_TABLE_CREATE	\
			"/event/billing/balgrp_map/create"
#define	PIN_OBJ_TYPE_MAPPING_TABLE_MODIFY	\
			"/event/billing/balgrp_map/modify"
#define	PIN_OBJ_TYPE_MAPPING_TABLE_DELETE	\
			"/event/billing/balgrp_map/delete"

#define MAPPING_TABLE_POID_TYPE		"/balgrp_map"
#define MAPPING_TABLE_PROGRAM_STR	"Mapping Table"
#define MAPPING_TABLE_TYPE_STR		"Mapping Table"

#define PIN_SUBS_MAPPING_TABLE_ACTION_CREATE   "Create"
#define PIN_SUBS_MAPPING_TABLE_ACTION_MODIFY   "Modify"
#define PIN_SUBS_MAPPING_TABLE_ACTION_DELETE   "Delete"

/*******************************************************************
 * Ordered Balance Group definitions.
 *******************************************************************/
#define	PIN_OBJ_TYPE_ORDERED_BALGRP_CREATE	\
			"/event/billing/ordered_balgrp/create"
#define	PIN_OBJ_TYPE_ORDERED_BALGRP_MODIFY	\
			"/event/billing/ordered_balgrp/modify"
#define	PIN_OBJ_TYPE_ORDERED_BALGRP_DELETE	\
			"/event/billing/ordered_balgrp/delete"

#define ORDERED_BALGROUP_POID_TYPE		"/ordered_balgrp"
#define ORDERED_BALGROUP_PROGRAM_STR		"Ordered Balance Group"
#define ORDERED_BALGROUP_TYPE_STR		"Ordered Balance Group"

#define PIN_SUBS_ORDERED_BALGRP_ACTION_CREATE   "Create"
#define PIN_SUBS_ORDERED_BALGRP_ACTION_MODIFY   "Modify"
#define PIN_SUBS_ORDERED_BALGRP_ACTION_DELETE   "Delete"
#define PIN_SUBS_ORDERED_BALGRP_ACTION_LIST     "List"

/*******************************************************************
 * Event Description Strings
 *******************************************************************/
#define PIN_EVENT_DESCR_ORDERED_BALGRP_CREATE	"Create Ordered Balance Group"
#define PIN_EVENT_DESCR_ORDERED_BALGRP_MODIFY	"Modify Ordered Balance Group"
#define PIN_EVENT_DESCR_ORDERED_BALGRP_DELETE	"Delete Ordered Balance Group"

/*******************************************************************
 * Group Sharing definitions.
 *******************************************************************/
#define PIN_OBJ_TYPE_DISCOUNTS_GROUP_CREATE	\
			"/event/group/sharing/discounts/create"
#define PIN_OBJ_TYPE_DISCOUNTS_GROUP_MODIFY	\
			"/event/group/sharing/discounts/modify"
#define PIN_OBJ_TYPE_DISCOUNTS_GROUP_DELETE	\
			"/event/group/sharing/discounts/delete"
#define PIN_OBJ_TYPE_PRODUCTS_GROUP_CREATE      \
			"/event/group/sharing/products/create"
#define PIN_OBJ_TYPE_PRODUCTS_GROUP_MODIFY      \
			"/event/group/sharing/products/modify"
#define PIN_OBJ_TYPE_PRODUCTS_GROUP_DELETE      \
			"/event/group/sharing/products/delete"
#define PIN_OBJ_TYPE_CHARGES_GROUP_CREATE	\
			"/event/group/sharing/charges/create"
#define PIN_OBJ_TYPE_CHARGES_GROUP_MODIFY	\
			"/event/group/sharing/charges/modify"
#define PIN_OBJ_TYPE_CHARGES_GROUP_DELETE	\
			"/event/group/sharing/charges/delete"
#define PIN_OBJ_TYPE_PROFILES_GROUP_CREATE	\
			"/event/group/sharing/profiles/create"
#define PIN_OBJ_TYPE_PROFILES_GROUP_MODIFY	\
			"/event/group/sharing/profiles/modify"
#define PIN_OBJ_TYPE_PROFILES_GROUP_DELETE	\
			"/event/group/sharing/profiles/delete"

#define PIN_EVENT_DESCR_DISCOUNTS_GROUP_CREATE	\
			"Create Discount Sharing Group"
#define PIN_EVENT_DESCR_DISCOUNTS_GROUP_MODIFY	\
			"Modify Discount Sharing Group"
#define PIN_EVENT_DESCR_DISCOUNTS_GROUP_DELETE	\
			"Delete Discount Sharing Group"
#define PIN_EVENT_DESCR_PRODUCTS_GROUP_CREATE   \
			"Create Product Sharing Group"
#define PIN_EVENT_DESCR_PRODUCTS_GROUP_MODIFY   \
			"Modify Product Sharing Group"
#define PIN_EVENT_DESCR_PRODUCTS_GROUP_DELETE   \
			"Delete Product Sharing Group"
#define PIN_EVENT_DESCR_CHARGES_GROUP_CREATE	\
			"Create Charge Sharing Group"
#define PIN_EVENT_DESCR_CHARGES_GROUP_MODIFY	\
			"Modify Charge Sharing Group"
#define PIN_EVENT_DESCR_CHARGES_GROUP_DELETE	\
			"Delete Charge Sharing Group"
#define PIN_EVENT_DESCR_PROFILES_GROUP_CREATE	\
			"Create Profile Sharing Group"
#define PIN_EVENT_DESCR_PROFILES_GROUP_MODIFY	\
			"Modify Profile Sharing Group"
#define PIN_EVENT_DESCR_PROFILES_GROUP_DELETE	\
			"Delete Profile Sharing Group"

#define SHARING_GROUP_CHARGES_TYPE_STR          "Sharing Charges Group"
#define SHARING_GROUP_DISCOUNTS_TYPE_STR        "Sharing Discounts Group"
#define SHARING_GROUP_PRODUCTS_TYPE_STR         "Sharing Products Group"
#define SHARING_GROUP_PROFILES_TYPE_STR         "Sharing Profiles Group"

#define SHARING_GROUP_CHARGES_PROGRAM_STR       "Sharing Charges"
#define SHARING_GROUP_DISCOUNTS_PROGRAM_STR     "Sharing Discounts"
#define SHARING_GROUP_PRODUCTS_PROGRAM_STR      "Sharing Products"
#define SHARING_GROUP_PROFILES_PROGRAM_STR      "Sharing Profiles"

#define GROUP_SHARING_POID_TYPE			"/group/sharing"
#define GROUP_SHARING_CHARGES_POID_TYPE		"/group/sharing/charges"
#define GROUP_SHARING_DISCOUNTS_POID_TYPE	"/group/sharing/discounts"
#define GROUP_SHARING_PRODUCTS_POID_TYPE        "/group/sharing/products"
#define GROUP_SHARING_PROFILES_POID_TYPE	"/group/sharing/profiles"

#define ORDERED_BALGRP_POSITION_TOP 	1	
#define ORDERED_BALGRP_POSITION_BOTTOM 	0

#define PIN_FLD_AUTO_DSG_TYPE_HIERARCHY         "H_DSG"
#define PIN_FLD_AUTO_DSG_TYPE_PAYMNT_RESP       "PR_DSG"
#define PIN_FLD_AUTO_PSG_TYPE_HIERARCHY         "H_PSG"
#define PIN_FLD_AUTO_PSG_TYPE_PAYMNT_RESP       "PR_PSG"

#define PIN_SUBS_FLG_TYPE_AUTO_DSG               0x01
#define PIN_SUBS_FLG_TYPE_AUTO_PSG               0x02

/*-----------------------------------------------------------------*
 * End Group Sharing, Ordered Balance group and Mapping Table related 
 * definitions.
 *-----------------------------------------------------------------*/

/*******************************************************************
 * Line management definitions.
 *******************************************************************/
#define PIN_OBJ_TYPE_EVENT_AUDIT_TRANSFER_SUBSCRIPTION	\
			"/event/audit/subscription/transfer"
#define PIN_OBJ_TYPE_EVENT_AUDIT_CANCEL_SUBSCRIPTION	\
			"/event/audit/subscription/cancel"
#define PIN_OBJ_NAME_EVENT_AUDIT 	"Audit event"

#define PIN_TRANSFER_NAME_TRANSFER		"transfer"
#define PIN_TRANSFER_RECREATE_NAME_TRANSFER	"recreate_transfer"
/*******************************************************************
 * Balance Monitoring definitions
 *******************************************************************/
#define PIN_FLD_MONITOR_TYPE_PAYMNT_RESP        "PR_CE"
#define PIN_FLD_MONITOR_TYPE_HIERARCHY          "H_CE"
#define PIN_FLD_MONITOR_TYPE_SUBSCRIPTION          "SUB_CE"
#define PIN_FLD_MONITOR_TYPE_PAYMNT_REALTIME_RESP        "PR_RTCE"

#define GROUP_SHARING_MONITOR_POID_TYPE         "/group/sharing/monitor"
#define SHARING_GROUP_MONITOR_PROGRAM_STR       "Sharing Monitor"
#define SHARING_GROUP_MONITOR_TYPE_STR          "Sharing Monitor Group"

#define PIN_OBJ_TYPE_MONITOR_GROUP_CREATE       \
                        "/event/group/sharing/monitor/create"
#define PIN_OBJ_TYPE_MONITOR_GROUP_MODIFY       \
                        "/event/group/sharing/monitor/modify"
#define PIN_OBJ_TYPE_MONITOR_GROUP_DELETE       \
                        "/event/group/sharing/monitor/delete"
#define PIN_OBJ_TYPE_MONITOR_UPDATE \
                        "/event/billing/monitor/update"

#define PIN_EVENT_DESCR_MONITOR_GROUP_CREATE       \
			"Create Monitor Sharing Group"
#define PIN_EVENT_DESCR_MONITOR_GROUP_MODIFY       \
			"Modify Monitor Sharing Group"                       
#define PIN_EVENT_DESCR_MONITOR_GROUP_DELETE       \
			"Delete Monitor Sharing Group"

/*******************************************************************
 * CR61, 1033, 1016 and 1035/36 definitions
 *******************************************************************/

#define PIN_OBJ_TYPE_EVENT_LC_UPDATE "/event/billing/lcupdate"
#define PIN_EVENT_DESCR_LC_UPDATE   "Line Counter Update"

#define PIN_OBJ_TYPE_EVENT_CDC_UPDATE "/event/billing/cdc_update"
#define PIN_EVENT_DESCR_CDC_UPDATE   "CDC Counter Update"

#define PIN_OBJ_TYPE_EVENT_CDCD_UPDATE "/event/billing/cdcd_update"
#define PIN_EVENT_DESCR_CDCD_UPDATE   "CDCD Counter Update"

#define PIN_OBJ_TYPE_EVENT_MFUC_UPDATE "/event/billing/mfuc_update"
#define PIN_EVENT_DESCR_MFUC_UPDATE   "MFUC Counter Update"

/*******************************************************************
* Best Pricing definitions
*******************************************************************/
#define PIN_OBJ_TYPE_EVENT_BEST_PRICING "/event/billing/best_pricing"
#define PIN_EVENT_DESCR_BEST_PRICING "Best Pricing Event"
#define PIN_EVENT_BEST_PRICING_PROGRAM_STR "Best Pricing"
#define PIN_EVENT_DESCR_BEST_PRICING_ADJUSTMENT "Best Pricing Adjustment Event"

/*******************************************************************
 * PIN_RERATE_FU_PRODUCTS - Transaction flist for first usage
 * products.
 *******************************************************************/
#define PIN_RERATE_FU_PRODUCTS "PIN_RERATE_FU_PRODUCTS"

/*******************************************************************
 * PIN_OFFER_OVERRIDE_FLAGS - Flags for PCM_OP_SUBSCRIPTION_SET_PRODINFO 
 and PCM_OP_SUBSCRIPTION_SET_DISCOUNTINFO
 *******************************************************************/
typedef enum pin_offer_override_flags {
        PIN_FLAG_UNDEFINED =           0,
        PIN_CREATE_DATE_RANGE =        1,
        PIN_UPDATE_TAGS =              2,
        PIN_DELETE_OFFER =             3,
        PIN_DELETE_DATE_RANGE =        4,
        PIN_DELETE_TAGS =              5,
        PIN_READ_OFFER =               6,
        PIN_MTA_READ_OFFER =           7,
	PIN_DELETE_USAGE_MAP =         8
} pin_offer_override_flags_t;

/*******************************************************************
 * PIN_FLD_OFFERING_FLAGS - Used by the GET_OFFERINGS OPCODE
 *******************************************************************/
#define PIN_SUBS_FLG_OFFERING_STATUS_ACTIVE             0x1
#define PIN_SUBS_FLG_OFFERING_STATUS_INACTIVE           0x2
#define PIN_SUBS_FLG_OFFERING_STATUS_CLOSED             0x4

/**********************************************************************
* NOTE: These flags will be deprecated after the new Caching
* Mechanism is implemented
 *******************************************************************/

#define PIN_SUBS_FLG_OFFERING_VALID_CYCLE               0x8
#define PIN_SUBS_FLG_OFFERING_VALID_PURCHASE            0x10
#define PIN_SUBS_FLG_OFFERING_VALID_USAGE               0x20
#define PIN_SUBS_FLG_OFFERING_INCLUDE_ACCT              0x40
#define PIN_SUBS_FLG_OFFERING_INCLUDE_SUBS              0x80
#define PIN_SUBS_FLG_BASE_PRODUCTS_ONLY                 0x100
#define PIN_SUBS_FLG_OVERRIDE_PRODUCTS_ONLY		0x200

/**********************************************************************
 * These are the new flags used by GET_PURCHASED_OFFERINGS
 * Opcode.
 **********************************************************************/
#define PIN_SUBS_FLG_OFFERING_VALIDITY_CYCLE		0x1
#define PIN_SUBS_FLG_OFFERING_VALIDITY_PURCHASE		0x2
#define PIN_SUBS_FLG_OFFERING_VALIDITY_USAGE		0x4

#define PIN_SUBS_FLG_ACCT_LEVEL_ONLY			0x1
#define PIN_SUBS_FLG_OVERRIDE_PRODS_ONLY		0x2

#define PIN_SUBS_FLG_INCLUDE_ALL_ELIGIBLE_PRODS		0x1
#define PIN_SUBS_FLG_INCLUDE_ALL_ELIGIBLE_DISCS		0x2
#define PIN_SUBS_FLG_INCLUDE_ALL_SUBSCRIPTION_SERVICES	0x4
#define PIN_SUBS_FLG_INCLUDE_SUBSCRIBER_CONTRACTS	0x8
#define PIN_SUBS_FLG_INCLUDE_OFFERING_OVERRIDES	        0x10

/*******************************************************************
 *Product/Discount  Flags for First usage cases, till 16 th bit of 
 * pin_fld_flags are defined, so iam using 18 th bit for this.
 * The other product related flags defined pin_bill.h 
 *******************************************************************/

#define PIN_SUBS_FLG_FIRST_USAGE             0x020000

/*******************************************************************
 * Validity Periods First usage defination
 *******************************************************************/

#define FIRST_USAGE_SET	-1

#define PIN_SUBS_VAL_OFFSET_ONE_CYCLE 1
#define PIN_SUBS_VAL_OFFSET_TWO_CYCLES 2

/*******************************************************************
 * Best Pricing Flag
 *******************************************************************/
#define PIN_SUBS_FLG_BEST_PRICING	0x1000000

/*******************************************************************
 * PIN_FLD_RERATE_REASON Definitions.
 * Portal Reserved Values: 100-120
 *******************************************************************/
/* Portal Reserved Values */
#define PIN_RERATE_REASON_MIN_RESERVED_VALUE            100
#define PIN_RERATE_REASON_MAX_RESERVED_VALUE            120

#define PIN_RERATE_REASON_BACKDATE_RESOURCE_GRANT       100
#define PIN_RERATE_REASON_BACKDATE_ERA_CREATE           101
#define PIN_RERATE_REASON_BACKDATE_ERA_MODIFY           102
#define PIN_RERATE_REASON_BACKDATE_DISC_CANCEL          103
#define PIN_RERATE_REASON_BACKDATE_PROD_CANCEL          104
#define PIN_RERATE_REASON_BACKDATE_DISC_PURCHASE        105
#define PIN_RERATE_REASON_BACKDATE_PROD_PURCHASE        106
#define PIN_RERATE_REASON_BACKDATE_DISC_STATUS_SET      107
#define PIN_RERATE_REASON_BACKDATE_PROD_STATUS_SET      108
#define PIN_RERATE_REASON_RATE_CHANGE                   109
#define PIN_RERATE_REASON_ROLLOVER_CORRECTION           110
#define PIN_RERATE_REASON_MONITOR_UPDATE                111
#define PIN_RERATE_REASON_ECE				112
#define PIN_RERATE_REASON_AUTO_JOB_PROD_CANCEL		113
/* End PIN_FLD_RERATE_REASON Definitions */

/****************************************************************
* Definitions for PCM_OP_SUBSCRIPTION_SERVICE_BALGRP_TANSFER
*******************************************************************/
#define PIN_OBJ_TYPE_EVENT_SERVICE_BALGRP_TRANSFER_START \
	"/event/notification/service_balgrp_transfer/start"
#define PIN_OBJ_TYPE_EVENT_SERVICE_BALGRP_TRANSFER_DATA \
	"/event/notification/service_balgrp_transfer/data"
#define PIN_OBJ_TYPE_EVENT_SERVICE_BALGRP_TRANSFER_END \
	"/event/notification/service_balgrp_transfer/end"
#define PIN_OBJ_TYPE_EVENT_AUDIT_SERVICE_BALGRP_TRANSFER  \
	"/event/audit/service_balgrp_transfer"

#define PIN_TRANSFER_PURCHASED_BUNDLE "transfer_purchased_bundle"
/**************************************************************************
 * PROGRAM_NAME used to identify application 'pin_cycle_fees'
 **************************************************************************/
#define CYCLE_FEES_PROG_NAME "pin_cycle"

/******************************************************************
 * Trans flist name
 ******************************************************************/
#define PIN_TRANS_NAME_RECAPPLY "rec_apply"
#define PIN_TRANS_NAME_GRP_NAME "group_name"
#define PIN_TRANS_NAME_GRP_INFO "group_info"

/**************************************************************************
 * Error messge with message id 
 **************************************************************************/
#define PIN_ERR_PRODUCT_EXPIRED		217 
#define PIN_ERR_DISCOUNT_EXPIRED 	218 
#define PIN_ERR_DEAL_EXPIRED		219
#define PIN_ERR_FUTURE_DATED_DEAL	220

/***************************************************************************
 * Info used while rerating to cut or Maintain buckets for cancelled/inact
***************************************************************************/
#define PIN_RERATE_CANCELLED_PROD_INFO               "cancelled_or_inact_bucket_handling_info"

/**************************************************************************
 * Definitions for PCM_OP_SUBSCRIPTION_CRUD_OFFER_OVERRIDE
 ***************************************************************************/
/* Object Name Strings */
#define PIN_OBJ_NAME_EVENT_OFFERING_OVERRIDE_CREATE	        "Creation of offering_override notify"
#define PIN_OBJ_NAME_EVENT_OFFERING_OVERRIDE_MODIFY             "Update of offering_override notify"
#define PIN_OBJ_NAME_EVENT_OFFERING_OVERRIDE_DELETE             "Deletion of offering_override notify"

/* Object Type Strings */
#define PIN_OBJ_TYPE_EVENT_NOTIFY_OFFERING_OVERRIDE_CREATE	"/event/notification/offering_override/create"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_OFFERING_OVERRIDE_MODIFY      "/event/notification/offering_override/modify"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_OFFERING_OVERRIDE_DELETE      "/event/notification/offering_override/delete"

/* Event Description Strings */
#define PIN_EVENT_DESCR_OFFERING_OVERRIDE_CREATE          "Creation of offering_override"
#define PIN_EVENT_DESCR_OFFERING_OVERRIDE_MODIFY          "Update of offering_override"
#define PIN_EVENT_DESCR_OFFERING_OVERRIDE_DELETE          "Deletion of offering_override"

/**************************************************************************
 * Definitions for PCM_OP_SUBSCRIPTION_NOTIFY_PRODUCT_EXPIRY
 ***************************************************************************/
/* Object Name Strings */
#define PIN_OBJ_NAME_EVENT_PRODUCT_EXPIRY               "Product Expiry Notification Log"
#define PIN_OBJ_NAME_EVENT_SUBSCRIPTION_RENEWAL_DUE	"Subscription Renewal Due Notification Log"
#define PIN_OBJ_NAME_EVENT_SUBSCRIPTION_RENEWAL               "Subscription Renewal Notification Log"

/* Object Type Strings */
#define PIN_OBJ_TYPE_EVENT_NOTIFY_PRODUCT_EXPIRY        "/event/notification/product/expiry"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_PRODUCT_POST_EXPIRY        "/event/notification/product/post_expiry"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_SUBSCRIPTION_RENEWAL_DUE	"/event/notification/subscription/renewal_due"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_SUBSCRIPTION_POST_RENEWAL_DUE      "/event/notification/subscription/post_renewal_due"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_SUBSCRIPTION_RENEWAL                "/event/notification/subscription/renewal"

/* Event Description Strings */
#define PIN_EVENT_DESCR_SUBSCRIPTION_RENEWAL_DUE	"Subscription Renewal is Due"
#define PIN_EVENT_DESCR_PRODUCT_EXPIRY                  "Product Expires"
#define PIN_EVENT_DESCR_SUBSCRIPTION_RENEWAL          "Subscription Renewal"

#define PIN_SUBS_FLG_SKIP_CYCLE_FEE				0x1000

/**************************************************************************
 *  Definitions for PCM_OP_SUBSCRIPTION_NOTIFY_SVC_LIFECYCLE_STATE_CHANGE
 ***************************************************************************/
/* Object Name Strings */
#define PIN_OBJ_NAME_EVENT_STATE_SERVICE_EXPIRY               "Service Lifecycle State Change Expiry Log"

/* Object Type Strings */
#define PIN_OBJ_TYPE_EVENT_NOTIFY_STATE_SVC_EXPIRY            "/event/notification/service/state_change/pre_expiry"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_STATE_SVC_POST_EXPIRY       "/event/notification/service/state_change/post_expiry"

#endif	/*_PIN_SUBSCRIPTION_H*/
