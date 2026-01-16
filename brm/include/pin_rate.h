/*******************************************************************
 * @(#)$Id: pin_rate.h /portalbase/1 2023/08/24 00:27:49 mkothari Exp $
 *      
 * Copyright (c) 1996, 2023, Oracle and/or its affiliates.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef _PIN_RATE_H
#define	_PIN_RATE_H

/*******************************************************************
 * Rating FM Definitions.
 *
 * All values defined here are embedded in the database
 * and therefore *cannot* change!
 *******************************************************************/

/*******************************************************************
 * Object Name Strings
 *******************************************************************/
#define PIN_OBJ_NAME_STAT_TAX 			"PIN Tax Statistics"

/*******************************************************************
 * Object Type Strings
 *******************************************************************/
#define	PIN_OBJ_TYPE_STAT_TAX			"/stat/tax"

/*******************************************************************
 * Rates Used Array Fields (in Event Objects)
 *******************************************************************/
/*
 * PIN_FLD_FLAGS - Tracks information about how rate was applied.
 * Unused Flags: 
 *
 */
#define PIN_RATE_FLG_OVERRIDE_CREDIT_LIMIT	0x01    /* rating to skip credit limit check */
#define PIN_RATE_FLG_PRORATE_CREDIT_LIMIT       0x02    /* rating to prorate balance during purchase */

#define	PIN_RATE_FLG_INVERTED			0x04	/* rate was inverted */
#define	PIN_RATE_FLG_PRO_FIRST			0x08	/* rate was prorated */
#define	PIN_RATE_FLG_PRO_LAST			0x10	/* rate was prorated */
#define PIN_RATE_FLG_DEFERRED_CYCLE_AT_MIDNIGHT 0x20    /* for deferred purchase in
                                                           rerating flow to decide
                                                           items of which cycle to be used */
#define PIN_RATE_FLG_FAIL_ON_PURCHASE   0x40    /* rating to fail during purchase */
#define PIN_RATE_FLG_CYCLE_FEE_RENEWABLE       	0x80 /* renewable flag to be passed in event. */
#define	PIN_RATE_FLG_PRO_NORMAL			0x100 	/* prorate normal */
#define PIN_RATE_FLG_TAXABLE_OVERRIDDEN         0x200 /* Taxable amount different from charge amount */
#define	PIN_RATE_FLG_FAIL_ON_CREDIT_LIMIT	0x400	/* limit only pass */
#define	PIN_RATE_FLG_DEFERRED			0x800	/* defferred cycle fee */
#define PIN_RATE_FLG_CYCLE_CHARGED		0x1000	/* cycle fee charged */ 
#define PIN_RATE_FLG_REACTIVATE			0x2000	/* account reactivate */
#define PIN_RATE_FLG_RATE_ONLY			0x4000	/* rate-only request */
#define PIN_RATE_FLG_RERATE			0x8000	/* re-rate */
#define PIN_RATE_FLG_INADVANCE_BILLING		0x10000 /* inadvance billing */
#define PIN_RATE_FLG_CYCLE_ARREARS_CHARGED	0x20000	/* cycle_arrears chged - used by SUBSCRIPTION*/
#define PIN_RATE_FLG_DISCOUNT_ONLY		0x40000	/* discount-only */
#define PIN_RATE_FLG_PRO_FORCED 		0x80000 /* forced proration */
#define PIN_RATE_FLG_CYCLE_FORWARD_ARREAR	0x100000  /* cfa flag         */
#define PIN_RATE_FLG_PRODUCT_CANCEL		0x200000 /* used for discount */
/* used for re-rating to flag item is billed */
#define PIN_RATE_FLG_ITEM_BILLED		0x400000
/* used to flag the cycle events in the case of multiple tiers due to rate change.
   If there are multiple tiers, for each tier a separate cycle event is produced. 
   Except the first event, the rest of the events will be flagged with this value.
   This is used during re-rating to filter the events. */
#define PIN_RATE_FLG_RATE_CHG			0x800000
/* Used to indicate that we have to refund cycle_arrear fees
   for a charged cycle; this is used in the cycle in which the
   backdated cancellation time of a cycle arrear falls */
#define PIN_RATE_FLG_CYCLE_ARREARS_REFUND       0x1000000
/* Truncate the end date of the bucket(VALID_TO) to the time of cancellation */
#define PIN_RATE_FLG_CUT			0x2000000 /* used by SUBSCRIPTION */
/* Retain the original end date(VALID_TO) of the bucket.
   i.e. no change to the end date */
#define PIN_RATE_FLG_MAINTAIN			0x4000000 /* used by SUBSCRIPTION */
/* Align the end date of the bucket to destination accounts(billinfo's) DOM.
   This is used during Line transfer. */
#define PIN_RATE_FLG_ALIGN			0x8000000 /* used by SUBSCRIPTION */
#define PIN_RATE_FLG_MONITOR_ADJ_RERATE		0x10000000 /* re-rate to act_usage */  
#define PIN_RATE_FLG_STOP_ROLLOVER		0x20000000 /* stop rollover - used by SUBSCRIPTION */	
#define PIN_RATE_FLG_CALC_MAX			0x40000000 /* calc_max rating */  

/* Get the discounts based on a period instead of a timepoint */
#define PIN_RATE_DISCOUNT_GET_BASED_ON_PERIOD 0x10

#define PIN_ACT_NOT_NOTIFY			0x200 /* prevents notification */

/* used in generate monitor impacts opcode, to differ rated vs. pe-rated */
#define PIN_OPFLG_MONITOR_PRE_RATED       	0x01 
#define PIN_OPFLG_MONITOR_RATED       		0x02 

/* used candidate discount list opcode, but get monitor list only */
#define PIN_RATE_INTERN_FLG_MONITOR_ONLY       0x100 

/* price override indicator, in RATE_AND_DISCOUNT_EVENT input */
#define PIN_RATE_TYPE_PRICE_OVERRIDES		1

/* This value is used to indicate that credit limit check should
 * be applied to the event that is being processed.
 */
#define PIN_RATE_CREDIT_LIMIT_NEEDED		0x80

/* credit limit check values in field PIN_FLD_CHECK_CREDIT_LIMIT, used in:
 *  RATE_AND_DISCOUNT_EVENT 
 *  RATE_EVENT
 *  RATE_DISCOUNT_EVENT 
 *
 * PIN_RATE_CHECK_CREDIT_LIMIT, PIN_RATE_CREDIT_LIMIT_LOCATION_CRE, PIN_RATE_CREDIT_LIMIT_LOCATION_RTP,
 * PIN_RATE_NO_CREDIT_LIMIT_DISCOUNT_ONLY and PIN_RATE_CREDIT_LIMIT_CHECK are used for credit limit 
 * check in CRE. These can not be passed by other applications to CRE.
 */

/* This value is used to indicate that credit limit check should
 * be applied to the event that is being processed.
 */

#define PIN_RATE_CHECK_CREDIT_LIMIT		0x1000

/* credit limit check will be enforced in the CRE */
#define PIN_RATE_CREDIT_LIMIT_LOCATION_CRE	0x100

/* credit limit check will be enforced by the realtime pipeline */
#define PIN_RATE_CREDIT_LIMIT_LOCATION_RTP	0x200

/* For events that are processed by the real-time pipeline
 * no credit limit should be applied. The event is sent to the
 * real-time pipeline for discounting only.
 */ 
#define PIN_RATE_NO_CREDIT_LIMIT_DISCOUNT_ONLY	0x0

/* This value is passed to the real-time pipeline to
 * indicate that credit limit check should be applied.
 * In some cases, Reverse rating calculation may be applied
 * if the complete rated quantity cannot be allowed within
 * the user's and/or sponsor's credit limits. 
 */
#define PIN_RATE_CREDIT_LIMIT_CHECK		0x1

/*
 * Those are old define's to used older version project .
 * The values they occupy can be reused by new features. 
 * 
 * They are deprecated from the lastest release (DaVinci) and later. 
 * they can now be removed entirely since the current file is release dependent.
 * #define PIN_RATE_FLG_DEFAULT                    0x01    / rate was default /
 * #define PIN_RATE_FLG_NON_CURRENCY               0x20    / non currency rate/
 * #define PIN_RATE_FLG_ACNT_CURRENCY              0x40    / rate matched /
 * #define PIN_RATE_FLG_SYS_CURRENCY               0x80    / rate matched /
 * #define PIN_RATE_FLG_SPONSOR                    0x200   / sponsor pass /
 * #define PIN_RATE_FLG_MONITOR_ONLY               0x1000000
 * #define PIN_RATE_FLG_MONITOR_PRE_RATED       	0x2000000
 * #define PIN_RATE_FLG_MONITOR_RATED       	0x4000000
 */
/* end of the deprecated block */


/*******************************************************************
 * Rate Object Fields.
 *******************************************************************/
/*
 * PIN_FLD_NAME - Predefined rates applied by the system
 */
#define PIN_RATE_NAME_PURCHASE		"purchase"
#define PIN_RATE_NAME_CANCEL		"cancel"
#define PIN_RATE_NAME_CYCLE_ARREARS	"cycle_arrears"
#define PIN_RATE_NAME_CYCLE_FORWARD	"cycle_forward"
#define PIN_RATE_NAME_MMC_MONTHLY	"cycle_forward_monthly"
#define PIN_RATE_NAME_MMC_BIMONTHLY	"cycle_forward_bimonthly"
#define PIN_RATE_NAME_MMC_QUARTERLY	"cycle_forward_quarterly"
#define PIN_RATE_NAME_MMC_SEMIANNUAL	"cycle_forward_semiannual"
#define PIN_RATE_NAME_MMC_ANNUAL	"cycle_forward_annual"

/* The maximum event quantity allowed by the rating engine */
#define PIN_RATE_MAX_QUANTITY   "1000000000000"

/*
 * PIN_FLD_PRORATE_FIRST & PIN_FLD_PRORATE_LAST
 * Value of first and last parameters for a rate.
 */
typedef enum pin_rate_fstlst {
	PIN_RATE_FSTLST_NA =		0,	/* not applicable */
	PIN_RATE_FSTLST_FULL =		701,
	PIN_RATE_FSTLST_PRORATE =	702,
	PIN_RATE_FSTLST_NONE =		703
} pin_rate_fstlst_t;

/*
 * PIN_FLD_TYPE - Type of rate (is it default or not).
 * (We make the 0 val of the enum normal since type is optional)
 */
typedef enum pin_rate_type {
	PIN_RATE_TYPE_NORMAL =		0,	/* not default */
	PIN_RATE_TYPE_DEFAULT =		740,	/* default rate */
	PIN_RATE_TYPE_DEFAULT_ONLY =	741,	/* default only rate */
	PIN_RATE_TYPE_LOAN =            742,
	PIN_RATE_TYPE_DEDUCT_RENTAL =   743,
	PIN_RATE_TYPE_OUTSTANDING_AMOUNT = 744,
	PIN_RATE_TYPE_RENTAL_FAILURE = 745,
	PIN_RATE_TYPE_SKIP_CYCLE = 746,
	PIN_RATE_TYPE_MINIMUM_AMOUNT = 747,
	PIN_RATE_TYPE_AUTO_RENEW_CANCEL = 748
} pin_rate_type_t;

/*
 * PIN_FLD_TYPE - Type of balance impact for a rate (or debit).
 */
typedef enum pin_bal_impact_type {
	PIN_BAL_IMPACT_TYPE_INVALID =	0,	/* not applicable */
	PIN_BAL_IMPACT_TYPE_REBATE =	880,	/* a "rebate" */
	PIN_BAL_IMPACT_TYPE_BALANCE =	881,	/* a balance change */
	PIN_BAL_IMPACT_TYPE_CASH =	882	/* a cash op */
} pin_bal_impact_type_t;

/*
 * PIN_FLD_FLAG (for each balance impact). 
 */
#define PIN_RATE_BAL_FLG_DISCOUNTABLE	0x01
#define PIN_RATE_BAL_FLG_PRORATABLE	0x02
#define PIN_RATE_BAL_FLG_SPONSORABLE	0x04
#define PIN_RATE_BAL_FLG_GRANTABLE	0x08
#define PIN_RATE_BAL_FLG_TAXABLE_OVERRIDDEN	0x10

/* 
 * PIN_FLD_IMPACT_TYPE - Impact type in the /event bal impacts array.
 */
#define PIN_IMPACT_TYPE_RATED			0x01
#define PIN_IMPACT_TYPE_PRERATED		0x02
#define PIN_IMPACT_TYPE_TAX			0x04
#define PIN_IMPACT_TYPE_PO			0x08
#define PIN_IMPACT_TYPE_TOLERANCE		0x10
#define PIN_IMPACT_TYPE_RERATED			0x20 /* for re-rated bal impacts */
#define PIN_IMPACT_TYPE_RERATED_TAX		0x40 /* for re-rated tax bal impacts */
#define PIN_IMPACT_TYPE_DISCOUNT		0x80 /* for discount bal impacts from pipeline */
#define PIN_IMPACT_TYPE_DISCOUNT_RERATED	0x100 /* for re-rated discount bal impacts from pipeline */
#define PIN_IMPACT_TYPE_INTEGRATE_PRERATED      0x102  /* Propogation from earlier release */
#define PIN_IMPACT_TYPE_INTEGRATE_RERATED       0x120  /* Propogation from earlier release */
#define PIN_IMPACT_TYPE_SHARING			0x200 /* for sharing bal impacts from pipeline */
#define PIN_IMPACT_TYPE_SHARING_RERATED		0x400 /* for re-rated sharing bal impacts from pipeline */
#define PIN_IMPACT_TYPE_ECE_RERATED		0x1000 /* for ECE re-rated bal impacts */
#define PIN_IMPACT_TYPE_ECE_PRERATED		0x2000 /* for ECE pre-rated bal impacts */
#define PIN_IMPACT_TYPE_ECE_RATED		0x4000 /* for ECE rated bal impacts */

/*
 * PIN_IMPACT_CATEGORY - default if there is no impact category.
 */
#define	PIN_IMPACT_CATEGORY_DEFAULT	"default"		
/*******************************************************************
 * Fold Object Fields.
 *******************************************************************/
/*
 * PIN_FLD_TYPE - Type of fold.
 */
typedef enum pin_fold_type {
	PIN_FOLD_TYPE_INVALID =	0,	/* not applicable */
	PIN_FOLD_TYPE_BILLING =	920,	/* a "real" fold for billing */
	PIN_FOLD_TYPE_ACCOUNTING = 921	/* an accounting fold */
} pin_fold_type_t;


/* PIN_FLD_FLAG (for each balance impact).
 */
#define PIN_FOLD_BAL_FLG_SPONSORABLE    0x04

         
/* 
 * PIN_FLD_FLAG - Cancel driven, or cycle driven
 */
#define PIN_FOLD_FLG_CANCEL_DRIVEN	0x01
#define PIN_FOLD_FLG_CYCLE_DRIVEN	0x02

/* 
 * PIN_FLD_FLAG - (for each balance impact). 
 */

#define PIN_FOLD_BAL_FLG_SPONSORABLE	0x04

/*******************************************************************
 * Tax related fields
 *******************************************************************/
#define PIN_RATE_TAX_STAT_SRCH		257

/*
 * PIN_FLD_TYPE - Type of Jurisdiction
 *
 * The enum values are internal to BRM and may map to different values as
 * used in different tax engines. In addition, only a subset of these
 * jurisdictions may be relevant for any given tax engine.
 */
typedef enum pin_rate_tax_jur {
        PIN_RATE_TAX_JUR_FED =		0,
        PIN_RATE_TAX_JUR_STATE =	1, 
	PIN_RATE_TAX_JUR_COUNTY =	2,
	PIN_RATE_TAX_JUR_CITY =		3,
	PIN_RATE_TAX_JUR_SEC_COUNTY =	4,
	PIN_RATE_TAX_JUR_SEC_CITY =	5,
	PIN_RATE_TAX_JUR_TERRITORY =    6,
	PIN_RATE_TAX_JUR_SEC_STATE =    7,
	PIN_RATE_TAX_JUR_DISTRICT =     8,
	PIN_RATE_TAX_JUR_SEC_FED =      9,
	PIN_RATE_TAX_JUR_COUNTY_DISTRICT = 10,
	PIN_RATE_TAX_JUR_CITY_DISTRICT = 11,
	PIN_RATE_TAX_JUR_OTHER =       12,
	PIN_RATE_TAX_JUR_LOC = 		13	/*Real-time location based taxation*/
} pin_rate_tax_jur_t;

/*
 * PIN_FLD_SUBTYPE - Type of Tax (ie, Sales, Use, Rental, Consumer's use, Services)
 */
typedef enum pin_rate_tax_type {
        PIN_RATE_TAX_TYPE_SALES =	0,
        PIN_RATE_TAX_TYPE_USE =		1, 
	PIN_RATE_TAX_TYPE_RENTAL =	2,
	PIN_RATE_TAX_TYPE_C_USE =	3,	/* Consumer's use tax */
	PIN_RATE_TAX_TYPE_SERVICES =	4,	/* Services tax */
	PIN_RATE_TAX_TYPE_UNKNOWN =	5,
	PIN_RATE_TAX_TYPE_NOTAX =	6,
	PIN_RATE_TAX_TYPE_EXEMPT =	7,
	PIN_RATE_TAX_TYPE_OVERRIDE = 	8
} pin_rate_tax_type_t;

/*
 * Taxation switch from pin.conf
 */
typedef enum pin_rate_taxation_switch {
	PIN_RATE_TAX_SWITCH_NONE =	0,
	PIN_RATE_TAX_SWITCH_EVENT =	1,
	PIN_RATE_TAX_SWITCH_CYCLE =	2,
	PIN_RATE_TAX_SWITCH_BOTH =	3
} pin_rate_taxation_switch_t;

/*
 * PIN_FLD_TAX_WHEN - when tax is applied for rate (now, defer, never, dynamic)
 */
typedef enum pin_rate_tax_when {
        PIN_RATE_TAX_WHEN_NEVER =	0,
        PIN_RATE_TAX_WHEN_NOW =		1, 
	PIN_RATE_TAX_WHEN_DEFER =	2,
	PIN_RATE_TAX_WHEN_DYNAMIC =	3
} pin_rate_tax_when_t;

/*
 * PIN_FLD_RESIDENCE_FLAG - residential or business type of account
 */
typedef enum pin_rate_res_flag {
	PIN_RATE_RES_RESIDENTIAL = 	0,
	PIN_RATE_RES_BUSINESS =		1
} pin_rate_res_flag_t;

/*
 * PIN_FLD_INCORPORATED_FLAG - location is inside or outside incorporated 
 *                             area of a city.
 */
typedef enum pin_rate_inc_flag {
	PIN_RATE_INC_INCORPORATED = 	0,
	PIN_RATE_INC_UNINCORPORATED =	1
} pin_rate_inc_flag_t;

/*
 * PIN_FLD_REGULATED_FLAG - company doing the billing is regulated by
 *                          a governement authority. 
 */
typedef enum pin_rate_reg_flag {
	PIN_RATE_REG_REGULATED = 	0,
	PIN_RATE_REG_UNREGULATED =	1
} pin_rate_reg_flag_t;

/*
 * PIN_FLD_TAX_FLAGS - Flag that contains masked values for the following
 *                     flags: residence, incorporated, and regulated. 
 *                     Value should be a power of 2.
 */
#define PIN_RATE_RES_MASK		1 	/* residence flag mask */
#define PIN_RATE_INC_MASK		2   	/* incorporated flag mask */
#define PIN_RATE_REG_MASK		4   	/* regulated flag mask */

/*
 * PIN_FLD_LOCATION_MODE - access mode for a location: by address,  
 *                         geocode, or npa/nxx. 
 */
typedef enum pin_rate_loc_mode {
	PIN_RATE_LOC_ADDRESS = 		0,
	PIN_RATE_LOC_GEOCODE =		1,
	PIN_RATE_LOC_NPANXX = 		2
} pin_rate_loc_mode_t;

/*
 * PIN_FLD_TAXPKG_TYPE - tax package used to calculate taxes
 *
 * Taxware (tax pkg: WORLDTAX and TAXWARE) has been removed since 75PS16. 
 * However, these two packages remain in enum in case people are wondering 
 * why PKG numbers are discontinuous.
 */
typedef enum pin_rate_tax_pkg {
	PIN_RATE_TAXPKG_CUSTOM = 	0,
	PIN_RATE_TAXPKG_TAXWARE = 	1, //obsolete
	PIN_RATE_TAXPKG_WORLDTAX =	2, //obsolete
	PIN_RATE_TAXPKG_QUANTUM =	3,
	PIN_RATE_TAXPKG_COMMTAX =	4,
	PIN_RATE_TAXPKG_EZTAX = 	5, //obsolete
	PIN_RATE_TAXPKG_GENERIC =       10
} pin_rate_tax_pkg_t;

/*******************************************************************
 * PCM_OP_RATE_GET_PRODLIST related fields
 *******************************************************************/
/*
 * PIN_FLD_SCOPE - the scope (or class) of rates being applied
 */
typedef enum pin_rate_scope {
	PIN_RATE_SCOPE_PURCHASE = 1,
	PIN_RATE_SCOPE_CYCLE = 2,
	PIN_RATE_SCOPE_USAGE = 3,
	PIN_RATE_SCOPE_CYCLE_FORWARD = 4,
	PIN_RATE_SCOPE_CYCLE_ARREARS = 5,
	PIN_RATE_SCOPE_CYCLE_FOLD = 6
} pin_rate_scope_t;

/*
 * PIN_FLD_OFFSET_UNIT_OF_MEASURE  - 	the unit of measure for the
 * 					field PIN_FLD_BILL_OFFSET
 */
typedef enum pin_offset_uom {
        PIN_OFFSET_DAYS =   0,
        PIN_OFFSET_MONTHS = 1,
        PIN_OFFSET_WEEKS = 2,
        PIN_OFFSET_YEARS = 3
} pin_offset_uom_t;

/*
 * PIN_FLD_OFFSET_UNIT_OF_MEASURE  - 	the unit of measure for the
 * 					field PIN_FLD_ROLLOVER_UOM 
 */
typedef enum pin_offset_uom_rollover {
        PIN_OFFSET_UOM_MONTHS = 0, 
        PIN_OFFSET_UOM_DAYS = 1,
        PIN_OFFSET_UOM_WEEKS = 2,
        PIN_OFFSET_UOM_YEARS = 3
} pin_offset_uom_rollover_t;


/*
 * PIN_PROD_DATE_RANGE_TYPE  -   Enum to specify the date selection for rate object
 *                                     
 */
typedef enum Pin_Prod_Date_Range_Type {
        PIN_PROD_DATE_RANGE_EVENT_DATE = 0,
        PIN_PROD_DATE_RANGE_PURCHASE_DATE = 1,
        PIN_PROD_DATE_RANGE_INSTANTIATED_DATE = 2 
}Pin_Prod_Date_Range_Type_t;

/***********************************************************************
 * Rate Stack traversal related constants
 * The pass numbers should be in order in which they should be done.
 * Update PIN_RATE_PASS_BEG and PIN_RATE_PASS_END if the order changes.
 ***********************************************************************/
/* Use normal rates that are either non-currency or in account's currency*/
#define PIN_RATE_SPONSOR_PASS_NORM_ACNT_CURRENCY	1
/* Use default rates that are either non-currency or in account's currency*/
#define PIN_RATE_SPONSOR_PASS_DEF_ACNT_CURRENCY		2
/* Use default rates that are either non-currency or in system's currency*/
#define PIN_RATE_SPONSOR_PASS_DEF_SYS_CURRENCY		3

/* Use normal rates that are either non-currency or in account's currency*/
#define PIN_RATE_USER_PASS_NORM_ACNT_CURRENCY		4
/* Use default rates that are either non-currency or in account's currency*/
#define PIN_RATE_USER_PASS_DEF_ACNT_CURRENCY		5
/* Use default rates that are either non-currency or in system's currency*/
#define PIN_RATE_USER_PASS_DEF_SYS_CURRENCY		6

#define PIN_RATE_PASS_BEG	PIN_RATE_SPONSOR_PASS_NORM_ACNT_CURRENCY
#define PIN_RATE_PASS_END	PIN_RATE_USER_PASS_DEF_SYS_CURRENCY	

/*******************************************************************
 * Type of UNITs 
 *******************************************************************/
typedef enum pin_rate_unit {
	PIN_RATE_UNIT_FIRST_USAGE = -1,
	PIN_RATE_UNIT_NONE = 0,
	PIN_RATE_UNIT_SECOND = 1,
	PIN_RATE_UNIT_MINUTE = 2,
	PIN_RATE_UNIT_HOUR = 3,
	PIN_RATE_UNIT_DAY = 4,
	PIN_RATE_UNIT_MONTH = 5,
	PIN_RATE_UNIT_YEAR = 6,
	PIN_RATE_UNIT_WEEK = 10,
	PIN_RATE_UNIT_EVT_CYCLE = 7,
	PIN_RATE_UNIT_ACT_CYCLE = 8,
	PIN_RATE_UNIT_BILL_CYCLE = 9,
	PIN_RATE_UNIT_BYTE = 11,
	PIN_RATE_UNIT_KILOBYTE = 12,
	PIN_RATE_UNIT_MEGABYTE = 13,
	PIN_RATE_UNIT_GIGABYTE = 14,
	PIN_RATE_UNIT_FIXED_CHARGE = 15,	// For price_tag validations.
	PIN_RATE_UNIT_ANY = 16			// For price_tag validations.
} pin_rate_unit_t;

/*******************************************************************
 * Time of day mode
 *******************************************************************/
typedef enum pin_rate_tod_mode {
	PIN_RATE_TOD_STARTTIME = 0,
	PIN_RATE_TOD_ENDTIME = 1,
	PIN_RATE_TOD_TIMED = 2
} pin_rate_tod_mode_t;
	
/*******************************************************************
 * Time of day mode
 *******************************************************************/
typedef enum pin_rate_timezone_mode {
	PIN_RATE_TIMEZONE_EVENT = 0,
	PIN_RATE_TIMEZONE_SERVER = 1,
	PIN_RATE_TIMEZONE_ACCOUNT = 2
} pin_rat_timezone_mode_t;

/*******************************************************************
 * Type of quantities that quantity-tiering is based on
 *******************************************************************/
typedef enum pin_rate_step_type {
	PIN_RATE_TOTAL_QUANTITY_RATED = 0,
	PIN_RATE_QUANTITY_PER_RATE = 1,
	PIN_RATE_RESOURCE_QUANTITY = 2
} pin_rate_step_type_t;

/*******************************************************************
 * Type of date ranges
 *******************************************************************/
typedef enum pin_rate_date_range_type {
	PIN_RATE_DATE_RANGE_ABSOLUTE = 0,
	PIN_RATE_DATE_RANGE_RELATIVE = 1
} pin_rate_date_range_type_t;

/*******************************************************************
 * Type of operator supported in rate plan selector
 *******************************************************************/
typedef enum pin_rate_operator_type {
	PIN_RATE_OPERATOR_EQUAL	= 0,
	PIN_RATE_OPERATOR_IN = 1
} pin_rate_operator_type_t;

/*******************************************************************
 * Status returned as part of op_rate_event return flist after rating
 *******************************************************************/
typedef enum rating_result {
	SUCCESSFUL_RATING = 0,
	ZERO_QUANTITY = 1,
	NO_SCALE_TO_RATE = 10,
	NO_CANDIDATE_RUMS = 11,
	NO_INITIAL_PRODUCTS = 12,
	CALC_MAX_IN_MULTI_RUM = 13,
	NO_MATCHING_RUM = 14,
	NO_QUALIFIED_PRODUCTS = 15,
	NO_RUM = 16,
	STATUS_MISMATCH = 17,
	PRODUCT_NOT_IN_DB = 18,
	NO_PRODUCT_IN_AUDIT_DB = 19,
	NO_RATE_PLAN = 20, 
	NO_MATCHING_SELECTOR_DATA = 21,
	NO_RATING_CURRENCY = 22,
	NO_VALID_RATE_SPAN = 23,	
	NO_VALID_RATE = 24,
	NO_MATCHING_IMPACT_CATEGORY = 25,
	CREDIT_LIMIT_EXCEEDED = 26,
	LIFE_CYCLE_SUSPEND_STATE_MISMATCH = 27,
	MINIMUM_AMOUNT_APPLIED = 28,
	DO_NOT_MOVE_CYCLE_ON_FAILURE = 29
} rating_result_t;

/*******************************************************************
 * Provider Taxes (Type) - Percentage (0), Fixed (1)
 *******************************************************************/
typedef enum prov_tax_type {
	PROV_TAX_TYPE_PERCENTAGE = 0,
	PROV_TAX_TYPE_FIXED      = 1
} prov_tax_type_t;

/*******************************************************************
 * Provider Taxes - Jurisdictions
 *******************************************************************/
typedef enum prov_tax_jur {

	PROV_TAX_JUR_FED             = 0,
        PROV_TAX_JUR_STATE           = 1,
        PROV_TAX_JUR_COUNTY          = 2,
        PROV_TAX_JUR_CITY            = 3,
        PROV_TAX_JUR_SEC_COUNTY      = 4,
        PROV_TAX_JUR_SEC_CITY        = 5,
        PROV_TAX_JUR_TERRITORY       = 6,
        PROV_TAX_JUR_SEC_STATE       = 7,
        PROV_TAX_JUR_DISTRICT        = 8,
        PROV_TAX_JUR_SEC_FED         = 9,
        PROV_TAX_JUR_COUNTY_DISTRICT = 10,
        PROV_TAX_JUR_CITY_DISTRICT   = 11,
        PROV_TAX_JUR_OTHER           = 12

} prov_tax_jur_t;

typedef enum split_validity_mode {
	SPLIT_ALIGN_BUCKET = 5,
	SPLIT_ALIGN_TOTAL = 6
} split_validity_mode_t;

#endif	/*_PIN_RATE_H*/

