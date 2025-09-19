/*******************************************************************
 *
* Copyright (c) 1998, 2016, 2023 Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its subsidiaries or licensors and may be used, reproduced, stored
 *      or transmitted only in accordance with a valid Oracle license or
 *      sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: fm_rate_pol_map_tax_supplier.c /cgbubrm_7.5.0.portalbase/2 2023/05/02 13:26:49 sreejs Exp $";
#endif

/*******************************************************************
 * Contains the PCM_OP_RATE_POL_MAP_TAX_SUPPLIER operation. 
 *
 * Used to provide tax_supplier information based on:
 *	 product map (tax_supplier defined using Price Tool)
 * or
 *	tax_supplier_map(lookup table)
 *
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 
 
#include "pcm.h"
#include "ops/rate.h"
#include "pin_bill.h"
#include "pin_cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "fm_utils.h"
#include "fm_bill_utils.h"
#include "pin_rate.h"

#define FILE_SOURCE_ID  "fm_rate_pol_map_tax_supplier.c"

PIN_IMPORT  int32	bparam_location_mode_for_taxation;

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_rate_pol_map_tax_supplier(
        cm_nap_connection_t	*connp,
	int32			opcode,
        int32			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp);

static void
fm_rate_pol_map_tax_supplier(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_flist_t             **o_flistpp,
        pin_errbuf_t            *ebufp);

static void
fm_rate_pol_map_tax_supplier_map(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_flist_t             **o_flistpp,
        pin_errbuf_t            *ebufp);

extern void fm_bill_utils_tax_loc_shipto();

/*******************************************************************
 * Main routine for the PCM_OP_RATE_POL_MAP_TAX_SUPPLIER operation.
 *******************************************************************/
void
op_rate_pol_map_tax_supplier(
        cm_nap_connection_t	*connp,
	int32			opcode,
        int32			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_RATE_POL_MAP_TAX_SUPPLIER) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_rate_pol_map_tax_supplier opcode error", ebufp);
		return;
		/*****/
	}

	/*
	 * Debug: What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_rate_pol_map_tax_supplier input flist", i_flistp);

	/*
	 * Do the actual op in a sub.
	 */
	fm_rate_pol_map_tax_supplier(ctxp, i_flistp, o_flistpp, ebufp);

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_rate_pol_map_tax_supplier error", ebufp);
	} else {
		/*
		 * Debug: What we're sending back.
		 */
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		  "op_rate_pol_map_tax_supplier return flist", *o_flistpp);
	}

	return;
}

/*******************************************************************
 * fm_rate_pol_map_tax_supplier():
 *
 * This policy will search for the tax_supplier ID and other tax_supplier
 * related information from:
 *   - products if no tax_supplier_map table is available
 *   - or from tax_supplier_map table otherwise
 *
 * Return flist with PIN_FLD_TAX_SUPPLIER_INFO substruct
 * in the 'match' case or return a NULL flist if no findings...
 *
 *******************************************************************/
static void
fm_rate_pol_map_tax_supplier(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/*****/
	}

	*o_flistpp = NULL;

	if( fm_utils_is_ts_map() ) {
		fm_rate_pol_map_tax_supplier_map(ctxp,
						i_flistp, o_flistpp, ebufp);
	}
	
	return;
}

/*******************************************************************
 * fm_rate_pol_map_tax_supplier_map():
 *
 *    This policy will search through /config/tax_supplier object
 *    and return findings...
 *
 *******************************************************************/
static void
fm_rate_pol_map_tax_supplier_map(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	void		*vp = NULL;
	pin_cookie_t    cookie = NULL;
	pin_flist_t     *flistp = NULL;
	pin_flist_t     *r_flistp = NULL;
	pin_flist_t     *ip_flistp = NULL;
	char		*prod_name = NULL;
	char		*ship_to = NULL;
	char		*company_id = NULL;
	char		*business_location = NULL;
	char		*ship_from = NULL;
	char		*geocode = NULL;
	int32		reg_flag = 1;
	int		tax_pkg = 0;
	char		template[256] = "";
	int32           rec_id = 0;
	int32		geocode_flag = PIN_RATE_LOC_GEOCODE;
	char		*buffer = NULL;
	size_t		buf_len = 0;

	/*
	 * Create tmp ip_flistp to share.
	 */
	ip_flistp = PIN_FLIST_CREATE(ebufp);

	/*
	 * Find tax supplier id and add it to the output flist.
	 * 1. get product name from /product (first one)
	 * 2. get Ship To address from /account
	 */
	
	while ((flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_PRODUCTS,
		 &rec_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_PRODUCT_OBJ, 1, ebufp);
		/*
		 * possible ?
		 */
		if( vp == (pin_flist_t *)NULL) continue;

		PIN_FLIST_FLD_SET(ip_flistp, PIN_FLD_POID, vp, ebufp);
		PIN_FLIST_FLD_SET(ip_flistp, PIN_FLD_NAME, NULL, ebufp);

		/*
       		 * Read the fields from a /product obj.
       		 */
       		PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, ip_flistp, &r_flistp, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			break;
			/****/
		}

		vp = PIN_FLIST_FLD_TAKE(r_flistp, PIN_FLD_NAME, 1, ebufp);
		if( vp ==  NULL) {
			PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
			continue;
			/*******/
		}
		prod_name = (char *)vp;

		/*
		 * Found what we are looking for (just the first one)
		 */
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		r_flistp = NULL;
		break;
		/****/
	}

	if( prod_name == NULL ) {
		goto Done;
		/********/
	}

	/* 
	 * build Ship To address 
	 *	(same as in PCM_OP_RATE_POL_TAX_LOC)
	 */
	
	if (bparam_location_mode_for_taxation == PIN_RATE_LOC_GEOCODE) {
		PIN_FLIST_FLD_SET(i_flistp, PIN_FLD_FLAGS, (void *)&geocode_flag, ebufp);
	}

	r_flistp = PIN_FLIST_CREATE(ebufp);

	fm_bill_utils_tax_loc_shipto(ctxp, i_flistp, r_flistp, ebufp);

	ship_to = (char*) PIN_FLIST_FLD_GET(r_flistp,
			PIN_FLD_SHIP_TO, 1, ebufp);
	if (!ship_to) {
		goto Done;
		/********/
	}

	/*
	 * get company_id, business_location and Ship From from 
	 * tax_supplier_map table
	 */
	if( !fm_utils_get_ts_map(prod_name, ship_to, &company_id,
			&business_location, &ship_from, &reg_flag, &geocode, &tax_pkg) ) {
		goto Done;
		/********/
	}

	/*
	 * Set something to return
	 */
	*o_flistpp = PIN_FLIST_CREATE(ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	if( PIN_ERR_IS_ERR(ebufp)) {
		goto Done;
		/********/
	}
	PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_POID, vp, ebufp);

        fm_utils_init_tax_supplier_objects_flistp = fm_bill_utils_cfg_get_tax_supplier(ctxp, ebufp);
	if (!fm_utils_init_tax_supplier_objects_flistp) {
		goto Done;
		/********/
	}

	/*
	 * Loop through list of cached tax suppliers.
	 * We are searching for a tax_supplier with 
	 * 	PIN_FLD_NAME = company_id
	 * it is always one tax_supplier record per company_id
	 * (in this case)
	 */
	rec_id = 0; cookie = NULL;
	while ((flistp = PIN_FLIST_ELEM_GET_NEXT(
		fm_utils_init_tax_supplier_objects_flistp,
		PIN_FLD_TAX_SUPPLIERS,
		&rec_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL ) {

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_NAME, 0, ebufp);
		if (vp && (strcmp(company_id, (char*)vp) == 0)) {
			/*
			 * found what we are looking for ( just the first one)
			 * Buyers VAT cert. will be defined with the 
			 * exemptions later ...
 			 */
			break;
			/****/
		}
	}

	if (flistp == (pin_flist_t*)NULL) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_VALUE, 0, 0, 0);
		/* we don't need template */
		pin_snprintf(template, sizeof (template), 
			"fm_rate_pol_map_tax_supplier_map: can't "
			"find tax_supplier for company id %s", company_id);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, template, ebufp );

		goto Done;
		/********/
	}

	/* Add geocode to ship_from string if geocode taxation is enabled */	
	if (bparam_location_mode_for_taxation == PIN_RATE_LOC_GEOCODE 
			&& geocode!=NULL && ship_from!=NULL) {
		buf_len = strlen(ship_from) + strlen(geocode) + 8;
		buffer = pin_malloc(buf_len);
		if (buffer == NULL) {
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_SYSTEM_DETERMINATE,
				PIN_ERR_NO_MEM , 0, 0, 0);
			goto Done;
	   		/***********/
	   	}
		pin_snprintf(buffer, buf_len, "%s;[%s,1,1]", ship_from, geocode);
	}

	/* 
	 * Return the TAX_SUPPLIER along with information that 
	 * was found from the tax_supplier_map table
	 */
	PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_TAX_SUPPLIER, &rec_id, ebufp);

	flistp = PIN_FLIST_SUBSTR_ADD(*o_flistpp, PIN_FLD_TAX_SUPPLIER_INFO,
					 ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_NAME,
					(void *)company_id, ebufp);
	if(buffer!=NULL) {
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_SHIP_FROM, (void *)buffer, ebufp);
	} else {
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_SHIP_FROM,
					(void *)ship_from, ebufp);
	}
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_SHIP_TO,
					(void *)ship_to, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_LOCATION,
					(void *)business_location, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_REGULATED_FLAG,
					(void *)&reg_flag, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_GEOCODE,
					(void*)geocode, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_TAXPKG_TYPE, 
					(void*)&tax_pkg, ebufp);
	
Done:
	/*
	 * Error?
	 */
	PIN_FLIST_DESTROY_EX(&ip_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	
	if( prod_name != NULL ) {
		free( prod_name);
	}
	if(buffer != NULL) {
		free(buffer);
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_rate_pol_map_tax_supplier_map error", ebufp);
	}

	return;
}

