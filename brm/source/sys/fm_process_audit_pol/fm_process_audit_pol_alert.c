/*
 * Copyright (c) 2004, 2024, Oracle and/or its affiliates.
 *
 *     This material is the confidential property of Oracle Corporation
 *     or licensors and may be used, reproduced, stored or transmitted
 *     only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_process_audit_pol_alert.c /cgbubrm_7.5.0.portalbase/1 2023/06/21 12:05:32 sreejs Exp $";
#endif

/*******************************************************************
 * Contains the PCM_OP_PROCESS_AUDIT_POL_ALERT operation.
 *******************************************************************/

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>

#define FILE_LOGNAME "fm_process_audit_pol_alert.c(2)"

#include "ops/process_audit.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_channel.h"
#include "fm_utils.h"


#define PIN_OBJ_NAME_DOMAIN "Email Alert for RA Notification"
#define PIN_ALERT_CONFIG_NAME "pin_config_ra_alerts"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/

EXPORT_OP void
op_process_audit_pol_alert(
    cm_nap_connection_t    *connp,
    int32            opcode,
    int32            flags,
    pin_flist_t      *i_flistp,
    pin_flist_t      **r_flistpp,
    pin_errbuf_t     *ebufp);

static void
fm_process_audit_pol_alert(
    pcm_context_t    *ctxp,
    int32            flags,
    pin_flist_t      *i_flistp,
    pin_flist_t      **r_flistpp,
    pin_errbuf_t     *ebufp);

static void
fm_process_audit_pol_read_ra_alerts(
    pcm_context_t    *ctxp,
    pin_flist_t      *in_flistp,
    pin_flist_t      **o_flistpp,
    pin_errbuf_t     *ebufp);

static void
fm_process_audit_pol_read_ra_thresholds(
    pcm_context_t    *ctxp,
    pin_flist_t      *in_flistp,
    pin_flist_t      **o_flistpp,
    pin_errbuf_t     *ebufp);

static void
fm_process_audit_pol_read_locale(
    pcm_context_t    *ctxp,
    pin_flist_t      *in_flistp,
    pin_flist_t      **o_flistpp,
    pin_errbuf_t     *ebufp);

static void
fm_process_audit_pol_format_email(
    pcm_context_t    *ctxp,
    pin_flist_t      *in_flistp,
    pin_flist_t      *threshold_flistp,
    pin_flist_t      *locale_flistp,
    pin_errbuf_t     *ebufp);

static void
fm_process_audit_pol_send_email(
    pcm_context_t    *ctxp,
    pin_flist_t      *alert_flistp,
    pin_flist_t      *loc_flistp,
    pin_flist_t      **o_flistpp,
    pin_errbuf_t     *ebufp);

static char*
fm_process_audit_pol_replace_param(
    const char   *source_str,
    const char   *param_str,
    const char   *replace_str);

static poid_t *
fm_process_audit_pol_get_brand(
    pcm_context_t    *ctxp,
    pin_errbuf_t     *ebufp);


/*******************************************************************
 * Main routine for the PCM_OP_PROCESS_AUDIT_POL_ALERT operation.
 *******************************************************************/
void
op_process_audit_pol_alert(
    cm_nap_connection_t    *connp,
    int32            opcode,
    int32            flags,
    pin_flist_t      *i_flistp,
    pin_flist_t      **r_flistpp,
    pin_errbuf_t     *ebufp)
{
    pcm_context_t        *ctxp = connp->dm_ctx;

    if (PIN_ERR_IS_ERR(ebufp))
        return;
    PIN_ERR_CLEAR_ERR(ebufp);

    /***********************************************************
     * Null out results
     ***********************************************************/
    *r_flistpp = NULL;

    /***********************************************************
     * Insanity check.
     ***********************************************************/
    if (opcode != PCM_OP_PROCESS_AUDIT_POL_ALERT) {
        pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_SYSTEM_DETERMINATE,
            PIN_ERR_BAD_OPCODE, 0, 0, opcode);
        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
            "op_process_audit_pol_alert opcode error", ebufp);

        return;
    }
    
    /***********************************************************
     * Debug what we got.
    ***********************************************************/
    PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        "op_process_audit_pol_alert input flist", i_flistp);


    /***********************************************************
     * Main rountine for this opcode
     ***********************************************************/
    fm_process_audit_pol_alert(ctxp, flags, i_flistp, r_flistpp, ebufp);
    

    /***********************************************************
     * Error?
     ***********************************************************/
    if (PIN_ERR_IS_ERR(ebufp)) {
        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
            "op_process_audit_pol_alert error", ebufp);

        /* 
        *  Don't pass back error results to
        *  RA notification framework 
        */
        PIN_ERR_CLEAR_ERR(ebufp);

        /* 
           Return input flist as o/p to avoid null error in calling routine 
           This is done as Notification framework will trigger this opcode
           and it should not get error and rollback if 
           there is error in sending email
        */
        PIN_FLIST_DESTROY_EX(r_flistpp, NULL);
        *r_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
    } else {
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
            "op_process_audit_pol_alert output flist", *r_flistpp);
    }

    return;
}

/*******************************************************************
 * fm_process_audit_pol_alert:
 *******************************************************************/
static void
fm_process_audit_pol_alert(
    pcm_context_t       *ctxp,
    int32               flags,
    pin_flist_t         *i_flistp,
    pin_flist_t         **r_flistpp,
    pin_errbuf_t        *ebufp)
{

    int32        result_count = 0;
    void         *vp = NULL;
    const char   *alert_namep = NULL;
    
    poid_t       *old_poid = NULL;
    pin_flist_t  *alert_flistp = NULL;
    pin_flist_t  *threshold_flistp = NULL;
    pin_flist_t  *locale_flistp = NULL;
    pin_cookie_t cookie = NULL;
    char  object_type[256] = "";
    char  control_point[256] = "";
    char  batch_id[256] = "";
    int32   elem_id = 0;
    
    if (PIN_ERR_IS_ERR(ebufp)) {
        return;
    }
    PIN_ERR_CLEAR_ERR(ebufp);

    /* 
    *  Check for mandatory fields on i/p flist
    *  Array PIN_FLD_THRESHOLDS should have PIN_FLD_NAME
    */
    vp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_THRESHOLDS,
            &elem_id, 0, &cookie, ebufp);

    vp = PIN_FLIST_FLD_GET((pin_flist_t*)vp, PIN_FLD_NAME, 0, ebufp);

    if (PIN_ERR_IS_ERR(ebufp)) {
        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
            "fm_process_audit_pol_alert: threshold name not found on input flist", ebufp);
        return;
    }

    /******
    * read /config/ra_alerts object 
    ******/
    fm_process_audit_pol_read_ra_alerts(ctxp, i_flistp, &alert_flistp, ebufp);

    PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        "fm_process_audit_pol_read_ra_alerts return flist", alert_flistp);


    /******
    * read subject and message /strings for the locale
    ******/
    fm_process_audit_pol_read_locale(ctxp, alert_flistp, &locale_flistp, ebufp);
    
    PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        "fm_process_audit_pol_read_locale return flist", locale_flistp);


    /******
    * read /config/ra_thresholds object
    ******/
    fm_process_audit_pol_read_ra_thresholds(ctxp, i_flistp, &threshold_flistp, ebufp);

    PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        "fm_process_audit_pol_read_ra_thresholds return flist", threshold_flistp);

    /******
    * format email subject and message
    ******/
    fm_process_audit_pol_format_email(ctxp, i_flistp, threshold_flistp, 
        locale_flistp, ebufp);


    /******
    * send the email
    ******/
    fm_process_audit_pol_send_email(ctxp, alert_flistp, locale_flistp, r_flistpp, ebufp);

    if (PIN_ERR_IS_ERR(ebufp)) {
        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
            "fm_process_audit_pol_alert error", ebufp);
    }

    PIN_FLIST_DESTROY_EX(&alert_flistp, NULL);
    PIN_FLIST_DESTROY_EX(&locale_flistp, NULL);
    PIN_FLIST_DESTROY_EX(&threshold_flistp, NULL);

    return;
}

    
/********************************************************************
* Routine to read details from /config/ra_alerts for this alert
********************************************************************/
static void
fm_process_audit_pol_read_ra_alerts(
    pcm_context_t      *ctxp,
    pin_flist_t        *in_flistp,
    pin_flist_t        **o_flistpp,
    pin_errbuf_t       *ebufp)
{
    pin_flist_t    *search_flistp = NULL;
    pin_flist_t    *result_flistp = NULL;
    pin_flist_t    *search_args_flistp = NULL;
    pin_flist_t    *sub_flistp = NULL;
    poid_t         *search_poid = NULL;
    poid_t         *brand_poidp = NULL;

    void    *vp = NULL;
    void    *vp1 = NULL;
    void    *vp2 = NULL;
    char    *alert_namep = NULL;

    int     search_flags = SRCH_EXACT + SRCH_DISTINCT;
    char    *search_template = " select X from /config/ra_alerts where F1 = V1 ";

    int32 elem_id = 0;
    pin_cookie_t cookie = NULL;

    if (PIN_ERR_IS_ERR(ebufp))
        return;
    PIN_ERR_CLEAR_ERR(ebufp);

    *o_flistpp = (pin_flist_t *)NULL;

    /* Get the alert name from input flist */
    vp = PIN_FLIST_ELEM_GET_NEXT(in_flistp, PIN_FLD_THRESHOLDS,
                    &elem_id, 0, &cookie, ebufp);

    alert_namep = (char*) PIN_FLIST_FLD_GET((pin_flist_t*)vp, PIN_FLD_NAME, 0, ebufp);

    search_flistp = PIN_FLIST_CREATE(ebufp);

    brand_poidp = fm_process_audit_pol_get_brand(ctxp, ebufp);

    search_poid = PIN_POID_CREATE(PIN_POID_GET_DB(brand_poidp), "/search", -1, ebufp);
    PIN_FLIST_FLD_PUT(search_flistp, PIN_FLD_POID, search_poid, ebufp);

    PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_TEMPLATE, (void *)search_template, ebufp);

    PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_FLAGS, (void *)&search_flags, ebufp);

    /* arguments for where clause */
    search_args_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS, 1, ebufp);

    PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_NAME, (void *)PIN_ALERT_CONFIG_NAME, ebufp);

    /* result fields */
    search_args_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_RESULTS, PIN_ELEMID_ANY, ebufp);
    search_args_flistp = PIN_FLIST_ELEM_ADD(search_args_flistp, PIN_FLD_THRESHOLDS, 
                                    PIN_ELEMID_ANY, ebufp);

    PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_NAME, (void *)NULL, ebufp);
    PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_LOCALE, (void *)NULL, ebufp);
    PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_SENDER, (void *)NULL, ebufp);

    search_args_flistp = PIN_FLIST_ELEM_ADD(search_args_flistp, 
                                    PIN_FLD_RECIPIENTS, PIN_ELEMID_ANY, ebufp);

    PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_EMAIL_ADDR, (void *)NULL, ebufp);
    PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_TYPE, (void *)NULL, ebufp);

    PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
        "fm_process_audit_pol_read_ra_alerts: op_search input flist", search_flistp);

    PCM_OP(ctxp, PCM_OP_SEARCH, 0, search_flistp, &result_flistp, ebufp);

    PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
        "fm_process_audit_pol_read_ra_alerts: op_search return flist", result_flistp);

    /* Destroy the poid used for brand account */
    PIN_POID_DESTROY_EX(&brand_poidp, ebufp);

    /* 
    *  Loop through result and see if threshold name matches to one on i/p flist
    *  Multiple PIN_FLD_THRESHOLDS array elements are expected in result.
    */ 
    elem_id = 0;
    cookie = NULL;

    if ((sub_flistp = PIN_FLIST_ELEM_GET_NEXT(result_flistp, PIN_FLD_RESULTS,
        &elem_id, 0, &cookie, ebufp)) != (pin_flist_t*) NULL)
    {
        elem_id = 0;
        cookie = NULL;

        while( (vp1 = PIN_FLIST_ELEM_GET_NEXT(sub_flistp, PIN_FLD_THRESHOLDS, 
            &elem_id, 1, &cookie, ebufp)) != (pin_flist_t*)NULL)
        {
           /* Check if this element has the same alert name in PIN_FLD_THRESHOLDS */
            vp2 = PIN_FLIST_FLD_GET((pin_flist_t*)vp1, PIN_FLD_NAME, 0, ebufp);
            if ( vp2 && (strcmp(alert_namep, vp2) != 0) ) {
                continue;
            }

            /* Check if locale is present in PIN_FLD_THRESHOLDS array */
            vp2 = PIN_FLIST_FLD_GET((pin_flist_t*)vp1, PIN_FLD_LOCALE, 0, ebufp);
            
            /* log error if locale is not configured */
            if(PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                    "fm_process_audit_pol_read_ra_alerts: Locale not configured in /config/ra_alerts", ebufp);
            }

            /* return PIN_FLD_THRESHOLDS array */
            *o_flistpp = PIN_FLIST_ELEM_TAKE(sub_flistp, PIN_FLD_THRESHOLDS,
                            elem_id, 0, ebufp);
            break;
        } /* end loop for PIN_FLD_THRESHOLDS*/
    }
    else {
        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
            "fm_process_audit_pol_read_ra_alerts: /config/ra_alerts object not found");
    }

    PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
    PIN_FLIST_DESTROY_EX(&result_flistp, NULL);

    if(PIN_ERR_IS_ERR(ebufp)) {
        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
            "fm_process_audit_pol_read_ra_alerts error", ebufp);

        PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
    }

    /* If threshold not configured give warning */
    if ( (!PIN_ERR_IS_ERR(ebufp)) && *o_flistpp == NULL) {
        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_WARNING,
            "fm_process_audit_pol_read_ra_alerts: threshold not configured in /config/ra_alerts");
    }
    return;
}


/*************************************************
* Routine to read /strings objects for given locale
*************************************************/
static void
fm_process_audit_pol_read_locale(
    pcm_context_t      *ctxp,
    pin_flist_t        *i_flistp,
    pin_flist_t        **o_flistpp,
    pin_errbuf_t       *ebufp)
{
    string_list_t   *str_list = NULL;
    string_info_t   str_info;

    short   sub = 0, msg = 0;
    char    *locale = NULL;

    if (PIN_ERR_IS_ERR(ebufp))
        return;
    PIN_ERR_CLEAR_ERR(ebufp);

    *o_flistpp = (pin_flist_t *)NULL;

    locale = (char*) PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_LOCALE, 1, ebufp);

    str_list = pin_get_localized_string_list(ctxp, locale, 
                            PIN_OBJ_NAME_DOMAIN, -1, -1, ebufp);

    if (str_list == NULL) {
        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
            "fm_process_audit_pol_read_locale: Locale /strings objects not found", ebufp);
        return;
    }

    *o_flistpp = PIN_FLIST_CREATE(ebufp);

    while ( pin_string_list_get_next(str_list, &str_info, ebufp) 
        != (string_info_t*)NULL)
    {
        switch (str_info.string_id)
        {
        /*subject*/
        case 1 :
            sub = 1;
            PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_SUBJECT, 
                        (void*)&str_info.loc_string_utf8, ebufp);
            break;
        /*message*/
        case 2 :
            msg = 1;
            PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_STRING, 
                        (void*)&str_info.loc_string_utf8, ebufp);
            break;
        }
    }

    if (!sub || !msg) {
        PIN_FLIST_DESTROY_EX(o_flistpp, NULL);

        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
            "fm_process_audit_pol_read_locale: subject and/or message string not found");

        pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_SYSTEM_DETERMINATE,
            PIN_ERR_NOT_FOUND, 0, 0, 0);
    }
    
    /* clean up */
    pin_string_list_destroy(str_list, ebufp);

    return;
}


/*************************************************
* Routine to read /config/ra_threshold object
*************************************************/
static void
fm_process_audit_pol_read_ra_thresholds(
    pcm_context_t      *ctxp,
    pin_flist_t        *in_flistp,
    pin_flist_t        **o_flistpp,
    pin_errbuf_t       *ebufp)
{
    pin_flist_t    *search_flistp = NULL;
    pin_flist_t    *result_flistp = NULL;
    pin_flist_t    *search_args_flistp = NULL;
    pin_flist_t    *sub_flistp = NULL;

    int            search_flags = SRCH_EXACT + SRCH_DISTINCT;
    poid_t         *search_poid = NULL;
    poid_t         *brand_poidp = NULL;

    char    *alert_namep;
    void    *vp = NULL;
    int32   count = 0;
    int32   elem_id = 0;

    pin_cookie_t cookie = NULL;

    char    *search_template = " select X from /config/ra_thresholds where F1 = V1 ";

    if (PIN_ERR_IS_ERR(ebufp))
        return;
    PIN_ERR_CLEAR_ERR(ebufp);

    *o_flistpp = (pin_flist_t *)NULL;

    /* Get the alert name from input flist */
    vp = PIN_FLIST_ELEM_GET_NEXT(in_flistp, PIN_FLD_THRESHOLDS,
                    &elem_id, 0, &cookie, ebufp);

    alert_namep = (char*) PIN_FLIST_FLD_GET((pin_flist_t*)vp, PIN_FLD_NAME, 0, ebufp);

    search_flistp = PIN_FLIST_CREATE(ebufp);

    brand_poidp = fm_process_audit_pol_get_brand(ctxp, ebufp);

    search_poid = PIN_POID_CREATE(PIN_POID_GET_DB(brand_poidp), "/search", -1, ebufp);
    PIN_FLIST_FLD_PUT(search_flistp, PIN_FLD_POID, search_poid, ebufp);

    /* Destroy brand poid */
    PIN_POID_DESTROY_EX(&brand_poidp, ebufp);

    PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_TEMPLATE, (void *)search_template, ebufp);

    PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_FLAGS, (void *)&search_flags, ebufp);

    /* arguments for where clause */
    search_args_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS, 1, ebufp);
    search_args_flistp = PIN_FLIST_ELEM_ADD(search_args_flistp, 
                                    PIN_FLD_THRESHOLDS, 0, ebufp);

    PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_NAME, (void *)alert_namep, ebufp);

    /* result fields */
    search_args_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_RESULTS,
                                PIN_ELEMID_ANY, ebufp);

    search_args_flistp = PIN_FLIST_ELEM_ADD(search_args_flistp, PIN_FLD_THRESHOLDS, 
                                    PIN_ELEMID_ANY, ebufp);

    PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_NAME, (void *)NULL, ebufp);
    PIN_FLIST_FLD_SET(search_args_flistp, PIN_FLD_OBJECT_TYPE, (void *)NULL, ebufp);

    sub_flistp = PIN_FLIST_ELEM_ADD(search_args_flistp, PIN_FLD_FILTER_DETAILS, 
                                    PIN_ELEMID_ANY, ebufp);

    PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_FIELD_NAME, (void *)NULL, ebufp);
    PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_INT_VAL, (void *)NULL, ebufp);
    PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_STR_VAL, (void *)NULL, ebufp);

    sub_flistp = PIN_FLIST_ELEM_ADD(search_args_flistp, PIN_FLD_VALUE_RANGES, 
                                    PIN_ELEMID_ANY, ebufp);

    PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_FIELD_NAME, (void *)NULL, ebufp);
    PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_MAXIMUM, (void *)NULL, ebufp);
    PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_MINIMUM, (void *)NULL, ebufp);

    PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
        "fm_process_audit_pol_read_ra_thresholds: op_search input flist", search_flistp);

    PCM_OP(ctxp, PCM_OP_SEARCH, 0, search_flistp, &result_flistp, ebufp);

    PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
        "fm_process_audit_pol_read_ra_thresholds: op_search return flist", result_flistp);

    /* reset */
    cookie = NULL;
    elem_id = 0;

    /* Only one elem in PIN_FLD_RESULTS is expected */
    sub_flistp = PIN_FLIST_ELEM_GET_NEXT(result_flistp, PIN_FLD_RESULTS, 
                            &elem_id, 0, &cookie, ebufp);

    /* reset */
    elem_id = 0;
    cookie = NULL;

    *o_flistpp = PIN_FLIST_ELEM_TAKE_NEXT(sub_flistp, PIN_FLD_THRESHOLDS,
                            &elem_id, 0, &cookie, ebufp);
    
    PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
    PIN_FLIST_DESTROY_EX(&result_flistp, NULL);

    if(PIN_ERR_IS_ERR(ebufp)) {
        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
            "fm_process_audit_pol_read_ra_thresholds error", ebufp);

        PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
    }

    return;

}



/*************************************************
* Routine to format subject and message text for email.
* Parameters will be replaced with dynamic values
*************************************************/
static void
fm_process_audit_pol_format_email(
    pcm_context_t      *ctxp,
    pin_flist_t        *in_flistp,
    pin_flist_t        *threshold_flistp,
    pin_flist_t        *locale_flistp,
    pin_errbuf_t       *ebufp)
{
    void    *vp1 = NULL;
    void    *vp2 = NULL;

    char    start_t[80]={0};
    char    end_t[80]={0};
    char    obj_type[80]={0};

    char    filter_str[1024]={0};
    char    threshold_str[1024]={0};

    char    *alert_namep=NULL;
    char    *fld_p = NULL;
    int32   elem_id = 0;
    int32   i = 0;

    pin_cookie_t cookie = NULL;

    pin_fld_num_t fld_code;

    char    tmp_str[80]={0};
    short   isFirst = 1;
    char    *fld_name = NULL;

    char    *sub = NULL;
    char    *msg = NULL;
    char    *t1, *t2;

    /* For formating Filter and Threshold conditions */
    char    *fltr_string = NULL;
    size_t  fltr_string_len = 0;
    char    *str_open_brace = "(";
    char    *str_close_brace = ") ";
    char    *str_equal = " = ";
    char    *str_greater = " >= ";
    char    *str_less = " <= ";
    char    *str_and = " && ";
    char    *str_or = " || ";

    pin_flist_t *tmp_flistp = NULL;
    pin_flist_t *string_flistp = NULL;
    int32   string_index = 0;
    size_t  string_length = 0;
    int     type;

    if (PIN_ERR_IS_ERR(ebufp))
        return;
    PIN_ERR_CLEAR_ERR(ebufp);

    tmp_str[0] = '\0';

    /* get alert name from i/p flist*/
    vp1 = PIN_FLIST_ELEM_GET_NEXT(in_flistp, PIN_FLD_THRESHOLDS,
            &elem_id, 0, &cookie, ebufp);

    alert_namep = (char*) PIN_FLIST_FLD_GET((pin_flist_t*)vp1, PIN_FLD_NAME, 0, ebufp);

    /* get start and end timestamp */
    start_t[0] = '\0';
    end_t[0] = '\0';

    vp2 = PIN_FLIST_FLD_GET((pin_flist_t*)vp1, PIN_FLD_START_T, 0, ebufp);
    if (vp2 != NULL) {
        size_t start_t_len = pin_strlcpy(start_t, ctime((time_t*)vp2), sizeof(start_t));
        /* Remove new line character at the end */
        start_t[start_t_len-1] = '\0';
    }

    vp2 = PIN_FLIST_FLD_GET((pin_flist_t*)vp1, PIN_FLD_END_T, 0, ebufp);
    if (vp2 != NULL) {
        size_t end_t_len = pin_strlcpy(end_t, ctime((time_t*)vp2), sizeof(end_t));
        /* Remove new line character at the end */
        end_t[end_t_len-1] = '\0';
    }

    /* get  threshold details */
    obj_type[0] = '\0';
    filter_str[0] = '\0';
    threshold_str[0] = '\0';
    cookie = NULL;
    elem_id = 0;

    vp2 = PIN_FLIST_FLD_GET((pin_flist_t*)threshold_flistp, 
              PIN_FLD_OBJECT_TYPE, 0, ebufp);

    if (vp2 != NULL) {
        pin_strlcpy(obj_type, (char*)vp2, sizeof(obj_type));
    }

    /*
    * format Filter string
    */
    tmp_flistp = PIN_FLIST_CREATE(ebufp);

    while( (vp1 = PIN_FLIST_ELEM_GET_NEXT(threshold_flistp, 
            PIN_FLD_FILTER_DETAILS, &elem_id, 1, &cookie, ebufp)) != (pin_flist_t*)NULL)
    {
        vp2 = PIN_FLIST_FLD_GET((pin_flist_t*)vp1, PIN_FLD_FIELD_NAME, 1, ebufp);

        if (vp2 != NULL) {
            if (isFirst) {
                isFirst = 0;
            }
            else {
                string_flistp = PIN_FLIST_ELEM_ADD(tmp_flistp, PIN_FLD_MESSAGES, string_index, ebufp);
                PIN_FLIST_FLD_SET(string_flistp, PIN_FLD_STRING, (void *)str_and, ebufp);
                ++string_index;
                string_length += strnlen(str_and,5);
            }

            pin_strlcpy(filter_str, str_open_brace, sizeof(filter_str));
            pin_strlcat(filter_str, (char*)vp2, sizeof(filter_str));
            pin_strlcat(filter_str, str_equal, sizeof(filter_str));

            string_flistp = PIN_FLIST_ELEM_ADD(tmp_flistp, PIN_FLD_MESSAGES, string_index, ebufp);
            PIN_FLIST_FLD_SET(string_flistp, PIN_FLD_STRING, (void *)filter_str, ebufp);
            ++string_index;
            string_length += strnlen(filter_str,sizeof(filter_str));
        }
        else 
            continue;

        /* Based on field datatype get PIN_FLD_INT_VAL or PIN_FLD_STR_VAL */
        fld_p = strrchr((char*)vp2, '.');
        if(!fld_p)
            fld_p = (char*)vp2;
        else
            fld_p++;

        fld_code = pin_field_of_name(fld_p);
        type = pin_type_of_field(fld_code);

        if (type==PIN_FLDT_STR) {
            vp2 = PIN_FLIST_FLD_GET((pin_flist_t*)vp1, PIN_FLD_STR_VAL, 1, ebufp);
            if (vp2 != NULL) {
                string_flistp = PIN_FLIST_ELEM_ADD(tmp_flistp, PIN_FLD_MESSAGES, string_index, ebufp);
                PIN_FLIST_FLD_SET(string_flistp, PIN_FLD_STRING, (void *)vp2, ebufp);
                ++string_index;
                string_length += strnlen((char*)vp2,STRBUF_LEN);
            }
        }
        else {
            vp2 = PIN_FLIST_FLD_GET((pin_flist_t*)vp1, PIN_FLD_INT_VAL, 1, ebufp);
            if (vp2 != NULL) {
                pin_snprintf(tmp_str, sizeof(tmp_str), "%d", *((int*)vp2));
                string_flistp = PIN_FLIST_ELEM_ADD(tmp_flistp, PIN_FLD_MESSAGES, string_index, ebufp);
                PIN_FLIST_FLD_SET(string_flistp, PIN_FLD_STRING, (void *)tmp_str, ebufp);
                ++string_index;
                string_length += strnlen(tmp_str,sizeof(tmp_str));
            }
        }

        string_flistp = PIN_FLIST_ELEM_ADD(tmp_flistp, PIN_FLD_MESSAGES, string_index, ebufp);
        PIN_FLIST_FLD_SET(string_flistp, PIN_FLD_STRING, (void *)str_close_brace, ebufp);
        ++string_index;
        string_length += 2;  //str_close_brace is length is 2 , removed use of strlen
    }

    /* Combine to form one string */
    fltr_string_len = (sizeof(char) * (string_length + 1));
    fltr_string = (char*) pin_malloc(fltr_string_len);
    if(!fltr_string)
    {
        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
            "fm_process_audit_pol: memory allocation error");
	return;
    }
   pin_memset(fltr_string, fltr_string_len, '\0', fltr_string_len);
    for (i = 0; i < string_index; i++) {
        string_flistp = PIN_FLIST_ELEM_GET(tmp_flistp, PIN_FLD_MESSAGES, i, 0, ebufp);
	if( string_flistp != NULL) {
		char *strp = (char *) PIN_FLIST_FLD_GET((pin_flist_t*)string_flistp, 
						PIN_FLD_STRING, 0, ebufp);
		if( strp != NULL) {
        		pin_strlcat(fltr_string, strp, fltr_string_len);
		}
	}
    }
    /* Destroy flist */
    PIN_FLIST_DESTROY_EX(&tmp_flistp, NULL);


    elem_id = 0;
    cookie = NULL;
    isFirst = 1;
    while( (vp1 = PIN_FLIST_ELEM_GET_NEXT(threshold_flistp, PIN_FLD_VALUE_RANGES, 
                    &elem_id, 1, &cookie, ebufp)) != (pin_flist_t*)NULL)
    {

        fld_name = (char*)PIN_FLIST_FLD_GET((pin_flist_t*)vp1, 
            PIN_FLD_FIELD_NAME, 1, ebufp);

        if (fld_name != NULL) {
            if (isFirst) {
                isFirst = 0;
            }
            else
                pin_strlcat(threshold_str, " && ", sizeof(threshold_str));

            pin_strlcat(threshold_str, "(", sizeof(threshold_str));
            pin_strlcat(threshold_str, fld_name, sizeof(threshold_str));
            pin_strlcat(threshold_str, " >= ", sizeof(threshold_str));
        }

        vp2 = PIN_FLIST_FLD_GET((pin_flist_t*)vp1, PIN_FLD_MAXIMUM, 1, ebufp);
        if (vp2 != NULL) {
	    char *dstrp = NULL;
	    dstrp = pbo_decimal_to_str((pin_decimal_t *)vp2, ebufp);
	    if( dstrp != NULL ) {
            	pin_strlcpy(tmp_str, dstrp, sizeof(tmp_str));
                pin_strlcat(threshold_str, tmp_str, sizeof(threshold_str));
		PIN_FREE_EX(&dstrp);                
	    }
        }

        if (fld_name != NULL) {
            pin_strlcat(threshold_str, " || ", sizeof(threshold_str));
            pin_strlcat(threshold_str, fld_name, sizeof(threshold_str));
            pin_strlcat(threshold_str, " <= ", sizeof(threshold_str));
        }

        vp2 = PIN_FLIST_FLD_GET((pin_flist_t*)vp1, PIN_FLD_MINIMUM, 1, ebufp);
        if (vp2 != NULL) {
	    char *dstrp = NULL;
	    dstrp = pbo_decimal_to_str((pin_decimal_t *)vp2, ebufp);
	    if ( dstrp != NULL ) {
		 pin_strlcpy(tmp_str, dstrp, sizeof(tmp_str));
		 pin_strlcat(threshold_str, tmp_str, sizeof(threshold_str));
		 PIN_FREE_EX(&dstrp);
	    }
        }

        pin_strlcat(threshold_str, ")", sizeof(threshold_str));
    }

    sub = PIN_FLIST_FLD_GET(locale_flistp, PIN_FLD_SUBJECT, 1, ebufp);
    msg = PIN_FLIST_FLD_GET(locale_flistp, PIN_FLD_STRING, 1, ebufp);

    /* replace parameters in subject and message with dynamic values 
    *  free memory after each call to fm_process_audit_pol_replace_param()
    *  as it allocates memory dynamically to return new string
    */
 
    /* Replace %ALERT_NAME with alert name */
    t1 = sub = fm_process_audit_pol_replace_param(sub, "%ALERT_NAME", alert_namep);
    t2 = msg = fm_process_audit_pol_replace_param(msg, "%ALERT_NAME", alert_namep);

    /* Replace %START_TIME with start timestamp */
    sub = fm_process_audit_pol_replace_param(sub, "%START_TIME", start_t);
    msg = fm_process_audit_pol_replace_param(msg, "%START_TIME", start_t);
    PIN_FREE_EX(&t1);
    PIN_FREE_EX(&t2);
    t1 = sub;
    t2 = msg;

    /* Replace %END_TIME with end timestamp */
    sub = fm_process_audit_pol_replace_param(sub, "%END_TIME", end_t);
    msg = fm_process_audit_pol_replace_param(msg, "%END_TIME", end_t);
    PIN_FREE_EX(&t1);
    PIN_FREE_EX(&t2);
    t1 = sub;
    t2 = msg;

    /* Replace %OBJECT_TYPE with object type */
    sub = fm_process_audit_pol_replace_param(sub, "%OBJECT_TYPE", obj_type);
    msg = fm_process_audit_pol_replace_param(msg, "%OBJECT_TYPE", obj_type);
    PIN_FREE_EX(&t1);
    PIN_FREE_EX(&t2);
    t1 = sub;
    t2 = msg;

    /* Replace %FILTERS with filter string */
    sub = fm_process_audit_pol_replace_param(sub, "%FILTERS", fltr_string);
    msg = fm_process_audit_pol_replace_param(msg, "%FILTERS", fltr_string);
    PIN_FREE_EX(&t1);
    PIN_FREE_EX(&t2);
    t1 = sub;
    t2 = msg;

    /* Replace %THRESHOLD_VALUES with threshold string */
    sub = fm_process_audit_pol_replace_param(sub, "%THRESHOLD_VALUES", threshold_str);
    msg = fm_process_audit_pol_replace_param(msg, "%THRESHOLD_VALUES", threshold_str);
    PIN_FREE_EX(&t1);
    PIN_FREE_EX(&t2);
    t2 = msg;

    /* Replace %LINE_BREAK with new line char '\n' (only for message) */
    msg = fm_process_audit_pol_replace_param(msg, "%LINE_BREAK", "\n");
    PIN_FREE_EX(&t2);

    /* Put new subject and message back on locale flist 
    *   Memory ownership with flist
    */
    if (sub)
        PIN_FLIST_FLD_PUT(locale_flistp, PIN_FLD_SUBJECT, sub, ebufp);

    if (msg)
        PIN_FLIST_FLD_PUT(locale_flistp, PIN_FLD_STRING, msg, ebufp);

    /* Clean up */
    PIN_FREE_EX(&fltr_string);
}


/*************************************************
* Routine to send email to configured recepients
*************************************************/
static void
	fm_process_audit_pol_send_email(
    pcm_context_t      *ctxp,
    pin_flist_t        *alert_flistp,
    pin_flist_t        *loc_flistp,
    pin_flist_t        **o_flistpp,
    pin_errbuf_t       *ebufp)
{
    pin_flist_t    *email_flist = NULL;
    pin_flist_t    *sub_flistp = NULL;
    pin_flist_t    *rec_flistp = NULL;

    poid_t  *a_pdp = NULL;
    void    *vp = NULL;
    int32   elem_id = 0;
    char    *mp = NULL;
    size_t  mp_len = 0;
    pin_buf_t       *pin_bufp = NULL;
    pin_cookie_t    cookie = NULL;

    if (PIN_ERR_IS_ERR(ebufp))
        return;
    PIN_ERR_CLEAR_ERR(ebufp);

    *o_flistpp = NULL;

    email_flist = PIN_FLIST_CREATE(ebufp);

    /* get account poid to use on email flist */
    a_pdp = fm_process_audit_pol_get_brand(ctxp, ebufp);
    PIN_FLIST_FLD_PUT(email_flist, PIN_FLD_POID, a_pdp, ebufp);

    sub_flistp = PIN_FLIST_ELEM_ADD(email_flist, PIN_FLD_MESSAGES, 0, ebufp);

    vp = PIN_FLIST_FLD_GET(alert_flistp, PIN_FLD_SENDER, 0, ebufp);
    if (vp) {
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_SENDER, vp, ebufp);
    }

    vp = PIN_FLIST_FLD_GET(loc_flistp, PIN_FLD_SUBJECT, 0, ebufp);
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_SUBJECT, vp, ebufp);

    PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_TYPE_STR, (void *)"text/plain", ebufp);
	
    /* Add message body */
    vp = PIN_FLIST_FLD_GET(loc_flistp, PIN_FLD_STRING, 0, ebufp);

    pin_bufp = (pin_buf_t *) pin_malloc(sizeof(pin_buf_t));
    if (pin_bufp == NULL) {
        PIN_FLIST_DESTROY_EX(&email_flist, NULL);

        pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_SYSTEM_DETERMINATE, 
            PIN_ERR_NO_MEM, 0, 0, 0);

        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
            "fm_process_audit_pol_send_email: memory allocation error");

        return;
    }

    mp_len = strnlen(((char*)vp),STRBUF_LEN)+1;
    mp = (char *) pin_malloc(mp_len);
    if (mp == NULL) {
        PIN_FLIST_DESTROY_EX(&email_flist, NULL);
	PIN_FREE_EX(&pin_bufp);

        pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_SYSTEM_DETERMINATE, 
            PIN_ERR_NO_MEM, 0, 0, 0);

        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
            "fm_process_audit_pol_send_email: memory allocation error");

        return;
    }

    int32 mpLen = (int32)pin_strlcpy(mp, (char*)vp, mp_len);

    pin_bufp->flag = 0;
    pin_bufp->size = mpLen + 1;
    pin_bufp->offset = 0;
    pin_bufp->data = mp;
    pin_bufp->xbuf_file = (char *)NULL;

    PIN_FLIST_FLD_PUT(sub_flistp, PIN_FLD_LETTER, pin_bufp, ebufp);

    /* Add recepients */
    while( (vp = PIN_FLIST_ELEM_GET_NEXT(alert_flistp, PIN_FLD_RECIPIENTS, 
            &elem_id, 1, &cookie, ebufp)) != (pin_flist_t*)NULL)
    {
        rec_flistp = PIN_FLIST_ELEM_ADD(sub_flistp, PIN_FLD_RECIPIENTS, elem_id, ebufp);
        PIN_FLIST_CONCAT(rec_flistp, (pin_flist_t*)vp, ebufp);
    }

    PCM_OP(ctxp, PCM_OP_DELIVERY_MAIL_SENDMSGS, 0, email_flist, o_flistpp, ebufp);

    /* Clean up */
    PIN_FLIST_DESTROY_EX(&email_flist, NULL);

    return;
}


/*************************************************
* Routine to get current brand using the context
*************************************************/
static poid_t *
fm_process_audit_pol_get_brand(
    pcm_context_t       *ctxp,
    pin_errbuf_t        *ebufp)
{
    poid_t  *a_pdp = NULL;

    a_pdp = cm_fm_perm_get_brand_account(ebufp);

    if (PIN_POID_IS_NULL(a_pdp)) {
            a_pdp = fm_utils_lineage_root_poid(
                    cm_fm_get_current_db_no(ctxp), ebufp);
    } 
    else {
            a_pdp = PIN_POID_COPY(a_pdp, ebufp);
    }

    return a_pdp;
}


/*************************************************
* Routine to replace all occurances of parameter
* with given string, memory allocated dynamically
* hence calling function frees the returned string
* This is recursive function. Intermediate memory 
* is freed by the routine
*************************************************/
static char*
fm_process_audit_pol_replace_param(
    const char * source_str,
    const char * param_str,
    const char * replace_str)
{
    size_t i;
    char *new_str;
    size_t new_str_len = 0;
    char *p;
    size_t slen = strnlen(source_str,STRBUF_LEN);
    size_t plen = strnlen(param_str,STRBUF_LEN); 
    size_t rlen = strnlen(replace_str,STRBUF_LEN);

    if (source_str == NULL || replace_str == NULL || param_str == NULL) {
        return NULL;
    }

    new_str_len = (sizeof(char) * (slen + rlen + 1));
    new_str = (char*) pin_malloc(new_str_len);

    if (new_str == NULL) {
        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
            "fm_process_audit_pol_replace_param: memory allocation error");
        return NULL;
    }

    /* initialize the new_str with NULL */
    pin_memset(new_str, new_str_len, '\0', new_str_len);


    for (i = 0; i < slen; i++)
    {
        /* look for place to put the replacement string */
        if ( !strncmp(source_str+i, param_str, plen) ) {
            pin_strlcpy(new_str, source_str, i);
            pin_strlcat(new_str, replace_str, new_str_len);
            pin_strlcat(new_str, (source_str+i+plen), new_str_len);

            /* Call the function recursively to replace all occurances */
            p = new_str;
            new_str = fm_process_audit_pol_replace_param(new_str,param_str,replace_str);

            /* free memory allocated by earlier call */
	    PIN_FREE_EX(&p);
            break;
        }
        else if ( i == slen-1) {
            pin_strlcpy(new_str, source_str, new_str_len);
            break;
        }
    }

    return new_str;
}

