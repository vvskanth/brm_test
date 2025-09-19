/*******************************************************************
 *
 *	Copyright (c) 1999-2024 Oracle.
 *
 *	This material is the confidential property of Oracle Corporation
 *	or its licensors and may be used, reproduced, stored or transmitted
 *	only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: pin_mta_test.c /portalbase 2023/04/25 12:00:00 sreejs Exp $";
#endif

#include <stdio.h>

#include "pcm.h"
#include "pin_errs.h"
#include "pinlog.h"

#include "pin_mta.h"
#include "pin_string_wrapper.h"

/*******************************************************************
 * During real application development, there is no necessity to implement 
 * empty MTA call back functions. Application will recognize implemented
 * functions automatically during run-time.
 *******************************************************************/

/*******************************************************************
 * Configuration of application
 * Called prior MTA_CONFIG policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_config(
		pin_flist_t		*param_flistp,
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp)
{
	int32		rec_id = 0;
	pin_cookie_t	cookie = 0;
	pin_cookie_t	prev_cookie = 0;
	pin_flist_t	*flistp = 0;
	char		*option = 0;
	int32		mta_flags = 0;
	void		*vp = 0;
	int32		i = 0;
	int64		n = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_config parameters flist", 
					   param_flistp);
	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_config application info flist", 
					   app_flistp);

	vp = PIN_FLIST_FLD_GET (app_flistp, PIN_FLD_FLAGS, 0,ebufp);
	if(vp)
		mta_flags = *((int32*)vp);

	/***********************************************************
         * The new flag MTA_FLAG_VERSION_NEW has been introduced to
         * differentiate between new mta & old mta applications as
         * only new applications have the ability to drop PIN_FLD_PARAMS
         * for valid parameters . It is only for new apps that
         * checking err_params_cnt is valid, otherwise for old applications
         * this check without a distincion for new applications would hamper
         * normal functioning.
         ***********************************************************/
	mta_flags = mta_flags | MTA_FLAG_VERSION_NEW;
	while((flistp = /*!=NULL*/
		   PIN_FLIST_ELEM_GET_NEXT(param_flistp, PIN_FLD_PARAMS, &rec_id,
								   1, &cookie, ebufp))!= NULL){
		
		option = PIN_FLIST_FLD_GET (flistp, PIN_FLD_PARAM_NAME, 0, ebufp);
		/***************************************************
		 * Test options options.
		 ***************************************************/
		if(option && (strcmp(option, "-batch_mode") == 0)) {	

			mta_flags = mta_flags | MTA_FLAG_BATCH_MODE;	

			PIN_FLIST_ELEM_DROP(param_flistp, PIN_FLD_PARAMS, rec_id, ebufp);
			cookie = prev_cookie;

		} else if(option && (strcmp(option, "-header_number") == 0)) {

			vp = PIN_FLIST_FLD_GET (flistp, PIN_FLD_PARAM_VALUE, 1, ebufp);

			if(vp == 0) {
				mta_flags = mta_flags | MTA_FLAG_USAGE_MSG;
			}else{
				i = pin_strtoi ((char*)vp, PIN_DECIMAL_BASE, PIN_OPTIONAL_EBUF, "pin_mta_test.c-pin_mta_config-1", ebufp);
				n = pin_atoi64 ((char*)vp);
				PIN_FLIST_FLD_SET (app_flistp, PIN_FLD_HEADER_NUM, &n, ebufp);
			}

			PIN_FLIST_ELEM_DROP(param_flistp, PIN_FLD_PARAMS, rec_id, ebufp);
			cookie = prev_cookie;

		}else if(option && (strcmp(option, "-increment") == 0)) {

			vp = PIN_FLIST_FLD_GET (flistp, PIN_FLD_PARAM_VALUE, 1, ebufp);

			if(vp) {
				i = pin_strtoi ((char*)vp, PIN_DECIMAL_BASE, PIN_OPTIONAL_EBUF, "pin_mta_test.c-pin_mta_config-2", ebufp);
				PIN_FLIST_FLD_SET (app_flistp, PIN_FLD_INCR_AMOUNT, &i, ebufp);
			}

			PIN_FLIST_ELEM_DROP(param_flistp, PIN_FLD_PARAMS, rec_id, ebufp);
			cookie = prev_cookie; 
		}else if(option && (strcmp(option, "-error") == 0)) {

			vp = PIN_FLIST_FLD_GET (flistp, PIN_FLD_PARAM_VALUE, 1, ebufp);

			if(vp == 0) {
				mta_flags = mta_flags | MTA_FLAG_USAGE_MSG;
			}else{
				i = pin_strtoi ((char*)vp, PIN_DECIMAL_BASE, PIN_OPTIONAL_EBUF, "pin_mta_test.c-pin_mta_config-3", ebufp);
				PIN_FLIST_FLD_SET (app_flistp, PIN_FLD_OPS_ERROR, &i, ebufp);
			}

			PIN_FLIST_ELEM_DROP(param_flistp, PIN_FLD_PARAMS, rec_id, ebufp);
			cookie = prev_cookie;

		} else if(option && (strcmp(option, "-opt2") == 0)) {

			PIN_FLIST_FLD_SET (app_flistp, PIN_FLD_ACTION_NAME, option, ebufp);
			
			vp = PIN_FLIST_FLD_GET (flistp, PIN_FLD_PARAM_VALUE, 1, ebufp);

			if((vp == 0) || (strcmp ("param2.1 param2.2", vp) != 0)) {
				mta_flags = mta_flags | MTA_FLAG_USAGE_MSG;
			}else{
				PIN_FLIST_FLD_SET (app_flistp, PIN_FLD_FILTER_STRING, vp, ebufp);
			}

			PIN_FLIST_ELEM_DROP(param_flistp, PIN_FLD_PARAMS, rec_id, ebufp);
			cookie = prev_cookie;

		} else if(option && (strcmp(option, "-opt3") == 0)) {

			vp = PIN_FLIST_FLD_GET (flistp, PIN_FLD_PARAM_VALUE, 1, ebufp);

			if((vp == 0) || (strcmp ("param3.1", vp) != 0)) {
				mta_flags = mta_flags | MTA_FLAG_USAGE_MSG;
			}else{
				PIN_FLIST_FLD_SET (app_flistp, PIN_FLD_CRITERION, vp, ebufp);
			}
		
			PIN_FLIST_ELEM_DROP(param_flistp, PIN_FLD_PARAMS, rec_id, ebufp);
			cookie = prev_cookie;
		} else {
			prev_cookie = cookie;
		}
	}

	PIN_FLIST_FLD_SET (app_flistp, PIN_FLD_FLAGS, &mta_flags, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_config error", ebufp);
	}
	return;
}

/*******************************************************************
 * Post configuration of application
 * Called after MTA_CONFIG policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_config(
		pin_flist_t		*param_flistp,
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_config parameters flist", 
					   param_flistp);
	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_config application info flist", 
					   app_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_config error", ebufp);
	}
	return;
}

/*******************************************************************
 * Usage information for the specific app.
 * Called prior MTA_USAGE policy opcode
 *******************************************************************/
PIN_EXPORT void
pin_mta_usage(
		char	*prog)
{

	pin_errbuf_t	ebuf;
	pin_flist_t	*ext_flistp = NULL;
	char		*usage_str = NULL;

	char		*format = "\nUsage:\t %s [-verbose] [-test] \n"
			"\t\t -header_number <number> \n"
			"\t\t -increment <number> ; default 1\n"
			"\t\t -batch_mode ; default single mode\n"
			"\t\t -opt2 param2.1 param2.2\n"
			"\t\t -opt3 param3.1\n";

	PIN_ERRBUF_CLEAR (&ebuf);
	size_t usage_str_len = strlen(format) + strlen(prog) + 1;	
	usage_str = (char*)pin_malloc(usage_str_len);

	if (usage_str == NULL) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,"No Memory error");
		return;
	}

	pin_snprintf(usage_str, usage_str_len, format ,prog);

	ext_flistp =
		pin_mta_global_flist_node_get_no_lock (PIN_FLD_EXTENDED_INFO,
											   &ebuf);

	PIN_FLIST_FLD_SET (ext_flistp, PIN_FLD_DESCR, usage_str, &ebuf)

	if (PIN_ERR_IS_ERR(&ebuf)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_usage error", &ebuf);
	}
	PIN_FREE_EX(&usage_str);
	return;
}

/*******************************************************************
 * Usage information for the specific app.
 * Called after MTA_USAGE policy opcode
 * Information passed from customization layer
 * can be processed and displayed here
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_usage(
		pin_flist_t		*param_flistp,
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp)
{
	pin_flist_t	*ext_flistp = NULL;
	void		*vp = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_usage parameters flist", 
					   param_flistp);
	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_usage application info flist", 
					   app_flistp);

	ext_flistp =
		pin_mta_global_flist_node_get_no_lock (PIN_FLD_EXTENDED_INFO,
											   ebufp);

	vp = PIN_FLIST_FLD_GET (ext_flistp, PIN_FLD_DESCR, 1, ebufp);

	if(vp) {
		printf("%s",(char*) vp);
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_usage error", ebufp);
	}
	return;
}

/*******************************************************************
 * Initialization of application
 * Called prior MTA_INIT_APP policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_init_app(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_init_app application info flist", 
					   app_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_init_app error", ebufp);
	}
	return;
}

/*******************************************************************
 * Post initialization of application
 * Called after MTA_INIT_APP policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_init_app(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_init_app application info flist", 
					   app_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_init_app error", ebufp);
	}
	return;
}

/*******************************************************************
 * Application defined search criteria.
 * Called prior MTA_INIT_SEARCH policy opcode
 *******************************************************************/
PIN_EXPORT void
pin_mta_init_search(
		pin_flist_t		*app_flistp,
		pin_flist_t		**s_flistpp,
		pin_errbuf_t	*ebufp)
{

	pin_flist_t	*s_flistp = NULL;
	char		template[64] = {"select X from /data where F1 = V1 "};
	int32		s_flags = 0;
	poid_t		*s_pdp = NULL;
	int64		id = -1;
	int64		db = 0;
	void		*vp = NULL;
	pin_flist_t	*tmp_flistp = NULL;
	int64		i = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	*s_flistpp = 0;

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_init_search application info flist", 
					   app_flistp);

	s_flistp = PIN_FLIST_CREATE(ebufp);

	/***********************************************************
	 * Simple search flist
	 ***********************************************************
	 * 0 PIN_FLD_POID           POID [0] 0.0.0.1 /search -1 0
	 * 0 PIN_FLD_FLAGS           INT [0] 0
	 * 0 PIN_FLD_TEMPLATE        STR [0] "select X from /data where F1 = V1 "
	 * 0 PIN_FLD_ARGS          ARRAY [1] allocated 20, used 1
	 * 1     PIN_FLD_HEADER_NUM      INT64 [0] 0
	 * 0 PIN_FLD_RESULTS       ARRAY [0] allocated 20, used 2
	 * 1     PIN_FLD_POID           POID [0] 
	***********************************************************/

	vp = PIN_FLIST_FLD_GET (app_flistp, PIN_FLD_POID_VAL, 0, ebufp);
	if(vp)
		db = PIN_POID_GET_DB ((poid_t*)vp);

	s_pdp = PIN_POID_CREATE(db, "/search/pin", id, ebufp);
	PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, 
					  s_pdp, ebufp);

	PIN_FLIST_FLD_SET (s_flistp, PIN_FLD_FLAGS, &s_flags, ebufp);

	PIN_FLIST_FLD_SET (s_flistp, PIN_FLD_TEMPLATE, template, ebufp);

	/***********************************************************
	 * Search arguments
	 ***********************************************************/
	tmp_flistp = PIN_FLIST_ELEM_ADD (s_flistp, PIN_FLD_ARGS,
									 1, ebufp);

	vp = PIN_FLIST_FLD_GET (app_flistp, PIN_FLD_HEADER_NUM,
							0, ebufp);
	if(vp)
		i = *((int64*)vp);
	PIN_FLIST_FLD_SET (tmp_flistp, PIN_FLD_HEADER_NUM, &i, ebufp);

	/***********************************************************
	 * Search results
	 ***********************************************************/
	tmp_flistp = PIN_FLIST_ELEM_ADD (s_flistp, PIN_FLD_RESULTS,
									 0, ebufp);
	vp = 0;
	PIN_FLIST_FLD_SET (tmp_flistp, PIN_FLD_POID, vp, ebufp);


	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_init_search error", ebufp);

		PIN_FLIST_DESTROY_EX (&s_flistp,0);
	}else{
		*s_flistpp = s_flistp;

		PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_init_search search flist", 
						   s_flistp);
	}

	return;
}

/*******************************************************************
 * Application defined search criteria.
 * Called after MTA_INIT_SEARCH policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_init_search(
		pin_flist_t		*app_flistp,
		pin_flist_t		*search_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_init_search application info flist", 
					   app_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_init_search search flist", 
					   search_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_init_search error", ebufp);
	}
	return;
}

/*******************************************************************
 * Search results may be updated, modified or enriched
 * Called prior MTA_TUNE policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_tune(
		pin_flist_t		*app_flistp,
		pin_flist_t		*srch_res_flistp,
		pin_errbuf_t	*ebufp)
{
	pin_flist_t	*multi_res_flistp = NULL;
	pin_flist_t	*tmp_flistp = NULL;
	int32		i = 0;
	int32		mta_flags = 0;
	void		*vp = 0;
	pin_cookie_t	s_cookie = 0;
	int32		s_rec_id = 0;
	pin_cookie_t	m_cookie = 0;
	int32		m_rec_id = 0;
	int64		db = 0;
	int64		id = 0;
	pin_const_poid_type_t		type = 0;
	poid_t		*pdp = 0;
	poid_t		*n_pdp = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_tune application info flist", 
					   app_flistp);
	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_tune search results flist", 
					   srch_res_flistp);

	vp = PIN_FLIST_FLD_GET (app_flistp, PIN_FLD_OPS_ERROR, MTA_OPTIONAL,ebufp);
	if(vp)
		i = *((int32*)vp);

	if (i) {

		while((multi_res_flistp = 
			   PIN_FLIST_ELEM_GET_NEXT(srch_res_flistp, PIN_FLD_MULTI_RESULTS,
									   &s_rec_id, 1, &s_cookie, ebufp)) != NULL) {

			m_cookie = 0;
			while((tmp_flistp = 
				   PIN_FLIST_ELEM_GET_NEXT(multi_res_flistp, PIN_FLD_RESULTS,
										   &m_rec_id, 1, &m_cookie, ebufp)) != NULL) {

				vp = PIN_FLIST_FLD_TAKE (tmp_flistp, PIN_FLD_POID, 0, ebufp);

				if(vp) {
					pdp = (poid_t*)vp;
					db = PIN_POID_GET_DB (pdp);
					id = PIN_POID_GET_ID (pdp);
					type = PIN_POID_GET_TYPE (pdp);
					id = id * i;
					n_pdp = PIN_POID_CREATE (db, type,id, ebufp);
					PIN_POID_DESTROY_EX(&pdp, ebufp);

					PIN_FLIST_FLD_PUT (tmp_flistp, PIN_FLD_POID, n_pdp, ebufp);
				}
			}
		}
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_tune error", ebufp);
	}
	return;
}

/*******************************************************************
 * Search results may be updated, modified or enriched
 * Called after MTA_TUNE policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_tune(
		pin_flist_t		*app_flistp,
		pin_flist_t		*srch_res_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_tune application info flist", 
					   app_flistp);
	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_tune search results flist", 
					   srch_res_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_tune error", ebufp);
	}
	return;
}

/*******************************************************************
 * Function executed after seach results were processed
 * Called prior MTA_JOB_DONE policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_job_done(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_job_done application info flist", 
					   app_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_job_done error", ebufp);
	}
	return;
}

/*******************************************************************
 * Function executed after seach results were processed
 * Called after MTA_JOB_DONE policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_job_done(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_job_done application info flist", 
					   app_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_job_done error", ebufp);
	}
	return;
}

/*******************************************************************
 * Function executed at application exit
 * Called prior MTA_EXIT policy opcode
 *******************************************************************/
PIN_EXPORT void
pin_mta_exit(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_exit application info flist", 
					   app_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_exit error", ebufp);
	}
	return;
}

/*******************************************************************
 * Function executed at application exit
 * Called after MTA_EXIT policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_exit(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_exit application info flist", 
					   app_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_exit error", ebufp);
	}
	return;
}

/*******************************************************************
 * Initialization of worker thread
 * Called prior MTA_WORKER_INIT policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_worker_init(
		pcm_context_t	*ctxp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_init thread info flist", 
					   ti_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_worker_init error", ebufp);
	}
	return;
}

/*******************************************************************
 * Post initialization of worker thread
 * Called after MTA_WORKER_INIT policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_worker_init(
		pcm_context_t	*ctxp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_worker_init thread info flist", 
					   ti_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_worker_init error", ebufp);
	}
	return;
}

/*******************************************************************
 * Function called when new job is avaialble to worker
 * Called prior MTA_WORKER_JOB policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_worker_job(
		pcm_context_t	*ctxp,
		pin_flist_t		*srch_res_flistp,
		pin_flist_t		**op_in_flistpp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp)
{

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_job search results flist", 
					   srch_res_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_job thread info flist", 
					   ti_flistp);


	*op_in_flistpp = PIN_FLIST_COPY (srch_res_flistp, ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_job prepared flist for main opcode", 
					   *op_in_flistpp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_worker_job error", ebufp);
	}
	return;
}


/*******************************************************************
 * Function called when new job is avaialble to worker
 * Called after MTA_WORKER_JOB policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_worker_job(
		pcm_context_t	*ctxp,
		pin_flist_t		*srch_res_flistp,
		pin_flist_t		*op_in_flistp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_worker_job search results flist", 
					   srch_res_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_worker_job thread info flist", 
					   ti_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_worker_job prepared flist for main opcode", 
					   op_in_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_worker_job error", ebufp);
	}
	return;
}

/*******************************************************************
 * Main application opcode is called here
 *******************************************************************/
PIN_EXPORT void 
pin_mta_worker_opcode(
		pcm_context_t	*ctxp,
		pin_flist_t		*srch_res_flistp,
		pin_flist_t		*op_in_flistp,
		pin_flist_t		**op_out_flistpp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp)
{
	pin_flist_t	*tmp_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	int32		i = 1;
	int64 		n = 1;
	int32		mta_flags = 0;
	void		*vp = 0;
	pin_cookie_t	cookie = 0;
	int32		rec_id = 0;
	pin_flist_t	*app_flistp = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_opcode search results flist", 
					   srch_res_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_opcode prepared flist for main opcode", 
					   op_in_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_opcode thread info flist", 
					   ti_flistp);

	app_flistp =
		pin_mta_global_flist_node_get_with_lock (PIN_FLD_APPLICATION_INFO,
												 ebufp);

	vp = PIN_FLIST_FLD_GET (app_flistp, PIN_FLD_FLAGS, 0,ebufp);
	if(vp)
		mta_flags = *((int32*)vp);

	/*should/may be moved to worker_job or tune*/
	vp = PIN_FLIST_FLD_GET (app_flistp, PIN_FLD_INCR_AMOUNT, 1,ebufp);
	if(vp)
		i = *((int32*)vp);
	n = i;
	pin_mta_global_flist_node_release (PIN_FLD_APPLICATION_INFO,
									   ebufp);


	if(mta_flags & MTA_FLAG_BATCH_MODE) {

		*op_out_flistpp = PIN_FLIST_CREATE (ebufp);

		while((tmp_flistp = 
			   PIN_FLIST_ELEM_GET_NEXT(op_in_flistp, PIN_FLD_RESULTS,
									   &rec_id, 1, &cookie, ebufp)) != NULL) {

			PIN_FLIST_FLD_SET (tmp_flistp, PIN_FLD_HEADER_NUM, &n, ebufp);
			PCM_OP (ctxp, PCM_OP_INC_FLDS, 0, tmp_flistp, &r_flistp, ebufp);

			PIN_FLIST_ELEM_PUT (*op_out_flistpp, r_flistp, PIN_FLD_RESULTS,
								rec_id, ebufp);

		}
	} else {
		/*should/may be moved to worker_job or tune*/
		tmp_flistp = PIN_FLIST_COPY(op_in_flistp, ebufp);

		PIN_FLIST_FLD_SET (tmp_flistp, PIN_FLD_HEADER_NUM, &n, ebufp);

		PCM_OP (ctxp, PCM_OP_INC_FLDS, 0, tmp_flistp, op_out_flistpp, ebufp);

		PIN_FLIST_DESTROY_EX (&tmp_flistp, 0);
	}


	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_worker_opcode error", ebufp);

		PIN_FLIST_DESTROY_EX (op_out_flistpp, 0);
	}else{
		PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, 
						   "pin_mta_worker_opcode output flist from main opcode", 
						   *op_out_flistpp);
	}

	return;
}

/*******************************************************************
 * Function called when worker completed assigned job
 * Called prior MTA_WORKER_JOB_DONE policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_worker_job_done(
		pcm_context_t	*ctxp,
		pin_flist_t		*srch_res_flistp,
		pin_flist_t		*op_in_flistp,
		pin_flist_t		*op_out_flistp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_job_done search results flist", 
					   srch_res_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_job_done prepared flist for main opcode", 
					   op_in_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_job_done output flist from main opcode", 
					   op_out_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_job_done thread info flist", 
					   ti_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_worker_job_done error", ebufp);
	}
	return;
}

/*******************************************************************
 * Function called when worker completed assigned job
 * Called after MTA_WORKER_JOB_DONE policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_worker_job_done(
		pcm_context_t	*ctxp,
		pin_flist_t		*srch_res_flistp,
		pin_flist_t		*op_in_flistp,
		pin_flist_t		*op_out_flistp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_worker_job_done search results flist", 
					   srch_res_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_worker_job_done prepared flist for main opcode", 
					   op_in_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_worker_job_done output flist from main opcode", 
					   op_out_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_worker_job_done thread info flist", 
					   ti_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_worker_job_done error", ebufp);
	}
	return;
}

/*******************************************************************
 * Worker thread exit function
 * Called prior MTA_WORKER_EXIT policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_worker_exit(
		pcm_context_t	*ctxp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_worker_exit thread info flist", 
					   ti_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_worker_exit error", ebufp);
	}
	return;
}

/*******************************************************************
 * Worker thread exit function
 * Called after MTA_WORKER_EXIT policy opcode
 *******************************************************************/
PIN_EXPORT void 
pin_mta_post_worker_exit (
		pcm_context_t	*ctxp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_post_worker_exit thread info flist", 
					   ti_flistp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_post_worker_exit error", ebufp);
	}
	return;
}


/*******************************************************************
 * Obsolete callback API , supported for backward compatibility
 * There are several drawbacks in keeping using obsolete function,
 * one of them obsolete functions do not match MTA policy opcodes ...
 *******************************************************************/
/*******************************************************************
 * User-defined initialization.
 * Add user specified arguments or parameters to arg_flistp.
 * Should not be used , since there is better approach with pin_mta_config
 * and pin_mta_post_config, pin_mta_init_app and pin_mta_post_init_app.
 *******************************************************************/
PIN_EXPORT void
pin_mta_init_job(
		int				argc,
		char			*argv[],
		int				*work_modep,
		pin_flist_t		**arg_flistpp,
		pin_errbuf_t	*ebufp)
{
	pin_flist_t	*arg_flistp = NULL;
	int32 i = argc;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	*arg_flistpp = 0;

	*work_modep = MTA_WORKMODE_SINGLE;

	arg_flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET (arg_flistp, PIN_FLD_ATTRIBUTE, &i , ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_init_job error", ebufp);

		PIN_FLIST_DESTROY_EX (&arg_flistp,0);
	}else {
		*arg_flistpp = arg_flistp;

		PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_init_job arguments flist", 
						   arg_flistp);
	}

	return;
}

/*******************************************************************
 * User-defined hotlist initialization.
 * Should not be used , since there is better approach with search flist
 * option reading from a file.
 *******************************************************************/
PIN_EXPORT void
pin_mta_init_hotlist(
		pin_flist_t		*app_flistp,
		pin_flist_t		**hot_flistpp,
		pin_errbuf_t	*ebufp )
{

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_init_hotlist application info flist", 
					   app_flistp);

	*hot_flistpp = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_init_hotlist error", ebufp);
	}
	return;
}

/*******************************************************************
 * Do the real job for each work unit.
 * Should not be used, better approach pin_mta_worker_opcode 
 * and rest of pin_mta_worker_* functions
 *******************************************************************/	
PIN_EXPORT void
pin_mta_exec_work_unit(
		pcm_context_t	*ctxp,
		pin_flist_t		*app_flistp,
		pin_flist_t		*i_flistp,
		pin_errbuf_t	*ebufp)
{
	pin_flist_t	*tmp_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	int32		i = 1;
	int64 		n = 1;
	int32		mta_flags = 0;
	void		*vp = 0;
	pin_cookie_t	cookie = 0;
	int32		rec_id = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_exec_work_unit application info flist", 
					   app_flistp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_exec_work_unit input flist", 
					   i_flistp);

	vp = PIN_FLIST_FLD_GET (app_flistp, PIN_FLD_FLAGS, 0,ebufp);
	if(vp)
		mta_flags = *((int32*)vp);

	vp = PIN_FLIST_FLD_GET (app_flistp, PIN_FLD_INCR_AMOUNT, 1,ebufp);
	if(vp)
		i = *((int32*)vp);
	n = i;

	if(mta_flags & MTA_FLAG_BATCH_MODE) {

		while((tmp_flistp = 
			   PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_RESULTS,
									   &rec_id, 1, &cookie, ebufp)) != NULL) {

			PIN_FLIST_FLD_SET (tmp_flistp, PIN_FLD_HEADER_NUM, &n, ebufp);
			PCM_OP (ctxp, PCM_OP_INC_FLDS, 0, tmp_flistp, &r_flistp, ebufp);


			PIN_FLIST_DESTROY_EX (&r_flistp, ebufp);
		}


	} else {
		tmp_flistp = PIN_FLIST_COPY(i_flistp, ebufp);

		PIN_FLIST_FLD_SET (tmp_flistp, PIN_FLD_HEADER_NUM, &n, ebufp);

		PCM_OP (ctxp, PCM_OP_INC_FLDS, 0, tmp_flistp, &r_flistp, ebufp);

		PIN_FLIST_DESTROY_EX (&tmp_flistp, 0);
		PIN_FLIST_DESTROY_EX (&r_flistp, ebufp);
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_exec_work_unit error", ebufp);
	}
	return;
}

/*******************************************************************
 * Cleanup. 
 * Should not be used, better approach is to use pin_mta_exit and 
 * pin_mta_post_exit; in such cases, no need to destroy app_flist :-)
 * doing this to test backward compatibility, just in case ...
 * and destroying passed copy of app_flistp ...
 *******************************************************************/
PIN_EXPORT void
pin_mta_cleanup_job(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp)
{

	pin_flist_t	*app_flistp_x = app_flistp;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERRBUF_CLEAR (ebufp);

	PIN_ERR_LOG_FLIST (PIN_ERR_LEVEL_DEBUG, "pin_mta_clean_up application info flist", 
					   app_flistp);

	PIN_FLIST_DESTROY_EX (&app_flistp_x, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
						 "pin_mta_cleanup_job error", ebufp);
	}
	return;
}
