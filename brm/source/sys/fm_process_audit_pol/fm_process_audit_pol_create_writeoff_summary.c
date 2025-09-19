/*
 *      Copyright (c) 2004-2007 Oracle Corp. All rights reserved.
 *      This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "@(#) %full_filespec: fm_process_audit_pol_create_writeoff_summary.c~1.0.0:csrc:1 %";
#endif

/*******************************************************************
 * Contains the PCM_OP_PROCESS_AUDIT_POL_CREATE_WRITEOFF_SUMMARY operation.
 *******************************************************************/

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>

#define FILE_LOGNAME "fm_process_audit_pol_create_writeoff_summary.c(1)"

#include "ops/process_audit.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_channel.h"
#include "fm_utils.h"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/

EXPORT_OP void
op_process_audit_pol_create_writeoff_summary(
    cm_nap_connection_t *connp,
    int32               opcode,
    int32               flags,
    pin_flist_t         *i_flistp,
    pin_flist_t         **r_flistpp,
    pin_errbuf_t        *ebufp);

static void
fm_process_audit_pol_create_writeoff_summary(
    pcm_context_t      *ctxp,
    int32              flags,
    pin_flist_t        *i_flistp,
    pin_flist_t        **r_flistpp,
    pin_errbuf_t       *ebufp);


static void
fm_process_audit_map_suspended_usage_fields(
    pin_flist_t       *sub_flistp,
    pin_decimal_t     **volume_sentp,
    pin_decimal_t     **volume_receivedp,
    pin_decimal_t     **event_durationp,
    pin_errbuf_t      *ebufp);
    

static void
fm_process_audit_prepare_flist(
    pin_flist_t      *r_flistp,
    const  char      *original_batch_id,
    const  char      *suspended_from_batch_id,
    const char       *pipeline_name,
    int32            edr_count,
    pin_decimal_t    *total_volume_sentp,
    pin_decimal_t    *total_volume_receivedp,
    pin_decimal_t    *total_call_duration,
    int32            array_elem_id,
    pin_errbuf_t     *ebufp);
    

/******************************************************************************
 * Main routine for the PCM_OP_PROCESS_AUDIT_POL_CREATE_WRITEOFF_SUMMARY opr.
 *****************************************************************************/
void
op_process_audit_pol_create_writeoff_summary(
    cm_nap_connection_t  *connp,
    int32                opcode,
    int32                flags,
    pin_flist_t          *i_flistp,
    pin_flist_t          **r_flistpp,
    pin_errbuf_t         *ebufp)
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
        if (opcode != PCM_OP_PROCESS_AUDIT_POL_CREATE_WRITEOFF_SUMMARY) {
            pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_SYSTEM_DETERMINATE,
            PIN_ERR_BAD_OPCODE, 0, 0, opcode);
            PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
            "op_process_audit_pol_create_writeoff_summary opcode error", ebufp);

            return;
        }
    

        /***********************************************************
         * Debug what we got.
        ***********************************************************/
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        "op_process_audit_pol_create_writeoff_summary input flist", i_flistp);


        /***********************************************************
        * Main rountine for this opcode
        ***********************************************************/
        fm_process_audit_pol_create_writeoff_summary(ctxp, flags, i_flistp, r_flistpp, ebufp);
            

        /***********************************************************
        * Error?
        ***********************************************************/
        if (PIN_ERR_IS_ERR(ebufp)) {
        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
        "op_process_audit_pol_create_writeoff_summary error", ebufp);

        PIN_FLIST_DESTROY_EX(r_flistpp, NULL);
        } else {
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        "op_process_audit_pol_create_writeoff_summary output flist", *r_flistpp);
        }

        return;
}

/*******************************************************************
 * fm_process_audit_pol_create_writeoff_summary:
 *******************************************************************/
static void
fm_process_audit_pol_create_writeoff_summary(
    pcm_context_t    *ctxp,
    int32            flags,
    pin_flist_t      *i_flistp,
    pin_flist_t      **r_flistpp,
    pin_errbuf_t     *ebufp)
{
    pin_flist_t            *sub_flistp             = NULL;
    pin_cookie_t           cookie                  = NULL;

    int32                  elem_id                 = -1;
    int32                  array_elem_id           = 1;    

    pin_decimal_t         *total_volume_sentp      = NULL;
    pin_decimal_t         *total_volume_receivedp  = NULL;
    pin_decimal_t         *volume_sentp            = NULL;
    pin_decimal_t         *volume_receivedp        = NULL;
    pin_decimal_t         *event_durationp         = NULL;
    pin_decimal_t         *total_event_durationp   = NULL;

    double                volume_sent              = 0;
    double                volume_received          = 0;

    const char            *batch_idp               = NULL;
    const char            *orig_batch_idp          = NULL;
    const char            *pipeline_name           = NULL;
    const char            *last_batch_idp          = NULL;
    const char            *last_orig_batch_idp     = NULL;
    const char            *last_pipeline_name     = NULL;

    int                   totalEDRs                = 0;
    int                   totalBatchEDRs           = 0;
    short                 isFirst                  = 1;

    void                  *vp                      = NULL;


    if (PIN_ERR_IS_ERR(ebufp)) {
        return;
    }

    PIN_ERR_CLEAR_ERR(ebufp);


    vp = PIN_FLIST_FLD_GET((pin_flist_t*)i_flistp, PIN_FLD_POID, 0, ebufp);

    *r_flistpp =  PIN_FLIST_CREATE(ebufp);

    PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, vp, ebufp);

    while ((sub_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
        PIN_FLD_RESULTS, &elem_id, 1, &cookie, ebufp)) != (pin_flist_t*) NULL)
        {

             fm_process_audit_map_suspended_usage_fields(sub_flistp,
                                                &volume_sentp, &volume_receivedp,
                                                &event_durationp, ebufp);

            if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_process_audit_pol_create_writeoff_summary "
                        ": error in mapping object    ",     ebufp);
                return;
            }

            totalEDRs++;

            batch_idp = (const char*)PIN_FLIST_FLD_GET(sub_flistp,
                                PIN_FLD_SUSPENDED_FROM_BATCH_ID, 0, ebufp);

            orig_batch_idp = (const char*)PIN_FLIST_FLD_GET(sub_flistp,
                                        PIN_FLD_BATCH_ID, 0, ebufp);

            pipeline_name = (const char*)PIN_FLIST_FLD_GET(sub_flistp,
                                        PIN_FLD_PIPELINE_NAME, 0, ebufp);

	    if( (batch_idp == NULL) || 
	        (orig_batch_idp == NULL) || 
	        (pipeline_name == NULL) )  {
			return;
	    }

            /* 
             *    if batch_id is null set it equal to orig_batch_id 
             */
            if ( !strcmp(batch_idp, "") && strcmp(orig_batch_idp, "")) {
                    batch_idp = orig_batch_idp;
            }


            /* 
             *    If first record or batch_id and orig_batch_id
             *  are same as previous then increase EDRcount
             */
            if ( isFirst
                    || (!strcmp(batch_idp, last_batch_idp) &&
                    !strcmp(orig_batch_idp, last_orig_batch_idp) && 
                    !strcmp(pipeline_name, last_pipeline_name)) )
            {

                totalBatchEDRs  = totalBatchEDRs + 1;

                if(isFirst == 1)
                {
                    total_event_durationp     = pbo_decimal_copy(event_durationp,     
                                                                    ebufp);
                    total_volume_sentp         = pbo_decimal_copy(volume_sentp,
                                                                    ebufp);
                    total_volume_receivedp     = pbo_decimal_copy(volume_receivedp,
                                                                    ebufp);
                }
                else
                {

                    pbo_decimal_add_assign(total_volume_sentp,  volume_sentp, 
                                                                    ebufp);
                    pbo_decimal_add_assign(total_volume_receivedp, volume_receivedp,     
                                                                    ebufp);
                    pbo_decimal_add_assign(total_event_durationp,     
                                                    event_durationp, ebufp);
                }
            

                if ( PIN_ERR_IS_ERR(ebufp) ) {
                    PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_process_audit_pol_create_writeoff_summary"
                        ": Decimal Manipulation Error", ebufp);
                    return;
                }

                isFirst = 0;
            
            }
            else 
            {
                /* 
                 * create /process_audit flist for previous batch 
                 */
                fm_process_audit_prepare_flist(*r_flistpp ,
				last_orig_batch_idp ,
				last_batch_idp,
				last_pipeline_name,
				totalBatchEDRs, 
				total_volume_sentp,
				total_volume_receivedp, 
				total_event_durationp,
				array_elem_id, ebufp);

                if ( PIN_ERR_IS_ERR(ebufp) ) {
                    PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                    "fm_process_audit_pol_create_writeoff_summary: "
                    "error preparing flist for process_audit objects"
                                                            ,ebufp);

                    pbo_decimal_destroy(&total_volume_sentp);
                    pbo_decimal_destroy(&total_volume_receivedp);
                    pbo_decimal_destroy(&total_event_durationp);
                    return;
                }

                pbo_decimal_destroy(&total_volume_sentp);
                pbo_decimal_destroy(&total_volume_receivedp);
                pbo_decimal_destroy(&total_event_durationp);

                totalBatchEDRs = 1;

                total_event_durationp     = pbo_decimal_copy(event_durationp,     
                                                                ebufp);
                total_volume_sentp         = pbo_decimal_copy(volume_sentp,
                                                                ebufp);
                total_volume_receivedp     = pbo_decimal_copy(volume_receivedp,
                                                                ebufp);

                array_elem_id             = array_elem_id + 1;

            }

            last_batch_idp = batch_idp;

            last_orig_batch_idp = orig_batch_idp;

            last_pipeline_name = pipeline_name;

            pbo_decimal_destroy(&volume_sentp);
            pbo_decimal_destroy(&volume_receivedp);
            pbo_decimal_destroy(&event_durationp);


        } /* end while() */


        /* 
         *    prepare process_audit flist for last batch 
         */
        if(totalBatchEDRs > 0)
        {
            fm_process_audit_prepare_flist(*r_flistpp,
				last_orig_batch_idp,
				last_batch_idp,
				last_pipeline_name,
				totalBatchEDRs,
				total_volume_sentp,
				total_volume_receivedp,
				total_event_durationp,
				array_elem_id, ebufp);

            if ( PIN_ERR_IS_ERR(ebufp) ) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_process_audit_pol_create_writeoff_summary:" 
                        "error preparing flist for process_audit objects"
                                                            , ebufp);
                return;
            }
                    pbo_decimal_destroy(&total_volume_sentp);
                    pbo_decimal_destroy(&total_volume_receivedp);
                    pbo_decimal_destroy(&total_event_durationp);
        }
        else
        {
            if ( (!PIN_ERR_IS_ERR(ebufp)) && !totalEDRs  ) {
                    PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_WARNING,
                    "fm_process_audit_pol_create_writeoff_summary:" 
                    "/suspended_usage/telco object not in i/p flist");
            }    
        }    

}


/*        
 *    this function maps the fields from suspended usage objects
 *  to fields in the process audit objects.if the object not
 *  found or error results it returns 0.On success it
 *    returns 1
*/
static void
fm_process_audit_map_suspended_usage_fields(
    pin_flist_t     *sub_flistp,
    pin_decimal_t   **volume_sentp,
    pin_decimal_t   **volume_receivedp,
    pin_decimal_t   **event_durationp,
    pin_errbuf_t    *ebufp)
{
        pin_flist_t       *telco_flistp   = NULL;
        poid_t            *poidp          = NULL;

        int32             *bytes_inp      = NULL;
        int32             *bytes_outp     = NULL;
        pin_decimal_t     *call_durationp = NULL;
        const char        *poid_typep     = NULL;

        double           volume_sent      = 0;
        double           volume_received  = 0;

        poidp        =  PIN_FLIST_FLD_GET(sub_flistp,
                            PIN_FLD_POID, 0, ebufp);

        if ( PIN_ERR_IS_ERR(ebufp) ) {
            PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                    "fm_process_audit_map_suspended_usage_fields:" 
                    "poid type not found in the input flist", ebufp);
        }

        poid_typep = PIN_POID_GET_TYPE(poidp);    
        if (!strcmp(poid_typep,"/suspended_usage/telco") ||
	    !strncmp(poid_typep,"/suspended_usage/telco/", sizeof("/suspended_usage/telco/")-1))
        {
            telco_flistp = (pin_flist_t *)PIN_FLIST_SUBSTR_GET(sub_flistp,
                                               PIN_FLD_TELCO_INFO, 0, ebufp);    

            bytes_inp = PIN_FLIST_FLD_GET(telco_flistp, PIN_FLD_BYTES_IN ,
                                                                    0, ebufp);
            if(bytes_inp)
                volume_received = *bytes_inp;

            bytes_outp = PIN_FLIST_FLD_GET(telco_flistp, PIN_FLD_BYTES_OUT,
                                                                    0, ebufp);
            if(bytes_outp)
                volume_sent = *bytes_outp;


            call_durationp = PIN_FLIST_FLD_GET(telco_flistp,
                                            PIN_FLD_CALL_DURATION, 0, ebufp);    

            *event_durationp     = pbo_decimal_copy(call_durationp, ebufp);     

            *volume_sentp        = pbo_decimal_from_double(volume_sent , ebufp);

            *volume_receivedp    = pbo_decimal_from_double(volume_received , ebufp);

            if ( PIN_ERR_IS_ERR(ebufp) ) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_process_audit_map_suspended_usage_fields:" 
                        "required fields not found in the flist", ebufp);

                pbo_decimal_destroy(volume_sentp);
                pbo_decimal_destroy(volume_receivedp);
                pbo_decimal_destroy(event_durationp);
                return ;
            }

        }
		else
		{
		    pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, 
		                PIN_ERR_BAD_ARG, 0, 0, 0);
            PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                     "fm_process_audit_map_suspended_usage_fields:" 
                      "object other than /suspended_usage/telco found", ebufp);
				
		}

        return ;

}

        
/*         
 *        this fuction maps suspended_usage fields to process_audit fields
 *      and prepare Group Array flist for creating process_audit object 
 */

static void 
fm_process_audit_prepare_flist(
    pin_flist_t     *r_flistp,
    const char      *original_batch_idp,
    const char      *suspended_from_batch_idp,
    const char      *pipeline_name,
    int32           edr_count,
    pin_decimal_t   *total_volume_sentp,
    pin_decimal_t   *total_volume_receivedp,
    pin_decimal_t   *total_call_durationp,
    int32           array_elem_id,
    pin_errbuf_t    *ebufp)
{
    pin_flist_t        *sub_flistp = NULL;    

    sub_flistp = PIN_FLIST_ELEM_ADD(r_flistp, PIN_FLD_GROUP_DETAILS, 
                    array_elem_id, ebufp);

    PIN_FLIST_FLD_SET(sub_flistp,PIN_FLD_ORIGINAL_BATCH_ID,
                    (void *)original_batch_idp, ebufp);

    PIN_FLIST_FLD_SET(sub_flistp,PIN_FLD_SUSPENDED_FROM_BATCH_ID,
                    (void *)suspended_from_batch_idp, ebufp);

    PIN_FLIST_FLD_SET(sub_flistp,PIN_FLD_PIPELINE_NAME,
                    (void *)pipeline_name, ebufp); 

    PIN_FLIST_FLD_SET(sub_flistp,PIN_FLD_EVENT_COUNT,
                    (void *)&edr_count, ebufp); 

    PIN_FLIST_FLD_SET(sub_flistp,PIN_FLD_VOLUME_SENT,
                    (void *)total_volume_sentp, ebufp);

    PIN_FLIST_FLD_SET(sub_flistp,PIN_FLD_VOLUME_RECEIVED,
                    (void *)total_volume_receivedp, ebufp);

    PIN_FLIST_FLD_SET(sub_flistp,PIN_FLD_EVENT_DURATION,
                    (void *)total_call_durationp, ebufp);

}
