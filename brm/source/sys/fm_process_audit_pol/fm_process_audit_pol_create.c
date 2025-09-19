/*******************************************************************
 *
 * @(#)fm_process_audit_pol_create.c 1.0
 *
* Copyright (c) 2003, 2023, Oracle and/or its affiliates.
 *
 *     This material is the confidential property of Oracle Corporation
 *     or licensors and may be used, reproduced, stored or transmitted
 *     only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static char Sccs_id[] = "@(#)$Id: fm_process_audit_pol_create.c /cgbubrm_7.5.0.portalbase/5 2016/05/18 14:39:32 arcmisra Exp $";
#endif

#include <stdio.h>
#include <string.h>

#include "pcm.h"
#include "ops/process_audit.h"
#include "pin_cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "fm_utils.h"
#include "fm_bill_utils.h"

#define FILE_SOURCE_ID	"fm_process_audit_pol_create.c (3)"

#define MAX_THRESHOLD_VALUE	20
#define BASE_POID_TYPE	"/process_audit"
#define PIPE_LINE_POID_TYPE	"/process_audit/pipe_linie"
#define BILLING_POID_TYPE	"/process_audit/billing"
#define PRICING_POID_TYPE       "/process_audit/price_sync"
#define CONFIG_POID_TYPE	"/config/process_audit"
#define PIN_OBJ_TYPE_JOB_BOC    "/job/boc"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void op_process_audit_pol_create();
void fm_process_audit_pol_create();
void fm_process_audit_pol_create_prepare_input_flist();
void fm_process_audit_pol_read_config();
void fm_process_audit_pol_create_check_duplicate();
/*******************************************************************
 * Main routine for the PCM_OP_PROCESS_AUDIT_POL_CREATE operation.
 *******************************************************************/
void
op_process_audit_pol_create(
        cm_nap_connection_t	*connp,
	int32			opcode,
        int32			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**r_flistpp,
        pin_errbuf_t		*ebufp)
{
	void			*poid_ptr = NULL;
	pcm_context_t   *ctxp = connp->dm_ctx;
	pin_flist_t             *t_flistp;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Null out results until we have some.
	 */
	*r_flistpp = NULL;

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_PROCESS_AUDIT_POL_CREATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_process_audit_create", 
			ebufp); 
		return;
	}

	/*
	 * Debug: What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_process_audit_pol_create input flist", i_flistp);

	fm_process_audit_pol_create(ctxp,flags,i_flistp, &t_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_process_audit_create error", ebufp);
	} else {
		*r_flistpp = t_flistp;
		 PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "return flist ", *r_flistpp);
	}
	return;
}

void 
fm_process_audit_pol_create(
        pcm_context_t   *ctxp,
        uint            flags,
        pin_flist_t     *i_flistp,
        pin_flist_t     **r_flistp,
        pin_errbuf_t    *ebufp)
{

	pin_flist_t	*config_flistp = NULL;
	pin_flist_t	*t_flistp = NULL;
	char		*proc_name = NULL;
	/*
	 * Call the main worker function for process_audit...
	 * Call read config object (input i_flistp and output is config flist
	 * Call prepare input flist (input i_flist, config flist and output is processed flist
	 * if (config flist is not null) call duplicate check function for duplicate check
         *    input processed flist and config flist, output check error buf for results
	 */
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_process_audit_pol_create input flist", i_flistp);

	fm_process_audit_pol_read_config(ctxp,flags,i_flistp, &config_flistp, ebufp);

	if (config_flistp != NULL) {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "op_process_audit_pol_create config flist", config_flistp);
	}
	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "", ebufp);
	}
	fm_process_audit_pol_create_prepare_input_flist(ctxp,flags,i_flistp,config_flistp, &t_flistp, ebufp);
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_process_audit_pol_create input flist", i_flistp);
	if (t_flistp != NULL) {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_process_audit_pol_create prepare flist", t_flistp);
	}

	proc_name = (char *)PIN_FLIST_FLD_GET(i_flistp,PIN_FLD_PROCESS_NAME,1,ebufp);
	if (proc_name &&
			(strcmp(proc_name,PIN_ARA_BILL_NOW) !=0) &&
			(strcmp(proc_name,PIN_ARA_BILL_ON_DEMAND) !=0) &&
			(strcmp(proc_name,PIN_ARA_AUTO_TRIGGER_BILL) !=0)) {
		fm_process_audit_pol_create_check_duplicate(ctxp,flags,t_flistp,config_flistp, ebufp);
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(&t_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&config_flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_process_audit_pol_create error", ebufp);
	} else {
		PIN_FLIST_DESTROY_EX(&config_flistp, NULL);
		*r_flistp = t_flistp;
		 PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "return flist ", *r_flistp);
	}
	return;
}

void 
fm_process_audit_pol_read_config(
        pcm_context_t   *ctxp,
        uint            flags,
        pin_flist_t     *i_flistp,
        pin_flist_t     **r_flistpp,
        pin_errbuf_t    *ebufp)
{
	pin_flist_t	*config_search_flistp;
	pin_flist_t	*result_flistp;
	pin_flist_t	*res_flistp;
	pin_flist_t	*arg_flistp;
	pin_flist_t	*sub_flistp;
	poid_t		*ara_poidp;
	poid_t		*spoidp;
	poid_t		*poidp;
	char 		*poid_type;
	int		elem = 0;
	int		found_match = 0;
	int		search_template_id = 500;
	pin_cookie_t    cookie = NULL;
	void		*vp;
	char		buf[125];
	

	ara_poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp,
                PIN_FLD_POID, 0, ebufp);
	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "", ebufp);
                *r_flistpp = (pin_flist_t *)NULL;
                return;
        }  
        poid_type =  (char *) PIN_POID_GET_TYPE(ara_poidp);
        config_search_flistp = PIN_FLIST_CREATE(ebufp);
        spoidp = PIN_POID_CREATE(PIN_POID_GET_DB(ara_poidp), "/search", search_template_id, ebufp);
        PIN_FLIST_FLD_PUT(config_search_flistp, PIN_FLD_POID,
                spoidp, ebufp);
        PIN_FLIST_FLD_SET(config_search_flistp, PIN_FLD_PARAMETERS, "process_audit_conf", ebufp);

        arg_flistp = PIN_FLIST_ELEM_ADD(config_search_flistp, PIN_FLD_ARGS,
                                        1, ebufp);
        poidp = PIN_POID_CREATE(PIN_POID_GET_DB(ara_poidp), "/config/process_audit_conf",
                                -1, ebufp);
        PIN_FLIST_FLD_PUT(arg_flistp, PIN_FLD_POID, poidp,
                ebufp);

        PIN_FLIST_ELEM_SET(config_search_flistp, NULL, PIN_FLD_RESULTS,
                0, ebufp);
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_process_audit_pol_read_config search flist", config_search_flistp);

        PCM_OP(ctxp, PCM_OP_SEARCH, 0, config_search_flistp, &res_flistp,
                ebufp);


	if (PIN_ERR_IS_ERR(ebufp) || res_flistp == NULL) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_process_audit_pol_read_config error", ebufp);
		*r_flistpp = (pin_flist_t *) NULL;
		PIN_FLIST_DESTROY_EX(&res_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&config_search_flistp, NULL);
		return;
	}
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_process_audit_pol_read_config config object result is ", res_flistp);

        result_flistp = PIN_FLIST_ELEM_GET(res_flistp,
                                        PIN_FLD_RESULTS, 0, 1, ebufp);
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_process_audit_pol_read_config config object result is ", res_flistp);

        if (result_flistp != NULL) {
       		 while ((sub_flistp = PIN_FLIST_ELEM_GET_NEXT(result_flistp,
                                       PIN_FLD_AUDIT_CONFIG , &elem, 1, &cookie, ebufp)) != NULL) {
                                        vp = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_AUDIT_PROCESS, 1, ebufp);
                                        pin_snprintf (buf,sizeof(buf),"Poid type %s and config poid type %s",poid_type,(char *)vp);
                                        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, buf);
                                        if (vp != NULL) {
                                        if (! strcmp(poid_type, (char *)vp)) {

                                            found_match = 1;
                                                break;
                                        }
                                }
                }
		if (found_match == 1) {
			*r_flistpp = pin_flist_copy(sub_flistp,ebufp);
        		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                		"fm_process_audit_pol_read_config config return flist ", *r_flistpp);
		}else {
			*r_flistpp = (pin_flist_t *) NULL;
		}
        }
	PIN_FLIST_DESTROY_EX(&config_search_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&res_flistp, NULL);
	return;

}

void
fm_process_audit_pol_create_prepare_input_flist(
        pcm_context_t   *ctxp,
        uint            flags,
        pin_flist_t     *i_flistp,
        pin_flist_t     *conf_flistp,
        pin_flist_t     **r_flistpp,
        pin_errbuf_t    *ebufp)
{

	pin_flist_t		*prep_flistp = NULL;
	pin_flist_t		*buf_flistp = NULL;
	pin_errbuf_t		ebuf;
	poid_t		*poidp = NULL;
	char		*poid_type;
	pin_flist_t		*extended_flistp = NULL;
	pin_flist_t		*t_extended_flistp = NULL;
	pin_flist_t		*fail_acct_flistp = NULL;
	pin_flist_t		*billing_segmentsp = NULL;
	pin_flist_t		*fail_price_flistp = NULL;
	int32		rec_id = 0;
	int32		rec1_id = 0;
	int		threshold_val = MAX_THRESHOLD_VALUE;
	int		*tmp_valp = NULL;
	int		*failed_records = NULL;
	int		failed_cnt = 0;
	int32		fail_price_len = 0;
	pin_cookie_t		cookie = NULL;
	pin_cookie_t		cookie1 = NULL;
	pin_cookie_t		prev_cookie = NULL;
	char			buf[125];
	char		*failed_priceinfop = NULL;
	int64		database = 1;
	pin_buf_t		*pin_failed_bufp = NULL;
	pin_buf_t		*pin_new_failed_bufp = NULL;

        /*
         * If there is already a problem, return
         */
        if(PIN_ERR_IS_ERR(ebufp))
                return;

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"Input flist for fm_process_audit_pol_create_prepare_input_flist is  ", i_flistp);
	poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp,
                PIN_FLD_POID, 0, ebufp);
	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "", ebufp);
                *r_flistpp = (pin_flist_t *)NULL;
                return;
        }  
	poid_type = (char *) PIN_POID_GET_TYPE(poidp);
	if (conf_flistp != NULL) {
		tmp_valp  = (int *) PIN_FLIST_FLD_GET(conf_flistp,
			PIN_FLD_THRESHOLD ,1,ebufp);
		if (tmp_valp != NULL) {
			threshold_val = (*tmp_valp);
		}
	}
        if (threshold_val == 0 || threshold_val > MAX_THRESHOLD_VALUE) { threshold_val = MAX_THRESHOLD_VALUE ; }

	/******************
	*
	*  Needs customisation for extended classes of process_audit.
	*  It has the implementation for the extended classes billing and pipeline
	*
	********************/

	extended_flistp = (pin_flist_t*)PIN_FLIST_SUBSTR_TAKE(i_flistp, PIN_FLD_INHERITED_INFO,
                	1, ebufp);
	if (extended_flistp == NULL) { /* input flist does not have information of extended classes */
		*r_flistpp = PIN_FLIST_COPY(i_flistp,ebufp);
		return;
	}
	if (extended_flistp) {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"Inherited Info is ", extended_flistp);
		if (!strcmp( PIN_POID_GET_TYPE(poidp),"/process_audit/billing") ) {
				/* Get the data for extended class of billing   */
			t_extended_flistp = (pin_flist_t*)PIN_FLIST_SUBSTR_GET(extended_flistp,PIN_FLD_BILLING_INFO,1,ebufp);
			if (t_extended_flistp) {
				PIN_FLIST_SUBSTR_SET(i_flistp,t_extended_flistp,PIN_FLD_BILLING_INFO,ebufp);
			}	

			while ((billing_segmentsp = PIN_FLIST_ELEM_GET_NEXT(extended_flistp,
					PIN_FLD_BILLING_SEGMENTS,&rec_id,1,&cookie,ebufp)) != NULL) {

					PIN_FLIST_ELEM_SET(i_flistp, (void *) billing_segmentsp,
                       		         PIN_FLD_BILLING_SEGMENTS, rec_id,  ebufp);
			}

                        failed_records = (int *)PIN_FLIST_FLD_GET(i_flistp,
                                              PIN_FLD_FAILED_RECORDS, 1, ebufp);

			/**************************************************
                         * IF failed records is greater than the threshold 
                         * value then traverse the 
                         * i/p flist and remove the extra failed accounts
                         *************************************************/
                          
			if(failed_records)
                        {
                         if ( *failed_records > threshold_val)
			 { 
                           failed_cnt = 0; 
                           rec_id = 0;
                           cookie = NULL;
				while ((billing_segmentsp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
					PIN_FLD_BILLING_SEGMENTS,&rec_id,1,&cookie,ebufp)) != NULL) {

                          		cookie1 = NULL;
                          		rec1_id = 0;
					prev_cookie = NULL;

					while ((fail_acct_flistp = PIN_FLIST_ELEM_GET_NEXT(billing_segmentsp,
					PIN_FLD_FAILED_ACCOUNTS,&rec1_id,1,&cookie1,ebufp)) != NULL) {

                                  	failed_cnt++;

                                         if(PIN_ERR_IS_ERR(ebufp)) {
                                           PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						"Inside Biling segments failed accounts ",ebufp);
					  }

					 if (failed_cnt > threshold_val) {
                                           PIN_FLIST_ELEM_DROP(billing_segmentsp, 
							PIN_FLD_FAILED_ACCOUNTS,rec1_id, ebufp);
						cookie1 = prev_cookie;
					  }	
					  else {
						prev_cookie = cookie1;
					  }	
			   		}
                        	} 
                        	pin_snprintf (buf,sizeof(buf),"Limiting the failed records upto the threshold value (%d)",threshold_val);
                            	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, buf);
                         } 
                       }


			PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"final input flist is ", i_flistp);
		}
		if (!strcmp( PIN_POID_GET_TYPE(poidp),"/process_audit/pipeline") ) {
				/* Get the data for extended class of pipeline   */
			t_extended_flistp = PIN_FLIST_SUBSTR_GET(extended_flistp,PIN_FLD_PIPE_LINE_INFO,1,ebufp);
			if (t_extended_flistp) {
				PIN_FLIST_SUBSTR_SET(i_flistp,t_extended_flistp,PIN_FLD_PIPE_LINE_INFO,ebufp);

			}	
			while ((fail_acct_flistp = PIN_FLIST_ELEM_GET_NEXT(extended_flistp,
					PIN_FLD_FAILED_CDRS,&rec_id,1,&cookie,ebufp)) != NULL) {
				if (rec_id < threshold_val) {
					PIN_FLIST_ELEM_SET(i_flistp, (void *) fail_acct_flistp,
                       		         PIN_FLD_FAILED_CDRS, rec_id,  ebufp);
				}
				else {
					break;
				}	
			}
			PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"final input flist is ", i_flistp);
		}
		if (!strcmp( PIN_POID_GET_TYPE(poidp),"/process_audit/ledger_report") ) {
			/* Get the data for extended class of GL */
			t_extended_flistp = PIN_FLIST_SUBSTR_GET(extended_flistp,PIN_FLD_GL_INFO,0,ebufp);
			if (t_extended_flistp) {
				PIN_FLIST_SUBSTR_SET(i_flistp,t_extended_flistp,PIN_FLD_GL_INFO,ebufp);
			}
			PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"final input flist is ", i_flistp);
		}
		if (!strcmp( PIN_POID_GET_TYPE(poidp),"/process_audit/price_sync") ) {
			/* Get the data for extended class of price_sync */
			t_extended_flistp = (pin_flist_t*)PIN_FLIST_SUBSTR_GET(extended_flistp,PIN_FLD_PRICING_INFO,0,ebufp);
			if (t_extended_flistp) {
				pin_failed_bufp = (pin_buf_t*)PIN_FLIST_FLD_GET(t_extended_flistp, PIN_FLD_FAILED_PRICING_DETAILS, 1, ebufp);
				if(pin_failed_bufp != NULL){
					pin_str_to_flist(pin_failed_bufp->data, 
						PIN_POID_GET_DB(poidp),
						&buf_flistp, ebufp);
					PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
						"price_sync failed pricing flist is ", buf_flistp);
					failed_cnt = PIN_FLIST_ELEM_COUNT(buf_flistp, PIN_FLD_RESULTS, ebufp);
					/*Need to drop failed records more than threshold value*/
					if(failed_cnt > threshold_val){
						failed_cnt = 0;
						prev_cookie = NULL;
						cookie1 = NULL;
						rec1_id = 0;
						while ((fail_price_flistp = PIN_FLIST_ELEM_GET_NEXT(buf_flistp,
							PIN_FLD_RESULTS,&rec1_id,1,&cookie1,ebufp)) != NULL) {
							failed_cnt++;
							if (failed_cnt > threshold_val) {
								PIN_FLIST_ELEM_DROP(buf_flistp, 
									PIN_FLD_RESULTS,rec1_id, ebufp);
								cookie1 = prev_cookie;
							}
							else {
								prev_cookie = cookie1;
							}
						}
						
						PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
							"price_sync failed pricing flist after drop is ", buf_flistp);
						PIN_FLIST_TO_STR_COMPACT(buf_flistp, &failed_priceinfop, &fail_price_len,ebufp);
						pin_new_failed_bufp = (pin_buf_t *)pin_malloc(sizeof(pin_buf_t));
						if (pin_new_failed_bufp == NULL) {
							pin_set_err(ebufp, PIN_ERRLOC_FM,
								PIN_ERRCLASS_SYSTEM_DETERMINATE,
								PIN_ERR_NO_MEM, 0, 0 ,0);
							PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
								"Memory for pin_new_failed_bufp couldn't be allocated",
								ebufp);
							goto Done;
						}
						pin_new_failed_bufp->flag = 0;
						pin_new_failed_bufp->size = fail_price_len;
						pin_new_failed_bufp->offset = 0;
						pin_new_failed_bufp->data = failed_priceinfop;
						failed_priceinfop = NULL;
						pin_new_failed_bufp->xbuf_file = (char *)NULL;
						PIN_FLIST_FLD_DROP(t_extended_flistp, PIN_FLD_FAILED_PRICING_DETAILS, ebufp);
						PIN_FLIST_FLD_PUT(t_extended_flistp, PIN_FLD_FAILED_PRICING_DETAILS,
								pin_new_failed_bufp, ebufp);
					}
				}
				PIN_FLIST_SUBSTR_SET(i_flistp,t_extended_flistp,PIN_FLD_PRICING_INFO,ebufp);
			}
			PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"final input flist is ", i_flistp);
		}
		
		*r_flistpp = PIN_FLIST_COPY(i_flistp,ebufp);
		PIN_FLIST_DESTROY_EX(&extended_flistp,NULL);
	}
	
Done:
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_process_audit_pol_create_prepare_input_flist output flist is  ", *r_flistpp);
	return;
}

void 
fm_process_audit_pol_create_check_duplicate(
        pcm_context_t   *ctxp,
        uint            flags,
        pin_flist_t     *i_flistp,
        pin_flist_t     *conf_flistp,
        pin_errbuf_t    *ebufp)
{
	pin_flist_t     *ara_search_flistp = NULL;
	pin_flist_t     *ara_result = NULL;
	pin_flist_t     *tflistp = NULL;
	pin_flist_t     *ttflistp = NULL;
	pin_flist_t     *t_tflistp = NULL;
	pin_flist_t     *sub_flistp = NULL;
	pin_flist_t     *t_billing_inf_flistp = NULL;


	poid_t		*s_pdp = NULL;
	poid_t		*poidp = NULL;
	poid_t		*job_pdp = NULL;
	poid_t		*process_audit_poidp = NULL;

	pin_fld_num_t       field1 ;
        pin_fld_num_t       field2 ;

	pin_cookie_t            cookie = NULL;

	int		args = 0;
	int		elem = 0;
	int		count = 0;
	int		type = 0;
        int             *ver_id = NULL;
	
	char		template[512] = "";
	char            template1[256] = "";
	char 		template2[256] = "";
	char		*poid_type = NULL;
	char		*field_name = NULL;
	char		*first_str_tokp = NULL;
	char		*second_str_tokp = NULL;
	char		*job_type = NULL;

	char		buf[128] = "";
	void            *vp = NULL;
	void            *dom_vp = NULL;
        char            *proc_name = NULL;
	int32		*bill_type = NULL;


	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);
	
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"Input flist for fm_process_audit_pol_create_check_duplicate is  ", i_flistp);
	poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp,
                PIN_FLD_POID, 0, ebufp);
	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_process_audit_pol_create error", ebufp);
		return;
        } 

	job_pdp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp,
		PIN_FLD_JOB_OBJ,1, ebufp);

	if(!PIN_POID_IS_NULL(job_pdp)){
		job_type = (char *) PIN_POID_GET_TYPE(job_pdp);
		if (strcmp(job_type, PIN_OBJ_TYPE_JOB_BOC) == 0){
			//skip the duplicate check
			return;
		}
	}

	poid_type = (char *) PIN_POID_GET_TYPE(poidp);

	if(strcmp(poid_type,PRICING_POID_TYPE) == 0){
		//skip the duplicate check
		return;
	}
	ara_search_flistp = PIN_FLIST_CREATE(ebufp);

        s_pdp = (poid_t *)PIN_POID_CREATE(PIN_POID_GET_DB(poidp), "/search", -1, ebufp);
        PIN_FLIST_FLD_PUT(ara_search_flistp, PIN_FLD_POID, s_pdp, ebufp);
        pin_snprintf(template,sizeof(template) ,"select X  from %s where  F1 = V1 and F2 = V2  and F3 = V3 and F4 = V4 ",poid_type);
        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, template);


        /***************************************************************************
         * If the version id is 3 and process_name is pin_bill_accts add DOM check in
         * search template.
         *************************************************************************/

        proc_name = (char *)PIN_FLIST_FLD_GET(i_flistp,PIN_FLD_PROCESS_NAME,1,ebufp);
        ver_id = (int *)PIN_FLIST_FLD_GET(i_flistp,PIN_FLD_VERSION_ID,1,ebufp);

	if (ver_id && *ver_id == 3 && proc_name && !strcmp(proc_name,"pin_bill_accts")) {
               t_billing_inf_flistp = PIN_FLIST_SUBSTR_GET(i_flistp,
							PIN_FLD_BILLING_INFO,1,ebufp);
               if (t_billing_inf_flistp) {
                   dom_vp = PIN_FLIST_FLD_GET(t_billing_inf_flistp,PIN_FLD_ACTG_CYCLE_DOM,1,ebufp);
               }
	}

	/**********************************************************************
     	 * If the version id is 3 and process_name is pin_inv_accts add 
	 * BILL_TYPE check in search template.
     	 ***********************************************************************/

	if (ver_id && *ver_id == 3 && proc_name && 
		!strcmp(proc_name,"pin_inv_accts")) {
		t_billing_inf_flistp = PIN_FLIST_SUBSTR_GET(i_flistp,
					PIN_FLD_BILLING_INFO, 1, ebufp);
		if (t_billing_inf_flistp) {
			vp = PIN_FLIST_FLD_GET(t_billing_inf_flistp, 
				PIN_FLD_BILL_TYPE, 1, ebufp);
			if (vp) {
				bill_type = (int32*)vp;
			}
		}
	}

        args = 1 ;
        tflistp = PIN_FLIST_ELEM_ADD (ara_search_flistp, PIN_FLD_ARGS, args, ebufp);
        PIN_FLIST_FLD_SET (tflistp, PIN_FLD_PROCESS_NAME, PIN_FLIST_FLD_GET(i_flistp,PIN_FLD_PROCESS_NAME,1,ebufp) , ebufp);
        args++;


        tflistp = PIN_FLIST_ELEM_ADD (ara_search_flistp, PIN_FLD_ARGS, args, ebufp);
        PIN_FLIST_FLD_SET (tflistp, PIN_FLD_PROCESS_START_T, PIN_FLIST_FLD_GET(i_flistp,PIN_FLD_PROCESS_START_T,1,ebufp) , ebufp);
        args++;


        tflistp = PIN_FLIST_ELEM_ADD (ara_search_flistp, PIN_FLD_ARGS, args, ebufp);
        PIN_FLIST_FLD_SET (tflistp, PIN_FLD_PROCESS_END_T, PIN_FLIST_FLD_GET(i_flistp,PIN_FLD_PROCESS_END_T,1,ebufp) , ebufp);
        args++;

        tflistp = PIN_FLIST_ELEM_ADD (ara_search_flistp, PIN_FLD_ARGS, args, ebufp);
	process_audit_poidp = PIN_POID_CREATE(PIN_POID_GET_DB(poidp),poid_type,-1,ebufp);
        PIN_FLIST_FLD_PUT (tflistp, PIN_FLD_POID, process_audit_poidp, ebufp);
        args++;

        if (dom_vp) {
	   pin_strlcpy(template2,template,sizeof(template2));
           pin_snprintf(template,sizeof(template),"%sand F5 = V5 ",template2);	
	   tflistp = PIN_FLIST_ELEM_ADD (ara_search_flistp, PIN_FLD_ARGS, args, ebufp);
           t_billing_inf_flistp = PIN_FLIST_SUBSTR_ADD(tflistp,PIN_FLD_BILLING_INFO,ebufp);
           PIN_FLIST_FLD_SET (t_billing_inf_flistp,PIN_FLD_ACTG_CYCLE_DOM,dom_vp,ebufp);
           
        }

	/***************************************************************
	 * The process name will be either pin_bill_accts or pin_inv_accts
	 * So in the search template the field name and value will be F5
	 * and V5 only rather than F6 and V6
	 ***************************************************************/
	if (bill_type) {
		pin_strlcpy(template1, " and F5 = V5 ", sizeof(template1));
		pin_strlcat(template, template1, sizeof(template));
		tflistp = PIN_FLIST_ELEM_ADD (ara_search_flistp, 
			PIN_FLD_ARGS, args, ebufp);
		t_billing_inf_flistp = PIN_FLIST_SUBSTR_ADD(tflistp,
			PIN_FLD_BILLING_INFO, ebufp);
		PIN_FLIST_FLD_SET(t_billing_inf_flistp, PIN_FLD_BILL_TYPE,
			(void *)bill_type, ebufp);
	}

        elem = 0;
        cookie = NULL;
	if ( conf_flistp != NULL) {
        	while ((sub_flistp = PIN_FLIST_ELEM_GET_NEXT(conf_flistp,PIN_FLD_FIELD_NAMES,&elem,
				1,&cookie,ebufp)) != NULL) {

        		field_name = (char *) PIN_FLIST_FLD_GET(sub_flistp,PIN_FLD_FIELD_NAME,1,ebufp);
                        pin_snprintf (buf,sizeof(buf),"fileds found is %s",field_name);
                            PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, buf);
        		if (field_name != NULL) {
                		first_str_tokp = strtok( field_name, "." );
                		second_str_tokp = strtok (NULL,".");
                		if ( second_str_tokp == NULL) {
                        		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                                	"error in specifying the check field ", sub_flistp);
                        		continue;
                		}
                                pin_snprintf (buf,sizeof(buf),"fileds  psrsed are %s and %s",first_str_tokp ,second_str_tokp);
                                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, buf);
                		field1 = pin_field_of_name(first_str_tokp);
                		type = pin_type_of_field( field1);
				switch(type) {

					case PIN_FLDT_SUBSTRUCT: break;
					default:
                                		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                                    		"Unexpected field type");

						PIN_FLIST_DESTROY_EX(&ara_search_flistp,NULL);
						return;
				}
                		field2 = pin_field_of_name(second_str_tokp);
                		type = pin_type_of_field( field2);
				switch( type)
				{
					case PIN_FLDT_INT:
					case PIN_FLDT_INT64:
					case PIN_FLDT_ENUM:
					case PIN_FLDT_STR:
					case PIN_FLDT_POID:
					case PIN_FLDT_TSTAMP:
					case PIN_FLDT_DECIMAL:
						break;
					default:
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, "Unexpected field  found\n");
						return;
				}
	                	ttflistp = PIN_FLIST_SUBSTR_GET(i_flistp, field1, 1,
       		                                 ebufp);
				vp = PIN_FLIST_FLD_GET(ttflistp,field2,1,ebufp);
                		if (vp != NULL ) {
                        		tflistp = PIN_FLIST_ELEM_ADD (ara_search_flistp, PIN_FLD_ARGS, args, ebufp);
                        		t_tflistp = PIN_FLIST_SUBSTR_ADD(tflistp,field1,  ebufp);
                        		PIN_FLIST_FLD_SET (t_tflistp,field2,vp, ebufp);
                        		pin_memset(buf,sizeof(buf),0,sizeof(buf));
                        		pin_snprintf (buf , sizeof(buf), "and F%d = V%d ",args,args);
                                	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, template);
                        		pin_strlcat(template, buf, sizeof(template));
                                	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, template);
                        		args++;
                		}

        		}
        	}
	}
	/* if there are no extended clas fields keep the search poid to base class  */
        flags = SRCH_DISTINCT ;
        PIN_FLIST_FLD_SET(ara_search_flistp, PIN_FLD_FLAGS, (void *)&flags, ebufp);
        PIN_FLIST_ELEM_SET (ara_search_flistp, NULL, PIN_FLD_RESULTS, -1, ebufp );
        PIN_FLIST_FLD_SET(ara_search_flistp,PIN_FLD_TEMPLATE, (void *)template, ebufp);
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        	 " search flist ", ara_search_flistp);

        PCM_OP(ctxp, PCM_OP_SEARCH, 0, ara_search_flistp, &ara_result,
             		 ebufp);
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
	               "result ", ara_result);
       	count = PIN_FLIST_ELEM_COUNT(ara_result, PIN_FLD_RESULTS, ebufp);
        pin_snprintf (buf, sizeof(buf), "duplicate count is %d", count);
        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, buf);
	if (count > 0) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
               		PIN_ERRCLASS_SYSTEM_DETERMINATE,
              		PIN_ERR_DUPLICATE, 0, 0, 0);
              		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                      	"op_process_audit_create duplicate record found",
                      		ebufp);
	}
	PIN_FLIST_DESTROY_EX(&ara_search_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&ara_result, NULL);
	return;
}

