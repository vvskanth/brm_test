/* 
 * @(#)%Portal Version: pcp.h:CUPmod7.3PatchInt:2:2006-Nov-27 13:52:25 %
 *
* Copyright (c) 1996, 2022, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or
 * its licensors and may be used, reproduced, stored or transmitted only
 * in accordance with a valid Oracle license or sublicense agreement.
 */

/*
 * This file contains definitions for the pcp layer.
 */

#ifndef _PCP_H
#define _PCP_H

#include <sys/types.h>

#if defined(INCL_SSL_)
#if defined(FOR_CM) || defined(FOR_PORTAL) || defined(FOR_DM) || defined(FOR_QM)
#include "pin_ssl.h"
#endif
#endif

#ifndef _PIN_TYPE_H_
  #include "pin_type.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Setup the external declaration stuff correctly */
#ifdef WINDOWS
  #if defined(PCP_DLL) || defined(_PCP_)
    #define PCP_EXTERN          __declspec(dllexport)
  #else
    #ifdef _NO_DLLIMPORT
      #define PCP_EXTERN        extern
    #else
      #define PCP_EXTERN        __declspec(dllimport)
    #endif /* _NO_DLLIMPORT */
  #endif /* PCP_DLL */
#else
  #define PCP_EXTERN            extern
#endif 

/************************************************************************
 * List header structure. Used throughout the system for maintaining
 * lists of data. Actual list definition gets customized depending
 * on the type of each element.
 *
 * The header keeps track of how many elements are on the list, and how
 * there is allocated space to hold. Entry is a pointer to an array
 * of elements that comprise the list.
 ************************************************************************/
typedef u_int32 pin_flist_field_cnt_t;      /* the flist count of fields type */

typedef struct pcp_list_hdr {
    pin_flist_field_cnt_t    allocated;
    pin_flist_field_cnt_t    used;
} pcp_list_hdr_t;

/*
 * The poid.
 * This identifier for an object is unique forever in the whole universe.
 * Poids are opaque to the PCM level.
 */
typedef struct poid {
    pin_db_no_t       database;
    pin_poid_id_t     id;
    pin_poid_type_t   type;
    pin_poid_rev_t    rev;
#if defined(MEMPOOL) && !defined(USE_POID_MAP)
    char	      type_data[PCM_MAX_POID_TYPE+1];
#endif
} real_poid_t;

/*
 * Performance testing shows that setting the individual
 * values is actually faster than using memset.
 */
#if defined(MEMPOOL) && !defined(USE_POID_MAP)
  #define REAL_POID_CLEAR(ptr)                      \
	(ptr)->database = 0;                            \
	(ptr)->id = 0;                                  \
	(ptr)->type = 0;                                \
	(ptr)->rev = 0;                                 \
	(ptr)->type_data[0] = '\0'
#else
  #define REAL_POID_CLEAR(ptr)                      \
	(ptr)->database = 0;                            \
	(ptr)->id = 0;                                  \
	(ptr)->type = 0;                                \
	(ptr)->rev = 0
#endif

/***********************************************************************************
 * PIN_FLDT_TIME - private/opaque data/defines
 **********************************************************************************/
/*
 * private structure - known only to pcm/pin_time.c and to pcp_xdrst.c's XDR rules.
 */
typedef struct pin_real_time {
    int32   flag;       /* PIN_TIME_FLAG_VALID_xxx */
    int32   year;
    int32   month;      /* 1 - 12 */
    int32   mday;       /* 1 - 31 */
    int32   hour;       /* 0 - 23 */
    int32   minute;     /* 0 - 59 */
    int32   second;     /* 0 - 59 */
    char    tzstr[8];
} pin_real_time_t;

#define PIN_TIME_FLAG_VALID_TIME    0x0001  /* year, ... valid, else NULL */
#define PIN_TIME_FLAG_VALID_ZONE    0x0002  /* zone valid, else NULL */

/***********************************************************************************
 * PIN_FLDT_TEXTBUF - private/opaque data/defines
 **********************************************************************************/
/*
 * XXX not clear if really opaque, but...
 */
typedef struct pin_real_textbuf {
    int32       flag;       /* if XBUF, ... */
    int32       size;       /* size of data */
    int32       offset;     /* offset (for read) */
    caddr_t     data;       /* pointer to data (BUF) */
    char*       xbuf_file;  /* ptr to filename for XBUF */
} pin_real_textbuf_t;


/*
 * private, "real" type of value which is externally "void *"
 */
typedef union pcp_fld_val {
    int32                  f_int;
    u_int32                f_uint;     /* DEPRECATED*/
    enum pcp_enum {XX,YY}  f_enum;
    double*                f_numeric;  /* DEPRECATED*/
    char*                  f_str;
    pin_buf_t*             f_buf;
    real_poid_t*           f_poid;
    pin_fld_tstamp_t       f_tstamp;
    struct pcp_fld_list*   f_array;
    struct pcp_fld_list*   f_substruct;
    struct pcp_fld_list*   f_obj;
    pin_binstr_t*          f_binstr;
    pin_errbuf_t*          f_errbuf;
    pin_decimal_t*         f_decimal;
    pin_real_time_t*       f_time;
    pin_real_textbuf_t*    f_textbuf;
    int64                  f_int64;
} pcp_fld_val_t;

/*
 * Define a field number/value pair. (16 bytes)
 * Record-id is only used when dealing with array elements.
 */
typedef struct pcp_fld {
    pin_fld_type_t   type;       /* fundamental type */
    u_short          flags;
    pin_fld_num_t    field;      /* field number */
    pin_rec_id_t     record_id;  /* array element id */
    pcp_fld_val_t    val;        /* content / content pointer */
} pcp_fld_t;

/*
 * Flist heap structure
 */
#define FLIST_HEAP_ROUNDUP(x)   \
        (((x) + (sizeof(u_int64) - 1)) & ~(sizeof(u_int64) - 1))

typedef struct pcp_flist_heap{
        u_int32         totalHeapSize;  /* totalHeapSize in char */
        u_int32         freeHeapSize;   /* freeHeapSize in char */
        char*           heapCurrentPtr;
        char*           nextHeapPtr;
	int32		mempool;
	int32		pad;		/* MAKE SURE THIS STRUCT 
					   IS SIZED on 64bit boundary */
  } pcp_flist_heap_t;

/*
 * Define a list of fields.
 */
typedef struct pcp_fld_list {
	/* these items are fixed size, regardless of platform */
	pcp_list_hdr_t  hdr;		/* 8 bytes */
	int32		flags;		/* 4 bytes */
	u_int32         freeHeapSize;	/* 4 bytes */
	/* these items could be 4 or 8 bytes depending on platform */
	pcp_fld_t*      entry;		/* 4 or 8 */
	char*           heapCurrentPtr;	/* 4 or 8 */
	char*           flist_heap;	/* 4 or 8 */
#ifndef PIN_64_BIT
	/* For 32 bit platforms, neccessary to pad out since the 
	 * 3 previous members end up putting localHeap on uneven
	 * boundary.
	 */
	int32		reserved;	/* pad out localHeap word64 aligned */
#endif
	/* Put flist heap at end of struct */
	char            localHeap[PCM_FLIST_HEAP_SIZE]; /* huge */
} pcp_fld_list_t;

/*
 * defined flags for pcp_fld_list_t flags
 */

#define PCP_FLIST_FLDS_IN_POOL		0x0001

/*
 * An entry in the poid index cache
 */
typedef struct pcp_poid_idx_entry {
     real_poid_t*       pdp;       /* Pointer to poid */
     pcp_fld_list_t*    fldp;      /* Pointer to piece of flist containing */
} pcp_poid_idx_entry_t;
                
/*
 * Structure to contain the index of poids in an flist for fast lookup
 */
typedef struct pcp_poid_idx {
    pcp_list_hdr_t          hdr;
    pcp_poid_idx_entry_t*   entry;
} pcp_poid_idx_t;

/*
 * flags for pcm_open
 */

typedef int32 pcp_open_flags_t;           /* pcp_open flags type */

/* first two exclusive bit values for pcm_open() defined in pcp.h */
/* let's reserve the first byte for user flags */
#define PCP_OPEN_NAP           0x02
#define PCP_OPEN_CM2DM       0x0200      /* CM to DM */
#define PCP_OPEN_CM2NP       0x0400      /* CM to N*P */
#define PCP_OPEN_CM          0x0600      /* either CM to DM or CM to N*P */
#define PCP_OPEN_DM          0x0800      /* DM back to CM, no malloc! */

#define PCP_OPEN_BIDIR      0x01000     /* CM <-> EM */

#define PCP_OPEN_NMGR       0x02000     /* clients to Node Manager */
#define PCP_OPEN_DM2DM      0x04000     /* DM to DM (e.g. dm to timos) */
#define PCP_OPEN_CM2EM      0x08000     /* CM to EM */
#define PCP_OPEN_PCMDD2DM   0x20000     /* PCMDD to DM */

/* type for supporting dual timeout */
typedef int32 pcp_timeout_mode_t;
#define PCP_SINGLE_TIMEOUT	0
#define PCP_DUAL_TIMEOUT	1

/*
 * Set this if program using pcp is multithreaded
 */
PCP_EXTERN int32 pcp_multithreaded;

/*
 * DEBUG things
 */

PCP_EXTERN int32 pcp_debug;
/*
 * debug defines: set via environment "PCP_DEBUG"
 */
#define PCP_DBG_OPEN        0x0001
#define PCP_DBG_SEND        0x0002
#define PCP_DBG_RECEIVE     0x0004

PCP_EXTERN int32 pcp_xdr_debug;

    /* errors */
#define PCP_XDR_DBG_USUAL_ERRORS    0x0001  /* expected from time to time */
#define PCP_XDR_DBG_BAD_ERRORS      0x0002  /* should never happen XXX NYD */

    /* Protocol info */
#define PCP_XDR_DBG_PROTOCOL        0x0004  /* Debug protocol negotiation */

    /* top level xdr of a transaction */
#define PCP_XDR_DBG_OP_ENCODE       0x0010
#define PCP_XDR_DBG_OP_DECODE       0x0020
#define PCP_XDR_DBG_OP_FREE         0x0040
    /* Read and write */
#define PCP_XDR_DBG_INDATA          0x10000 /* print incoming data */
#define PCP_XDR_DBG_OUTDATA         0x20000 /* print outgoing data */
#define PCP_XDR_DBG_READ            0x40000 /* info about reading wire */
#define PCP_XDR_DBG_WRITE           0x80000 /* info about writing to wire */
    /* xdr */
#define PCP_XDR_DBG_XDR_X_L         0x01000000  /* xdr_x_*long */
#define PCP_XDR_DBG_XDR_X_B         0x02000000  /* xdr_x_*bytes */
#define PCP_XDR_DBG_XDR_X_GF        0x04000000  /* for xdrpcp_x_getfile */
#define PCP_XDR_DBG_XDR_X_PF        0x08000000  /* for xdrpcp_x_putfile */


/************************************************************************
 * Define structure of a PCP Transaction and its components.
 *
 * Each transaction consists of a list of operations.
 * Each operation consists of an operator and a list of operands.
 * Each operand is field number/value pair.
 ************************************************************************/

/*
 * Define an operation on a list of fields. Not all operations use
 * the poid and/or field list, so they may be null. Both requests
 * and responses fit into this structure.
 */
typedef int32 pcp_opcode_flags_t;        /* the pcp opcode flags type */

#define PCP_FLG_REQUEST     0x0001
#define PCP_FLG_RESPONSE    0x0002
#define PCP_FLG_TRANS_OPEN  0x0004  /* part of open transaction */
                                    /* (else single operation) */
#define PCP_FLG_TRANS_RW    0x0008  /* trans is read-write */
#define PCP_FLG_TRANS_LOCK  0x0010  /* trans is also locking start */
#define PCP_FLG_TRANS_CALC_ONLY   0x0020  /* trans is calc only */
                                    /* (must be r/w too) */
#define PCP_FLG_FROM_CM     0x0100  /* mark as coming from a CM (to DM) */
#define PCP_FLG_FROM_TIMOS  0x0200  /* mark as coming from a TIMOS (to DM) */
#define PCP_FLG_FROM_DM     0x0400  /* mark as coming from a DM (to TIMOS) */
#define PCP_FLG_FROM_MASK   0x0700  /* mask to get PCP_FLG_FROM_xxx flag value */

typedef struct pcp_op {
    pin_opcode_t         opcode;      /* PCM_OP_xxx */
    pin_opcode_flags_t   opflags;     /* PCM_OPFLG_xxx */
    pcp_opcode_flags_t   pcp_flags;   /* PCP_FLG_xxx (ie internal stuff) */
    pcp_fld_list_t*      flds;        /* field list */
} pcp_op_t;

/*
 * Performance testing shows that setting the individual
 * values is actually faster than using memset.
 */
#define PCP_OP_T_CLEAR(ptr)                         \
	(ptr)->opcode = 0;                              \
	(ptr)->opflags = 0;                             \
	(ptr)->pcp_flags = 0;                           \
	(ptr)->flds = NULL;

#define PCP_OPSTAT_NONE         0  /* no operation outstanding */
#define PCP_OPSTAT_SENDING      1  /* sending an operation */
#define PCP_OPSTAT_SENT         2  /* sent an operation */
#define PCP_OPSTAT_RECEIVING    3  /* receiving reply to operation */
#define PCP_OPSTAT_FINI         4  /* operation done, ret_flist avail */

#define PCP_CANSTAT_NONE        0  /* no cancellation in progress */
#define PCP_CANSTAT_CANCEL      1  /* cancellation requested */
#define PCP_CANSTAT_SENT        2  /* cancellation sent out */
#define PCP_CANSTAT_FINI        3  /* cancellation done */


#define PCP_FWDSTAT_NONE                0
#define PCP_FWDSTAT_CONNECTED           1
#define PCP_FWDSTAT_TIMEWAIT_RETRY      2
#define PCP_FWDSTAT_TIMEWAIT_NORETRY    3

/************************************************************************
 * Structure to represent a communication channel to upper layers.
 ************************************************************************/
/* note: these two enums moved out, need to use C++ compiler on some stuff... ?? */
typedef enum pcp_crypt_type {  /* encryption type */
    CRYPT_TYPE_INVALID  = 0,
    CRYPT_TYPE_NONE     = 1
} pcp_crypt_type_t;

typedef enum pcp_sig_type {  /* signature type */
    SIG_TYPE_INVALID    = 0,
    SIG_TYPE_NONE       = 1
} pcp_sig_type_t;

typedef struct pcp_context {
    int32 is_using_given_poid_db_number;
    struct pcp_context*  fwd;               /* for NCP's useage only */
    struct pcp_context*  bak;
    pcp_open_flags_t     open_flag;         /* save from pcp_open() */
    int     fd;                             /* file/socket descriptor */
    enum    pcp_transport_type {            /* transport type to be used */
            USE_SOCKET,                     /* use BSD like sockets */
            USE_TLI,                        /* use SysV tli */
            USE_PIPE                        /* pipes (for NAP <-> NCP) */
            /* XXX selections for the various PC/Mac stuff? */
    } transport_type;
    caddr_t              transport_info;    /* transport info pointer: */
    int32                transport_infolen;
        /* for SOCKET: (struct sockaddr_in *) AF_INET,<IPaddr>,<port> */
        /* for TLI: (<some structure *>) */
    real_poid_t*         user;              /* user id (to send) */
    real_poid_t*         user_in;           /* user id, received off wire */
    int32                proto_vers;        /* protocol version. This is
					     * written into the wire header. 
					     * 0 means use 4k chunks
					     * 1 means use 16k chunks
					     */
    char                 session_id;        /* [char] session id */
    long                 login_session_id;  /* [long] login session id */
    pcp_crypt_type_t     crypt_type;        /* encryption type */
    int32                salt;              /* for crypt */
    caddr_t              crypt_data;        /* encryption data (session key, etc.) */
    pcp_sig_type_t       sig_type;          /* signature type */
    caddr_t              sig_data;          /* signature data */
    int16                sig_pad;           /* pad for sig/crypt, >= 8, pwr of 2 */
    int16                sig_size;          /* sizeof signature data on wire */
    int16                header_len;        /* size of packet header */
/* XXX *** retry info */
/* XXX *** timeout info */
    int16                max_chunk;         /* biggest chunk to send this link */
    int32                trans_state;       /* (sending) state of transaction */
#define PCP_TRANS_NONE           0   /* none outstanding */
#define PCP_TRANS_FIRST          1   /* 1st op in transaction */
#define PCP_TRANS_OPEN           2   /* 2nd, ... in transaction */
#define PCP_TRANS_COMMIT         3   /* commit in progress */
#define PCP_TRANS_ABORT          4   /* abort in progress */
#define PCP_TRANS_ONLY           5   /* solo operation in progress */
#define PCP_TRANS_TRANS_FLAGS  0x7  /* mask for all above */

    int32                trans_type;        /* type of transaction */
#define PCP_TRANS_RO		0
#define PCP_TRANS_RW		8  /* avoid bits of trans_state */
#define PCP_TRANS_LOCK  	0x10  /* *with* PCP_TRANS_RW only */
#define PCP_TRANS_CALC_ONLY	0x20
    pcp_fld_list_t*      trans_flist;       /* XXX (for now) list of trans poids*/

/* XXX */
    int32                r_status;          /* status of read side (errno) */
    int32                w_status;          /* status of write side (errno) */
/* XXX some state variable so that nobody can disconnect in the middle unknowingly */

    /*
     * if this function is non-null, it is invoked by pcp_send(), et.al.
     * to do stuff like connect to the right DM (if used inside CM).
     */
    void (*pre_write)();
    pin_db_no_t          current_dm;        /* for CM dm context(s) */
    char                 current_em[16];    /* for CM em context(s) */

    /*
     * state xdr - XXX FOR NOW, stash a single copy of 
     * pcpxdr stuff.  FUTURE: array per operation/session ??
     */
    void*                v_pcpxdr_p;
    pcm_os_cbi_t*        callback_info_p;
    pcm_os_cbi_t*        save_callback_info_p;  /* save over BLOCKING calls */
#if defined(unix)
    void (*old_sigio_hndlr)();  /* for pcp_op_set_callback() */
#endif
    int32                op_status;         /* PCP_OPSTAT_xxx - async op status */
    int32                can_status;        /* PCP_CANSTAT_xxx - cancel status */
    real_poid_t*         session_obj;       /* session from login */
    pcp_fld_list_t*      login_flist;       /* copy of login flist for reconnect */
    int32                reconnect_retries; /* pcp_send() retry counter */
    int32                doing_connect;     /* non-recursion flag pcp_connect() */
    pcp_fld_list_t*      user_trans_flistp; /* for user transaction ids */
    int32                fwd_status;        /* PCP_FWDSTAT_xxx - main CM status */
    pcp_fld_list_t*      gs_in_flistp;      /* global search in flist */
    pcp_fld_list_t*      gs_out_flistp;     /* global search out flist */
    char*                cfg_str;           /* host/port info for this connection from reading pin.conf */
    int32                pcm_op_max_retries;          /* maximum op retries */
    int32                pcm_op_timeout_max_retries;  /* maximum op timeout retries */
    int32                pcm_connect_max_retries;     /* maximum connection retries */
    int32                pcm_reconnect_max_retries;   /* maximum reconnection retries (replaced PCP_MAX_RECONNECT_RETRIES) */
    int32                pcm_timeout_in_msecs;        /* op timeout in milli-seconds */
    int32                pcna_proto;        /* AF_INET, AF_INET6, AF_UNIX, ...  */
    int32                pcm_connect_retry_delay_time_in_secs;  /* delay time in secs for the 
                                                                 * next retry to this connection 
                                                                 * if failed
                                                                 */
    int32                pcm_reconnect;	    /* reconnect logic: first (0) or next (1) */

    pcp_timeout_mode_t	timeout_mode; /* flag to indicate dual timeout mode */
    time_t		suspect_time; /* time at which context became suspect */
    int32		pcm_suspect_timeout_in_msecs; /* dual short timeout */

    /* 
     * Timeout for directory server request 
     * This timeout if present overrides pcm_timeout_in_msecs
     * for directory server request only.  
     */
    int32		pcm_directory_server_timeout_in_msecs; /* directory server long timeout */

    /* enum to keep track of the current connection state.  When going 
     * through suspect, recovery, the state must first pass through PENDING
     * state
     */
    enum pcp_connection_state {
		PCP_CTX_READY    = 0,	/* ready to connect/send */
		PCP_CTX_SUSPECT  = 1,	/* ctx is in suspect state */
		PCP_CTX_RECOVER  = 2,	/* ctx recovery */
		PCP_CTX_FAILOVER = 3	/* ctx failover */
    } connection_state;

    /* flag to tell whether recovery/failover needs to wait for trans
     * to finish */
    int32		wait_on_client_trans;
    /* override flag to use during lock_obj to disable dual timeout */
    int32		timeout_lock_override;

    /* holds lock type state while an op is in progress in the CM */
    int32		lock_type;

    /* For SOX : Auto reconnect flag */
    int32 pcm_auto_reconnect;
	
#if defined(INCL_SSL_) 
#if defined(FOR_CM) || defined(FOR_PORTAL) || defined(FOR_DM) || defined(FOR_QM)
    /* SSL context */
    struct pin_ssl_ctxt  *psc;
#endif
#endif

    /* interval at which reconnect should be tried */
    int32 refresh_interval; 

    /* interval after which pcp_reconnect should be called for STREAM_IO/STREAM_EOF/DM/EM_CONNECT_FAILED */
    int32 pcp_reconnect_delay_on_socket_error; 

    /* interval after which pcp_connect to be retried if there is an error */
    int32 pcp_connect_retry_delay_on_error;

    /* time at which connect or reconnect happened */
    time_t start_time;		
    int32 connect_timeout_in_msecs;
    char         *conf_role;
    int32	 pcm_opcode_timeout_in_usecs;
    /* guaranteed timeout per opcode */
} pcp_context_t;

/* defines for pcpxdr_create() "mode", ? really an enum but... */
#define PCPXDR_BLOCKING        1
#define PCPXDR_NON_BLOCKING    2
/* ?? PCPXDR_CM_BUFS ?? */

/* private enum - no FREE! */
enum pcpxdr_op {
    PCPXDR_ENCODE = 0,
    PCPXDR_DECODE = 1
};

/* how many times to retry a "first" pcp_send() */
#define PCP_MAX_RECONNECT_RETRIES    3

/* By default, our chunk size is 4k. However, we support chunk sizes that
 * are 16k too.
 */
#define PCP_DEFAULT_MAX_CHUNK 4 * 1024
#define PCP_LARGEST_MAX_CHUNK 16 * 1024	

/** Protocol versions for pcp */
#define PCP_PROTOCOL_DEFAULT 0
#define PCP_PROTOCOL_CAN_DO_LARGE_CHUNK  0x0001
#define PCP_PROTOCOL_SENDING_LARGE_CHUNK 0x0002	

/*
 * Functions in the PCP API.
 */
#if defined(__STDC__) || defined(PIN_USE_ANSI_HDRS) || defined(__cplusplus)
PCP_EXTERN void pcp_connect(
		pcp_context_t*	     ctxp,
		pin_errbuf_t*	     ebufp);

PCP_EXTERN void pcp_open(
                pcp_context_t**      ctxpp,
                pcp_open_flags_t     flags,
                pcp_fld_list_t*      flistp,
                int*                 sockp,
                pin_errbuf_t*        ebufp);

PCP_EXTERN void pcp_close(
                pcp_context_t*       ctxp,
                int32                how,
                pin_errbuf_t*        ebufp);

PCP_EXTERN void pcp_send(
                pcp_context_t*       ctxp,
                pcp_op_t*            op_p,
                int32                how,
                pin_errbuf_t*        ebufp);

PCP_EXTERN void pcp_receive(
                pcp_context_t*       ctxp,
                pcp_op_t*            op_p,
                poid_t*              userp,
                pin_errbuf_t*        ebufp);

PCP_EXTERN void pcp_receive_no_trans_cleanup(
                pcp_context_t*       ctxp,
                pcp_op_t*            op_p,
                poid_t*              userp,
                pin_errbuf_t*        ebufp);

PCP_EXTERN void pcp_verify_suspect_context(
		pcp_context_t*       ctxp,
		int32                nowaitflg,
		pin_errbuf_t*        ebufp);

PCP_EXTERN int32 pcp_is_dual_timeout_mode(
		pcp_context_t*       pxp);

PCP_EXTERN void pcp_set_async(
                pcp_context_t*       ctxp,
                pcm_os_cbi_t*        callback_info_p,
                int*                 sdp,
                pin_errbuf_t*        ebufp);

PCP_EXTERN int connect_with_timeout(
		int sockfd, 
		const struct sockaddr *addr,
     		socklen_t addrlen, 
		int connect_timeout_in_ms);

#if defined(INCL_SSL_) 
#if defined(FOR_CM) || defined(FOR_PORTAL)|| defined(FOR_DM) || defined(FOR_QM)
PCP_EXTERN void pcp_set_multithread();
PCP_EXTERN void pcp_set_SSL_config(int useSSL,int _2way,char *cipher_strp,int cipher_strp_len,
				   char *wallet_loc,int wallet_loc_Len);
#endif
#endif

PCP_EXTERN void pcp_op_done(
                pcp_context_t*       ctxp,
                pin_errbuf_t*        ebufp);

PCP_EXTERN void pcp_op_get(
                pcp_context_t*       ctxp,
                pcp_op_t*            op_p,
                poid_t*              userp,
                pin_errbuf_t*        ebufp);

PCP_EXTERN poid_t* pcp_get_userid(
                pcp_context_t*       ctxp);

PCP_EXTERN poid_t* pcp_get_session(
                pcp_context_t*       ctxp);

PCP_EXTERN pin_flist_t* pcp_get_trans_flist(
                pcp_context_t*       ctxp);

PCP_EXTERN pin_flist_t* pcp_get_trans_flist_ref(
                pcp_context_t*       ctxp);

PCP_EXTERN int64 pcp_get_current_db_no(
	        pcp_context_t*       ctxp);

PCP_EXTERN int pcp_is_trans_locked(
                pcp_context_t*       ctxp);

PCP_EXTERN int pcp_trans_contains_poid(
                pcp_context_t*       ctxp,
                poid_t*              pdp,
		int32		     flags,
                pin_errbuf_t*        ebufp);

PCP_EXTERN int pcp_trans_search_and_add(
                pcp_context_t*       ctxp,
                poid_t*              pdp,
		int32		     flags,
                pin_errbuf_t*        ebufp);

PCP_EXTERN void pcp_trans_add_poid(
                pcp_context_t*       ctxp,
                poid_t*              a_pdp,
		int32		     flags,
                pin_errbuf_t*        ebufp);

PCP_EXTERN void pcp_promote_trans_to_locked(
                pcp_context_t*       ctxp);

PCP_EXTERN void pcp_encrypt_dosig(
                pcp_context_t*       ctxp,
                caddr_t              buf,
                int32                len,
                pin_err_t*           errp);

PCP_EXTERN void pcp_decrypt_chksig(
                pcp_context_t*       ctxp,
                caddr_t              buf,
                int32                len,
                pin_err_t*           errp);

PCP_EXTERN void _pcp_readwire(
                pcp_context_t*       ctxp,
                caddr_t              buf,
                int32                needed,
                int32*               got,
                int32                once,
                pin_err_t*           errp);

PCP_EXTERN int _pcp_cmp_userid(
                real_poid_t*         u1,
                real_poid_t*         u2);

PCP_EXTERN void pcp_trans_cleanup(
                pcp_context_t*       ctxp,
                pcp_op_t*            op_p);

PCP_EXTERN void pcp_do_login(
                pcp_context_t*       ctxp,
                pin_errbuf_t*        ebufp);

PCP_EXTERN void pcp_trans_open(
                pcp_context_t*       ctxp,
                pcp_op_t*            reqp,
                pin_errbuf_t*        ebufp);

PCP_EXTERN void pcp_trans_doit(
                pcp_context_t*       ctxp,
                pcp_op_t*            reqp,
                pin_errbuf_t*        ebufp);

#else

PCP_EXTERN void pcp_open();
PCP_EXTERN void pcp_close();
PCP_EXTERN void pcp_send();
PCP_EXTERN void pcp_receive();
PCP_EXTERN void pcp_set_async();
PCP_EXTERN int connect_with_timeout();
#if defined(INCL_SSL_) 
#if defined(FOR_CM) || defined(FOR_PORTAL) || defined(FOR_DM) || defined(FOR_QM)
PCP_EXTERN void pcp_set_multithread();
#endif
#endif
PCP_EXTERN void pcp_op_done();
PCP_EXTERN void pcp_op_get();
PCP_EXTERN poid_t* pcp_get_userid();
PCP_EXTERN poid_t* pcp_get_session();
PCP_EXTERN pin_flist_t* pcp_get_trans_flist();
PCP_EXTERN int64 pcp_get_current_db_no();
PCP_EXTERN int  pcp_is_trans_locked();
PCP_EXTERN int pcp_trans_contains_poid();
PCP_EXTERN void pcp_trans_add_poid();
PCP_EXTERN void pcp_promote_trans_to_locked();

PCP_EXTERN void pcp_encrypt_dosig();
PCP_EXTERN void pcp_decrypt_chksig();

PCP_EXTERN void _pcp_readwire();
PCP_EXTERN int _pcp_cmp_userid();

PCP_EXTERN void pcp_trans_cleanup();
PCP_EXTERN void pcp_do_login();
PCP_EXTERN void pcp_trans_open();
PCP_EXTERN void pcp_trans_doit();
PCP_EXTERN void pcp_verify_suspect_context();
PCP_EXTERN int32 pcp_is_dual_timeout_mode();

#endif	/* ! (defined(__STDC__) || defined(PIN_USE_ANSI_HDRS)) */
/*
 * defines for chunk flags.  (transaction flags defined above in ctx)
 * they are defined this way to facilitate packing in the wireheader
 * (defined in pcp_xdr.c).
 */
#define CHUNK_ONLY      0x0  /* xx00 */
#define CHUNK_FIRST     0x1  /* xx01 */
#define CHUNK_MIDDLE    0x2  /* xx10 */
#define CHUNK_LAST      0x3  /* xx11 */

#define session_id_of_sfs(sfs)    (((sfs) >> 8) & 0xFF)

#define CHUNK_MASK      0x3  /* xx11 */
#define TRANS_MASK      0xF0 /* 111100xx */
#define TRANS_SHIFT     4    /* << 4 */
#define chunk_flags_of_sfs(sfs)    ((sfs) & CHUNK_MASK)
#define trans_flags_of_sfs(sfs)    (((sfs) & TRANS_MASK) >> TRANS_SHIFT)


/*
 * for pcp login PIN_FLD_RESULT (an "enum")
 */
#define PCP_LOGIN_RESULT_OK         0   /* logged in OK */
#define PCP_LOGIN_RESULT_REDIRECT   1   /* redirect to another CM */
#define PCP_LOGIN_RESULT_BAD        2   /* generally bad attempt */
#define PCP_LOGIN_RESULT_BAD_DM     3   /* BAD Connection to DM */

/* 
 * private opflags to be used only between infranet system processes
 */
#define PCM_OPFLG_SUSPEND_CREDENTIALS    0x80000000 

/*
 * some flags used to say where a poid is in a transaction
 */
#define PCP_TRANS_POID_IS_LOCKED     2  /* Poid is locked */
#define PCP_TRANS_POID_IN_TRANS      1  /* Poid is used in trans,
                                         * but might not be locked */
#define PCP_TRANS_POID_NOT_IN_TRANS  0  /* Poid not found in trans */

#define PCP_MAX_PORT_OR_SERV_SIZE 11
#define PCP_MAX_HOST_NAME_SIZE 201

#ifndef WINDOWS
  #define _GET_LU32(var64) ((int32)var64)
#else
  #define _GET_LU32(var64) ((&var64)[1])
#endif

#ifdef __cplusplus
}
#endif

#endif /* !_PCP_H */

