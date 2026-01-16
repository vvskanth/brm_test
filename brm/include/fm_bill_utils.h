/*
 *
 * Copyright (c) 2003, 2023, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef _FM_BILL_UTILS_H
#define _FM_BILL_UTILS_H

/*******************************************************************
"@(#)$Id: fm_bill_utils.h /portalbase/1 2023/07/17 23:33:59 visheora Exp $";
*******************************************************************/

#if defined(__STDC__) || defined(PIN_USE_ANSI_HDRS)
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

#include "pcm.h"
#include "cm_fm.h"		/* for cm_op_info_t   */
#include "cm_cache.h"
#include "fm_utils.h"
#include "pin_cust.h"
#include <float.h>

#ifdef __cplusplus
#include "Pin.h"
#include "PinContext.h"
#include "PinFlist.h"
#endif /* __cplusplus */


#include <regex.h>

#define EXTERN extern


#if !defined(VAR_EXTERN)
#define VAR_EXTERN EXTERN
#endif

/*******************************************************************
 * Define the /config/gl_calendar name
 *******************************************************************/
#define CONFIG_GL_CALENDAR_NAME     "gl_calendar_start_date" 

/*******************************************************************
 * Define the hash key for the exchange rate refresh time, to be used
 * to store in cm hash.
 *******************************************************************/
#define PIN_EXCHANGE_RATE_HASH_KEY "refresh_time"

/*******************************************************************
 * Define the maximum and minumum value for GLID  to skip journal creation 
 * Journal objects are not created for GLIDs, that are in range of 
 * PIN_MIN_GLID_SKIP_JOURNAL <= GLID < PIN_MAX_GLID_SKIP_JOURNAL
 *******************************************************************/
#define PIN_MIN_GLID_SKIP_JOURNAL   1
#define PIN_MAX_GLID_SKIP_JOURNAL   100

/*******************************************************************
 * Define macro to set the bit value if tax journals created for the item
 * *****************************************************************/
#define PIN_ITEM_FLAG_TAX_JOURNALED     0x10000
/*******************************************************************
 * Define macro to set the bit value if dispute is already settled
 * for the item in case of wholesale hierarchy
 * *****************************************************************/
#define PIN_ITEM_FLAG_SETTLED   0x01000
/*******************************************************************
 * Define macro to set the bit value for rolled up AR items at parent 
 * level in case of wholesale hierarchy 
 * *****************************************************************/
#define PIN_ITEM_FLAG_ROLLUP_ITEM   0x00100
/*******************************************************************
 * Define macro that can be used to indicate that the 
 * real string could not be stored in some field due to overflow.
 * Initially used for /event INVOICE_DATA field.
 ******************************************************************/
#define MAX_LENGTH_EXCEEDED_STRING "MAX_LENGTH_EXCEEDED"
   
/*******************************************************************
 * Define the POID ID0 and config payment object
*******************************************************************/
#define PIN_OBJ_TYPE_PAYMENT_CONFIG             "/config/payment"
#define PIN_OBJ_ID0_PAYMENT_CONFIG              200

#define PIN_APPLY_BAL_BACKOUT_ONLY 	1
#define PIN_APPLY_BAL_ITEM_UPDATE 	2
#define PIN_APPLY_BAL_APPLY_BAL 	4
#define PIN_APPLY_BAL_MONITOR_IMPACTS 	8
#define PIN_APPLY_BAL_FU_VALIDITY_ONLY 	16

#define PIN_OBJ_TYPE_EVENT_RERATE_START "/event/notification/rerating/start"
#define PIN_OBJ_TYPE_EVENT_RERATE_END "/event/notification/rerating/end"
#define PIN_OBJ_TYPE_EVENT_RERATE_DELTA_BALANCES "/event/notification/rerating/delta_balances"
#define PIN_OBJ_TYPE_OVERFLOW		"/overflow"
#define MAXIMUM_POID_LIST_STR 		32000
#define EXTRA_BUF			5

#define PIN_OBJ_TYPE_CONFIG_TAX_SUPPLIER  "/config/tax_supplier"
#define PIN_OBJ_TYPE_CONFIG_GLID  "/config/glid"
/*******************************************************************
 * Entries for Revenue Assurance in bill now ,bill on demand and close bill
 *******************************************************************/
#define PIN_ARA_BILL_NOW 			"Bill-Now"
#define PIN_ARA_BILL_ON_DEMAND			"Bill-ON-DEMAND"
#define PIN_ARA_AUTO_TRIGGER_BILL		"Auto-Trigger"
#define PIN_ARA_VERSION				3 

/*******************************************************************
 * Entries for default credit profile
 *******************************************************************/
#define PIN_CURRENCY_CREDIT_PROFILE             0
#define PIN_LIMIT_CREDIT_PROFILE                1
#define PIN_NONCURRENCY_CREDIT_PROFILE          2

/*******************************************************************
 * Trial bill program name
 ******************************************************************/
#define TRIAL_BILLING_PROGRAM_NAME              "pin_trial_bill_accts"

/**********************************************************
 *  Constants to hold the billing_perf_adv_tune_setting bits
 **********************************************************/
#define PERF_ADV_TUNE_ITEM_NO_UPDATE   0x01
#define PERF_ADV_TUNE_EVENT_TRANSFER   0x02;
#define PERF_ADV_TUNE_ITEM_GL          0x04;
#define PERF_ADV_TUNE_SUPPRESS_TRIGGER_SUBORD_BILL    0x20;
#define PERF_ADV_TUNE_DISABLE_TRIAL_BILLING_JOURNAL_OPTIMIZATIONS    0x40

/*******************************************************************
 * Entries for Collection Group validation
 *******************************************************************/
#define PIN_COLL_GRP_VALIDATE 1
#define PIN_COLL_DELETE_AND_MOVE 2

/*****************************************************************
 * For PCM_OP_BILL_SET_LIMIT_AND_CR
 * 0x00  : Default value. If no thresholds passed, Merge with
 * 	   existing.
 * 0x01 : If no thresholds array passed, consider it as NULL.
 * 	  Dont merge with existing values.
 ****************************************************************/
#define PIN_BILL_MERGE_THRESHOLDS    0x00
#define PIN_BILL_USE_NULL_THRESHOLDS 0x01

/*******************************************************************
 * Entries to determine Business parameter SplitSponsorItemByMember
 *******************************************************************/
#define PIN_ENABLE_FOR_REAL_TIME_RATING 0x01


/*******************************************************************
 * String used by Subscription to mark non-relevant sequential 
 * discounting events so that Invoicing can drop them
 *******************************************************************/
#define PIN_INV_EXTRA_SEQ_DISC_EVENT            "EXTRA_SEQ_DISC_EVENT"

/*******************************************************************
 * Trial bill program name
 ******************************************************************/
#define TRIAL_BILLING_PROGRAM_NAME		"pin_trial_bill_accts"

/*******************************************************************
 * The number of decimal digits of precision
 *******************************************************************/
#define PIN_DECIMAL_DIG_PRECISION	DBL_DIG

/*******************************************************************
 * The transaction flist names related for contract transaction price
 *******************************************************************/
#define PIN_CONTRACT_QUOTE	"pin_contract_quote"
#define PIN_DEAL_LIST	"pin_deal_list"
#define PIN_PLAN_LIST	"pin_plan_list"

/*******************************************************************
 * * Error Messages 
 * *******************************************************************/
#define PIN_ERR_REQUIRED_BUNDLE_TRANSITION_FAIL		222
#define PIN_ERR_PREREQUISITE_BUNDLE_TRANSITION_FAIL	223

/*******************************************************************
 * The package (plan or deal) preceeding types
 *******************************************************************/
typedef enum package_proceeding_type {
        PACKAGE_NOT_CONTRACTABLE =   0,      /* not for contract */
        PACKAGE_NOT_SSP_BASED =   1,      /* contractable but not for SSP based revenue */
        PACKAGE_SSP_BASED =   2,      /* for SSP based revenue  */
        PACKAGE_NOT_COMMITTED_CONTRACT =   3,      /* Default non-commit contract  */
        PACKAGE_HYBRID_CONTRACTS =   4,      /* have both non-commit and normal contracts */
        PACKAGE_HYBRID_SSP_BASED =   5      /* have both SSP and non-SSP based contracts */
} package_proceeding_type_t;

/*******************************************************************
 * The promotion unit types
 *******************************************************************/
typedef enum promo_unit_type
{
	PROMO_UNIT_FIRST = 0,
	YEARLY = PROMO_UNIT_FIRST,
	QUARTERLY = 1,
	MONTHLY = 2,
	WEEKLY = 3,
	PROMO_UNIT_LAST = WEEKLY
} promo_unit_t;

typedef enum fm_tax_level {
        PIN_TAX_EVENT_LEVEL = 1,
        PIN_TAX_BILL_LEVEL = 2

}fm_tax_level_t;

typedef enum fm_tax_transaction_type
{
        PIN_TAX_ON_PAYMENT = 1,
        PIN_TAX_ON_ADJUSTMENT = 2

}fm_tax_transaction_type_t;

#define BUFF_SIZE 200
#define BILL_UTILS_LOG_DECIMAL(value, msg, level, ebufp)              \
if (PINLOG_LOGLEVEL_IS_DEBUG) {                                 \
        char buff [BUFF_SIZE];                                  \
        pin_snprintf(buff, sizeof(buff), "%s : %15.15f", msg,        \
            pbo_decimal_is_null(value, ebufp) ? -9999999999.00 :   \
                        pbo_decimal_to_double(value, ebufp));   \
        PIN_ERR_LOG_MSG(level, buff);                           \
}

#define BILL_UTILS_LOG_NUM(value, msg, level, ebufp)              \
if (PINLOG_LOGLEVEL_IS_DEBUG) {                                 \
        char buff [BUFF_SIZE];                                  \
        pin_snprintf(buff, sizeof(buff), "%s : %15.15f", msg,        \
                        (double)value);   \
        PIN_ERR_LOG_MSG(level, buff);                           \
}

#define BILL_UTILS_LOG_TIME_STAMP(value1, value2, msg, level, ebufp)  \
if (PINLOG_LOGLEVEL_IS_DEBUG) {                                 \
        char t1[25], t2[25];                                    \
        char buff [BUFF_SIZE];                                  \
        fm_rate_utils_print_tstamp(t1, value1, sizeof(t1), ebufp);  \
        fm_rate_utils_print_tstamp(t2, value2, sizeof(t2), ebufp);  \
        pin_snprintf(buff, sizeof(buff), "%s : (%s--%s)", msg ? msg: "",      \
                t1 ? t1: "", t2 ? t2: "" );                     \
        PIN_ERR_LOG_MSG(level, buff);                           \
}



/*******************************************************************
 * Public functions exported from fm_bill_utils.
 *******************************************************************/


/*
 * fm_bill_utils_beid.c
 */

EXTERN void
fm_utils_taxcode_to_glid PROTO_LIST((
                pcm_context_t   *ctxp,
                char            *taxcode,
                u_int           *tax_glip,
                pin_errbuf_t    *ebufp));

/*
 * fm_bill_utils_bill.c
 */

EXTERN void
fm_bill_mb_apply_cycle PROTO_LIST((
	pcm_context_t	*ctxp,
	int32			opcode,
	pin_flist_t		*c_flistp,
	pin_flist_t		**b_flistpp,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_bill_mb_apply_cycle_fees PROTO_LIST((
	pcm_context_t	*ctxp,
	int32			opcode,
	int32			rate_flags,
	pin_flist_t		*c_flistp,
	pin_flist_t		**b_flistpp,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_bill_mb_apply_purchase_fees PROTO_LIST((
	pcm_context_t   *ctxp,
	pin_flist_t     *c_flistp,
	pin_flist_t     **b_flistpp,
	pin_errbuf_t    *ebufp));

EXTERN void
fm_bill_mb_get_ts PROTO_LIST((
	pcm_context_t	*ctxp,
	poid_t			*a_pdp,
	poid_t			*b_pdp,
	time_t			ts_start_t,
	time_t			ts_end_t,
	pin_flist_t		**ts_flistpp,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_create_bill_obj PROTO_LIST((
		pcm_context_t	*ctxp,
		u_int		flags,
		pin_flist_t	*i_flistp,
		pin_flist_t	**nb_flistpp,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_create_item_obj PROTO_LIST((
		pcm_context_t	*ctxp,
		u_int		flags,
		pin_flist_t	*i_flistp,
		pin_flist_t	**ni_flistpp,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_prep_create_service_item PROTO_LIST((
		pcm_context_t	*ctxp,
		pin_flist_t	*item_flistp,
		poid_t		*a_pdp,
		poid_t		*s_pdp,
		poid_t		*bi_pdp,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_bill_create_items PROTO_LIST((
		pcm_context_t	*ctxp,
		int32		flags,
		pin_flist_t	*i_flistp,
		pin_flist_t	**r_flistpp,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_bill_utils_precreate_config_items PROTO_LIST((
	pcm_context_t	*ctxp,
	poid_t		*in_pdp,
	poid_list_t	**i_pdlpp,
	pin_flist_t	*item_flistp,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_bill_get_sponsorees PROTO_LIST((
		pcm_context_t	*ctxp,
		int32		flags,
		pin_flist_t	*i_flistp,
		pin_flist_t	**r_flistpp,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_bill_get_discount_members PROTO_LIST((
		pcm_context_t	*ctxp,
		int32		flags,
		pin_flist_t	*i_flistp,
		pin_flist_t	**r_flistpp,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_bill_get_group_parents PROTO_LIST((
		pcm_context_t	*ctxp,
		int32		flags,
		char            *group_type,
		pin_flist_t	*i_flistp,
		pin_flist_t	**r_flistpp,
		pin_errbuf_t	*ebufp));

EXTERN void 
fm_bill_get_config_billing_delay_and_cycle(
		pcm_context_t	*ctxp,
		int32		*config_billing_delay,
		int32		*config_billing_cycle,
		pin_errbuf_t	*ebufp);

EXTERN void
fm_utils_bill_get_service_billinfo PROTO_LIST((
        pcm_context_t	*ctxp,
        poid_t		*service_obj,
        poid_t		**billinfo_obj,
        pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_bill_get_service_group PROTO_LIST((
        pcm_context_t	*ctxp,
        poid_t		*srv_pdp,
	poid_t		*acc_pdp,
        pin_flist_t	**o_flistpp,
        pin_errbuf_t    *ebufp));

EXTERN void
fm_bill_utils_negate_event_balances PROTO_LIST((
    pin_flist_t *e_flistp,
    pin_errbuf_t *ebufp));

EXTERN void
fm_bill_utils_get_rerated_event_from_list PROTO_LIST((
	poid_t			*re_pdp,
	pin_flist_t		*ae_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp));

EXTERN void
fm_utils_bill_search_in_tt_nodes PROTO_LIST((
		pcm_context_t   *ctxp,
        	int32           flags,
        	pin_flist_t     *i_flistp,
        	pin_flist_t     **o_flistpp,
        	pin_errbuf_t    *ebufp));

EXTERN void
fm_utils_bill_sum_from_tt_nodes PROTO_LIST((
		pcm_context_t   *ctxp,
        	int32           flags,
        	pin_flist_t     *i_flistp,
        	pin_flist_t     **o_flistpp,
        	pin_errbuf_t    *ebufp));

EXTERN int
fm_utils_bill_get_count_log_partitions PROTO_LIST((
		pcm_context_t   *ctxp,
		pin_errbuf_t    *ebufp));

EXTERN int32
fm_bill_utils_is_sponsor_balance_impact PROTO_LIST((
	pin_flist_t		*e_flistp,
	int32			bi_rec_id,
	pin_errbuf_t	*ebufp));

EXTERN int32
fm_bill_utils_is_sponsor_item_type PROTO_LIST((
	const char		*item_poid_type));

EXTERN void
fm_bill_utils_base_item_type_to_sponsor_type PROTO_LIST((
	const char		*item_poid_type,
	char			sp_poid_type_buf[],
	pin_errbuf_t	*ebufp));

EXTERN int32
fm_bill_utils_validate_cycle_fee_processing(
	cm_nap_connection_t	*connp,
	pin_flist_t		*c_flistp,
	pin_errbuf_t		*ebufp);

EXTERN int32
fm_bill_utils_check_account_has_multiple_billinfos(
        pcm_context_t  *ctxp,
        poid_t *a_pdp,
        pin_errbuf_t *ebufp);

EXTERN void
fm_bill_utils_update_items_journals PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp));

EXTERN u_int32
fm_bill_utils_apply_cycle_fee_parallel_by_service();

EXTERN u_int32
fm_bill_utils_enforce_fee_processing_before_billing();

EXTERN u_int32
fm_bill_utils_aggregate_cycle_fee_to_account_item();

EXTERN pin_flist_t* 
fm_utils_srvc_match_item(
        pcm_context_t   *ctxp,
        poid_t          *s_pdp,
        char            *i_typep,
        pin_errbuf_t    *ebufp);

EXTERN void
fm_bill_utils_get_prefixed_suffixed_bill_no PROTO_LIST((
	pcm_context_t	*ctxp, 
	char		*in, 
	char		*out,
	int		nChars,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_bill_utils_get_prefixed_suffixed_acct_no PROTO_LIST((
	pcm_context_t	*ctxp,
	char		*in,
	char		*out,
	int		nChars,
	pin_errbuf_t	*ebufp));

EXTERN u_int32
fm_bill_utils_count_bal_impact_type PROTO_LIST((
	pin_flist_t		*in_flistp,
	int32			resource_id,
	u_int32			impact_type,
	pin_errbuf_t	*ebufp));

EXTERN int32
fm_bill_utils_get_cfg_include_extended_attr PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_errbuf_t	*ebufp));

/********************??????????????????
EXTERN void
fm_utils_add_rates_used_array PROTO_LIST((
	pin_flist_t		*a_flistp,
	double			*amount,
	u_int			resource_id,
	pin_errbuf_t		*ebufp));
*********************************************************/
/********************??????????????????
EXTERN pin_flist_t *
fm_utils_get_subtotal_elem PROTO_LIST((
	pin_flist_t		*ru_flistp,
	u_int			resource_id,
	pin_errbuf_t		*ebufp));
*********************************************************/
/********************??????????????????
EXTERN void
fm_utils_init_bill_obj PROTO_LIST((
		pin_flist_t	*c_flistp,
		pin_flist_t	**nb_flistpp,
		pin_errbuf_t	*ebufp));
*********************************************************/
/********************??????????????????
EXTERN void
fm_utils_init_actg_obj PROTO_LIST((
		pin_flist_t	*c_flistp,
		pin_flist_t	**na_flistpp,
		pin_errbuf_t	*ebufp));
*********************************************************/
/********************??????????????????
EXTERN void
fm_utils_init_item_obj PROTO_LIST((
		pin_flist_t	*c_flistp,
		pin_flist_t	**ni_flistpp,
		pin_errbuf_t	*ebufp));
*********************************************************/

/********************??????????????????
EXTERN void
fm_utils_round PROTO_LIST((
		double	  	*amountp,
		int		precision));
*********************************************************/
/*
 * fm_bill_utils_cycle.c
 */
EXTERN time_t
fm_utils_cycle_next_by_nunits(
	time_t          a_time,
	int32           unit,
	int32           freq,
	pin_errbuf_t    *ebufp);

EXTERN void
fm_utils_cycle_this_by_ncycles(
	int		ncycles,
	time_t		a_time,
	time_t		cycle[],
	pin_errbuf_t	*ebufp);

EXTERN void
fm_utils_cycle_apply_offset PROTO_LIST((
	int32		bill_offset,
	int32		offset_unit,
	pin_flist_t	*p_flistp,
	int		dom,
	time_t		*cycle_begp,
	time_t		*cycle_endp,
	pin_errbuf_t	*ebufp));

EXTERN time_t
fm_utils_time_round_to_midnight PROTO_LIST(( 
		time_t  a_time));

EXTERN void
fm_bill_utils_call_disc_dep PROTO_LIST((
	pcm_context_t   *ctxp,
	poid_t          *a_pdp,
	pin_flist_t     *i_flistp,
	pin_flist_t     *dis_flistp,
	pin_flist_t     *plan_flistp,
	pin_flist_t     **ddr_flistpp,
	int32		flags,
	pin_errbuf_t    *ebufp));	

EXTERN int32
fm_bill_utils_perform_disc_dep_validation PROTO_LIST ((
	pcm_context_t   *ctxp,
	poid_t          *a_pdp,
	poid_t          *p_pdp,
	pin_flist_t     *i_flistp,
	pin_flist_t     *di_flistp,
	pin_flist_t     **r_flistpp,
	time_t		eend_t,
	pin_errbuf_t    *ebufp));	


EXTERN void
fm_bill_utils_add_product_address (
        pcm_context_t           *ctxp,
        int32                   flags,
        pin_flist_t             *in_flistp,
        pin_flist_t             *d_flistp,
        pin_errbuf_t            *ebufp);

EXTERN int32
fm_bill_utils_validate_nameinfo(
        pcm_context_t           *ctxp,
        poid_t                  *ni_pdp,
        poid_t                  *a_pdp,
        pin_flist_t             **r_flistpp,
        pin_errbuf_t            *ebufp);
/*
 * fm_bill_utils_init.c
 */

VAR_EXTERN pin_flist_t *fm_utils_init_cfg_beid;
VAR_EXTERN pin_flist_t *fm_utils_init_cfg_glid;
VAR_EXTERN int update_cache4_incflds;
VAR_EXTERN pin_flist_t *pin_flist_sm_cycle_forward_events;
VAR_EXTERN pin_flist_t *pin_flist_sm_cycle_rollover_events; 
VAR_EXTERN pin_flist_t *fm_act_candidate_rums_flistp; 
VAR_EXTERN pin_flist_t *fm_bill_utils_config_era_flistp; 
VAR_EXTERN pin_flist_t *fm_bill_utils_config_bus_profile_flistp; 
VAR_EXTERN pin_flist_t *fm_bill_utils_config_template_flistp; 

/*----- Global variables to store some of stanzas from CM pin.conf ----*/
VAR_EXTERN int pin_conf_allow_move_close_acct;
VAR_EXTERN int pin_conf_valid_forward_interval;
VAR_EXTERN int pin_conf_valid_backward_interval;
VAR_EXTERN int pin_conf_enforce_scoping;
VAR_EXTERN int pin_conf_bill_sponsorees;
VAR_EXTERN int pin_conf_item_fetch_size;
VAR_EXTERN int pin_conf_subord_fetch_size;
VAR_EXTERN int pin_conf_use_number_of_days_in_month;
VAR_EXTERN int pin_conf_has_netflow;
VAR_EXTERN int pin_conf_extended_month_proration;
VAR_EXTERN int pin_conf_cycle_delay_use_special_days;
VAR_EXTERN int pin_conf_delay_cycle_fees;
VAR_EXTERN int pin_conf_custom_bill_no;
VAR_EXTERN int pin_conf_purchase_fees_backcharge;
VAR_EXTERN int pin_conf_midnight;
VAR_EXTERN int pin_conf_billnow_apply_fees;
VAR_EXTERN int pin_conf_validate_disc_dependency; 
VAR_EXTERN char pin_conf_cycle_tax_interval[64];
VAR_EXTERN int pin_conf_advance_bill_cycle;
VAR_EXTERN char fm_rate_evt_str_for_start_t[];
VAR_EXTERN long pin_conf_max_duration;
VAR_EXTERN int pin_conf_set_status_non_subord;
VAR_EXTERN int pin_conf_reserve_extend_incremental;
VAR_EXTERN int pin_conf_taxation_switch;
VAR_EXTERN int pin_conf_dynamic_taxation;
VAR_EXTERN int32 pin_conf_to_include_zero_tax;
VAR_EXTERN int32 pin_conf_stop_bill_closed_accounts;
VAR_EXTERN int32 pin_conf_calc_from_cycle_start_t;
VAR_EXTERN int   pin_conf_enable_30_day_proration;
VAR_EXTERN pin_flist_t *fm_utils_init_tax_supplier_objects_flistp;
VAR_EXTERN int32 fm_utils_init_tax_supplier_objects_ncount;
VAR_EXTERN pin_flist_t *business_param_billing_ncresources_flistp;
VAR_EXTERN int32 fm_utils_default_ts_elemid;
VAR_EXTERN int32 pin_conf_backdated_rate;
VAR_EXTERN int32 pin_conf_attach_item_to_event;
VAR_EXTERN int32 pin_conf_group_children_fetch;
VAR_EXTERN int	 business_param_flow_sponsorship;
VAR_EXTERN int	 business_param_flow_discount;
VAR_EXTERN int	 business_param_auto_triggering_limit;
VAR_EXTERN int   business_param_billing_skip_check_subords_billed;
VAR_EXTERN int   business_param_billing_ncresource_flag;
VAR_EXTERN int   business_param_billing_split_sponsor_item_by_member;
VAR_EXTERN int32 business_param_enable_ara;
VAR_EXTERN int32 pin_conf_validate_dependencies;
VAR_EXTERN int   business_param_generate_journal_epsilon;
VAR_EXTERN int   business_param_billing_rerate;
VAR_EXTERN int   business_param_billing_rollover_correction;
VAR_EXTERN int   business_param_billing_create_two_billnow_bills;
VAR_EXTERN int32 bparam_subs_dis_74_bd_validations;
VAR_EXTERN int32 bparam_transfer_scheduled_action;
VAR_EXTERN int32 bparam_transfer_rerate_requests;
VAR_EXTERN int32 bparam_inv_drop_extra_seq_disc_events;
VAR_EXTERN int   bparam_perf_adv_tune_setting;
VAR_EXTERN int32 business_param_billing_perf_adv_tune_setting;
VAR_EXTERN int32 business_param_billing_def_tax_journaling;
VAR_EXTERN int32 bparam_location_mode_for_taxation;
VAR_EXTERN int32 business_param_event_charge_discount_mode;
VAR_EXTERN pin_flist_t *fm_bill_utils_config_price_tags_flistp;

/*********************************************************
 * Bus param introduced for Telecom Argentina to associate
 * the refund event to the original purchase event via the
 * SESSION_OBJ of the event.  For more details please refer
 * to bug 10252556.
 *********************************************************/
VAR_EXTERN int32 bparam_subs_assoc_refund_to_purchase;

VAR_EXTERN int32 pin_conf_group_members_fetch;
VAR_EXTERN int32 pin_conf_event_fetch_size;
VAR_EXTERN int32 pin_conf_open_item_actg_include_prev_total;
VAR_EXTERN int32 pin_conf_skip_precreate_default_item;
VAR_EXTERN int32 general_ledger_flag;
VAR_EXTERN int32 enable_product_level_dependency_validation;
VAR_EXTERN int   is_timesten_used;
VAR_EXTERN int   is_single_row_event;
VAR_EXTERN int32 business_param_tailormade_products_search;
VAR_EXTERN int32 business_param_cancelled_offerings_search;
VAR_EXTERN int32 allow_backdate_with_no_rerate;
VAR_EXTERN int32 bparam_backdating_past_gl_posted_date;
VAR_EXTERN int32 bparam_evt_adj_during_cancellation;
VAR_EXTERN int32 bparam_multiple_plan;
VAR_EXTERN int32 business_param_apply_ar_parent_cycle_fee_first;
VAR_EXTERN int32 bparam_use_priority_subscription_fees;
VAR_EXTERN int32 bparam_recreate_during_subscription_transfer;
VAR_EXTERN int32 bparam_get_rate_plan_from_cache;
VAR_EXTERN int32 business_param_ece_rating;
VAR_EXTERN int32 bparam_staged_billing_fee_processing;
VAR_EXTERN int32 bparam_create_two_events_in_first_cycle;
VAR_EXTERN int32 bparam_apply_proration_rules;
VAR_EXTERN int32 bparam_apply_validity_discount_rules;
VAR_EXTERN int32 bparam_wholesale_billing_system;
VAR_EXTERN int32 contracts_configured;

/*********************************************************
 * Bus Param Introduced For Tax Calculation on Payment
 * and Adjustment.
 **********************************************************/
VAR_EXTERN int32 business_param_pymt_tax_calc;

EXTERN void
fm_utils_init_config_glid PROTO_LIST((
		pcm_context_t	*ctxp,
		int64		database,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_init_config_beid PROTO_LIST((
		pcm_context_t	*ctxp,
		int64		database,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_init_config_sequence PROTO_LIST((
		pcm_context_t	*ctxp,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_subs_init_cycle_events PROTO_LIST((
		pcm_context_t	*ctxp,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_subs_init_valid_cycle_forward_events PROTO_LIST((
	pin_flist_t 	**cycle_forward_events,
	char 		*required_event,
        pin_errbuf_t    *ebufp));

EXTERN  void
fm_act_init_config_rum PROTO_LIST((
        pcm_context_t   *ctxp,
        int64           database,
        pin_errbuf_t    *ebufp));

EXTERN void
fm_bill_utils_init_config_era PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_errbuf_t	*ebufp));

EXTERN time_t
fm_bill_utils_get_max_mod_t PROTO_LIST((
	pin_flist_t		*i_flistp,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_init_config_price_tags PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_errbuf_t	*ebufp));

/*
 * fm_bill_utils_balance.c
 */

EXTERN void
fm_utils_apply2_total PROTO_LIST((
        pin_flist_t             *a_flistp,
        pin_flist_t             *i_flistp,
	int32                    use_event_rounding_flag,
        pin_errbuf_t            *ebufp));


EXTERN void
fm_utils_read_bals PROTO_LIST((
		pcm_context_t   *ctxp,
		poid_t	      	*a_pdp,
		int32		flags,
		pin_flist_t     **b_flistpp,
		pin_errbuf_t    *ebufp));

EXTERN void
fm_utils_apply_multi_bal_impact PROTO_LIST((
		cm_nap_connection_t     *connp,
		u_int32			flags,
		pin_flist_t             *a_flistp,
		pin_errbuf_t            *ebufp));

EXTERN void
fm_utils_add_event_bal_impacts PROTO_LIST((
        pcm_context_t           *ctxp,
        pin_flist_t             *a_flistp,
        pin_decimal_t           *amount,
        int32                   resource_id,
	pin_decimal_t		*amount_original,
	int32			*resource_id_original,
	int32			*impact_typep,
        pin_errbuf_t            *ebufp));

EXTERN void 
fm_utils_update_acct_pending_recv PROTO_LIST((
	pcm_context_t		*ctxp,
	pin_flist_t		*a_flistp,
	pin_errbuf_t		*ebufp));

/*
 * fm_bill_utils_beid.c
 */

EXTERN pin_decimal_t *
fm_utils_round_balance PROTO_LIST((
		pin_decimal_t	*amountp,
		u_int		bal_id));

EXTERN int
fm_utils_get_precision PROTO_LIST((
		u_int		bal_id,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_round_list PROTO_LIST(( 
		u_int		currency,
		double		sum_amount,
		u_int		n_list,
		double		*d_listp,
		pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_beid_name PROTO_LIST((
		pcm_context_t   *ctxp,
		int32 		bal_id,
		char		*name_buf,
		int		name_len));

EXTERN void
fm_utils_beid_get_beid_flist PROTO_LIST((
		pcm_context_t   *ctxp,
		pin_errbuf_t *	ebufp));
		
EXTERN int32
fm_utils_beid_get_rounding PROTO_LIST((
		u_int32 	bal_id));

EXTERN int32
fm_utils_beid_get_rounding_mode PROTO_LIST((
		u_int32 	bal_id));

EXTERN pin_decimal_t *
fm_utils_beid_get_tolerance_amt_min PROTO_LIST((
		u_int32 	bal_id));

EXTERN pin_decimal_t *
fm_utils_beid_get_tolerance_amt_max PROTO_LIST((
		u_int32 	bal_id));

EXTERN pin_decimal_t *
fm_utils_beid_get_tolerance_percent PROTO_LIST((
		u_int32 	bal_id));

EXTERN char *
fm_utils_beid_get_beid_str_code PROTO_LIST((
		pcm_context_t *ctxp,
		u_int32 	bal_id,
		pin_errbuf_t *	ebufp));

EXTERN void
fm_utils_beid_get_beid_symbol PROTO_LIST((
		pcm_context_t   *ctxp,
		int32 		bal_id,
		char		*symbol_buf,
		int32		symbol_len,
		pin_errbuf_t *	ebufp));

EXTERN int32
fm_utils_beid_get_type PROTO_LIST((
		pcm_context_t *ctxp,
		u_int32 	bal_id,
		pin_errbuf_t *	ebufp));

EXTERN pin_decimal_t *
fm_utils_get_tolerance_amount PROTO_LIST((
		pin_decimal_t	*amount,
		u_int32		bal_id,
		pin_errbuf_t	*ebufp));
EXTERN pin_decimal_t *
fm_utils_round_amount PROTO_LIST((
                pin_decimal_t   *amountp,
                u_int           bal_id,
                char            *event_type,
                u_int           processing_stage));

EXTERN int
fm_utils_get_evt_precision PROTO_LIST((
		u_int		bal_id,
                char            *event_type,
                u_int           processing_stage,
		pin_errbuf_t	*ebufp));

EXTERN int32
fm_utils_beid_get_evt_rounding_mode PROTO_LIST((
		u_int32 	bal_id,
		char            *event_type,
                u_int           processing_stage));

EXTERN int32
fm_utils_beid_get_evt_rounding PROTO_LIST((
		u_int32 	bal_id,
		char            *event_type,
                u_int           processing_stage));

EXTERN void
fm_utils_evt_round_list PROTO_LIST(( 
		u_int		currency,
		double		sum_amount,
		u_int		n_list,
		double		*d_listp,
		char            *event_type,
                u_int           processing_stage,
		pin_errbuf_t	*ebufp));
EXTERN int
match_expression  PROTO_LIST ((
        config_beid_rules_t     **rulesentryp,
        u_int                   bal_id,
        char                    *event_type,
        u_int                   processing_stage,
        pin_errbuf_t            *ebufp));

EXTERN int32
fm_bill_utils_get_modes PROTO_LIST((
	pcm_context_t *ctxp,
	int32         resource_id,
	pin_errbuf_t  *ebufp));

EXTERN int32
fm_bill_utils_get_consumption_rule PROTO_LIST((
	pcm_context_t *ctxp,
	int32         resource_id));
/*
 * fm_bill_utils_billing.c
 */

EXTERN void
fm_utils_billing_get_product_rates PROTO_LIST((
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	char			*event_type,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp));

EXTERN void
fm_utils_payment_get_config_items PROTO_LIST((
	pcm_context_t	*ctxp,		
	pin_flist_t		*i_flistp,		
	int32			op_type,		
	pin_errbuf_t	*ebufp));

EXTERN pin_flist_t* 
fm_bill_utils_get_payment_cfg_objp PROTO_LIST((
	pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_read_acct PROTO_LIST((
	pcm_context_t	*ctxp,
	int32		flags,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_get_account_product PROTO_LIST((
	pcm_context_t	*ctxp,
	poid_t		*p_pdp,
	pin_flist_t	**p_flistpp,
	pin_errbuf_t	*ebufp));

EXTERN u_int32
fm_utils_is_backdating_allowed PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *a_pdp,
	time_t              end_t,
	pin_errbuf_t        *ebufp));

EXTERN u_int32 
fm_utils_get_max_invoice_data_length PROTO_LIST(());

/*
 * Account_Product.Node-location related function definitions.
 */


/*******************************************************************
 * Define the node_location seperator for the account products array
 *******************************************************************/
#define PIN_BILL_NODE_LOCATION_SEPARATOR        ':'
#define PIN_BILL_NODE_LOCATION_HOSTSIZE			64


VAR_EXTERN char 
fm_utils_ap_nodeloc_hostname[PIN_BILL_NODE_LOCATION_HOSTSIZE];

VAR_EXTERN int32 
fm_utils_ap_nodeloc_pid;

EXTERN void
fm_utils_bill_get_unique_id PROTO_LIST((
	char		*tmp_str,
	int32		size));

EXTERN void
fm_utils_cc_get_poid PROTO_LIST((
        poid_t          **o_pdpp,
        pin_errbuf_t    *ebufp));

EXTERN void
fm_utils_dd_get_poid PROTO_LIST((
        poid_t          **o_pdpp,
        pin_errbuf_t    *ebufp));

/********************??????????????????
EXTERN void
fm_utils_fillin_cc_specifics PROTO_LIST((
        pcm_context_t   *ctxp,
        pin_flist_t     *i_flistp,
        int32          op_type,
        pin_errbuf_t    *ebufp));
*********************************************************/
 
EXTERN void
fm_utils_trans_ids PROTO_LIST((
        pcm_context_t   *ctxp,
        pin_flist_t     *i_flistp,
        pin_errbuf_t    *ebufp));
 
EXTERN void
fm_utils_get_tids PROTO_LIST((
        pcm_context_t           *ctxp,
        int64                   db_no,
        int32                   count,
        int32                   seq_type,
        int64                   *seqp,
        char                    *seq_idp,
        char                    *separator,
        pin_errbuf_t            *ebufp));

EXTERN void
fm_utils_read_cfg_sequence PROTO_LIST((
        pcm_context_t           *ctxp,
        int64                   db_no,
        int32                   seq_type,
        char                    *seq_idp,
        char                    *separator,
        pin_errbuf_t            *ebufp));

EXTERN pin_flist_t *
fm_utils_get_orig_charge PROTO_LIST((
        pin_flist_t           *i_flistp,
        pin_flist_t           *r_flistp,
        int32                 element_id,
        pin_errbuf_t          *ebufp));

EXTERN void
fm_utils_fillin_charge_specifics PROTO_LIST((
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp));

EXTERN void 
fm_utils_get_rate_plan_name PROTO_LIST((
	pcm_context_t		*ctxp,
	char			*event_type,
	poid_t			*p_pdp,
	char			*rate_plan_name,
    size_t          rateplan_name_size,
        pin_errbuf_t            *ebufp));

EXTERN int
fm_utils_is_normal_item(
        poid_t          *b_pdp);

/*  New utilities for Deal/Product dependency validation routines
*
*/

EXTERN int32
fm_utils_check_pre_req PROTO_LIST((
        pcm_context_t   *ctxp,          /* Connection context   */
        poid_t          *src_dealp,     /* source deal to be checked*/
        poid_t          *pre_req_dealp,  /* pre_req deal of src_deal? */
        pin_flist_t     **pre_req_flistpp,  /* All pre_req rules object */
        pin_errbuf_t    *ebufp));         /* Error buffer         */

EXTERN int32
fm_utils_has_pre_req PROTO_LIST((
        pcm_context_t   *ctxp,          /* Connection context   */
        poid_t          *src_dealp,     /* Does this deal have any pre_reqs? */
        pin_flist_t     **pre_req_flistpp,  /* All pre_req rules object */
        pin_errbuf_t    *ebufp));         /* Error buffer         */

EXTERN int32
fm_utils_check_mutually_exclusive PROTO_LIST((
        pcm_context_t   *ctxp,          /* Connection context   */
        poid_t          *a_dealp,       /* Check a_dealp and b_dealp are
                                         mutually exclusive */
        poid_t          *b_dealp,
        pin_flist_t     **mutex_flistpp,  /* All mut-ex rules object */
        pin_errbuf_t    *ebufp));         /* Error buffer         */

EXTERN int32
fm_utils_is_required_deal PROTO_LIST((
        pcm_context_t   *ctxp,          /* Connection context   */
        poid_t          *deal_pdp,
        poid_t          *plan_pdp,
        pin_errbuf_t    *ebufp));         /* Error buffer         */

EXTERN	pin_flist_t *
fm_utils_get_valid_transitions PROTO_LIST((
        pcm_context_t   *ctxp,          /* Connection context   */
        poid_t          *from_pdp,
        poid_t          *to_pdp,
        int             transition_type,/*Transition type upgrade, downgrade */
        time_t          end_t,         /* Current Time */
        pin_errbuf_t    *ebufp));         /* Error buffer         */

EXTERN int32
fm_utils_deal_validation PROTO_LIST((
        pcm_context_t   *ctxp,          /* Connection context   */
        pin_flist_t     *deals_flistp,  /* A set of deals to validate */
        pin_flist_t     **err_flistpp,
        pin_errbuf_t    *ebufp));         /* Error buffer */

EXTERN int32
fm_utils_deal_validation_for_account PROTO_LIST((
        pcm_context_t   *ctxp,          /* Connection context   */
        poid_t          *a_pdp,         /* Account to be validated */
        pin_flist_t     *di_flistp,     /* A set of deals to be added */
        pin_errbuf_t    *ebufp));         /* Error buffer */


EXTERN void
fm_utils_get_deals_from_account PROTO_LIST((
        pcm_context_t   *ctxp,          /* Connection context   */
        poid_t          *acct_pdp,
	poid_t          *plan_pdp,
        pin_flist_t     *acct_flistp,   /* optional account flist */
        pin_flist_t     *di_flistp,     /* deals to be included(added) */
        pin_flist_t     **deals_flistpp,        /* optional account flist */
        pin_errbuf_t    *ebufp));         /* Error buffer         */

EXTERN void
fm_utils_get_plans_from_account PROTO_LIST((
        pcm_context_t   *ctxp,          /* Connection context   */
        poid_t          *acct_pdp,
        pin_flist_t     *acct_flistp,   /* optional account flist */
        pin_flist_t     **plans_flistpp,        /* optional account flist */
	time_t		eend_t,
	poid_t		*plan_pdp,
        pin_errbuf_t    *ebufp));         /* Error buffer         */


/*
 * fm_bill_utils_bill.c
 */

PIN_EXPORT void
fm_utils_bill_sort_purchased_offerings(
        pcm_context_t           *ctxp,
        pin_flist_t             *offering_flistpp,
        pin_fld_num_t           fld,
        pin_errbuf_t            *ebufp);

PIN_EXPORT int32
fm_bill_check_wholesale_billinfo(
        pcm_context_t *ctxp,
        poid_t *bi_pdp,
        pin_errbuf_t *ebufp);

EXTERN time_t
fm_utils_bill_timet_from_cycle PROTO_LIST((
	time_t          start_t,
	double          cycles));

EXTERN void
fm_utils_bill_validate_product_timespan(
	pin_flist_t	*product_flistp,
	pin_errbuf_t	*ebufp);

EXTERN void
fm_utils_create_bracket_event(
        pcm_context_t *ctxp,
        pin_flist_t *in_flistp,
        char    *event_type,
        pin_errbuf_t *ebufp );

EXTERN void
fm_utils_create_flag_bracket_event(
        pcm_context_t *ctxp,
        pin_flist_t *in_flistp,
        char    *event_type,
        int32   flag,
        pin_errbuf_t *ebufp );

EXTERN int32
fm_bill_utils_has_bus_profile_value(
         pcm_context_t	*ctxp,
         poid_t          *billinfop,
         const char*     	 keyp,
         const char*           val_bufp,
         pin_errbuf_t*   ebufp);

EXTERN char*
fm_bill_utils_get_bus_profile_value(
         pcm_context_t	*ctxp,
         poid_t          *bus_profilep,
         const char*     	 keyp,
         char*           val_bufp,
         int32           val_buf_size,
         pin_errbuf_t*   ebufp);

EXTERN char*
fm_bill_utils_get_bus_profile_value_from_billinfo(
         pcm_context_t *ctxp,
         poid_t          *billinfop,
         const char*     keyp,
         char*           val_bufp,
         int32           val_buf_size,
         pin_errbuf_t*   ebufp);

/*
 * fm_bill_utils_cfg_cache.c
 */

EXTERN u_int
fm_utils_get_glid PROTO_LIST((
        pcm_context_t   *ctxp,
	pin_flist_t             *i_flistp,
        pin_errbuf_t    *ebufp ));

EXTERN void
fm_utils_get_dialup_rate PROTO_LIST((
        pcm_context_t   *ctxp,
        char            *tsrv_id_ev,
        char            *tsrv_port_ev,
        char            **rate,
        pin_decimal_t   **min_gty,
        pin_decimal_t	**increment,
        pin_errbuf_t    *ebufp ));

EXTERN char *
fm_utils_get_fld_validate_name PROTO_LIST((
        pcm_context_t   *ctxp,
        char            *country,
        char            *name,
        pin_errbuf_t    *ebufp ));

EXTERN void
fm_utils_cfg_fld_validate_set_cache PROTO_LIST((
        pin_flist_t     *i_flistp,
        pin_errbuf_t    *ebufp ));

EXTERN void
fm_utils_cfg_fld_validate_stack_search PROTO_LIST((
	pcm_context_t   *ctxp,
        char            *cfg_name,
        pin_flist_t     **o_flistp,
        pin_errbuf_t    *ebufp));

EXTERN pin_flist_t *
fm_bill_utils_cfg_get_tax_supplier PROTO_LIST((
		pcm_context_t *ctxp,
		pin_errbuf_t *ebufp));

EXTERN int32
fm_bill_utils_cfg_get_tax_supplier_ncount PROTO_LIST((
		pcm_context_t *ctxp,
		pin_errbuf_t *ebufp));

EXTERN int32
fm_bill_utils_cfg_get_default_ts_elemid PROTO_LIST((
		pcm_context_t *ctxp,
		pin_errbuf_t *ebufp));

EXTERN pin_flist_t *
fm_bill_utils_cfg_get_ar_taxes PROTO_LIST((
		pcm_context_t *ctxp,
		pin_errbuf_t *ebufp));

EXTERN pin_flist_t *
fm_bill_utils_cfg_get_event_map_cr_events PROTO_LIST((
        pcm_context_t *ctxp,
        pin_errbuf_t *ebufp));


EXTERN pin_flist_t *
fm_bill_utils_cfg_get_event_map_cf_events PROTO_LIST((
        pcm_context_t *ctxp,
        pin_errbuf_t *ebufp));

EXTERN pin_flist_t *
fm_bill_utils_cfg_get_rum PROTO_LIST((
        pcm_context_t *ctxp,
        pin_errbuf_t *ebufp));

EXTERN pin_flist_t *
fm_bill_utils_cfg_get_beid PROTO_LIST((
        pcm_context_t *ctxp,
        pin_errbuf_t *ebufp));

EXTERN pin_flist_t *
fm_bill_utils_cfg_get_era PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_errbuf_t	*ebufp));

EXTERN pin_flist_t *
fm_bill_utils_cfg_get_business_profile PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_errbuf_t	*ebufp));

EXTERN pin_flist_t *
fm_bill_utils_cfg_get_template PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_errbuf_t	*ebufp));

EXTERN pin_flist_t *
fm_bill_utils_cfg_get_price_tags PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_errbuf_t	*ebufp));

/*
 * fm_bill_utils_cycle.c
 */
EXTERN time_t
fm_utils_cycle_get_offset PROTO_LIST((
	int32		bill_offset,
	int32		offset_unit,
        int             dom,
        time_t          cycle_end,
        pin_errbuf_t    *ebufp));

EXTERN void
fm_utils_cycle_set_offset PROTO_LIST((
        pcm_context_t   *ctxp,
        pin_flist_t     *e_flistp,
        pin_flist_t     *r_flistp,
        poid_t          *p_pdp,
        char            *event_type,
        pin_errbuf_t    *ebufp));

EXTERN pin_decimal_t *
fm_utils_mmc_scale PROTO_LIST((
		pcm_context_t   *ctxp,
                int32           ncycles,
                int32           unit,
                time_t          next_t,
                time_t          a_time,
                time_t          bill_t,
                int             dom,
                pin_flist_t     *scale_flistp,
                int             proration_30_day,
                int32           is_timestamp_rounding,
                int32           scale_rounding,
                pin_errbuf_t    *ebufp));

EXTERN void
fm_utils_add_n_days PROTO_LIST((
                int             n_days,
                time_t          *a_time));

EXTERN void
fm_utils_cycle_this PROTO_LIST((
                u_int          when,
                time_t         a_time,
                time_t         cycle[],
                pin_errbuf_t    *ebufp));

EXTERN void
fm_utils_cycle_last PROTO_LIST((
                u_int          when,
                time_t         a_time,
                time_t         cycle[],
                pin_errbuf_t    *ebufp));

EXTERN u_int
fm_utils_cycle_is_valid_dom PROTO_LIST((
                u_int           dom,
                pin_errbuf_t    *ebufp));

EXTERN int
fm_utils_cycle_get_dom PROTO_LIST((
                time_t          a_time,
                pin_errbuf_t    *ebufp));
/*
EXTERN time_t
fm_utils_cycle_actgfuturet PROTO_LIST((
                u_int           dom,
                pin_actg_len_t  cycle_len,
                time_t          actg_next_t,
                pin_errbuf_t    *ebufp));
*/
EXTERN time_t
fm_utils_cycle_actgnextt_by_ncycles PROTO_LIST((
                int             ncycles,
                int             dom,
                time_t          curr_actg_t,
                pin_errbuf_t    *ebufp));

EXTERN time_t
fm_utils_cycle_actgnextt PROTO_LIST((
                time_t          curr_actg_t,
                int             dom,
                pin_errbuf_t    *ebufp));

EXTERN time_t
fm_utils_cycle_billnextt PROTO_LIST((
                int             dom,
                int             bill_cycles_left,
                time_t          actg_next_t,
                time_t          actg_future_t,
                pin_errbuf_t   *ebufp));

EXTERN u_int
fm_utils_cycle_matching_nextbillt PROTO_LIST((
                int             m_dom,
                time_t          m_actgnextt,
                time_t          m_actgfuturet,
                int             p_dom,
                time_t          p_actgnextt,
                time_t          p_actgfuturet,
                int             p_billwhen,
                int             p_cyclesleft,
                pin_errbuf_t    *ebufp));

EXTERN void
fm_utils_cycle_last_by_ncycles_ex PROTO_LIST((
		int             ncycles,
		time_t          a_time,
		int		dom,
		time_t          cycle[],
		pin_errbuf_t    *ebufp));
/*
 * fm_bill_utils_ts_map.c
 */
EXTERN int
fm_utils_is_ts_map PROTO_LIST(());

EXTERN int
fm_utils_get_ts_map PROTO_LIST((
        char            *prod_name,
        char            *ship_to,
        char            **company_id,
        char            **business_location,
        char            **ship_from,
        int             *reg_flag,
	char		**geocode,
	int		*tax_pkg));

EXTERN void
fm_utils_tax_load_taxcodes PROTO_LIST((
	pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_tax_load_taxcodes_new PROTO_LIST((
	pcm_context_t	*ctxp,
	int64		db_no,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_tax_parse_fld PROTO_LIST((
        char            **buf,
        char            *fld,
        size_t          fld_size,
        char            delim));

EXTERN void
fm_utils_tax_get_taxcode PROTO_LIST((
	pcm_context_t	*ctxp,
	char		*tax_code,
	char		*table_entry,
	char		*tax_pkg,
	char		*taxpkg_tax_code,
	int32		*taxpkg_type,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_utils_tax_get_taxcodes PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_flist_t	*o_flistp,
	int32		flag,
	pin_errbuf_t	*ebufp));

EXTERN pin_flist_t *
fm_utils_get_tax_supplier_elem(
	pcm_context_t   *ctxp,
	int32		ts_id,
	pin_errbuf_t	*ebufp);

EXTERN void 
fm_utils_tax_load_provider_taxcodes PROTO_LIST((
	pcm_context_t   *ctxp,
	int64           db_no,
	pin_errbuf_t	*ebufp)) ;

EXTERN int32
fm_bill_utils_get_31day_billing_opt PROTO_LIST((
	pin_errbuf_t	*ebufp));

#define XLONG_STR_LEN	256
#define LONG_STR_LEN	80
#define SHORT_STR_LEN	10
/* structure to hold the taxcodes_map table */
typedef struct taxcode_s {
	char taxCode[LONG_STR_LEN];		/* tax code */
	char pkgCode[SHORT_STR_LEN];		/* tax package code */
	char code1[LONG_STR_LEN];	/* code 1 / tax rate */
	char code2[LONG_STR_LEN];	/* code 2 / valid-from */
	char ind[LONG_STR_LEN];		/* indicator / valid-to */
	char wtCode[LONG_STR_LEN];	/* worldtax code / juris level */
	char cmCode[XLONG_STR_LEN];	/* commodity code / juris list */
	char taxDesc[XLONG_STR_LEN];	/* tax description */
	char taxRule[SHORT_STR_LEN];	/* tax rule */
	/* Below fields taxPkgTaxCode and taxPkgType are used only for Third party 
	 *tax codes when used with generic Tax DM */
	char taxPkgTaxCode [LONG_STR_LEN];	/* tax provider internal tax code */
	int32  taxPkgType ;	/* tax provider package type */
	struct taxcode_s *next;		/* next element in list */
} taxcode_t;
extern taxcode_t *TaxCodeMap ;	/* head pointer to table */
extern pin_flist_t *Provider_Tax_Table ;

EXTERN taxcode_t*
fm_utils_cfg_get_taxcode_map PROTO_LIST((
	pcm_context_t	*ctxp,
	int64		db_no,
	pin_errbuf_t	*ebufp)) ;

EXTERN pin_flist_t*
fm_bill_utils_cfg_get_tax_table PROTO_LIST( (
	pcm_context_t   *ctxp,
	int64           db_no,
	pin_errbuf_t    *ebufp)) ;

/*
 * fm_bill_utils.c
 */
EXTERN void 
fm_bill_utils_pre_create_notify PROTO_LIST((
	pcm_context_t   *ctxp,
	pin_flist_t     *c_flistp,
	poid_t          *a_pdp,
	char            *event_type,
	char            *program,
	pin_flist_t     *svc_flistp,
	pin_bill_billing_state_t bill_state,
	pin_errbuf_t    *ebufp));


/*
 * fm_bill_utils_close.c
 */
EXTERN void
fm_utils_close_bill PROTO_LIST((
		pcm_context_t   *ctxp,
		pin_flist_t     *i_flistp,
		pin_errbuf_t    *ebufp));
        
/*
 * set the optional balance group into the event flistp; if not exists, get and
 * set the account level balance group
 * also set the passed sub-balances into it if passed.
 */
EXTERN void 
fm_bill_utils_set_bal_grp(
    pcm_context_t       *ctxp,
    int32                   flags,
    pin_flist_t             *in_flistp,
    pin_flist_t             *evt_flistp,
    pin_errbuf_t            *ebufp);

/*
 * put the balance group into the bal impact flist
 * also put the passed sub-balances into it if passed.
 */
EXTERN void 
fm_bill_utils_put_bal_grp(
    pin_flist_t             	*evt_flistp,
    pin_flist_t             	*bal_imp_flistp,
    pin_errbuf_t            	*ebufp);

/* new API written in C++ for Apollo */
EXTERN void
fm_bill_utils_apply_multi_bal_impact(
	pcm_context_t     	*ctxp,
	u_int32			flags,
	pin_flist_t		*a_flistp,
	int32			flag,
	pin_errbuf_t		*ebufp);

/* fill the item's balance group object */
EXTERN void fm_bill_utils_fill_items_bal_grp(
	pcm_context_t 		*ctxp,
    	u_int32 		flags,
	pin_flist_t 		*item_flistp,
	pin_errbuf_t 		*ebufp);

/* get balances from associated balance group,
   this is for resource reservation */
EXTERN void fm_bill_utils_get_bals_for_reserve(
	pcm_context_t 		*ctxp,
    	u_int32 		flags,
	pin_flist_t 		*in_flistp,
	pin_flist_t 		**out_flistpp,
	pin_errbuf_t 		*ebufp);

EXTERN void
fm_utils_generate_notify_event(
	pcm_context_t		*ctxp,
	pin_flist_t		*a_flistp,
	pin_flist_t		*b_flistp,
	char			*event_type,
	char			*event_name,
	char			*event_descr,
	pin_errbuf_t		*ebufp);

EXTERN void fm_bill_utils_init_cfg_sub_bal_contributor(
	pcm_context_t	*ctxp,
	int64 			database,
	pin_errbuf_t	*ebufp);

/*
 * fm_bill_utils_bal_utils.cpp
 */

EXTERN int32 fm_bill_utils_get_todays_rec_id(void);


/*
 * fm_utils_remit.c
 */

/* remittance fields */
VAR_EXTERN pin_flist_t	*fm_remit_flds_flistp;
/* remittance spec */
VAR_EXTERN cm_cache_t	*fm_remit_spec_cache_p;
/* list of fields that will be read from /account object. */
VAR_EXTERN pin_flist_t	*fm_remit_acct_search_flds_flistp;
/* list of fields that will be read from /profile object. */
VAR_EXTERN pin_flist_t	*fm_remit_profile_search_flds_flistp;

EXTERN void fm_utils_remit_config_remittance_flds(
	pcm_context_t	*ctxp,
	int64		database,
	pin_errbuf_t	*ebufp);

EXTERN void fm_utils_remit_config_remittance_spec(
	pcm_context_t	*ctxp,
	int64		database,
	pin_errbuf_t	*ebufp);

EXTERN void fm_utils_remit_organize_remit_criteria(
        pcm_context_t           *ctxp,
        int64                    database,
        pin_flist_t             *remit_acct_flistp,
        pin_errbuf_t            *ebufp);

EXTERN void fm_utils_remit_get_remit_flds_info(
        pin_flist_t             *criteria_flistp,
        int                     *reserved_type,
        int                     *base_type,
        pin_fld_num_t           *substruct,
        pin_fld_num_t           *remit_field,
        int                     *op,
        pin_errbuf_t            *ebufp);

EXTERN void fm_utils_remit_get_remit_criteria_product_poids(
        pcm_context_t           *ctxp,
        int64                    database,
        pin_flist_t             *criteria_flistp,
        pin_errbuf_t            *ebufp);

EXTERN void fm_bill_utils_set_def_billinfo(
        pcm_context_t           *ctxp,
		u_int32					flags,
        pin_flist_t             *in_flistp,
		pin_fld_num_t			acct_pd_fld,
		pin_fld_num_t			billinfo_pd_fld,
        pin_errbuf_t            *ebufp);

/*
 * fm_bill_utils_tax_utils.c
 */
EXTERN void
fm_bill_utils_tax_apply2_impact(
	int32		fld_flags,
	pin_flist_t	*a_flistp,
	pin_flist_t	*b_flistp,
	pin_flist_t	*tax_flistp,
	pin_flist_t	*res_flistp,
	pin_errbuf_t	*ebufp);

EXTERN void
fm_bill_utils_tax_negate_tax_jurisdiction_element PROTO_LIST((
	pin_flist_t		*tj_flistp,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_bill_utils_update_taxcode(
        pcm_context_t           *ctxp,
        pin_flist_t             *e_flistp,
        pin_flist_t             *b_flistp,
        int32                   elm_id,
        pin_errbuf_t            *ebufp);

/*
 * fm_utils_channel.c
 */
EXTERN void
fm_utils_channel_push PROTO_LIST((
	pcm_context_t		*ctxp,
	u_int			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp));

EXTERN void
fm_utils_channel_precommit PROTO_LIST((
	pcm_context_t		*ctxp,
	u_int			flags,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp));

EXTERN void
fm_utils_channel_postabort PROTO_LIST((
	pcm_context_t		*ctxp,
	u_int			flags,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp));
	

EXTERN int
fm_utils_channel_is_enabled PROTO_LIST(());

/*
 * fm_bill_utils_feature_control.c
 */


EXTERN void
fm_utils_check_feature_status PROTO_LIST((
        pcm_context_t    *ctxp,
        char             *feature_namep,
        pin_errbuf_t     *ebufp));

EXTERN void
fm_utils_log_feature_disabled PROTO_LIST((
        char             *feature_namep,
        pin_errbuf_t     *ebufp));

EXTERN void fm_bal_utils_set_bal_cache(
        pcm_context_t    *ctxp,
        pin_flist_t      *in_flistp,
        pin_flist_t      **out_flistp,
        pin_errbuf_t     *ebufp);

/*
 * fm_bill_utils_iscript_utils.c
 */
EXTERN void
fm_bill_utils_iscript_load_templates(
        pcm_context_t   *ctxp,
        pin_flist_t     *i_flistp,
        pin_errbuf_t    *ebufp );

EXTERN void
fm_bill_utils_iscript_validate_object(
        pcm_context_t   *ctxp,
        poid_t          *b_pdp,
        pin_flist_t     *in_flistp,
        pin_flist_t     **out_flistpp,
        pin_errbuf_t    *ebufp );

EXTERN pin_flist_t *
fm_bill_utils_get_bus_pro_obj(
        pcm_context_t   *ctxp,
        poid_t          *k_pdp,
        pin_errbuf_t    *ebufp);

EXTERN pin_flist_t *
fm_bill_utils_get_template_obj(
        pcm_context_t   *ctxp,
        poid_t          *k_pdp,
        pin_errbuf_t    *ebufp);

EXTERN void
fm_bill_utils_init_config_business_profile PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_bill_utils_init_config_template PROTO_LIST((
	pcm_context_t	*ctxp,
	pin_errbuf_t	*ebufp));

EXTERN void
fm_bill_utils_update_object_cache_residency_value(
        pcm_context_t   *ctxp,
        pin_flist_t     *in_flistp,
        pin_flist_t     *t_flistp,
        pin_errbuf_t    *ebufp );

EXTERN void
fm_bill_utils_iscript_validation(
        pcm_context_t   *ctxp,
        pin_flist_t     *in_flistp,
        pin_flist_t     **out_flistpp,
        pin_errbuf_t    *ebufp );

EXTERN poid_t *
fm_bill_utils_search_bus_pro (
        pcm_context_t   *ctxp,
        poid_t          *k_pdp,
        pin_errbuf_t    *ebufp);

EXTERN void
fm_bill_utils_prep_lock_obj (
		pcm_context_t	*ctxp,
		int32		in_flags,
		pin_flist_t	*in_flistp,
		pin_flist_t	**out_flistpp,
		pin_errbuf_t	*ebufp);

EXTERN void
fm_utils_get_prune_start_end_from_bill_dates(
        pin_flist_t     *c_flistp,
        time_t          *prune_start_t,
        time_t          *prune_end_t,
	pin_errbuf_t	*ebufp);

EXTERN void
fm_utils_bill_prune_range_for_cycle(
        time_t          cycle_start_t,
        time_t          cycle_end_t,
        time_t          *prune_start_t,
        time_t          *prune_end_t);

/*
 *  fm_bill_utils_gl.c
 *
 */

EXTERN void
fm_bill_utils_create_contract_ssp_based_revenue(
	pcm_context_t	*ctxp,
	pin_flist_t	*in_flistp,
	pin_flist_t	**r_flistpp,
	pin_errbuf_t	*ebufp);

EXTERN void
fm_bill_utils_get_revenue_for_event(
	pcm_context_t   *ctxp,
	poid_t		*contract_pdp,
        int32           contract_flags,
	time_t		start_t,
	time_t		end_t,
	pin_flist_t     *e_flistp,
	pin_flist_t     **journal_flistpp,
	pin_errbuf_t    *ebufp);

EXTERN void
fm_bill_utils_close_contract_revenue(
	pcm_context_t	*ctxp,
	pin_flist_t	*in_flistp,
	pin_flist_t	**r_flistpp,
	pin_errbuf_t	*ebufp);

EXTERN  void
fm_bill_utils_create_journal(
        pcm_context_t   *ctxp,
        pin_flist_t     *rev_flistp,
        pin_flist_t     *j_flistp,
        pin_errbuf_t    *ebufp );

EXTERN void
fm_bill_utils_update_rev_dis(
        pcm_context_t   *ctxp,
        pin_flist_t     *rev_flistp,
        pin_flist_t     *j_flistp,
        pin_decimal_t*  contract_amount,
        pin_errbuf_t    *ebufp );

EXTERN  pin_flist_t*
fm_bill_utils_get_revenue_distribution(
        pcm_context_t   *ctxp,
        poid_t          *contract_pdp,
        time_t          start_t,
        time_t          end_t,
        pin_errbuf_t    *ebufp );

EXTERN void
fm_bill_utils_update_journal_earning_date(
	pcm_context_t	*ctxp,
	pin_flist_t	*journal_flistp,
	pin_errbuf_t	*ebufp); 

EXTERN int32 
fm_bill_utils_deal_has_active_offerings(
        pcm_context_t           *ctxp,
        poid_t                  *acct_pdp,
        int32                   package_id,
        poid_t                  *deal_pdp,
        pin_errbuf_t            *ebufp);

EXTERN int32
fm_bill_utils_has_other_active_services(
        pcm_context_t           *ctxp,
        pin_flist_t             *in_flistp,
        poid_t                  *s_pdp,
        pin_errbuf_t            *ebufp);

/*
 *   FM_CONTRACT related utils functions
 *
 */

EXTERN void
fm_bill_utils_create_subscriber_contract(
        pcm_context_t           *ctxp,
        int32                   flags,
        pin_flist_t             *cont_i_flistp,
        pin_flist_t             *sc_flistp,
        pin_flist_t             **cont_o_flistpp,
        pin_errbuf_t            *ebufp);

EXTERN void
fm_bill_utils_read_object(
        pcm_context_t   *ctxp,
        poid_t          *pdp,
        pin_flist_t     **contract_flistpp,
        pin_errbuf_t    *ebufp);

EXTERN package_proceeding_type_t
fm_bill_utils_get_subscriber_contracts(
        pcm_context_t           *ctxp,
        int32                   flags,
        pin_flist_t             *in_flistp,
        pin_flist_t             **ret_flistpp,
        pin_errbuf_t            *ebufp);

EXTERN  int fm_bill_utils_contract_apply_penalty_charges(
        pcm_context_t           *ctxp,
        u_int                   flags,
        pin_flist_t             *i_flistp,
        pin_flist_t             *sc_flistp,
        pin_flist_t             *st_flistp,
        pin_decimal_t           **penalty_charges,
        int32                   *penalty_option,
        pin_fld_tstamp_t        eff_end_t,
        pin_errbuf_t            *ebufp);

EXTERN void fm_bill_utils_renew_contracts(
        pcm_context_t       *ctxp,
        int32               flags,
        pin_flist_t         *i_flistp,
        pin_flist_t         **ren_aft_bill_flistpp,
        pin_errbuf_t        *ebufp);

EXTERN void fm_bill_utils_expire_contracts(
        pcm_context_t       *ctxp,
        int32               flags,
        pin_flist_t         *i_flistp,
        pin_flist_t         **exp_aft_bill_flistpp,
        pin_errbuf_t        *ebufp);

EXTERN pin_flist_t *
fm_utils_get_config_glid(
        pcm_context_t           *ctxp,
        int64             database,
        pin_errbuf_t            *ebufp);

EXTERN void
fm_bill_utils_get_subscriber_contracts_for_acct(
        pcm_context_t           *ctxp,
        int32                   flags,
        pin_flist_t             *in_flistp,
        pin_flist_t             **ret_flistpp,
        pin_errbuf_t            *ebufp);

EXTERN
pin_flist_t *fm_bill_utils_contract_search_active_offerings_flist(
        pcm_context_t           *ctxp,
        poid_t                  *a_pdp,
        int32                   pkg_id,
        poid_t                  *plan_pdp,
        int32                   infld,
        pin_errbuf_t            *ebufp);

EXTERN package_proceeding_type_t
fm_utils_contract_package_proceeding(
        pcm_context_t   *ctxp,
        poid_t          *package_obj,
        pin_errbuf_t    *ebufp);

EXTERN void
fm_utils_contract_prepare_quote(
        pcm_context_t   *ctxp,
        pin_flist_t     *in_flistp,
        pin_flist_t     **r_flistpp,
        pin_errbuf_t    *ebufp);

EXTERN int32
fm_bill_utils_check_cycle_fee_align(
        pcm_context_t   *ctxp,
        poid_t          *pricing_pdp,
        pin_errbuf_t      *ebufp);

EXTERN void
fm_bill_utils_get_all_contracts_by_plan_contract(
        pcm_context_t   *ctxp,
        poid_t          *a_pdp,
        poid_t          *pl_cont_pdp,
        pin_flist_t     **ret_flistpp,
        pin_errbuf_t    *ebufp);

EXTERN void
fm_bill_utils_get_services_by_pkg_ids(
        pcm_context_t   *ctxp,
        int32           infld,
        pin_flist_t     *c_flistp,
        pin_flist_t     *s_flistp,
        poid_t          *pl_cont_pdp,
        pin_flist_t     **ret_flistpp,
        pin_errbuf_t    *ebufp);

EXTERN void
fm_bill_utils_contract_set_status(
        pcm_context_t   *ctxp,
        poid_t          *contract_pdp,
        int32           in_status,
        pin_errbuf_t    *ebufp);

EXTERN void
fm_bill_utils_contract_cancel_if_no_offerings PROTO_LIST((
        pcm_context_t   *ctxp,
        pin_flist_t     *i_flistp,
        pin_fld_num_t   infld,
        poid_t          *deal_pdp,
        int32           package_id,
        pin_errbuf_t    *ebufp));

EXTERN void
fm_bill_utils_get_subscriber_contracts_for_service_if_last(
        pcm_context_t           *ctxp,
	int32			flags,
        pin_flist_t             *in_flistp,
        pin_flist_t             **s_flistpp,
        pin_errbuf_t            *ebufp);

EXTERN int32
fm_bill_utils_is_any_deal_under_services_req_deal(
        pcm_context_t           *ctxp,
        pin_flist_t             *sd_flistp,
        poid_t                  *plan_pdp,
        poid_t                  *s_pdp,
        poid_t                  *deal_pdp,
        pin_errbuf_t            *ebufp);

EXTERN int32
fm_bill_utils_is_req_deals_purchased(
        pcm_context_t           *ctxp,
        poid_t                  *plan_pdp,
        poid_t                  *a_pdp,
        pin_errbuf_t            *ebufp);

EXTERN void  
fm_bill_utils_calc_taxinfo(
	pin_flist_t             *i_flistp, 
	fm_tax_transaction_type_t  trx_type, 
	int32                   currency, 
	pin_decimal_t           *real_amount, 
	pin_decimal_t           *item_total, 
	fm_tax_level_t           level, 
	pin_errbuf_t            *ebufp); 

EXTERN int32
fm_bill_utils_bus_profile_matches_str(
        pcm_context_t   *ctxp,
        poid_t          *bp_pdp,
        char            *key,
        char            *value,
        int             case_insensitive,
        int             default_value,
        pin_errbuf_t    *ebufp);


#undef EXTERN
#undef EXTERN
#undef VAR_EXTERN

#endif /* !_FM_BILL_UTILS_H */
