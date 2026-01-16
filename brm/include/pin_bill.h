/* @(#)$Id: pin_bill.h /portalbase/2 2023/03/29 12:32:55 navrkaur Exp $ */
/*
 *
* Copyright (c) 2000, 2023, Oracle and/or its affiliates. 
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef _PIN_BILL_H
#define	_PIN_BILL_H

/*******************************************************************
 * Billing FM Definitions.
 *
 * All values defined here are embedded in the database
 * and therefore *cannot* change!
 *******************************************************************/

/*******************************************************************
 * OP Flags specific to the Billing FM.
 * (op specific flags go in high 16 bits)
 *******************************************************************/
#define PCM_BILLFLG_CLEANUP			0x0100000
#define PCM_BILLFLG_NO_LOG			0x0200000	/* DANGER */
#define PIN_BILLFLG_UPDATE_ITEM_FLDS		0x0400000	
#define PIN_BILLFLG_SELECT_FINAL		0x0800000	
#define PIN_BILLFLG_DEFER_ALLOCATION		0x1000000	

/*******************************************************************
 * Performance tuning business parameter
 *******************************************************************/
#define PIN_BILL_FLG_ITEM_NUMBER                0x01
#define PIN_BILL_FLG_TRANSFER_EVENT             0x02
#define PIN_BILL_FLG_ITEM_GL_EFFECTIVE          0x04
#define PIN_BILL_FLG_OPEN_BILL_NO_PREV_TOT      0x08
#define PERF_TUNING_NO_SUBORDS_LOCK_ON_PARENT_BILLING	0x10

/*******************************************************************
 * Non-currency resource journaling parameter
 *******************************************************************/
#define NCRESOURCE_JOURNALING_NONE		0x01
#define NCRESOURCE_JOURNALING_ALL		0x02
#define NCRESOURCE_JOURNALING_PLUS		0x04
#define NCRESOURCE_JOURNALING_MINUS		0x08

/*******************************************************************
 * Flag to indicate non-currency adjustment 
 * PIN_RATE_FLG_OVERRIDE_CREDIT_LIMIT flag(0x01) is already used
 * for PIN_FLD_FLAG value in PCM_OP_BILL_DEBIT opcode.
 *******************************************************************/

#define PIN_BILL_FLG_BILL_DEBIT_NC_ADJUSTMENT		0x02

/*******************************************************************
 * Modes of PCM_OP_BILL_GET_ITEM_EVENT_CHARGE_DISCOUNT opcode 
 *******************************************************************/
#define PIN_BILL_ANY_BAL_IMPACT_CHARGE		0
#define PIN_BILL_GROSS_CHARGE_IMPACT		1
#define PIN_BILL_NET_CHARGE_IMPACT		2
#define PIN_BILL_TOTAL_CHARGE_IMPACT		3

/*******************************************************************
 * Object Name Strings
 *******************************************************************/
#define PIN_OBJ_NAME_BILL 			"PIN Bill"
#define PIN_OBJ_NAME_BILL_ON_DEMAND 		"PIN Bill On Demand"
#define PIN_OBJ_NAME_BILL_NOW 			"PIN Bill NOW"
#define PIN_OBJ_NAME_CORRECTIVE_BILL		"PIN Corrective Bill"
#define PIN_OBJ_NAME_CORRECTIVE_BILL_NOW	"PIN Corrective Bill Now"
#define PIN_OBJ_NAME_CORRECTIVE_BILL_ON_DEMAND	"PIN Corrective Bill On Demand"
#define PIN_OBJ_NAME_DELAY_FIRST_BILL_NOW	"PIN Bill Delay First Bill NOW"
#define PIN_OBJ_NAME_DELAY_SECOND_BILL_NOW	"PIN Bill Delay Second BIll NOW"
#define PIN_OBJ_NAME_ACTG 			"PIN Accounting"
#define PIN_OBJ_NAME_EVENT_ACCOUNTING		"Accounting Summary"
#define PIN_OBJ_NAME_EVENT_BILL			"Billing Event Log"
#define PIN_OBJ_NAME_EVENT_INVOICE		"Billing Invoice"
#define PIN_OBJ_NAME_EVENT_SUMMARY		"Billing Summary"
#define PIN_OBJ_NAME_EVENT_ITEM			"Billing Change Item Event"
#define PIN_OBJ_NAME_EVENT_ACCOUNT		"Billing Change Account Event"
#define PIN_OBJ_NAME_ITEM_TRANSFER              "Item Transfer Notify Event"
#define PIN_OBJ_NAME_EVENT_ADJUSTMENT		"Event Adjustment"
#define PIN_OBJ_NAME_BILL_ADJUSTMENT		"Bill Adjustment"
#define PIN_OBJ_NAME_TAX_EVENT_ADJUSTMENT	"Tax Event Adjustment"
#define PIN_OBJ_NAME_TAX_ITEM_ADJUSTMENT	"Tax Item Adjustment"
#define PIN_OBJ_NAME_EVENT_BILL_WRITEOFF	"Bill Writeoff"
#define PIN_OBJ_NAME_EVENT_TAX_BILL_WRITEOFF	"Tax Bill Writeoff"
#define PIN_OBJ_NAME_EVENT_ITEM_WRITEOFF	"Item Writeoff"
#define PIN_OBJ_NAME_EVENT_TAX_ITEM_WRITEOFF	"Tax Item Writeoff"
#define PIN_OBJ_NAME_EVENT_ACCOUNT_WRITEOFF	"Account Writeoff"
#define PIN_OBJ_NAME_EVENT_TAX_ACCOUNT_WRITEOFF	"Tax Account Writeoff"
#define PIN_OBJ_NAME_EVENT_NOTIFY_ITEM_REFUND   "Refund Initiated"

#define BILLING_PROGRAM_NAME			"pin_bill_accts"
/*******************************************************************
 * Item Name Strings
 *******************************************************************/
#define PIN_OBJ_NAME_ITEM_USAGE 		"Usage"
#define PIN_OBJ_NAME_ITEM_SPONSOR 		"Sponsor"
#define PIN_OBJ_NAME_ITEM_CYCLE_FORWARD_ARREAR  "Cycle Forward Arrear"
#define PIN_OBJ_NAME_ITEM_CYCLE_FORWARD		"Cycle Forward"
#define PIN_OBJ_NAME_ITEM_CYCLE_ARREARS		"Cycle Arrears"
#define PIN_OBJ_NAME_ITEM_PURCHASE		"Purchase"
#define PIN_OBJ_NAME_ITEM_CANCEL		"Cancel"
#define PIN_OBJ_NAME_ITEM_PAYMENT		"Payment"
#define	PIN_OBJ_NAME_ITEM_REVERSAL		"Payment Reversal"
#define PIN_OBJ_NAME_ITEM_REFUND		"Refund"
#define PIN_OBJ_NAME_ITEM_DISPUTE		"Dispute"
#define PIN_OBJ_NAME_ITEM_SETTLEMENT		"Settlement"
#define PIN_OBJ_NAME_ITEM_ADJUSTMENT		"Adjustment"
#define PIN_OBJ_NAME_ITEM_WRITEOFF		"Write-off"
#define PIN_OBJ_NAME_ITEM_LOAN_CREDIT           "Loan Credit"
#define PIN_OBJ_NAME_ITEM_LOAN_DEBIT            "Loan Debit"
#define PIN_OBJ_NAME_ITEM_LOAN_FEE              "Loan Fee"
#define PIN_OBJ_NAME_ITEM_LOAN_LATE_FEE			"Loan Late Fee"
#define PIN_OBJ_NAME_ITEM_LOAN_RECOVERY			"Loan Recovery"

/*******************************************************************
 * Item Object Type Strings
 *******************************************************************/
#define	PIN_OBJ_TYPE_ITEM_USAGE			"/item/misc"
#define	PIN_OBJ_TYPE_ITEM_SPONSOR		"/item/sponsor"
#define PIN_OBJ_TYPE_ITEM_CYCLE_FORWARD_ARREAR  "/item/cycle_forward_arrear"
#define	PIN_OBJ_TYPE_ITEM_CYCLE_FORWARD		"/item/cycle_forward"
#define	PIN_OBJ_TYPE_ITEM_CYCLE_ARREARS		"/item/cycle_arrears"
#define	PIN_OBJ_TYPE_ITEM_PURCHASE		"/item/purchase"
#define	PIN_OBJ_TYPE_ITEM_CANCEL		"/item/cancel"
#define	PIN_OBJ_TYPE_ITEM_PAYMENT		"/item/payment"
#define	PIN_OBJ_TYPE_ITEM_REVERSAL		"/item/payment/reversal"
#define PIN_OBJ_TYPE_ITEM_REFUND		"/item/refund"
#define	PIN_OBJ_TYPE_ITEM_DISPUTE		"/item/dispute"
#define	PIN_OBJ_TYPE_ITEM_SETTLEMENT		"/item/settlement"
#define	PIN_OBJ_TYPE_ITEM_ADJUSTMENT		"/item/adjustment"
#define	PIN_OBJ_TYPE_ITEM_WRITEOFF		"/item/writeoff"
#define PIN_OBJ_TYPE_ITEM_WRITEOFF_REVERSAL  	"/item/writeoff_reversal"
#define PIN_OBJ_TYPE_ITEM_LOAN_CREDIT           "/item/loan_credit"
#define PIN_OBJ_TYPE_ITEM_LOAN_DEBIT            "/item/loan_debit"
#define PIN_OBJ_TYPE_ITEM_LOAN_FEE              "/item/loan_fee"
#define PIN_OBJ_TYPE_ITEM_LOAN_LATE_FEE              "/item/loan_late_fee"
#define PIN_OBJ_TYPE_ITEM_CYCLE_TAX             "/item/cycle_tax"

/*******************************************************************
 * Bill State Strings
 *******************************************************************/
#define PIN_BILL_STATE_UNDEFINED 		"Undefined"
#define PIN_BILL_STATE_NEW 			"New"
#define PIN_BILL_STATE_IN_PROGRESS 		"InProgress"
#define PIN_BILL_STATE_PARTIALLY_PAID 	        "PartiallyPaid"
#define PIN_BILL_STATE_SETTLED 		        "Settled"
#define PIN_BILL_STATE_ONHOLD 		        "OnHold"
#define PIN_BILL_STATE_SENT 		        "Sent"
#define PIN_BILL_STATE_VALIDATED 		"Validated"

/* Bill Due Notifications */
#define PIN_OBJ_NAME_EVENT_BILL_DUE 		"Bill Due Notification Log"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_BILL_DUE	"/event/notification/billing/due"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_BILL_POST_DUE "/event/notification/billing/post_due"
#define PIN_EVENT_DESCR_BILL_DUE		"Bill Due Notification"

/* Temporary Credit Limit Notifications */
#define PIN_OBJ_NAME_EVENT_TEMP_CREDIT_LIMIT  "Temporary Credit Limit Notification Log"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_TEMP_CREDIT_LIMIT  "/event/notification/billing/temp_limit"
#define PIN_EVENT_DESCR_TEMP_CREDIT_LIMIT "Temporary Credit Limit Notification"

#define PIN_OBJ_NAME_EVENT_TEMP_CREDIT_LIMIT_EXPIRY  "Temporary Credit Limit Expiry Notification Log"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_TEMP_CREDIT_LIMIT_EXPIRY  "/event/notification/billing/temp_limit/expiry"
#define PIN_EVENT_DESCR_TEMP_CREDIT_LIMIT_EXPIRY "Temporary Credit Limit Expiry Notification"

#define PIN_BILL_TEMPORARY_CREDIT_LIMIT_NOTIFY    "Generate an /event/notification/billing/temp_limit/expiry notification"
#define PIN_BILL_TEMPORARY_CREDIT_LIMIT_SYS_DESCR "Defer an /event/notification/billing/temp_limit/expiry notification when temporary credit limit expires"


/*******************************************************************
 * Item Status Flags
 *******************************************************************/
#define	PIN_ITEM_STATUS_PENDING			0x01
#define	PIN_ITEM_STATUS_OPEN			0x02
#define	PIN_ITEM_STATUS_CLOSED			0x04

/*******************************************************************
 * Item Event Search flags
 *******************************************************************/
#define	PIN_ITEM_EVENT_OWNED			0x01
#define	PIN_ITEM_EVENT_SPONSORED		0x02
#define	PIN_ITEM_EVENT_TRANSFERRED		0x04

/*******************************************************************
 * Item validation result codes.
 *******************************************************************/
#define	PIN_VALID_ITEM_STATUS_PENDING		0x02
#define	PIN_VALID_ITEM_STATUS_CLOSED		0x04
#define	PIN_VALID_ITEM_NO_TOTAL			0x08
#define	PIN_VALID_ITEM_NOT_IN_DISPUTE		0x10

/*******************************************************************
 * Corrective Bill validation results
 *******************************************************************/
#define PIN_BILL_VALIDATION_PASSED		0x0001
#define PIN_BILL_VALIDATION_FAILED		0x0002
#define PIN_BILL_VALIDATION_ONLY		0x0004
#define PIN_BILL_VALIDATION_NO_CHARGES		0x0008 
#define PIN_BILL_VALIDATION_AR_CHARGES_EXIST	0x0010
#define PIN_BILL_VALIDATION_FOR_AR_CHARGES_NEEDED 0x0020
#define PIN_BILL_VALIDATION_BILL_NOT_LAST	0x0040
#define PIN_BILL_VALIDATION_BILL_IS_FULLY_PAID	0x0080
#define PIN_BILL_VALIDATION_BILL_IS_PAID	0x0100
#define PIN_BILL_VALIDATION_AR_TOO_LOW		0x0200
#define PIN_BILL_VALIDATION_NOT_REPLACEMENT	0x0400
#define PIN_BILL_VALIDATION_FOR_PAYMENT_NEEDED	0x0800
#define PIN_BILL_VALIDATION_PAYMENTS_EXIST	0x1000
#define PIN_BILL_VALIDATION_NO_INVOICE		0x2000

/*******************************************************************
 * Invoice status error flags
 *******************************************************************/
#define PIN_INV_VALID_OK			0x00
#define	PIN_INV_INVOICES_NOT_FOUND		0x02
#define	PIN_INV_HANDLE_MISSING			0x04
#define	PIN_INV_TYPE_STR_MISSING		0x08
#define	PIN_INV_HANDLE_INVALID			0x10
#define	PIN_INV_TYPE_STR_INVALID                0x20

/* NSC - Additional flags for XML invoicing
         NOTE: currently re-using values from above,
               but should be save as NOT used together.
*/
#define PIN_INV_SEE_XML                         0x01
#define PIN_INV_STORE_XML                       0x02
#define PIN_INV_SEE_FLIST                       0x04

/*******************************************************************
 * Accounting types.
 *******************************************************************/
#define	PIN_ACTG_TYPE_OPEN_ITEMS		0x01
#define	PIN_ACTG_TYPE_BALANCE_FORWARD		0x02

/*******************************************************************
 * Bill types.
 *******************************************************************/
#define		PIN_BILL_FLG_BILL_NOW		0x01
#define		PIN_BILL_FLG_BILL_ON_DEMAND	0x02
#define		PIN_BILL_FLG_SKIPPED_BILL	0x04
#define		PIN_BILL_FLG_RETRY_BILL		0x08
#define         PIN_BILL_FLG_APPLY_FOLDS	0x10
#define         PIN_BILL_FLG_APPLY_DISCOUNTS    0x20   	
#define         PIN_BILL_FLG_AUTOTRIGGER	0x40
#define		PIN_BILL_FLG_CREATE_TWO_BILLS	0x800
#define         PIN_BILL_FLG_NEXT_BILL_NOW_BILL 0x100
#define		PIN_BILL_FLG_SETTLEMENT_ALLOC	0x200
#define		PIN_BILL_FLG_REFUND_ALLOC	0x400

/*******************************************************************
 * Flags which are used by the billing opcodes
 *******************************************************************/
/* 
 * It indicates that there are open items for the account
 */
#define PIN_BILL_FLG_OPEN_ITEM_EXIST		0x01
/* 
 * It indicates that the billing is provided inside of the delay interval
 */
#define PIN_BILL_FLG_DELAY_INTERVAL		0x02

/*******************************************************************
 * Flags used to indicate if DOM is incremented 
 *******************************************************************/
#define		PIN_BILL_FLG_DOM_NOT_INCREMENTED	0x00
#define		PIN_BILL_FLG_DOM_INCREMENTED		0x01

/*******************************************************************
 * G/L types.
 *******************************************************************/
#define	PIN_GL_TYPE_UNBILLED			0x01
#define	PIN_GL_TYPE_BILLED			0x02
#define	PIN_GL_TYPE_UNBILLED_EARNED		0x04
#define	PIN_GL_TYPE_BILLED_EARNED		0x08
#define	PIN_GL_TYPE_UNBILLED_UNEARNED		0x10
#define	PIN_GL_TYPE_BILLED_UNEARNED		0x20
#define	PIN_GL_TYPE_PREV_BILLED_EARNED		0x40
#define PIN_GL_TYPE_EARNED                      0x80
#define PIN_GL_TYPE_AGGREGATE                   0x100
#define PIN_GL_TYPE_PREVIEW_AGGREGATE           0x200

/*******************************************************************
 * G/L attributes.
 *******************************************************************/
#define	PIN_GL_ATTRIBUTE_NET			0x01
#define	PIN_GL_ATTRIBUTE_DISC			0x02
#define	PIN_GL_ATTRIBUTE_TAX			0x04
#define	PIN_GL_ATTRIBUTE_GROSS			0x08

/*******************************************************************
 * G/L account types
 *******************************************************************/
#define	PIN_GL_ACCT_TYPE_AR			0x01
#define	PIN_GL_ACCT_TYPE_OFFSET			0x02

/*******************************************************************
 * G/L reporting types
 *******************************************************************/
#define	PIN_GL_REPORT_TYPE_SUMMARY		0x00
#define	PIN_GL_REPORT_TYPE_DETAIL		0x01
#define	PIN_GL_REPORT_TYPE_CUSTOMIZED		0x02
#define	PIN_GL_REPORT_TYPE_INCREMENTAL		0x04

/*******************************************************************
 * G/L Frequency of reports
 *******************************************************************/
#define PIN_GL_REPORT_FREQUENCY_DAILY 		1
#define PIN_GL_REPORT_FREQUENCY_WEEKLY		2
#define PIN_GL_REPORT_FREQUENCY_MONTHLY		3
#define PIN_GL_REPORT_FREQUENCY_YEARLY		4
#define PIN_GL_REPORT_FREQUENCY_SPECIFIC_DATES	5
#define PIN_GL_REPORT_FREQUENCY_ONE_TIME	6

/*******************************************************************
 * G/L NON_Monetary Flags
 *******************************************************************/
#define PIN_GL_EXCLUDE_NON_MONETARY_RESOURCE	0
#define PIN_GL_INCLUDE_NON_MONETARY_RESOURCE    1

/*******************************************************************
 * G/L Element ID array of Reports
 *******************************************************************/
#define PIN_GL_ELEMENT_ID_BASIC_REPORT		0
#define PIN_GL_ELEMENT_ID_CUSTOMIZED_REPORT	1
#define PIN_GL_ELEMENT_ID_INCREMENTAL_REPORT	2

/*******************************************************************
 * G/L resource category
 *******************************************************************/
#define PIN_GL_RESOURCE_NON_CURRENCY_TYPE	0
#define PIN_GL_RESOURCE_CURRENCY_TYPE		1
#define PIN_GL_RESOURCE_ANY_TYPE		2
/*******************************************************************
 * G/L earnings type.
 *******************************************************************/
#define	PIN_EARNED_TYPE_IMMEDIATE		0x00
#define	PIN_EARNED_TYPE_PERIOD			0x01

/*******************************************************************
 * Object Type Strings
 *******************************************************************/
#define	PIN_OBJ_TYPE_BILL			"/bill"
#define	PIN_OBJ_TYPE_ACTG			"/accounting"
#define	PIN_OBJ_TYPE_ITEM			"/item"
#define	PIN_OBJ_TYPE_BASE_ITEM			"/item/"

#define	PIN_OBJ_TYPE_EVENT_ACCOUNTING		"/event/billing/accounting"
#define	PIN_OBJ_TYPE_EVENT_CHARGE		"/event/billing/charge"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_FOLD		"/event/billing/cycle/fold"
#define	PIN_OBJ_TYPE_EVENT_PURCHASE_FEE		"/event/billing/purchase_fee"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_TAX		"/event/billing/cycle/tax"
#define	PIN_OBJ_TYPE_EVENT_DEBIT		"/event/billing/debit"
#define	PIN_OBJ_TYPE_EVENT_DEBIT_ADJUSTMENT	"/event/billing/debit/adjustment"
#define	PIN_OBJ_TYPE_EVENT_DEAL			"/event/billing/deal"
#define	PIN_OBJ_TYPE_EVENT_DEAL_PURCHASE	"/event/billing/deal/purchase"
#define	PIN_OBJ_TYPE_EVENT_DEAL_CANCEL		"/event/billing/deal/cancel"
#define	PIN_OBJ_TYPE_EVENT_INVOICE		"/event/billing/invoice/invoice"
#define	PIN_OBJ_TYPE_EVENT_SUMMARY		"/event/billing/invoice/summary"
#define PIN_OBJ_TYPE_EVENT_AUDIT_ROLLOVER	"/event/audit/rollover"
#define PIN_OBJ_TYPE_EVENT_ROLLOVER_CORRECTION	"/event/billing/cycle/rollover_correction"

#define PIN_OBJ_TYPE_TMP_EVENTS_TO_PROCESS      "/tmp_events_to_process"
#define PIN_OBJ_TYPE_TMP_JOURNALS_TO_PROCESS    "/tmp_journals_to_process"

/*******************************************************************
 * Other Object Type Strings 
 *******************************************************************/
#define	PIN_OBJ_TYPE_EVENT_LIMIT		"/event/billing/limit"
#define	PIN_OBJ_TYPE_EVENT_CONSUMPTION		"/event/billing/consumption_rule"
#define	PIN_OBJ_TYPE_EVENT_ITEM_DISPUTE		"/event/billing/dispute/item"
#define	PIN_OBJ_TYPE_EVENT_ITEM_SETTLEMENT	"/event/billing/settlement/item"
#define	PIN_OBJ_TYPE_EVENT_ITEM_ADJUSTMENT	"/event/billing/adjustment/item"
#define	PIN_OBJ_TYPE_EVENT_ITEM_WRITEOFF	"/event/billing/writeoff/item"
#define	PIN_OBJ_TYPE_EVENT_TAX_ITEM_WRITEOFF	"/event/billing/writeoff/tax_item"
#define	PIN_OBJ_TYPE_EVENT_BILL_WRITEOFF	"/event/billing/writeoff/bill"
#define	PIN_OBJ_TYPE_EVENT_TAX_BILL_WRITEOFF	"/event/billing/writeoff/tax_bill"
#define	PIN_OBJ_TYPE_EVENT_ACCOUNT_WRITEOFF	"/event/billing/writeoff/account"
#define	PIN_OBJ_TYPE_EVENT_TAX_ACCOUNT_WRITEOFF	"/event/billing/writeoff/tax_account"
#define	PIN_OBJ_TYPE_EVENT_ITEM_TRANSFER	"/event/billing/item/transfer"
#define	PIN_OBJ_TYPE_EVENT_VALIDATE		"/event/billing/validate"
#define	PIN_OBJ_TYPE_EVENT_ADJUSTMENT		"/event/billing/adjustment/event"
#define PIN_OBJ_TYPE_TAX_EVENT_ADJUSTMENT	"/event/billing/adjustment/tax_event"
#define PIN_OBJ_TYPE_EVENT_ACCOUNT_ADJUSTMENT	"/event/billing/adjustment/account"
#define PIN_OBJ_TYPE_EVENT_BILL_ADJUSTMENT	"/event/billing/adjustment/bill"

#define PIN_OBJ_TYPE_EVENT_NOTIFY_ITEM_REFUND   "/event/notification/refund/create"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_ITEM_TRANSFER   "/event/notification/billing/item/transfer"

/*******************************************************************
 * Event Description Strings
 *******************************************************************/
#define PIN_EVENT_DESCR_CHARGE_VALIDATE		"Validation"
#define PIN_EVENT_DESCR_CHARGE_AUTH		"Authorization"
#define PIN_EVENT_DESCR_CHARGE_DEPOSIT		"Deposit"
#define PIN_EVENT_DESCR_CHARGE_CONDITION	"Conditional Deposit"
#define PIN_EVENT_DESCR_CHARGE_REFUND		"Refund"
#define PIN_EVENT_DESCR_CHARGE_REVERSE		"Reverse"

#define PIN_EVENT_DESCR_ACCOUNTING		"Accounting Folds"

#define PIN_EVENT_DESCR_DEBIT			"Debit"
#define PIN_EVENT_DESCR_CREDIT			"Credit"

#define PIN_EVENT_DESCR_LIMIT			"Set Credit Limit"

#define PIN_EVENT_DESCR_INVOICE			"Invoice"
#define PIN_EVENT_DESCR_SUMMARY			"Summary"

#define PIN_EVENT_DESCR_ITEM_DISPUTE		"Item Dispute"
#define PIN_EVENT_DESCR_ITEM_SETTLEMENT		"Item Settlement"
#define PIN_EVENT_DESCR_ITEM_ADJUSTMENT		"Item Adjustment"
#define PIN_EVENT_DESCR_ITEM_WRITEOFF		"Item Writeoff"
#define PIN_EVENT_DESCR_TAX_ITEM_WRITEOFF	"Tax Item Writeoff"
#define PIN_EVENT_DESCR_ITEM_TRANSFER		"Item Transfer"
#define PIN_EVENT_DESCR_PAYMENT			"Payment - Thank you"
#define PIN_EVENT_DESCR_REFUND                  "Refund Issued"
#define PIN_EVENT_DESCR_PAYMENT_REVERSAL	"Payment - Reversal"
#define PIN_EVENT_DESCR_EVENT_ADJUSTMENT	"Event Adjustment"
#define PIN_EVENT_DESCR_BILL_ADJUSTMENT		"Bill Adjustment"
#define PIN_EVENT_DESCR_TAX_EVENT_ADJUSTMENT	"Tax Event Adjustment"
#define PIN_EVENT_DESCR_TAX_ITEM_ADJUSTMENT	"Tax Item Adjustment"
#define PIN_EVENT_DESCR_ACCOUNT_ADJUSTMENT	"Event for Account Adjustment"
#define PIN_EVENT_DESCR_BILL_WRITEOFF		"Event for Bill Writeoff"
#define PIN_EVENT_DESCR_TAX_BILL_WRITEOFF	"Event for Tax Bill Writeoff"
#define PIN_EVENT_DESCR_ACCOUNT_WRITEOFF	"Event for Account Writeoff"
#define PIN_EVENT_DESCR_TAX_ACCOUNT_WRITEOFF	"Event for Tax Account Writeoff"
#define PIN_EVENT_DESCR_AUDIT_ROLLOVER		"Event for Rollover Correction"
#define PIN_EVENT_DESCR_ROLLOVER_CORRECTION	"Event for Rollover Correction"

#define PIN_EVENT_NOTIFY_DESCR_ITEM_REFUND      "Refund Initiated"

/*******************************************************************
 * Billing group related strings
 *******************************************************************/
#define BILLING_GROUP_TYPE_STR			"Billing Hierarchy"
#define BILLING_GROUP_TYPE_ACCOUNT		"/account"
#define BILLING_GROUP_TYPE_BILLING		"/group/hierarchy"
#define BILLING_GROUP_PROGRAM_STR		"Billing Group"
#define BILLING_GROUP_POID_TYPE			"/group/billing"
#define BILL_IN_PREVIOUS_CYCLE                  0
#define BILL_IN_CURRENT_CYCLE                   1


/*******************************************************************
 * Re-rating/Re-billing related.
 *******************************************************************/
#define PIN_BILL_RERATE_BY_END_T		1
#define PIN_BILL_RERATE_BY_CREATED_T		2
#define PIN_BILL_RERATE_BY_START_T              16
#define PIN_BILL_SEARCH_BY_END_T		4
#define PIN_BILL_SEARCH_BY_CREATED_T		8
#define	PIN_OBJ_TYPE_EVENT_RERATE_CONTROL	"/event/control_rerate"

/*******************************************************************
 * pin.conf related.
 *******************************************************************/
#define FM_BILL_POL			"fm_bill_pol"

/*******************************************************************
 * Sponsor group related strings
 *******************************************************************/
#define SPONSOR_GROUP_TYPE_STR			"Sponsorship group"
#define SPONSOR_GROUP_TYPE_ACCOUNT		"/account"
#define SPONSOR_GROUP_PROGRAM_STR		"sponsor"
#define SPONSOR_GROUP_POID_TYPE			"/group/sponsor"

/*******************************************************************
 * Sponsor flags
 * The new flag PIN_SPONSOR_FLG_FOLD is to determine whether it is
 * a fold being sponsored, and the other 2 flags are not used when
 * this flag is set because we don't do any guarantee checks and we
 * don't let the user select all folds too.
 *******************************************************************/
#define	PIN_SPONSOR_FLG_ALL_RATE_TYPES		0x1
#define	PIN_SPONSOR_FLG_GUARANTEED		0x2
#define PIN_SPONSOR_FLG_FOLD			0x4

/*
 * PIN_FLD_TYPE - Checking account type for ECP (direct debit) accts
 */
typedef enum pin_acct_types {
        PIN_ACCOUNT_TYPE_NONE =         0,
        PIN_ACCOUNT_TYPE_CHECKING =     1,
        PIN_ACCOUNT_TYPE_SAVINGS =      2,
        PIN_ACCOUNT_TYPE_CORPORATE =    3
} pin_acct_types_t;

/*
 * PIN_FLD_REVERSAL_RESULT - Result of the reversal.
 */
#define	PIN_REVERSE_RES_PASS		0
#define	PIN_REVERSE_RES_FAIL		1

/*
 * PIN_FLD_REVERSAL_RESULT_TYPE - Type of reversal result error.
 */
#define	PIN_REVERSE_TYPE_UNKNOWN_ERR		0x1
#define	PIN_REVERSE_TYPE_INVALID_PAYMENT	0x2
#define	PIN_REVERSE_TYPE_ALREADY_REVERSED	0x4
#define PIN_REVERSE_TYPE_INVALID_ACTIVE_PAYMENT 0x8

/*
 * Type of reversals.
 */

#define PIN_REVERSE_FLAG_REVERSE_AS_UNALLOCATED 0x01

/*
 * Different Sequence types. 
 * These values are used as element ids in the /data/sequence object.
 */
#define	PIN_SEQUENCE_TYPE_TRANS_ID	0
#define	PIN_SEQUENCE_TYPE_BILL_NO	1
#define	PIN_SEQUENCE_TYPE_PAYMENT	2
#define	PIN_SEQUENCE_TYPE_DISPUTE	3
#define	PIN_SEQUENCE_TYPE_ADJUSTMENT	4
#define	PIN_SEQUENCE_TYPE_SETTLEMENT	5
#define	PIN_SEQUENCE_TYPE_WRITEOFF	6
#define	PIN_SEQUENCE_TYPE_ACCOUNT	7
#define	PIN_SEQUENCE_TYPE_REFUND	8
#define	PIN_SEQUENCE_TYPE_FIAS_ID	9
#define	PIN_SEQUENCE_TYPE_PACKAGE_ID	10
#define	PIN_SEQUENCE_TYPE_CORR_BILL_NO  11 
#define PIN_SEQUENCE_TYPE_MANDATE_ID    12
#define	PIN_SEQUENCE_TYPE_SDD_MSG_ID    13
#define	PIN_SEQUENCE_TYPE_SCT_MSG_ID    14
#define	PIN_SEQUENCE_TYPE_REV_MSG_ID    15
#define	PIN_SEQUENCE_TYPE_PYMTINFO_ID   16
#define	PIN_SEQUENCE_TYPE_CONTRACT_NO   17
#define PIN_SEQUENCE_TYPE_BAL_EXT_ID   18
#define PIN_SEQUENCE_TYPE_SUB_BAL_EXT_ID   19
#define PIN_SEQUENCE_TYPE_PAYINFO_EXT_ID   20
#define PIN_SEQUENCE_TYPE_BILLINFO_EXT_ID   21
#define PIN_SEQUENCE_TYPE_EVENT_EXT_ID   22
#define PIN_SEQUENCE_TYPE_LOAN_ID   23


/*
 * type of failures during event adjustments
 */
#define	PIN_EVENT_ADJ_ERR_UNKNOWN	1	/* unknown error */
#define	PIN_EVENT_ADJ_ERR_REVERSED	2	/* already adjusted */
#define	PIN_EVENT_ADJ_ERR_INVALID	3	/* couldn't adjust */
#define	PIN_EVENT_ADJ_ERR_NOIMPACT	4	/* no balance impact */
#define	PIN_EVENT_ADJ_ERR_IRREVERSIBLE	5	/* event not found */

/*
 * PIN_FLD_FLAG to set adjustment
 */
#define PIN_EVENT_ADJ_NO_TAX		0x1
#define PIN_EVENT_ADJ_TAX_ONLY		0x2
#define PIN_EVENT_ADJ_EVENT		0x4
#define PIN_EVENT_ADJ_BATCH		0x8

#define PIN_EVENT_ADJ_FOR_RERATE	0x10

/*
 * PIN_FLD_FLAG values for AR to indicate Without Tax, With Tax or 
 * Tax Inclusive
 */
#define PIN_AR_WITHOUT_TAX				0x1
#define PIN_AR_WITH_TAX					0x2
#define PIN_AR_TAX_INCLUSIVE            		0x4
#define PIN_AR_TAX_ONLY_PERCENT				0x8
#define PIN_AR_DEBIT_AMOUNT_TAX_ONLY_ADJUSTMENT		0x10
#define PIN_AR_CREDIT_AMOUNT_TAX_ONLY_ADJUSTMENT	0x20
#define PIN_AR_TAX_PRERATED_EVENT			0x40
#define PIN_AR_CALC_PROP_TAX				0x80
#define PIN_AR_ADJ_DIS_GROSS				0x100


/*
 * A value to indicate pre-billing dispute and post-billing settlement.
 */

#define PRE_BILL_DISPUTE_POST_BILL_SETTLEMENT	0x1

/*
 * flags used to set command for adjustment opcodes
 */
#define PIN_BILL_ADJ_ESTIMATE_ONLY	1

/*******************************************************************
 * Modular billing opcode types.
 *******************************************************************/
typedef enum pin_billing_opcode {
	PIN_BILL_VALIDATE		= 0,
	PIN_BILL_CHARGE			= 1,
	PIN_BILL_RECOVER		= 2,
	PIN_BILL_REVERSAL		= 3
} pin_billing_opcode_t;

typedef enum pin_delivery_prefer {
       PIN_INV_EMAIL_DELIVERY          = 0,
       PIN_INV_USP_DELIVERY            = 1,
       PIN_INV_FAX_DELIVERY            = 2
} pin_delivery_prefer_t;


/*******************************************************************
 * Modular billing configuration structure.
 *******************************************************************/
typedef struct fm_utils_billing_conf {
	char		payinfo_type[256];
	char		payment_event_type[256];
	char		opcode_name[256];
	u_int		opcode;
	u_int		flags;
	u_int		event_type[256];
} fm_utils_billing_conf_t;	

/*******************************************************************
 * Billing Related Object Fields
 *******************************************************************/
/*
 * PIN_FLD_ZONE - which timezone to use: server (default) or user.
 */
typedef enum pin_zone {
	PIN_ZONE_SERVER=	0,
	PIN_ZONE_USER = 1
} pin_zone_t;

/*******************************************************************
 * Flag to indicate the delay specified at the deal level in the 
 * cycle should be aligned to Actg Cycle. 
 *******************************************************************/
#define PIN_BILL_ALIGN_TO_ACTG_CYCLE	1

/*******************************************************************
 * Bits on the PIN_FLD_FLAGS field added and used internally to the
 * input flist of OP_BILL_MAKE_BILL. These should not generally be
 * used from other places.
 *******************************************************************/
#define PIN_BILL_FLG_TURNOFF_SKIP_BILL  0x01
#define PIN_BILL_FLG_EXECUTING_WALK_CHILDREN  0x02
#define PIN_BILL_FLG_SKIP_APPLY_AR_PARENT_FEES_FIRST  0x04

/*******************************************************************
 * Bits on the PIN_FLD_CHECK_SPLIT_FLAGS field from the input flist of Opcode
 * OP_BILL_MAKE_BILL.
 *******************************************************************/

#define PIN_BILL_FLG_SPLIT  0x01

/*
 * The above bit is used to  check whether additional ARA summaries  need 
 * to be collected from database or not.
 */

/*=================================================================*
 * Product and Product instance related definitions.
 *=================================================================*/

/*******************************************************************
 * Object Type Strings (for product action events)
 *******************************************************************/
#define	PIN_OBJ_TYPE_EVENT_PRODUCT 		"/event/billing/product"
#define	PIN_OBJ_TYPE_EVENT_PRODUCT_ACTION	"/event/billing/product/action"
#define	PIN_OBJ_TYPE_EVENT_PRODUCT_ACTION_PURCHASE "/event/billing/product/action/purchase"
#define	PIN_OBJ_TYPE_EVENT_PRODUCT_ACTION_CANCEL "/event/billing/product/action/cancel"
#define	PIN_OBJ_TYPE_EVENT_PRODUCT_ACTION_MODIFY "/event/billing/product/action/modify"
#define	PIN_OBJ_TYPE_EVENT_PRODUCT_ACTION_MODIFY_STATUS "/event/billing/product/action/modify/status"

/*******************************************************************
 * Object Type Strings (for system fee events: recurring/non-recurring )
 *******************************************************************/
#define	PIN_OBJ_TYPE_EVENT_PRODUCT_FEE_PURCHASE "/event/billing/product/fee/purchase"
#define	PIN_OBJ_TYPE_EVENT_PRODUCT_FEE_CANCEL "/event/billing/product/fee/cancel"
#define	PIN_OBJ_TYPE_EVENT_CYCLE		"/event/billing/product/fee/cycle/%"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_BASE   	"/event/billing/product/fee/cycle"
#define PIN_OBJ_TYPE_EVENT_CYCLE_FORWARD_ARREAR "/event/billing/product/fee/cycle/cycle_forward_arrear"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_FORWARD_MONTHLY "/event/billing/product/fee/cycle/cycle_forward_monthly"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_FORWARD_BIMONTHLY "/event/billing/product/fee/cycle/cycle_forward_bimonthly"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_FORWARD_QUARTERLY "/event/billing/product/fee/cycle/cycle_forward_quarterly"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_FORWARD_SEMIANNUAL "/event/billing/product/fee/cycle/cycle_forward_semiannual"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_FORWARD_ANNUAL "/event/billing/product/fee/cycle/cycle_forward_annual"
#define	PIN_OBJ_TYPE_EVENT_CYCLE_ARREAR "/event/billing/product/fee/cycle/cycle_arrear"
#define PIN_OBJ_TYPE_EVENT_CYCLE_FORWARD "/event/billing/product/fee/cycle/cycle_forward"

/*******************************************************************
 * Product Instance event description strings.
 *******************************************************************/
#define PIN_EVENT_DESCR_CYCLE_ARREARS		"Cycle Arrears Fees"
#define PIN_EVENT_DESCR_CYCLE_FOLD		"Cycle Folds"
#define PIN_EVENT_DESCR_CYCLE_FORWARD_ARREAR    "Cycle Forward Arrear Fees"
#define PIN_EVENT_DESCR_CYCLE_FORWARD		"Cycle Forward Fees"
#define PIN_EVENT_DESCR_PURCHASE_FEE		"Purchase Fees"
#define PIN_EVENT_DESCR_CYCLE_TAX		"Cycle Tax"
#define PIN_EVENT_DESCR_DEAL			"Purchase Deal"
#define PIN_EVENT_DESCR_DEAL_CANCEL		"Cancel Deal"
#define PIN_EVENT_DESCR_PURCHASE		"Purchase Product"
#define PIN_EVENT_DESCR_CANCEL			"Cancel Product"
#define PIN_EVENT_DESCR_SET_PRODINFO		"Modified Product"

/*******************************************************************
 * Product Waive Flags.
 *******************************************************************/
#define PIN_PROD_WAIVE_PURCHASE		0x001000
#define PIN_PROD_WAIVE_CYCLE		0x002000
#define PIN_PROD_WAIVE_CANCEL		0x004000
#define PIN_PROD_WAIVE_EVENT		0x008000

/*******************************************************************
 * Product Flags.
 *******************************************************************/
#define PIN_PROD_PURCHASE_CHARGED	   0x01
#define PIN_PROD_CYCLE_FORWARD_CHARGED	   0x02
#define PIN_PROD_FLG_BILL_ON_DEMAND	   0x04
#define PIN_PROD_FLG_DEFERRED_ITEM_PROD	   0x08
#define PIN_PROD_FLG_DOM_CHANGE		   0x20 /*To support proration rules for cycle arrear event*/
#define PIN_PROD_FLG_PRORATE_30_DAY	   0x40
#define PIN_PROD_FLG_PRORATE_DAYS_IN_MONTH 0x80
#define PIN_PROD_FLG_OFFER_VALIDITY_ROUNDING_ON     0x100
#define PIN_PROD_FLG_OFFER_VALIDITY_ROUNDING_OFF    0x200
#define PIN_PROD_FLG_SCALE_ROUNDING_ON              0x400

/*******************************************************************
 * Discount Flags.
 *******************************************************************/
#define PIN_DISC_FLG_DISCOUNT_VALIDITY_ROUNDING_ON     0x100
#define PIN_DISC_FLG_DISCOUNT_VALIDITY_ROUNDING_OFF    0x200

/*******************************************************************
 * Don't use 0x0800000 value as it is being used in some whereelse.
 ******************************************************************/

/*******************************************************************
 * PIN_FLD_MMC_TYPE - type of multi-month cycle fee for the product.
 *******************************************************************/
#define PIN_MMC_TYPE_NONE		0
#define PIN_MMC_TYPE_MONTHLY		1
#define PIN_MMC_TYPE_BIMONTHLY		2
#define PIN_MMC_TYPE_QUARTERLY		3
#define PIN_MMC_TYPE_SEMIANNUAL		6
#define PIN_MMC_TYPE_ANNUAL		12

/*******************************************************************
 * PIN_FLD_CYCLE_FEE_FLAGS - Indicates the fact of applying cycle 
 * forward fees. It is used In Advance Billing in order to shift the 
 * start cycle time.
 *******************************************************************/
#define PIN_CYCLE_FEE_CHARGED		0x1
#define PIN_CYCLE_FEE_CHANGE_START	0x2

/*******************************************************************
 * Product Type
 *******************************************************************/
typedef enum pin_prod_type {
	PIN_PROD_TYPE_ITEM =	601,
	PIN_PROD_TYPE_ONGOING = 602,
	PIN_PROD_TYPE_SYSTEM = 603
} pin_prod_type_t;

typedef enum pin_prod_override_type {
	PIN_PROD_OVERRIDE_TYPE_STANDALONE = 0,
	PIN_PROD_OVERRIDE_TYPE_BASE       = 1,
	PIN_PROD_OVERRIDE_TYPE_OVERRIDE   = 2
} pin_prod_override_type_t;

/*******************************************************************
 * OFFER_VALIDITY_ROUNDING and SCALE_ROUNDING paramter for the product
 *******************************************************************/
typedef enum pin_prod_offer_validity_rounding {
    PIN_PROD_OFFER_VALIDITY_ROUNDING_NOT_SET = 0,
    PIN_PROD_OFFER_VALIDITY_ROUNDING_ON = 1,
    PIN_PROD_OFFER_VALIDITY_ROUNDING_OFF = 2
} pin_prod_offer_validity_rounding_t;

typedef enum pin_prod_scale_rounding {
    PIN_PROD_SCALE_ROUNDING_OFF = 0,
    PIN_PROD_SCALE_ROUNDING_ON = 1
} pin_prod_scale_rounding_t;

/*******************************************************************
 * DISCOUNT_VALIDITY_ROUNDING paramter for the discount
 *******************************************************************/
typedef enum pin_discount_validity_rounding {
    PIN_DISCOUNT_VALIDITY_ROUNDING_NOT_SET = 0,
    PIN_DISCOUNT_VALIDITY_ROUNDING_ON = 1,
    PIN_DISCOUNT_VALIDITY_ROUNDING_OFF = 2
} pin_discount_validity_rounding_t;

/*******************************************************************
 * PIN_FLD_PARTIAL - Value of partial parameter for a product.
 *******************************************************************/
typedef enum pin_prod_partial {
	PIN_PROD_PARTIAL_NO =	0,
	PIN_PROD_PARTIAL_YES =	1
} pin_prod_partial_t;

/*******************************************************************
 * Product Status.
 *******************************************************************/
typedef enum pin_product_status {
	PIN_PRODUCT_STATUS_NOT_SET = 0,
	PIN_PRODUCT_STATUS_ACTIVE = 1,
	PIN_PRODUCT_STATUS_INACTIVE = 2,
	PIN_PRODUCT_STATUS_CANCELLED = 3
} pin_product_status_t;
 
/*******************************************************************
 * PIN_FLD_STATUS-_FLAGS - Status flags for the product
 *******************************************************************/
#define	PIN_PROD_STATUS_FLAGS_OVERRIDE_PURCHASE_FEE	0x01000000
#define	PIN_PROD_STATUS_FLAGS_OVERRIDE_CYCLE_FEE	0x02000000


/*-----------------------------------------------------------------*
 * End Product and Product instance related definitions.
 *-----------------------------------------------------------------*/
	

/* 
 * PIN_FLD_STATUS - Status of a beid
 */
typedef enum pin_beid_status {
        PIN_BEID_STATUS_NOT_SET =    0,
        PIN_BEID_STATUS_ACTIVE =     1,
        PIN_BEID_STATUS_INACTIVE =	2
} pin_beid_status_t;

/* 
 * PIN_FLD_ROUNDING_MODE - type of rounding to use 
 */
typedef enum pin_round_mode {
	PIN_BEID_ROUND_NEAREST =	0,
	PIN_BEID_ROUND_UP =		1,
	PIN_BEID_ROUND_DOWN =		2,
	PIN_BEID_ROUND_EVEN =		3,
	PIN_BEID_ROUND_FLOOR =		4,
	PIN_BEID_ROUND_FLOOR_ALT =	5,
	PIN_BEID_ROUND_DOWN_ALT = 	6
} pin_round_mode_t;

/*
 * Processing stage for Billing time rounding
 */
typedef enum pin_round_stage {
      PIN_BEID_ROUND_STAGE_RATING = 0,
      PIN_BEID_ROUND_STAGE_DISCOUNTING = 1,
      PIN_BEID_ROUND_STAGE_TAXATION = 2,
      PIN_BEID_ROUND_STAGE_AR = 3
} pin_round_stage_t;


/***********************************************************************
 * Default System Product (/product 1)
 ***********************************************************************/

/* If you change this macro then make sure you update init_tables.source
 * to the new value (product_t with poid_id0 1).
 */
#define PIN_PROD_SYSDEFAULT_NAME	"Default System Product"
#define PIN_PROD_SYSDEFAULT_ID		(int64)1

/*******************************************************************
 * Invoice Object Fields.
 *******************************************************************/
/*
 * PIN_FLD_TYPE - Type of invoice activity
 *  Used to build the activity array of the invoice event
 */
typedef enum pin_inv_act_type {
        PIN_INV_ACT_TYPE_UNDEFINED =   0,         /* invalid */
        PIN_INV_ACT_TYPE_HEADER =   1,         /* invalid */
	PIN_INV_ACT_TYPE_ACCT_SUM = 2,
	PIN_INV_ACT_TYPE_SRVC_SUM = 3,
	PIN_INV_ACT_TYPE_EVENT_SUM = 4,
	PIN_INV_ACT_TYPE_EVENT_DETAIL = 5,
	PIN_INV_ACT_TYPE_RATE_DETAIL = 6,
	PIN_INV_ACT_TYPE_ITEM = 7
} pin_inv_act_type_t;

/*******************************************************************
 * Opcode specific stuff.
 *******************************************************************/
/*******************************************************************
 * PCM_OP_BILL_MAKE_INVOICE defines.
 *******************************************************************/
#define FM_INV_EVENT_WILDCARD		'*'
#define FM_INV_EVENT_SEPARATOR		'/'

#define FM_INV_LIST_CHUNK_SIZE		20

/*******************************************************************
 * Emums for /config/invoice
 *******************************************************************/
/*
 * These enums are used as the PIN_FLD_TYPE, PIN_FLD_CYCLE_TYPE,
 * PIN_FLD_GROUP_TYPE and PIN_FLD_DETAIL_TYPE of the PIN_FLD_EVENTS
 * array in the /config/invoice objects
 */
typedef enum pin_bill_inv_item {
	FM_INV_DEFAULT_ITEM = 0,    /*  Default item      */
	FM_INV_SINGLE_ITEM = 1,     /*  Single Item       */
	FM_INV_SUM_ITEM	= 2,        /*  Summary Item      */
	FM_INV_HIDDEN_ITEM = 3      /*  Hidden Item       */
} pin_bill_inv_item_t;


typedef enum pin_bill_inv_grp {
	FM_INV_DEFAULT_GRP = 0,    /*  Unused Group level  */
	FM_INV_ACCT_GRP = 1,       /*  Account level group */
	FM_INV_SRVC_GRP = 2,       /*  Service level group */
	FM_INV_ANY_GRP = 3         /*  Any group           */
} pin_bill_inv_grp_t;


typedef enum pin_bill_inv_detail {
	FM_INV_DEFAULT_DETAIL = 0, /*  Unused details      */
	FM_INV_SINGLE_DETAIL = 1,  /*  Event level details */
	FM_INV_RATE_DETAIL = 2,    /*  Rate level details   */
	FM_INV_FULL_DETAIL = 3     /*  Full level details   */
} pin_bill_inv_detail_t;

typedef enum pin_bill_inv_cycle {
	FM_INV_DEFAULT_CYCLE = 0, /*  unused cycle         */
	FM_INV_IN_CYCLE	= 1,      /*  within bill cycle    */
	FM_INV_POST_CYCLE = 2,    /*  After bill cycle(ie, forward) */
	FM_INV_ANY_CYCLE = 3      /*  Any cycle            */
} pin_bill_inv_cycle_t;

/*******************************************************************
 * Emums for new invoicing   
 *******************************************************************/
typedef enum pin_bill_inv_status {
	PIN_INV_STATUS_INIT = 0,	/* Charge event created	*/
	PIN_INV_STATUS_FORMATTED = 1,	/* Formatted in file	*/
	PIN_INV_STATUS_PRINTED = 2,	/* File sent to printer */
	PIN_INV_STATUS_FAXED = 3,	/* File sent via FAX	*/
	PIN_INV_STATUS_EMAILED = 4	/* File emailed		*/
} pin_bill_inv_status_t;

/*******************************************************************
 * Emums for new invoice types   
 *******************************************************************/
typedef enum pin_bill_inv_types {
	PIN_INV_TYPE_NONE = 0,		/* Not set		*/
	PIN_INV_TYPE_REAL_INVOICE = 1, 	/* Request for payment	*/
	PIN_INV_TYPE_VIRTUAL_INVOICE = 2 /* Statement		*/
} pin_bill_inv_types_t;

/*
** Search indexes
*/
#define FM_INV_CYCLE_SINGLE_SEARCH	238
#define FM_INV_SRVC_SEARCH		235
#define FM_INV_CYCLE_SUM_SEARCH		239
#define FM_INV_END_BILL_SEARCH		209
#define FM_INV_ITEM_SEARCH		274
#define FM_INV_ITEM_SINGLE_SEARCH	275
#define FM_INV_ITEM_SUM_SEARCH		276

/*
** Types of objects - summary or event
*/
#define PIN_INVOICE_TYPE_INVOICE	277
#define PIN_INVOICE_TYPE_SUMMARY	278

/*
** Invoice descriptions.
*/
#define FM_INV_SUBORD_ACCT_DESC		"Subordinate Account"
#define FM_INV_ACCT_SUM_DESC		"Account Summary"
#define FM_INV_NON_SRVC_SUM_DESC	"Non-Service Summary"
#define FM_INV_SERVICE_DESC		"Service"


/*  Set up structure to be used for the event tree nodes */
typedef struct fm_inv_tree_node {
	char				*event;
	struct fm_inv_tree_node		*next;
	struct fm_inv_tree_node		*children;
	pin_bill_inv_item_t		item_type;
	pin_bill_inv_grp_t		group;
	pin_bill_inv_cycle_t		cycle;
	pin_bill_inv_detail_t		detail;
	char				*description;
} fm_inv_tree_node_t;

/*
 * Invoice format types.
 */
typedef enum pin_inv_format_types {
	PIN_INV_FORMAT_TYPE_UNDEFINED =	 0,
	PIN_INV_FORMAT_TYPE_HTML =	 1,
	PIN_INV_FORMAT_TYPE_ASCII =	 2,
	PIN_INV_FORMAT_TYPE_PDF =	 3,
	PIN_INV_FORMAT_TYPE_PCL =	 4,
	PIN_INV_FORMAT_TYPE_AFP =	 5,
	PIN_INV_FORMAT_TYPE_POSTSCRIPT = 6,
	PIN_INV_FORMAT_TYPE_METACODE =	 7
} pin_inv_format_types_t;


/*
 * Default invoice directory is used if a pin_billd invoice_dir
 * entry is not found in the cm/pin.conf file. default_invoice_dir
 * is assumed to be a subdirectory of PIN_HOME.
 */
#define PIN_DEFAULT_INVOICE_DIR "apps/pin_billd/invoice_dir"

/*
 * These are structures to hold the list of strings, or events
 * Using a general purpose dynamic list mechanism
 */

typedef struct fm_inv_list_header {
	int	size;
	int	allocated;
} fm_inv_list_header_t;

typedef struct fm_inv_list_entry {
	int	elem;
	char	*value;
} fm_inv_list_entry_t;

typedef struct fm_inv_list {
	fm_inv_list_header_t	header;
	fm_inv_list_entry_t	*entry;
} fm_inv_list_t;

/*
 * Tolerance
 */
typedef enum {
	PIN_TOLERANCE_NONE,
	PIN_TOLERANCE_PRIMARY,
	PIN_TOLERANCE_SECONDARY,
	PIN_TOLERANCE_BOTH
} pin_tolerance_t;

/*
 * Pin.conf stuff for tolerance
 */
#define PIN_BILL_PC_OVERDUE_TOLERANCE	"overdue_tolerance"
#define PIN_BILL_PC_UNDERDUE_TOLERANCE	"underdue_tolerance"
#define PIN_BILL_PC_TOLERANCE_NONE	"none"
#define PIN_BILL_PC_TOLERANCE_PRIMARY	"primary"
#define PIN_BILL_PC_TOLERANCE_SECONDARY	"secondary"
#define PIN_BILL_PC_TOLERANCE_BOTH	"primary_secondary"

/*******************************************************************
 * Make bill deferred taxation skip related defines.
 *******************************************************************/
#define BILL_BUS_PROFILE_SKIP_DEFER_TAX		"Skip_Deferred_Taxation"
#define BILL_BUS_PROFILE_SKIP_DEFER_TAX_VALUE	"YES"

/*******************************************************************
 * Make bill skip previous total calculation for open item billinfo.
 *******************************************************************/
#define BILL_BUS_PROFILE_SKIP_PREV_TOTAL	"Skip_Prev_Total"
#define BILL_BUS_PROFILE_SKIP_PREV_TOTAL_VALUE	"YES"

/*******************************************************************
 * Make bill AutoClose Billed Items related defines.
 *******************************************************************/
#define BILL_BUS_PROFILE_AUTOCLOSE_BILLED_ITEMS		"AutoClose_Billed_Items"
#define BILL_BUS_PROFILE_AUTOCLOSE_BILLED_ITEMS_VALUE	"YES"
/*******************************************************************
 * Make bill related defines.
 *******************************************************************/
#define PIN_BILL_MAKE_BILL_NONE		0
#define PIN_BILL_MAKE_BILL_PARTIAL	1
#define PIN_BILL_MAKE_BILL_FINAL	2
#define PIN_BILL_MAKE_BILL_SKIP		3

/*******************************************************************
 * PCM_OP_BILL_CANCEL. ACTIONS for Cancel product.
 *******************************************************************/
#define PIN_BILL_CANCEL_PRODUCT_ACTION_CANCEL_ONLY	"cancel_only"
#define PIN_BILL_CANCEL_PRODUCT_ACTION_CANCEL_DELETE	"cancel_delete"
#define PIN_BILL_CANCEL_PRODUCT_ACTION_DONOT_CANCEL	"donot_cancel"

/*******************************************************************
 * Writeoff flags.
 *******************************************************************/
#define PIN_BILL_WRITEOFF_TAX			0x1
#define PIN_BILL_WRITEOFF_ACCOUNT_ONLY		0x2

/*******************************************************************
 * Writeoff preferences.
 *******************************************************************/
#define PIN_BILL_WRITEOFF_MIN_NET		1
#define PIN_BILL_WRITEOFF_MIN_TAX		2
#define PIN_BILL_WRITEOFF_LOW_NET_ORDER		3
#define PIN_BILL_WRITEOFF_LOW_TAX_ORDER		4
#define PIN_BILL_WRITEOFF_HIGH_NET_ORDER	5
#define PIN_BILL_WRITEOFF_HIGH_TAX_ORDER	6

/*******************************************************************
 * Writeoff error messages.
 *******************************************************************/
#define PIN_BILL_WRITEOFF_CONVERSION_ERR	"Conversion to the primary currency error"
#define PIN_BILL_WRITEOFF_BATCH_ERR		"Items validation error: check item(s) status"
#define PIN_BILL_WRITEOFF_POLVALID_ERR		"Policy validation error"
#define PIN_BILL_WRITEOFF_UNKNOWN_ERR		"Unknown error (see cm.pinlog)"
#define PIN_BILL_WRITEOFF_GET_ITEMS_ERR		"Error on getting items (see cm.pinlog)"
#define PIN_BILL_WRITEOFF_GET_BILLS_ERR		"Error on getting bills (see cm.pinlog)"
#define PIN_BILL_WRITEOFF_NO_BILLS_ERR		"No bills to writeoff"
#define PIN_BILL_WRITEOFF_NO_ITEMS_ERR		"No items to writeoff"
#define PIN_BILL_WRITEOFF_NOTHING_ERR		"Nothing to writeoff"
#define PIN_BILL_WRITEOFF_NO_DUE_ERR            "Write-off error : Cannot writeoff billinfo as billinfo due < 0"

/*******************************************************************
 * Writeoff reason codes.
 *******************************************************************/
#define PIN_REASON_CODES_CREDIT_REASONS_AUTO_WRITEOFF 8
#define PIN_WRITEOFF_FOR_PAYMENT_REVERSAL             4

/*******************************************************************
 * Deferred Action Names
 *******************************************************************/
#define PIN_BILL_GROUP_MOVE_MEMBER_TO_PARENT      "Account Parent Change"
#define PIN_BILL_GROUP_MOVE_MEMBER_SYS_DESCR      "Move Account to %s Parent"

/*******************************************************************
 * Disputes and Settlements
 *******************************************************************/
#define PIN_OBJ_TYPE_BASE_EVENT_DISPUTE		"/event/billing/dispute/"
#define PIN_OBJ_TYPE_BASE_EVENT_SETTLEMENT	"/event/billing/settlement/"

#define PIN_OBJ_TYPE_TAX_EVENT_DISPUTE		"/event/billing/dispute/tax_event"
#define PIN_OBJ_NAME_TAX_EVENT_DISPUTE		"Tax Event Dispute"
#define PIN_OBJ_NAME_TAX_ITEM_DISPUTE		"Tax Item Dispute"
#define PIN_EVENT_DESCR_TAX_EVENT_DISPUTE	"Tax Event Dispute"
#define PIN_EVENT_DESCR_TAX_ITEM_DISPUTE	"Tax Item Dispute"

#define PIN_OBJ_TYPE_TAX_EVENT_SETTLEMENT	"/event/billing/settlement/tax_event"
#define PIN_OBJ_NAME_TAX_EVENT_SETTLEMENT	"Tax Event Settlement"
#define PIN_OBJ_NAME_TAX_ITEM_SETTLEMENT	"Tax Item Settlement"
#define PIN_EVENT_DESCR_TAX_EVENT_SETTLEMENT	"Tax Event Settlement"
#define PIN_EVENT_DESCR_TAX_ITEM_SETTLEMENT	"Tax Item Settlement"

#define PIN_OBJ_TYPE_EVENT_DISPUTE		"/event/billing/dispute/event"
#define PIN_OBJ_NAME_EVENT_DISPUTE		"Event Dispute"
#define PIN_EVENT_DESCR_EVENT_DISPUTE		"Event Dispute"

#define PIN_OBJ_TYPE_EVENT_SETTLEMENT		"/event/billing/settlement/event"
#define PIN_OBJ_NAME_EVENT_SETTLEMENT		"Event Settlement"
#define PIN_EVENT_DESCR_EVENT_SETTLEMENT	"Event Settlement"

#define PIN_OBJ_TYPE_EVENT_BILL_DISPUTE		"/event/billing/dispute/bill"
#define PIN_OBJ_NAME_EVENT_BILL_DISPUTE		"Bill Dispute Event"
#define PIN_EVENT_DESCR_BILL_DISPUTE		"Bill Dispute Event"

#define PIN_OBJ_TYPE_EVENT_BILL_SETTLEMENT      "/event/billing/settlement/bill"
#define PIN_OBJ_NAME_EVENT_BILL_SETTLEMENT      "Bill Settlement Event"
#define PIN_EVENT_DESCR_BILL_SETTLEMENT         "Bill Settlement Event"

/*******************************************************************
 * Payment Reversal
 *******************************************************************/
#define PIN_OBJ_TYPE_BASE_EVENT_BILL_REVERSAL   "/event/billing/reversal/"

#define PIN_TRANS_NAME_PYMT_REVERSAL            "pymt_reversal"
#define PIN_TRANS_NAME_BILLINFO			"billinfo"

/*******************************************************************
 * PCM_OP_AR_GET_DISPUTES defines.
 *******************************************************************/

/*
 * Dispute Type Information
 */
typedef enum dispute_type {
        PIN_DISPUTE_ITEM_LEVEL =                        0,
        PIN_DISPUTE_EVENT_LEVEL =     			1,
        PIN_DISPUTE_BILL_LEVEL =     			2
} dispute_type_t;


/*******************************************************************
 * PCM_OP_AR_RESOURCE_AGGREGATION defines.
 *******************************************************************/

/*
 * Available Type Information
 */
typedef enum available_type {
        PIN_AVAILABLE_DISPUTE =		0,
        PIN_AVAILABLE_ADJUSTMENT =	1,
        PIN_AVAILABLE_SETTLEMENT =	2
} available_type_t;


#ifdef	BILL_GROUP_AUTHORIZATION

/*******************************************************************
 * PCM_OP_BILL_POL_SPEC_RELATIONSHIP return type fields.
 *******************************************************************/
/*
 * PIN_FLD_TYPE - type of accounts allowed to perform this operation.
 */
typedef enum pin_bill_check {
	PIN_ACCT_TYPE_UNDEFINED =	0,
	PIN_ACCT_TYPE_SELF =		1,
	PIN_ACCT_TYPE_PARENT =		2,
	PIN_ACCT_TYPE_ANCESTOR =	3,
	PIN_ACCT_TYPE_UNRELATED =	4,
	PIN_ACCT_TYPE_GROUP_PARENT =	5,
	PIN_ACCT_TYPE_ADMIN_CLIENT =	6,
} pin_bill_check_t;

#endif	/* BILL_GROUP_AUTHORIZATION */

/*******************************************************************
 * PCM_OP_BILL_TRANSFER_BALANCE defines.
 *******************************************************************/

/*
 * ResultS of transfer balance operation.
 */
typedef enum transfer_failure_failure {
	PIN_BILL_TRANSFER_PASS =			0,
	PIN_BILL_TRANSFER_FAIL_INSUFFICIENT_FUNDS =	1,
	PIN_BILL_TRANSFER_FAIL_INVALID_TRANSFER_AMT =	2
} transfer_failure_t;

/*
 * Constants for required/optional service
 */
typedef enum service_option {    
        PIN_BILL_SERVICE_REQUIRED =       1,         
        PIN_BILL_SERVICE_OPTIONAL =       0     
} pin_service_option_t;    

/*******************************************************************
 * TRANSITION opcodes defines
 *******************************************************************/

#define PIN_TRANS_NAME_DEAL_VALIDATION          "deal validation"
#define PIN_TRANS_NAME_TRANSITION               "transition"
#define PIN_TRANS_NAME_ECE_PERIOD               "ece_period_based_balimpacts"
#define PIN_TRANS_NAME_ECE_TIMEPOINT            "ece_period_based_timepoints"
#define PIN_FORCE_PRORATION               	"force proration"

#define PIN_TRANS_WAIVE_PURCHASE_FEES           0x1
#define PIN_TRANS_WAIVE_CANCEL_FEES             0x2

#define PIN_TRANS_PRO_NORMAL_ON_TRANSITION      0x1000

#define PIN_TRANS_FORCE_PRO_NORMAL              0x1000
#define PIN_TRANS_CHARGE_FULL_FROM_SERVICE      0x2000
#define PIN_TRANS_CHARGE_FULL_TO_SERVICE        0x4000

typedef enum pin_transition_flag {
        PIN_TRANS_FORCE_PRORATE            =    1,
        PIN_TRANS_CHG_FULL_FROM_SVC        =    2,
        PIN_TRANS_CHG_FULL_TO_SVC          =    4
} pin_transition_flag_t;


/**********************************************************************
 * Flags used during Customer Registration for change in Service Status 
 **********************************************************************/
#define PIN_BILL_CHANGE_ALL_SERVICES		0x01 
#define PIN_BILL_ALLOW_ACTIVE_SERVICE		0x02

#define PIN_BILL_CANCEL_DEAL                    0x01

/****************************************************************
 * Exception results in case of bill suppression.
 ***************************************************************/
typedef enum bill_suppression_exceptions {
        PIN_NO_EXCEPTION =		                     0,
        PIN_DUE_TO_PAYMENT_ADJUSTMENT_MADE =                 1,
        PIN_DUE_TO_FIRST_BILL =                              2,
        PIN_DUE_TO_ACCOUNT_CLOSED =                          3,
        PIN_DUE_TO_MAX_ALLOWED_SUPPRESSION_COUNT_REACHED =   4
} bill_suppression_exceptions_t;

/****************************************************************
 * Suppression reasons in case of bill suppression.
 ***************************************************************/
typedef enum bill_suppression_reasons {
        PIN_DUE_TO_MIN_BAL_MORE_THAN_BILL_TOTAL =            1,
        PIN_DUE_TO_BILL_SUPPRESSED_MANUALLY =                2,
        PIN_DUE_TO_ACCOUNT_SUPPRESSED_MANUALLY =             3
} bill_suppression_reasons_t;

/******************************************************************
* Flags used by PCM_OP_BILL_SET_ACCOUNT_SUPPRESSION and 
* PCM_OP_BILL_REMOVE_ACCOUNT_SUPPRESSION  
******************************************************************/
#define PIN_ACCOUNT_SUPPRESSED      1
#define PIN_ACCOUNT_UNSUPPRESSED    0

/*******************************************************************
 * Bill Suppression, Account Suppression related events
 *******************************************************************/
#define PIN_OBJ_TYPE_EVENT_BILLING_SUPPRESSION_EXCEPTION          "/event/billing/suppression/exception"
#define PIN_OBJ_NAME_EVENT_BILLING_SUPPRESSION_EXCEPTION          "Suppression Exception Event"
#define PIN_EVENT_DESCR_BILLING_SUPPRESSION_EXCEPTION             "Suppression Exception Event"

#define PIN_OBJ_TYPE_EVENT_BILLING_SUPPRESSION		          "/event/billing/suppression"
#define PIN_OBJ_NAME_EVENT_BILLING_SUPPRESSION		          "Billing Suppression Event"
#define PIN_EVENT_DESCR_BILLING_SUPPRESSION		          "Billing Suppression Event"

#define PIN_OBJ_TYPE_EVENT_AUDIT_SUPPRESSION_ACCOUNT_ON           "/event/audit/suppression/account/on"
#define PIN_OBJ_NAME_EVENT_AUDIT_SUPPRESSION_ACCOUNT_ON           "Account Suppression Set"
#define PIN_EVENT_DESCR_AUDIT_SUPPRESSION_ACCOUNT_ON          	  "Account Suppression Set"

#define PIN_OBJ_TYPE_EVENT_AUDIT_SUPPRESSION_ACCOUNT_OFF	  "event/audit/suppression/account/off"
#define PIN_OBJ_NAME_EVENT_AUDIT_SUPPRESSION_ACCOUNT_OFF          "Account Suppression un set"
#define PIN_EVENT_DESCR_AUDIT_SUPPRESSION_ACCOUNT_OFF         	  "Account Suppression un set"


#define PIN_OBJ_TYPE_EVENT_AUDIT_SUPPRESSION_BILL          	  "/event/audit/suppression/bill"
#define PIN_OBJ_NAME_EVENT_AUDIT_SUPPRESSION_BILL		  "Bill Suppression audit event"
#define PIN_EVENT_DESCR_AUDIT_SUPPRESSION_BILL		          "Bill Suppression audit event"

/*******************************************************************
 * Loan related events
 *******************************************************************/
#define PIN_OBJ_TYPE_EVENT_LOAN_CREDIT                          "/event/billing/loan_credit"
#define PIN_OBJ_NAME_EVENT_LOAN_CREDIT                          "Loan Credit Event"
#define PIN_EVENT_DESCR_LOAN_CREDIT                             "Loan Credit Event"

#define PIN_OBJ_TYPE_EVENT_LOAN_DEBIT                           "/event/billing/loan_debit"
#define PIN_OBJ_NAME_EVENT_LOAN_DEBIT                           "Loan Debit Event"
#define PIN_EVENT_DESCR_LOAN_DEBIT                              "Loan Debit Event"

#define PIN_OBJ_TYPE_EVENT_LOAN_GRANT                           "/event/billing/loan_grant"
#define PIN_OBJ_NAME_EVENT_LOAN_GRANT                           "Loan Grant Event"
#define PIN_EVENT_DESCR_LOAN_GRANT                              "Loan Grant Event"

#define PIN_OBJ_TYPE_EVENT_LOAN_FEE                             "/event/billing/loan_fee"
#define PIN_OBJ_NAME_EVENT_LOAN_FEE                             "Loan Fee Event"
#define PIN_EVENT_DESCR_LOAN_FEE                                "Loan Fee Event"

#define PIN_OBJ_TYPE_EVENT_LOAN_LATE_FEE                             "/event/billing/loan_late_fee"
#define PIN_OBJ_NAME_EVENT_LOAN_LATE_FEE                             "Loan Late Fee Event"
#define PIN_EVENT_DESCR_LOAN_LATE_FEE                                "Loan Late Fee Event"

#define PIN_OBJ_TYPE_EVENT_LOAN_RECOVERY                             "/event/billing/loan_recovery"
#define PIN_OBJ_NAME_EVENT_LOAN_RECOVERY                             "Loan Recovery Event"
#define PIN_EVENT_DESCR_LOAN_RECOVERY                                "Loan Recovery Event"

#define PIN_OBJ_TYPE_EVENT_LOAN_REJECT				"/event/notification/loan_reject"
#define PIN_OBJ_NAME_EVENT_LOAN_REJECT				"Loan Rejection Event"
#define PIN_EVENT_DESCR_LOAN_REJECT				"Loan Rejection Event"

#define PIN_OBJ_TYPE_EVENT_LOAN_SUCCESS				"/event/notification/loan_success"
#define PIN_OBJ_NAME_EVENT_LOAN_SUCCESS				"Loan Success Event"
#define PIN_EVENT_DESCR_LOAN_SUCCESS				"Loan Success Event"

#define PIN_OBJ_TYPE_EVENT_LOAN_PULLBACK			"/event/billing/loan_pullback"
#define PIN_OBJ_NAME_EVENT_LOAN_PULLBACK			"Loan Pullback Event"
#define PIN_EVENT_DESCR_LOAN_PULLBACK				"Loan Pullback Event"

#define PIN_OBJ_TYPE_EVENT_LOAN_RECOVERY_REVERSAL		"/event/billing/loan_recovery_reversal"
#define PIN_OBJ_NAME_EVENT_LOAN_RECOVERY_REVERSAL		"Loan Recovery Reversal Event"
#define PIN_EVENT_DESCR_LOAN_RECOVERY_REVERSAL			"Loan Recovery Reversal Event"

/*******************************************************************
 * Type of loans
 *******************************************************************/
typedef enum pin_loan_types {
        PIN_LOAN_TYPE_EMERGENCY =	0,
        PIN_LOAN_TYPE_DATA =		1,
        PIN_LOAN_TYPE_HYBRID =		2,
}pin_loan_types_t;

/*******************************************************************
 * Reason codes for loan management component.
 *******************************************************************/
typedef enum fm_loan_reason_codes {
        PIN_LOAN_RES_SUCCESS =                                          0,
        PIN_LOAN_RES_MAX_AMOUNT_EXCEEDED =                              1,
        PIN_LOAN_RES_NO_OF_TIMES_LOAN_GRANT_EXCEEDED =                  2,
        PIN_LOAN_RES_LOCATION_NOT_ELIGIBLE_FOR_LOAN =                   3,
        PIN_LOAN_RES_REJECTED_BY_EXTERNAL_ENTITY =                      4,
        PIN_LOAN_RES_AMOUNT_REQUESTED_LESS_THAN_MIN_AMOUNT =            5,
        PIN_LOAN_RES_ACCT_OR_SERVICE_IS_UNDERAGE =                      6,
	PIN_LOAN_RES_ACCT_NOT_ACTIVE =					7,
	PIN_LOAN_RES_FAILURE =						8,
	PIN_LOAN_RES_OPT_OUT =						9,
	PIN_LOAN_RES_ACTIVE_LOANS_EXCEEDED =				10
}fm_loan_reason_codes_t;

/*******************************************************************
 * fm_utils_close_bill() defines
 *******************************************************************/

#define PIN_UTILS_CLOSE_BILL_LAST_BILLINFO	"last_billinfo"
#define PIN_UTILS_CLOSE_BILL_LAST_ACCOUNT	"last_account"

/*******************************************************************
 * fm_bill_suspend_billing defines
 *******************************************************************/

#define PIN_BILL_SUSPEND_BILL_WALK_THROUGH 	0x01
#define PIN_BILL_SUSPEND_BILL_NO_WALK_THROUGH 	0x00

/*******************************************************************
 * fm_bill_resume_billing defines
 *******************************************************************/

#define PIN_BILL_RESUME_BILL_WALK_THROUGH 	0x01
#define PIN_BILL_RESUME_BILL_NO_WALK_THROUGH 	0x00


/*******************************************************************
 * wholesale process tmp events and journals defines
 *******************************************************************/

#define PIN_BILL_PROCESS_ONLY_TMP_EVENTS 	0x3F01
#define PIN_BILL_PROCESS_ONLY_TMP_JOURNALS 	0x3F02
#define PIN_BILL_PROCESS_ONLY_CHILD_JOURNALS	0x3F03

/*******************************************************************
 * Bits used in business parameter staged_billing_fee_processing that
 * configures parallel cycle fee processing (by service) as well as
 * related behavior. 
 *******************************************************************/

#define BILL_FLAG_APPLY_CYCLE_FEE_PARALLEL_BY_SERVICE	0x01
#define BILL_FLAG_ENFORCE_FEE_PROCESSING_BEFORE_BILLING	0x02
#define BILL_FLAG_AGGREGATE_CYCLE_FEE_TO_ACCOUNT_ITEM	0x04

/*******************************************************************
 * Named transaction flists
 *******************************************************************/

/*
 * List of items newly created in current transaction
 * (Initially used by update_journal() for performance improvement)
 */
#define PIN_TRANS_NAME_NEW_ITEMS        "Newly Created Items"
#define PIN_TRANS_NAME_ITEM_TRANSFER    "bill_item_transfer"

/******************************************************************
* BOC Initiated application run status
******************************************************************/
#define  PIN_BOC_STATUS_APP_START  2
#define  PIN_BOC_STATUS_APP_SUCCESS  3
#define  PIN_BOC_STATUS_APP_FAIL   4

/**************************************************************************
 *  *PIN_FLD_FLAGS value for item transfer opcode 
 *   * ***********************************************************************/
#define PIN_BILL_FLG_UPDATE_PENDING_RECV 0x01

typedef enum pin_product_flags {
	PIN_PRODUCT_FLAGS_PRORATE_SYSTEM = 0,
	PIN_PRODUCT_FLAGS_PRORATE_30_DAY = 1,
	PIN_PRODUCT_FLAGS_PRORATE_DAYS_IN_MONTH = 2
}pin_product_flags_t;

#define PIN_BILL_CREDIT_LIMIT_ROLL_UP 1
#define PIN_BILL_CREDIT_LIMIT_ROLL_DOWN 2

#endif	/*_PIN_BILL_H*/
