/*	
* Copyright (c) 2016, 2019, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PIN_JOB_H
#define	_PIN_JOB_H

/*******************************************************************
 * JOB FM Definitions.
 *
 * All values defined here are embedded in the database
 * and therefore *cannot* change!
 *******************************************************************/

/*******************************************************************
 * Job Template Name Strings
 *******************************************************************/
#define PIN_OBJ_TYPE_BILLING_JOB_TEMPLATE "/job_template/billing"
#define PIN_OBJ_TYPE_COLLECT_JOB_TEMPLATE "/job_template/collect"
#define PIN_OBJ_TYPE_INVOICE_JOB_TEMPLATE "/job_template/invoice"
#define PIN_OBJ_TYPE_GL_JOB_TEMPLATE      "/job_template/ledger_report"
#define PIN_OBJ_TYPE_PRICE_SYNC_JOB_TEMPLATE      "/job_template/price_sync"
#define PIN_OBJ_TYPE_PYMT_CLEAN_JOB_TEMPLATE "/job_template/pymt_clean"
#define PIN_OBJ_TYPE_RECOVER_JOB_TEMPLATE "/job_template/recover"
#define PIN_OBJ_TYPE_REFUND_JOB_TEMPLATE "/job_template/refund"
#define PIN_OBJ_TYPE_TRIAL_BILLING_JOB_TEMPLATE "/job_template/trial_billing"
#define PIN_OBJ_TYPE_TRIAL_BILLING_PURGE_JOB_TEMPLATE "/job_template/trial_billing_purge"
#define PIN_OBJ_TYPE_DEFERRED_ACTIONS_JOB_TEMPLATE      "/job_template/deferred_actions"
#define PIN_OBJ_TYPE_WORKFLOW_JOB_TEMPLATE     "/job_template/workflow"
#define PIN_OBJ_TYPE_SYSTEM_JOB_TEMPLATE  "/job_template/system"
#define PIN_OBJ_TYPE_CUSTOM_JOB_TEMPLATE  "/job_template/custom"

/*******************************************************************
 * Action Name Strings
 *******************************************************************/
#define PIN_JOB_TEMPLATE_ACTION_DELETE    "Delete"
#define PIN_JOB_TEMPLATE_ACTION_CREATE    "Create"
#define PIN_JOB_TEMPLATE_ACTION_MODIFY    "Modify"

/*******************************************************************
 * Job Template Category
 *******************************************************************/
#define PIN_JOB_TEMPLATE_BILLING_CATEGORY       601
#define PIN_JOB_TEMPLATE_COLLECT_CATEGORY       602
#define PIN_JOB_TEMPLATE_INVOICE_CATEGORY       603
#define PIN_JOB_TEMPLATE_GL_CATEGORY            604
#define PIN_JOB_TEMPLATE_PRICE_SYNC_CATEGORY    605
#define PIN_JOB_TEMPLATE_REFUND_CATEGORY        606
#define PIN_JOB_TEMPLATE_PYMT_CLEAN_CATEGORY    607
#define PIN_JOB_TEMPLATE_RECOVER_CATEGORY       608
#define PIN_JOB_TEMPLATE_TRIAL_BILLING_CATEGORY 609
#define PIN_JOB_TEMPLATE_TRIAL_BILLING_PURGE_CATEGORY 610
#define PIN_JOB_TEMPLATE_DEFERRED_ACTIONS_CATEGORY         611
#define PIN_JOB_TEMPLATE_WORKFLOW_CATEGORY     612
#define PIN_JOB_TEMPLATE_SYSTEM_CATEGORY        613
#define PIN_JOB_TEMPLATE_CUSTOM_CATEGORY        614

/*******************************************************************
 * Job Template Relative start and end unit value
 *******************************************************************/
#define PIN_JOB_TEMPLATE_RELATIVE_END_UNIT    0
#define PIN_JOB_TEMPLATE_RELATIVE_START_UNIT  2

/*******************************************************************
 * Job Template expired status value
 *******************************************************************/
#define PIN_STATUS_EXPIRED    10104

/*******************************************************************
 * Job Template pin_deposit indication
 *******************************************************************/
#define PIN_DEPOSIT_NO_EXEC   0
#define PIN_DEPOSIT_EXEC      1

/*******************************************************************
 * Job Template pin_clean action mode
 *******************************************************************/
#define PIN_CLEAN_MODE_VERIFY   0
#define PIN_CLEAN_MODE_AUTH     1

/*******************************************************************
 * Job Template pin_recover action mode
 *******************************************************************/
#define PIN_RECOVER_MODE_RFR       0
#define PIN_RECOVER_MODE_RESUBMIT  1
#define PIN_RECOVER_MODE_PAYMENT   2

/*******************************************************************
 * Invoice Job Template Related Values
 *******************************************************************/
/*
 * Constants for Invoice Bill Type
 */
typedef enum inv_bill_type_option {
        PIN_JOB_INV_UNDEFINED_BILL_TYPE  =  0,
        PIN_JOB_INV_REGULAR_BILL =  1,
        PIN_JOB_INV_CORRECTIVE_BILL =  2
} pin_template_inv_bill_type_option_t;


/*
 * Constants for Invoice Report type
 */
typedef enum inv_report_option {
        PIN_JOB_INV_UNDEFINED_REPORT = 0,
        PIN_JOB_INV_SUMMARY_REPORT  =  1,
        PIN_JOB_INV_DETAIL_REPORT =  2
} pin_template_inv_report_option_t;

/*
 * Constants for corrective invoice mode
 */
typedef enum inv_corr_mode_option {
	PIN_JOB_INV_UNDEFINED_CORR_MODE = 0,
        PIN_JOB_INV_REPLACE = 1,
        PIN_JOB_INV_CORRECTION  =  2
} pin_template_inv_corr_mode_option_t;

/*******************************************************************
 * GL Report Job Template Related Values
 *******************************************************************/
/*
 *  Constants for GL Report running mode
 */
typedef enum gl_mode_option {
        PIN_JOB_GL_RUN_REPORT_MODE  =  0,
        PIN_JOB_GL_RUN_EXPORT_MODE  =  1
} pin_template_gl_mode_option_t;

/*
 * Constants for GL Report Currency type
 */
typedef enum currency_option {    
        PIN_JOB_GL_CURRENCY_TYPE  =  0,         
        PIN_JOB_GL_NON_CURRENCY_TYPE =  1,
        PIN_JOB_GL_ALL_RESOURCE_TYPE =  2     
} pin_template_currency_option_t;    


/*
 * Constants for GL Report type
 */
typedef enum gl_report_option {
	PIN_JOB_GL_UNDEFINED_REPORT = 0,
        PIN_JOB_GL_SUMMARY_REPORT  =  1,
        PIN_JOB_GL_DETAIL_REPORT =  2
} pin_template_gl_report_option_t;

/*
 * Constants for GL Report type
 */
typedef enum gl_posted_option {
        PIN_JOB_GL_NOT_POSTED = 0,
        PIN_JOB_GL_POSTED  =  1
} pin_template_gl_posted_option_t;

/*
 * Constants for /job/boc status
 */
typedef enum boc_job_status_values {
        PIN_BOC_STATUS_UNDEFINED = 0,
        PIN_BOC_STATUS_NEW = 1,
        PIN_BOC_STATUS_BRMAPP_START = 2,
        PIN_BOC_STATUS_BRMAPP_SUCCESS  =  3,
        PIN_BOC_STATUS_BRMAPP_FAIL = 4,
        PIN_BOC_STATUS_SUCCESS = 5,
        PIN_BOC_STATUS_FAILURE = 6,
        PIN_BOC_STATUS_BLACKOUT = 7,
        PIN_BOC_STATUS_UNKNOWN = 8,
        PIN_BOC_STATUS_INACTIVATE_TEMPLATE = 9,
        PIN_BOC_STATUS_INACTIVATE_JOB_INSTANCE = 10,
        PIN_BOC_STATUS_FAILURE_RERUN_INITIATE = 11,
        PIN_BOC_STATUS_BLACKOUT_RERUN_INITIATE = 12,
        PIN_BOC_STATUS_UNKNOWN_RERUN_INITIATE = 13
} pin_template_boc_job_status_values_t;

/*
 * Constants for Job_Template Type
 */
typedef enum job_template_type_option {
        PIN_JOB_TEMPLATE_TYPE_UNKNOWN = 0,
        PIN_JOB_TEMPLATE_ONETIME      = 1,
        PIN_JOB_TEMPLATE_RECURRING    = 2
} pin_template_job_template_type_option_t;

/*
 * Constants for price_sync publish
 */
typedef enum job_template_publish_option {
        PIN_JOB_TEMPLATE_PUBLISH_OFF = 0,
        PIN_JOB_TEMPLATE_PUBLISH_ON  =  1
} pin_template_job_template_publish_option_t;

/*
 * Constants for Job Completion publish
 */
typedef enum job_template_notify_option {
        PIN_JOB_TEMPLATE_NOTIFY_OFF = 0,
        PIN_JOB_TEMPLATE_NOTIFY_ON  =  1
} pin_template_job_template_notify_option_t;


#endif	/*_PIN_JOB_H*/
