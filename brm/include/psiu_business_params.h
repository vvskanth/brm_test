/*
 * Copyright (c) 1997, 2022, Oracle and/or its affiliates.
 * 
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PSIU_BUSINESS_PARAMS_H
#define _PSIU_BUSINESS_PARAMS_H

#include <pcm.h>

/* Setup the external declaration stuff correctly */
/* For a static library just use extern!          */

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************
 * Item Object Type Strings
 *******************************************************************/

#define PIN_OBJ_TYPE_BUS_PARAMS   "/config/business_params"


/************************************************************************
 * Names of Parameter Groups
 ***********************************************************************/

#define PSIU_BPARAMS_BILLING_PARAMS  "billing"
#define PSIU_BPARAMS_BILLING_FLOW    "billing_flow"
#define PSIU_BPARAMS_FEATURE_CONTROL "feature_control_information"
#define PSIU_BPARAMS_INVOICING_PARAMS  "Invoicing"
#define PSIU_BPARAMS_SUBSCRIPTION_PARAMS  "subscription"
#define PSIU_BPARAMS_MULTI_BAL		"multi-bal"
#define PSIU_BPARAMS_RATING_PARAMS  	 "rating"
#define PSIU_BPARAMS_CUSTOMER_PARAMS  "customer"
#define PSIU_BPARAMS_TCF_PARAMS "tcf"
#define PSIU_BPARAMS_COLLECTIONS_PARAMS "collections"
#define PSIU_BPARAMS_SYSTEM "system"
#define PSIU_BPARAMS_NOTIFICATION_PARAMS "notification"
#define PSIU_BPARAMS_PRICING_PARAMS "pricing"

/************************************************************************
* AR Group Name 
************************************************************************/
#define PSIU_BPARAMS_AR_PARAMS  "ar"
/************************************************************************
* Name of parameters for AR 
************************************************************************/
#define PSIU_BPARAMS_MINIMUM_PAYMENT "minimum_payment"
#define PSIU_BPARAMS_MINIMUM_REFUND "minimum_refund"
#define PSIU_BPARAMS_AR_BILL_PAYMENT_DEALLOCATION "bill_payment_deallocation"
#define PSIU_BPARAMS_AR_USE_EVENT_ROUNDING_RULE_FOR_ADJ "use_event_rounding_rules_for_adjustment"
#define PSIU_BPARAMS_AR_PINLESS_DEBIT_PROCESSING "pinless_debit_processing"
#define PSIU_BPARAMS_AR_CID_REQUIRED "cid_required"
#define PSIU_BPARAMS_AR_CVV_REQUIRED "cvv2_required"
#define PSIU_BPARAMS_AR_DD_COLLECT "dd_collect"
#define PSIU_BPARAMS_AR_DD_REVALIDATION_INTERVAL "dd_revalidation_interval"
#define PSIU_BPARAMS_AR_CC_REVALIDATION_INTERVAL "cc_revalidation_interval"
#define PSIU_BPARAMS_AR_CC_COLLECT "cc_collect"

/************************************************************************
* AAA Group Name
************************************************************************/
#define PSIU_BPARAMS_AAA_PARAMS  "AAA"

#define PSIU_BPARAMS_AAA_PIGGYBACK_ENABLE  "piggyback_enable"

/************************************************************************
* Activity Group Name 
************************************************************************/
#define PSIU_BPARAMS_ACT_PARAMS  "activity"

#define PSIU_BPARAMS_ACT_PREPAID_TRAFFIC_LIGHT_ENABLE  	"prepaid_traffic_light_enable"

#define PSIU_BPARAMS_ACT_PREPAID_TRAFFIC_LIGHT_DISABLED   0
#define PSIU_BPARAMS_ACT_PREPAID_TRAFFIC_LIGHT_ENABLED    1

#define PSIU_BPARAMS_ACT_MAX_LOGIN_ATTEMPTS	"max_login_attempts"
#define PSIU_BPARAMS_ACT_USE_EXACT_TIME_FOR_DEFERRED_ACTION	"use_exact_time_for_deferred_action"

#define PSIU_BPARAMS_ACT_AUTH_LAST_INCR_ROUNDING  	"last_increment_rounding"
#define PSIU_BPARAMS_ACT_AUTH_LAST_INCR_NO_ROUNDING  	0
#define PSIU_BPARAMS_ACT_AUTH_LAST_INCR_ROUNDUP  	1
#define PSIU_BPARAMS_ACT_AUTH_LAST_INCR_ROUNDDOWN  	2
#define PSIU_BPARAMS_ACT_SET_FIRST_USAGE_IN_SESSION  	"set_first_usage_in_session"

#define PSIU_BPARAMS_ACT_SET_FIRST_USAGE_IN_SESSION_DISABLED   0
#define PSIU_BPARAMS_ACT_SET_FIRST_USAGE_IN_SESSION_ENABLED   1

/************************************************************************
* Name of parameter for the Invoicing 
************************************************************************/
#define PSIU_BPARAMS_ADST_TAX_HANDLE			"ADST_Tax_Handle"
#define PSIU_BPARAMS_THR_SUB_SUMMARY			"threshold_subords_summary"
#define PSIU_BPARAMS_THR_SUB_DETAIL			"threshold_subords_detail"
#define PSIU_BPARAMS_SUB_AR_ITEM_INCLUDED		"sub_ar_items_included"
#define PSIU_BPARAMS_PROMOTION_DETAIL_DISPLAY   	"promotion_detail_display"
#define PSIU_BPARAMS_ENABLE_INVOICING_INTEGRATION   	"enable_invoicing_integration"
#define PSIU_BPARAMS_INVOICE_STORAGE_TYPE   		"invoice_storage_type"
#define PSIU_BPARAMS_IN_OPERATOR_SIZE			"in_operator_size"
#define PSIU_BPARAMS_DROP_EXTRA_SEQ_DISC_EVENTS         "drop_extra_seq_disc_events"
#define PSIU_BPARAMS_INCLUDE_EXTENDED_ATTR         	"include_extended_attr"
#define PSIU_BPARAMS_AR_ITEMS_IN_CORR_INVOICE           "ar_items_in_corr_invoice"
#define PSIU_BPARAMS_INV_EVENT_FETCH_SIZE		"inv_event_fetch_size"
#define PSIU_BPARAMS_INV_ITEM_FETCH_SIZE		"inv_item_fetch_size"
#define PSIU_BPARAMS_EVENT_CACHE			"event_cache"
#define PSIU_BPARAMS_DISABLE_FILTER_ITEM_BY_PRUNE_TIME   "dis_fil_item_by_prune_time"

/************************************************************************
 * Names of parameters for the Business Parameter Group 'billing'
 ***********************************************************************/

#define PSIU_BPARAMS_MULTI_BAL_BALANCE_MONITOR	"balance_monitoring"
#define PSIU_BPARAMS_MULTI_BAL_SKIP_BAL_MONITOR_IMPACT "skip_bal_monitor_impact"
#define PSIU_BPARAMS_BILLING_GL_REPORTING	"general_ledger_reporting"
#define PSIU_BPARAMS_BILLING_CYCLE_OFFSET       "billing_cycle_offset"
#define PSIU_BPARAMS_BILLING_BILLNOW_APPLY_FEES		"apply_cycle_fees_for_bill_now"
#define PSIU_BPARAMS_BILLING_AUTO_TRIGGERING_LIMIT	"auto_triggering_limit"
#define PSIU_BPARAMS_BILLING_SKIP_CHECK_SUBORDS_BILLED         "skip_check_for_subordinates_billed"  
#define PSIU_BPARAMS_BILLING_ENABLE_ARA         	"enable_ara"
#define PSIU_BPARAMS_BDOM_BILLING_DIR         		"move_day_forward"
#define PSIU_BPARAMS_REMOVE_SPONSOREE         		"remove_sponsoree"
#define PSIU_BPARAMS_BILLING_FLOW_DISCOUNT    		"billing_flow_discount"
#define PSIU_BPARAMS_BILLING_FLOW_SPONSORSHIP 		"billing_flow_sponsorship"
#define PSIU_BPARAMS_BILLING_FLOW_CYCLIC_DSG_CHECK      "billing_flow_skip_cyc_dsg_check"
#define PSIU_BPARAMS_PROD_END_OFFSET_PLAN_TRANSITION    "prod_end_offset_plan_transition"
#define PSIU_BPARAMS_BILLING_GEN_EPSILON 		"generate_journal_epsilon"
#define PSIU_BPARAMS_AR_INCENTIVE_ENABLE  		"payment_incentive_enable"
#define PSIU_BPARAMS_AR_BILL_SEARCH_ENABLE      	"search_bill_amount_enable"
#define PSIU_BPARAMS_AR_PYMT_SUSPENSE_ENABLE    	"payment_suspense_enable"
#define PSIU_BPARAMS_AR_NON_REFUNDABLE_CREDIT_ITEMS    	"nonrefundable_credit_items"
#define PSIU_BPARAMS_AR_PYMT_ITEM_OVERALLOCATION    	"item_overallocation"
#define PSIU_BPARAMS_VALIDATE_DISC_DEP			"validate_discount_dependency"
#define PSIU_BPARAMS_BILLING_RERATE_ENABLE  		"rerate_during_billing"
#define PSIU_BPARAMS_BILLING_ROLLOVER_CORRECTION_ENABLE "rollover_correction_during_billing"
#define PSIU_BPARAMS_BILLING_SUB_BAL_VALIDITY   	"sub_bal_validity"
#define PSIU_BPARAMS_BILLING_CREATE_TWO_BILLNOW_BILLS   "create_two_bill_now_bills_in_delay"
#define PSIU_BPARAMS_BILLING_SEQUENTIAL_CYCLE_DISCOUNTING	"sequential_cycle_discounting"
#define PSIU_BPARAMS_BILLING_BILL_DOM_CHARGES_ON_CURRENT_BILL "bill_dom_charges_on_current_bill"
#define PSIU_BPARAMS_SORT_VALIDITY_BY				"sort_validity_by"
#define PSIU_BPARAMS_BILLING_CACHE_RESIDENCY_DISTINCTION	"cache_residency_distinction"
#define PSIU_BPARAMS_BILLING_DEFAULT_BUSINESS_PROFILE		"default_business_profile"
#define PSIU_BPARAMS_GET_OFFERINGS_FROM_CACHE_THRESHOLD 	"get_offerings_from_cache_threshold"
#define PSIU_BPARAMS_LOCK_CONCURRENCY 		"lock_concurrency"
#define PSIU_BPARAMS_BILLING_CUSTOM_JOURNAL_UPDATE	"custom_journal_update"
#define PSIU_BPARAMS_BILLING_PERF_ADVANCED_TUNING_SETTINGS	"perf_advanced_tuning_settings"
#define PSIU_BPARAMS_BILLING_BILL_GENERATE_CHILD_BILLNOW	"generate_bill_for_child_billnow"
#define PSIU_BPARAMS_BILLING_STAGED_BILLING_FEE_PROCESSING "staged_billing_fee_processing"
#define PSIU_BPARAMS_BACKDATING_PAST_GL_POSTED_DATE	"backdating_past_gl_posted_date"
#define PSIU_BPARAMS_BILLING_DEAL_PURCHASE_CLOSED_ACCOUNT       "deal_purchase_for_closed_account"
#define PSIU_BPARAMS_BILLING_CYCLE_DELAY_ALIGN                "cycle_delay_align"
#define PSIU_BPARAMS_BILLING_CONFIG_BILLING_DELAY		"config_billing_delay"
#define PSIU_BPARAMS_BILLING_CONFIG_BILLING_CYCLE		"config_billing_cycle"
#define PSIU_BPARAMS_BILLING_31DAY_BILLING                     "31day_billing"
#define PSIU_BPARAMS_MULTI_BAL_CONSUME_SUBTYPE  "consume_subtype"

#define PSIU_BPARAMS_BILLING_GEN_CORR_BILL_NO		"generate_corrective_bill_no"
#define PSIU_BPARAMS_BILLING_ENABLE_CORR_INV		"enable_corrective_invoices"
#define PSIU_BPARAMS_BILLING_GEN_CORR_PAID_BILLS	"allow_corrective_paid_bills"
#define PSIU_BPARAMS_BILLING_PYMT_REJECT_DUE_TO_BILL_NO	"reject_payments_for_previous_bill"
#define PSIU_BPARAMS_BILLING_CORR_BILL_THRESHOLD        "corrective_bill_threshold"
#define PSIU_BPARAMS_BILLING_ENABLE_SSP_REV_RECOGNITION  "enable_ssp_rev_recognition"

#define PSIU_BPARAMS_REFRESH_EXCHANGE_RATE               "refresh_exchange_rate"
#define PSIU_BPARAMS_BILLING_AR_PARENT_CYCLE_FEE_FIRST    "apply_ar_parent_cycle_fee_first"
#define PSIU_BPARAMS_BILLING_SPLIT_SPONSOR_ITEM_MEMBER   "split_sponsor_item_by_member"

#define	PSIU_BPARAMS_NONCURRENCY_RESOURCE_JOURNALING	"noncurrency_resource_journaling"
#define PSIU_BPARAMS_LOCATION_MODE_FOR_TAXATION		"location_mode_for_taxation"
#define PSIU_BPARAMS_GET_ITEM_EVENT_CHARGE_DISCOUNT_MODE 	"item_event_charge_discount_mode"
#define PSIU_BPARAMS_SEGREGATE_JOURNALS_BY_GL_PERIOD	"segregate_journals_by_gl_period"
#define PSIU_BPARAMS_ACTG_TYPE "actg_type"
#define PSIU_BPARAMS_STOP_BILL_CLOSED_ACCOUNTS "stop_bill_closed_accounts"
#define PSIU_BPARAMS_BILLING_DEF_TAX_JOURNALING "deferred_tax_journaling"
#define PSIU_BPARAMS_WHOLESALE_BILLING_SYSTEM "wholesale_billing_system"
#define PSIU_BPARAMS_BILLING_CYCLE_TAX_INTERVAL	"cycle_tax_interval"
#define PSIU_BPARAMS_BILLING_ZERO_EVENT_BALANCES	"zero_event_balances"
#define PSIU_BPARAMS_BILLING_BILL_NUM_PREFIX            "bill_num_prefix"
#define PSIU_BPARAMS_BILLING_BILL_NUM_SUFFIX            "bill_num_suffix"
#define PSIU_BPARAMS_BILLING_EXCLD_PERCENT_ZERO_TAX     "exclude_percent_for_zero_tax"
#define PSIU_BPARAMS_BILLING_APPLY_FOLDS		"apply_folds"
#define PSIU_BPARAMS_BILLING_APPLY_ROLLOVER		"apply_rollover"
#define PSIU_BPARAMS_BILLING_CANCEL_TOLERANCE		"cancel_tolerance"

/************************************************************************
* Subscription
************************************************************************/
#define PSIU_BPARAMS_BEST_PRICING				"best_pricing"
#define PSIU_BPARAMS_DISCOUNT_BASED_ON_CONTRACT_DAYS_FEATURE	"discount_based_on_contract_days_feature"
#define PSIU_BPARAMS_SUBS_AUTOMATED_MONITOR_SETUP		"automated_monitor_setup"
#define PSIU_BPARAMS_SUBS_AUTO_GROUP_SHARING_SETUP		"auto_group_sharing_setup"
#define PSIU_BPARAMS_SUBS_BACKDATE_TRIGGER_AUTO_RERATE		"backdate_trigger_auto_rerate"
#define PSIU_BPARAMS_SUBS_NUM_BILLING_CYCLES			"num_billing_cycles"
#define PSIU_BPARAMS_SUBS_EVENT_FETCH_SIZE			"event_fetch_size"
#define PSIU_BPARAMS_SUBS_PROPAGATE_DISCOUNT			"propagate_discount"
#define PSIU_BPARAMS_SUBS_RATE_CHANGE				"rate_change"
#define PSIU_BPARAMS_SUBS_CDC_LINE_CANCEL_DAY_INCLUDE           "cdc_line_cancel_day_include"
#define PSIU_BPARAMS_SUBS_CDC_LINE_CREATE_DAY_INCLUDE           "cdc_line_create_day_include"
#define PSIU_BPARAMS_BILL_TIME_DISCOUNT_WHEN			"bill_time_discount_when"
#define PSIU_BPARAMS_ROLLOVER_TRANSFER				"rollover_transfer"
#define PSIU_BPARAMS_ENABLE_PRODUCT_VALIDATION                  "enable_product_validation"
#define PSIU_BPARAMS_MAX_SERVICES_TO_SEARCH                     "max_services_to_search"
#define PSIU_BPARAMS_CANCEL_FULL_DISCOUNT_IMMEDIATE             "cancel_full_discount_immediate"
#define PSIU_BPARAMS_TAILORMADE_PRODUCTS_SEARCH                 "tailormade_products_search"
#define PSIU_BPARAMS_CANCELLED_OFFERINGS_SEARCH                 "cancelled_offerings_search"
#define PSIU_BPARAMS_BACKDATE_NO_RERATE			        "allow_backdate_with_no_rerate"
#define PSIU_BPARAMS_SUBS_DIS_74_BD_VALIDATIONS                 "subs_disable_74_backdated_validations"
#define PSIU_BPARAMS_TRANSFER_SCHEDULED_ACTIONS                 "transfer_scheduled_actions"
#define PSIU_BPARAMS_TRANSFER_RERATE_REQUESTS			"transfer_rerate_requests"
#define PSIU_BPARAMS_EVT_ADJ_DURING_CANCELLATION	        "event_adjustments_during_cancellation"
#define PSIU_BPARAMS_MULTI_PLAN					"return_multiple_plan_instances_purchased"
#define PSIU_BPARAMS_PRIORITY_SUBSCRIPTION_FEES			"use_priority_for_subscription_fees"
#define PSIU_BPARAMS_RECREATE_DURING_TRANSFER			"recreate_during_subscription_transfer"
#define PSIU_BPARAMS_CREATE_TWO_EVENTS				"create_two_events_in_first_cycle"
#define PSIU_BPARAMS_APPLY_PRORATION_RULES			"apply_proration_rules"
#define PSIU_BPARAMS_TIMESTAMP_ROUNDING                         "timestamp_rounding"
#define PSIU_BPARAMS_GET_RATE_PLAN_FROM_CACHE			"get_rate_plan_from_cache"
#define PSIU_BPARAMS_APPLY_VALIDITY_DISCOUNT_RULES		"apply_validity_discount_rules"
#define PSIU_BPARAMS_SUBS_ACCT_NUM_PREFIX                       "acct_num_prefix"
#define PSIU_BPARAMS_SUBS_ACCT_NUM_SUFFIX                       "acct_num_suffix"
#define PSIU_BPARAMS_CREATE_RERATE_JOB_DURING_CANCEL            "create_rerate_job_during_cancel"
#define PSIU_BPARAMS_DEFAULT_ZONEMAP_NAME			"default_zonemap_name"
#define PSIU_BPARAMS_MAX_RETRY_COUNT                            "max_retry_count"
#define PSIU_BPARAMS_APPLY_DISCOUNT_ON_ZERO_CHARGE		"apply_discount_on_zero_charge"
#define PSIU_BPARAMS_LOAN_REPAYMENT_PERCENT                     "loan_repayment_percent"
#define PSIU_BPARAMS_APPLY_CHARGE_ON_INACTIVE_OR_CANCEL_PRODUCT "apply_charge_on_inactive_or_cancel_product"
#define PSIU_BPARAMS_APPLY_ROLLOVER_BEFORE_CYCLE_FEES		"apply_rollover_before_cycle_fees"
#define PSIU_BPARAMS_LOAN_REDUCE_LOAN_BALANCE_DURING_RECOVERY   "reduce_loan_bal_during_recovery"
#define PSIU_BPARAMS_BACKDATE_WINDOW                            "backdate_window"
#define PSIU_BPARAMS_KEEP_CANCELLED_PRODUCTS_OR_DISCOUNTS       "keep_cancelled_products_or_discounts"
#define PSIU_BPARAMS_TIMESTAMP_ROUNDING_DURING_APPLY_LOAN	"timestamp_rounding_during_apply_loan"
#define PSIU_BPARAMS_DEL_SHARING_GRP_DURING_TERMINATION		"del_sharing_grp_during_term"

/*********************************************************
 * Bus param introduced for Telecom Argentina to associate
 * the refund event to the original purchase event via the
 * SESSION_OBJ of the event.  For more details please refer
 * to bug 10252556.
 *********************************************************/
#define PSIU_BPARAMS_SUBS_ASSOC_REFUND_TO_PURCHASE              "subs_assoc_refund_to_purchase"

/************************************************************************
* Writeoff reversal 
************************************************************************/
#define PSIU_BPARAMS_AR_AUTO_WO_REV_ENABLE  "auto_writeoff_reversal_enable"
#define PSIU_BPARAMS_AR_WRITEOFF_LEVEL  "writeoff_level"

/************************************************************************
* Business parameter for Event Adjustment
************************************************************************/
#define PSIU_BPARAMS_AR_EVENT_OVER_ADJUST_ENABLE  "event_over_adjust_enable"
/************************************************************************
* Business parameter for caching in rating 
************************************************************************/
#define PSIU_BPARAMS_RATE_PRE_CACHE_PROD_AND_DISCOUNT "rate_pre_cache_product_and_disc"
/************************************************************************
* Business parameter for rating 
************************************************************************/
#define PSIU_BPARAMS_EXTRA_RATE_FLAG "extra_rate_flags"
/************************************************************************
* Business parameter for pricing 
************************************************************************/
#define PSIU_BPARAMS_FM_PRICE_CACHE_REF_AT_START "cache_references_at_start"
#define PSIU_BPARAMS_FM_PRICE_LOG_CHANGE_EVENT "log_price_change_event"
/************************************************************************
 * Names of parameter values for the Business Parameter Group 'billing'
 ***********************************************************************/

#define PSIU_BPARAMS_BILLING_GL_REPORTING_ENABLED 1
#define PSIU_BPARAMS_BILLING_GL_REPORTING_DISABLED 0
#define PSIU_BPARAMS_MULTI_BAL_BALANCE_MONITOR_ENABLED 1
#define PSIU_BPARAMS_MULTI_BAL_BALANCE_MONITOR_DISABLED 0
#define PSIU_BPARAMS_DISCOUNT_FLOW_UNDEFINED 0
#define PSIU_BPARAMS_BILL_PARENT_DISCOUNT    1
#define PSIU_BPARAMS_BILL_MEMBER_DISCOUNT    2
#define PSIU_BPARAMS_SPONSOR_FLOW_UNDEFINED  0
#define PSIU_BPARAMS_BILL_SPONSOR            1
#define PSIU_BPARAMS_BILL_SPONSOREE          2
#define PSIU_BPARAMS_GENERATE_JOURNAL_EPSILON  1
#define PSIU_BPARAMS_AR_INCENTIVE_DISABLED   0
#define PSIU_BPARAMS_AR_INCENTIVE_ENABLED    1
#define PSIU_BPARAMS_AR_BILL_SEARCH_DISABLED	0
#define PSIU_BPARAMS_AR_BILL_SEARCH_ENABLED	1
#define PSIU_BPARAMS_AR_PYMT_SUSPENSE_ENABLED  1
#define PSIU_BPARAMS_AR_PYMT_SUSPENSE_DISABLED 0
#define PSIU_BPARAMS_AR_PYMT_ITEM_OVERALLOCATION_ENABLED 1
#define PSIU_BPARAMS_AR_PYMT_ITEM_OVERALLOCATION_DISABLED 0
#define PSIU_BPARAMS_BILL_RERATE_DISABLED    0
#define PSIU_BPARAMS_BILL_RERATE_ENABLED    1
#define PSIU_BPARAMS_BILL_ROLLOVER_CORRECTION_DISABLED     0
#define PSIU_BPARAMS_BILL_ROLLOVER_CORRECTION_ENABLED     1
#define PSIU_BPARAMS_BILLING_CUSTOM_JOURNAL_UPDATE_ENABLED 1
#define PSIU_BPARAMS_BILLING_CUSTOM_JOURNAL_UPDATE_DISABLED 0
#define PSIU_BPARAMS_BILLING_CHECK_PERF_ADVANCED_TUNING_SETTINGS 0
#define PSIU_BPARAMS_BILL_AR_PARENT_CYCLE_FEE_FIRST_ENABLED 1
#define PSIU_BPARAMS_BILLING_SPLIT_SPONSOR_ITEM_MEMBER_DISABLED  0
#define PSIU_BPARAMS_BILLING_CHECK_SUBORDS_BILLED_ENABLED   0
#define PSIU_BPARAMS_SEGREGATE_JOURNALS_BY_GL_PERIOD_ENABLED   1
#define PSIU_BPARAMS_SEGREGATE_JOURNALS_BY_GL_PERIOD_DISABLED   0
#define PSIU_BPARAMS_GL_SSP_REV_RECOGNITION_ENABLED 1
#define PSIU_BPARAMS_GL_SSP_REV_RECOGNITION_DISABLED 0
#define PSIU_BPARAMS_BILLING_DEF_TAX_JOURNALING_DISABLED 0
#define PSIU_BPARAMS_BILLING_DEF_TAX_JOUR_ENABLED_NO_TAX_LOCALE 1
#define PSIU_BPARAMS_BILLING_DEF_TAX_JOURNALING_ENABLED 2
#define PSIU_BPARAMS_BILLING_JOURNAL_ZERO_BALIMP_EVENT_DISABLED 0
#define PSIU_BPARAMS_BILLING_JOURNAL_ZERO_BALIMP_EVENT_ENABLED 1
#define PSIU_BPARAMS_GL_SSP_REV_RECOGNITION_ENABLED 1
#define PSIU_BPARAMS_GL_SSP_REV_RECOGNITION_DISABLED 0

/************************************************************************
* Rerating  Group Name 
************************************************************************/
#define PSIU_BPARAMS_RERATE_PARAMS  "rerate"

#define PSIU_BPARAMS_BILLING_TIME_DISCOUNT_BASED_ON_PERIOD "billing_time_discount_based_on_period"

#define PSIU_BPARAMS_RERATE_BATCH_PIPELINE		"batch_rating_pipeline"
#define PSIU_BPARAMS_RERATE_LINE_MANAGEMENT		"line_management"
#define PSIU_BPARAMS_RERATE_ALLOC_DURING_RERATING       "allocate_rerating_adjustments"
#define PSIU_BPARAMS_RERATE_APPLY_DEFERRED_TAX_DURING_RERATING       "apply_deferred_tax_during_rerating"
#define PSIU_BPARAMS_RERATE_APPLY_PERIOD_BASED_DURING_RERATING       "apply_period_based_during_rerating"
#define PSIU_BPARAMS_TIMESTAMP_ROUNDING_FOR_PURCHASE_GRANT "timestamp_rounding_for_purchase_grant"

#define PSIU_BPARAMS_RERATE_BATCH_PIPELINE_DISABLED    	 0
#define PSIU_BPARAMS_RERATE_BATCH_PIPELINE_ENABLED    	 1

#define PSIU_BPARAMS_RERATE_ECE_RATING			"ece_rating"
#define PSIU_BPARAMS_RERATE_ECE_RATING_DISABLED    	 0
#define PSIU_BPARAMS_RERATE_ECE_RATING_ENABLED    	 1

/************************************************************************
* Customer param
************************************************************************/
#define PSIU_BPARAMS_CUSTOMER_LIFECYCLE "subscriber_life_cycle"
#define PSIU_BPARAMS_CUSTOMER_PASSWORD_RESTRICTION "enable_password_restriction"
#define PSIU_BPARAMS_CUSTOMER_CLOSED_ACCTS_RETENTION_MONTHS "closed_accts_retention_months"
#define PSIU_BPARAMS_CUSTOMER_ACTG_DOM "actg_dom"
#define PSIU_BPARAMS_CUSTOMER_BILL_WHEN "bill_when"
#define PSIU_BPARAMS_CUSTOMER_CC_CHECKSUM "cc_checksum"

/************************************************************************
* TCF parameters 
************************************************************************/
#define PSIU_BPARAMS_DEVICE_ONLY  "restrict_device_to_service_state_propagation"


/************************************************************************
* Error Control Messages 
************************************************************************/
#define PSIU_BPARAMS_PARAMETER_OPTIONAL  0
#define PSIU_BPARAMS_PARAMETER_MANDATORY 1
/************************************************************************
* Collections
************************************************************************/
#define PSIU_BPARAMS_DELIVERY_PREFERENCE "delivery_preference"

/************************************************************************
* System
************************************************************************/
#define PSIU_BPARAMS_CONFIG_CACHE_REFRESH_INTERVAL  "config_cache_refresh_interval"
#define PSIU_BPARAMS_PREPAID_ENGINE "prepaid_engine"
#define PSIU_BPARAMS_PREPAID_PARTITION_SET "prepaid_partition_set"
#define PSIU_BPARAMS_NOTIFICATION_SUBSCRIBER_PREFERENCES "notification_subscriber_preferences"
#define PSIU_BPARAMS_NOTIFICATION_SILENT_PERIOD "notification_silent_period"
#define PSIU_BPARAMS_NOTIFICATION_SUB_PREFS_NAME "notification_subscriber_preferences"
#define PSIU_BPARAMS_ACCEPTABLE_DELAY_TIME "acceptable_delay_time"
#define PSIU_BPARAMS_SILENT_PERIOD_CALENDAR_NAME "silent_days_calendar_name"
#define PSIU_BPARAMS_CROSS_SCHEMA_SHARING_GROUP "cross_schema_sharing_group"
#define PSIU_BPARAMS_CM_AUTHORIZATION "cm_authorization"

/***********************************************************************
 * Business parameter to select Short or Long billing cycles
 ***********************************************************************/
#define PSIU_BPARAMS_SHORT_CYCLE  "short_cycle"

/***********************************************************************
 * Business parameter for Installments
 ***********************************************************************/
#define PSIU_BPARAMS_INSTALLMENT_PARAM "Installment"

#define PSIU_BPARAMS_INSTL_REMINDER_DUE_DATE  "due_date_reminder_notification"
#define PSIU_BPARAMS_INSTL_REMINDER_END_DATE  "end_date_reminder_notification"

/***********************************************************************
 *  * Business parameter for Tax Calculation
 *   ***********************************************************************/
#define PSIU_BPARAMS_PAYMENT_TAX_CALCULATION  "itemized_tax_calculation"
#define PSIU_BPARAMS_TAX_RETURN_JURIS "tax_return_juris"

/***********************************************************************
 * Business parameter for roll up of Credit Limit
 ************************************************************************/
#define PSIU_BPARAMS_RESET_MEMBER_CREDIT_LIMIT  "reset_member_credit_limit"

/*****************************************************************************
 *  * Names of parameter values for the Business Parameter Group 'notification'
 * **************************************************************************/
#define PSIU_BPARAMS_NOTIFICATION_KAFKA_DYNAMIC_KEY_ENABLED "kafka_dynamic_key_enabled"
#define PSIU_BPARAMS_NOTIFICATION_KAFKA_DB_NUMBER "kafka_db_number"

/***********************************************************************
 * API uses to retrieve Business Logic Parameters
 ***********************************************************************/

/*
 *  This function is obsolete.  Please use its "_ex" version instead for 
 *  future purposes. 
 */
extern int32 psiu_bparams_get_int(
        pcm_context_t*  contextp,
        const char*     group_namep,
        const char*     param_namep,
        pin_errbuf_t*   ebufp);

/*
 * Get the specified parameter as an integer value from a "global"
 * configuration object using the specified Infranet connection.
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   param_must_exist - A flag to check whether to turn on displaying errors
 *                      or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 * Returns:
 *   The integer representation of the value associated with the specified
 *   parameter.  If the parameter does not exist or the cannot be converted
 *   to an integer then a value of zero will be returned.
 */
extern int32 psiu_bparams_get_int_ex(
        pcm_context_t*  contextp,
        const char*     group_namep,
        const char*     param_namep,
	int32		param_must_exist,
        pin_errbuf_t*   ebufp);

/*
 *  This function is obsolete.  Please use its "_ex" version instead for 
 *  future purposes. 
 */
extern pin_decimal_t* psiu_bparams_get_decimal(
        pcm_context_t*  contextp,
        const char*     group_namep,
        const char*     param_namep,
        pin_errbuf_t*   ebufp);

/*
 * Get the specified parameter as a decimal value from a "global"
 * configuration object using the specified Infranet connection.
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   param_must_exist - A flag to check whether to turn on displaying errors
 *                      or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 * Returns:
 *   The decimal representation of the value associated with the specified
 *   parameter.  If the parameter does not exist or the cannot be converted
 *   to an decimal value then a null pointer will be returned.
 *
 *  It will be the responsibility of the caller to free the decimal value.
 */
extern pin_decimal_t* psiu_bparams_get_decimal_ex(
        pcm_context_t*  contextp,
        const char*     group_namep,
        const char*     param_namep,
	int32		param_must_exist,
        pin_errbuf_t*   ebufp);

/*
 *  This function is obsolete.  Please use its "_ex" version instead for 
 *  future purposes. 
 */
extern char* psiu_bparams_get_str(
        pcm_context_t*  contextp,
        const char*     group_namep,
        const char*     param_namep,
        char*           bufp,
        int32           buf_size,
        pin_errbuf_t*   ebufp);

/*
 * Get the specified parameter as a character array from a "global"
 * configuration object using the specified Infranet connection.
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   bufp        - Pointer to the buffer array to contain the string.
 *   buf_size    - The size of the buffer array (bufp) to contain the string
 *                 representation of the parameter.  The size is in characters
 *                 and includes the null terminator.  If buf_size is 10 then
 *                 bufp points to a buffer that contain a string with a
 *                 maximum of 9 characters.
 *   param_must_exist - A flag to check whether to turn on displaying errors
 *                      or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 *
 * Returns:
 *   Pointer to the character buffer passed in to contain the string
 *   representation of the value associated with the specified parameter.  If
 *   the parameter does not exist or cannot be converted to a string then an
 *   "empty string" will be returned.
 */
extern char* psiu_bparams_get_str_ex(
        pcm_context_t*  contextp,
        const char*     group_namep,
        const char*     param_namep,
        char*           bufp,
        int32           buf_size,
	int32		param_must_exist,
        pin_errbuf_t*   ebufp);

/*
 * Does the specified parameter exist in the specified billing parameter
 * group?
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 * Returns:
 *   Return true if the parameter exists and return false if it doesn't exist
 *   or an error was encountered.
 */
extern int32 psiu_bparams_does_param_exist(
        pcm_context_t*  contextp,
        const char*     group_namep,
        const char*     param_namep,
        pin_errbuf_t*   ebufp);

extern int32 psiu_bparams_get_sec_from_days_hours(
	char*		str_days_hours,
	pin_errbuf_t*	ebufp);

#if defined(__cplusplus)
}
#endif

#endif /* _PSIU_BUSINESS_PARAMS_H */

