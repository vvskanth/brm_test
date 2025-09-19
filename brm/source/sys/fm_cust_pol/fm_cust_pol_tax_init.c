/*******************************************************************
 *
* Copyright (c) 2001, 2024, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char Sccs_Id[] = "@(#)$Id: fm_cust_pol_tax_init.c /cgbubrm_7.5.0.portalbase/4 2016/06/07 11:27:44 mmukeshk Exp $";
#endif

/************************************************************************
 * Contains the following operations:
 *
 *   PCM_OP_CUST_POL_TAX_INIT - can be used to initialize custom tax rates 
 *
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "pcm.h"
#include "ops/cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "pin_cust.h"
#include "pin_rate.h"
#include "fm_utils.h"
#include "fm_bill_utils.h"

#define FILE_SOURCE_ID  "fm_cust_pol_tax_init.c(5)"

/************************************************************************
 * Forward declarations.
 ************************************************************************/
EXPORT_OP void
op_cust_pol_tax_init(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
	pin_errbuf_t		*ebufp);

void 
fm_cust_pol_tax_init(
	pcm_context_t		*ctxp,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
	pin_errbuf_t		*ebufp);

void
fm_cust_pol_tax_init_new(
        pcm_context_t           *ctxp,
        pin_flist_t             *in_flistp,
        pin_flist_t             **out_flistpp,
        pin_errbuf_t            *ebufp);

void
fm_cust_pol_load_taxes(
	pcm_context_t           *ctxp,
	pin_flist_t             *in_flistp,
	pin_errbuf_t            *ebufp);

static void
fm_cust_pol_parse_map_file(
	char			*buf,
	char			*code,
	char			*pkg,
	char			*rate,
	char			*start_t,
	char			*end_t,
	char			*j_level,
	char			*j_list,
	char			*descr,
	char			*rule,
	size_t			code_len,
	size_t			pkg_len,
	size_t			rate_len,
	size_t			start_len,
	size_t			end_len,
	size_t			j_level_len,
	size_t			j_list_len,
	size_t			descr_len,
	size_t			rule_len);

void
fm_cust_pol_str_to_time_t(
	char			*str,
	time_t			*time_ptr,
	int32			flag);

/*******************************************************************
 * Declare any internal data structures to hold the custom tax data.
 * For this default implementation, the rates are stored in an flist
 * with the following specs:
 *
 *    array PIN_FLD_TAXES (
 *       type  = PIN_FLDT_ARRAY,
 *       perms = O,
 *    );
 *    ! The tax code that identifies product to be taxed.
 *    field * PIN_FLD_TAX_CODE (
 *       type  = PIN_FLDT_STR(255),
 *       perms = M,
 *    );
 *    ! The tax rate for the product (in percentage, e.g. 0.50 = 50%)
 *    field * PIN_FLD_PERCENT (
 *       type  = PIN_FLDT_DECIMAL,
 *       perms = M,
 *    );
 *    ! Start of validity date for the tax.
 *    ! Format is "mm/dd/yy".  Example, "02/27/03"
 *    field * PIN_FLD_START_T (
 *       type  = PIN_FLDT_TSTAMP,
 *       perms = M,
 *    );
 *    ! End of validity date for the tax.
 *    ! Format is "mm/dd/yy".  Example, "02/27/03"
 *    field * PIN_FLD_END_T (
 *       type  = PIN_FLDT_TSTAMP,
 *       perms = M,
 *    );
 *    ! Type of jurisdiction (e.g. Fed, State, County, etc.)
 *    ! Valid values are:
 *    !    "Federal"   --> PIN_RATE_TAX_JUR_FED
 *    !    "State"     --> PIN_RATE_TAX_JUR_STATE
 *    !    "County"    --> PIN_RATE_TAX_JUR_COUNTY 
 *    !    "City"      --> PIN_RATE_TAX_JUR_CITY
 *    field * PIN_FLD_TYPE (
 *       type  = PIN_FLDT_ENUM,
 *       perms = M,
 *    );
 *    ! List of valid names for the jurisdiction, separated by 
 *    ! semicolon (";"). This would be the "nexus" for the TYPE.
 *    ! Example:
 *    !   1. If TYPE is "Federal", then list could be:
 *    !        "US; CA; GB; FR"
 *    !   2. If TYPE is "State", then list could be:
 *    !        "CA; NY; TX"
 *    ! Use "*" to denote all values in the domain.
 *    field * PIN_FLD_NAME (
 *       type  = PIN_FLDT_STR(255),
 *       perms = M,
 *    );
 *    ! Name or description of the tax. Example, "VAT"
 *    field * PIN_FLD_DESCR (
 *       type  = PIN_FLDT_STR(54),
 *       perms = O,
 *    );
 *    ! Tax rule (how taxes are to be computed)
 *    ! Possible values are:
 *    !    "Std"   - Standard Tax. Taxes are computed based on the 
 *    !              'taxable amount', and added to the total.
 *    !    "Tax"   - Tax on Tax. Taxes are computed based on previous
 *    !              'taxable amounts' and taxes, and added to the total. 
 *    !    "Fee"   - Tax amount. Use specified amount (in the rate column) as
 *    !              the tax to be added to the total. Similar to a adding a 
 *    !              fee or surcharge regardless of the amount to be taxed.
 *    !    "Inc"   - Inclusive tax. The amount already has the tax included.
 *    !              The taxable amount and tax amount are broken up from the
 *    !              gross amount, and the tax is added to the total. 
 *    !    "NCS"   - Non Cumulative. Taxes are computed base on the 
 *    !              'taxable amount', but are not added to the total.
 *    !    "NCT"   - Non Cumulative Tax on Tax. Taxes are computed based
 *    !              on previous 'taxable amounts' and taxes, but are not
 *    !              added to the total. 
 *    !    "NCF"   - Non Cumulative Fee.  The tax amount will not be added
 *    !              to the total.
 *    !    "NCI"   - Non Cumulative Inclusive. The taxable amount and tax 
 *    !              amount are broken up from the gross amount, but the tax
 *    !              will not be added to the total.
 *    field * PIN_FLD_TAX_WHEN (
 *       type  = PIN_FLDT_ENUM,
 *       perms = M,
 *    );
 *    ! end array TAXES
 *******************************************************************/

#define STR_LEN		256
pin_flist_t *Tax_Table = NULL;
extern pin_flist_t *Provider_Tax_Table ;
extern pin_flist_t *fm_utils_init_cfg_taxcodes_map;


/*******************************************************************
 * Main routine for the PCM_OP_CUST_POL_TAX_INIT command
 *******************************************************************/
void
op_cust_pol_tax_init(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_CUST_POL_TAX_INIT) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_cust_pol_tax_init opcode error", ebufp);
		return;
	}

	/*
	 * Debug - What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_cust_pol_tax_init input flist", in_flistp);

	/*
	 * Do the actual op in a sub. 
	 */
	if(pin_conf_dynamic_taxation == 1){
		/* Dynamic taxation */
		fm_cust_pol_tax_init(ctxp, in_flistp, out_flistpp, ebufp);
	}
	else {
		fm_cust_pol_tax_init_new(ctxp, in_flistp, out_flistpp, ebufp);	
	}

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_DEBUG,
			"op_cust_pol_tax_init error", ebufp);
	} else {
		/*
		 * Debug: What we're sending back.
		 */
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_cust_pol_tax_init return flist", *out_flistpp);
	}

	return;
}


/*******************************************************************
 * fm_cust_pol_tax_init_old()
 *
 * Use this policy to load any custom user tax data into an internal 
 * data structure you may have declared in this file. In this default
 * implementation, the structure will be the Tax_Table global flist.
 *
 *******************************************************************/
void 
fm_cust_pol_tax_init(
	pcm_context_t	*ctxp,
	pin_flist_t	*in_flistp,
	pin_flist_t	**out_flistpp,
	pin_errbuf_t	*ebufp)
{
	int32		errp = 0;
	void		*vp = NULL;
	char		*taxfile = NULL;
	FILE		*fp = NULL;
	char		buf[STR_LEN];
	char		code[STR_LEN];
	char		pkg[STR_LEN];
	char		rate[STR_LEN];
	char		start_t[STR_LEN];
	char		end_t[STR_LEN];
	char		j_level[STR_LEN];
	char		j_list[STR_LEN];
	char		descr[STR_LEN];
	char		rule[STR_LEN];
	pin_decimal_t	*pct = NULL;
	pin_decimal_t   *npct=NULL;
	pin_decimal_t   *dpct=NULL;
	pin_decimal_t   *tmp_dec=NULL;
	size_t		len = 0;
	int32		lvl = 0;
	int32		trl = 0;
	int32		elemid = 0;
	int32		precision = 6;
	time_t		startTm = 0;
	time_t		endTm = 0;
	pin_flist_t	*flistp = NULL;
	int64		db_no = 0 ;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/* Create the return flist */
	*out_flistpp = PIN_FLIST_CREATE(ebufp);

	/* Set the account POID */
	vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*out_flistpp, PIN_FLD_POID, (void *)vp, ebufp);
	db_no = PIN_POID_GET_DB((poid_t*)vp);

	if (Tax_Table == (pin_flist_t*)NULL) {
		/* Create the flist to hold the tax rates */
		Tax_Table = (pin_flist_t*)PIN_FLIST_CREATE(ebufp);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
			"Creating the Tax_Table");
	} else {
		/* the tax table has been initialized */
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_WARNING, 
			"The Tax_Table has already been created!");
		return;
		/*****/
	}

	/* 
	 * Build Tax_Table from external table, which in this 
	 * implementation happens to be the same taxcodes_map 
	 * file specified in the cm/pin.conf
	 */

	/* get the taxcodes_map path from the pin.conf */
	pin_conf("fm_rate", "taxcodes_map", PIN_FLDT_STR,
		(caddr_t*)&taxfile, &errp);
	if (errp != PIN_ERR_NONE) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
			"taxcodes_map table config error!");
		return;
		/*****/
	}

	/* Let's redefine the taxcodes_map table so that the following
	 * columns:
	 *
	 * # Taxcode Pkg  Code1 Code2 Si      Wt_code  Cmdty_code
	 * # ------- ---  ----- ----- ------  -------  ----------
	 *   usage  : Q :  01 :  01 :     S :        :
	 *   VAT    : U : 4.5 :     :       :        :
	 *
	 * will now become these:
	 *
	 * # Taxcode Pkg  Rate  Start      End        Lvl   List    Desc    Rule
	 * # ------- ---- ----  --------   --------   ---   -----   ------  ----
	 *   VAT    : U : 5.0 : 02/01/01 : 01/31/02 : Fed : GB;FR : VAT-EU : Std
	 *   VAT    : U : 4.5 : 02/01/02 : 01/31/08 : Fed : GB    : VAT-GB : Std
	 *   VAT    : U : 4.0 : 02/01/02 : 01/31/08 : Fed : FR    : VAT-FR : Std
	 */

	fp = (FILE*) fopen(taxfile, "r");
	free(taxfile);
	if (fp == (FILE*)NULL) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
			"Failed to open taxcodes_map file!");
		goto Done;
		/********/
	}

	while (fgets(buf, STR_LEN, fp) != NULL) {

		if (strchr(buf, '#') || *buf == '\n') {
			/* skip comments and blank lines*/
			continue;
		}

		/* Strip newline, if it is there */
		len = strlen(buf);
		if (buf[len-1] == '\n') {
			buf[len-1] = '\0';
		}

		code[0] = pkg[0] = rate[0] = start_t[0] = end_t[0] = '\0';
		j_level[0] = j_list[0] = descr[0] = rule[0] = '\0';

		/* Parse the line in the taxcodes_map */
		fm_cust_pol_parse_map_file(buf, code, pkg, rate,
			start_t, end_t, j_level, j_list, descr, rule,
			sizeof(code), sizeof(pkg), sizeof(rate),
			sizeof(start_t), sizeof(end_t), sizeof(j_level),
			sizeof(j_list),	sizeof(descr), sizeof(rule));

		if (strncmp(pkg, "U", 1) != 0) {
			/* skip entries other than "user" */
			continue;
			/*******/
		}

		/* Start of validity date */
		fm_cust_pol_str_to_time_t(start_t, &startTm, 0);

		/* End of validity date */
		fm_cust_pol_str_to_time_t(end_t, &endTm, 1);

		/* Jurisdiction level */
		if (strncmp("Federal", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_FED;
		} else if (strncmp("State", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_STATE;
		} else if (strncmp("County", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_COUNTY;
		} else if (strncmp("City", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_CITY;
		} else if (strncmp("Territory", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_TERRITORY;
		} else if (strncmp("District", j_level, 3) == 0){
			lvl = PIN_RATE_TAX_JUR_DISTRICT;
		} else if (strncmp("Loc", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_LOC;
		} else {
			lvl = PIN_RATE_TAX_JUR_FED;
		}

		/* Tax rule (how to tax) */
		if (strncmp(rule, "Std", 3) == 0) {
			trl = 0; /* standard tax */
		} else if (strncmp(rule, "Tax", 3) == 0) {
			trl = 1; /* tax on tax */
		} else if (strncmp(rule, "Fee", 3) == 0) {
			trl = 2; /* flat fee */
		} else if (strncmp(rule, "Inc", 3) == 0) {
			trl = 3; /* inclusive tax */
		} else if (strncmp(rule, "NCS", 3) == 0) {
			trl = 4; /* non-cumulative standard tax */
		} else if (strncmp(rule, "NCT", 3) == 0) {
			trl = 5; /* non-cumulative tax on tax */
		} else if (strncmp(rule, "NCF", 3) == 0) {
			trl = 6; /* non-cumulative flat fee */
		} else if (strncmp(rule, "NCI", 3) == 0) {
			trl = 7; /* non-cumulative inclusive */
		} else {
			trl = 0; /* default */
		}

		/* Tax rate */
		if (trl == 2 || trl == 6) {
			/* 'rate' is a flat fee */
			pct = pbo_decimal_from_str(rate,ebufp);
		} else {
			/* 'rate' is a percentage */
			npct = pbo_decimal_from_str(rate,ebufp);
			dpct = pbo_decimal_from_str("100.0",ebufp); 
			pct = pbo_decimal_divide( npct, dpct ,ebufp);
		}

		/* Set some defaults */
		if (!*j_list) {
			pin_strlcpy(j_list, "*",sizeof(j_list));
		}
		if (!*descr) {
			pin_strlcpy(descr, "No Tax Description",sizeof(descr));
		}

		flistp = PIN_FLIST_ELEM_ADD(Tax_Table, PIN_FLD_TAXES, 
			elemid++, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_TAX_CODE, 
			(void*)code, ebufp);
		tmp_dec = pbo_decimal_round(pct, 
			(int32)precision,ROUND_HALF_UP,ebufp);
		PIN_FLIST_FLD_PUT(flistp, PIN_FLD_PERCENT, tmp_dec, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_START_T,
			(void*)&startTm, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_END_T, (void*)&endTm, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_TYPE, (void*)&lvl, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_NAME, (void*)j_list, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_DESCR, (void*)descr, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_TAX_WHEN, (void*)&trl, ebufp);
		
		pbo_decimal_destroy(&npct);
		pbo_decimal_destroy(&dpct);
		pbo_decimal_destroy(&pct);

	} /* end while */

	fclose(fp);
Done:

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
		"Tax Table from taxcodes_map file", Tax_Table);

	return;
}

/*******************************************************************
 * fm_cust_pol_tax_init_new()
 *
 * Use this policy to load any custom user tax data into an internal 
 * data structure you may have declared in this file. In this default
 * implementation, the structure will be the Tax_Table global flist.
 *
 *******************************************************************/
void 
fm_cust_pol_tax_init_new(
	pcm_context_t		*ctxp,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
	pin_errbuf_t		*ebufp)
{
	void		*vp = NULL;
	int64		db_no = 0 ;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/* Create the return flist */
	*out_flistpp = PIN_FLIST_CREATE(ebufp);

	/* Set the account POID */
	vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*out_flistpp, PIN_FLD_POID, (void *)vp, ebufp);
	db_no = PIN_POID_GET_DB((poid_t*)vp);

	fm_cust_pol_load_taxes(ctxp, in_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_DEBUG,
			"fm_cust_pol_tax_init error", ebufp);
	}

	return;
}

/*****************************************************************
 * This function load the data for /config/taxcodes_map object and
 * also updates the tax_table with the required information 
 *****************************************************************/
void
fm_cust_pol_load_taxes(
	pcm_context_t           *ctxp,
	pin_flist_t             *in_flistp,
	pin_errbuf_t            *ebufp)
{
	void		*vp = NULL;
	char		code[STR_LEN];
	char		pkg[STR_LEN];
	char		rate[STR_LEN];
	char		j_level[STR_LEN];
	char		j_list[STR_LEN];
	char		descr[STR_LEN];
	char		rule[STR_LEN];
	char		*strp = NULL;
	int32		len = 0;
	int32		lvl = 0;
	int32		trl = 0;
	int32		elemid = 0;
	int32		recid = 0;
	pin_cookie_t	cookie = NULL;
	int32		precision = 6;
	int64           db_no = 0 ;
	time_t		startTm = 0;
	time_t		endTm = 0;
	pin_flist_t	*flistp = NULL;
	pin_flist_t	*taxcodes_flistp = NULL;
	u_int32		refresh_needed = PIN_BOOLEAN_FALSE;
	pin_decimal_t	*pct = NULL;
	pin_decimal_t	*npct=NULL;
	pin_decimal_t	*dpct=NULL;
	pin_decimal_t	*tmp_dec=NULL;

	/* Set the account POID */
	vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID, 0, ebufp);
	db_no = PIN_POID_GET_DB((poid_t*)vp);

	if (Tax_Table == (pin_flist_t*)NULL) {
		/* Create the flist to hold the tax rates */
		Tax_Table = (pin_flist_t*)PIN_FLIST_CREATE(ebufp);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
					"Creating the Tax_Table");
	} else {
		PIN_FLIST_DESTROY_EX(&Tax_Table, NULL);
		Tax_Table = (pin_flist_t*)PIN_FLIST_CREATE(ebufp);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
			"Tax configuration changed, Recreate tax table...");
	}

	fm_utils_cfg_get_taxcode_map(ctxp, db_no, ebufp);
	taxcodes_flistp = PIN_FLIST_COPY (fm_utils_init_cfg_taxcodes_map, ebufp);

	while ((flistp = PIN_FLIST_ELEM_GET_NEXT(taxcodes_flistp, PIN_FLD_TAXES,
					&recid, 1, &cookie, ebufp)) != NULL) {

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_TAXPKG_TAX_CODE, 0, ebufp);
		if ((vp != NULL) && (strncmp((char *)vp, "U", 1) != 0)) {
			/* skip entries other than "user" */
			continue;
			/*******/
		}
		pin_strlcpy(pkg, (char *)vp, sizeof(pkg));

		/* Tax code */
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_TAX_CODE, 0, ebufp);
		if (vp != NULL) {
			pin_strlcpy(code, (char *)vp, sizeof(code));
		}

		/* End of validity date */
		strp = (char *) PIN_FLIST_FLD_GET(flistp, 
					PIN_FLD_VALID_TO_STR, 1, ebufp);
		fm_cust_pol_str_to_time_t(strp, &endTm, 1);

		/* Start of validity date */
		strp = (char *) PIN_FLIST_FLD_GET(flistp, 
					PIN_FLD_VALID_FROM_STR, 1, ebufp);
		fm_cust_pol_str_to_time_t(strp, &startTm, 0);

		/* Jurisdiction level */
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_TAX_JURISDICTION, 1, ebufp);
		if ((vp != NULL) && (strlen((char *)vp) != 0)) {
			pin_strlcpy(j_level, (char *)vp, sizeof(j_level));
		}

		/* Jurisdiction level */
		if (strncmp("Federal", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_FED;
		} else if (strncmp("State", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_STATE;
		} else if (strncmp("County", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_COUNTY;
		} else if (strncmp("City", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_CITY;
		} else if (strncmp("Territory", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_TERRITORY;
		} else if (strncmp("District", j_level, 3) == 0){
			lvl = PIN_RATE_TAX_JUR_DISTRICT;
		} else if (strncmp("Loc", j_level, 3) == 0) {
			lvl = PIN_RATE_TAX_JUR_LOC;
		} else {
			lvl = PIN_RATE_TAX_JUR_FED;
		}

		/* Get the Tax rule */
                vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_RULE_TYPE, 1, ebufp);
                if (vp && strlen((char *)vp) != 0) {
                        pin_strlcpy(rule, (char *)vp, sizeof(rule));
                }
		
		/* Tax rule (how to tax) */
		if (strncmp(rule, "Std", 3) == 0) {
			trl = 0; /* standard tax */
		} else if (strncmp(rule, "Tax", 3) == 0) {
			trl = 1; /* tax on tax */
		} else if (strncmp(rule, "Fee", 3) == 0) {
			trl = 2; /* flat fee */
		} else if (strncmp(rule, "Inc", 3) == 0) {
			trl = 3; /* inclusive tax */
		} else if (strncmp(rule, "NCS", 3) == 0) {
			trl = 4; /* non-cumulative standard tax */
		} else if (strncmp(rule, "NCT", 3) == 0) {
			trl = 5; /* non-cumulative tax on tax */
		} else if (strncmp(rule, "NCF", 3) == 0) {
			trl = 6; /* non-cumulative flat fee */
		} else if (strncmp(rule, "NCI", 3) == 0) {
			trl = 7; /* non-cumulative inclusive */
		} else {
			trl = 0; /* default */
		}

		/* Consider code1 data as percentage, if its non-zero */
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_CODE1, 1, ebufp);
		if (!pbo_decimal_is_zero((pin_decimal_t *)vp, ebufp)) {
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_PERCENT, (void *)vp, ebufp);	
		}

		/* Tax rate */
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_PERCENT, 1, ebufp);
		if (vp != NULL) {
			strp = pbo_decimal_to_str(vp, ebufp);
			pin_strlcpy(rate, strp, sizeof(rate));
			PIN_FREE_EX(&strp);
		}
		
		if (trl == 2 || trl == 6) {
			/* 'rate' is a flat fee */
			pct = pbo_decimal_from_str(rate,ebufp);
		} else {
			/* 'rate' is a percentage */
			npct = pbo_decimal_from_str(rate,ebufp);
			dpct = pbo_decimal_from_str("100.0",ebufp); 
			pct = pbo_decimal_divide( npct, dpct ,ebufp);
		}
		
		/* Set some defaults */
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_TAX_JURISDICTION_LIST, 1, ebufp);
		if ((vp != NULL) && (strlen((char *)vp) != 0)) {
			pin_strlcpy(j_list, (char *)vp, sizeof(j_list));
		}
		else {
			pin_strlcpy(j_list, "*",sizeof(j_list));
		}
		
		/* DESCR  */
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_DESCR, 1, ebufp);
		if ((vp != NULL) && (strlen((char *)vp) != 0)) {
			pin_strlcpy(descr, (char *)vp, sizeof(descr));
		}
		else {
			pin_strlcpy(descr, "No Tax Description",sizeof(descr));
		}
		
		/* Create Tax_Table */
		flistp = PIN_FLIST_ELEM_ADD(Tax_Table, PIN_FLD_TAXES, 
					elemid++, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_TAX_CODE, 
					(void*)code, ebufp);
		tmp_dec = pbo_decimal_round(pct, 
				(int32)precision,ROUND_HALF_UP,ebufp);
		PIN_FLIST_FLD_PUT(flistp, PIN_FLD_PERCENT, tmp_dec, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_START_T,
					(void*)&startTm, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_END_T, (void*)&endTm, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_TYPE, (void*)&lvl, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_NAME, (void*)j_list, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_DESCR, (void*)descr, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_TAX_WHEN, (void*)&trl, ebufp);
			
		pbo_decimal_destroy(&npct);
		pbo_decimal_destroy(&dpct);
		pbo_decimal_destroy(&pct);
		
	} /* end while */

Done:
	PIN_FLIST_DESTROY_EX(&taxcodes_flistp, NULL);
	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_DEBUG,
                        "fm_cust_pol_load_taxes error", ebufp);
        }

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
		"fm_cust_pol_load_taxes : Tax Table from taxcodes_map file", Tax_Table);

	return;
}

/*******************************************************************
 * fm_cust_pol_parse_map_file ()
 *
 *******************************************************************/
static void
fm_cust_pol_parse_map_file(
	char			*buf,
	char			*code,
	char			*pkg,
	char			*rate,
	char			*start_t,
	char			*end_t,
	char			*j_level,
	char			*j_list,
	char			*descr,
	char			*rule,
	size_t			code_len,
	size_t			pkg_len,
	size_t			rate_len,
	size_t			start_len,
	size_t			end_len,
	size_t			j_level_len,
	size_t			j_list_len,
	size_t			descr_len,
	size_t			rule_len)
{
	char	*p = NULL;
	char	delim = ':';

	if (buf != (char*)NULL) {
		p = buf;
		fm_utils_tax_parse_fld(&p, code, code_len, delim);
		fm_utils_tax_parse_fld(&p, pkg, pkg_len, delim);
		fm_utils_tax_parse_fld(&p, rate, rate_len, delim);
		fm_utils_tax_parse_fld(&p, start_t, start_len, delim);
		fm_utils_tax_parse_fld(&p, end_t, end_len, delim);
		fm_utils_tax_parse_fld(&p, j_level, j_level_len, delim);
		fm_utils_tax_parse_fld(&p, j_list, j_list_len, delim);
		fm_utils_tax_parse_fld(&p, descr, descr_len, delim);
		fm_utils_tax_parse_fld(&p, rule, rule_len, delim);
	}

	return;
}


/*******************************************************************
 * fm_cust_pol_str_to_time_t ()
 *
 *******************************************************************/
void
fm_cust_pol_str_to_time_t(
	char			*str,
	time_t			*time_ptr,
	int32			flag)
{
	struct tm	*tm = 0;
	time_t		cur_time = 0;
	int32		dd = 0;
	int32		mm = 0;
	int32		yy = 0;
	char		delim[5] = {0};
	pin_errbuf_t	*ebufp = NULL;

	*time_ptr = 0;
	if (*str == '\0') {
		if (flag) {
			pin_strlcpy(str, "01/01/2038",STR_LEN);
		} else {
			pin_strlcpy(str, "01/01/1970",STR_LEN);
		}
	}

	/* parse the date */
	sscanf(str, "%d%01[/.-]%d%01[/.-]%d", &mm, delim, &dd, delim, &yy);

	/* get virtual time to set defaults */
	pin_virtual_time(&cur_time);
	tm = localtime(&cur_time);
	if(!tm) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
		PIN_ERRCLASS_SYSTEM_DETERMINATE, PIN_ERR_BAD_VALUE,
			0, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"NULL pointer returned from localtime", ebufp);
		return;
	}

	if (yy < 70) {
		/* assume years < 70 are after 2000 */
		yy += 100;
	} else if (yy > 1900) {
		yy -= 1900;
	}
	tm->tm_mday = dd;
	tm->tm_mon = mm - 1;
	tm->tm_year = yy;
	tm->tm_sec = 0;
	tm->tm_min = 0;
	tm->tm_hour = 0;
	tm->tm_isdst = -1;
	if (flag) {
		/* make it the end of the day */
		tm->tm_mday = dd + 1;
		tm->tm_sec = -1;
	}
	*time_ptr = mktime(tm);

	return;
}
