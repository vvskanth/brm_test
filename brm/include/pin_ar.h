/*
 *
* Copyright (c) 2000, 2018, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */


#ifndef _PIN_AR_H
#define	_PIN_AR_H

/*******************************************************************
 * AR Definitions.
 *
 * All values defined here are embedded in the database
 * and therefore *cannot* change!
 *******************************************************************/

/*
 * PCM_OP_AR values.
 */
#define	PIN_AR_SEARCH_LEVEL_AR_ACCOUNT	1
#define	PIN_AR_SEARCH_LEVEL_BILL	2
#define	PIN_AR_SEARCH_LEVEL_ACCOUNT	3
#define	PIN_AR_SEARCH_LEVEL_ITEM	4
#define	PIN_AR_SEARCH_TYPE_ACCOUNT	1
#define	PIN_AR_SEARCH_TYPE_IMMEDIATE_SUB	2
#define	PIN_AR_EVENT_SEARCH		1
#define	PIN_AR_EVENT_DETAILS		2
#define	PIN_AR_EVENT_ADJUST		3
#define	PIN_AR_EVENT_SEARCH_ADJUST	4
#define	PIN_AR_SEARCH_LEVEL_ALL_PENDING	5
#define PIN_INCLUDE_CHILDREN_TRUE       1 
#define PIN_INCLUDE_CHILDREN_FALSE      0 
#define PIN_INCLUDE_CHILDREN_ONLY       2
#define PIN_INCLUDE_CHILDREN_ALL        3
#define PIN_ITEM_STATUS_REVERSED        0x08 
#define PIN_AMOUNT_INDICATOR_CREDIT     0 
#define PIN_AMOUNT_INDICATOR_DEBIT      1 
typedef enum pending_item_status{
        PIN_FLD_FLG_PENDING_ITEM_NOT_EXISTS = 0,
        PIN_FLD_FLG_PENDING_ITEM_EXISTS = 1
} pending_item_status_t;
/*
 * Added for WOR functionality
 */
#define PIN_WRITEOFF_REV_ENABLE 1
#define PIN_WRITEOFF_REV_DISABLE 0
#define PIN_WRITEOFF_REV_LEVEL_ACCOUNT "account"
#define PIN_WRITEOFF_REV_LEVEL_BILLINFO "billinfo"
#define PIN_WRITEOFF_REV_LEVEL_BILL "bill"
#define PIN_WRITEOFF_REV_LEVEL_ITEM "item"
#define PIN_WRITEOFF_REV_LEVEL_ANY "any"

#define PIN_WRITENOFF 1
#define PIN_WRITEOFF_REVERSED  2
#define PIN_WRITEOFF_REV_PROFILE_TYPE "/profile/writeoff"
#define PIN_OBJ_TYPE_ACCOUNT "/account"

#define PIN_REASON_CODES_CREDIT_REASONS 8
#define PIN_WRITEOFF_FOR_AUTO_WRITEOFF_REVERSAL 4

/*
 * Strings used for WOR
 * PIN_OBJ_TYPE_ITEM_WRITEOFF_REVERSAL is defined in pin_bill.h
 */

#define PIN_OBJ_NAME_ITEM_WRITEOFF_REVERSAL  "Writeoff Reversal"
#define PIN_OBJ_TYPE_EVENT_WRITEOFF_REVERSAL    "/event/billing/writeoff_reversal"
#define PIN_OBJ_NAME_EVENT_WRITEOFF_REVERSAL     "Write-off Reversal"
#define PIN_EVENT_DESCR_WRITEOFF_REVERSAL        "Write-off Reversal"
#define PIN_OBJ_TYPE_EVENT_WRITEOFF_REVERSAL_TAX        "/event/billing/writeoff_reversal/tax"
#define PIN_OBJ_NAME_EVENT_WRITEOFF_REVERSAL_TAX        "Write-off Reversal Tax"
#define PIN_EVENT_DESCR_WRITEOFF_REVERSAL_TAX           "Write-off Reversal Tax"

/*******************************************************************
 * PCM_OP_AR_GET_ITEM_DETAIL defines.
 *******************************************************************/

/*
 * Adjustment Type Information
 */
typedef enum adjustment_type {
        PIN_ADJUSTMENT_ITEM_LEVEL =                        0,
        PIN_ADJUSTMENT_EVENT_LEVEL =                       1,
        PIN_ADJUSTMENT_BILL_LEVEL =                        2,
        PIN_ADJUSTMENT_ACCOUNT_LEVEL =                     3
} adjustment_type_t;

/*
 * Settlement Type Information
 */
typedef enum settlement_type {
        PIN_SETTLEMENT_ITEM_LEVEL =                        0,
        PIN_SETTLEMENT_EVENT_LEVEL =                       1,
        PIN_SETTLEMENT_BILL_LEVEL =                        2
} settlement_type_t;

/*
 * Writeoff Type Information
 */
typedef enum writeoff_type {
        PIN_WRITEOFF_ITEM_LEVEL =                        0,
        PIN_WRITEOFF_BILL_LEVEL =                        1,
        PIN_WRITEOFF_BILLINFO_LEVEL =                    2,
        PIN_WRITEOFF_ACCOUNT_LEVEL =                     3
} writeoff_type_t;

/*
 * Resource Impacted Type Information
 */
typedef enum resource_impacted_type {
        PIN_RESOURCE_SINGLE =                      	  0,
        PIN_RESOURCE_MULTIPLE =			          1
} resource_impacted_type_t;


/*
 * Added for billinfo write-off 
 */
#define PIN_REASON_WRITEOFF_ACCOUNT_LEVEL 1
#define PIN_REASON_WRITEOFF_BILLINFO_LEVEL 2
#define PIN_WRITEOFF_LEVEL_REASON_DOMAIN_ID 42

/*
 * strings used for billinfo write-off
 */
#define PIN_OBJ_TYPE_EVENT_BILLINFO_WRITEOFF            "/event/billing/writeoff/billinfo"
#define PIN_OBJ_NAME_EVENT_BILLINFO_WRITEOFF            "Billinfo Write-off"
#define PIN_EVENT_DESCR_ACCOUNT_BILLINFO_WRITEOFF       "Account: Event for Billinfo Write-off"
#define PIN_EVENT_DESCR_BILLINFO_WRITEOFF               "Billinfo: Event for Billinfo Write-off"
#define PIN_OBJ_TYPE_EVENT_TAX_BILLINFO_WRITEOFF        "/event/billing/writeoff/tax_billinfo"
#define PIN_OBJ_NAME_EVENT_TAX_BILLINFO_WRITEOFF        "Billinfo Write-off Tax"
#define PIN_EVENT_DESCR_TAX_ACCOUNT_BILLINFO_WRITEOFF   "Account: Event for Billinfo Write-off Tax"
#define PIN_EVENT_DESCR_TAX_BILLINFO_WRITEOFF           "Billinfo: Event for Billinfo Write-off Tax"

/* 
 * Flags for Bill/Item Adjustment/Dispute
 * Flags are used in adjustment, dispute and settlement to differentiate if item contains tax amount or not.
 */
#define PIN_AR_TAX_ITEM 0
#define PIN_AR_USAGE_ITEM 1
#define PIN_AR_TAX_AND_USAGE_ITEM 2

/* 
 * Flags for input/output in AR opcodes
 */
#define	PIN_AR_LAST_BILL	0x01
#define	PIN_AR_ORIG_BILL	0x02
#define	PIN_AR_ALL_BILLS	0x04

#define	PIN_AR_BILLED_ITEM	0x01
#define	PIN_AR_UNBILLED_ITEM	0x02

/*
 * Flag to set the validation errors in AR opcodes
 */
#define PIN_AR_SET_EBUF_ON_ERROR       0x20

/*
 * Trans flist name
 */
#define PIN_TRANS_NAME_WRITEOFF_TAX "writeoff_tax"
#define PIN_TRANS_NAME_DISPUTE_CLOSED "dispute_closed"
#define PIN_TRANS_NAME_EVENT_DISPUTE "event_dispute"
#define PIN_TRANS_NAME_BILL_ADJUSTMENT "bill_adjustment"
#define PIN_TRANS_NAME_BILL_DISPUTE "bill_dispute"
#define PIN_TRANS_NAME_WRITEOFF_REVR "writeoff_reversal"

#define PIN_ERR_BILL_DISPUTE_WHOLESALE            159
#define PIN_ERR_BILL_SETLLEMNT_WHOLESALE          158


#endif	/*_PIN_AR_H*/
