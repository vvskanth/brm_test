/*******************************************************************
 *   
 * Copyright (c) 1999, 2009, Oracle and/or its affiliates. 
 * All rights reserved. 
 *
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

/*******************************************************************
 @(#)$Id: pin_mta.h /cgbubrm_7.5.0.portalbase/1 2015/11/27 06:22:45 nishahan Exp $
 *******************************************************************/

#ifndef _PIN_MTA_H
#define _PIN_MTA_H

#include "pcm.h"
#include "pcp.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
 * This is the shared header file for all muti-thread apps.
 *******************************************************************/
#define MTA_PROG		"pin_mta"

/*******************************************************************
 * pin.conf MTA fields
 *******************************************************************/
#define MTA_PERBATCH    "per_batch"
#define MTA_PERSTEP     "per_step"
#define MTA_FETCHSIZE   "fetch_size"
#define MTA_CHILDREN    "children"
#define MTA_HOTLIST     "hotlist"
#define MTA_MONITOR     "monitor"
#define MTA_MAXERRS     "max_errs"
#define MTA_MULTIDB     "multi_db" /* To turn on/off global search in MTA */
#define MTA_LOGFILE     "logfile"
#define MTA_LOGLEVEL    "loglevel"
#define MTA_MAXTIME		"max_time"
#define MTA_RETRYSRCH	"retry_mta_srch"
#define MTA_RSPWNTHRDS	"respawn_threads"
#define MTA_LOOP_FOREVER	"loop_forever"
#define MTA_SLEEP_INTERVAL	"sleep_interval"
#define MTA_RETURN_WORKER_ERROR "return_worker_error"
#define MTA_ENABLE_ARA	"enable_ara"
/*******************************************************************
 * Legacy MTA flags
 *******************************************************************/
#define MTA_WORKMODE_SINGLE	0
#define MTA_WORKMODE_BATCH	1

/*******************************************************************
 * MTA flags; bitmap flags
 * on application global flist
 * 0 PIN_FLD_APPLIACTION_NFO    SUBSTRUCT [0]
 * 1     PIN_FLD_FLAGS                    INT [0]
 *******************************************************************/
#define MTA_FLAG_TEST_MODE		0x00000001
#define MTA_FLAG_VERBOSE_MODE		0x00000002
#define MTA_FLAG_MULTI_DB		0x00000004
#define MTA_FLAG_BATCH_MODE		0x00000008
#define MTA_FLAG_RESPAWN_THRDS		0x00000010
#define MTA_FLAG_USAGE_MSG		0x00000020
#define MTA_FLAG_HOTLIST		0x00000040
#define MTA_FLAG_VERSION_NEW		0x00000080
#define MTA_FLAG_RETURN_WORKER_ERROR	0x00000100
#define MTA_FLAG_LOOP_FOREVER		0x00000200
#define MTA_FLAG_ENABLE_ARA		0x00000400
#define MTA_FLAG_RESERVED		0x00000FFF
/* reserved for framework layer from 0x00000001 to 0x000000800*/	
/* reserved for application layer from 0x00001000 to 0x00800000*/
/* reserved for customization layer from 0x01000000 to 0x80000000*/

/*******************************************************************
 * Common definitions for defaults and API
 *******************************************************************/
#define MTA_DEFPINLOG	"default.pinlog"
#define MTA_DEFCFGOBJTYPE	"/config/mta"
#define	MTA_NOFLAG		0
#define	MTA_OPTIONAL	1
#define MTA_MANDATORY	0
#define DEFAULT_SLEEP_INTERVAL 60

/******************************************************************
 * BOC Initiated application run status
 ******************************************************************/
#define  PIN_BOC_STATUS_APP_START  2
#define  PIN_BOC_STATUS_APP_SUCCESS  3
#define  PIN_BOC_STATUS_APP_FAIL   4

/*******************************************************************
 * CALL BACK API functions
 *******************************************************************/
PIN_EXPORT void 
pin_mta_config(
		pin_flist_t		*param_flistp,
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_post_config(
		pin_flist_t		*param_flistp,
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_usage(
		char		*prog);

PIN_EXPORT void 
pin_mta_post_usage(
		pin_flist_t		*param_flistp,
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_init_app(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_post_init_app(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_init_search(
		pin_flist_t		*app_flistp,
		pin_flist_t		**search_flistpp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_post_init_search(
		pin_flist_t		*app_flistp,
		pin_flist_t		*search_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_tune(
		pin_flist_t		*app_flistp,
		pin_flist_t		*srch_res_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_post_tune(
		pin_flist_t		*app_flistp,
		pin_flist_t		*srch_res_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_job_done(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_post_job_done(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void
pin_mta_exit(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_post_exit(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_worker_init(
		pcm_context_t	*ctxp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_post_worker_init(
		pcm_context_t	*ctxp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_worker_job(
		pcm_context_t	*ctxp,
		pin_flist_t		*srch_res_flistp,
		pin_flist_t		**op_in_flistpp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_post_worker_job(
		pcm_context_t	*ctxp,
		pin_flist_t		*srch_res_flistp,
		pin_flist_t		*op_in_flistpp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_worker_opcode(
		pcm_context_t	*ctxp,
		pin_flist_t		*srch_res_flistp,
		pin_flist_t		*op_in_flistp,
		pin_flist_t		**op_out_flistpp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_worker_job_done(
		pcm_context_t	*ctxp,
		pin_flist_t		*srch_res_flistp,
		pin_flist_t		*op_in_flistp,
		pin_flist_t		*op_out_flistp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_post_worker_job_done(
		pcm_context_t	*ctxp,
		pin_flist_t		*srch_res_flistp,
		pin_flist_t		*op_in_flistp,
		pin_flist_t		*op_out_flistp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_worker_exit(
		pcm_context_t	*ctxp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void 
pin_mta_post_worker_exit (
		pcm_context_t	*ctxp,
		pin_flist_t		*ti_flistp,
		pin_errbuf_t	*ebufp);

PIN_EXPORT void
pin_mta_record_ra_data(
		pin_flist_t		*app_flistp,
		pin_errbuf_t	*ebufp);

/*******************************************************************
 * Obsolete CALL BACK functions
 *******************************************************************/

PIN_EXPORT void 
pin_mta_init_job(
	int             argc,
	char            *argv[],
	int             *work_modep,
	pin_flist_t     **arg_flistpp,
	pin_errbuf_t    *ebufp);

PIN_EXPORT void 
pin_mta_init_hotlist(
	pin_flist_t     *arg_flistpp,
	pin_flist_t     **hot_flistpp,
	pin_errbuf_t    *ebufp);

PIN_EXPORT void 
pin_mta_exec_work_unit(
	pcm_context_t   *ctxp,
	pin_flist_t     *arg_flistp,
	pin_flist_t     *i_flistp,
	pin_errbuf_t    *ebufp);

PIN_EXPORT void 
pin_mta_cleanup_job(
	pin_flist_t     *arg_flistp,
	pin_errbuf_t    *ebufp);


/*******************************************************************
 * CAF-MTA API functions
 *******************************************************************/

pin_flist_t* 
pin_mta_global_flist_node_get_no_lock(
	pin_fld_num_t   field,
	pin_errbuf_t    *ebufp);


pin_flist_t* 
pin_mta_global_flist_node_take(
	pin_fld_num_t   field,
	pin_errbuf_t    *ebufp);

pin_flist_t* 
pin_mta_global_flist_node_get_with_lock(
	pin_fld_num_t   field,
	pin_errbuf_t    *ebufp);

void 
pin_mta_global_flist_node_release(
	pin_fld_num_t   field,
	pin_errbuf_t    *ebufp);

void
pin_mta_global_flist_node_set(
		pin_flist_t		*in_flistp,
		pin_fld_num_t   field,
		pin_errbuf_t    *ebufp);

void
pin_mta_global_flist_node_put(
		pin_flist_t		*in_flistp,
		pin_fld_num_t   field,
		pin_errbuf_t    *ebufp);

void
pin_mta_reset_total_errcnt();

int 
pin_mta_system_error(
        pin_errbuf_t    *ebufp);


/*******************************************************************
 * To use main thread PCM context on Application layer, if necessary
 * Returned PCM context is not htread safe and has to be used
 * in main thread only.
 *******************************************************************/
pcm_context_t *
pin_mta_main_thread_pcm_context_get (
	pin_errbuf_t    *ebufp);


void
pin_mta_get_int_from_pinconf(
	pin_flist_t		*app_info_flistp,
	char			*pinconf_name,
	int32			field,
	int32			flag,
	int32			optional,
	pin_errbuf_t	*ebufp);

void
pin_mta_get_str_from_pinconf(
		pin_flist_t 	*app_info_flistp,
		char 			*pinconf_name,
		int32			field,
		int32			optional,
		pin_errbuf_t	*ebufp);

void
pin_mta_get_decimal_from_pinconf(
		pin_flist_t 	*app_info_flistp,
		char 			*pinconf_name,
		int32			field,
		int32			optional,
		pin_errbuf_t	*ebufp);

void pin_mta_request_prep_flist(pcm_context_t *ctxp, pin_flist_t *main_op_in_flistp,
					pin_flist_t *request_info_flistp, pin_flist_t **out_flistpp, pin_errbuf_t *ebufp);
bool pin_mta_mask_sensitive_flds(pin_flist_t *flistp, pin_errbuf_t *ebufp);
void pin_mta_request_record(pcm_context_t *ctxp, pin_flist_t *op_in_flistp, pin_flist_t **op_out_flistp, pin_errbuf_t *ebufp);
bool pin_mta_is_enabled_record_failure();

#ifdef __cplusplus
}
#endif

#endif /*_PIN_MTA_H_*/
