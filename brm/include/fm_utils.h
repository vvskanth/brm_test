/*******************************************************************
 *     @(#)$Id: fm_utils.h /cgbubrm_omc.19.main.portalbase/1 2019/03/11 21:01:03 arcmisra Exp $ 
 *
* Copyright (c) 1996, 2023, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/
#ifndef _FM_UTILS_H
#define _FM_UTILS_H

/*******************************************************************
 *******************************************************************/

#if defined(__STDC__) || defined(PIN_USE_ANSI_HDRS)
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

#ifdef WINDOWS
  #define snprintf _snprintf

  #ifdef    FM_CONTENT_DLL
    #define FM_CONTENT_DLL_API  __declspec(dllexport)
    #define FM_CONTENT_DLL_DATA __declspec(dllexport)
  #else
    #define FM_CONTENT_DLL_API  __declspec(dllimport)
    #define FM_CONTENT_DLL_DATA __declspec(dllimport)
  #endif

  #ifdef    FM_CUST_UTILS_DLL
    #define FM_CUST_UTILS_DLL_API  __declspec(dllexport)
    #define FM_CUST_UTILS_DLL_DATA __declspec(dllexport)
  #else
    #define FM_CUST_UTILS_DLL_API  __declspec(dllimport)
    #define FM_CUST_UTILS_DLL_DATA __declspec(dllimport)
  #endif

  #ifdef    FM_DEVICE_DLL
    #define FM_DEVICE_DLL_API  __declspec(dllexport)
    #define FM_DEVICE_DLL_DATA __declspec(dllexport)
  #else
    #define FM_DEVICE_DLL_API  __declspec(dllimport)
    #define FM_DEVICE_DLL_DATA __declspec(dllimport)
  #endif

  #ifdef    FM_UTILS_DLL
    #define EXTERN __declspec(dllexport)
    #define FM_UTILS_DLL_API  __declspec(dllexport)
    #define FM_UTILS_DLL_DATA __declspec(dllexport)
  #else
    #define EXTERN __declspec(dllimport)
    #define FM_UTILS_DLL_API  __declspec(dllimport)
    #define FM_UTILS_DLL_DATA __declspec(dllimport)
  #endif

  #ifdef    FM_WAP_DLL
    #define FM_WAP_DLL_API  __declspec(dllexport)
    #define FM_WAP_DLL_DATA __declspec(dllexport)
  #else
    #define FM_WAP_DLL_API  __declspec(dllimport)
    #define FM_WAP_DLL_DATA __declspec(dllimport)
  #endif
#else
         /* Unix */
  #define EXTERN extern

  #ifdef    FM_CONTENT_DLL
    #define FM_CONTENT_DLL_API  extern
    #define FM_CONTENT_DLL_DATA
  #else
    #define FM_CONTENT_DLL_API  extern
    #define FM_CONTENT_DLL_DATA extern
  #endif

  #ifdef    FM_CUST_UTILS_DLL
    #define FM_CUST_UTILS_DLL_API  extern
    #define FM_CUST_UTILS_DLL_DATA
  #else
    #define FM_CUST_UTILS_DLL_API  extern
    #define FM_CUST_UTILS_DLL_DATA extern
  #endif

  #ifdef    FM_DEVICE_DLL
    #define FM_DEVICE_DLL_API  extern
    #define FM_DEVICE_DLL_DATA
  #else
    #define FM_DEVICE_DLL_API  extern
    #define FM_DEVICE_DLL_DATA extern
  #endif

  #ifdef    FM_UTILS_DLL
    #define FM_UTILS_DLL_API  extern
    #define FM_UTILS_DLL_DATA
  #else
    #define FM_UTILS_DLL_API  extern
    #define FM_UTILS_DLL_DATA extern
  #endif

  #ifdef    FM_WAP_DLL
    #define FM_WAP_DLL_API  extern
    #define FM_WAP_DLL_DATA
  #else
    #define FM_WAP_DLL_API  extern
    #define FM_WAP_DLL_DATA extern
  #endif
#endif


#include "pcm.h"
#include "cm_fm.h"		/* for cm_op_info_t   */
#include "pinlog.h"


typedef struct config_item_tags_entry {
	char                *e_expr;
	char                *p_expr;
#if defined(_pin_os_regex_h_) || defined(_REGEX_H)
	regex_t             e_compiled_expr;
	regex_t             p_compiled_expr;
#else
	char                e_compiled_expr[256];
	char                p_compiled_expr[256];
#endif
	char                *i_tag;
} config_item_tags_t;

typedef struct config_item_tags_table {
	char 		    confName[256];
	u_int               howmany;
	config_item_tags_t  *table;
} config_item_tags_table_t;

typedef struct config_item_types_entry {
	char                *i_tag;
	char                *i_type;
	char                *descr;
	u_int               type;
} config_item_types_t;

typedef struct config_item_types_table {
	char 		    confName[256];
	u_int               howmany;
	config_item_types_t *table;
} config_item_types_table_t;

typedef struct config_refresh_map {
        char    *config_obj_type;
        time_t    last_mod_t;
} config_refresh_map_t;

typedef struct config_beid_rules {
	char                *e_expr;
#if defined(_pin_os_regex_h_) || defined(_REGEX_H)
	regex_t             e_compiled_expr;
#else
	char                e_compiled_expr[256];
#endif
	u_int               bal_id;
	u_int               processing_stage;
	int32               rounding_mode;
	int32               rounding;
	pin_decimal_t       *tol_amt_minp;
	pin_decimal_t       *tol_amt_maxp;
	pin_decimal_t       *tol_amt_perp;
} config_beid_rules_t;

typedef struct config_beid_rules_table {
	u_int               howmany;
	config_beid_rules_t *table;
} config_beid_rules_table_t;

/*
 * Structure to cache a opcode operation
 */
typedef struct pin_op_info {
	pcm_context_t       *ctxp;
	u_int               opcode;
	u_int               opflags;
	pin_flist_t         *in_flistp;
	struct pin_op_info  *next;
} pin_op_info_t;

/*
 * Thread local storage to be used by fm_utils
 */
typedef struct channel_states {
	pin_op_info_t       *channel_push_oplist_head;
	pin_op_info_t       *channel_push_oplist_tail;
	u_int               account_create_pushed;
	u_int               account_modify_pushed;
	u_int               service_create_pushed;
	u_int               service_modify_pushed;
} pin_channel_states;

/*
 * Time zone and daylight saving time corrections applied to GMT to
 * obtain the local time of some geographic location
 */

typedef struct time_parameters {
	long tp_gmt_offset;     /* the offset from GMT in seconds */
	long tp_dst_offset;     /* contribution of DST in seconds */
} time_parameters;

/*
 * exploded_time --
 *
 *     Time broken down into human-readable components such as year, month,
 *     day, hour, minute, second, and microsecond.  Time zone and daylight
 *     saving time corrections may be applied.  If they are applied, the
 *     offsets from the GMT must be saved in the 'tm_params' field so that
 *     all the information is available to reconstruct GMT.
 */
typedef struct exploded_time {
	int tm_usec;               /* microseconds past tm_sec (0-99999)  */
	int tm_sec;                /* seconds past tm_min (0-61, accomodating
                                   up to two leap seconds) */
	int tm_min;                /* minutes past tm_hour (0-59) */
	int tm_hour;               /* hours past tm_day (0-23) */
	int tm_mday;               /* days past tm_mon (1-31, note that it
                                               starts from 1) */
	int tm_month;              /* months past tm_year (0-11, Jan = 0) */
	int tm_year;                /* absolute year, AD (note that we do not
                                                count from 1900) */

	int tm_wday;                /* calculated day of the week
                                                (0-6, Sun = 0) */
	int tm_yday;                /* calculated day of the year
                                                (0-365, Jan 1 = 0) */

	time_parameters tm_params;  /* time parameters used by conversion */
} exploded_time;

/* 
 * Default parameters for the service profile cache.
 */
#define PROFILE_CACHE_NAME  "fm_utils_gal_srvc_profile_cache"
#define PROFILE_CACHE_TTL   60*60


/*
 * Default parameters for the content profile cache.
 */
#define CONTENT_PROFILE_CACHE_NAME      "fm_utils_content_srvc_profile_cache"
#define CONTENT_PROFILE_CACHE_TTL       60*60

/*
 * Default parameters for the field validation cache.
 */
#define FIELD_VALIDATE_CACHE_NAME       "fm_cfg_fld_validate_cache"

/********************************************************************
 * BRM ECE Realtime Synchronization
 *******************************************************************/

/* Named transaction flist caching ECE business events */
#define PIN_TRANS_NAME_ECE_PUBLISH "ece_business_event_cache"

/* Return value from ECE publisher */
#define PIN_ECE_PUBLISH_SUCCESS 0 /* Success */

/*******************************************************************
 * Public functions exported from fm_utils.
 *******************************************************************/

/*
 * fm_utils.c
 */
FM_UTILS_DLL_API void
fm_utils_init_alog PROTO_LIST((
	pin_flist_t         *a_flistp,
	char                *type,
	char                *name,
	int64               db_no,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_prep_alog PROTO_LIST((
	pin_flist_t         *a_flistp,
	pin_flist_t         *i_flistp,
	pin_errbuf_t        *ebufp));


FM_UTILS_DLL_API void
fm_utils_create_alog PROTO_LIST((
	pcm_context_t       *ctxp,
	u_int               opcode,
	u_int               flags,
	pin_flist_t         *i_flistp,
	pin_flist_t         **r_flistpp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_pre_create_notify PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *a_pdp,
	char                *event_type,
	char                *program,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_add_result PROTO_LIST((
	pcm_context_t       *ctxp,
	u_int               opcode,
	u_int               flags,
	pin_flist_t         *a_flistp,
	pin_flist_t         *r_flistp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_create_result PROTO_LIST((
	pcm_context_t       *ctxp,
	u_int               flags,
	pin_flist_t         *a_flistp,
	pin_flist_t         **r_flistpp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_update_result PROTO_LIST((
	pcm_context_t       *ctxp,
	u_int               flags,
	pin_flist_t         *a_flistp,
	pin_flist_t         **r_flistpp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API u_int32
fm_utils_get_bill_type PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *a_pdp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API u_int32
fm_utils_is_gl_posted PROTO_LIST((
	pcm_context_t	*ctxp,
	poid_t		*a_pdp,
	char*		gl_segment,
	time_t		end_t,
	pin_errbuf_t	*ebufp ));

FM_UTILS_DLL_API time_t
fm_utils_get_gl_posted_date PROTO_LIST((
        pcm_context_t   *ctxp,
        poid_t          *a_pdp,
        char*           gl_segment,
        pin_errbuf_t    *ebufp ));

FM_UTILS_DLL_API void
fm_utils_get_gl_flistp PROTO_LIST((
	pcm_context_t       *ctxp,
	int64               database,
	pin_flist_t         **r_flistpp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_get_rollup_parent_gl_segment PROTO_LIST((
	pin_flist_t         *gl_flistp,
	char                *in_gl,
	char                *out_gl,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_log_event PROTO_LIST((
	pcm_context_t       *ctxp,
	pin_flist_t         *i_flistp,
	poid_t              *pdp,
	char                *typep,
	char                *namep,
	char                *descp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API pcm_context_t*
fm_utils_check_context_trans_info PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *in_poidp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API int32
fm_utils_is_sponsoree PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *a_pdp,
	pin_flist_t         **g_flistpp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API int32
fm_utils_is_sponsor PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *a_pdp,
	pin_flist_t         **g_flistpp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_read_field_account PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *a_pdp,
	pin_flist_t         **r_flistpp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API int32
fm_utils_get_customized_event_format PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *e_pdp,
	pin_flist_t         **f_flistpp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API int32
fm_utils_is_customized_event PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *e_pdp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void 
fm_utils_get_acct_svc_obj PROTO_LIST((
	pcm_context_t   *ctxp,
	pin_flist_t     *a_flistp,
	pin_flist_t	**r_flistpp,
	pin_errbuf_t    *ebufp));

FM_UTILS_DLL_API void 
fm_utils_create_schedule_object PROTO_LIST((
	pcm_context_t	*ctxp,
	int32		opcode_num,
	int32		flags,
	time_t		scheduled_date,
	char		*append_obj_type,
	char		*action_descr,
	char		*system_descr,
	char		*program_name,
	pin_flist_t	*input_flist,
	pin_errbuf_t	*ebufp));

FM_UTILS_DLL_API int32
fm_utils_notify_get_lastnotify_stamp PROTO_LIST((
	pin_flist_t	*in_flistp,
	time_t		in_curr_time_t,
	time_t		*last_notify_t,
	char		*last_notify_offset,
	int32		offset_len,
	time_t		sched_date,
	pin_errbuf_t	*ebufp));

/*----- Global variables to store some of stanzas from CM pin.conf ----*/
/*
 * fm_utils_comment.c
 */

FM_UTILS_DLL_API void
fm_utils_sys_comment PROTO_LIST((
	pin_flist_t         *flistp,
	char                *description,
	u_int               level,
	char                *detail,
	pin_errbuf_t        *ebufp));

/*
 * fm_utils_init.c
 */
FM_UTILS_DLL_API void
fm_utils_init PROTO_LIST((
	int32               *errp));

/*
 * fm_utils_search.c
 */

FM_UTILS_DLL_API void
fm_utils_get_obj PROTO_LIST((
	pcm_context_t       *ctxp,
	u_int               search_id,
	u_int64             db_no,
	char                *obj_name,
	pin_flist_t         **obj_flistpp,
	pin_errbuf_t        *ebufp));

/*
 * fm_utils_publish.c
 */

/*******************************************************************
 * Object Name Strings (for message events)
 *******************************************************************/
#define PIN_OBJ_NAME_EVENT_MSG_PASSWD           \
                        "Password Message"
#define PIN_OBJ_NAME_EVENT_MSG_INVOICE_READY    \
                        "Invoice Available Message"
#define PIN_OBJ_NAME_EVENT_MSG_PAYMENT_SUCCESS  \
                        "Payment Success Message"
#define PIN_OBJ_NAME_EVENT_MSG_PAYMENT_FAILURE  \
                        "Payment Failure Message"
#define PIN_OBJ_NAME_EVENT_MSG_CARD_EXPIRY      \
                        "Card Expiry Message"
#define PIN_OBJ_NAME_EVENT_MSG_CONTRACT_CREATED \
                        "Contract created Message"
#define PIN_OBJ_NAME_EVENT_MSG_CONTRACT_CANCELED        \
                        "Contract Canceled Message"
#define PIN_OBJ_NAME_EVENT_MSG_CONTRACT_ENDED   \
                        "Contract Ended Message"
#define PIN_OBJ_NAME_EVENT_MSG_CONTRACT_MODIFIED        \
                        "Contract Modified Message"
#define PIN_OBJ_NAME_EVENT_MSG_CONTRACT_PHASE_STARTED   \
                        "Contract Phase Start Message"
#define PIN_OBJ_NAME_EVENT_MSG_CONTRACT_PHASE_ENDED     \
                        "Contract Phase End Message"
#define PIN_OBJ_NAME_EVENT_MSG_CONTRACT_PHASE_END_UPCOMING      \
                        "Contract Phase End Upcoming Message"

/*******************************************************************
 * Object Name Strings (for publish/API events)
 *******************************************************************/
#define PIN_OBJ_NAME_EVENT_PUBLISH_ACCOUNT_CREATE\
                        "Account Create"
#define PIN_OBJ_NAME_EVENT_PUBLISH_SERVICE_CREATE\
                        "Service Create"
#define PIN_OBJ_NAME_EVENT_PUBLISH_PRODUCT_PURCHASE\
                        "Product Purchase"
#define PIN_OBJ_NAME_EVENT_PUBLISH_BILL_CREATE\
                        "Bill Create"
#define PIN_OBJ_NAME_EVENT_PUBLISH_INVOICE_READY\
                        "Invoice Available"
#define PIN_OBJ_NAME_EVENT_PUBLISH_PAYMENT_SUCCESS\
                        "Payment Success"
#define PIN_OBJ_NAME_EVENT_PUBLISH_PAYMENT_FAILURE\
                        "Payment Failure"
#define PIN_OBJ_NAME_EVENT_PUBLISH_CARD_EXPIRY\
                        "Card Expiry"
#define PIN_OBJ_NAME_EVENT_PUBLISH_PAYMENT_REQUEST \
                        "Payment Request"
#define PIN_OBJ_NAME_EVENT_PUBLISH_CYCLE_FORWARD \
                        "Cycle Forward"
#define PIN_OBJ_NAME_EVENT_PUBLISH_CONTRACT_CREATED     \
                        "Contract created"
#define PIN_OBJ_NAME_EVENT_PUBLISH_CONTRACT_CANCELED    \
                        "Contract Canceled"
#define PIN_OBJ_NAME_EVENT_PUBLISH_CONTRACT_ENDED       \
                        "Contract Ended"
#define PIN_OBJ_NAME_EVENT_PUBLISH_CONTRACT_MODIFIED    \
                        "Contract Modified"
#define PIN_OBJ_NAME_EVENT_PUBLISH_CONTRACT_PHASE_ENDED \
                        "Contract Phase Ended"
#define PIN_OBJ_NAME_EVENT_PUBLISH_CONTRACT_PHASE_STARTED       \
                        "Contract Phase Started"
#define PIN_OBJ_NAME_EVENT_PUBLISH_CONTRACT_PHASE_END_UPCOMING  \
                        "Contract Phase End Upcoming"
/*******************************************************************
 * Object Type Strings (for message events)
 *******************************************************************/
#define PIN_OBJ_TYPE_EVENT_MSG_PASSWD           \
                        "/event/notification/message/password"
#define PIN_OBJ_TYPE_EVENT_MSG_PAYMENT_SUCCESS  \
                        "/event/notification/message/payment_success"
#define PIN_OBJ_TYPE_EVENT_MSG_PAYMENT_FAILURE  \
                        "/event/notification/message/payment_fail"
#define PIN_OBJ_TYPE_EVENT_MSG_INVOICE_READY    \
                        "/event/notification/message/invoice_ready"
#define PIN_OBJ_TYPE_EVENT_MSG_CARD_EXPIRY      \
                        "/event/notification/message/card_expiry"
#define PIN_OBJ_TYPE_EVENT_MSG_CONTRACT_CREATED \
                        "/event/notification/message/contract_created"
#define PIN_OBJ_TYPE_EVENT_MSG_CONTRACT_CANCELED        \
                        "/event/notification/message/contract_canceled"
#define PIN_OBJ_TYPE_EVENT_MSG_CONTRACT_ENDED   \
                        "/event/notification/message/contract_ended"
#define PIN_OBJ_TYPE_EVENT_MSG_CONTRACT_MODIFIED        \
                        "/event/notification/message/contract_modified"
#define PIN_OBJ_TYPE_EVENT_MSG_CONTRACT_PHASE_START     \
                        "/event/notification/message/contract_phase_start"
#define PIN_OBJ_TYPE_EVENT_MSG_CONTRACT_PHASE_END       \
                        "/event/notification/message/contract_phase_end"
#define PIN_OBJ_TYPE_EVENT_MSG_CONTRACT_PHASE_END_UPCOMING      \
                        "/event/notification/message/contract_phase_upcoming_end"

#define BRMC_TRANS_PUBLISH_PAYLOAD "brmc_publish"

/*******************************************************************
 * Object Type Strings (for publish/API events)
 *******************************************************************/
#define PIN_OBJ_TYPE_EVENT_PUBLISH_ACCOUNT_CREATE\
                        "/event/notification/publish/account_create"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_SERVICE_CREATE\
                        "/event/notification/publish/service_create"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_PRODUCT_PURCHASE\
                        "/event/notification/publish/product_purchase"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_BILL_CREATE\
                        "/event/notification/publish/bill_create"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_INVOICE_READY        \
                        "/event/notification/publish/invoice_ready"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_PAYMENT_SUCCESS      \
                        "/event/notification/publish/payment_success"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_PAYMENT_FAILURE      \
                        "/event/notification/publish/payment_fail"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_CARD_EXPIRY  \
                        "/event/notification/publish/card_expiry"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_PAYMENT_REQUEST \
                        "/event/notification/publish/payment_request"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_CYCLE_FORWARD \
                        "/event/notification/publish/cycle_forward"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_CONTRACT_CREATED     \
                        "/event/notification/publish/contract_created"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_CONTRACT_CANCELED    \
                        "/event/notification/publish/contract_canceled"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_CONTRACT_ENDED       \
                        "/event/notification/publish/contract_ended"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_CONTRACT_MODIFIED    \
                        "/event/notification/publish/contract_modified"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_CONTRACT_PHASE_END   \
                        "/event/notification/publish/contract_phase_end"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_CONTRACT_PHASE_START \
                        "/event/notification/publish/contract_phase_start"
#define PIN_OBJ_TYPE_EVENT_PUBLISH_CONTRACT_PHASE_END_UPCOMING  \
                        "/event/notification/publish/contract_phase_upcoming_end"


typedef struct publish_context_info {
	int64               dbno;
	int                 txn_open_sent;
	pcm_context_t       *dm_publish_ctxp;
	struct publish_context_info *next;
} pin_publish_context_info;

typedef struct publish_state_info {
	int                 publish_in_txn;
	pcm_context_t       *trans_initiatorp;
	struct publish_context_info *head;
	int                 opening_publish_trans;
} pin_publish_state_info;

struct publish_ctx_info_wrap {
        void * markerp; /* this holds the owning context */
        pin_publish_context_info *pub_ctx_infop;
        struct publish_ctx_info_wrap *next;
};
typedef struct publish_ctx_info_wrap pin_publish_ctx_info_wrap;

struct publish_state_info_wrap {
        void *markerp; /* this holds the owning context */
        pin_publish_state_info *pub_state_infop;
        struct publish_state_info_wrap *next;
};
typedef struct publish_state_info_wrap pin_publish_state_info_wrap;
FM_UTILS_DLL_API void
fm_utils_publish_init PROTO_LIST((
        pcm_context_t           *ctxp,
        int64                   database,
	int32               *errp));

FM_UTILS_DLL_API pin_publish_context_info*
fm_utils_get_publish_context_info_for_dm PROTO_LIST((
	int64               dbno,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API pin_publish_state_info*
fm_utils_get_publish_state_info PROTO_LIST((
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_publish_transaction_state PROTO_LIST((
	cm_nap_connection_t  *connp,
	int32                opcode,
	pin_flist_t          *in_flistp,
	pin_errbuf_t         *ebufp));

FM_UTILS_DLL_API int
fm_utils_is_publish_enabled PROTO_LIST(());

FM_UTILS_DLL_API void
fm_utils_publish_prep_event(
        pcm_context_t   *ctxp,
        poid_t          *a_pdp,
        char            *event_namep,
        char            *event_typep,
        pin_flist_t     *i_flistp,
        pin_flist_t     **ret_flistpp,
        pin_errbuf_t    *ebufp);


/*
 * fm_utils_read_config.c
 */

FM_UTILS_DLL_API void
fm_utils_read_config PROTO_LIST((
	pcm_context_t       *ctxp,
	const poid_t        *i_poidp,
	pin_flist_t         **return_flp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_refresh_config(
	int32* err);

FM_UTILS_DLL_API int
fm_utils_get_refresh_interval(
		int32* err);

FM_UTILS_DLL_API int
fm_utils_cfg_get_refresh_interval(
	 pcm_context_t *in_ctxp, int32* err);

FM_UTILS_DLL_API int
fm_utils_cfg_external_ids_enabled(
	 pcm_context_t *in_ctxp, int32* err);

FM_UTILS_DLL_API u_int32
fm_utils_cfg_config_refresh_required(
	pcm_context_t	*ctxp,
	const char	*config_obj_type,
	pin_errbuf_t	*ebufp);

FM_UTILS_DLL_API time_t 
fm_utils_cfg_get_config_in_mem_mod_t(
        char            *config_obj_type,  
        pin_errbuf_t    *ebufp);

FM_UTILS_DLL_API void 
fm_utils_cfg_set_config_in_mem_mod_t(
        char    *config_obj_type,  
        time_t  last_mod_t,
        pin_errbuf_t    *ebufp);

/*
 * fm_utils_prov.c
 */
FM_UTILS_DLL_API void
fm_utils_prov_init(
	void (*txn_op_func)(
		cm_nap_connection_t       *connp,
		int32                     opcode,
		pin_flist_t               *in_flistp,
		pin_errbuf_t              *ebufp) );

FM_UTILS_DLL_API void
fm_utils_prov_txn_op(
	cm_nap_connection_t *connp,
	int32               opcode,
	pin_flist_t         *in_flistp,
	pin_errbuf_t        *ebufp);

/*
 * fm_utils_gals_password.c
 * REVISIT - Should be renamed fm_utils_password.c
 */
FM_UTILS_DLL_API void
fm_utils_gal_passwd_find_password(
	pcm_context_t       *ctxp,
	pin_flist_t         *i_flistp,
	pin_errbuf_t        *ebufp);

/************************************************************
 * THINGS THAT SHOULD BE MOVED BECAUSE THEY ARE NOT
 * IN THIS DIRECTORY
 ************************************************************/

/*
 * fm_utils_cust.c
 */

FM_CUST_UTILS_DLL_API void
fm_utils_cust_add_missing_service_substructs PROTO_LIST((
	pcm_context_t       *ctxp,
	pin_flist_t         *service_flistp,
	pin_errbuf_t        *ebufp));

/*
 * fm_utils_gprs.c
 */

FM_UTILS_DLL_API void
fm_utils_gprs_resolve_acct PROTO_LIST((
	pcm_context_t       *ctxp,
	pin_flist_t         *i_flp,
	pin_flist_t         **o_flistpp,
	pin_errbuf_t        *ebufp));

/*
 * fm_utils_wireless.c
 */

FM_WAP_DLL_API void
fm_utils_wireless_find_service PROTO_LIST((
	pcm_context_t       *ctxp,
	pin_flist_t         *i_flistp,
	pin_flist_t         **o_flistpp,
	pin_errbuf_t        *ebufp));

FM_WAP_DLL_API int 
fm_utils_wireless_validate_target PROTO_LIST((
	pcm_context_t       *ctxp,
	pin_flist_t         *i_flistp,
	int32               flags,
	int                 mode,
	int                 (*compare)(),
	pin_errbuf_t        *ebufp));

FM_WAP_DLL_API void
fm_utils_wireless_transform_url PROTO_LIST((
	char                *inUrl,
	char                **outUrl));

FM_WAP_DLL_API void
fm_utils_wireless_get_profile_filter_lists PROTO_LIST((
	pcm_context_t       *ctxp,
	int32               flags,
	poid_t              *svc_profile_pdp,
	pin_flist_t         **r_flistpp,
	int                 mode,
	pin_errbuf_t        *ebufp));

FM_WAP_DLL_API int
fm_utils_wireless_compare_URLs PROTO_LIST((
	char                *targetURL,
	char                *filterlistURL));

FM_WAP_DLL_API void
fm_utils_wireless_alias_to_services PROTO_LIST((
	pcm_context_t       *ctxp,
	pin_flist_t         *i_flistp,
	char                *alias,
	int32               id,
	pin_errbuf_t        *ebufp));

/* 
 * fm_utils_device.c
 */

FM_DEVICE_DLL_API void
fm_utils_device_get_next_states PROTO_LIST((
	pcm_context_t       *ctxp,
	char                *device_type,
	int32               current_state,
	pin_flist_t         **next_states_flistpp,
	pin_errbuf_t        *ebufp));

FM_DEVICE_DLL_API void
fm_utils_device_find_state_machine PROTO_LIST((
	pcm_context_t       *ctxp,
	char                *device_type,
	pin_flist_t         **device_state_flistpp,
	pin_errbuf_t        *ebufp));

FM_DEVICE_DLL_API void
fm_utils_device_find_state_machine_for_device PROTO_LIST((
	pcm_context_t       *ctxp,
	pin_flist_t         *i_flistp,
	pin_flist_t         **device_state_flistpp,
	pin_errbuf_t        *ebufp));

FM_DEVICE_DLL_API poid_t*
fm_utils_get_device_brand PROTO_LIST((
	pcm_context_t       *ctxp,
	pin_flist_t         *i_flistp,
	pin_errbuf_t        *ebufp));

/*
 * fm_utils_content.c
 */

FM_CONTENT_DLL_API int
fm_utils_content_validate_content_cat PROTO_LIST((
	pcm_context_t       *ctxp,
	pin_flist_t         *i_flistp,
	int32               flags,
	int                 mode,
	int                 (*compare)(),
	pin_errbuf_t        *ebufp));


FM_CONTENT_DLL_API void
fm_utils_content_get_profile_filter_lists PROTO_LIST((
	pcm_context_t       *ctxp,
	int32               flags,
	poid_t              *svc_profile_pdp,
	pin_flist_t         **r_flistpp,
	int                 mode,
	pin_errbuf_t        *ebufp));

FM_CONTENT_DLL_API int
fm_utils_content_pol_compare_content_cat_names PROTO_LIST((
	char                *targetContentCat,
	char                *filterContentCat));

/*
 * fm_utils_lineage.c 
 */
FM_UTILS_DLL_API poid_t*
fm_utils_lineage_get_billing_group_by_parent PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *parent_poidp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_lineage_add_to_billing_group PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *account_poidp,
	poid_t              *bill_group_poidp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_lineage_create_billing_group PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *parent_poidp,
	char                *namep,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API int
fm_utils_account_is_brand PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *account_poidp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API poid_t*
fm_utils_lineage_get_parent_poid PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *member_poidp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API char*
fm_utils_lineage_get_group_name PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *account_poidp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API char*
fm_utils_lineage_get_brand_from_account PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *account_poidp,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API char*
fm_utils_lineage_poid_to_lineage_str PROTO_LIST((
	poid_t              *poidp,
	int                 brand,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_util_lineage_set_toplevel_account_lineage PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *account_poidp,
	int                 isbrand,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API poid_t*
fm_utils_lineage_root_poid PROTO_LIST((
	u_int64             db_no,
	pin_errbuf_t        *ebufp));

FM_UTILS_DLL_API void
fm_utils_lineage_get_brand_hierarchy PROTO_LIST((
	pcm_context_t       *ctxp,
	poid_t              *a_pdp,
	pin_flist_t         **b_flistpp,
	pin_errbuf_t        *ebufp));

/***********************************************************************
 * Prototypes for Timezone routines 
 ***********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
FM_UTILS_DLL_API void
fm_utils_timezone_get_default_timezone PROTO_LIST((
	char            *tzone_str,
	pin_errbuf_t    *ebufp,
	int				tzone_str_len));

/*
 * Expand time binding it to time parameters provided by fm_utils_time_param_fn.
 * The calculation is envisoned to proceed in the following steps:
 *   - From given long, calculate exploded_time in GMT
 *   - Apply the given fm_utils_time_param_fn to the GMT that we just calculated
 *     to obtain time_parameters.
 *   - Add the time_parameters offsets to GMT to get the local time
 *     as exploded_time.
 */

FM_UTILS_DLL_API void
fm_utils_timezone_apply_sec_offset PROTO_LIST((
	exploded_time   *time,
	long            secOffset,
pin_errbuf_t    *ebufp));

FM_UTILS_DLL_API void
fm_utils_timezone_explode_time PROTO_LIST((
	long secs,
	const char* tzone_str,
	exploded_time *exploded,
	pin_errbuf_t    *ebufp));

/* Reverse operation of fm_utils_explode_time */
FM_UTILS_DLL_API long
fm_utils_timezone_implode_time PROTO_LIST((
	const exploded_time *exploded,
	pin_errbuf_t    *ebufp));

FM_UTILS_DLL_API void
fm_utils_timezone_apply_sec_offset PROTO_LIST((
	exploded_time *time,
	long secOffset,
	pin_errbuf_t    *ebufp));

FM_UTILS_DLL_API int
fm_utils_timezone_is_valid_timezone PROTO_LIST((
	const char      *tzone_str,
	pin_errbuf_t    *ebufp));

FM_UTILS_DLL_API int
fm_utils_timezone_is_timezone_available PROTO_LIST((
	const char      *tzone_str,
	pin_errbuf_t    *ebufp));

FM_UTILS_DLL_API void
fm_utils_timezone_compute_offset PROTO_LIST((
	const char      *tzone_str,
	pin_flist_t     **e_flispp,
	pin_errbuf_t    *ebufp));

FM_UTILS_DLL_API void
fm_utils_timezone_compute_GMT PROTO_LIST((
	long            time,
	exploded_time   *gmt,
	pin_errbuf_t    *ebufp));

FM_UTILS_DLL_API int 
fm_utils_timezone_get_dst PROTO_LIST((
	exploded_time *tm1,
	char *tzone_str,
	time_t start_t,
	time_t end_t,
	time_t *ts));

FM_UTILS_DLL_API void
fm_utils_cfg_role_refresh PROTO_LIST((
        pcm_context_t           *ctxp,
        int64                   database,
        pin_errbuf_t            *ebufp));

#ifdef __cplusplus
}
#endif

#undef EXTERN

#endif /* !_FM_UTILS_H */
