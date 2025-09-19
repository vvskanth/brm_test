/***********************************************************************
 *
 *      Copyright (c) 2000, 2024 Oracle and/or its affiliates..
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 ***********************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_cust_pol_init.c:BillingVelocityInt:3:2006-Sep-05 04:30:40 %";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pcm.h"
#include "ops/cust.h"
#include "pinlog.h"
#include "pin_errs.h"
#include "cm_fm.h"
#include "cm_cache.h"
#include "pin_cust.h"
#include "psiu_business_params.h"
#include "fm_utils_bparams_cache.h"

/*******************************************************************
 * Global for holding cache ptr and dd vendor pointer
 *******************************************************************/
PIN_EXPORT cm_cache_t *fm_cust_pol_ctrcur_map_ptr = (cm_cache_t *)0;
PIN_EXPORT char *fm_cust_pol_cm_dd_vendor = (char *)NULL;

PIN_EXPORT int32 *fm_cust_pol_passwd_exp_daysp = (int32 *)NULL;

/* Global variable for holding Billing Segment & DOM map pointer in cache */
PIN_EXPORT cm_cache_t *fm_cust_pol_paymentterm_ptr = (cm_cache_t *)0;
PIN_EXPORT cm_cache_t *fm_cust_pol_customer_segment_ptr = (cm_cache_t *)0;

PIN_IMPORT int pin_conf_dynamic_taxation;
/***********************************************************************
 *Forward declaration
 ***********************************************************************/
EXPORT_OP void fm_cust_pol_init(int32 *errp);

extern void
fm_cust_pol_tax_init_new(
        pcm_context_t           *ctxp,
        pin_flist_t             *in_flistp,
        pin_flist_t             **out_flistpp,
        pin_errbuf_t            *ebufp);



void
fm_cust_pol_init_config_multi_db(
	pcm_context_t	*ctxp,
	int64		database,
	pin_errbuf_t	*ebufp);

void 
fm_cust_pol_init_custom_tax_data(
	pcm_context_t	*ctxp,
	int64		database,
	pin_errbuf_t	*ebufp);

void
fm_cust_pol_init_country_currency_map_cache(
	pcm_context_t	*ctxp,
	int64		database,
	pin_errbuf_t	*ebufp);

void
fm_cust_pol_init_paymentterm_type_cache( 
	pcm_context_t	*ctxp,
	int64		database,
	pin_errbuf_t	*ebufp);

void
fm_cust_pol_init_customer_segment_cache(
	pcm_context_t	*ctxp,
	int64		database,
	pin_errbuf_t	*ebufp);

/*******************************************************************
 * Routines defined elsewere.
 *******************************************************************/

extern pin_flist_t *
fm_cust_pol_util_get_config_country_currency_map_from_db(
	pcm_context_t	*ctxp,
	int64		db,
	pin_errbuf_t	*ebufp);

extern void
fm_cust_pol_util_add_config_country_currency_map_to_cache(
	pin_flist_t	*flistp);

extern void fm_cust_pol_tax_init(
	pcm_context_t	*ctxp,
	pin_flist_t	*in_flistp,
	pin_flist_t	**ret_flistpp,
	pin_errbuf_t	*ebufp);

extern void
fm_cust_pol_init_config_business_type_cache(
	pcm_context_t   *ctxp,
	int64		database,
	pin_errbuf_t	*ebufp);

/***********************************************************************
 *fm_utils_init: One time initialization for fm_cust_pol
 ***********************************************************************/

void
fm_cust_pol_init(int32 *errp)
{
	pcm_context_t	*ctxp = NULL;
	poid_t		*pdp = NULL;
	pin_errbuf_t	ebuf;
	int		err;
	int64		database;
	int32		*flagp = NULL;
	char		conf[20];
	char		cc_checksum_conf[20]={0};
 
	PIN_ERR_CLEAR_ERR(&ebuf);

	/***********************************************************
	 * open the context and get the database number. Don't do this
	 * first, otherwise we don't cleanup.
	 ***********************************************************/
	PCM_CONTEXT_OPEN(&ctxp, (pin_flist_t *)0, &ebuf);
	if(PIN_ERR_IS_ERR(&ebuf)) {
		pin_set_err(&ebuf, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_DM_CONNECT_FAILED, 0, 0, ebuf.pin_err);
		PIN_FLIST_LOG_ERR("fm_cust_pol_init pcm_context_open err", 
				&ebuf);
		*errp = PIN_ERR_DM_CONNECT_FAILED;
		return;
	}

	/***********************************************************
	 * HACK: we need a db number. this will eventually
	 * be handled by the 'get beid object' method from
	 * likely the rating FM. But for now, assume db
	 * matches userid found in pin.conf
	 ***********************************************************/
	pdp = PCM_GET_USERID(ctxp);
	database = PIN_POID_GET_DB(pdp);
	PIN_POID_DESTROY_EX(&pdp, NULL);

	/***********************************************************
	 * Read information about /config/distribution objects.
	 * This is for the multi_db feature.
	**********************************************************/
	fm_cust_pol_init_config_multi_db(ctxp, database, &ebuf);
	if(PIN_ERR_IS_ERR(&ebuf)) {
		PIN_FLIST_LOG_ERR("fm_cust_pol_init_config_multi_db error",
			&ebuf);
	}

	/***********************************************************
	 * Initialize any custom tax data.
	 ***********************************************************/
	fm_cust_pol_init_custom_tax_data(ctxp, database, &ebuf);

	/***********************************************************
	 * Load business type cache
	 ***********************************************************/
	fm_cust_pol_init_config_business_type_cache( ctxp, database, &ebuf );

	/***********************************************************
	 * Load country currency map cache
	 ***********************************************************/
	fm_cust_pol_init_country_currency_map_cache( ctxp, database, &ebuf );

	/***********************************************************
	 * Load payment term  type cache
	 ***********************************************************/
	fm_cust_pol_init_paymentterm_type_cache( ctxp, database, &ebuf );

	/***********************************************************
	 * Load customer segment cache
	 ***********************************************************/
	fm_cust_pol_init_customer_segment_cache( ctxp, database, &ebuf );

	/***********************************************************
	 * Cache the dd vendor type from the cm's pin.conf so that
	 * we don't have to read it during every account creation
	 * (which would be slooooooooow)
	 * We don't particularly care about the error, because this
	 * is an optional parameter.
	 ***********************************************************/
	pin_conf("cm", "dd_vendor", PIN_FLDT_STR, &fm_cust_pol_cm_dd_vendor,
		&err);
			       
	pin_conf("cm", "passwd_age", PIN_FLDT_INT,
		 (caddr_t *)&fm_cust_pol_passwd_exp_daysp, &(err));

	pin_conf("fm_cust_pol", "actg_dom", PIN_FLDT_INT,
                        (caddr_t *)&flagp, &err);
	if(flagp){
		pin_snprintf(conf,sizeof(conf),"%d",*flagp);
		fm_utils_bparam_cache_update_from_pinconf(PSIU_BPARAMS_CUSTOMER_PARAMS,
			PSIU_BPARAMS_CUSTOMER_ACTG_DOM, conf, &ebuf);
		free(flagp);
		flagp = NULL;
	}

	pin_conf("fm_cust_pol", "actg_type", PIN_FLDT_INT,
                        (caddr_t *)&flagp, &err);
	if(flagp){
		pin_snprintf(conf,sizeof(conf),"%d",*flagp);
		fm_utils_bparam_cache_update_from_pinconf(PSIU_BPARAMS_BILLING_PARAMS,
			PSIU_BPARAMS_ACTG_TYPE, conf, &ebuf);
		free(flagp);
		flagp = NULL;
	}

	pin_conf("fm_cust_pol", "bill_when", PIN_FLDT_INT,
                        (caddr_t *)&flagp, &err);
	if(flagp){
		pin_snprintf(conf,sizeof(conf),"%d",*flagp);
		fm_utils_bparam_cache_update_from_pinconf(PSIU_BPARAMS_CUSTOMER_PARAMS,
			PSIU_BPARAMS_CUSTOMER_BILL_WHEN, conf, &ebuf);
		free(flagp);
		flagp = NULL;
	}
	
	/* Reading cc_checksum param value */
	pin_conf(FM_CUST_POL, FM_CC_CHECKSUM_TOKEN, PIN_FLDT_INT,
                        (caddr_t *)&flagp, &err);
	if(flagp){
		pin_snprintf(cc_checksum_conf,sizeof(cc_checksum_conf),"%d",*flagp);
		fm_utils_bparam_cache_update_from_pinconf(PSIU_BPARAMS_CUSTOMER_PARAMS,
				PSIU_BPARAMS_CUSTOMER_CC_CHECKSUM, cc_checksum_conf, &ebuf);
		free(flagp);
		flagp = NULL;
	}

	/***********************************************************
	 * close the context and return.
	 ***********************************************************/
	PCM_CONTEXT_CLOSE(ctxp, 0, &ebuf);
	*errp = ebuf.pin_err;
	return;
        
}


/*******************************************************************
 * fm_cust_pol_init_custom_tax_data
 *      This routine calls policy PCM_OP_CUST_POL_TAX_INIT
 *      to initialize any custom tax data.
 *******************************************************************/
void
fm_cust_pol_init_custom_tax_data(
	pcm_context_t	*ctxp,
	int64		database,
	pin_errbuf_t	*ebufp)
{
	u_int32		flags = 0;
	pin_flist_t	*i_flistp = NULL;
	pin_flist_t	*o_flistp = NULL;
	poid_t		*pdp = NULL;

	/***********************************************************
	 * Create a dummy poid with the database 
	 ***********************************************************/
	pdp = PIN_POID_CREATE(database, "/account", 0, ebufp);

	/***********************************************************
	 * Create the input flist
	 ***********************************************************/
	i_flistp = PIN_FLIST_CREATE(ebufp);
	PIN_FLIST_FLD_PUT(i_flistp, PIN_FLD_POID, pdp, ebufp);

	/***********************************************************
	 * Call the policy
	 ***********************************************************/
#ifdef NO_OPCODES_MAPPED_YET
	PCM_OP(ctxp, PCM_OP_CUST_POL_TAX_INIT,
		flags, i_flistp, &o_flistp, ebufp);
#else
	/***********************************************************
	 * call policy directly
	 ***********************************************************/
        if(pin_conf_dynamic_taxation == 1){
                fm_cust_pol_tax_init(ctxp, i_flistp, &o_flistp, ebufp);
        }
        else {
		fm_cust_pol_tax_init_new(ctxp, i_flistp, &o_flistp, ebufp);
	}
#endif

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_CLEAR_ERR(ebufp);
	}

	PIN_FLIST_DESTROY_EX(&i_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&o_flistp, NULL);

	return;
}

/***********************************************************************
 * fm_cust_pol_init_country_currency_map_cache
 *
 * Initialize the cache of the mapping between country names and the
 * value currencies associated with them
 ***********************************************************************/
void
fm_cust_pol_init_country_currency_map_cache(
	pcm_context_t	*ctxp,
	int64		database,
	pin_errbuf_t	*ebufp)
{
	poid_t		*s_pdp = NULL;
	poid_t		*a_pdp = NULL;
	pin_flist_t	*s_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	pin_flist_t	*res_flistp = NULL;
	pin_flist_t	*arg_flistp = NULL;
	pin_flist_t	*c_flistp = NULL;

	pin_cookie_t	cookie = NULL;
	int32		err = PIN_ERR_NONE;
	int32		cnt = 0;
	int32		recid;
	int32		elemid = 0;
	int32		s_flags = SRCH_DISTINCT;

	s_flistp = PIN_FLIST_CREATE(ebufp);

	s_pdp = PIN_POID_CREATE(database, "/search", -1, ebufp);
	PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, s_pdp, ebufp);
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_FLAGS, (void *)&s_flags, ebufp);
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_TEMPLATE, 
			"select X from /config where F1 = V1 ",
			ebufp);


	/* setup arguments */
	arg_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);

	a_pdp = PIN_POID_CREATE(database,
				PIN_OBJ_TYPE_CONFIG_COUNTRY_CURRENCY_MAP, -1,
				ebufp);
	PIN_FLIST_FLD_PUT(arg_flistp, PIN_FLD_POID, a_pdp, ebufp);
	PIN_FLIST_ELEM_SET( s_flistp, NULL, PIN_FLD_RESULTS, 0, ebufp);

	PCM_OP(ctxp, PCM_OP_SEARCH, 0, s_flistp, &r_flistp, ebufp);
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_cust_pol_util_get_config_country_currency_map_from_db: "
			"error loading /config/business_type object",
			ebufp);

		goto cleanup;
		/***********/
	}

	res_flistp = PIN_FLIST_ELEM_TAKE(r_flistp, PIN_FLD_RESULTS, 
					PIN_ELEMID_ANY, 1, ebufp);


	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "Read country_currency_map",
			  res_flistp);

	/* Bail if there's an error (such as no such object!) */
	if (PIN_ERR_IS_ERR(ebufp) || !res_flistp) {
		PIN_ERR_CLEAR_ERR(ebufp);
		goto cleanup;
		/***********/
	}

	/*
	 * Get the number of items in the flist so we can create an
	 * appropriately sized cache
	 */
	cnt = PIN_FLIST_COUNT(res_flistp, ebufp);

	/*
	 * If there's no data, there's no point initializing it, is there.
	 * Especially since this is optional!
	 */
	if (cnt == 0) {
		goto cleanup;
	}
	
	/*
	 * This cache doesn't need configuration since the sizes of the
	 * entries are computed
	 */
	fm_cust_pol_ctrcur_map_ptr =
		cm_cache_init("fm_cust_pol_country_currency_map", cnt,
			      cnt * 1024, 8, CM_CACHE_KEY_IDDBSTR, &err);

	if (err != PIN_ERR_NONE) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
				"Error: Couldn't initialize "
				"country/currency cache in "
				"fm_cust_pol_init_country_currency_map_cache");
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			    PIN_ERRCLASS_SYSTEM_DETERMINATE,
			    PIN_ERR_NO_MEM, 0, 0, err);
		goto cleanup;
	}

	if (!fm_cust_pol_ctrcur_map_ptr) {
		goto cleanup;
	}

	while ((c_flistp = PIN_FLIST_ELEM_GET_NEXT(res_flistp,
						  PIN_FLD_COUNTRIES,
						  &recid, 1, &cookie, ebufp)) != NULL) {
		cm_cache_key_iddbstr_t  kids;
		
		kids.id = 0;    /* Not relevant for us */
		kids.db = 0;    /* Not relevant for us */
		kids.str = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_CANON_COUNTRY,
					     0, ebufp);

		if (kids.str) {
			cm_cache_add_entry(fm_cust_pol_ctrcur_map_ptr,
					   &kids, c_flistp, &err);
			switch(err) {
			case PIN_ERR_NONE:
				break;
			case PIN_ERR_OP_ALREADY_DONE:
				pinlog(__FILE__, __LINE__,
				       LOG_FLAG_WARNING,
				"fm_cust_pol_init_country_currency_map_cache: "
				       "cache already done: <%s>", kids.str);
				break;
			default:
				pinlog(__FILE__, __LINE__,
				       LOG_FLAG_ERROR,
		  "fm_cust_pol_util_add_config_country_currency_map_to_cache: "
				       "error adding cache <%s>", kids.str);
				break;
			}
		}
	}
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			   "fm_cust_pol_init_country_currency_map_cache error",
				 ebufp);
	}

 cleanup:
	PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&res_flistp, NULL);
}
					   
/***********************************************************************
 * fm_cust_pol_init_paymentterm_type_cache
 *
 * Initialize the cache of the payment terms
 ***********************************************************************/
void
fm_cust_pol_init_paymentterm_type_cache( pcm_context_t *ctxp,
                                         int64 database,
                                         pin_errbuf_t *ebufp)
{
	poid_t                  *s_pdp = NULL;
	poid_t                  *arg_pdp = NULL;
	pin_flist_t             *s_flistp = NULL;
	pin_flist_t             *res_flistp = NULL;
	pin_flist_t             *arg_flistp = NULL;
	pin_flist_t             *pt_flistp = NULL;
        int32                   err = PIN_ERR_NONE;
	int32                   s_flags = 256;
        int32                   cnt = 0;
        int32                   recid = 0;
        int32                   elemid = 0;
        pin_cookie_t            cookie = NULL;
        char                    *paymentterm_key;

        if (PIN_ERR_IS_ERR(ebufp))
                return;

        PIN_ERR_CLEAR_ERR(ebufp);

	/* Create Search flist */
	s_flistp = PIN_FLIST_CREATE(ebufp);

        s_pdp = PIN_POID_CREATE(database, "/search", -1, ebufp);
        PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, s_pdp, ebufp);
        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_FLAGS, (void *)&s_flags, ebufp);
        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_TEMPLATE, 
			"select X from /config where F1 = V1 ",
                       	ebufp);

        /* setup arguments */
        arg_pdp = PIN_POID_CREATE(database, PIN_OBJ_TYPE_CONFIG_PAYMENTTERM, 
				  -1, ebufp);
	arg_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);
        PIN_FLIST_FLD_PUT(arg_flistp, PIN_FLD_POID, arg_pdp, ebufp);
        PIN_FLIST_ELEM_SET( s_flistp, NULL, PIN_FLD_RESULTS, 0, ebufp);

	/***********************************************************
         * Do the database search.
         ***********************************************************/
	PCM_OP(ctxp, PCM_OP_SEARCH, 0, s_flistp, &res_flistp, ebufp);

	/***********************************************************
         * If Payment_term not configured, bail out from here.
         ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_cust_pol_init_paymentterm_type_cache() "
			"error loading /config/payment_term object",
                        ebufp);
		PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&arg_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&res_flistp, NULL);
        }

	if (PIN_FLIST_ELEM_COUNT(res_flistp, PIN_FLD_RESULTS, ebufp) > 1){
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_cust_pol_init_paymentterm_type_cache() "
			"/config/payment_term object should NOT be > 1",
                        ebufp);	        
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			    PIN_ERRCLASS_SYSTEM_DETERMINATE,
			    PIN_ERR_BAD_VALUE, 0, 0, err);
	       	goto cleanup;		
	}
	pt_flistp = PIN_FLIST_ELEM_GET(res_flistp,
                                      PIN_FLD_RESULTS, 
				       PIN_ELEMID_ANY, 1, ebufp);
        if ( pt_flistp ) {
	        cnt = PIN_FLIST_COUNT(pt_flistp, ebufp);
	}

	if (cnt == 0) {
	        goto cleanup;
	}

	fm_cust_pol_paymentterm_ptr =
                cm_cache_init("fm_cust_pol_paymentterm_cache", 1,
                              cnt*1024, 1, CM_CACHE_KEY_STR, &err);
	if (err != PIN_ERR_NONE) {
                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                                "Error: Couldn't allocate memory"
				"for paymentterm cache in "
                                "fm_cust_pol_paymentterm cache");
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			    PIN_ERRCLASS_SYSTEM_DETERMINATE,
			    PIN_ERR_NO_MEM, 0, 0, err);
	       	goto cleanup;
        }

        paymentterm_key =  PIN_OBJ_TYPE_CONFIG_PAYMENTTERM;
      
        cm_cache_add_entry(fm_cust_pol_paymentterm_ptr,
                                           paymentterm_key, pt_flistp, &err);
        switch(err) {
                 case PIN_ERR_NONE:
		         break;
                 case PIN_ERR_OP_ALREADY_DONE:
                         pinlog(__FILE__, __LINE__,
				LOG_FLAG_WARNING,
				"fm_cust_pol_paymentterm cache(): "
                                "cache already done: <%s>", paymentterm_key);
                                break;
	         default:
                        pinlog(__FILE__, __LINE__,
                               LOG_FLAG_ERROR,
                               "fm_cust_pol_paymentterm cache() "
                               "error adding cache <%s>", paymentterm_key);
                                break;
	}
         
        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                           "fm_cust_pol_paymentterm cache() error",
                                 ebufp);
        }

 cleanup:
        PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&res_flistp, NULL);
}



/***********************************************************************
 * fm_cust_pol_init_customer_segment_cache
 *
 * Initialize the cache of the customer segments
 ***********************************************************************/
void
fm_cust_pol_init_customer_segment_cache( pcm_context_t *ctxp,
                                         int64 database,
                                         pin_errbuf_t *ebufp)
{
	poid_t			*s_pdp = NULL;
	poid_t			*arg_pdp = NULL;
	pin_flist_t		*s_flistp = NULL;
	pin_flist_t		*res_flistp = NULL;
	pin_flist_t		*arg_flistp = NULL;
	pin_flist_t		*cs_flistp = NULL;
	int32			err = PIN_ERR_NONE;
	int32			s_flags = 256;
	int32			cnt = 0;
	int32			recid = 0;
	int32			elemid = 0;
	pin_cookie_t		cookie = NULL;
	char			*customer_segment_key;
	char			tmpstr[200];

	if (PIN_ERR_IS_ERR(ebufp))
		return;

	PIN_ERR_CLEAR_ERR(ebufp);

	/* Create Search flist */
	s_flistp = PIN_FLIST_CREATE(ebufp);

	s_pdp = PIN_POID_CREATE(database, "/search", -1, ebufp);
	PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, s_pdp, ebufp);
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_FLAGS, (void *)&s_flags, ebufp);
        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_TEMPLATE, 
			"select X from /config where F1 = V1 ",
			ebufp);

        /* setup arguments */
        arg_pdp = PIN_POID_CREATE(database, 
				PIN_OBJ_TYPE_CONFIG_CUSTOMER_SEGMENT, 
				-1, ebufp);
	arg_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);
	PIN_FLIST_FLD_PUT(arg_flistp, PIN_FLD_POID, arg_pdp, ebufp);
	PIN_FLIST_ELEM_SET( s_flistp, NULL, PIN_FLD_RESULTS, 0, ebufp);

	/***********************************************************
	 * Do the database search.
	 ***********************************************************/
	PCM_OP(ctxp, PCM_OP_SEARCH, 0, s_flistp, &res_flistp, ebufp);

	/***********************************************************
	 * If error in search bail out from here.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		pin_snprintf( tmpstr,sizeof(tmpstr) ,"fm_cust_pol_init_customer_segment_cache() "
				"error loading %s", 
				PIN_OBJ_TYPE_CONFIG_CUSTOMER_SEGMENT );
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, tmpstr, ebufp);
		goto cleanup;
        }

	if (PIN_FLIST_ELEM_COUNT(res_flistp, PIN_FLD_RESULTS, ebufp) > 1){
		pin_snprintf( tmpstr,sizeof(tmpstr) ,"fm_cust_pol_init_customer_segment_cache() "
				"number of %s objects should NOT be > 1", 
				PIN_OBJ_TYPE_CONFIG_CUSTOMER_SEGMENT );
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, tmpstr, ebufp);	        
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			    PIN_ERRCLASS_SYSTEM_DETERMINATE,
			    PIN_ERR_BAD_VALUE, 0, 0, err);
	       	goto cleanup;		
	}
	cs_flistp = PIN_FLIST_ELEM_GET(res_flistp,
                                      PIN_FLD_RESULTS, 
				       PIN_ELEMID_ANY, 1, ebufp);
        if ( cs_flistp ) {
	        cnt = PIN_FLIST_COUNT(cs_flistp, ebufp);
	}

	if (cnt == 0) {
	        goto cleanup;
	}

	fm_cust_pol_customer_segment_ptr =
                cm_cache_init("fm_cust_pol_customer_segment_cache", 1,
                              cnt*1024, 1, CM_CACHE_KEY_STR, &err);

	if (err != PIN_ERR_NONE) {
                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                                "Error: Couldn't allocate memory"
				"for customer_segment cache in "
                                "fm_cust_pol_init_customer_segment_cache()");
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			    PIN_ERRCLASS_SYSTEM_DETERMINATE,
			    PIN_ERR_NO_MEM, 0, 0, err);
	       	goto cleanup;
        }

        customer_segment_key =  PIN_OBJ_TYPE_CONFIG_CUSTOMER_SEGMENT;
      
        cm_cache_add_entry(fm_cust_pol_customer_segment_ptr,
                                           customer_segment_key, cs_flistp, &err);
        switch(err) {
                 case PIN_ERR_NONE:
		         break;
                 case PIN_ERR_OP_ALREADY_DONE:
                         pinlog(__FILE__, __LINE__,
				LOG_FLAG_WARNING,
				"fm_cust_pol_init_customer_segment_cache(): "
                                "cache already done: <%s>", customer_segment_key);
                                break;
	         default:
                        pinlog(__FILE__, __LINE__,
                               LOG_FLAG_ERROR,
                               "fm_cust_pol_init_customer_segment_cache() "
                               "error adding cache <%s>", customer_segment_key);
                                break;
	}
         
        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                           "fm_cust_pol_init_customer_segment_cache() error",
                                 ebufp);
        }

 cleanup:
        PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&res_flistp, NULL);
}
       
