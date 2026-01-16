/*
 *
* Copyright (c) 2004, 2022, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

/* @(#)%Portal Version: pin_bal.h:BillingVelocityInt:3:2006-Sep-05 21:51:34 % */

#ifndef _PIN_BAL_H
#define _PIN_BAL_H

/* PIN_FLD_READ_BALGRP_MODE for PCM_OP_BAL_GET_BALANCES */
#define PIN_BAL_READ_BALGRP_CALC_ONLY           0
#define PIN_BAL_READ_BALGRP_LOCK                1
#define PIN_BAL_READ_BALGRP_FROM_DB		2

/* Defines for Balance Monitoring */
/* Alert Type */
#define	PIN_MONITOR_BAL_ALERT_TYPE_LIMIT		0x01
#define	PIN_MONITOR_BAL_ALERT_TYPE_THRESHOLD		0x02
#define	PIN_MONITOR_BAL_ALERT_TYPE_THRESHOLD_FIXED	0x03
#define	PIN_MONITOR_BAL_ALERT_TYPE_FLOOR		0x04
#define	PIN_MONITOR_BAL_ALERT_TYPE_LOAN_THRESHOLD	0x05
#define	PIN_MONITOR_BAL_ALERT_TYPE_LOAN_THRESHOLD_FIXED 0x06

/* Alert Reason */
/* BREACH_UP - the current balance is above the set threshold, because of increase
 * in current balance
 * BREACH_DOWN - the current balance is below the set threshold, because of
 * decrease in current blance
 * REASON_RESET_UP - this alert reason is set if the current balance becomes 
 * below threshold because of increase in threshold or credit limit or floor
 * value from their current value
 * REASON_RESET_DOWN - this alert reason is set if the current balance becomes 
 * above threshold because of decrease in threshold or credit limit or floor
 * value from their current value
 * REASON_INDETERMINATE - unknown reason
 */
#define	PIN_MONITOR_BAL_ALERT_REASON_BREACH_UP		0x01
#define	PIN_MONITOR_BAL_ALERT_REASON_BREACH_DOWN	0x02
#define	PIN_MONITOR_BAL_ALERT_REASON_RESET_UP		0x04
#define	PIN_MONITOR_BAL_ALERT_REASON_RESET_DOWN		0x08
#define	PIN_MONITOR_BAL_ALERT_REASON_INDETERMINATE	0x10

/* PIN_FLD_FLAGS for PCM_OP_BAL_GET_BAL_GRP_AND_SVC */
#define PIN_BAL_GET_SERVICE_DEFAULT		0
#define PIN_BAL_GET_SERVICE_LOGIN		1
#define PIN_BAL_GET_SERVICE_TRANSFER_LIST	2
#define PIN_BAL_GET_DEFAULT_BAL_GRP		3
#define PIN_BAL_GET_DEFAULT_BAL_GRP_AND_SVC	4

/* Consumption rules.
 * These consumption rules are defined as
 * UNDEFINED - Consumption rule is not defined.
 * EST - Consume the sub-balance with earliest valid_from first. 
 * EET - Consume earliest expired (earliest valid_to) sub-balance first. 
 * LST - Consume the sub-balance with the latest valid_from first. 
 * LET - Consume latest expired (latest valid_to) sub-balance first.
 * ESTLET - Consume the earliest sub-balance first. If more then one sub-balances 
 * have same valid_from then consume latest expired sub-balance first.
 * ESTEET - Consume the earliest sub-balance first. If more then one sub-balances 
 * have same valid_from, consume earlier expired sub-balance first.
 * LSTEET - Consume latest sub-balance first. If more then one sub-balances have 
 * same valid_from then consume earliest expired sub-balance first.
 * LSTLET - Consume latest sub-balance first. If more then one sub-balances have 
 * same valid_from then consume latest expired sub-balance first.
 * EETEST - Consume earliest expired sub-balance first. If more then one 
 * sub-balances have same valid_to, consume the earliest sub-balance first.
 * EETLST - Consume earliest expired sub-balance first. If more then one 
 * sub-balances have same valid_to, consume latest sub-balance first.
 * LETEST - Consume latest expired sub-balance first. If more then one sub-balances
 * have the same valid_to: consume the earliest sub-balance first.
 * LETLST - Consume latest expired sub-balance first. If more then one sub-balances 
 * have the same valid_to, latest sub-balance first.
 */
typedef enum pin_consumption_rule {
	PIN_CONSUMPTION_RULE_UNDEFINED	=	0,
	PIN_CONSUMPTION_RULE_EST	=	1,
	PIN_CONSUMPTION_RULE_LST	=	2,
	PIN_CONSUMPTION_RULE_EET	=	3,
	PIN_CONSUMPTION_RULE_LET	=	4,
	PIN_CONSUMPTION_RULE_ESTLET	=	5,
	PIN_CONSUMPTION_RULE_ESTEET	=	6,
	PIN_CONSUMPTION_RULE_LSTEET	=	7,
	PIN_CONSUMPTION_RULE_LSTLET	=	8,
	PIN_CONSUMPTION_RULE_EETEST	=	9,
	PIN_CONSUMPTION_RULE_EETLST	=	10,
	PIN_CONSUMPTION_RULE_LETEST	=	11,
	PIN_CONSUMPTION_RULE_LETLST	=	12
} pin_consumption_rule_t;

/* Sub-balance type
 * PIN_SUB_BAL_TYPE_DEFAULT - Allocate/Consume the default sub-balance bucket.
 * PIN_SUB_BAL_TYPE_LOAN - Allocate/Consume the loan sub-balance bucket. 
 * PIN_SUB_BAL_TYPE_TCL - Allocate/Consume the Temp credit limit bucket
*/
typedef enum pin_sub_bal_types {
	PIN_SUB_BAL_TYPE_DEFAULT	=	0,
	PIN_SUB_BAL_TYPE_LOAN		=	1,
	PIN_SUB_BAL_TYPE_TCL		=	2
}pin_sub_bal_types_t;

/* PIN_FLD_FEE_FLAG for PCM_OP_BAL_TRANSFER_BALANCE */
#define PIN_CHARGE_NONE 	0x00
#define PIN_CHARGE_SOURCE 	0x01
#define PIN_CHARGE_TARGET 	0x02
#define PIN_CHARGE_BOTH 	0x03

/* Object Name Strings */
#define PIN_OBJ_NAME_EVENT_BALANCE_EXPIRY      "Balance Expiry Notification Log"

/* Object Type Strings */
#define PIN_OBJ_TYPE_EVENT_NOTIFY_BAL_EXPIRY	"/event/notification/balance/expiry"
#define PIN_OBJ_TYPE_EVENT_NOTIFY_BAL_POST_EXPIRY    "/event/notification/balance/post_expiry"

/* Event Description Strings */
#define PIN_EVENT_DESCR_BALANCE_EXPIRY   "Balance Expiry"

/*******************************************************************
 * TRANSACTION flist defines
 *******************************************************************/

#define PIN_TRANS_NAME_ECE_SYNC_BALANCES               "ece_sync_balances"

#endif  /*_PIN_BAL_H*/
