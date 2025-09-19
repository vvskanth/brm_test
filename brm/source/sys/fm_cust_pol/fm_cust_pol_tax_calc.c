/*******************************************************************
 *
* Copyright (c) 2001, 2023, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char Sccs_Id[] = "@(#)$Id: fm_cust_pol_tax_calc.c /cgbubrm_7.5.0.portalbase/3 2016/05/26 05:35:49 ansiddha Exp $ ";
#endif

/************************************************************************
 * Contains the following operations:
 *
 *   PCM_OP_CUST_POL_TAX_CALC - can be used to calculate taxes using those
 *                              custom tax rates.
 *
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pcm.h"
#include "ops/cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "pin_bill.h"
#include "pin_cust.h"
#include "pin_rate.h"
#include "fm_utils.h"
#include "fm_bill_utils.h"

/************************************************************************
 * Forward declarations.
 ************************************************************************/

extern void
fm_cust_pol_str_to_time_t(
        char                    *str,
        time_t                  *time_ptr,
        int32                   flag);


EXPORT_OP void
op_cust_pol_tax_calc(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
	pin_errbuf_t		*ebufp);

void
fm_cust_pol_tax_calc(
	pcm_context_t		*ctxp,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
	pin_errbuf_t		*ebufp);

void
fm_cust_pol_update_item(
	pcm_context_t		*ctxp,
	poid_t			*item_pdp,
	pin_decimal_t		*tax_amt,
	char			*taxcode,
	pin_flist_t		*ot_flistp,
	pin_errbuf_t		*ebufp);

static void
fm_cust_pol_build_subtotal(
	pin_flist_t		*os_flistp,
	int32			j_level,
	pin_decimal_t		*tax_amt,
	pin_decimal_t		*tax_rate,
	pin_decimal_t		*amt_taxed,
	pin_decimal_t		*amt_exempt,
	pin_decimal_t		*amt_gross,
	char			*name,
	char			*descr,
	int32			loc_mode,
	int32			precision,
	int32			round_mode,
	pin_errbuf_t		*ebufp);

static void
fm_cust_pol_do_juris_only(
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
	pin_errbuf_t		*ebufp);

static pin_decimal_t *
fm_cust_pol_get_amt_exempt(
	pcm_context_t           *ctxp,
	pin_flist_t		*in_flistp,
	pin_flist_t             *t_flistp,
	int32			j_level,
	pin_decimal_t		*g_amt,
	pin_errbuf_t		*ebufp);

static void
fm_cust_pol_parse_tax_locale(
	char			*locale,
	char			*city,
	char			*state,
	char			*zip,
	char			*country,
	char			*code,
	size_t			city_len,
	size_t			state_len,
	size_t			zip_len,
	size_t			country_len,
	size_t			code_len);

static int32 
fm_cust_pol_is_locale_in_juri_list(
	char		*j_list,
	char		*locale);

PIN_IMPORT void fm_cust_pol_load_taxes(
  	pcm_context_t           *ctxp,
  	pin_flist_t             *in_flistp,
  	pin_errbuf_t            *ebufp);

/* Symbol defines */
#define STR_LEN		480
#define TOK_LEN		80
#define RatePrecision	6

/* Global variable (from fm_cust_pol_tax_init)  */
extern pin_flist_t *Tax_Table;
extern int32 refresh_cust_pol_tax_table;
extern u_int32 fm_utils_cfg_config_refresh_required();
extern int32 business_param_pymt_tax_calc;
PIN_IMPORT void fm_utils_tax_get_taxcodes();
PIN_IMPORT int32 tax_return_juris;

/*******************************************************************
 * Main routine for the PCM_OP_CUST_POL_TAX_CALC command
 *******************************************************************/
void
op_cust_pol_tax_calc(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	u_int32         	refresh_needed = PIN_BOOLEAN_FALSE ;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_CUST_POL_TAX_CALC) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_cust_pol_tax_calc opcode error", ebufp);
		return;
	}

	/*
	 * Debug - What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_cust_pol_tax_calc input flist", in_flistp);

	/* If /config/taxcodes_map is updated then Tax_Table should also get updated with the new data */
	if (!pin_conf_dynamic_taxation) {
		refresh_needed = fm_utils_cfg_config_refresh_required (ctxp,
				"/config/taxcodes_map", ebufp);

		if (refresh_needed || refresh_cust_pol_tax_table) {
			fm_cust_pol_load_taxes(ctxp, in_flistp, ebufp);
			refresh_cust_pol_tax_table = PIN_BOOLEAN_FALSE;
		}
	}

	/*
	 * Do the actual op in a sub. 
	 */
	fm_cust_pol_tax_calc(ctxp, in_flistp, out_flistpp, ebufp);

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_DEBUG,
			"op_cust_pol_tax_calc error", ebufp);
	} else {
		/*
		 * Debug: What we're sending back.
		 */
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_cust_pol_tax_calc return flist", *out_flistpp);
	}

	return;
}


/*******************************************************************
 * fm_cust_pol_tax_calc()
 *
 * Use this policy to do your tax calculation using the tax data 
 * that was loaded into the internal data structure that was 
 * declared in fm_cust_pol_tax_init.
 *
 *******************************************************************/
void 
fm_cust_pol_tax_calc(
	pcm_context_t	*ctxp,
	pin_flist_t	*in_flistp,
	pin_flist_t	**out_flistpp,
	pin_errbuf_t	*ebufp)
{
	pin_flist_t	*t_flistp = NULL;   /* in taxes array */
	pin_flist_t	*ot_flistp = NULL;  /* out taxes array */
	pin_flist_t	*os_flistp = NULL;  /* out subtotal array */
	pin_flist_t	*flistp = NULL;
	pin_cookie_t	cookie = NULL;
	pin_cookie_t	cookie2 = NULL;
	int32		elemid = 0;
	int32		elemid2 = 0;
	int32		elemcnt = 0;
	int32		j_level = 0;
	char		*j_list = NULL;
	char		*descr = NULL;
	char		*str_tax_rate = NULL;
	void		*vp = NULL;
	char		*taxcode = NULL;
	char		*locale = NULL;
	char		locale_copy[256] = "";
	char		locale_temp[256] = "";
	char		city[TOK_LEN] = "";
	char		state[TOK_LEN] = "";
	char		zip[TOK_LEN] = "";
	char		county[TOK_LEN] = "";
	char		country[TOK_LEN] = "";
	char		territory[TOK_LEN] = "";
	char		district[TOK_LEN] = "";
	char		code[TOK_LEN] = "";
	char		name[STR_LEN] = "";
	char		msg[STR_LEN+136] = "";
	char		tcode[STR_LEN] = "";
	char		str_date[STR_LEN] = "";
	pin_decimal_t	*tax_rate=NULL; 
	pin_decimal_t	*tot_tax=NULL;
	pin_decimal_t	*tmp_dec = NULL;
	pin_decimal_t	*tax_amt=NULL;
	pin_decimal_t	*amt_gross=NULL;
	pin_decimal_t	*amt_taxed = NULL;
	pin_decimal_t	*amt_exempt = NULL;
	int32 		tax_pkg = PIN_RATE_TAXPKG_CUSTOM;
	int32		month = 0;
	int32		day = 0;
	int32		year = 0;
	int32		tax_rule = 0;
	int32		loc_mode = 0;
	int32		precision = 6;
	int32		round_mode = ROUND_HALF_UP;
	time_t		*startTm = NULL;
	time_t		*endTm = NULL;
	time_t		*transDt = NULL;
	time_t		now = 0;
	poid_t		*item_pdp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/* Create the return flist */
	*out_flistpp = PIN_FLIST_CREATE(ebufp);

	/* Set the account POID */
	vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*out_flistpp, PIN_FLD_POID, (void *)vp, ebufp);

	/* check for jurisdiction only ? */
	vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_COMMAND, 1, ebufp);
	if ((vp != (void*)NULL) && (*(int32*)vp == PIN_CUST_TAX_VAL_JUR)) {
		fm_cust_pol_do_juris_only(in_flistp, out_flistpp, ebufp);
		goto CleanUp;
		/***********/
        }

	/* get the transaction date = END_T */
	transDt = (time_t*) PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_END_T,
		1, ebufp);
	if (transDt == (time_t*)NULL) {
		/* not available. use current time */
		now = pin_virtual_time((time_t*)NULL);
		transDt = &now;
	}

	/* get precision, if any */
	vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_ROUNDING, 1, ebufp);
        if (vp != (void*)NULL) {
		precision = *(int32*)vp;
	}

	/* get rounding mode, if any */
	vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_ROUNDING_MODE, 1, ebufp);
	if (vp != (void*)NULL) {
		round_mode = *(int32*)vp;
	}

	/* Loop through array of incoming taxes */
	while ((t_flistp = PIN_FLIST_ELEM_GET_NEXT(in_flistp, PIN_FLD_TAXES,
		&elemid, 1, &cookie, ebufp)) != (pin_flist_t*)NULL) {

		/* initialize */
		tot_tax = pbo_decimal_from_str("0.0",ebufp);
		tax_amt = (pin_decimal_t*)NULL;
		tax_rate = (pin_decimal_t*)NULL;
		amt_gross = (pin_decimal_t*)NULL;
		amt_taxed = (pin_decimal_t*)NULL;
		amt_exempt = (pin_decimal_t*)NULL;
		j_level = elemcnt = 0;
		descr = "Zero Taxes"; 
		loc_mode = 0;

		/* add a tax array into the return flist */
		ot_flistp = PIN_FLIST_ELEM_ADD(*out_flistpp, 
			PIN_FLD_TAXES, elemid, ebufp);

		/* Add package type to the taxes array */
		PIN_FLIST_FLD_SET(ot_flistp, PIN_FLD_TAXPKG_TYPE,
			(void*)&tax_pkg, ebufp);

		/* add the total taxes to the taxes array */
		tmp_dec = pbo_decimal_round (tot_tax,
			(int32)precision,round_mode,ebufp);
		PIN_FLIST_FLD_PUT(ot_flistp, PIN_FLD_TAX, 
			tmp_dec, ebufp);

		/* get taxcode */
		taxcode = (char*) PIN_FLIST_FLD_GET(t_flistp,
			PIN_FLD_TAX_CODE, 1, ebufp);
		if (taxcode == (char*)NULL) {
			/* no taxcode? then skip */
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
				"No taxcode!  Skipping ...");
			continue;
			/*******/
		}

		if (pin_conf_dynamic_taxation != 1 && strstr(taxcode, "-") != NULL) {
			sscanf(taxcode, "%[^-]-%s", tcode, str_date);

			if (strlen(str_date) == 6) {
				/* Date in the format MMDDYY */
				sscanf(str_date, "%2d%2d%2d", &month, &day, &year);
				pin_snprintf(str_date, sizeof(str_date),
					"%02d/%02d/%02d", month, day, year);
				/* Convert string date to Timestamp */
				fm_cust_pol_str_to_time_t(str_date , &now, 0) ;
				transDt = &now;
				taxcode = tcode;
			}
		}

		/* get tax locale = PIN_FLD_SHIP_TO */
		locale = (char*) PIN_FLIST_FLD_GET(t_flistp,
			PIN_FLD_SHIP_TO, 1, ebufp);
		if (locale == (char*)NULL) {
			/* no tax locale? then skip */
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
				"No SHIP_TO locale!  Skipping ...");
			continue;
			/*******/
		}
		pin_strlcpy(locale_copy, locale, sizeof(locale_copy));

		/* get location mode */
		vp =  PIN_FLIST_FLD_GET(t_flistp,
			PIN_FLD_LOCATION_MODE, 1, ebufp);
		if (vp) {
			loc_mode = *(int32 *)vp;
		}
		/* initialize */
		city[0] = county[0] = state[0] = zip[0] = country[0] = territory[0] = district[0] = '\0';
		code[0] = name[0] = '\0'; 

		/* parse the locale */
		fm_cust_pol_parse_tax_locale(locale, city, state, zip,
			country, code, sizeof(city), sizeof(state),
			sizeof(zip), sizeof(country), sizeof(code));

		/* get taxable amount */
		vp = PIN_FLIST_FLD_GET(t_flistp, 
			PIN_FLD_AMOUNT_TAXED, 1, ebufp);
		if (vp == (void*)NULL) {
			/* no amount?  default to zero */
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
				"No AMOUNT_TAXED!  Defaulting to 0.");
			amt_gross = pbo_decimal_from_str("0.0",ebufp);
		} else {
			amt_gross = pbo_decimal_copy((pin_decimal_t*)vp, ebufp);
		}

		/* construct the taxed jurisdiction */
		if (*code) {
			pin_snprintf(name,sizeof(name), "%s; %s; %s; %s; %s; [%.*s]",
				country, state, city, county, zip, strlen(code), code);
		} else {
			pin_snprintf(name,sizeof(name) ,"%s; %s; %s; %s; %s",
				country, state, city, county, zip);
		}

		elemid2 = 0; cookie2 = NULL;
		/* compute tax for each matching taxcode in the table */
		while ((flistp = PIN_FLIST_ELEM_GET_NEXT(Tax_Table, 
			PIN_FLD_TAXES, &elemid2, 1, &cookie2, ebufp))
						!= (pin_flist_t*)NULL) {

			/* get the taxcode */
			vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_TAX_CODE,
				1, ebufp);

			/* taxcode match? */
			if (vp) {
				if (strcmp((char*)vp, taxcode) != 0) {
					/* taxcode did not match */
					pin_snprintf(msg,sizeof(msg),"Checking taxcode %s with %s. Skipping ...",
						taxcode, (char*)vp);
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);
					continue;
					/*******/
				} else {
					/* taxcode matches */
				pin_snprintf(msg,sizeof(msg),"Checking taxcode %s with %s. Matched!", 
						taxcode, (char*)vp);
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);
				}
			}
			else {
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_WARNING, "no tax code available");	
			}
			/* get validity dates */
			startTm = (time_t*) PIN_FLIST_FLD_GET(flistp,
				PIN_FLD_START_T, 1, ebufp);
			endTm = (time_t*) PIN_FLIST_FLD_GET(flistp,
				PIN_FLD_END_T, 1, ebufp);

			/* tax rate valid? */
			if (startTm && endTm && *startTm && *endTm &&
			    ((*transDt < *startTm) || (*transDt >= *endTm))) {
				/* tax rate is not valid */
				pin_snprintf(msg,sizeof(msg),
				"Taxcode %s has expired rate. Skipping ...\n",
					taxcode);
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);
				continue;
				/*******/
			} else {
				/* tax rate is valid */
				pin_snprintf(msg,sizeof(msg),"Taxcode %s has valid rate!\n",
					taxcode);
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);
			}
			/* get jurisdiction level */
			vp =  PIN_FLIST_FLD_GET(flistp, 
				PIN_FLD_TYPE, 1, ebufp);
			if (vp) {
				j_level = *(int32 *)vp;
			}
			else {
				j_level = 0;
			}
			switch (j_level) {
			case PIN_RATE_TAX_JUR_FED:
				locale = country;
				break;
			case PIN_RATE_TAX_JUR_STATE:
				locale = state;
				break;
			case PIN_RATE_TAX_JUR_COUNTY:
				locale = county;
				break;
			case PIN_RATE_TAX_JUR_CITY:
				locale = city;
				break;
			case PIN_RATE_TAX_JUR_TERRITORY:
				locale = territory;
				break;
			case PIN_RATE_TAX_JUR_DISTRICT:
				locale = district;
				break;
			case PIN_RATE_TAX_JUR_LOC:
				locale = "Loc";
				break;
			default:
				/* unsupported level */
				pin_snprintf(msg,sizeof(msg),
		"Unsupported jurisdiction level %d!  Skipping ...", j_level);
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);
				continue;
				/*******/
			}

			/* check for location mode */
			if (loc_mode != PIN_RATE_LOC_ADDRESS) {
#ifdef MATCH_NPA_ONLY
				code[3] = '\0';
#endif
				locale = code;
			}

			/* get jurisdiction list (nexus) for the level */
			j_list = (char*) PIN_FLIST_FLD_GET(flistp, 
				PIN_FLD_NAME, 1, ebufp);

			/* If the tax_level is "Loc", then it is an 
			 * real-time location based taxation. If 
			 * Zonemap_lineage is passed in the input, 
			 * just pick the substring that matches */
			if ((strcmp(locale,"Loc") == 0)) {
			        pin_strlcpy(locale_temp, locale_copy, sizeof(locale_temp));
				vp = strstr(locale_temp, j_list);
				if(vp){
					locale = vp;
					locale[strlen(j_list)] = '\0';
				}
			}

			if (j_list) {
				/* is locale in jurisdiction list? */
				if (j_list && (strstr(j_list, "*") || 
                                               fm_cust_pol_is_locale_in_juri_list(j_list, locale))) {
					/* locale in jurisdiction list */
					pin_snprintf(msg,sizeof(msg),
			     		"Locale %s is in juris list '%s' for taxcode %s\n",
						locale, j_list, taxcode);
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);

				} else {
					/* locale not in jurisdiction list */
					pin_snprintf(msg,sizeof(msg),
	      				"Locale %s not in juris list '%s' for taxcode %s. Skipping ...\n",
						locale, j_list, taxcode);
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);
					continue;
					/*******/
				} 
			}
			else {
				continue;
			}

			/* get exemptions, if any */
			amt_exempt = fm_cust_pol_get_amt_exempt(ctxp, in_flistp, t_flistp,
				j_level, amt_gross, ebufp);
			
			/* get tax rate */
			vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_PERCENT,
				1, ebufp);
			if (vp == (void*)NULL) {
				/* no tax rate?  default to zero */
				tax_rate = pbo_decimal_from_str("0.0",ebufp);
			} else {
				tax_rate = pbo_decimal_copy((pin_decimal_t*)vp,
					ebufp);
			}
		
			str_tax_rate = pbo_decimal_to_str(tax_rate,ebufp);
			pin_snprintf(msg,sizeof(msg),"Tax rate for taxcode %s is %s.", 
				taxcode,str_tax_rate); 
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);

			/* get tax rule */
			vp =  PIN_FLIST_FLD_GET(flistp, 
				PIN_FLD_TAX_WHEN, 1, ebufp);
			if (vp) {
				tax_rule = *(int32*)vp;
			}
			else {
				tax_rule = 0;
			}

			PIN_FLIST_FLD_SET(ot_flistp, PIN_FLD_FLAGS, &tax_rule, ebufp);
			/* this is the amount that will be taxed */
			/* amt_taxed = amt_gross - amt_exempt; */
			amt_taxed = pbo_decimal_subtract(amt_gross,
				amt_exempt,ebufp);


			/* tax on tax? */
			if (tax_rule == 1 || tax_rule == 5) {
				/* amt_taxed += tot_tax; */
				pbo_decimal_add_assign(amt_taxed,
					tot_tax, ebufp);
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
					"Tax on tax rule applied.");
			}

			/* calculate tax */
			if (tax_rule == 2 || tax_rule == 6) {
				/* The tax_rate is the flat fee */
				/* tax_amt = tax_rate; 
				   tax_rate = 0.0;
				*/
				tax_amt = tax_rate;
				tax_rate = pbo_decimal_from_str("0.0", ebufp);
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
					"Flat fee rule applied.");
			} else if (tax_rule == 3 || tax_rule == 7) {
				/* The tax is included in the amount */
				pin_decimal_t *base_amt = NULL; 
				pin_decimal_t *adj_rate = NULL;

				/* adj_rate = 1 + tax_rate */
				adj_rate = pbo_decimal_from_str("1.0", ebufp);
				pbo_decimal_add_assign(adj_rate, tax_rate,
					ebufp); 
                                /* base_amt = amt_taxed / (1 + tax_rate); */
				base_amt = pbo_decimal_divide(amt_taxed,
					adj_rate, ebufp);
                                /* tax_amt = amt_taxed - base_amt; */
				tax_amt = pbo_decimal_subtract(amt_taxed,
					base_amt, ebufp);
                                /* amt_taxed = base_amt; */
				pbo_decimal_destroy(&amt_taxed);
				amt_taxed = base_amt;
				pbo_decimal_destroy(&adj_rate);
				pbo_decimal_destroy(&amt_gross);
				amt_gross = pbo_decimal_add(amt_taxed, amt_exempt, ebufp);

				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
					"Inclusive tax rule applied.");
			} else {
				/* The tax_rate is a percentage */
				/* tax_amt = amt_taxed * tax_rate; */
				tax_amt = pbo_decimal_multiply(amt_taxed,
					tax_rate, ebufp);
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
					"Standard tax rule applied.");
			}

			/* add a subtotal array into the taxes array */ 
			os_flistp = PIN_FLIST_ELEM_ADD(ot_flistp, 
				PIN_FLD_SUBTOTAL, elemcnt++, ebufp);

			/* include the tax in the total? */
			if (tax_rule >= 0 && tax_rule <= 3) {

				pbo_decimal_round_assign(tax_amt,
					 (int32)precision, round_mode, ebufp );

				PIN_FLIST_FLD_SET(os_flistp, PIN_FLD_TAX, 
					tax_amt, ebufp);

				/* tot_tax += tax_amt; */
				pbo_decimal_add_assign(tot_tax, tax_amt, ebufp);
				
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
					"Cumulative tax rule applied.");
			} else {
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
					"Non-cumulative tax rule applied.");
			}

			/* get tax description */
			descr = (char*) PIN_FLIST_FLD_GET(flistp, 
				PIN_FLD_DESCR, 1, ebufp);

			/* build the subtotals array */
			fm_cust_pol_build_subtotal(os_flistp, j_level,
				tax_amt, tax_rate, amt_taxed, amt_exempt,
				amt_gross, name, descr, loc_mode, precision,
				round_mode, ebufp); 

                	if (amt_taxed) {
                       		 pbo_decimal_destroy(&amt_taxed);
                	}
                	if ( amt_exempt) {
                       		 pbo_decimal_destroy(&amt_exempt);
                	}
                	if ( tax_rate ) {
                       		 pbo_decimal_destroy(&tax_rate);
                	}
                	if ( tax_amt ) {
                        	pbo_decimal_destroy(&tax_amt);
                	}
			if (str_tax_rate ) {
				free(str_tax_rate);
			}

		}

		if (elemcnt == 0) {
			/* No taxes computed.  Just add a (default)
			 * subtotal array into the taxes array.
			 */ 
			os_flistp = PIN_FLIST_ELEM_ADD(ot_flistp, 
				PIN_FLD_SUBTOTAL, elemcnt, ebufp);

			tax_amt = pbo_decimal_from_str("0.0", ebufp);
		        tax_rate = pbo_decimal_from_str("0.0", ebufp);
        		amt_taxed = pbo_decimal_from_str("0.0", ebufp);
        		amt_exempt = pbo_decimal_from_str("0.0", ebufp); 


			/* initialize subtotal array */ 
			fm_cust_pol_build_subtotal(os_flistp, j_level, tax_amt,
				tax_rate, amt_taxed, amt_exempt, amt_gross,
				name, descr, loc_mode, precision, round_mode,
				ebufp); 
		}


		/* add the total taxes to the taxes array */
		tmp_dec = pbo_decimal_round(tot_tax,
			(int32)precision,round_mode,ebufp);
		PIN_FLIST_FLD_PUT(ot_flistp, PIN_FLD_TAX, 
			tmp_dec, ebufp);

		/*check if business param and item poid is there 
 		then update the tax data in item*/
		if ( business_param_pymt_tax_calc ) {
		
			item_pdp = (poid_t*) NULL;
			item_pdp = (poid_t*) PIN_FLIST_FLD_GET ( t_flistp, PIN_FLD_ITEM_OBJ, 1, ebufp);
			/* get taxcode from incoming tax array*/
			vp =  PIN_FLIST_FLD_GET(t_flistp,
				PIN_FLD_TAX_CODE, 1, ebufp);
			if (item_pdp && !PIN_POID_IS_NULL(item_pdp)) {
				fm_cust_pol_update_item (ctxp, item_pdp, pbo_decimal_round(tot_tax,
					(int32)precision,round_mode,ebufp), (char*)vp, ot_flistp, ebufp);
			}
		}
 
		if (amt_gross) {
			pbo_decimal_destroy(&amt_gross);
		}
		if (amt_taxed) {
			pbo_decimal_destroy(&amt_taxed);
		}
		if ( amt_exempt) {
			pbo_decimal_destroy(&amt_exempt);
		}
		if ( tot_tax ) {
			pbo_decimal_destroy(&tot_tax);
		}
		if ( tax_rate ) {
			pbo_decimal_destroy(&tax_rate);
		}
		if ( tax_amt ) {
			pbo_decimal_destroy(&tax_amt);
		}

	} /* end while */

CleanUp:
	
	/* Error? */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_cust_pol_tax_calc error", ebufp);
	}

	return;
}


/*******************************************************************
 * fm_cust_pol_build_subtotal ()
 *
 *******************************************************************/
static void
fm_cust_pol_build_subtotal(
	pin_flist_t		*os_flistp,
	int32			j_level,
	pin_decimal_t		*tax_amt,
	pin_decimal_t		*tax_rate,
	pin_decimal_t		*amt_taxed,
	pin_decimal_t		*amt_exempt,
	pin_decimal_t		*amt_gross,
	char			*name,
	char			*descr,
	int32			loc_mode,
	int32			precision,
	int32			round_mode,
	pin_errbuf_t		*ebufp)
{
	int32		subtype = PIN_RATE_TAX_TYPE_SALES;
	pin_decimal_t		*tmp_dec = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/* fill in the subtotal array */
	PIN_FLIST_FLD_SET(os_flistp, PIN_FLD_TYPE, (void*)&j_level,ebufp);
	PIN_FLIST_FLD_SET(os_flistp, PIN_FLD_NAME, (void*)name, ebufp);
	tmp_dec = pbo_decimal_round(amt_gross,
		 (int32)precision,round_mode,ebufp);
	PIN_FLIST_FLD_PUT(os_flistp, PIN_FLD_AMOUNT_GROSS,
		tmp_dec, ebufp);
	tmp_dec = pbo_decimal_round(tax_amt,
	 	(int32)precision,round_mode,ebufp);
	PIN_FLIST_FLD_PUT(os_flistp, PIN_FLD_TAX,
		tmp_dec, ebufp);
	tmp_dec = pbo_decimal_round(tax_rate, 
		(int32)RatePrecision, round_mode,ebufp);
	PIN_FLIST_FLD_PUT(os_flistp, PIN_FLD_PERCENT,
		tmp_dec, ebufp);
	tmp_dec = pbo_decimal_round(amt_taxed, 
	        (int32)precision,round_mode,ebufp );
	PIN_FLIST_FLD_PUT(os_flistp, PIN_FLD_AMOUNT_TAXED,
		tmp_dec, ebufp);
	tmp_dec = pbo_decimal_round(amt_exempt, 
		(int32)precision,round_mode,ebufp);
	PIN_FLIST_FLD_PUT(os_flistp, PIN_FLD_AMOUNT_EXEMPT,
		tmp_dec, ebufp);
	PIN_FLIST_FLD_SET(os_flistp, PIN_FLD_SUBTYPE, (void*)&subtype, ebufp);
	PIN_FLIST_FLD_SET(os_flistp, PIN_FLD_DESCR, (void*)descr, ebufp);
	PIN_FLIST_FLD_SET(os_flistp, PIN_FLD_LOCATION_MODE,
		(void*)&loc_mode, ebufp);

	return;
}


/*******************************************************************
 * fm_cust_pol_do_juris_only ()
 *
 *******************************************************************/
static void
fm_cust_pol_do_juris_only(
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
	pin_errbuf_t		*ebufp)
{
	int32		result = 1;

	/* no address validation. just return PASSED */
	PIN_FLIST_FLD_SET(*out_flistpp, PIN_FLD_RESULT, (void *)&result, ebufp);

	return;
}


/*******************************************************************
 * fm_cust_pol_get_amt_exempt ()
 *
 *******************************************************************/
static pin_decimal_t *
fm_cust_pol_get_amt_exempt(
	pcm_context_t		*ctxp,
	pin_flist_t		*in_flistp,
	pin_flist_t             *t_flistp,
	int32			j_level,
	pin_decimal_t		*g_amt,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t	*flistp = NULL;
	pin_flist_t     *tmp_flistp = NULL;
	pin_flist_t     *s_flistp = NULL;
	pin_flist_t     *sr_flistp = NULL;
	pin_cookie_t	cookie = NULL;
	int32		s_flags = SRCH_EXACT;
	int32		elemid = 0;
	pin_decimal_t	*rate =NULL; 
	void		*vp = NULL;
	char            s_template[1024] = {'\0'};
	char		*exem_code = NULL;
	char		*level = NULL;
	poid_t          *s_pdp = NULL;
	time_t		startTm = 0;
	time_t		endTm = 0;
	time_t		now = 0;
	
	/* Walk the EXEMPTIONS looking for matching jurisdiction level */
	while ((flistp = PIN_FLIST_ELEM_GET_NEXT(in_flistp, PIN_FLD_EXEMPTIONS,
		&elemid, 1, &cookie, ebufp)) != (pin_flist_t*)NULL) {

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_TYPE, 0, ebufp);
		if (vp && (j_level == *(int32*)vp)) {
			/* Match found */
			rate = (pin_decimal_t*)PIN_FLIST_FLD_GET(flistp,
				PIN_FLD_PERCENT, 0, ebufp);
			return pbo_decimal_multiply(g_amt, rate, ebufp); 
			/*****/
		}
	}
	/* Check for product-level exemption */
	if (t_flistp) {	
		exem_code = (char*)PIN_FLIST_FLD_GET(t_flistp, PIN_FLD_TAX_EXEMPTION_CODE, 1, ebufp);
		if (exem_code && (strcmp(exem_code, "") != 0)) {
			/* Get the data from the config table */
			s_flistp = PIN_FLIST_CREATE(ebufp);

			vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID, 0, ebufp);
                        s_pdp = PIN_POID_CREATE(PIN_POID_GET_DB((poid_t*)vp), "/search", -1, ebufp);
                        PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, s_pdp, ebufp);
                        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_FLAGS, &s_flags, ebufp);

                        /* arg-1 -- matching tax_exempt_code */
                        tmp_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);
			flistp = PIN_FLIST_ELEM_ADD(tmp_flistp, PIN_FLD_EXEMPTIONS, 0, ebufp);
                        PIN_FLIST_FLD_SET(flistp, PIN_FLD_TAX_EXEMPTION_CODE, (void*)exem_code, ebufp);

			/* arg-2 -- matching level */
                        tmp_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 2, ebufp);
			flistp = PIN_FLIST_ELEM_ADD(tmp_flistp, PIN_FLD_EXEMPTIONS, 0, ebufp);
			/* Convert j_level to string format */
			switch (j_level) {
 				case PIN_RATE_TAX_JUR_FED:
 					level = "Fed";
	 				break;
 				case PIN_RATE_TAX_JUR_STATE:
 					level = "Sta";
 					break;
	 			case PIN_RATE_TAX_JUR_COUNTY:
 					level = "Cou";
 					break;
 				case PIN_RATE_TAX_JUR_CITY:
 					level = "Cit";
	 				break;
				case PIN_RATE_TAX_JUR_TERRITORY:
                                	level = "Ter";
                                	break;
                        	case PIN_RATE_TAX_JUR_DISTRICT:
                                	level = "Dis";
                                	break;
 				case PIN_RATE_TAX_JUR_LOC:
 					level = "Loc";
 					break;
	 			default:
					break;
			}
                        PIN_FLIST_FLD_SET(flistp, PIN_FLD_TAX_JURISDICTION, (void*)level, ebufp);

			/* Template */
                        pin_strlcpy(s_template, "select X from /config/taxexemption_code_map where F1 = V1 and F2 = V2",
                                                                      sizeof(s_template));
                        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_TEMPLATE, (void *)s_template, ebufp);

                        /* Results */
                        tmp_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_RESULTS, 0, ebufp);

                        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,"taxexem_code_map search input flist:", s_flistp);

                        /* Perform the search */
                        PCM_OP(ctxp, PCM_OP_SEARCH, 0, s_flistp, &sr_flistp, ebufp);

			PIN_FLIST_DESTROY_EX(&s_flistp, ebufp);

                        if (PIN_ERR_IS_ERR(ebufp)) {
	                        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, "search error", ebufp);
                        }

                        if (sr_flistp) {
                                tmp_flistp = PIN_FLIST_ELEM_GET(sr_flistp, PIN_FLD_RESULTS,
        		                                                        0, 1, ebufp);
				if (tmp_flistp) {
					flistp = PIN_FLIST_ELEM_GET(tmp_flistp, PIN_FLD_EXEMPTIONS, 
										PIN_ELEMID_ANY, 1, ebufp);	
					PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,"taxexem_code_map flist:", flistp);
					if (flistp) {
						vp =  PIN_FLIST_FLD_GET(in_flistp,
									 PIN_FLD_END_T, 1, ebufp);
						if (vp) {
							now =  *(time_t*)vp;
						}
						if (now == 0) {
							/* Not available, use current time */
							now = pin_virtual_time((time_t*)NULL);
						}
						vp = PIN_FLIST_FLD_GET(flistp,
								PIN_FLD_VALID_FROM_STR, 1, ebufp);
						if (vp) {
							fm_cust_pol_str_to_time_t((char *)vp, &startTm, 0);
						}
						vp =  PIN_FLIST_FLD_GET(flistp,
								PIN_FLD_VALID_TO_STR, 1, ebufp);
						if (vp) {
							fm_cust_pol_str_to_time_t((char *)vp, &endTm, 1);
						}
						/* check if dates are valid */
						if ( ((startTm != 0) && (now < startTm)) ||
                                                     ((endTm !=0) && (now >= endTm)) ) {
							PIN_FLIST_DESTROY_EX(&sr_flistp, ebufp);
							PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,"Exemption Dates are not valid!!");
							return pbo_decimal_from_str("0.0",ebufp);
						}
						rate = (pin_decimal_t*)PIN_FLIST_FLD_GET(flistp,
									PIN_FLD_PERCENT, 0, ebufp);
						PIN_FLIST_DESTROY_EX(&sr_flistp, ebufp);
						return pbo_decimal_multiply(g_amt, rate, ebufp); 
					}
				}
			}
		}//code
		PIN_FLIST_DESTROY_EX(&sr_flistp, ebufp);
	}//while

	return pbo_decimal_from_str("0.0",ebufp);
}


/*******************************************************************
 * fm_cust_pol_parse_tax_locale()
 *
 *******************************************************************/
static void
fm_cust_pol_parse_tax_locale(
	char		*locale,
	char		*city,
	char		*state,
	char		*zip,
	char		*country,
	char		*code,
	size_t      city_len,
	size_t      state_len,
	size_t      zip_len,
	size_t      country_len,
	size_t      code_len)
{
	char	*p = NULL;
	char	delim = ';';

	/* format = "city; state; zip; country"  */
	if (locale != (char*)NULL) {
		p = locale;
		fm_utils_tax_parse_fld(&p, city, city_len, delim);
		fm_utils_tax_parse_fld(&p, state, state_len, delim);
		fm_utils_tax_parse_fld(&p, zip, zip_len, delim);
		fm_utils_tax_parse_fld(&p, country, country_len, delim);
		fm_utils_tax_parse_fld(&p, code, code_len, delim);

		/* get rid of any "-" in Zip+4 code */
		p = (char*)strchr(zip, '-');
		while (p && (*p != '\0')) {
			*p = *(p + 1);
			p++;
		}
	}

	/* get rid of square brackets */
	if (code[0] == '[') {
		p = code;
		while ((*(p+1) != ']') && (*(p+1) != ',')) {
			*p = *(p+1);
			p++;
		}
		*p = '\0';
	}

	return;
}



/*******************************************************************
 * fm_cust_pol_is_locale_in_juri_list()
 * This function checks if the given locale is in the 
 * jurisdiction list (';' separeted). It trims the leading/trailing 
 * spaces and ignore case (case insensitive) during comparison. 
 *******************************************************************/
static int32
fm_cust_pol_is_locale_in_juri_list(
        char            *j_list,
        char            *locale){
	
	char *curr = j_list;
	char *end;
	char *next;

	size_t locale_size = strlen(locale);

	/* Trim leading spaces */
	while(isspace(*curr)) curr++;

	/* Compare the given locale with each one in jurisdiction list */
	while ((next=strchr(curr, ';')) != NULL) {

		/* Trim trailing spaces */
		end = next-1;
		while(end > curr && isspace(*end))  end--;
		

		if(locale_size == (end-curr+1) && 
		   strncasecmp(curr, locale, locale_size) == 0){
			return PIN_BOOLEAN_TRUE;
		}

		/* Go for next locale in the list*/
		curr = next + 1;

		/* Trim leading spaces */
		while(isspace(*curr)) curr++;

	}

	/* Check the last one */
	end = curr+strlen(curr)-1;
	while(end > curr && isspace(*end)) end--;
	if(locale_size == (end-curr+1) && 
		strncasecmp(curr, locale, locale_size) == 0){
		return PIN_BOOLEAN_TRUE;
	}

	return	PIN_BOOLEAN_FALSE;
}

void
fm_cust_pol_update_item(
	pcm_context_t		*ctxp,
	poid_t			*item_pdp,
	pin_decimal_t		*tax_amt,
	char			*taxcode,
	pin_flist_t		*ot_flistp,
	pin_errbuf_t		*ebufp) {

	pin_flist_t	*in_flistp = NULL;	
	pin_flist_t	*wout_flistp = NULL;	
	pin_flist_t	*win_flistp = NULL;	
	pin_flist_t	*out_flistp = NULL;	
	pin_flist_t	*tax_data_flistp = NULL;	
	pin_flist_t	*tax_flistp = NULL;
	pin_flist_t	*sub_total_flistp = NULL;
	pin_flist_t	*temp_flistp = NULL;	
	pin_decimal_t	*defferred_amt = NULL;
	pin_decimal_t	*tax = NULL;
	pin_decimal_t	*temp = NULL;
	pin_decimal_t	*zero = NULL;
	pin_decimal_t	*def_amount = NULL;
	pin_cookie_t	cookie = NULL;
	pin_cookie_t	cookie2 = NULL;
	pin_rec_id_t	elemid = 0;
	pin_rec_id_t	elemid2 = 0;
	void		*vp = NULL;
	char		*tax_data_str = NULL;
	char		*taxflist_str = NULL;
	char		*tax_code = NULL;
	char		*tax_juris = NULL;
	char		*descr;
	int64		db_no = 0;
	int32		flistLen = 0;
	int 		update = 0;
	int		count = 0;
	int		create_taxes_array = 0;

        if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }

        PIN_ERRBUF_CLEAR (ebufp);

	zero = pin_decimal("0.0", ebufp);

	PIN_ERR_LOG_POID(PIN_ERR_LEVEL_DEBUG, "Item poid ",
                 item_pdp);
	db_no = PIN_POID_GET_DB (item_pdp);
	
	/*create an input flist to fetch tax data from item*/	
	in_flistp = PIN_FLIST_CREATE(ebufp);
	PIN_FLIST_FLD_SET(in_flistp, PIN_FLD_POID, (void *) item_pdp, ebufp);
	PIN_FLIST_FLD_SET(in_flistp, PIN_FLD_TAX_DATA, vp, ebufp);
	
	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG,
		"input flist: ", in_flistp);


	PCM_OP (ctxp, PCM_OP_READ_FLDS, 0,
                                in_flistp, &out_flistp, ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG,
		" output flist: ", out_flistp);
	
	/*retrieve tax data from output flist*/	
	tax_data_str = (char*) PIN_FLIST_FLD_GET (out_flistp, PIN_FLD_TAX_DATA, 0, ebufp);
	if ( (strcmp (tax_data_str, "") == 0) || tax_data_str == (char*)NULL ) {
		PIN_ERR_LOG_MSG (PIN_ERR_LEVEL_DEBUG, " tax_data_str is not valid ");
		return;
	}
	
	PIN_STR_TO_FLIST (tax_data_str, db_no, &tax_data_flistp, ebufp);
	
	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG,
		"tax_data flist: ", tax_data_flistp);

	if(tax_return_juris == 0){ //Summarize
		/*loop over taxes array if taxcode matched update tax amount*/	
		while ((tax_flistp = PIN_FLIST_ELEM_GET_NEXT(tax_data_flistp,
			PIN_FLD_TAXES, &elemid, 1, &cookie, ebufp))
                                                != (pin_flist_t*)NULL) {
	
			tax_code = (char*) PIN_FLIST_FLD_GET (tax_flistp, PIN_FLD_TAX_CODE, 0, ebufp);
			if (strcmp (taxcode, tax_code) == 0) {
			
				defferred_amt = (pin_decimal_t*) PIN_FLIST_FLD_GET (tax_flistp, 
						PIN_FLD_AMOUNT_DEFERRED, 0, ebufp);
				if (!pbo_decimal_is_zero(defferred_amt, ebufp)) {
				
					tax = (pin_decimal_t*) PIN_FLIST_FLD_GET(tax_flistp, PIN_FLD_TAX, 0, ebufp);
					pbo_decimal_add_assign (tax_amt, tax, ebufp);
					PIN_FLIST_FLD_PUT(tax_flistp, PIN_FLD_TAX, tax_amt, ebufp);
					PIN_FLIST_FLD_SET(tax_flistp, PIN_FLD_TAX_JURISDICTION, "", ebufp);
					update = 1;
					break;
				}	
			}
		}
	}
	else if(tax_return_juris == 1){ //Itemize
		/* Loop over the sub_totals and taxes array, 
		 * if taxcode and tax_jurisdiction(description) matches 
		 * update tax amount or else create new taxes array. */
		count = PIN_FLIST_ELEM_COUNT(tax_data_flistp, PIN_FLD_TAXES, ebufp);

		while((sub_total_flistp = PIN_FLIST_ELEM_GET_NEXT(ot_flistp,
			PIN_FLD_SUBTOTAL, &elemid, 1, &cookie, ebufp))
						!= (pin_flist_t*)NULL) {

			create_taxes_array = 0;
			tax = PIN_FLIST_FLD_GET(sub_total_flistp, PIN_FLD_TAX, 0, ebufp);
			def_amount = PIN_FLIST_FLD_GET(sub_total_flistp, PIN_FLD_AMOUNT_TAXED, 0, ebufp);
			descr = PIN_FLIST_FLD_GET(sub_total_flistp, PIN_FLD_DESCR, 0, ebufp);
			//If Descr is not set, by default it is set to "No Tax Description" in tax_init file.
			//here making it to empty string again.
			if(strcmp (descr, "No Tax Description") == 0){
				pin_strlcpy(descr, "", sizeof(descr));
			}
			elemid2 = 0;
			cookie2 = NULL;
			while ((tax_flistp = PIN_FLIST_ELEM_GET_NEXT(tax_data_flistp,
				PIN_FLD_TAXES, &elemid2, 1, &cookie2, ebufp)) != (pin_flist_t*)NULL) {

				tax_code = (char*) PIN_FLIST_FLD_GET (tax_flistp, PIN_FLD_TAX_CODE, 0, ebufp);
				if (strcmp (taxcode, tax_code) == 0) {
					defferred_amt = (pin_decimal_t*) PIN_FLIST_FLD_GET (tax_flistp,
						PIN_FLD_AMOUNT_DEFERRED, 0, ebufp);

					if (!pbo_decimal_is_zero(defferred_amt, ebufp)) {
						tax_amt = (pin_decimal_t*) PIN_FLIST_FLD_GET(tax_flistp, PIN_FLD_TAX, 0, ebufp);
						tax_juris = PIN_FLIST_FLD_GET(tax_flistp, PIN_FLD_TAX_JURISDICTION, 1, ebufp);

						if(tax_juris == NULL || (strcmp(tax_juris, "")==0)){
							/*If it's default TAXES array, 
							 * then update the tax_jurisdiction and tax amount.*/
							pbo_decimal_add_assign (tax_amt, tax, ebufp);
							PIN_FLIST_FLD_PUT(tax_flistp, PIN_FLD_TAX, tax_amt, ebufp);
							PIN_FLIST_FLD_SET(tax_flistp, PIN_FLD_TAX_JURISDICTION, descr, ebufp);
						}
						else if(descr && tax_juris && (strcmp(tax_juris, descr) == 0)){
							/* If there exists a TAXES array with same taxcode and 
							 * jurisdiction update the tax amount. */
							pbo_decimal_add_assign (tax_amt, tax, ebufp);
							PIN_FLIST_FLD_PUT(tax_flistp, PIN_FLD_TAX, tax_amt, ebufp);
						}
						else{
							// If no TAXES array is present, create a new one.
							create_taxes_array = 1;
						}
						update = 1;
						break;
					}
				}
			}
			if(create_taxes_array){
				//build new PIN_FLD_TAXES under PIN_FLD_TAX_DATA
				temp_flistp = PIN_FLIST_ELEM_ADD(tax_data_flistp,
					PIN_FLD_TAXES, count, ebufp);
				count++;
				PIN_FLIST_FLD_SET(temp_flistp, PIN_FLD_TAX_CODE, taxcode, ebufp);
				PIN_FLIST_FLD_SET(temp_flistp, PIN_FLD_TAX_JURISDICTION, descr, ebufp);
				PIN_FLIST_FLD_SET(temp_flistp, PIN_FLD_AMOUNT_TAXED, zero, ebufp);
				PIN_FLIST_FLD_SET(temp_flistp, PIN_FLD_AMOUNT_DEFERRED, def_amount, ebufp);
				PIN_FLIST_FLD_SET(temp_flistp, PIN_FLD_TAX, tax, ebufp);
				PIN_FLIST_FLD_SET(temp_flistp, PIN_FLD_AMOUNT_TAX_ADJUSTED, zero, ebufp);
				PIN_FLIST_FLD_SET(temp_flistp, PIN_FLD_TAX_RECVD, zero, ebufp);
				update = 1;
			}
		}	
	}

	if (update) {
		PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG,
			"updated tax_data flist", tax_data_flistp);
	
		//Sorting the taxes array based on taxcode.
		pin_flist_t	*sort_flistp = NULL;
		pin_flist_t	*tmp_flistp = NULL;
		sort_flistp = PIN_FLIST_CREATE(ebufp);
		tmp_flistp = PIN_FLIST_ELEM_ADD(sort_flistp, PIN_FLD_TAXES, 0, ebufp);
		PIN_FLIST_FLD_SET(tmp_flistp, PIN_FLD_TAX_CODE, NULL, ebufp);	
		PIN_FLIST_SORT(tax_data_flistp, sort_flistp, 0, ebufp);
		PIN_FLIST_DESTROY_EX(&sort_flistp, NULL);
		PIN_FLIST_TO_STR (tax_data_flistp, &taxflist_str, &flistLen, ebufp);
	
		/*create an input flist to update item*/	
		win_flistp = PIN_FLIST_CREATE(ebufp);
		PIN_FLIST_FLD_SET (win_flistp, PIN_FLD_POID, (void *) item_pdp, ebufp);
	
		PIN_FLIST_FLD_PUT (win_flistp, PIN_FLD_TAX_DATA, taxflist_str, ebufp);	
	
		PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG,
			"updating field input flist: ", win_flistp);
		PCM_OP (ctxp, PCM_OP_WRITE_FLDS, 0,
	                                win_flistp, &wout_flistp, ebufp);
		PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG,
			"output flist after updating field: ", wout_flistp);
		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_MSG (PIN_ERR_LEVEL_DEBUG, " error in updating flist ");
		}
		PIN_FLIST_DESTROY_EX (&win_flistp, NULL);
		PIN_FLIST_DESTROY_EX (&wout_flistp, NULL);
	}
	PIN_FLIST_DESTROY_EX (&tax_data_flistp, NULL);
	PIN_FLIST_DESTROY_EX (&out_flistp, NULL);
	PIN_FLIST_DESTROY_EX (&in_flistp, NULL);
	pbo_decimal_destroy(&zero);
}

