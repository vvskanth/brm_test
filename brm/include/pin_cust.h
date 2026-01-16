/*******************************************************************
 *      @(#)$Id: pin_cust.h /cgbubrm_omc.19.main.portalbase/2 2019/04/12 16:38:05 bmaturi Exp $ 
 *
 * Copyright (c) 1996, 2022, Oracle and/or its affiliates.
 *      
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreemenT
 *
 *******************************************************************/

#ifndef _PIN_CUST_H
#define	_PIN_CUST_H

/*******************************************************************
 * Needed for the pin_flist stuff down below
 *******************************************************************/
#include "pcm.h"
        
/*******************************************************************
 * Customer Management FM Definitions.
 *
 * All values defined here (except those for verify op) are
 * embedded in the database and therefore *cannot* change!
 *******************************************************************/

/*******************************************************************
 * OP Flags specific to the Customer FM (be compat with Billing FM).
 * (op specific flags go in high 16 bits)
 *******************************************************************/
#define PCM_OPFLG_CUST_REGISTRATION		0x01000000
#define PCM_OPFLG_CUST_CREATE_PAYINFO		0x02000000
#define PCM_OPFLG_CUST_DELETE_PAYINFO		0x04000000
#define PCM_OPFLG_CUST_CREATE_SERVICE           0x08000000
#define PCM_OPFLG_CUST_CREATE_ROLE              0x10000000

/*******************************************************************
 * Object Name Strings
 *******************************************************************/
#define PIN_OBJ_NAME_ACCOUNT		"PIN Account Object"
#define PIN_OBJ_NAME_SERVICE		"PIN Service Object"
#define PIN_OBJ_NAME_EVENT_CUST		"Customer Mngmt. Event Log"
#define PIN_OBJ_NAME_EVENT_NOTE		"Customer Mngmt. Note Log"
#define PIN_OBJ_NAME_PROFILE		"PIN Profile Object"
#define PIN_OBJ_NAME_PAYINFO		"PIN Payinfo Object"

/*******************************************************************
 * Object Type Strings
 *******************************************************************/
#define	PIN_OBJ_TYPE_EVENT_ACTGINFO	"/event/customer/actginfo"
#define	PIN_OBJ_TYPE_EVENT_CREATE_BILLINFO	"/event/customer/billinfo/create"
#define	PIN_OBJ_TYPE_EVENT_MODIFY_BILLINFO	"/event/customer/billinfo/modify"
#define	PIN_OBJ_TYPE_EVENT_DELETE_BILLINFO	"/event/customer/billinfo/delete"
#define	PIN_OBJ_TYPE_EVENT_LOCALE	"/event/customer/locale"
#define	PIN_OBJ_TYPE_EVENT_LOGIN	"/event/customer/login"
#define PIN_OBJ_TYPE_EVENT_NAMEINFO	"/event/customer/nameinfo"
#define	PIN_OBJ_TYPE_EVENT_NOTE_CREATE	"/event/customer/note/create"
#define	PIN_OBJ_TYPE_EVENT_NOTE_MODIFY	"/event/customer/note/modify"
#define	PIN_OBJ_TYPE_EVENT_PASSWORD	"/event/customer/password"
#define	PIN_OBJ_TYPE_EVENT_PAYINFO	"/event/customer/payinfo"
#define	PIN_OBJ_TYPE_EVENT_STATUS	"/event/customer/status"
#define PIN_OBJ_TYPE_EVENT_PRODUCT_STATUS "/event/customer/product_status"
#define PIN_OBJ_TYPE_EVENT_BRANDINFO	"/event/customer/brandinfo"
#define PIN_OBJ_TYPE_CONFIG_BUSINESS_TYPE "/config/business_type"
#define PIN_OBJ_TYPE_CONFIG_COUNTRY_CURRENCY_MAP "/config/country_currency_map"
#define PIN_OBJ_TYPE_CONFIG_PAYMENTTERM "/config/payment_term"
#define PIN_OBJ_TYPE_CONFIG_CUSTOMER_SEGMENT "/config/customer_segment"
#define PIN_OBJ_TYPE_CONFIG_BILLING_SEGMENT "/config/billing_segment" 

/*******************************************************************
 * Object Type Strings
 *******************************************************************/
#define	PIN_OBJ_AUDIT_CUSTOMER		"/event/audit/customer"
#define	PIN_OBJ_AUDIT_CUSTOMER_CC	"/event/audit/customer/payinfo/cc"
#define	PIN_OBJ_AUDIT_CUSTOMER_DD	"/event/audit/customer/payinfo/dd"
#define	PIN_OBJ_AUDIT_CUSTOMER_INVOICE	"/event/audit/customer/payinfo/invoice"
/*******************************************************************
 * Object Type Strings
 *******************************************************************/
#define	PIN_OBJ_TYPE_PAYINFO_CC		"/payinfo/cc"
#define	PIN_OBJ_TYPE_PAYINFO_DDEBIT	"/payinfo/ddebit"
#define	PIN_OBJ_TYPE_PAYINFO_DD		"/payinfo/dd"
#define	PIN_OBJ_TYPE_PAYINFO_INVOICE	"/payinfo/invoice"
#define	PIN_OBJ_TYPE_PAYINFO_SUBORD	"/payinfo/subord"
#define	PIN_OBJ_TYPE_PAYINFO_SEPA	"/payinfo/sepa"
#define PIN_OBJ_TYPE_ASSOCIATED_BUS_PROFILE    "/associated_bus_profile"
#define PIN_OBJ_TYPE_INVOICE_DETAIL    "/invoice_detail"

/*******************************************************************
 * Recid's for Old and New Data in an Event Log 
 *******************************************************************/
#define	PIN_ELEMID_OLD			0
#define PIN_ELEMID_NEW			1

/******************************************************************
 * Detail, Summary, Replacement, and Correction Letter Invoice Type Values
 *****************************************************************/
#define PIN_INV_TYPE_DETAIL_INVOICE	0
#define PIN_INV_TYPE_SUMMARY_INVOICE	1
#define PIN_INV_TYPE_REPLACEMENT_INVOICE   0
#define PIN_INV_TYPE_CORRECTION_LETTER	   1

#define PIN_INV_TYPE_OF_REGULAR_INVOICE    0x01
/* 0 indicates DETAIL, 1 - SUMMARY */
#define PIN_INV_TYPE_OF_CORRECTIVE_INVOICE 0x02
/* 0 indicates DETAIL, 1 - SUMMARY */
#define PIN_INV_CORRECTIVE_TYPE_TO_USE     0x04  
/* 0 indicates to use Replacement, 1 - Correction Letter */

/*******************************************************************
 * Event Description Strings
 *******************************************************************/
#define PIN_EVENT_DESCR_ACCTINFO	"Set Account Info"
#define PIN_EVENT_DESCR_BILLINFO	"Set Billing Info"
#define PIN_EVENT_DESCR_CREATE_BILLINFO	"Create Billing Info"
#define PIN_EVENT_DESCR_DELETE_BILLINFO	"Delete Billing Info"
#define PIN_EVENT_DESCR_BALGRPINFO	"Set Balance Group Info"
#define PIN_EVENT_DESCR_TOPUPRULES	"Set Topup Rules Info"
#define PIN_EVENT_DESCR_ACTGINFO	"Set Accounting Info"
#define PIN_EVENT_DESCR_LOCALE		"Set Locale"
#define PIN_EVENT_DESCR_LOGIN		"Set Login"
#define PIN_EVENT_DESCR_NAMEINFO	"Set Name Info"
#define PIN_EVENT_DESCR_PASSWORD	"Set Password"
#define PIN_EVENT_DESCR_PAYINFO		"Set Payinfo"
#define PIN_EVENT_DESCR_NOTEINFO	"Set Note"
#define PIN_EVENT_DESCR_STATUS		"Set Status"
#define PIN_EVENT_DESCR_PRODUCT_STATUS	"Set Product Status"
#define PIN_EVENT_DESCR_BRANDINFO	"Set Brand Info"

/******************************************************************
 * PIN_FLD_PARENT_FLAGS values in billinfo object
*******************************************************************/
#define PIN_FLD_TYPE_SINGLE_BILLINFO            0x0
#define PIN_FLD_TYPE_SUBORDINATE_BILLINFO       0x01
#define PIN_FLD_TYPE_PARENT_BILLINFO            0x02
#define PIN_FLD_TYPE_COLL_GROUP_MEM_BILLINFO    0x04
#define PIN_FLD_TYPE_COLL_GROUP_PARENT_BILLINFO 0x08

/******************************************************************
 * Defines for PCM_OP_CUST_MODIFY_BAL_GRP
 *******************************************************************/
#define PIN_CUST_FLAG_TRANSFER_ALL_ITEMS                1
#define PIN_CUST_FLAG_TRANSFER_CURRENT_CYCLE_ITEMS      2

/******************************************************************
* Defines for PCM_OP_CUST_DELETE_ACCT for ECE
*******************************************************************/
#define PIN_CUST_FORCE_ACCT_DELETE              1

/******************************************************************
* Defines for PCM_OP_CUST_DELETE_ACCT for events
*******************************************************************/
#define PIN_CUST_FLAG_DELETE                    0
#define PIN_CUST_FLAG_PRE_DELETE                1

/*******************************************************************
 * PCM_OP_CUST_STATUS fields.
 *******************************************************************/
/*
 * PIN_FLD_STATUS - The states of an account or service entry.
 */
typedef enum pin_status {
		/* defunct, faulty AAC creation - will be auto-cleansed */
	PIN_STATUS_DEFUNCT =	0,
		/* fully functional, login subject to credit limit check */
	PIN_STATUS_ACTIVE =	10100,
		/* inactive - no logins, no fees being charged */
	PIN_STATUS_INACTIVE =	10102,
		/* permanently closed - system resources released */
	PIN_STATUS_CLOSED =	10103,
		/* if this is used, all types of accounts will be fetched */
	PIN_STATUS_NOT_SET = 	10104,
		/* Is used to MARK for DELETION */
 	PIN_STATUS_TOBEDELETED = 10105			
} pin_status_t;

typedef enum pin_note_status {
	PIN_NOTE_STATUS_NOT_SET = 100,
	PIN_NOTE_STATUS_RESOLVED = 101,
	PIN_NOTE_STATUS_UNRESOLVED = 102
} pin_note_status_t;

/*
 * PIN_FLD_PASSWD_STATUS - Status of passwd
 */
typedef enum pin_serv_passwd_status {
        PIN_SERV_PASSWDSTATUS_NORMAL = 0,
        PIN_SERV_PASSWDSTATUS_TEMPORARY = 1,
        PIN_SERV_PASSWDSTATUS_EXPIRES = 2,
        PIN_SERV_PASSWDSTATUS_INVALID = 3
} pin_serv_passwd_status_t;

/*
 * PIN_FLD_STATUS_FLAGS
 * Each flag is a reason for the account/service entry to be inactive/closed.
 * Sponsorship related flags are necessary to determine the course of
 * action to be taken when a sponsor account is closed.
 */
	/* future activate date (acct only) */
#define	PIN_STATUS_FLAG_ACTIVATE		0x01
	/* outstanding debt (acct only) */
#define	PIN_STATUS_FLAG_DEBT			0x02
	/* via admin operator */
#define	PIN_STATUS_FLAG_MANUAL			0x04
	/* close all related services on account (acct only) */
#define PIN_STATUS_FLAG_DUE_TO_ACCOUNT		0x08
	/* close all sub-ordinate accounts if the parent is closed */ 
#define PIN_STATUS_FLAG_DUE_TO_PARENT		0x10
	/* via PO Mgmt System */
#define PIN_STATUS_FLAG_PO_EXHAUSTED		0x20
	/* via Provisioning System */
#define PIN_STATUS_FLAG_PROVISIONING		0x40
	/* delete sponsor group on sponsor closure */
#define PIN_STATUS_FLAG_DELETE_SPONSOR_GROUP	0x80
	/* cancel sponsored products and delete sponsor group */
#define PIN_STATUS_FLAG_CANCEL_SPONSORED_PRODUCTS 0x100
	/* reassign sponsor group to a new sponsor parent */
#define PIN_STATUS_FLAG_REASSIGN_SPONSOR_PARENT		0x200
        /* For Bertelsmann - indicated inactive until auth received */
#define PIN_STATUS_FLAG_BERTELSMANN             0x400
        /*  Provisioning Failed */
#define PIN_STATUS_FLAG_PROVISIONING_FAILED     0x800
        /* Unprovisioning in progress */
#define PIN_STATUS_FLAG_UNPROVISIONING          0x1000
	/* Change status(inactive/closed) for all services in the Plan 
	   if a Required Service is closed/inactivated */
#define PIN_STATUS_FLAG_DUE_TO_REQ_SRVC		0x2000
        /* During Transition of Plan, if any service in the "CLOSED" status
	   it's status_flag is set to this  */
#define PIN_STATUS_FLAG_DUE_TO_TRANSITION	0x4000
       /* Deactivate account's  products and services due to account suppression */
#define PIN_STATUS_FLAG_DUE_TO_ACCOUNT_SUPPRESSION  0x8000
	/* close all related services on billinfo */
#define PIN_STATUS_FLAG_DUE_TO_BILLINFO		0x10000
	/* Inactive/closed  member service due to line/subscription service */
#define PIN_STATUS_FLAG_DUE_TO_SUBSCRIPTION_SERVICE  0x20000
        /* Closed subscription service & member services due to line cancel*/
#define PIN_STATUS_FLAG_CANCEL_LINE  0x40000
        /* This flag is used to handle deferred action on servcies. Used in TCF provisioning*/ 
#define PIN_STATUS_FLAG_DEFERRED  0x80000
	/* This flag is used to handle subscription transfer with recreate approach*/
#define PIN_STATUS_FLAG_DUE_TO_SERVICE_TRANSFER  0x100000
	/* To remove PIN_STATUS_FLAG_MANUAL in case of SLM enabled services */
#define	PIN_STATUS_FLAG_RESET_MANUAL		0x200000
        /* Change status for all offerings due to CONTRACT */
#define PIN_STATUS_FLAG_DUE_TO_CONTRACT         0x400000

/*
 *	Flag values to indicate whether to fetch ITEM products and
 *	Cancelled+Deleted products or not. Used by op-codes 
 *	PCM_OP_CUST_READ_ACCT_PRODUCTS & PCM_OP_CUST_GET_HISTROY
 *	to retreieve tree structure of Account's Services/Deals/Products
 *	and get history for any instance of Service/Deal/Product 
 *	respectively. These products are to be retrieved from the
 *	events.
 */
	/* Fetch Item products for a given account */
#define	PIN_CUST_RAP_FETCH_ITEM_PRODUCTS		0x01

	/* Fetch Cancelled+Deleted products for a given account */
#define	PIN_CUST_RAP_FETCH_DELETED_PRODUCTS		0x02

	/* Fetch base purchased products only for a given account */
#define	PIN_CUST_RAP_FETCH_BASE_PRODUCTS_ONLY		0x04


        /*Case insensitive search*/
#define PIN_CUST_CASE_INSENSITIVE                       0x10

        /*Include sub services in the search */
#define PIN_CUST_INCLUDE_SUB_SERVICE                    0x20

        /* Include sub devices in the search */
#define PIN_CUST_INCLUDE_SUB_DEVICE                     0x40

        /* Include addresses in the search */
#define PIN_CUST_INCLUDE_CONTACT_INFO                   0x80

/*
 *      Flag values to pass to the opcode op_cust_delete_acct
 *      These values will indicate , whether the account should be deleted or
 *	only  marked for deletion . Also a flag is to be passed if the audit
 *	tables need to cleaned up for the account that is being deleted
*/


        /* Set this flag if the audit table entries for the account
		 needs to be deleted */
#define PIN_CUST_AUDIT_DELETE           0x10

        /* Set the flag if the ACCOUNT is ONLY to be MARKED for deletion */
#define PIN_CUST_MARK_DELETE_ACCT       0x20

/*
 *	DB Status for Account Creation
*/
	/* Database is open for new accounts */
#define PIN_DB_STATUS_OPEN		0x01
	/* Database is closed for new accounts on account of being full */
#define PIN_DB_STATUS_FULL		0x02
	/* Database is not available for reasons unknown */
#define PIN_DB_STATUS_UNAVAILABLE	0x04

/*
 *	Status of the Uniqueness Object
 */
	/* 
	 * This is the stat with which the uniqueness object is 
	 * first created.
	 */
#define	PIN_ACCOUNT_STATUS_UNCONFIRMED	0x01
	/* 
	 * This is the status with which the uniqueness object is 
	 * updated upon successful account creation
	 */
#define PIN_ACCOUNT_STATUS_CONFIRMED	0x02
	/* 
	 * This is the status with which the uniqueness object is 
	 * updated when account creation fails
	 */
#define PIN_ACCOUNT_STATUS_FAILED	0x04
	/* 
	 * This is the status with which the uniqueness object is 
	 * updated when account login is being modified 
	 */
#define PIN_ACCOUNT_LOGIN_BEING_CHANGED	0x08


/*******************************************************************
 * PCM_OP_CUST_BILLINFO fields.
 *******************************************************************/
/*
 * PIN_FLD_CURRENCY
 */

/* See pin_currency.h for a list of the currencies */

/***********************************************************
 * PIN_FLD_BILL_WHEN
 *
 * NOTE: In release 5.3 (and above), the enum pin_bill_when
 *       is no longer supported. 
 *       BILL_WHEN actually reflects the frequency of billing 
 *       in terms of number of accounting cycles. 
 *
 * THIS ENUM WILL BE REMOVED IN A FUTURE RELEASE. DO NOT 
 * USE THIS ANYMORE.
 */
typedef enum pin_bill_when {
	PIN_BILL_MONTHLY =	0,		/* not supported */
	PIN_BILL_ANNIVERSARY =	1,
	PIN_BILL_QUARTERLY =	3,
	PIN_BILL_YEARLY =	12
} pin_bill_when_t;

/* The bill_type field is obsolete. This is moved to pay_type defined
 * in the pin_pymt.h. Need to remove this after changing all the FM's
 * that reference bill_type.
 */
/*
 * PIN_FLD_BILL_TYPE
 *
 * If new values are added, they should be added to the
 * PCM_OP_CUST_POL_VALID_BILLINFO op as well.
 *
 */
typedef enum pin_bill_type {
	PIN_BILL_TYPE_UNDEFINED =   0,	/* invalid - used during acct create */
	PIN_BILL_TYPE_PREPAID = 10000,	/* prepaid to keep negative balance */
	PIN_BILL_TYPE_INVOICE = 10001,	/* invoiced monthly */
	PIN_BILL_TYPE_DEBIT =	10002,	/* checking account debit */
	PIN_BILL_TYPE_CC =	10003,	/* Credit Card */
	PIN_BILL_TYPE_DDEBIT =	10004,	/* French Direct Debit */
	PIN_BILL_TYPE_DD = 	10005,  /* US/Canadian Direct Debit */ 
	PIN_BILL_TYPE_SMARTC =	10006,	/* Smartcard */
	PIN_BILL_TYPE_SUBORD =	10007,	/* roll up to parent account */
	PIN_BILL_TYPE_BETA =	10008,	/* beta testers - ignored by billing */
	PIN_BILL_TYPE_INTERNAL = 10009,	/* internal employee, same as guest */
	PIN_BILL_TYPE_GUEST =	10010,	/* no charge, but credit limits apply */
	PIN_BILL_TYPE_CASH  =	10011,	/* cash payments for payment tool */
	PIN_BILL_TYPE_CHECK  =	10012,	/* check payments for payment tool */
	PIN_BILL_TYPE_WTRANSFER = 10013,/* wire transfer payments for payment tool */
	PIN_BILL_TYPE_PAYORDER = 10014,	/* Inter bank payment order payments for payment tool */
	PIN_BILL_TYPE_POSTALORDER = 10015/* Postal order payments for payment tool */
} pin_bill_type_t;

/*
 * PIN_FLD_BILLING_STATE
 *
 */
typedef enum pin_bill_billing_state {
	PIN_ACTG_CYCLE_OPEN   		= 0,
	PIN_ACTG_CYCLE_CLOSED 		= 1,
	PIN_ACTG_CYCLE_ALL_CHARGES_DONE = 2
} pin_bill_billing_state_t;

/*
 * PIN_FLD_BILLING_STATUS
 *
 */
typedef enum pin_bill_billing_status {
	PIN_BILL_ACTIVE   	 =	0,
	PIN_BILL_INACTIVE   	 =	1,
	PIN_BILL_ERROR		 = 	4	
} pin_bill_billing_status_t;

/*
 * PIN_FLD_BILLING_STATUS_FLAGS
 *
 */
#define	PIN_BILL_FLAGS_DUE_TO_STOP_BILL_CLOSE_ACCT		0x01
#define	PIN_BILL_FLAGS_DUE_TO_RESUME_BILLING 			0x02
#define PIN_BILL_FLAGS_DUE_TO_STOP_BILL_CLOSE_ACCT_SUBORD       0x04
#define PIN_BILL_FLAGS_DUE_TO_BILLING				0x08
#define PIN_BILL_FLAGS_DUE_TO_SUBORD_BILLING			0x10
#define PIN_BILL_FLAGS_DUE_TO_SPONSOREE_BILLING			0x20
#define PIN_BILL_FLAGS_DUE_TO_MIGRATION				0x40
#define PIN_BILL_FLAGS_DUE_TO_CUSTOMER_REQUEST			0x80
#define PIN_BILL_FLAGS_CF_ERROR					0x100
#define PIN_BILL_FLAGS_DEF_CF_ERROR				0x200
#define PIN_BILL_FLAGS_DEF_PURCHASE_ERROR			0x400
#define PIN_BILL_FLAGS_DEF_CANCEL_ERROR				0x800
#define PIN_BILL_FLAGS_CF_NOT_APPLIED				0x1000
#define PIN_BILL_FLAGS_UPDATE_JOURNALS_ERROR			0x2000
#define PIN_BILL_FLAGS_UPDATE_ITEMS_ERROR			0x4000
#define PIN_BILL_FLAGS_TRIAL_BILL_SUBORD_ERROR			0x8000
#define PIN_BILL_FLAGS_STOP_BILLING_PATCH_ONHOLD_STATE          0x10000
#define PIN_BILL_FLAGS_RESUME_BILLING_PATCH_INPROGRESS_STATE    0x20000

/*
 * PIN_FLD_INV_TERMS
 */
typedef enum pin_inv_terms {
	PIN_INV_TERMS_UNDEFINED =   0		/* invalid */
} pin_inv_terms_t;

/*
 * PIN_FLD_INV_FORMAT
 */
typedef enum pin_inv_format {
	PIN_INV_FORMAT_UNDEFINED =   0		/* invalid */
} pin_inv_format_t;

/*******************************************************************
 * PCM_OP_CUST_NAMEINFO fields.
 *******************************************************************/
/*
 * PIN_FLD_NAMEINFO element ids.
 *
 * The billing address should always exist.
 *
 * If necessary, other contacts are added to the name_address
 * array with non-significant recids.
 */
#define PIN_NAMEINFO_BILLING	 	1	/* billing address */
#define PIN_NAMEINFO_MAILING 		2	/* mailing address */

	/* Reserve 3-100 (?) for Portal use. */

/*
 * PIN_FLD_TYPE - type of the phone number in a PIN_FLD_PHONES element
 */
typedef enum pin_phone_type {
	PIN_PHONE_TYPE_UNKNOWN =	0,
	PIN_PHONE_TYPE_HOME =		1,
	PIN_PHONE_TYPE_WORK =		2,
	PIN_PHONE_TYPE_FAX =		3,
	PIN_PHONE_TYPE_PAGER =		4,
	PIN_PHONE_TYPE_PORTABLE =	5,
	PIN_PHONE_TYPE_POP =		6,
	PIN_PHONE_TYPE_SUPPORT =	7
} pin_phone_type_t;

/*
 * PIN_FLD_BUSINESS_TYPE - type of the accounts stored in PIN_FLD_BUSINESS_TYPE
 */
typedef enum pin_business_type {
	PIN_BUSINESS_TYPE_UNKNOWN =	0,
	PIN_BUSINESS_TYPE_CONSUMER =	1,
	PIN_BUSINESS_TYPE_BUSINESS =	2
} pin_business_type_t;

/*******************************************************************
 * /nameinfo field PIN_FLD_CONTACT_TYPE
 * Indicates type of address information
 * Delivery: Address where service delivered or product shipped to
 * Shipped From: Source address product shipped from
 *******************************************************************/
#define PIN_CONTACT_TYPE_DELIVERY "Delivery"
#define PIN_CONTACT_TYPE_SHIPPED_FROM "Shipped From"

/*******************************************************************
 * PCM_OP_CUST_POL_VALID_* fields.
 *******************************************************************/
/*
 * Net result of a field validation operation.
 */
#define	PIN_CUST_VERIFY_PASSED		PIN_BOOLEAN_TRUE
#define	PIN_CUST_VERIFY_FAILED	 	PIN_BOOLEAN_FALSE

/*
 * Reasons for result from a cust_verify operation.
 */
typedef enum pin_cust_vrfy_reason {
	PIN_CUST_VERIFY_ACCT_INACTIVE =  1,
	PIN_CUST_VERIFY_ACCT_CLOSED =	 2,
	PIN_CUST_VERIFY_ACCT_PASSWD =	 3,
	PIN_CUST_VERIFY_SERV_INACTIVE =  4,
	PIN_CUST_VERIFY_SERV_CLOSED =	 5,
	PIN_CUST_VERIFY_SERV_PASSWD =	 6,
	PIN_CUST_VERIFY_NO_CREDIT =	 7, 
	PIN_CUST_VERIFY_ACTIVE_PAYINFO = 8,
	PCM_CUST_VERIFY_PENDING_REQ =    9
} pin_cust_vrfy_reason_t;

/*
 * Field Validation Error Codes
 */
typedef enum pin_cust_val_err {
	PIN_CUST_VAL_ERR_SYSTEM =	0,
	PIN_CUST_VAL_ERR_MISSING =	1,
	PIN_CUST_VAL_ERR_MIN_LEN =	2,
	PIN_CUST_VAL_ERR_MAX_LEN =	3,
	PIN_CUST_VAL_ERR_TYPE =		4,
	PIN_CUST_VAL_ERR_DUPE =		5,
	PIN_CUST_VAL_ERR_INVALID =	6,
	PIN_CUST_VAL_ERR_REJECT =	7,
	PIN_CUST_VAL_ERR_MAX	=	8,
	PIN_CUST_VAL_ERR_MIN	=	9,
	PIN_CUST_VAL_ERR_INCL	=	10,
	PIN_CUST_VAL_ERR_EXCL	=	11,
	PIN_CUST_VAL_ERR_FORMAT	=	12
} pin_cust_val_err_t;

/*******************************************************************
 * Tax related
 *******************************************************************/
/*
 * vaidation commands
 */
typedef enum pin_cust_tax_val_cmd {
	PIN_CUST_TAX_VAL_JUR 		=	1,
	PIN_CUST_TAX_VAL_VAT_CERT	=	2,
	PIN_CUST_TAX_VAL_EXEMPT		=	3
} pin_cust_tax_val_cmd_t;

/*
 * vaidation errors
 */
typedef enum pin_cust_tax_val_err {
        PIN_CUST_TAX_VAL_JUR_ERR 	=	1,
        PIN_CUST_TAX_VAL_VAT_CERT_ERR	=	2,
        PIN_CUST_TAX_VAL_EXEMPT_ERR	=	3,
        PIN_CUST_TAX_VAL_UNKNOWN_ERR	=	4
} pin_cust_tax_val_err_t;

/*
 * vaidation error messages
 */
#define PIN_CUST_TAX_VAL_JUR_ERR_MSG		"tax (jurisdiction) validation failed."
#define PIN_CUST_TAX_VAL_VAT_CERT_ERR_MSG	"tax (vat certificate) validation failed."
#define PIN_CUST_TAX_VAL_EXEMPT_ERR_MSG		"tax (exemptions) validation failed."
#define PIN_CUST_TAX_VAL_UNKNOWN_ERR_MSG	"tax (unknown error) validation failed."

/*
 * error messages
*/
#define PIN_CUST_MISSING_ARG_ERR_MSG	"Missing argument."
#define PIN_CUST_BAD_VALUE_ERR_MSG	"Bad value."
#define	PIN_CUST_DUPE_VALUE_ERR_MSG 	"Duplicate value."
#define PIN_CUST_REJECT_VALUE_ERR_MSG   "Change not allowed for this service"

/*
 * error messages with message id
*/
#define PIN_ERR_LOGIN_VALIDATION_FAIL			211  
#define PIN_ERR_INVALID_FUTURE_DOM 			212  
#define PIN_ERR_DEBTOR_INFO_MISSING 			213  
#define PIN_ERR_INVALID_ACCT_TYPE   			214  
#define PIN_ERR_ACCT_POID_NULL    			215  
#define PIN_ERR_INVALID_BAL_GRP				216  
#define PIN_ERR_OTHER_ACCT_NAMEINFO_POID		221  
#define PIN_ERR_WRONG_CONTACT_TYPE			222
#define PIN_ERR_INVALID_NAMEINFO			223
#define PIN_ERR_DUPLICATE_CONTACT_TYPE			224

/*******************************************************************
 * Define the node_location seperator for the account products array
 *******************************************************************/
#define PIN_CUST_NODE_LOCATION_SEPARATOR	':'

/*******************************************************************
 * Password management defines.
 *******************************************************************/
#define PIN_PWD_ENCRYPT_TYPE_CLEAR	"clear"
#define PIN_PWD_ENCRYPT_TYPE_MD5	"md5"

/*******************************************************************
 * PCM_OP_CUST_PREP/CREATE_CUSTOMER related fields.
 *******************************************************************/
/*
 * PIN_FLD_AAC_ACCESS values
 */
#define PIN_CUST_AAC_ACCESS_MECCAD	"meccad"
#define PIN_CUST_AAC_ACCESS_ADMIN	"admintool"


/*
 *  Host array types
 */
typedef enum pin_host_type {
	PIN_HOST_DNS =	1,
	PIN_HOST_SMTP =	2,
	PIN_HOST_NNTP =	3,
	PIN_HOST_POP =	4,
	PIN_HOST_CM =	5,
	PIN_HOST_DM =	6
} pin_host_type_t;

/*******************************************************************
 * PO Managment related fields.
 *******************************************************************/
#define PIN_CUST_PO_BAL_ELEMID		100001

typedef enum pin_po_types {
	PIN_PO_TYPE_NONE = 0,		/* No PO management	*/
	PIN_PO_TYPE_TIME = 1,		/* Time base PO		*/
	PIN_PO_TYPE_AMOUNT = 2,		/* Amount base PO	*/
	PIN_PO_TYPE_COMBO = 3		/* Time/Amount based	*/
} pin_po_types_t;

typedef enum pin_po_options {
	PIN_PO_OPTION_CONTINUE = 0,	/* PO exp has no affect */
	PIN_PO_OPTION_INACTIVATE = 1	/* PO exp inactivates acct */
} pin_po_options_t;

typedef enum pin_po_status {
	PIN_PO_NONE	= 0,			/* No PO 		*/
	PIN_PO_INIT	= 1,			/* New PO established 	*/
	PIN_PO_AMT_THRESHOLD_REACHED = 3,	/* PO Amt threshold crossed */ 
	PIN_PO_AMT_LIMIT_REACHED = 4,		/* PO Amt limit crossed */ 
	PIN_PO_TIME_THRESHOLD_REACHED = 5, 	/* PO time threshold crossed */
	PIN_PO_TIME_LIMIT_REACHED = 6		/* PO time expired	*/
} pin_po_status_t;

/***************************************************************
 * This struct will be used to hold string buffers so that we
 * can keep track of when to realloc more space
 ***************************************************************/
typedef struct pin_cust_buf {
	char	*bufp;
	int	end;
	int	space;
	int	chunk;
} pin_cust_buf_t;


/*
 * The Type of Account.
 */
typedef enum pin_account_type {
	PIN_ACCOUNT_TYPE_STANDARD 	= 1,	/* Normal account */
	PIN_ACCOUNT_TYPE_BRAND    	= 2,	/* Brand specifier */
	PIN_ACCOUNT_TYPE_SUBSCRIPTION   = 3	/* Subscription Account */
} pin_account_type_t;

/* 
 * Various customer creation flags 
 */
#define PIN_FLAG_CUST_CREATE_BILLING_GROUP	0x01

/*
 * Accouting Cycle Length 
 */
typedef enum pin_actg_len {
	PIN_ACTG_CYCLE_SHORT =   0,	/* short cycle */
	PIN_ACTG_CYCLE_LONG  =   1	/* long  cycle */
} pin_actg_len_t;

/************************************************************************
 * Definitions used to validate zipcodes, phone numbers, salutations
 * and states.
 ************************************************************************/
#define PIN_NOID        -999999
#define ZIP_FORMAT      "[0-9]\\{5,\\}|[0-9]\\{5,\\}-[0-9]\\{4,\\}"
#define PHONE_FORMAT    "+[0-9]* ([0-9]\\{1,3\\}) [0-9]*[. -][0-9]*|+[0-9]* [0-9]*[. -][0-9]*"
#define SALUTATION_VALS "Mr\\.|Mrs\\.|Ms\\.|Dr\\."
#define SALUTATION_ERR  "Invalid salutation.  Valid values include: \n \
        (\"Mr.\",\"Mrs.\",\"Ms.\",\"Dr.\")."
#define ZIP_ERR         "Invalid Zipcode format (Valid formats: \n \
        NNNNN-NNNN or NNNNN)."
#define PHONE_ERR       "Invalid Phone# format. (Valid format: \n \
        + CountryCode Space [(AreaCode) Space] SubscriberNumber. \n \
         Area code is optional."

#define FM_CUST_POL             	"fm_cust_pol"
#define FM_STATEFILE_TOKEN      	"state_file"
#define FM_DEFAULT_STATEFILE    	"./state_file"

#define FM_CC_CHECKSUM_TOKEN		"cc_checksum"
#define FM_CC_CHECKSUM_FLAG_DEFAULT	1
#define FM_COUNTRY_TOKEN		"country"
#define FM_PAYINFO_INVOICE_DB_TOKEN	"invoice_db"
#define FM_DEFAULT_COUNTRY		"USA"

/* "flags" values for PIN_FLD_FLAGS in PCM_OP_CUST_VALID_FLD operation*/
	/* fld value required 				*/
#define	PIN_FLD_VALID_FLAG_REQUIRED		0x01
	/* inclusive/exclusive use of PIN_FLD_DOMAINS[]	*/
#define	PIN_FLD_VALID_FLAG_INCLUSIVE		0x02
	/* case sensitive check against PIN_FLD_DOMAINS[] */
#define	PIN_FLD_VALID_FLAG_CASE			0x04

/* 
 * As defined in fm_cust_pol_valid_all.c 
 */
#define EXPAND_FROMFILE         0
#define VALIDATE_INFILE         1
#define VALIDATE_NOTINFILE      2
#define MAXLINE                 80

/*******************************************************************
 * Deferred Action names
 *******************************************************************/
#define PIN_CUST_DEF_ACCOUNT_ACT "Account Activation"
#define PIN_CUST_DEF_ACCOUNT_DE_ACT "Account De-activation"
#define PIN_CUST_DEF_ACCOUNT_CLOSE "Account Closing"
#define PIN_CUST_DEF_SERVICE_ACT "Service Activation"
#define PIN_CUST_DEF_SERVICE_DE_ACT "Service De-activation"
#define PIN_CUST_DEF_SERVICE_CLOSE "Service Closing"
#define PIN_CUST_DEF_CHANGE_ACT_STATUS "Account Changing Status"
#define PIN_CUST_DEF_CHANGE_SER_STATUS "Service Changing Status"

#define PIN_CUST_DEF_ACT_SYS_DESCR "Change account status to"
#define PIN_CUST_DEF_SRVC_SYS_DESCR "Change service status to"

#define PIN_CUST_DEF_ACCOUNT_SUPPRESS_ACT "Place an account under suppression"
#define PIN_CUST_DEF_REMOVE_ACCOUNT_SUPPRESS_ACT "Remove an account out of suppression"

/*
 * Names of account statuses
 */
#define PIN_STATUS_NAME_ACTIVE		"Active"
#define PIN_STATUS_NAME_INACTIVE	"Inactive"
#define PIN_STATUS_NAME_CLOSED		"Closed"

/*
 * Nameinfo actions
 */
#define PIN_CUST_SET_NAMEINFO_ACTION_VALIDATE_ONLY	"validate_only"
 
 
 
/*******************************************************************
 * Forward define for map_country routine (used in multiple places
 *******************************************************************/
extern void
fm_cust_pol_map_country(
	pin_flist_t		*in_flistp,
	pin_flist_t		*r_flistp,
        char            	*country,
        pin_errbuf_t		*ebufp );
        

/* Status values :
 * These statuses are for Service Features and Service ERAs.
 */

typedef enum {
	PIN_STATUS_FEA_ERA_PROVISIONING_NOOP		= 0,
	PIN_STATUS_FEA_ERA_PROVISIONING			= 1,
	PIN_STATUS_FEA_ERA_PROVISIONING_FAILED		= 2,
	PIN_STATUS_FEA_ERA_ACTIVE			= 3,
	PIN_STATUS_FEA_ERA_UNPROVISIONING		= 4,
	PIN_STATUS_FEA_ERA_UNPROVISIONING_FAILED	= 5,
	PIN_STATUS_FEA_ERA_UNPROVISIONED		= 6,
	PIN_STATUS_FEA_ERA_SUSPENDING			= 7,
	PIN_STATUS_FEA_ERA_SUSPENDING_FAILED		= 8,
	PIN_STATUS_FEA_ERA_SUSPENDED			= 9
} feature_service_era_status_t;


/******************************************************************
 * Version Flags.
 ******************************************************************/
#define PIN_PORTAL_VERSION_CURRENT 0
#define PIN_PORTAL_VERSION_LEGACY 1

/******************************************************************
 * DOM Status Flags.
 ******************************************************************/
#define PIN_ACTG_DOM_STATUS_CLOSE 0
#define PIN_ACTG_DOM_STATUS_OPEN 1
#define PIN_ACTG_DOM_STATUS_RESTRICTED 2

/******************************************************************
 * Definition for business_profile/template/billinfo object types
 ******************************************************************/
#define PIN_OBJ_TYPE_CFG_BUSINESS_PROFILE    "/config/business_profile"
#define PIN_OBJ_TYPE_CFG_TEMPLATE_CHILDREN   "/config/template/%"
#define PIN_OBJ_TYPE_BILLINFO                "/billinfo"
#define PIN_OBJ_TYPE_CFG_TEMPLATE            "/config/template"

#define PIN_OBJ_TYPE_SERVICE                    "/service"
#define PIN_OBJ_TYPE_BALGRP                     "/balance_group"
#define PIN_OBJ_TYPE_BALGRP_MONITOR		"/balance_group/monitor"
#define PIN_OBJ_TYPE_ORDERED_BALGRP             "/ordered_balgrp"
#define PIN_OBJ_TYPE_GRP_SHARING                "/group/sharing/"
#define PIN_OBJ_TYPE_GRP_SHARING_CHARGES        "/group/sharing/charges"
#define PIN_OBJ_TYPE_GRP_SHARING_DISCOUNTS      "/group/sharing/discounts"
#define PIN_OBJ_TYPE_GRP_SHARING_PRODUCTS       "/group/sharing/products"
#define PIN_OBJ_TYPE_GRP_SHARING_PROFILES       "/group/sharing/profiles"
#define PIN_OBJ_TYPE_GRP_SHARING_MONITOR        "/group/sharing/monitor"
#define PIN_OBJ_TYPE_PURCHASED_PRODUCT          "/purchased_product"
#define PIN_OBJ_TYPE_PURCHASED_DISCOUNT         "/purchased_discount"
#define PIN_OBJ_TYPE_PROFILE_ACCT_EXTRA         "/profile/acct_extrating"
#define PIN_OBJ_TYPE_NAMEINFO                   "/nameinfo"

/******************************************************************
 * Definition for key-value pair of business_profile template for
 * Prepaid Postpaid Distinction feature.
 ******************************************************************/
#define PIN_CUST_CACHE_RESIDENCY    "CacheResidency"   /*for PIN_FLD_PAIR_KEY*/
#define PIN_CACHE_RES_TYPE_REALTIME "REALTIME"   /*prepaid PIN_FLD_PAIR_VALUE*/
#define PIN_CACHE_RES_TYPE_BATCH    "BATCH"      /*postpaid PIN_FLD_PAIR_VALUE*/
#define PIN_CACHE_RES_TYPE_DEFAULT  "DEFAULT"    /*convergent PIN_FLD_PAIR_VALUE*/
#define PIN_CACHE_RES_TYPE_DBONLY   "DBONLY"     /*nonusage PIN_FLD_PAIR_VALUE*/

/******************************************************************
 * Trans flist name 
 ******************************************************************/

#define PIN_TRANS_NAME_BEFORE_BALGRP_MODIFY "before_balgrp_modify"
#define PIN_TRANS_NAME_PACKAGE "package id"
#define PIN_TRANS_NAME_PSMACCT "psm_account"
#define PIN_TRANS_NAME_PAYINFO "payinfo"
#define PIN_TRANS_NAME_SCHEDULE "schedule"
#define PIN_TRANS_NAME_MODIFY_BILLINFO "modify_billinfo"

/************************************************************************
 * "flag" values for PIN_FLD_TXN_FLAGS used in PCM_OP_CUST_COMMI_CUSTOMER
 *************************************************************************/

#define PIN_TXN_FLAG_USE_LOCAL    0x01 /* local transaction */
#define PIN_TXN_FLAG_USE_GLOBAL   0x02 /* global transaction */

/************************************************************************
 * Set Package id to -1 when required to generate new one by subscription
 ************************************************************************/
#define PIN_PACKAGE_ID_NOT_SET -1

/* Valid values for PIN_FLD_TARGET_ACTION */
#define PIN_TARGET_ACTION_TO_ITSELF                0
#define PIN_TARGET_ACTION_TO_PARENT_AND_ALL_MEMBERS  1
#define PIN_TARGET_ACTION_TO_ALL_MEMBERS           2

 /**
 * PIN_FLD_EXEMPT_FROM_COLLECTIONS can have the following values *
 */
#define PIN_COLLECTIONS_EXEMPTED  0x01
#define PIN_COLLECTIONS_GROUP_MEMBER  0x02
#define PIN_COLLECTIONS_GROUP_PARENT  0x04
#define PIN_COLLECTIONS_REMOVE_BILLINFO_EXEMPTION 0x08

/* Valid values for PIN_FLD_MANDATE_AMENDED */
#define PIN_AMEND_FLG_MANDATE_ID	0x01
#define PIN_AMEND_FLG_DB_IBAN		0x02
#define PIN_AMEND_FLG_DB_BIC		0x04
#define PIN_AMEND_FLG_CREDITOR_ID	0x08
#define PIN_AMEND_FLG_CREDITOR_NAME 	0x10

/* Sepa Activity event type */
#define PIN_SEPA_ACTIVITY_MANDATE_AMEND "/event/activity/sepa/mandate_amendment"

/* Action update description strings */
#define PIN_ACTION_DESCR_AMEND "Mandate Amended"

/* Buffer size to store the description string */
#define SEPA_ACTION_DESCR_LEN 1024

#define PIN_OBJ_TYPE_EVENT_AUDIT_PAYINFO_SEPA "/event/audit/customer/payinfo/sepa"

#define PIN_FLG_MANDATE_CREATE 0
#define PIN_FLG_MANDATE_UPDATE 1

/************************************************************************
 * GET_NEWSFEED Result values
 ************************************************************************/
#define PIN_RESULT_NEWSFEED_PASS        0
#define PIN_RESULT_NEWSFEED_PARSE_FAIL  1
#define PIN_RESULT_NEWSFEED_FAIL        2

/************************************************************************
 * STATES for Subscriber Life Cycle Management 
 * The defined life cycle state OOB (101 - 109)
 * reserve 110 - 115
 * Custom Life Cycle state starts from 115 - 125
************************************************************************/
#define PRE_ACTIVE_STATE		101
#define ACTIVE_STATE			102
#define RECHARGE_ONLY_STATE		103
#define CREDIT_EXPIRED_STATE	        104
#define DORMANT_STATE			105
#define FRAUD_INVESTIGATED_STATE        106
#define SUSPENDED_STATE			107
#define CLOSED_STATE			108
#define SUSPENDED_ACTIVE		109

#define SUBS_PREF_NAME_LANGUAGE			"Language"
#define SUBS_PREF_NAME_CHANNEL			"Channel"
#define SUBS_PREF_NAME_NOTIFY_EXPIRY		"Notification Expiry"
#define SUBS_PREF_NAME_NOTIFY_INTERVAL		"Notification Interval"
#define SUBS_PREF_NAME_STREAM_THRSLD		"Streaming Threshold"
#define SUBS_PREF_NAME_STREAM_RESOURCE		"Resource For Threshold"
#define SUBS_PREF_NAME_NOTIFICATION_TIME		"Notification Time"
#define SUBS_PREF_NAME_PREFERRED_TIME		"PreferredTime"
#define SUBS_PREF_NAME_NOTIFY_OPT_OUT_LIST	"NotificationsOptOutList"
#define SUBS_PREF_NAME_EMAIL			"Email"
#define SUBS_PREF_NAME_PHONE			"Phone"
#define SUBS_PREF_NAME_NOTIFY_OPT_IN_LIST		"NotificationsOptInList"

#endif	/*_PIN_CUST_H*/
