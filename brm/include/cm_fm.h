/*
 *      @(#)$Id: cm_fm.h /cgbubrm_7.5.0.portalbase/2 2016/02/11 02:16:02 thakuppu Exp $
 *    
* Copyright (c) 1996, 2016, Oracle and/or its affiliates. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _CM_FM_H
#define _CM_FM_H

#ifdef WINDOWS
  #include <windows.h>
#endif

#ifndef    _pin_stats_h_
  #include "pin_stats.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * pseudo-function prototype for FM functions.
 * fm_xxx_op_yyy(cm_nap_connection_t *connp, int32 opcode, int32 opflags,
 *	pin_flist_t *in_flistp,
 *	pin_flist_t **out_flistpp, pin_errbuf_t *ebufp);
 *
 */

/* 
 * The CM credentials are opaque to fm's
 */
typedef void cm_credentials_t;

/*
 * each time an FM makes a pcm_op() call, one of these is put on the list.
 * (also done on initial FM opcode).  They go on FIFO, so the current op
 * is pointed to by cm_nap_connection->coip;
 */
typedef struct cm_op_info {
	int32                   opcode;
	int32                   opflags;
	hrtime_t                recv_time;
	pin_flist_t*            in_flistp;
	pin_flist_t**           out_flistpp;
	pin_errbuf_t*           ebufp;
	struct cm_op_info*      next;
	pcm_context_t*          old_dm_ctx;
	void*                   dispatch;
	pin_opcode_qos_t*       opcode_qos_p;
} cm_op_info_t;

/* sort of a dummy - with pcm_context_t vs. pcp_context_t */
typedef struct public_cm_nap_connection {
	struct cm_op*           request;
	struct cm_op*           response;
	pcm_context_t*          nap_ctx;
	pcm_context_t*          dm_ctx;
	cm_op_info_t*           coip;
	pcm_context_t*          fwd_ctx;
	cm_credentials_t*       credp;
	/* XXX public stuff ends here */
} cm_nap_connection_t;

/* new, documented name for above */
typedef cm_nap_connection_t pin_fm_conn_t;

/*
 * the config table for FMs
 * Only initialize opcode and func_name, since the FMs are
 * dynamically loaded.
 */
struct cm_fm_config {
	int32                   opcode;
	char*                   func_name;
	int32                   overridable;        /* >0 indicates an op is*/
	                                            /* overridable */
	/* below is filled in by the config system */
	void                    (*func)();          /* invoke this to process */
	                                            /* opcode */
	void*                   prev;
	pin_opcode_qos_t*       qos_p;              /* non-null indicates we */
	                                            /* are collecting QOS stats */
	/*
	 * On Solaris, it seems that "twalk" and "tfind" both use a single
	 * non-recursive lock.  This means that in a function that
	 * is being called from "twalk", it can't call any function that
	 * will call "tfind" (or probably any of the other "t" functions).
	 * If it does, it will hang forever.
	 * "check_qos_stats" is called from "twalk" and that function
	 * wants to display the opcode name so it would like to call 
	 * "pr_opcode".  But "pr_opcode" calls "tfind".
	 * To solve this, I will store a pointer to the opcode name in this
	 * structure.
	 */
	const char*             opcode_name;
};

/* Define to use in FM config tables to specify an opcode is 
 * overridable
 */
#define CM_FM_OP_OVERRIDABLE            1

/* To sync brm balances with ece balances */
#define PIN_BAL_SYNC_ECE_BALANCES	0x400

/************************************************************************
 * Functions available to the FM
 ************************************************************************/

#ifndef WINDOWS
  #define PIN_EXTERN extern
  #define EXPORT_OP
#else
  #define EXPORT_OP __declspec(dllexport)
  #ifdef __CMPIN__
    #define PIN_EXTERN __declspec(dllexport)
  #else 
    #define PIN_EXTERN __declspec(dllimport)
  #endif
#endif

PIN_EXTERN poid_t* cm_fm_userid(
	pin_errbuf_t*           ebufp);

PIN_EXTERN cm_nap_connection_t* cm_fm_get_connection();

PIN_EXTERN poid_t* cm_fm_effective_userid(
	pin_errbuf_t*           ebufp);

PIN_EXTERN poid_t* cm_fm_base_session(
	pin_errbuf_t*           ebufp);

PIN_EXTERN void cm_fm_set_trans_flist(
	pcm_context_t*          ctxp,
	pin_flist_t*            flistp);

PIN_EXTERN void cm_fm_set_named_trans_flist(
	pcm_context_t*          ctxp,
	pin_flist_t*            flistp,
	char*                   namep);

PIN_EXTERN pin_flist_t* cm_fm_get_trans_flist(
	pcm_context_t*          ctxp);

PIN_EXTERN pin_flist_t* cm_fm_get_named_trans_flist(
	pcm_context_t*          ctxp,
	char*                   namep);

PIN_EXTERN void cm_fm_drop_named_trans_flist(
	pcm_context_t*          ctxp,
	char*                   namep,
	pin_errbuf_t*           ebufp);

PIN_EXTERN char* cm_fm_perm_get_lineage(
	pin_errbuf_t*           ebufp);

PIN_EXTERN poid_t* cm_fm_perm_get_brand_account(
	pin_errbuf_t*           ebufp);

PIN_EXTERN poid_t* cm_fm_perm_get_billing_group(
	pin_errbuf_t*           ebufp);

PIN_EXTERN int32 cm_fm_perm_is_brand_level(
	pin_errbuf_t*           ebufp);

PIN_EXTERN poid_t* cm_fm_perm_get_account(
	pin_errbuf_t*           ebufp);

PIN_EXTERN int32 cm_fm_perm_override_scope(
	int32                   scope);

PIN_EXTERN void cm_fm_get_credentials(
	pin_flist_t*            in_flistp,
	pin_flist_t*            flistp,
	pin_errbuf_t*           ebufp);

PIN_EXTERN void cm_fm_set_credentials(
	poid_t*                 aclp,
	poid_t*                 billp,
	pin_errbuf_t*           ebufp);

PIN_EXTERN void cm_fm_push_permission_stack(
	poid_t*                 service_poidp,
	int                     effective_credential,
	pin_errbuf_t*           ebufp);

PIN_EXTERN void cm_fm_pop_permission_stack(
	pin_errbuf_t*           ebufp);

PIN_EXTERN void cm_fm_setup_global_search_credentials(
	pin_errbuf_t*           ebufp);

PIN_EXTERN int32 cm_fm_is_scoping_enabled(void);

PIN_EXTERN int cm_fm_is_multi_db(void);

PIN_EXTERN void cm_fm_test_print_logical_partitions(void);

PIN_EXTERN int cm_fm_is_logical_partition(void);

PIN_EXTERN int cm_fm_is_timesten(void);

PIN_EXTERN int cm_fm_is_single_row_event(void);

PIN_EXTERN void cm_fm_get_logical_partitions(
	int64                   db_no,
	int64**                 partitions,
	int*                    partition_cntp,
	pin_errbuf_t*           ebufp);

PIN_EXTERN int64 cm_fm_get_acct_location(
        pcm_context_t*          ctxp,
        poid_t*                 acct_poidp,
        pin_errbuf_t*           ebufp);

PIN_EXTERN int cm_fm_is_satellite(void);

PIN_EXTERN int32 cm_fm_dm_has_attribute(
	int64                   db,
	char*                   tokenp);

PIN_EXTERN int64 cm_fm_get_current_db_no(
	pcm_context_t*          pcm_ctxp);

PIN_EXTERN void cm_fm_get_unique_schema_nos(
	int64**                    unique_schemas,
	int*                    unique_schema_no,
	pin_errbuf_t*           ebufp);

PIN_EXTERN void cm_fm_get_db_nos(
	const char*             attr,
	int64**                 db_nopp,
	int*                    db_no_cntp,
	pin_errbuf_t*           ebufp);

PIN_EXTERN void cm_fm_get_primary_db_no(
	int64*                  primary_db_nop,
	pin_errbuf_t*           ebufp);

PIN_EXTERN int cm_fm_is_primary_uniqueness(void);

PIN_EXTERN int64 cm_fm_get_startup_db_no(void);

PIN_EXTERN int32 cm_fm_has_directory_server(void);

PIN_EXTERN pin_db_no_t cm_fm_get_directory_server_db_no(void);

/*
 * Function to check the pin.conf values to see if the
 * PIN_ERR_PERF_LIMIT_REACHED hint should be ignored or treated as an error.
 *
 * This returns a 1 if the TRANS_OPEN should be treated as an error.  Else
 * it returns 0 if the hint should be ignored.
 */
PIN_EXTERN int cm_reject_on_overload(void);

/*
 * cm_utils_trans.c
 */

PIN_EXPORT int fm_utils_trans_open(
	pcm_context_t*          ctxp,
	int32                   i_flags,
	poid_t*                 acct_pdp,
	pin_errbuf_t*           ebufp);

PIN_EXPORT void fm_utils_trans_close(
	pcm_context_t*          ctxp,
	int                     local,
	pin_errbuf_t*           ebufp);

PIN_EXPORT void fm_utils_trans_abort(
	pcm_context_t*          ctxp,
	pin_errbuf_t*           ebufp);

PIN_EXPORT void fm_utils_trans_commit(
	pcm_context_t*          ctxp,
	pin_errbuf_t*           ebufp);

/*
 * cm_utils_opstack.c
 */
PIN_EXPORT u_int fm_utils_op_is_ancestor(
	cm_op_info_t*           coip,
	u_int                   opcode);

PIN_EXPORT u_int
fm_utils_op_is_parent(
	cm_op_info_t*           coip,
	u_int                   opcode);

PIN_EXPORT u_int fm_utils_op_is_grandparent(
	cm_op_info_t*           coip,
	u_int                   opcode);

PIN_EXPORT u_int fm_utils_op_is_entry(
	cm_op_info_t*           coip,
	u_int                   opcode);

/*
 * cm_utils_type.c
 */
PIN_EXPORT int fm_utils_type_equal(
	poid_t*                 a_pdp,
	char*                   s_type);

PIN_EXPORT int fm_utils_is_subtype(
	poid_t*                 a_pdp,
	char*                   s_type);

/*
 * cm_utils_dd_cache.c
 */
PIN_EXPORT pin_flist_t* fm_utils_dd_get_partial_object_spec(
	pcm_context_t*          ctxp,
	poid_t*                 poidp,
	pin_errbuf_t*           ebufp);

PIN_EXPORT pin_flist_t* fm_utils_dd_get_complete_object_spec(
	pcm_context_t*          ctxp,
	poid_t*                 poidp,
	pin_errbuf_t*           ebufp);

/*
 * cm_utils.c
 */
PIN_EXTERN void cm_fm_op_prev_impl(
	cm_nap_connection_t*    connp,
	int32                   flags,
	pin_flist_t*            in_flistp,
	pin_flist_t**           out_flistpp,
	int32                   byref,
	pin_errbuf_t*           ebufp);

/*
 * cm_utils_flist.c
 */
PIN_EXPORT void fm_utils_merge_array(
	pin_flist_t*            d_flistp,
	pin_flist_t*            s_flistp,
	u_int                   field_no,
	pin_errbuf_t*           ebufp);


PIN_EXPORT void fm_utils_copy_array(
	pin_flist_t*            d_flistp,
	u_int                   s_field_no,
	pin_flist_t*            s_flistp,
	u_int                   d_field_no,
	pin_errbuf_t*           ebufp);

#ifdef WINDOWS
  PIN_EXTERN HANDLE fm_mutex;
  PIN_EXTERN HANDLE master_heap_hdl;
  PIN_EXTERN HANDLE cm_set_global_heap();
  PIN_EXTERN void cm_reset_global_heap(
	HANDLE                  local_heap_hdl);
  PIN_EXTERN void* cm_get_thread_ls(
	int                     ls_index);
  PIN_EXTERN void cm_set_thread_ls(
	int                     ls_index,
	void*                   ls_p);
  #define FM_INV_SPEC_TREE 0
#endif

#undef PIN_EXTERN

EXPORT_OP void pin_opstack_to_str(
	cm_op_info_t*           coip,
	char**                  strpp,
	int*                    lenp,
	pin_errbuf_t*           ebufp);

EXPORT_OP void pin_log_opstack(
	int32                   level,
	char*                   msg,
	char*                   filep,
	int32                   lineno,
	cm_op_info_t*           coip);

/*
 * hook to set the precommit function pointer
 */
EXPORT_OP void cm_set_precommit_func(
	void                    (*precommit_func)());
	
/*
 * hook to set the preabort function pointer
 */
EXPORT_OP void cm_set_postabort_func(
	void                    (*preabort_func)());


/************************************************************************
 * MACROS
 ************************************************************************/
/* get the pcm_context_t pointer for pcm_op() calls to a DM */
#define PIN_FM_CONN_TO_CTX(connp) ((connp)->dm_ctx)

/* log the opcode op stack (from cm_op_info_t) */
#define PIN_ERR_LOG_OPSTACK(level, msg, coip)                           \
{                                                                       \
        pin_log_opstack(level, msg, __FILE__, __LINE__, coip);          \
}

/* Tokens defining various CM behaviour */
#define CM_TOKEN_SCOPED                 "scoped"
#define CM_TOKEN_ASSIGN_ACCOUNT_OBJ     "assign_account_obj"
#define CM_TOKEN_SEARCHABLE             "searchable"

/* Get the connection currently inuse */
#define CM_FM_GET_CONNECTION()                                          \
        cm_fm_get_connection()

/* get the userid for whom the FM is acting */
#define CM_FM_USERID(ebufp)                                             \
	(((ebufp)->pin_err == PIN_ERR_NONE) ?                               \
	((ebufp)->line_no = __LINE__,(ebufp)->filename = __FILE__,          \
		cm_fm_userid(ebufp)) :                                          \
	(NULL))

/* get the effective userid for whom the FM is acting */
#define CM_FM_EFFECTIVE_USERID(ebufp)                                   \
	(((ebufp)->pin_err == PIN_ERR_NONE) ?                               \
	((ebufp)->line_no = __LINE__,(ebufp)->filename = __FILE__,          \
		cm_fm_effective_userid(ebufp)) :                                \
	(NULL))

/* get the base session obj id */
#define CM_FM_BASE_SESSION(ebufp)                                       \
	(((ebufp)->pin_err == PIN_ERR_NONE) ?                               \
	((ebufp)->line_no = __LINE__,(ebufp)->filename = __FILE__,          \
		cm_fm_base_session(ebufp)) :                                    \
	(NULL))

/* set user transaction id (flist) */
#define CM_FM_SET_TRANS_FLIST(ctxp, flistp)                             \
        cm_fm_set_trans_flist(ctxp, flistp)

/* set "named" user transaction id (flist) */
#define CM_FM_SET_NAMED_TRANS_FLIST(ctxp, flistp, namep)                \
	cm_fm_set_named_trans_flist(ctxp, flistp, namep)

/* get user transaction id (flist) */
#define CM_FM_GET_TRANS_FLIST(ctxp)                                     \
        cm_fm_get_trans_flist(ctxp)

/* get "named" user transaction id (flist) */
#define CM_FM_GET_NAMED_TRANS_FLIST(ctxp, namep)                        \
	cm_fm_get_named_trans_flist(ctxp, namep)

/* drop "named" user transaction id (flist) */
#define CM_FM_DROP_NAMED_TRANS_FLIST(ctxp, namep, ebufp)                 \
        cm_fm_drop_named_trans_flist(ctxp, namep, ebufp)

/* Permission related macros used for overriding standard scoping mechanisms */
#define CM_CRED_SCOPE_OVERRIDE_NONE     0
#define CM_CRED_SCOPE_OVERRIDE_BRAND    1
#define CM_CRED_SCOPE_OVERRIDE_ROOT     2

#define CM_FM_BEGIN_OVERRIDE_SCOPE(x)                                   \
	cm_fm_perm_override_scope(x);

#define CM_FM_END_OVERRIDE_SCOPE(x)                                     \
	cm_fm_perm_override_scope(x);

/* Functions for calling overridden opcodes */
#define CM_FM_OP_PREV_IMPL(connp, flags, in_flistp, out_flistpp, ebufp) \
	cm_fm_op_prev_impl(connp, flags, in_flistp, out_flistpp, 0, ebufp)

#define CM_FM_OP_PREV_IMPL_BY_REF(connp, flags, in_flistp, out_flistpp, ebufp) \
	cm_fm_op_prev_impl(connp, flags, in_flistp, out_flistpp, 1, ebufp)

/* Use as a define for line length when reading a config file */
/* stop people from hard code 1028 everywhere */
#define CM_FM_MAX_CONFIG_FILE_LINE_LEN  1028

#ifdef WINDOWS
/*
 * Defines to switch between using the global heap and the local
 * heap under Windows.  The default is the local heap (one per thread)
 * to ensure the code is MT safe.
 * The global heap is one per process.  You only need to switch to the
 * global heap when allocating memory that you want accessable to all
 * threads.  After the memory is allocated, you should switch back to
 * the local heap (PIN_CM_RESET_GLOBAL_HEAP) as soon a possible because
 * other functions that you may call may allocate memory.
 */

#define PIN_CM_HEAP_VAR                 HANDLE local_thd_heap_hdl
#define PIN_CM_SET_GLOBAL_HEAP          (local_thd_heap_hdl = cm_set_global_heap())
#define PIN_CM_RESET_GLOBAL_HEAP        (cm_reset_global_heap(local_thd_heap_hdl))
#else
#define PIN_CM_HEAP_VAR
#define PIN_CM_SET_GLOBAL_HEAP
#define PIN_CM_RESET_GLOBAL_HEAP
#endif

/* Named transaction Id for user activity auditing */
#define PIN_TRANS_CREATE_USER_ACTIVITY "pin_trans_create_user_activity"
#define PIN_TRANS_UPDATE_PRICE_LIST_CHANGED "pin_trans_update_price_list_changed"
/** The following are functions that were formerly in fm_utils
 * but have been moved into the cm because they more logically belong there
 * The functions retain the fm_utils prefix so as not to break anything
 */
	
#ifdef __cplusplus
}
#endif

#endif /* !_CM_FM_H */
