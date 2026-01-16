/* Continuus file information --- %full_filespec: pin_gl.h~1:incl:1 % */
/*
 * @(#) %full_filespec: pin_gl.h~1:incl:1 %
 *
* Copyright (c) 2000, 2018, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef _PIN_GL_H
#define _PIN_GL_H

/*******************************************************************
 * G/L COA account types.
 *******************************************************************/
#define PIN_GL_COA_ACCT_TYPE_EXPENSE		0x01
#define PIN_GL_COA_ACCT_TYPE_LIABILITY		0x02
#define PIN_GL_COA_ACCT_TYPE_EQUITY		0x04
#define PIN_GL_COA_ACCT_TYPE_REVENUE		0x08
#define PIN_GL_COA_ACCT_TYPE_ASSET		0x10
#define PIN_GL_COA_ACCT_TYPE_UNDEFINED		0x20

/*******************************************************************
 * G/L COA account status.
 *******************************************************************/
#define	PIN_GL_ACCOUNT_ACTIVE			1
#define	PIN_GL_ACCOUNT_INACTIVE			0

/*******************************************************************
 * G/L COA validation status.
 *******************************************************************/
#define	PIN_GL_VALIDATION_PASSED		PIN_BOOLEAN_TRUE
#define	PIN_GL_VALIDATION_FAILED		PIN_BOOLEAN_FALSE

/*******************************************************************
 * G/L COA validation messages.
 *******************************************************************/
#define PIN_GL_ACCT_NOT_FOUND		"Active gl account not found in COA "
#define PIN_GL_OFF_ACCT_NOT_FOUND	"Active gl offset account not found "
#define PIN_GL_INVALID_TYPE_MSG		"Invalid gl account type "
#define PIN_GL_OFF_INVALID_TYPE_MSG	"Invalid gl offset account type "
#define PIN_GL_SAME_ACCOUNT_TYPE_MSG	"Both Credit and Debit accts. are same "

/*******************************************************************
 * G/L ID Flag and Type values in /config/glid
 *******************************************************************/
/* Flag values */
#define PIN_GL_FLG_PENALTY_AR_GL_ID		0x01

/* Type values */
#define STANDARD_GLID_TYPE                      0
#define AR_GLID_TYPE                            1
#define REVENUE_GLID_TYPE			2
#define CONTRACT_GLID_TYPE			3

/*******************************************************************
 * asc606 related parameters 
 ********************************************************************/
#define PIN_OBJ_TYPE_EVENT_SSP_MILESTONE		"/event/activity/ssp/milestone"
#define PIN_OBJ_NAME_EVENT_SSP_MILESTONE		"SSP based Milestone Mngmt. Event Log"
#define PIN_EVENT_DESCR_MILESTONE			"Set Milestone Completion Info"
#define PIN_OBJ_TYPE_PRODUCT_DELIVERABLE		"/deliverable/product"
#define PIN_OBJ_TYPE_SERVICE_DELIVERABLE		"/deliverable/service"
#define PIN_OBJ_TYPE_REVENUE_PRODUCT			"/revenue_product"
#define PIN_OBJ_TYPE_RBI_VALUE_BASED			"/revenue_basis_item/value_based"
#define PIN_OBJ_TYPE_RBI_RATE_BASED			"/revenue_basis_item/rate_based"
#define PIN_OBJ_TYPE_FEATURE_ENTITLEMENT		"/entitlement/feature"
#define PIN_OBJ_TYPE_RECURRING_ALLOWANCE_ENTITLEMENT	"/entitlement/allowance/recurring"
#define PIN_OBJ_TYPE_ONETIME_ALLOWANCE_ENTITLEMENT	"/entitlement/allowance/onetime"
#define PIN_OBJ_TYPE_EVENT_PENALTY			"/event/billing/product/fee/cancel/penalty"
#define PIN_OBJ_TYPE_CONTRACT				"/subscriber_contract"
#define PIN_DELIVERY_IMMEDIATE				1
#define PIN_DELIVERY_TIME_BASED				2
#define PIN_DELIVERY_RESOURCE_BASED			3
#define PIN_TIME_UNIT_MONTH				1
#define JOURNAL_TYPE_AR					0x01
#define JOURNAL_TYPE_REVENUE				0x02
#define JOURNAL_TYPE_CONTRACT				0x04
#define JOURNAL_CONTRACT_GL_ID				999999
#define EVENT_TYPE_ROUNDING				"*"
#define PIN_SUBS_MONTHLY_CYCLE				1
#define PIN_SUBS_BIMONTHLY_CYCLE			2
#define PIN_SUBS_QUARTERLY_CYCLE			3
#define PIN_SUBS_SEMIANNUAL_CYCLE			4
#define PIN_SUBS_ANNUAL_CYCLE				5
#define PIN_GL_STATUS_INITIAL				0
#define PIN_GL_STATUS_PROCESSING			1
#define PIN_GL_STATUS_CANCELLED				2
#define PIN_GL_STATUS_COMPLETED				3
#define PIN_GL_MILESTONE_IMMEDIATE			0
#define PIN_GL_MILESTONE_FUTURE				1
#define PIN_GL_MILESTONE_CLOSED				2
#define PIN_GL_NEW_CONTRACT				0
#define PIN_GL_RENEW_SAME_PACKAGE			1
#define PIN_GL_RENEW_DIFF_PACKAGE			2
#define PIN_GL_EVENT_ABSENT_REV_DISTRIBUTION		0
#define PIN_GL_EVENT_PRESENT_REV_DISTRIBUTION		1
#define PIN_GL_MILESTONE_STATUS_PENDING			0
#define PIN_GL_MILESTONE_STATUS_COMPLETED		1

#endif /*_PIN_GL_H*/
