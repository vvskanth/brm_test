/* continuus file information --- %full_filespec: psiu_currency.h~6:incl:3 % */
/*
 *  @(#) %full_filespec: psiu_currency.h~6:incl:3 %
 *
* Copyright (c) 1998, 2016, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */


/*
 * This file contains definitions and function prototyes for the 
 * Multi-Currency functionality (Euro functionality)
 * 
 */

#ifndef _PSIU_CURRENCY_H
#define	_PSIU_CURRENCY_H

#include "psiu_list.h"

#ifdef __cplusplus
extern "C" {
#endif


#define EXTERN				extern


/*
 * Currency Typedes, defines, enums
 */


#ifndef TRUE
#define TRUE 	1
#endif /*TRUE*/

#ifndef FALSE
#define FALSE 	0
#endif /*FALSE*/

#ifndef TIME_NEVER
#define TIME_NEVER  (pin_fld_tstamp_t)0
#endif /* TIME_NEVER */


/* 
 *  Global variables definition
 *  
 */


EXTERN pin_flist_t * g_psiu_currency_flist_EMU;
EXTERN u_int32	   * g_psiu_currency_rguiEMU_List;
EXTERN u_int32	     g_psiu_currency_EMU_List_Size;
EXTERN pin_flist_t * g_psiu_currency_flist_Supported_Combos;


/* Currency mode determines which operator to use for currency
 *  exchange rates.  This is necessary because it is illegal with
 *  the EURO and EMU currencies to multiply by the inverse rates.  
 */

typedef enum psiu_currency_operator
{
	PIN_CURRENCY_OPERATOR_MULTIPLY = 0,
	PIN_CURRENCY_OPERATOR_DIVIDE   = 1,
	PIN_CURRENCY_OPERATOR_NONE     = 2
}psiu_currency_operator_t;

 
/*
 ** _currency_exchange_rate
 *
 *  Comments:
 *	Exchange rate value for a particular currency
 *	Used as element in the tables on client and
 *	server for storing a currency exchange rate with
 *	a specific direction and a valid timeframe for which
 *	this rate is valid.  Valid range is between start
 *	time and end time.  If not valid, considered Expired.
 *	This is really a two way rate.  If you want the rate
 *	from eCurrencyB to eCurrencyA then you just divide the
 *	currency instead of multiply.  
 *
 *	SPECIAL CONSISERATIONS:
 *	Note that we CANNOT (for Euro specific rules)
 *	return an inverse rate or 1/uiRateAtoB, but must
 *	have a specific direction associated with the rate returned
 *	by _QUERY_RATES such that the Amount is divided by
 *	the returned uiRateAtoB.
 *
 *
 *  Field Definitions:
 *	uiCurrencyA 	- Currency Type A (see pin_currency.h)
 *	uiCurrencyB 	- Currency Type B (see pin_currency.h)
 *
 *	ddRateAtoB	- Value of rate from A to B
 *			  Value stored in /config/currency/conversionrates
 *	cmOperatorAtoB	- Mode for Rate, determines if operator is  multiply or
 *			  divide for the rate ddRateAtoB.
 *			  Value stored in /config/currency/conversionrates
 *	iRoundingA	- Rounding number of decimal/digits for Currency A
 *			  Value stored in /config/beid
 *	iRoundingB	- Rounding number of decimal/digits for Currency B
 *			  Value stored in /config/beid
 *	iRoundModeA	- Rounding mode for Currency A
 *			  Value stored in /config/beid
 *	iRoundModeB	- Rounding mode for Currency B
 *			  Value stored in /config/beid
 *
 *	ddTolerancePercentA- Tolerance for Currency A expressed as a Percent
 *			     Value stored in /config/beid
 *	ddToleranceAmountA - Tolerance for Currency A expressed as an Amount
 *			     Value stored in /config/beid
 *	ddTolerancePercentB- Tolerance for Currency B expressed as a Percent
 *			     Value stored in /config/beid
 *	ddToleranceAmountB - Tolerance for Currency B expressed as an Amount
 *
 *	tStartTime	- Time stamp after which this rate is valid
 *	tEndTime	- Time stamp after which this rate is invalid
 *
 *	uiFlags		- reserved for future use
 *	szDescr		- string contained in /config/currency/conversionrates
 */

typedef struct _currency_exchange_rate {

	u_int32		uiCurrencyA;
	u_int32		uiCurrencyB;

	pin_decimal_t	*ddRateAtoB;
	psiu_currency_operator_t coOperatorAtoB;
	
	int		iRoundingA;	        
	int		iRoundingB;		
	int32		iRoundModeA;	
	int32		iRoundModeB;	
	pin_decimal_t	*ddTolerancePercentA;	
	pin_decimal_t	*ddToleranceAmtMinA;  	
	pin_decimal_t	*ddToleranceAmtMaxA;  	
	pin_decimal_t	*ddTolerancePercentB;	
	pin_decimal_t	*ddToleranceAmtMinB;  	
	pin_decimal_t	*ddToleranceAmtMaxB;  	
	
	pin_fld_tstamp_t tStartTime;		
	pin_fld_tstamp_t tEndTime;

	u_int32		uiFlags;
	char		szDescr[256];
	
} psiu_currency_exchange_rate_t;

/************************************************************************
 * Functions definitions						*
 ************************************************************************/

#if defined(__STDC__) || defined(PIN_USE_ANSI_HDRS) || defined(__cplusplus)

/*
 * Currency Functions
 *
 *	(implemented in /sys/utils/psiu_currency.c)
 */

/* performs rounding on an amount */
/* same as /sys/fm_std/fm_utils/fm_utils_beid.c fm_utils_round() */
EXTERN void psiu_currency_round(
		double	  *	pddAmount,
		int		iPrecision);

/* worker function for Converting an amount from one currency to another */
EXTERN void psiu_currency_convert_amount(
		pcm_context_t	*ctxp,
		psiu_list_t *	psiu_plist_cerp,
		psiu_currency_exchange_rate_t *	pcerInp,
		u_int32		uiCurrencySrc,
		u_int32		uiCurrencyDst,
		pin_fld_tstamp_t tCurrencyTime,
		pin_decimal_t	*ddAmtSrc,
		pin_decimal_t * *pddAmtDstp,
		pin_errbuf_t *	ebufp);

		
/* worker function for Querying a Rate from the Table/List */
EXTERN void psiu_currency_query_currency_exchange_rate(
		pcm_context_t		*ctxp,
		psiu_list_t *		psiu_plist_cerp,
		u_int32			uiCurrencySrc,
		u_int32			uiCurrencyDst,
		pin_fld_tstamp_t 	tCurrencyTime,
		psiu_currency_exchange_rate_t * *   ppcerReturnpp,
		pin_decimal_t * *	pddRateReturnp,
		psiu_currency_operator_t * pcoOperatorReturnp,
		pin_errbuf_t *		ebufp);


/* Find a rate in the rate table and return pointer to rate and index */
EXTERN void psiu_currency_find_currency_exchange_rate(
		pcm_context_t		*ctxp,
		psiu_list_t *	psiu_plist_cerp,
		u_int32 	uiCurrencySrc, 
		u_int32 	uiCurrencyDst, 
		pin_fld_tstamp_t tTime,
		psiu_currency_exchange_rate_t * *  ppcerRatepp, 
		pin_errbuf_t *	ebufp);

/* Log the CER Structure */
EXTERN void psiu_currency_log_currency_exchange_rate(
		psiu_currency_exchange_rate_t * pcerp);


/*  Insert a Rate into the beginning of the rate table */
EXTERN void psiu_currency_insert_currency_exchange_rate(
		psiu_list_t * 	psiu_plist_cerp,
		psiu_currency_exchange_rate_t *	pcerRatep, 
		pin_errbuf_t *	ebufp);

/*  Append a Rate to the end of the rate table */
EXTERN void psiu_currency_append_currency_exchange_rate(
		psiu_list_t * 	psiu_plist_cerp,
		psiu_currency_exchange_rate_t *	pcerRatep, 
		pin_errbuf_t *	ebufp);

/*  Delete a Rate from the rate table */
EXTERN void psiu_currency_delete_currency_exchange_rate(
		psiu_list_t  * 	psiu_plist_cerp,
		psiu_currency_exchange_rate_t *	pcerRatep, 
		pin_errbuf_t *	ebufp);

/*  Create a rate list/table */
EXTERN void psiu_currency_create_psiu_list(
        	psiu_list_t * psiu_pplist_cerp,
		pin_errbuf_t *	ebufp);

/*  Delete a rate list/table */
EXTERN void psiu_currency_delete_psiu_list(
		psiu_list_t  *	psiu_plist_cerp,
		pin_errbuf_t *	ebufp);


/* is it a legal currency in ISO 4217 (between 1-999) */
EXTERN int psiu_currency_is_currency(
		u_int32 uiCurrency);

/* is the currency one of the EMU members */
EXTERN int psiu_currency_is_emu_currency(
		u_int32 uiCurrency);

/* is this a case we must do triangulation with for conversion of 
   an amount from one currency to another.  Right now, this means
   is Source and destination both EMU currencies, but this definition
   could be expanded in the future */
EXTERN int psiu_currency_is_triangulation_required(
		u_int32 uiCurrencySrc, 
		u_int32 uiCurrencyDst);

/* Builds the global array of EMU currencies g_psiu_currency_rguiEMU_List */
EXTERN void psiu_currency_build_emu_member_list(
		pin_errbuf_t  *	ebufp);

/* gets the /config/currency/emu data in flist format */
EXTERN pin_flist_t * psiu_currency_get_emu_flist(
		pcm_context_t * ctxp, 
		pin_errbuf_t  *	ebufp);

/*
 * Account Functions
 *
 *	(implemented in /sys/utils/psiu_currency.c)
 */

/* get the specified /account object's Primary and Secondary Currencies */
EXTERN void  psiu_currency_get_account_currencies(
		pcm_context_t * ctxp, 
		poid_t	      *	pdpAcctp, 
		u_int32       *	puiCurrencyPrimaryp,
		u_int32       *	puiCurrencySecondaryp,
		pin_errbuf_t  *	ebufp);

/* Get the list of Supported Primary Currencies from 
  /config/currency/supportedcombinations */
EXTERN void  psiu_currency_get_supported_primary_currencies(
		u_int32	      * puiCountPrimariesp,
		u_int32     * *	ppuiCurrenciespp,
		pin_errbuf_t  *	ebufp);

/* Given a Primary currency, get the list of Supported secondary
   Currencies from /config/currency/supportedcombinations */		
EXTERN void  psiu_currency_get_supported_secondary_currencies(
		u_int32        	uiCurrencyPrimary,
		u_int32	      * puiSecondaryRequiredp,
		u_int32	      * puiCountSecondariesp,
		u_int32     * *	ppuiSecondaryCurrenciespp,
		pin_errbuf_t  *	ebufp);

/* Determine if a Primary Currency is supported or not */
EXTERN int   psiu_currency_is_supported_primary_currency(
		u_int32       	uiCurrencyPrimary,
		pin_errbuf_t  *	ebufp);

		
/* Given a Primary Currency, determine if secondary currency 
   is supported or not */
EXTERN int   psiu_currency_is_supported_secondary_currency(
		u_int32 	uiCurrencyPrimary,
		u_int32 	uiCurrencySecondary,
		pin_errbuf_t  *	ebufp);
		
EXTERN int   psiu_currency_is_supported_combination(
		u_int32 	uiCurrencyPrimary,
		u_int32 	uiCurrencySecondaryp,
		pin_errbuf_t  *	ebufp);
		
EXTERN int   psiu_currency_get_default_secondary(
		int32 	uiCurrencyPrimary,
		int32 	*uiCurrencySecondaryp,
		pin_errbuf_t  *	ebufp);
		

/*
 *  Time Functions (useful for when time_t cycles in the year 2039)
 */


EXTERN int psiu_time_LT(   pin_fld_tstamp_t tTestTime, 
	pin_fld_tstamp_t tBaseTime);
EXTERN int psiu_time_LTE(  pin_fld_tstamp_t tTestTime, 
	pin_fld_tstamp_t tBaseTime);
EXTERN int psiu_time_GT(   pin_fld_tstamp_t tTestTime, 
	pin_fld_tstamp_t tBaseTime);
EXTERN int psiu_time_GTE(  pin_fld_tstamp_t tTestTime, 
	pin_fld_tstamp_t tBaseTime);
EXTERN int psiu_time_EQ(   pin_fld_tstamp_t tTestTime, 
	pin_fld_tstamp_t tBaseTime);
EXTERN int psiu_time_Within_Range(pin_fld_tstamp_t tTestTime,
	pin_fld_tstamp_t tStartTime,
	pin_fld_tstamp_t tEndTime);
		

#else	/* ! (defined(__STDC__) || defined(PIN_USE_ANSI_HDRS)) */

EXTERN void psiu_currency_round();
EXTERN void psiu_currency_convert_amount();
EXTERN void psiu_currency_query_currency_exchange_rate();
EXTERN void psiu_currency_find_currency_exchange_rate();
EXTERN void psiu_currency_log_currency_exchange_rate();
EXTERN void psiu_currency_insert_currency_exchange_rate();
EXTERN void psiu_currency_append_currency_exchange_rate();
EXTERN void psiu_currency_delete_currency_exchange_rate();
EXTERN void psiu_currency_create_psiu_list();
EXTERN void psiu_currency_delete_psiu_list();
EXTERN int  psiu_currency_is_currency();
EXTERN int  psiu_currency_is_emu_currency();
EXTERN int  psiu_currency_is_triangulation_required();
EXTERN void psiu_currency_build_emu_member_list();
EXTERN pin_flist_t * psiu_currency_get_emu_flist();
/*
 * Account Functions
 *
 *	(implemented in /sys/utils/psiu_currency.c)
 */

EXTERN void  psiu_currency_get_account_currencies();
EXTERN void  psiu_currency_get_supported_primary_currencies();
EXTERN void  psiu_currency_get_supported_secondary_currencies();
EXTERN int   psiu_currency_is_supported_primary_currency();
EXTERN int   psiu_currency_is_supported_secondary_currency();
EXTERN int   psiu_currency_is_supported_combination();

/*
 *  Time Functions (useful for when time_t cycles in the year 2039)
 */

EXTERN int psiu_time_LT();
EXTERN int psiu_time_LTE();
EXTERN int psiu_time_GT();
EXTERN int psiu_time_GTE();
EXTERN int psiu_time_EQ();
EXTERN int psiu_time_Within_Range();

#endif	/* ! (defined(__STDC__) || defined(PIN_USE_ANSI_HDRS)) */

/*
 * Time Macros (useful for when time_t cycles in the year 2039)
 */

	/*  Less Than */
#define PSIU_TIME_IS_TIME_LESS_THAN(  tTestTime, tBaseTime) 		  \
	psiu_time_LT(tTestTime, tBaseTime)

	/*  Less Than or Equal */
#define PSIU_TIME_IS_TIME_LESS_THAN_OR_EQUAL(  tTestTime, tBaseTime) 	  \
	psiu_time_LTE(tTestTime, tBaseTime)

	/*  Greater Than */
#define PSIU_TIME_IS_TIME_GREATER_THAN(  tTestTime, tBaseTime) 		  \
	psiu_time_GT(tTestTime, tBaseTime)

	/* Greater Than or Equal */
#define PSIU_TIME_IS_TIME_GREATER_THAN_OR_EQUAL(  tTestTime, tBaseTime)   \
	psiu_time_GTE(tTestTime, tBaseTime)

	/* Equal */
#define PSIU_TIME_IS_TIME_EQUAL(  tTestTime, tBaseTime) 		  \
	psiu_time_EQ(tTestTime, tBaseTime)

	/* Within specified time range */
#define PSIU_TIME_IS_TIME_WITHIN_RANGE(   tTestTime, tStartTime, tEndTime)\
	psiu_time_Within_Range(tTestTime, tStartTime, tEndTime)

/*
 * Currency Macros
 */
	/* Round an amount to a precision level */
#define PSIU_CURRENCY_ROUND_AMOUNT(pddAmount, iPrecision) \
	psiu_currency_round(pddAmount, iPrecision)

	/* is it a legal currency in ISO 4217 (between 1-999) ? */
#define PSIU_CURRENCY_IS_CURRENCY(uiCurrency) \
	psiu_currency_is_currency(uiCurrency)

	/* is the currency one of the EMU members ?*/
#define PSIU_CURRENCY_IS_EMU_CURRENCY(uiCurrency) \
	psiu_currency_is_emu_currency(uiCurrency) 

#define PSIU_CURRENCY_IS_TRIANGULATION_REQUIRED(uiCurrencySrc,uiCurrencyDst) \
	psiu_currency_is_triangulation_required(uiCurrencySrc, uiCurrencyDst)

#define PSIU_CURRENCY_IS_SUPPORTED_COMBINATION(uiCurrencyPrimary, 	\
	uiCurrencySecondary,ebufp) 					\
	(((ebufp)->pin_err == PIN_ERR_NONE) ?                         	\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = __FILE__,    	\
		psiu_currency_is_supported_combination(uiCurrencyPrimary,\
			uiCurrencySecondary, ebufp) ): 			\
	(0))


#undef EXTERN

#ifdef __cplusplus
}
#endif

#endif /* !_PSIU_CURRENCY_H */
