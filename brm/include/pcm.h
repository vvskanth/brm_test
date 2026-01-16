/*
 * @(#)$Id: pcm.h BRM_12.0.0.0.9_PORTALBASE_GENERIC/2 2023/07/19 09:15:50 visheora Exp $
 *
 * Copyright (c) 1996, 2023, Oracle and/or its affiliates.
 *
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */


/*
 * This file contains definitions for the PCM API.
 */

#ifndef _PCM_H
#define	_PCM_H

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#ifndef _PIN_TYPE_H_
  #include "pin_type.h"
#endif
/*** "pin_flds.h" *** must be AFTER PIN_MAKE_FLD() below ***/
#ifndef _PCM_OPS_H
  #include "pcm_ops.h"
#endif
#ifndef _PIN_ERRS_H
  #include "pin_errs.h"
#endif
#ifndef _pin_sys_h_
  #include "pin_sys.h"
#endif
#ifndef _PBO_DECIMAL_H
  #include "pbo_decimal.h"
#endif
#ifndef _pin_os_time_h_
  #include "pin_os_time.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Setup the external declaration stuff correctly */

#ifdef WINDOWS
  #if defined(PCM_DLL) || defined(_PCM_)
    #define EXTERN          __declspec(dllexport)
  #else
    #ifdef _NO_DLLIMPORT
      #define EXTERN        extern
    #else
      #define EXTERN        __declspec(dllimport)
    #endif /* _NO_DLLIMPORT */
  #endif /* PCM_DLL */
#else 
  #define EXTERN            extern
#endif /* defined(WINDOWS) */

/* Allow customizing the file name */
#ifndef PIN_FILE_SOURCE_ID
  #define PIN_FILE_SOURCE_ID __FILE__
#endif

/************************************************************************
 * Definitions related to pcm operations.
 ************************************************************************/
/*
 * Enumeration of fundamental field types. Not a formal enum because
 * it's stored in a short.
 */
#define PIN_FLDT_UNUSED     0
#define PIN_FLDT_INT        1
#define PIN_FLDT_UINT       2   /* OBSOLETE */
#define PIN_FLDT_ENUM       3
#define	PIN_FLDT_NUM        4   /* OBSOLETE */
#define PIN_FLDT_STR        5
#define PIN_FLDT_BUF        6
#define PIN_FLDT_POID       7
#define PIN_FLDT_TSTAMP     8
#define PIN_FLDT_ARRAY      9   /* array element */
#define PIN_FLDT_SUBSTRUCT  10  /* sub-type substructure */
#define	PIN_FLDT_OBJ        11  /* whole object */
#define PIN_FLDT_BINSTR     12  /* (short) binary string data */
#define PIN_FLDT_ERR        13
#define PIN_FLDT_DECIMAL    14
#define PIN_FLDT_TIME       15
#define PIN_FLDT_TEXTBUF    16
#define PIN_FLDT_ERRBUF     PIN_FLDT_ERR
#define PIN_FLDT_INT64      17

#define PIN_FLDT_LAST       17


/*
 * new fields have the type embedded in them, use this macro when
 * defining them, for example:
 * #define PIN_FLD_MY_NEW_ONE	PIN_MAKE_FLD(PIN_FLDT_STR, 999)
 */
#define PIN_MAKE_FLD(type, val)	((((type)&0xFF)<<24)|((val)&0xFFFFFF))

#define PIN_GET_TYPE_FROM_FLD(num)	(pin_fld_type_t)(((num) & 0xFF000000) >> 24)
#define PIN_GET_NUM_FROM_FLD(num)	(pin_fld_num_t)(((num) & 0x00FFFFFF))

/*
 * Get schema number from a given database number where a database number
 * is formed by N.logical_partition_no.M.schema_no
 */
#define PIN_GET_SCHEMA_NO(db_no)	(db_no & 0x000000FF)

/*
 * Get the part 2 number from a given database number where a database number
 * is formed by N.logical_partition_no.M.schema_no
 * and part 2 number is the M
 */
#define PIN_GET_PART2_NO(db_no)	((db_no >> (1*16)) & 0x000000FF)

/*
 * Get logical partition number from a given database number where a database number
 * is formed by N.logical_partition_no.M.schema_no
 */
#define PIN_GET_LOGICAL_PARTITION_NO(db_no)	((db_no >> (2*16)) & 0x000000FF)
/*
 * Get the part 4 number from a given database number where a database number
 * is formed by N.logical_partition_no.M.schema_no
 * and part 4 number is the N
 */
#define PIN_GET_PART4_NO(db_no)	((db_no >> (3*16)) & 0x000000FF)

/*
 * Create database number based on a schema_number and a logical partition number
 * A database number is formed by N.logical_partition_no.M.schema_no
 */
#define PIN_CREATE_DATABASE_NO(schema_no, logical_partition_no)	((logical_partition_no << (2*16)) + schema_no)

/*
 * Print db number to stderr with the following format
 * N.logical_partition_no.M.schema_no
 */
#define PIN_STDERR_DATABASE_NO(db_no)	fprintf(stderr, "db_no:%" I64_PRINTF_PATTERN "d\t%d.%d.%d.%d\n", \
                         db_no, \
			 (int) ((db_no >> (3*16)) & 0x000000FF), \
			 (int) PIN_GET_LOGICAL_PARTITION_NO(db_no), \
			 (int) ((db_no >> (1*16)) & (0x000000FF)), \
			 (int) PIN_GET_SCHEMA_NO(db_no)) 

#ifndef _PIN_FLDS_H
  #include <pin_flds.h>
#endif

/*
 * The poid.
 * This identifier for an object is unique forever in the whole universe.
 * NOTE these are opaque to the PCM API.
 */
typedef void poid_t;
#define PCM_MAX_POID_TYPE	255	/* max length of poid type string */

/*
 * Poid list specifics.
 */
typedef char poid_list_t;

/*
 * put the canonical output of a poid database into the buffer.
 * NOTE the buffer must be >= PIN_POID_PRINT_DB_BUFSIZE bytes big.
 * moved here from pin_poid_pr.c
 */
/* (4 * "ddddd.") + "\0" = 25 */
#define PIN_POID_PRINT_DB_BUFSIZE       32

/*
 * Define a time-stamp
 */
typedef time_t    pin_fld_tstamp_t;

/*
 * the cookie is used by get_next/take_next to keep track of where
 * it is in the flist.  N.B. do not "take" or "drop" things while
 * in a take_next/get_next loop - or the cookie will become meaningless.
 */
typedef caddr_t* pin_cookie_t;		/* for get/take next() */

/* max string length for "PIN_FLDT_STR" types */
/* (some may be even shorter than this, but varchar2 in db has max 4000) */
#define PCM_MAXIMUM_STR	32000

/*
 * data buffer.
 */
typedef struct pin_buf {
	int32		flag;		/* if XBUF, ... */
	int32		size;		/* size of data */
	int32		offset;		/* offset (for read) */
	caddr_t		data;		/* pointer to data (BUF) */
	char		*xbuf_file;	/* ptr to filename for XBUF */
} pin_buf_t;

#define PCM_BUF_FLAG_XBUF	0x0001	/* users wants data from/to a file... */
#define PCM_BUF_FLAG_XBUF_READ	0x0002	/* if XBUF, encode filename, not data */
#define PCM_BUF_XBUF_MAX_SIZE   2147483648 /* Maximum file size for XBUF type - 2GB */

/*
 * short binary data buffer (for PIN_FLDT_BINSTR)
 */
typedef struct pin_binstr {
    unsigned short size;
    char*          data;
} pin_binstr_t;

#define	PCM_BINSTR_MAX	255	/* max size of pin_binstr_t's data */



/*
 * defines for PIN_ERR_SET_LEVEL, PIN_ERR_LOG_* macros
 */
#define	PIN_ERR_LEVEL_NONE	0
#define	PIN_ERR_LEVEL_ERROR	1
#define	PIN_ERR_LEVEL_WARNING	2
#define	PIN_ERR_LEVEL_DEBUG	3
#define PIN_ERR_LEVEL_CLIENT_CONTEXT 0x7fffffff


typedef enum pin_enum {X,Y} pin_enum_t;

/*
 * Flags for flist element flag
 */
#define	PCM_FLDFLG_TYPE_ONLY	0x0002	/* use in SEARCH_ARGS only: */
					/* "only compare poid type" */
#define	PCM_FLDFLG_NO_QUOTE	0x0004	/* use in SEARCH_ARGS only: */
					/* "do not quote strings" */
#define	PCM_FLDFLG_WRWNP	0x0008	/* use in read_field cache : */
					/* "read with null ptr" INTERNAL USE ONLY! */
#define PCM_FLDFLG_FIFO		0x0100  /* use in pin_poid_list_add : */
					/* Removes first in the list in case of overflow */
#define	PCM_FLDFLG_CMPREV	0x0010	/* use in pin_poid_list_compare : */
					/* "compare poid with revision" */
#define PCM_FLDFLG_UNICODE   0x0020 /* For internal DM use only -  */
					/* Is set if the already UNICODE converted */
#define PCM_FLDFLG_ENCRYPTED 0x0040 /* For internal DM use only -  */
					/* Is set if the already ENCRYPTED */

#define PCM_FLDFLG_FLIST_HEAP 0x0100 /* For pin_flist use only -  */
					/* If set then memory is from local heap */
#define PCM_FLDFLG_DO_ROBJ 0x0200   /* For internal DM use only - */
                /*
                 * Is set if this part of the flist needs to be read in
                 * its entirety (for search
                 */
#define	PCM_FLDFLG_DB_ONLY	0x0400	/* use in SEARCH_ARGS only: */
					/* "only compare POID DB NO" */
#define	PCM_FLDFLG_ID_ONLY	0x0800	/* use in SEARCH_ARGS only: */
					/* "only compare POID ID" */
#define	PCM_FLDFLG_REV_ONLY	0x1000	/* use in SEARCH_ARGS only: */
					/* "only compare POID REVISION" */

/*
 * defines for record_id
 */
#define	PCM_RECID_ALL	0xFFFFFFFF	/* any record, n.a. */
#define	PCM_RECID_ASSIGN 0xFFFFFFFe	/* (on write) create new unique recid */
#define	PCM_RECID_MAX	0xFFFF0000	/* leave 64K reserved at top */

#define PIN_ELEMID_ANY		PCM_RECID_ALL
#define PIN_ELEMID_ASSIGN	PCM_RECID_ASSIGN
#define PIN_ELEMID_MAX		PCM_RECID_MAX

/*
 * Macro which tests for a record id in the reserved range.
 */
#define	PCM_RECID_IS_RESERVED(recid)					\
	(((recid) & PCM_RECID_MAX) == PCM_RECID_MAX)

/*
 * defines for PIN_FLD_SEARCH_FLAGS
 * NOTE: keep in sync from init_tables.source (see there before adding)
 */
#define SRCH_CALC_ONLY          0xFF    /* any SRCH_CALC_ONLY_n */
/******/
#define SRCH_CALC_ONLY_1        1       /* one result */
#define SRCH_DISTINCT           256
#define SRCH_EXACT           	512
#define SRCH_WITHOUT_POID       1024
#define SRCH_UNION_TT           2048
#define SRCH_ACCURATE           4096
#define SRCH_MULTI_OBJ_PRUNING  8192


/*
 * This defines the directions of the search links. Included in pcm.h because
 * they are applicable outside of the dm.
 */
#define DM_LINK_OBJ_RIGHT_TO_LEFT 1
#define DM_LINK_OBJ_LEFT_TO_RIGHT -1

/************************************************************************
 * Miscellaneous definitions.						*
 ************************************************************************/

/******************************************************
 * default start size for the list of fld pointers
 * TODO, have discussed a smarter scheme for Momentum
 * with JP.
 ******************************************************/

#define PCM_FLIST_DEFAULT_SIZE	20

/* FLISTHEAP heap comes from MEMPOOL, so may be ok to have larger
 * initial size?  Definitely should have larger extension size
 * since it goes not grow 2x each time
 */

#define PCM_FLIST_HEAP_SIZE           256  /* initial heap in flist hdr */
#define PCM_FLIST_HEAP_DEFAULT_SIZE   1024 /* heap extension size */

/**********************************************************************
 * Generic macro to return a rounded up size to the nearest multiple
 * of the current platform word size, based on size of int64 being
 * the word size, in other words, we're rounding up to 64 bit 
 * pointer sizes, for all platforms.  There is probably a better way
 * to handle this, I don't know what it is.
 **********************************************************************/
#define PIN_WORD_SIZE	sizeof(int64)
#define PIN_ROUNDUP_BY_WORD(size) \
	(int32)( (size+PIN_WORD_SIZE-1) & ~(PIN_WORD_SIZE-1) )

/* maximum and minimum values for a PIN_FLD_NUM */
#define PIN_NUM_MAX	(+1E+100)
#define PIN_NUM_MIN	(-1E+100)

/* value for max precision for decimal type */
#define PIN_MAX_SCALE	15

/* generic "enum" value for PIN_FLD_TYPE */
#define PIN_TYPE_NO	0
#define PIN_TYPE_YES	1

/* generic "enum" value for PIN_FLD_BOOLEAN */
#define PIN_BOOLEAN_FALSE	0
#define PIN_BOOLEAN_TRUE	1

/* generic "enum" value for PIN_FLD_RESULT */
#define PIN_RESULT_FAIL		0
#define PIN_RESULT_PASS		1

/*
 * typedef for pcm_context_open()
 */
typedef void pcm_context_t;		/* real structure is opaque to pcm */

/*
 * flag for pcm_context_close() "how"
 */
/* socket - no logout done.  Useful if application forks after */
/* pcm_context_open() already done. */
#define PCM_CONTEXT_CLOSE_FD_ONLY	0x01	/* just close underlying */

/* Does not read substructs or arrays unless explicitly given */
#define PCM_OPFLG_NO_DESCEND	        0x0002  /* robj only */

/* Only goes to the CM */
#define PCM_OPFLG_CM_LOOPBACK	        0x0002  /* test_loopback only */

/*
 * user-settable (bit mask) flags for operations.
 *	used in the "flags" argument of pcm_op()
 * generic flags occupy lower 16 bits.
 */
/* return "meta" data only of any PIN_FLDT_BUF type fields */
#define	PCM_OPFLG_META_ONLY		0x0004	/* robj, rfld only */
/* Only goes to the EM */
#define PCM_OPFLG_EM_LOOPBACK	        0x0004  /* test_loopback only */

/* This flag is used to suppress the descrioption in the output flist. 
 * the opcode used here will be PCM_OP_GET_DD
 */
#define PCM_OPFLG_SUPPRESS_DESC       0x0008  /* Used for get_dd opcode only */

/* This flag is used to suppress standard fields in the output flist. 
 * the opcode used here will be PCM_OP_GET_DD
 */
#define PCM_OPFLG_CUST_FLDS_ONLY       0x8000  /* Used for get_dd opcode only */

/* robj, rflds: if set && rev == current, don't bother returning data */
#define	PCM_OPFLG_REV_CHECK		0x0008	/* check rev before doing op */
/* XXX spell out rest of behaviour */


/* This flag is used to set the poid rev number of object created to -1.
 * PCM_OPFLG_REV_CHECK is not used with create opcode.
 * Hence using the same value for PCM_OPFLG_SET_REV_DELETED
 */
#define PCM_OPFLG_SET_REV_DELETED       0x0008  /* Used for create only */

/* FM must provide loopback opcode impl */
#define PCM_OPFLG_FM_LOOPBACK	        0x0008  /* test_loopback only */

/* Only goes to TIMOS */
#define PCM_OPFLG_TIMOS_LOOPBACK        0x0010  /* test_loopback only */

/* Only goes to TIMOS */
#define PCM_OPFLG_TIMOS_PEER_LOOPBACK   0x0020  /* test_loopback only */

/* In TT environment - In order to read data from database we are using this flag */
#define PCM_OPFLG_READ_DB       0x2000000 

#define	PCM_OPFLG_COUNT_ONLY		0x0010	/* for SEARCH, only count */

/* 
 * PCM_OPFLG_NO_LOCK is used to skip the balance group locking feature  
 * during WRITE_FLDS. This flag must be used during a  write into any object
 * if it's associated balance group is non-existent.
 */
#define PCM_OPFLG_NO_LOCK             0x8000000

/*
 * allows adding an array element if needed during write fields or
 * increment fields (otherwise fails with missing element id)
 */
#define PCM_OPFLG_ADD_ENTRY		0x0020	/* wfld, incfld only */

/* uses the given id for created poid instead of calc'ing one */
/* Also encoding the given poid will not be done for partitioned objects */
/* note: this is sort of opposite from our usual approach since
 * by default we determine the id automatically and have to
 * be told not to. Usually, we don't do work unless told to.
 */
#define PCM_OPFLG_USE_POID_GIVEN	0x0040	/* create and robj for  */
	                                        /* for audit-trail only */

/*
 * This flag is valid for TT enabled CM. This flag 
 * requests CM to do a locator service to route this
 * TRANS_OPEN opcode to the right logical partition.
 */
#define PCM_OPFLG_USE_LOCATOR_SRVC		0x0040	/* for trans open only. */

/* return the would be results without changing the db */
#define PCM_OPFLG_CALC_ONLY		0x0080

/* return the whole object instead of just the poid */
#define PCM_OPFLG_READ_RESULT		0x0100

#define	PCM_OPFLG_SORT_DESC			0x0010	/* for ReadField only */

/* These two bits are re-used for rflds to specify if	*/
/* the array would be sorted by record_ID	*/
#define PCM_OPFLG_SORT_ASCE			0x0080	/* for ReadField only */

/*
 * normally write fields or increment fields returns the updated
 * poid rev or updated values (respectively).  This flag disables
 * that extra work for higher performance.
 */
#define PCM_OPFLG_NO_RESULTS		0x0200	/* wfld, incfld only */

/*
 * this PCM_OP_READ_FLDS result ought to be cached in the CM
 * (as a performance hint).
 */
#define PCM_OPFLG_CACHEABLE		0x0400	/* read_flds only */

/*
 * This flag disables audit processing for the poid 
 * passed with wflds. PCM_OPFLG_READ_UNCOMMITTED is not
 * used with wflds. Hence using the same value for PCM_OPFLG_SUPPRESS_AUDIT.
 */
#define PCM_OPFLG_SUPPRESS_AUDIT              0x0800  /* wflds only */

/*
 * This flag causes wflds to ignore error returned for the case where the ARRAY [*]
 * being written is not present in the database. For example, in the below case
 * if the array PIN_FLD_SUB_BALANCES is not present for the object /balance_group 48449147,
 * the DM would return a success message if PCM_OPFLG_IGNORE_ERR is passed with the opcode
 * PCM_OP_WRITE_FLDS.
 * 
 *	0 PIN_FLD_POID           POID [0] 0.0.0.1 /balance_group 48449147 2
 *	0 PIN_FLD_BALANCES      ARRAY [*] allocated 20, used 1
 *	1     PIN_FLD_SUB_BALANCES  ARRAY [*] allocated 20, used 2
 *	2         PIN_FLD_DELAYED_BAL  DECIMAL [0] 0
 *	2         PIN_FLD_NEXT_BAL     DECIMAL [0] 0
 */
#define PCM_OPFLG_IGNORE_ERR              0x1000  /* wflds only */

/*
 * The normal mode of reading data is read-committed. This flag
 * is specially invented for SQL Server to prevent blocking while
 * reading from a table if a concurrent transaction has issued an 
 * update to the same record or range of records.
 */
#define PCM_OPFLG_READ_UNCOMMITTED		0x0800	
				/* rfld, robj, step, search,  only */

#define PCM_OPFLG_READ_DELETED_FIELDS   0x200000   /* dflds */


/* 
 * Incflds must be called with this flag if a caller wants the NULL 
 * field value on the input f-list is used for the addition. Other 
 * wise, the caller does not want to replace the value in the database 
 * with a NULL.
 */
#define PCM_OPFLG_USE_NULL		0x1000	
			/* incflds only - overloaded with NO_DB_FILTER */

/*
 * This flag is specially invented for DNA
 * to do a search on all records in an amalgamated view.
 */
#define PCM_OPFLG_NO_DB_FILTER		0x1000	
				/* search,  only */

/* if any of these flags is specified in op_robj, a search 
 * is done in the audit-trail of the object. for POID_GIVEN,
 * the object matching the given poid, including rev, is 
 * returned or NO_FOUND error is returned. for POID_PREV, the
 * object with last rev < given rev is returned. POID_NEXT returns
 * the object with first rev > given rev. POID_NEAREST implies
 * POID_GIVEN and if not found then POID_PREV.
 */
#define PCM_OPFLG_USE_POID_NEAREST	0x8000	/* robj for audit-trail only */
#define PCM_OPFLG_USE_POID_PREV		0x2000	/* robj for audit-trail only */
#define PCM_OPFLG_USE_POID_NEXT		0x4000	/* robj for audit-trail only */

/*
 * used for the opcode PCM_OP_SEARCH onlyin Brand envronment.
 * This flag is set when a READ_FLDS, READ_OBJ (with LOCK_OBJ flag)
 * is converted to a PCM_OP_SEARCH. This flag is for internal use
 * only.
 */
#define PCM_OPFLG_BRAND_READ		0x2000

/* Used in MultiDB environment to tell DM to read from the local
 * database if this flag is set. Overriding residency_type setting
 * of the remote object.
 */
#define PCM_OPFLG_USE_ROUTING_DB_CACHE  0x10000

/* Used in DM to say "do select for update", ie lock those rows that
 * are read for future accesses
 * NOTE - For historical reasons, there are PCM_TRANS_OPEN_xx flag
 * names which have same function as OPFLG versions related to
 * object locking
 */
#define PCM_OPFLG_LOCK_OBJ		0x20000
#define PCM_TRANS_OPEN_LOCK_OBJ		PCM_OPFLG_LOCK_OBJ

#define PCM_OPFLG_LOCK_DEFAULT		0x400000
#define PCM_TRANS_OPEN_LOCK_DEFAULT	PCM_OPFLG_LOCK_DEFAULT

/* This flag value is strictly for BRM-internal use only. */
#define PCM_OPFLG_LOCK_NONE             0x4000000


/*
 * Used by CM to skip schema search and by DMTT to search Oracle DB
 * The use case is ACT_FIND for account lookup via search over /service
 */
#define PCM_OPFLG_SEARCH_DB             0x800000

/*
 * Used by DM to enforce schema search including all logical partitions
 */
#define PCM_OPFLG_SEARCH_PARTITIONS     0x10000000

/*
 * Used by DM to enforce local search
 */
#define PCM_OPFLG_SEARCH_ONE_PARTITION     0x20000000

/*
 * Used by CM to inform DMTT to execute the sproc on Oracle DB.
 */
#define PCM_OPFLG_EXEC_SPROC_ON_DB             0x800000

/** Used by timos; searches with this flag will be executed on the
 * SDS (or ROC) rather than on the database. Ignored by dm_oracle
 */
#define PCM_OPFLG_SEARCH_SDS           0x40000	

/** Used by rflds and robj, search; 
 * if set, the results will be sorted by array index 
 */
#define PCM_OPFLG_ORDER_BY_REC_ID           0x80000	

#define PCM_OPFLG_SRCH_CALC_RESULTS           0x100000

/*
 * flags for PCM_OP_TRANS_OPEN
 * as op specific flags, they got in high 16 bits.
 */
#define PCM_TRANS_OPEN_READONLY		0x00000
#define PCM_TRANS_OPEN_READWRITE	0x10000
#define PCM_TRANS_OPEN_GLOBALTRANSACTION        0x20000000
#define PCM_OPFLG_USE_GIVEN_POID_DB_NUMBER 0x40000000
/* flags for audit delete */
#define PCM_OPFLG_DELETE_AUDIT_OBJ 0x80000000

/*
 * Used by schedule create use exact time 
 */
#define PCM_OPFLG_USE_EXACT_TIME  	0x0080

/*
 * More (bit mask) flags for operations. OPFLG2 using pcm_op_ex
 */
#define	PCM_OPFLG2_BY_REF		0x00000001 /* don't copy input flist */

/*
 * Used by rating for not doing Credit Limit Check
 */
#define	PCM_OPFLG_RATE_WITH_DISCOUNTING 0x100000

/*
 * Used by balance group locking feature
 */


/*
 * flags for OP_SEARCH when called through dm_timos.
 * as op specific flags, they go in high 16 bits
 */
#define PCM_SEARCH_EXEC_ONTIMOS         0x10000

/*
 * flags for PIN_FLD_PLATFORM
 *
 */
#define PIN_FLD_PLATFORM_SOLARIS	1
#define PIN_FLD_PLATFORM_HP		2


/*
 * flags for PIN_FLIST_TO_XML
 *
 */
#define PIN_XML_TYPE					0x000001
#define PIN_XML_FLDNO					0x000002
#define PIN_XML_BY_TYPE					0x000004  /* default */
#define PIN_XML_BY_NAME					0x000008
#define PIN_XML_BY_SHORT_NAME				0x000010
#define PIN_XML_BY_SHORT_NAME_FOR_BUILTIN_FLDS		0x000020

/*
 * modes for PIN_FLIST_TO_STR/PIN_FLIST_TO_STR_COMPACT
 */
#define PIN_STR_MODE_DETAIL		0
#define PIN_STR_MODE_COMPACT		1
#define PIN_STR_MODE_BINARY             2
#define PIN_STR_MODE_MASK             	4

/*
 * define the OS specific callback stuff
 */
#if defined(unix) || defined(__unix)
  typedef struct pcm_os_cbi {
    void (*call_back_func)();    /* call this function... */
    int32    arg0_int;           /* with this as 1st arg... */
    void*    arg1_ptr;           /* and this as 2nd arg. */
  } pcm_os_cbi_t;
#elif defined(WINDOWS)
  typedef struct pcm_os_cbi {
    /*
     * this *was* "HWND cb_win;" but HWND requires windows.h. instead of
     * fixing 20 src files, case to/from HWND in the 2 places that use it.
     */
    void*    cb_win;            /* send to this "window"... */
    int16    cb_msg;            /* this message. */
    int16    cb_private;        /* for internal use only */
  } pcm_os_cbi_t;
#else
  "ERROR no OS defined for pcm_os_cbi"
#endif

/*
 * flags for CANONICALIZATION 
 * used by opcode PCM_OP_CUST_POL_CANONICALIZE  PIN_FLD_FLAGS field.
 * PCM_CANON_INFLAG_ALLOW_WCARDS - to allow wildcards like * or %
 * PCM_CANON_OUTFLAG_CANON_ESCAPE - for the escape character "_"
 */
#define PCM_CANON_INFLAG_ALLOW_WCARDS   1
#define PCM_CANON_OUTFLAG_CANON_ESCAPE  1

/***************************************************************************
 *	Type:		pin_locale_cat_t
 *	Applies To:	Server Internationalization ( Server I18N )
 *
 *	pin_locale_cat_t is an enumeration type that defines categories
 *	( or sets ) of locale strings:
 *
 *	1.  INFRANET_LOCALE is a platform independent description of a
 *	    locale.
 *	2.  WIN32_LOCALE is the Windows version of the corresponding
 *	    Infranet locale.
 *	3.  UNIX_LOCALE is the Unix version of the corresponding
 *	    Infranet locale.
 *	4.  PLATFORM_LOCALE is used to indicate that either a Win32 or
 *	    Unix locale is needed and the system should resolve the
 *	    choice based on the running environment.
 *
 *	There are 1:1 mappings between the locale sets.
 **************************************************************************/

typedef enum pin_locale_category
{
	INFRANET_LOCALE = 1313,
	PLATFORM_LOCALE = 1314,
	WIN32_LOCALE    = 1315,
	UNIX_LOCALE     = 1316,
	HPUX_LOCALE     = 1317,

	AIX_LOCALE      = 1318

} pin_locale_cat_t ;

/***************************************************************************
 *	The following are used for facility code to domain string
 *	mapping under Server I18N.
 **************************************************************************/

#define PIN_DOMAIN_ERRORS			0
#define PIN_DOMAIN_REASON_CREDIT		1
#define PIN_DOMAIN_REASON_DEBIT			2
#define PIN_DOMAIN_REASON_ACTIVE		3
#define PIN_DOMAIN_REASON_INACTIVE		4
#define PIN_DOMAIN_REASON_CLOSED		5
#define PIN_DOMAIN_REASON_CHARGE		6
#define PIN_DOMAIN_REASON_REFUND		7
#define PIN_DOMAIN_REASON_CREDIT_LIMIT		8
#define PIN_DOMAIN_VIDEO_MANAGER_ERRORS		9
#define PIN_DOMAIN_VIDEO_MANAGER_CANCEL_REASONS	10
#define PIN_DOMAIN_VIDEO_MANAGER_COPY_VALUES	11
#define PIN_DOMAIN_VIDEO_MANAGER_SUBJECTS	12
#define PIN_DOMAIN_BROADBAND_MANAGER_ERRORS	13
#define PIN_DOMAIN_FEATURE_CONTROL_ERRORS	14

/***************************************************************************
 *      The NUM_FACILITIES define represents the number of facilities mapped
 *      under the Server I18N (see the above facility code to domain string 
 *      mappings).  This defines value needs to be appropriately changed
 *      as a define gets added/removed from the above list to reflect the
 *      total number of facilities  mapped.
 **************************************************************************/

#define NUM_FACILITIES				15

/***************************************************************************
 *	The following are array/buffer lengths used for locale strings,
 *	locale description strings, domain strings, and localized
 *	strings under Server I18N.
 **************************************************************************/

#define LOCALE_LEN	128
#define DESCR_LEN	128
#define DOMAIN_LEN	128
#define STRBUF_LEN	8192

/***************************************************************************
 *	Type:		string_info_t
 *	Applies To:	Server Internationalization ( Server I18N )
 *	
 *	string_info_t is a general purpose container used to hold
 *	information about a localized string object.  It can be used
 *	to collect the fields required to build a storable string
 *	object ( an instance of the /strings class ), or it can be
 *	used to retrieve the same information on a database query.
 *
 *	platform_locale[] is a system dependent version of the locale[]
 *	string ( the Infranet locale ).  For example, Infranet locale
 *	"en_US" is "enu" on Win32 and "en_US" on UNIX.
 *
 *	loc_string_mbcs[] and help_string_mbcs[] are MBCS encoded
 *	localized strings that are typically read from the external
 *	environment before being converted to UTF-8 format.  Infranet
 *	uses UTF-8 encoding internally, so database writes and reads
 *	would employ loc_string_utf8[] and help_string_utf8[]. 
 **************************************************************************/

typedef struct string_info
{
	char		locale[LOCALE_LEN];
	char		platform_locale[LOCALE_LEN];
	char		domain[DOMAIN_LEN];
	int32		string_id;
	int32		string_vers;
	char		loc_string_mbcs[STRBUF_LEN];
	unsigned char	loc_string_utf8[STRBUF_LEN];
	char		help_string_mbcs[STRBUF_LEN];
	unsigned char	help_string_utf8[STRBUF_LEN];

} string_info_t;

/***************************************************************************
 *	Type:		string_list_t
 *	Applies To:	Server Internationalization ( Server I18N )
 *
 *	string_list_t is a container intended to hold a list of
 *	localized string objects.  The primary use of this container
 *	is to provide a means of maintaining an iteration state on
 *	the contained list.
 *
 *	1.  return_flistp should be the flist returned by an opcode,
 *	    as in:
 *
 *	    PCM_OP(..., PCM_OP_SEARCH, ..., search_flistp,
 *		    &return_flistp, ...);
 *
 *	    The return flist is expected to contain an array of type
 *	    PIN_FLD_RESULTS, which is itself an flist.
 *
 *	2.  elem_id and cookie are used to maintain the iteration
 *	    state, as in:
 *
 *	    strobj_flistp = PIN_FLIST_ELEM_GET_NEXT(return_flistp,
 *					PIN_FLD_RESULTS, &elem_id, 1,
 *					&cookie, ebufp);
 *
 *	    In this example PIN_FLD_RESULTS is an array of flists,
 *	    each element reprsenting one string object.
 **************************************************************************/

typedef struct string_list
{
    int32           size;
    pin_rec_id_t    elem_id;
    pin_cookie_t    cookie;
    pin_flist_t*    return_flistp;
} string_list_t;

/********************************************************************
 * List of non billable items , defined in fm_bill_utils_billing.c *
 * *****************************************************************/
EXTERN char *not_billable_items[];
EXTERN int not_billable_size;
/************************************************************************
 * Functions in the PCM Library.					*
 ************************************************************************/

#if defined(__STDC__) || defined(PIN_USE_ANSI_HDRS) || defined(__cplusplus)

/*
 * PCM functions.
 */
EXTERN void pcm_connect(
                pcm_context_t**      pcm_ctxpp,
                pin_db_no_t*         db_num_p,
                pin_errbuf_t*        ebufp);

  EXTERN void pcm_context_open(
                pcm_context_t**      pcm_ctxpp,
                pin_flist_t*         in_flistp,
                pin_errbuf_t*        ebufp);

EXTERN void pcm_context_close(
                pcm_context_t*       pcm_ctxp,
                int32                how,
                pin_errbuf_t*        ebufp );

EXTERN int pcm_context_get_fd(
		pcm_context_t*       pcm_ctxp);

EXTERN void pcm_op(
                pcm_context_t*       pcm_ctxp,
                pin_opcode_t         opcode,
                pin_opcode_flags_t   flags,
                pin_flist_t*         in_flistp,
                pin_flist_t**        ret_flistpp,
                pin_errbuf_t*        ebufp);

EXTERN void pcm_op_ex(
                pcm_context_t*       pcm_ctxp,
                pin_opcode_t         opcode,
                pin_opcode_flags_t   flags,
                pin_opcode_flags_t   flags2,
                pin_flist_t*         in_flistp,
                pin_flist_t**        ret_flistpp,
                int                  *op_statep,
                pin_errbuf_t*        ebufp );

EXTERN void pcm_set_async(
                pcm_context_t*       pcm_ctxp,
                pcm_os_cbi_t*        call_back_stuff_p,
                int*                 sdp,
                pin_errbuf_t*        ebufp);

#if defined(INCL_SSL_)
#if defined(FOR_CM) || defined(FOR_PORTAL) || defined(FOR_MTAFW) || defined(FOR_DM) || defined(FOR_QM)
EXTERN void pcm_set_multithread();
#endif
#endif

EXTERN void pcm_op_send(
                pcm_context_t*       pcm_ctxp,
                pin_opcode_t         opcode,
                pin_opcode_flags_t   flags,
                pin_flist_t*         in_flistp,
                pin_errbuf_t*        ebufp);

EXTERN void pcm_op_done(
                pcm_context_t*       pcm_ctxp,
                pin_errbuf_t*        ebufp);

EXTERN void pcm_op_recv(
                pcm_context_t*       pcm_ctxp,
                pin_flist_t**        ret_flistpp,
                pin_errbuf_t*        ebufp);

EXTERN poid_t* pcm_get_userid(
                pcm_context_t*       pcm_ctxp);

EXTERN poid_t* pcm_get_session(
                pcm_context_t*       pcm_ctxp);

EXTERN pin_flist_t* pcm_get_trans_flist(
                pcm_context_t*       pcm_ctxp);

EXTERN pin_flist_t* pcm_get_trans_flist_ref(
                pcm_context_t*       pcm_ctxp);

EXTERN int pcm_is_trans_locked(
                pcm_context_t*       pcm_ctxp);
/*
 * Flist functions.
 */
EXTERN pin_flist_t* pin_flist_create(
                pin_errbuf_t*        ebufp);

EXTERN pin_flist_t* pin_flist_create_with_size(
		size_t 	             count, 
		pin_errbuf_t         *ebufp );

EXTERN void pin_flist_destroy(
                pin_flist_t*         flistp,
                pin_errbuf_t*        ebufp);

EXTERN void pini_flist_set(
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                pin_rec_id_t         rec_id,
                int16                flags,
                void*                val,
                pin_errbuf_t*        ebufp);

EXTERN void pini_flist_put(
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                pin_rec_id_t         rec_id,
                int16                flags,
                void*                val,
                pin_errbuf_t*        ebufp);

EXTERN pin_flist_t* pin_flist_elem_add(
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                pin_rec_id_t         elem_id,
                pin_errbuf_t*        ebufp);

EXTERN pin_flist_t* pin_flist_substr_add(
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                pin_errbuf_t*        ebufp);

EXTERN void* pin_flist_elem_poid_index_get(
                pin_poid_idx_t*      idxp,
                pin_cookie_t*        magicp,
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_elem_poid_index_destroy(
                pin_poid_idx_t*      idxp,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_elem_poid_index_init(
                pin_poid_idx_t**     idxpp,
                pin_flist_t*         i_flistp,
                pin_fld_num_t        fld,
                pin_errbuf_t*        ebufp);

EXTERN void* pini_flist_get(
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                pin_rec_id_t         rec_id,
                int32                optional,
                int16*               flagsp,
                pin_errbuf_t*        ebufp);

EXTERN pin_flist_t* pin_flist_elem_get_next(
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                pin_rec_id_t*        recidp, 
                int32                optional,
                pin_cookie_t*        cookiep,
                int16*               flagsp,
                pin_errbuf_t*        ebufp);

EXTERN void* pin_flist_any_get_next(
                pin_flist_t*         flistp,
                pin_fld_num_t*       fldp,
                pin_rec_id_t*        recidp,
                int16*               flagsp,
                pin_cookie_t*        cookiep,
                pin_errbuf_t*        ebufp);

EXTERN void* pini_flist_take(
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                int32                optional,
                int32                recid,
                int16*               flagsp,
                pin_errbuf_t*        ebufp);

EXTERN pin_flist_t* pin_flist_elem_take_next(
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                pin_rec_id_t*        recidp,
                int32                optional,
                pin_cookie_t*        cookiep,
                int16*               flagsp,
                pin_errbuf_t*        ebufp);

EXTERN pin_flist_t* pin_flist_elem_take_prev(
		pin_flist_t*            flistp,
		pin_fld_num_t           fld,
		int32*                  rec_idp,
		int32                   optional,
		pin_cookie_t*           cookiep,
		int16*                  flagsp,
		pin_errbuf_t*           ebufp);

EXTERN pin_rec_id_t pin_flist_elem_count(
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_flist_count(
                pin_flist_t*         flistp,
                pin_errbuf_t*        ebufp);

EXTERN void pini_flist_fld_rename(
                pin_flist_t*         flistp,
                pin_fld_num_t        src_fld,
                pin_fld_num_t        dest_fld,
                pin_errbuf_t*        ebufp);

EXTERN int32 pini_flist_fld_copy(
                pin_flist_t*         src_flistp,
                pin_fld_num_t        src_fld,
                pin_flist_t*         dest_flistp,
                pin_fld_num_t        dest_fld,
                pin_errbuf_t*        ebufp);

EXTERN int32 pini_flist_elem_copy(
                pin_flist_t*         src_flistp,
                pin_fld_num_t        src_fld,
                pin_rec_id_t         src_recid,
                pin_flist_t*         dest_flistp,
                pin_fld_num_t        dest_fld,
                pin_rec_id_t         dest_recid,
                pin_errbuf_t*        ebufp);

EXTERN int32 pini_flist_fld_move(
                pin_flist_t*         src_flistp,
                pin_fld_num_t        src_fld,
                pin_flist_t*         dest_flistp,
                pin_fld_num_t        dest_fld,
                pin_errbuf_t*        ebufp);

EXTERN int32 pini_flist_elem_move(
                pin_flist_t*         src_flistp,
                pin_fld_num_t        src_fld,
                pin_rec_id_t         src_recid,
                pin_flist_t*         dest_flistp,
                pin_fld_num_t        dest_fld,
                pin_rec_id_t         dest_recid,
                pin_errbuf_t*        ebufp);

EXTERN pin_flist_t* pin_flist_copy(
                pin_flist_t*         src_flistp,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_print(
                pin_flist_t*         flistp,
                FILE*                fi,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_to_str(
                pin_flist_t*         flistp,
                char**               strpp,
                int32*               len,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_to_str_masked(
                pin_flist_t*         flistp,
                char**               strpp,
                int32*               len,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_to_xml(
                pin_flist_t*         in_flistp,
                int32                flags,
                int32                encoding,
                char**               bufpp,
                int*                 lenp,
                char*                root_elemname,
                pin_errbuf_t*        ebufp);

/* Not currently supported
EXTERN void pin_xml_to_flist(
                const char*          buff,
                int32                flags,
                char*                root_elemname,
                pin_flist_t**        ret_flistpp,
                pin_errbuf_t*        ebufp);
*/

EXTERN void pin_flist_to_compact(
                pin_flist_t*         in_flistp,
                pin_flist_t*         template_flistp,
                char**               bufpp,
                int*                 lenp,
                pin_errbuf_t*        ebufp);

EXTERN void pin_compact_to_flist(
                char*                bufp,
                pin_flist_t*         template_flistp,
                pin_flist_t**        ret_flistpp,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_to_str_compact(
                pin_flist_t*         flistp,
                char**               strpp,
                int32*               len,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_to_str_compact_binary(
                pin_flist_t*         flistp,
                char**               strpp,
                int32*               len,
                pin_errbuf_t*        ebufp);

EXTERN void pin_str_to_flist(
                char*                str,
                pin_db_no_t          default_db,
                pin_flist_t**        flistp,
                pin_errbuf_t*        ebufp);

EXTERN void pini_flist_drop(
                pin_flist_t*         flistp,
                pin_fld_num_t        fld,
                pin_rec_id_t         record_id,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_concat(
                pin_flist_t*         dest_flistp,
                pin_flist_t*         src_flistp,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_sort(
                pin_flist_t*         flistp,
                pin_flist_t*         sort_listp,
                int32                descending,
                int32                sort_default,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_recursive_sort(
                pin_flist_t*         flistp,
                pin_flist_t*         sort_listp,
                int32                descending,
                int32                sort_default,
                pin_errbuf_t*        ebufp);

EXTERN void pin_flist_array_sort(
                pin_flist_t*         flistp,
                int32                desc,
                pin_errbuf_t*        ebufp);

EXTERN pin_flist_t* pin_flist_find_sorted(
                pin_flist_t*         flistp,
                pin_flist_t*         search_listp,
                pin_rec_id_t*        rec_idp,
                pin_cookie_t         cookie,
                int32                cnt,
                int32                sort_default,
                pin_errbuf_t*        ebufp);

EXTERN void pini_client_put_cleanup(
                void*                datap,
                pin_fld_num_t        fld,
                pin_errbuf_t*        ebufp);

EXTERN int64 pcm_get_current_db_no(
                pcm_context_t*       pcm_ctxp);

/* Misc funcs */
EXTERN void pin_err_log_ebuf(
                int32                level,
                const char*          msg,
                const char*          file_name,
                pin_err_line_no_t    line_number,
                pin_errbuf_t*        ebufp);

EXTERN void pin_err_log_msg(
                int32                level,
                const char*          msg,
                const char*          file_name,
                pin_err_line_no_t    line_number);

EXTERN int32 pcm_trans_is_poid_locked(
	pcm_context_t*	ctxp,
	poid_t*		poidp,
	int32		flags,
	pin_errbuf_t*	ebufp);

EXTERN int32 pin_err_set_level(
                int32                level);

EXTERN int32 pin_err_get_log_level(); 

EXTERN void pin_set_err(
                pin_errbuf_t*        ebuf,
                pin_err_location_t   location,
                pin_err_class_t      pin_errclass,
                pin_err_t            pin_err,
                pin_fld_num_t        field,
                pin_rec_id_t         rec_id,
                pin_err_reserved_t   reserved);

EXTERN const char* pin_name_of_field(
                pin_fld_num_t        field);

EXTERN int32 pin_name_len_of_field(
                pin_fld_num_t        field);

EXTERN pin_fld_num_t pin_field_of_name(
                const char*          name);

EXTERN pin_fld_num_t pin_dyn_field_of_name(
                const char*          name);

EXTERN const char* pin_pinerr_to_str(
                pin_err_t            pin_err);

EXTERN int32 pin_pinerr_to_str_len(
                pin_err_t            pin_err);

EXTERN const char* pin_errloc_to_str(
                pin_err_location_t   location);

EXTERN int32 pin_errloc_to_str_len(
                pin_err_location_t   location);

EXTERN const char* pin_errclass_to_str(
                pin_err_class_t      err_class);

EXTERN int32 pin_errclass_to_str_len(
                pin_err_class_t      err_class);

EXTERN void pin_log_flist(
                int32                level,
                const char*          msg,
                const char*          filep,
                pin_err_line_no_t    line_no,
                pin_flist_t*         fp);

EXTERN void pin_log_poid(
                int32                level,
                const char*          msg,
                const char*          filep,
                pin_err_line_no_t    line_no,
                poid_t*              pdp);

EXTERN pin_fld_type_t pin_type_of_field(
                pin_fld_num_t        fld);

EXTERN pin_opcode_t pcm_opname_to_opcode(
                const char*          opname);

EXTERN const char* pcm_opcode_to_opname(
                pin_opcode_t         op);

EXTERN int pin_field_num_builtin_fields();

EXTERN char* pin_field_get_next(
                pin_fld_num_t*       nump,
                void**               cookiepp);

EXTERN int32 pin_field_num_custom_fields();

EXTERN int32 pin_field_is_masked(pin_fld_num_t num);

EXTERN int32 pin_field_num_custom_masked_fields();

EXTERN int32 pin_field_num_builtin_masked_fields();

EXTERN const char* pin_custom_field_get(
                pin_fld_num_t*       nump,
                int32                index);
EXTERN int pcm_trans_state(pcm_context_t*	ctxp);

/*
 * Poid functions.
 */
EXTERN poid_t* pin_poid_create(
                pin_db_no_t          db,
                pin_const_poid_type_t type,
                pin_poid_id_t        id,
                pin_errbuf_t*        ebufp);

EXTERN void pin_poid_destroy(
                poid_t*              poidp,
                pin_errbuf_t*        ebufp);

EXTERN poid_t* pin_poid_copy(
                poid_t*              src_poidp,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_poid_compare(
                poid_t*              first_poidp,
                poid_t*              second_poidp,
                int32                check_rev,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_poid_compare_with_lp(
                poid_t*              first_poidp,
                poid_t*              second_poidp,
                int32                check_rev,
                pin_errbuf_t*        ebufp);

EXTERN void pin_poid_print(
                poid_t*              poidp,
                FILE*                fi,
                pin_errbuf_t*        ebufp);

EXTERN void _pin_poid_print_db(
                pin_db_no_t          db,
                char*                db_buf);

EXTERN void pin_poid_to_str(
                poid_t*              poidp,
                char**               strpp,
                int32*               lenp,
                pin_errbuf_t*        ebufp);

EXTERN poid_t* pin_poid_from_str(
                const char*          strp,
                char**               end_cpp,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_poid_is_null(
                poid_t*              poidp);

EXTERN int32 pin_poid_is_it_translatable(
                pin_db_no_t          db_no);

EXTERN pin_db_no_t pin_poid_get_db(
                poid_t*              poidp);

EXTERN void pin_poid_set_db(
                poid_t*              poidp,
                pin_db_no_t          db_no);

EXTERN int32 pin_poid_get_lp_no_32(
		int32 		db);

EXTERN int32 pin_poid_get_db_no_32(
		int32 		db);

EXTERN int32 pin_poid_encode_lp_db_no_32(
		int32 		lp_no,
		int32 		db_no);

EXTERN pin_poid_id_t pin_poid_get_id(
                poid_t*              poidp);

EXTERN pin_const_poid_type_t pin_poid_get_type(
                poid_t*              poidp);

EXTERN pin_poid_rev_t pin_poid_get_rev(
                poid_t*              poidp);

/*
 * Poid list functions.
 */
EXTERN poid_list_t* pin_poid_list_create(
                pin_errbuf_t*        ebufp);

EXTERN void pin_poid_list_destroy(
                poid_list_t*         poidlp,
                pin_errbuf_t*        ebufp);

EXTERN poid_list_t* pin_poid_list_copy(
                poid_list_t*         src_pldp,
                pin_errbuf_t*        ebufp);

EXTERN void pin_poid_list_add(
                char**               strpp,
                poid_t*              poidp,
                int32                check_rev,
                pin_errbuf_t*        ebufp);

EXTERN void pin_poid_list_remove(
                char**               strpp,
                poid_t*              poidp,
                int32                check_rev,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_poid_list_compare(
                char*                strp,
                poid_t*              poidp,
                int32                flags,
                pin_errbuf_t*        ebufp);

EXTERN poid_t* pin_poid_list_get_poid(
                char*                strp,
                void*                vp,
                int32                flags,
                pin_errbuf_t*        ebufp);

EXTERN poid_t* pin_poid_list_get_next(
                char*                strp,
                int32                optional,
                pin_cookie_t*        cookiep,
                pin_errbuf_t*        ebufp);

EXTERN poid_t* pin_poid_list_take_next(
                char**               strp,
                int32                optional,
                pin_errbuf_t*        ebufp);


/*
 * pin_time functions
 */
	/* create an opaque time object */
EXTERN pin_time_t* pin_time(
                int32                yyyy,
                int32                mm,
                int32                dd,
                int32                hh,
                int32                min,
                int32                secs,
                pin_errbuf_t*        ebufp);

    /* destroy an opaque time object */
EXTERN void pin_time_destroy(
                pin_time_t*          ptp);

    /* compare two opaque time objects: -1 <, 0 =, 1 > */
EXTERN int pin_time_compare(
                pin_time_t*          ptp1,
                pin_time_t*          ptp2,
                pin_errbuf_t*        ebufp);

    /* get pieces from the opaque time object */
EXTERN int32 pin_time_get_year(
                pin_time_t*          ptp,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_time_get_month(
                pin_time_t*          ptp,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_time_get_mday(
                pin_time_t*          ptp,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_time_get_hours(
                pin_time_t*          ptp,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_time_get_minutes(
                pin_time_t*          ptp,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_time_get_seconds(
                pin_time_t*          ptp,
                pin_errbuf_t*        ebufp);


/*
 * pin.conf functions
 */
EXTERN void pin_conf(
                const char*          prog_name,
                const char*          token,
                int                  valtype,
                caddr_t*             valpp,
                pin_err_t*           errp);

EXTERN void pin_conf_multi(
                const char*          prog_name,
                const char*          token,
                int                  valtype,
                caddr_t*             valpp,
                int32*               linep,
                time_t*              modtp,
                pin_err_t*           errp);

EXTERN int get_int_pin_conf(
		char *prog, 
		char *param, 
		int def_val, 
		int debug, 
		char *module);

EXTERN void pin_conf_exists(
                pin_err_t*           errp);

EXTERN pin_flist_t* pin_conf_beid(
                pcm_context_t*       ctxp,
                pin_errbuf_t*        ebufp);

EXTERN int pin_beid_is_currency(
                pin_beid_t           beid);

/*
 * pin_virtual_time() functions
 */
EXTERN time_t pin_virtual_time(
                time_t*              tlocp);

/*
 * Internationalization functions
 */
EXTERN int32 pin_convert_mbcs_to_utf8(
                char*                pLocaleStr,
                char*                pMultiByteStr,
                int32                nMultiByteLen,
                unsigned char*       pUTF8Str,
                int32                nUTF8Len,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_convert_utf8_to_mbcs(
                char*                pLocaleStr,
                unsigned char*       pUTF8Str,
                int32                nUTF8Len,
                char*                pMultiByteStr,
                int32                nMBLen,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_convert_unicode_to_utf8(
                wchar_t*             pszUnicodeStr,
                int32                nUnicodeStrLen,
                unsigned char*       pszUTF8Str,
                int32                nUTF8StrLen,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_convert_utf8_to_unicode(
                unsigned char*       pUTF8Str,
                int32                nUTF8Len,
                wchar_t*             pUnicodeStr,
                int32                nUnicodeLen,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_IsValidUtf8(
                unsigned char*       buf,
                int32                len,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_IsValidMBCS(
                char*                locale,
                char*                buf,
                int32                len,
                pin_errbuf_t*        ebufp);

EXTERN int32 pin_mbslen(
                char*                pLocaleStr,
                char*                pMBstr,
                pin_errbuf_t*        ebufp);

EXTERN char* pin_setlocale(
                const int            lcType_p,
                char*                locale_p,
                pin_errbuf_t*        ebufp);

EXTERN u_int32 pin_convert_sizet_to_uint32(
                size_t               val,
                pin_errbuf_t*        ebufp);

EXTERN int pin_convert_sizet_to_int(
                size_t               val,
                pin_errbuf_t*        ebufp);

EXTERN int pin_convert_longint_to_int(
                long int               val,
                pin_errbuf_t*        ebufp);

EXTERN int pin_convert_ssizet_to_int(
                ssize_t               val,
                pin_errbuf_t*        ebufp);

EXTERN unsigned short pin_convert_uint_to_ushort(
                unsigned int         val,
                pin_errbuf_t*        ebufp);

/*
 * If match_exact is TRUE then the "source locale" must be an exact
 * match.  If match_exact is FALSE then the function will try to find
 * an exact match and if that fails then it will just match the
 * lanugage portion of the locale.
 * Note: The matching will always be case sensitive.
 */
EXTERN char* pin_map_locale(
                pin_locale_cat_t     src_locale,
                char*                src_locale_str,
                pin_locale_cat_t     targ_locale,
                int32                match_exact,
                pin_errbuf_t*        ebufp);

EXTERN int pin_test_locale(
                pin_locale_cat_t     locale_set,
                char*                locale_str,
                pin_errbuf_t*        ebufp);

EXTERN char* pin_map_facility_to_domain(
                int32                facility_code,
                pin_errbuf_t*        ebufp);

EXTERN void pin_get_localized_string(
                pcm_context_t*       context_p,
                const char*          locale_p,
                const char*          domain_p,
                const int32          string_id,
                const int32          string_vers,
                unsigned char*       string_buff_p,
                const int            buff_size,
                unsigned char*       help_buff_p,
                const int            help_size,
                pin_errbuf_t*        ebuf_p);

EXTERN int pin_string_info_init(
                string_info_t*       string_infop,
                pin_errbuf_t*        ebufp);

EXTERN string_list_t* pin_string_list_create(
                pin_flist_t*         return_flistp,
                pin_errbuf_t*        ebufp);

EXTERN void pin_string_list_destroy(
                string_list_t*       string_listp,
                pin_errbuf_t*        ebufp);

EXTERN string_info_t* pin_string_list_get_next(
                string_list_t*       string_listp,
                string_info_t*       string_infop,
                pin_errbuf_t*        ebufp);

EXTERN string_list_t* pin_get_localized_string_list(
                pcm_context_t*       context_p,
                const char*          locale_p,
                const char*          domain_p,
                const int32          string_id,
                const int32          string_vers,
                pin_errbuf_t*        ebuf_p);

EXTERN void pin_get_error_message(
                pcm_context_t*       context_p,
                pin_errbuf_t*        msg_ebuf_p,
                const char*          locale_p,
                unsigned char*       string_buff_p,
                const int            buff_size,
                unsigned char*       help_buff_p,
                const int            help_size,
                pin_errbuf_t*        ebuf_p);


/*
 * functions for manipulating pin_errbuf_t structure
 */
EXTERN void pin_errbuf_reset(
                pin_errbuf_t*        ebufp);

EXTERN void pin_errbuf_add_ebuf(
                pin_errbuf_t*        ebufp,
                pin_errbuf_t*        ebnewp,
                pin_err_t*           errp);

EXTERN void pin_errbuf_set_err(
                pin_errbuf_t*        ebufp,
                pin_err_location_t   location,
                pin_err_class_t      errclass,
                pin_err_t            pin_err,
                pin_fld_num_t        fld,
                pin_rec_id_t         recid,
                pin_err_reserved_t   reserved,
                pin_err_facility_t   facility,
                pin_err_msg_id_t     msgid,
                int32                version,
                pin_err_reserved_t   reserved2,
                void*                ealistp);

EXTERN void pin_errbuf_args_add_int(
                pin_errbuf_t*        ebufp,
                int32                argnum,
                int32                ival,
                pin_err_t*           errp);

EXTERN void pin_errbuf_args_add_str(
                pin_errbuf_t*        ebufp,
                int32                argnum,
                char*                msg,
                pin_err_t*           errp);

EXTERN void pin_errbuf_args_add_tstamp(
                pin_errbuf_t*        ebufp,
                int32                argnum,
                pin_fld_tstamp_t     tval,
                pin_err_t*           errp);

EXTERN void pin_errbuf_args_add_decimal(
                pin_errbuf_t*        ebufp,
                int32                argnum,
                pin_decimal_t        *decp,
                pin_err_t*           errp);

EXTERN void pin_errbuf_args_add_poid(
                pin_errbuf_t*        ebufp,
                int32                argnum,
                poid_t               *poidp,
                pin_err_t*           errp);

EXTERN void pin_errbuf_get_field_string(
                pin_errbuf_t*        ebufp,
                int32                argnum,
                char**               bufpp,
                int32*               lenp,
                pin_err_t*           errp);

EXTERN void pin_errbuf_copy(
                pin_errbuf_t*        dstp,
                pin_errbuf_t*        srcp,
                pin_errbuf_t*        ebufp);

EXTERN void pin_errbuf_sprint(
                pin_errbuf_t*        ebufp,
                char**               bufpp,
                int32*               bufsizep,
                int32                level,
                pin_err_t*           errp);

EXTERN void pin_errbuf_print_size(
                pin_errbuf_t*        ebufp,
                int32*               ebufsizep,
                int32                level,
                pin_err_t*           errp);

#else	/* ! (defined(__STDC__) || defined(PIN_USE_ANSI_HDRS)) */

/*
 * PCM functions.
 */
EXTERN void pcm_connect();
EXTERN void pcm_context_open();
EXTERN void pcm_context_close();

EXTERN void pcm_op();
EXTERN void pcm_op_ex();

EXTERN void pcm_set_async();
#if defined(INCL_SSL_)
#if defined(FOR_CM) || defined(FOR_PORTAL) || defined(FOR_MTAFW) || defined(FOR_DM) || defined(FOR_QM)
EXTERN void pcm_set_multithread();
#endif
#endif
EXTERN void pcm_op_send();
EXTERN void pcm_op_done();
EXTERN void pcm_op_recv();

EXTERN poid_t* pcm_get_userid();
EXTERN poid_t* pcm_get_session();
EXTERN pin_flist_t* pcm_get_trans_flist();
EXTERN int pcm_is_trans_locked();
EXTERN int64 pcm_get_current_db_no();

/*
 * Flist functions.
 */
EXTERN pin_flist_t* pin_flist_create();
EXTERN pin_flist_t* pin_flist_create_with_size();
EXTERN void pin_flist_destroy();

EXTERN void pini_flist_set();
EXTERN void pini_flist_put();
EXTERN pin_flist_t* pin_flist_elem_add();
EXTERN pin_flist_t* pin_flist_substr_add();

EXTERN void* pini_flist_get();
EXTERN void* pin_flist_elem_poid_index_get();
EXTERN void pin_flist_elem_poid_index_init();        
EXTERN void pin_flist_elem_poid_index_destroy();
EXTERN pin_flist_t* pin_flist_elem_get_next();
EXTERN void* pin_flist_any_get_next();
EXTERN void* pini_flist_take();
EXTERN pin_flist_t* pin_flist_elem_take_next();
EXTERN pin_flist_t* pin_flist_elem_take_prev();

EXTERN pin_rec_id_t pin_flist_elem_count();
EXTERN int32 pin_flist_count();
EXTERN pin_flist_t* pin_flist_copy();
EXTERN void pin_flist_fldval_copy();
EXTERN void pin_flist_print();
EXTERN void pin_flist_to_str();
EXTERN void pin_str_to_flist();
EXTERN void pin_flist_to_xml();
EXTERN void pin_flist_to_str_compact();
EXTERN void pin_flist_to_str_compact_binary();
EXTERN void pini_flist_drop();

EXTERN void pin_flist_concat();
EXTERN void pin_flist_sort();
EXTERN pin_flist_t* pin_flist_find_sorted();
EXTERN void pini_client_put_cleanup();

EXTERN void pin_err_log_ebuf();
EXTERN void pin_err_log_msg();
EXTERN int32 pin_err_set_level();
EXTERN void pin_set_err();
EXTERN void pin_errbuf_args_add_str();
EXTERN char* pin_errclass_to_str();
EXTERN const char* pin_name_of_field();
EXTERN pin_fld_num_t pin_field_of_name();
EXTERN void pin_log_flist();
EXTERN void pin_log_poid();
EXTERN pin_fld_type_t pin_type_of_field();
EXTERN pin_opcode_t pcm_opname_to_opcode();
EXTERN const char* pcm_opcode_to_opname();

/*
 * Poid functions.
 */
EXTERN poid_t* pin_poid_create();
EXTERN void pin_poid_destroy();
EXTERN poid_t* pin_poid_copy();
EXTERN int32 pin_poid_compare();
EXTERN void pin_poid_print();
EXTERN void _pin_poid_print_db();
EXTERN void pin_poid_to_str();
EXTERN poid_t* pin_poid_from_str();
EXTERN int32 pin_poid_is_null();
EXTERN pin_db_no_t pin_poid_get_db();
EXTERN void pin_poid_set_db();
EXTERN pin_poid_id_t pin_poid_get_id();
EXTERN pin_const_poid_type_t pin_poid_get_type();
EXTERN pin_poid_rev_t pin_poid_get_rev();

/*
 * Poid list functions.
 */
EXTERN poid_list_t* pin_poid_list_create();
EXTERN void pin_poid_list_destroy();
EXTERN poid_list_t* pin_poid_list_copy();
EXTERN void pin_poid_list_add();
EXTERN void pin_poid_list_remove();
EXTERN int32 pin_poid_list_compare();
EXTERN poid_t* pin_poid_list_get_poid();
EXTERN poid_t* pin_poid_list_get_next();
EXTERN poid_t* pin_poid_list_take_next();

/*
 * pin.conf functions
 */
EXTERN void pin_conf();
EXTERN void pin_conf_multi();
EXTERN int get_int_pin_conf();
EXTERN void pin_conf_exists();
EXTERN pin_flist_t* pin_conf_beid();

/*
 * pin_virtual_time() functions
 */
EXTERN time_t pin_virtual_time();

/*
 * Internationalization functions
 */
EXTERN int32 pin_convert_mbcs_to_utf8();
EXTERN int32 pin_convert_utf8_to_mbcs();
EXTERN int32 pin_convert_unicode_to_utf8();
EXTERN int32 pin_convert_utf8_to_unicode();
EXTERN u_int32 pin_convert_sizet_to_uint32();
EXTERN int pin_convert_sizet_to_int();
EXTERN int pin_convert_longint_to_int();
EXTERN int pin_convert_ssizet_to_int();
EXTERN unsigned short pin_convert_uint_to_ushort(); 
EXTERN int32 pin_IsValidUtf8();
EXTERN int32 pin_IsValidMBCS();
EXTERN int32 pin_mbslen();
EXTERN char* pin_setlocale();
EXTERN char* pin_map_locale();
EXTERN int pin_test_locale();
EXTERN char* pin_map_facility_to_domain();
EXTERN void pin_get_localized_string();
EXTERN int pin_string_info_init();
EXTERN string_list_t* pin_string_list_create();
EXTERN void pin_string_list_destroy();
EXTERN string_info_t* pin_string_list_get_next();
EXTERN string_list_t* pin_get_localized_string_list();
EXTERN void pin_get_error_message();

#endif	/* ! (defined(__STDC__) || defined(PIN_USE_ANSI_HDRS)) */
EXTERN int32 pcm_is_nagle_disabled();
/*
XXX transition macros 
*/
#define pcm_trans_open(pcm_ctxp, flags, trans_id, flist, ebufp)		\
{									\
	pin_flist_t* ret;						\
	pcm_op(pcm_ctxp, PCM_OP_TRANS_OPEN, flags, flist, &ret, ebufp);	\
	pin_flist_destroy(ret, (pin_errbuf_t*)NULL);			\
}

#define pcm_trans_commit(pcm_ctxp, trans_id, ebufp)			\
{									\
	pin_flist_t* ret;						\
	pcm_op(pcm_ctxp, PCM_OP_TRANS_COMMIT, 0, (pin_flist_t*)0, &ret, ebufp);\
	pin_flist_destroy(ret, (pin_errbuf_t*)NULL);			\
}
#define pcm_trans_abort(pcm_ctxp, trans_id, ebufp)			\
{									\
	pin_flist_t* ret;						\
	pcm_op(pcm_ctxp, PCM_OP_TRANS_ABORT, 0, (pin_flist_t*)0, &ret, ebufp);\
	pin_flist_destroy(ret, (pin_errbuf_t*)NULL);			\
}

/*
 * PCM macros which act as public interface to the PCM functions
 */
EXTERN int pcm_skip_record_event_session;
#define PCM_CONNECT(pcm_ctxpp, db_num_p, ebufp)				\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pcm_connect(pcm_ctxpp, db_num_p, ebufp);		\
	}								\
}

#define PCM_CONTEXT_OPEN(pcm_ctxpp, in_flistp, ebufp)			\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pcm_context_open(pcm_ctxpp, in_flistp, ebufp);		\
	}								\
}

#define PCM_CONTEXT_REOPEN(pcm_ctxpp, password, ebufp)			\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pcm_context_reopen(pcm_ctxpp, password, ebufp);		\
	}								\
}

#define PCM_CONTEXT_CLOSE(pcm_ctxp, how, ebufp)				\
{									\
	pin_errbuf_t*		ep = (pin_errbuf_t*)ebufp;		\
	if((ep) == (pin_errbuf_t*)NULL) {				\
		pcm_context_close(pcm_ctxp, how, ep);			\
	} else if((ep)->pin_err == PIN_ERR_NONE) {			\
		(ep)->line_no = __LINE__;				\
		(ep)->filename = PIN_FILE_SOURCE_ID;                    \
		pcm_context_close(pcm_ctxp, how, ep);			\
	} else {                                                        \
                pcm_context_close(pcm_ctxp, how, NULL);                 \
        }                                                               \
}

#define PCM_OP(pcm_ctxp, opcode, flags, in_flistp, ret_flistpp, ebufp)	\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pcm_op(pcm_ctxp, opcode, flags, in_flistp, ret_flistpp, ebufp);\
	}								\
}

#define PCM_OPREF(pcm_ctxp, opcode, flags, in_flistp, ret_flistpp, ebufp)	\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pcm_op_ex(pcm_ctxp, opcode, flags, PCM_OPFLG2_BY_REF, in_flistp, ret_flistpp, NULL, ebufp);\
	}								\
}

#define PCM_OPP(ctxp, opcode, flags, in_flist, ret_flist) \
    ctxp->op(opcode, flags, in_flist, ret_flist, __FILE__, __LINE__)

#define PCM_OPPREF(ctxp, opcode, flags, in_flist, ret_flist) \
    ctxp->opref(opcode, flags, in_flist, ret_flist, __FILE__, __LINE__)

#define PCM_SET_ASYNC(pcm_ctxp, callback_infop, socket_descp, ebufp)	\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pcm_set_async(pcm_ctxp, callback_infop, socket_descp, ebufp); \
	}								\
}

#if defined(INCL_SSL_)
#if defined(FOR_CM) || defined(FOR_PORTAL) || defined(FOR_MTAFW)
#define PCM_SET_MULTITHREAD()	\
{									\
		pcm_set_multithread(); \
}
#endif
#endif

#define PCM_OP_SEND(pcm_ctxp, opcode, flags, in_flistp, ebufp)		\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pcm_op_send(pcm_ctxp, opcode, flags, in_flistp, ebufp);	\
	}								\
}

#define PCM_OP_DONE(pcm_ctxp, ebufp)					\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pcm_op_done(pcm_ctxp, ebufp);				\
	}								\
}

#define PCM_OP_RECV(pcm_ctxp, ret_flistpp, ebufp)			\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pcm_op_recv(pcm_ctxp, ret_flistpp, ebufp);		\
	}								\
}

#define PCM_GET_CURRENT_DB_NO(pcm_ctxp)                                 \
                (pcm_get_current_db_no(pcm_ctxp))
#define PCM_GET_USERID(pcm_ctxp)					\
		(pcm_get_userid(pcm_ctxp))
#define PCM_GET_SESSION(pcm_ctxp)					\
		(pcm_get_session(pcm_ctxp))


/* Field level utility macros */
#define PIN_FIELD_GET_TYPE(field)					\
	(pin_type_of_field(field))

/* pin_name_of_field() uses a static buf so isn't MT-safe */
#define PIN_FIELD_GET_NAME(field)					\
	(pin_name_of_field(field))
#define PIN_FIELD_OF_NAME(name)	 	(pin_field_of_name(name))


#define PIN_PINERR_TO_STR(pin_err)					\
	(pin_pinerr_to_str(pin_err))
#define PIN_ERRLOC_TO_STR(location)					\
	(pin_errloc_to_str(location))
#define PIN_ERRCLASS_TO_STR(err_class)					\
	(pin_errclass_to_str(err_class))

#define PIN_FLIST_CREATE(ebufp)						\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ? 				\
	((ebufp)->line_no = __LINE__, (ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_flist_create(ebufp)) :				\
	(NULL))

/* Flist macros which act as public interface to flist functions */
#define PIN_FLIST_CREATE_WITH_SIZE(count, ebufp)                \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?		\
        ((ebufp)->line_no = __LINE__, (ebufp)->filename = __FILE__,     \
                pin_flist_create_with_size(count, ebufp)) :                             \
        (NULL))

/* NOTE: on destroy's, the ebuf is optional... */
#define PIN_FLIST_DESTROY(flistp, ebufp)				\
{									\
	pin_errbuf_t*		ep = (pin_errbuf_t*)ebufp;		\
	if((ep) == (pin_errbuf_t*)NULL) {				\
		pin_flist_destroy(flistp, ep);				\
	} else if((ep)->pin_err == PIN_ERR_NONE) {			\
		(ep)->line_no = __LINE__; 				\
		(ep)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_destroy(flistp, ep); 				\
	}								\
}

#define PIN_FLIST_DESTROY_EX(flistpp, ebufp)				\
{									\
	if((flistpp != (pin_flist_t **)0)) {							\
		PIN_FLIST_DESTROY(*(flistpp), ebufp)			\
		*(flistpp) = (pin_flist_t*)NULL;			\
	}								\
}

#define PIN_FLIST_COUNT(flistp, ebufp)					\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ? 				\
	((ebufp)->line_no = __LINE__, (ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_flist_count(flistp, ebufp)) :			\
	(int32) (0))

#define PIN_FLIST_COPY(flistp, ebufp)					\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__, (ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_flist_copy(flistp, ebufp)) :			\
	(NULL))

#define PIN_FLIST_CONCAT(dest_flistp, src_flistp, ebufp)		\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_concat(dest_flistp, src_flistp, ebufp);	\
	}								\
}

#define PIN_FLIST_PRINT(flistp, fi, ebufp)				\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_print(flistp, fi, ebufp);			\
	}								\
}

#define PIN_FLIST_TO_STR(flistp, strpp, lenp, ebufp)			\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_to_str(flistp, strpp, lenp, ebufp);		\
	}								\
}
#define PIN_STR_TO_FLIST(strp, default_db, flistpp, ebufp)		\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_str_to_flist(strp, default_db, flistpp, ebufp);	\
	}								\
}

#define PIN_FLIST_TO_XML(flistp, flags, enc, strpp, lenp, rootname, ebufp)	\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_to_xml(flistp, flags, enc, strpp, lenp, rootname, ebufp); \
	}								\
}

/* Not currently supported
#define PIN_XML_TO_FLIST(buff, flags, rootname, ret_flistpp, ebufp)\
{                                                                       \
        if((ebufp)->pin_err == PIN_ERR_NONE) {                          \
                (ebufp)->line_no = __LINE__;                            \
                (ebufp)->filename = PIN_FILE_SOURCE_ID;                 \
                pin_xml_to_flist(buff, flags, rootname, ret_flistpp, ebufp);	\
        }                                                               \
}
*/

#define PIN_FLIST_TO_COMPACT(iflistp, tflistp, strpp, lenp, ebufp)	\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_to_compact(iflistp, tflistp, strpp, lenp, ebufp); \
	}								\
}

#define PIN_COMPACT_TO_FLIST(strpp, tflistp, ret_flistpp, ebufp)      \
{                                                                       \
        if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {                          \
                (ebufp)->line_no = __LINE__;                            \
                (ebufp)->filename = PIN_FILE_SOURCE_ID;                         \
                pin_compact_to_flist(strpp, tflistp, ret_flistpp, ebufp); \
        }                                                               \
}

#define PIN_FLIST_TO_STR_COMPACT(flistp, strpp, lenp, ebufp)			\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_to_str_compact(flistp, strpp, lenp, ebufp);		\
	}								\
}

#define PIN_FLIST_TO_STR_COMPACT_BINARY(flistp, strpp, lenp, ebufp)			\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_to_str_compact_binary(flistp, strpp, lenp, ebufp);		\
	}								\
}

#define PIN_FLIST_SORT(flistp, sort_listp, sort_default, ebufp)		\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_sort(flistp, sort_listp, 0, sort_default, ebufp);\
	}								\
}

#define PIN_FLIST_SORT_REVERSE(flistp, sort_listp, sort_default, ebufp)	\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_sort(flistp, sort_listp, 1, sort_default, ebufp);\
	}								\
}

#define PIN_FLIST_ARRAY_SORT(flistp, desc, ebufp)		\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_flist_array_sort(flistp, desc, ebufp);		\
	}								\
}

#define PIN_FLIST_FIND_SORTED(flistp, search_listp, rec_idp, ebufp)		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ? 				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
	pin_flist_find_sorted(flistp, search_listp, rec_idp, NULL, 0, 1, ebufp)) :\
	(NULL))

#define PIN_FLIST_FLD_SET(flistp, fld, valp, ebufp)			\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pini_flist_set(flistp, fld, 0, 0, valp, ebufp);		\
	}								\
}

#ifdef ASSIGN_NULL_AFTER_FLD_PUT 
#define SET_NULL_TO_FLD_VALUE(valp) valp = NULL;
#else
#define SET_NULL_TO_FLD_VALUE(valp) 
#endif

#define PIN_FLIST_FLD_PUT(flistp, fld, valp, ebufp)			\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pini_flist_put(flistp, fld, 0, 0, valp, ebufp);		\
		if((ebufp)->pin_err != PIN_ERR_NONE) {			\
			pini_client_put_cleanup(valp, fld, NULL);	\
		}							\
	} else {							\
		pini_client_put_cleanup(valp, fld, NULL);		\
	}								\
        SET_NULL_TO_FLD_VALUE(valp);                                    \
}

#define PIN_FLIST_FLD_GET(flistp, fld, optional, ebufp)			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ? 				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pini_flist_get(flistp, fld, 0, optional, NULL, ebufp)) :\
	(NULL))

#define PIN_FLIST_FLD_TAKE(flistp, fld, optional, ebufp)		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pini_flist_take(flistp, fld, 0, optional, NULL, ebufp)) : \
	(NULL))


#define PIN_FLIST_FLD_RENAME(flistp, src_fld, dest_fld, ebufp)	\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pini_flist_fld_rename(flistp, src_fld, dest_fld, ebufp); \
	}								\
}

#define PIN_FLIST_FLD_COPY(src_flistp, src_fld, dest_flistp, dest_fld, ebufp) \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pini_flist_fld_copy(src_flistp, src_fld, dest_flistp, dest_fld, ebufp)) : \
	(0))

#define PIN_FLIST_ELEM_COPY(src_flistp, src_fld, src_recid, dest_flistp, dest_fld, dest_recid, ebufp)	\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pini_flist_elem_copy(src_flistp, src_fld, src_recid, dest_flistp, dest_fld, dest_recid, ebufp)) : \
	(0))

#define PIN_FLIST_FLD_MOVE(src_flistp, src_fld, dest_flistp, dest_fld, ebufp)	\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pini_flist_fld_move(src_flistp, src_fld, dest_flistp, dest_fld, ebufp)) : \
	(0))

#define PIN_FLIST_ELEM_MOVE(src_flistp, src_fld, src_recid, dest_flistp, dest_fld, dest_recid, ebufp)	\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pini_flist_elem_move(src_flistp, src_fld, src_recid, dest_flistp, dest_fld, dest_recid, ebufp)) : \
	(0))

#define PIN_FLIST_FLD_DROP(flistp, fld, ebufp)				\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pini_flist_drop(flistp, fld, 0, ebufp);			\
	}								\
}

#define PIN_FLIST_ELEM_COUNT(flistp, fld, ebufp)			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ? 				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_flist_elem_count(flistp, fld, ebufp)) :		\
	(0))

#define PIN_FLIST_ELEM_ADD(flistp, fld, elem_id, ebufp)			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_flist_elem_add(flistp, fld, elem_id, ebufp)) :	\
	(NULL))

#define PIN_FLIST_ELEM_SET(flistp, elem_flistp, fld, elem_id, ebufp)	\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pini_flist_set(flistp, fld, elem_id, 0, elem_flistp, ebufp); \
	}								\
}

#ifdef ASSIGN_NULL_AFTER_ELEM_PUT 
#define SET_NULL_TO_ELEM_FLISTP(elem_flistp) elem_flistp = NULL;
#else
#define SET_NULL_TO_ELEM_FLISTP(elem_flistp)  
#endif

#define PIN_FLIST_ELEM_PUT(flistp, elem_flistp, fld, elem_id, ebufp)	\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pini_flist_put(flistp, fld, elem_id, 0, elem_flistp, ebufp); \
		if((ebufp)->pin_err != PIN_ERR_NONE) {			\
			pini_client_put_cleanup((void*)elem_flistp,	\
				fld, NULL);				\
		}							\
	} else {							\
		pini_client_put_cleanup((void*)elem_flistp, fld, NULL);\
	}								\
        SET_NULL_TO_ELEM_FLISTP(elem_flistp);                           \
}

#define PIN_FLIST_ELEM_GET(flistp, fld, elem_id, optional, ebufp)	\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pini_flist_get(flistp, fld, elem_id, optional, NULL, ebufp)) :\
	(NULL))

#define PIN_FLIST_ELEM_TAKE(flistp, fld, elem_id, optional, ebufp)	\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pini_flist_take(flistp, fld, elem_id, optional, NULL, ebufp)) :\
	(NULL))

#define PIN_FLIST_ELEM_GET_NEXT(flistp, fld, recidp, optional, cookiep, ebufp) \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_flist_elem_get_next(flistp, fld, recidp, optional, 	\
				cookiep, NULL, ebufp)) :		\
	(NULL))

#define PIN_FLIST_ANY_GET_NEXT(flistp, fldp, recidp, cookiep, ebufp) 	\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_flist_any_get_next(flistp, fldp, recidp, NULL,	\
				cookiep, ebufp)) :			\
	(NULL))

#define PIN_FLIST_ELEM_TAKE_NEXT(flistp, fld, recidp, optional, cookiep, ebufp)\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_flist_elem_take_next(flistp, fld, recidp, optional,	\
			cookiep, NULL, ebufp)) :			\
		(NULL))

#define PIN_FLIST_ELEM_TAKE_PREV(flistp, fld, recidp, optional, cookiep, ebufp)\
	((ebufp && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_flist_elem_take_prev(flistp, fld, recidp, optional,	\
			cookiep, NULL, ebufp)) :			\
		(NULL))

#define PIN_FLIST_ELEM_DROP(flistp, fld, elem_id, ebufp)		\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pini_flist_drop(flistp, fld, elem_id, ebufp);		\
	}								\
}

#define PIN_FLIST_SUBSTR_ADD(flistp, fld, ebufp)			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_flist_substr_add(flistp, fld, ebufp)) :		\
	(NULL))

#define PIN_FLIST_SUBSTR_SET(flistp, substr_flistp, fld, ebufp)		\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pini_flist_set(flistp, fld, 0, 0, substr_flistp, ebufp); \
	}								\
}



#ifdef ASSIGN_NULL_AFTER_SUBSTR_PUT 
#define SET_NULL_TO_SUBSTR_FLISTP(substr_flistp) substr_flistp = NULL;                                             
#else
#define SET_NULL_TO_SUBSTR_FLISTP(substr_flistp)                 
#endif

#define PIN_FLIST_SUBSTR_PUT(flistp, substr_flistp, fld, ebufp)		\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pini_flist_put(flistp, fld, 0, 0, substr_flistp, ebufp); \
		if((ebufp)->pin_err != PIN_ERR_NONE) {			\
			pini_client_put_cleanup((void*)substr_flistp, 	\
				fld, NULL);				\
		}							\
	} else {							\
		pini_client_put_cleanup((void*)substr_flistp, 		\
			fld, NULL);					\
	}								\
        SET_NULL_TO_SUBSTR_FLISTP(substr_flistp);                       \
}

#define PIN_FLIST_SUBSTR_GET(flistp, fld, optional, ebufp)		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pini_flist_get(flistp, fld, 0, optional, NULL, ebufp)) :\
	(NULL))

#define PIN_FLIST_SUBSTR_TAKE(flistp, fld, optional, ebufp)		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pini_flist_take(flistp, fld, 0, optional, NULL, ebufp)) :\
	(NULL))

#define PIN_FLIST_SUBSTR_DROP(flistp, fld, ebufp)			\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pini_flist_drop(flistp, fld, 0, ebufp);			\
	}								\
}

#define PIN_SET_ERR(ebufp, err_location, err_pin_errclass, err_pin_err, err_field, err_rec_id, err_reserved) \
  {  										\
	if (ebufp != (pin_errbuf_t *)0){								\
         (ebufp)->location = err_location;					\
         (ebufp)->pin_errclass = err_pin_errclass;                              \
         (ebufp)->pin_err = err_pin_err;                                        \
         (ebufp)->field = (pin_fld_num_t)err_field;                             \
         (ebufp)->rec_id = err_rec_id;                                          \
         (ebufp)->reserved = err_reserved;                                      \
    } 								        	\
}

#define PIN_ERR_LOG_EBUF(level, msg, ebufp)				\
	pin_err_log_ebuf(level, msg, PIN_FILE_SOURCE_ID, __LINE__, ebufp)

#define PIN_ERR_LOG_MSG(level, msg)					\
	pin_err_log_msg(level, ((char *)NULL != (char *)msg)?(msg):"", PIN_FILE_SOURCE_ID, __LINE__)

#define PIN_ERR_SET_LEVEL(level)					\
	pin_err_set_level(level)

#define PIN_ERR_SET_LOGFILE(path)					\
{									\
	pin_err_t	pinlog_err_junk;					\
	pinlog_path_set(path, &pinlog_err_junk);			\
}

#define PIN_ERR_SET_PROGRAM(program)					\
{									\
	pin_err_t	pinlog_err_junk;					\
	pinlog_name_set(program, &pinlog_err_junk);			\
}

#define PIN_ERR_IS_ERR(ebufp)						\
	((ebufp != (pin_errbuf_t *)0)?((ebufp)->pin_err != PIN_ERR_NONE):fprintf(stderr, "\n*** Error buffer not initialized ***\n"))

#define PIN_ERR_IS_RETRYABLE(ebufp)					\
	(((ebufp)->pin_err != PIN_ERR_NONE) &&					\
	((ebufp)->pin_errclass == PIN_ERRCLASS_SYSTEM_RETRYABLE))

/*
 * Performance testing shows that setting the individual
 * values is actually about 2 times faster than using memset.
 */
#define PIN_ERR_CLEAR_ERR(ebufp)                        \
{							\
     if (ebufp != (pin_errbuf_t *)0){                                        \
	(ebufp)->location = 0;                          \
	(ebufp)->pin_errclass = 0;                      \
	(ebufp)->pin_err = 0;                           \
	(ebufp)->field = 0;                             \
	(ebufp)->rec_id = 0;                            \
	(ebufp)->reserved = 0;                          \
	(ebufp)->line_no = 0;                           \
	(ebufp)->filename = NULL;                       \
	(ebufp)->facility = 0;                          \
	(ebufp)->msg_id = 0;                            \
	(ebufp)->err_time_sec= 0;                       \
	(ebufp)->err_time_usec = 0;                     \
	(ebufp)->version = 0;                           \
	(ebufp)->argsp = NULL;                          \
	(ebufp)->nextp = NULL;                          \
        (ebufp)->reserved2 = 0;                         \
     }  						\
} 

#define PIN_ERRBUF_IS_ERR(ebufp)					\
	PIN_ERR_IS_ERR(ebufp)

#define PIN_ERRBUF_CLEAR(ebufp)						\
	PIN_ERR_CLEAR_ERR(ebufp)

#define PIN_ERRBUF_RESET(ebufp)						\
	pin_errbuf_reset(ebufp);

#define PIN_FLIST_LOG_ERR(msg, ebufp)					\
{				                                        \
     if (ebufp != (pin_errbuf_t *)0){							\
	if((ebufp)->filename == (char*)NULL) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;			\
	}								\
	pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, msg, (ebufp)->filename,	\
	    (ebufp)->line_no, ebufp);					\
	}                        				        \
}

#define PIN_ERR_LOG_FLIST(level, msg, flistp)				\
{									\
	pin_log_flist(level, msg, PIN_FILE_SOURCE_ID, __LINE__, flistp);		\
}

#define PIN_ERR_LOG_POID(level, msg, pdp)				\
{									\
	pin_log_poid(level, msg, PIN_FILE_SOURCE_ID, __LINE__, pdp);		\
}


/* POID specific wrappers */
#define PIN_POID_CREATE(db, type, id, ebufp)				\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_poid_create(db, type, id, ebufp)) :			\
	(NULL))

#define PIN_POID_DESTROY(pdp, ebufp)					\
{									\
	pin_errbuf_t*		ep = (pin_errbuf_t*)ebufp;		\
	if((ep) == (pin_errbuf_t*)NULL) {				\
		pin_poid_destroy(pdp, ep);				\
	} else if((ep)->pin_err == PIN_ERR_NONE) {			\
		(ep)->line_no = __LINE__; 				\
		(ep)->filename = PIN_FILE_SOURCE_ID;				\
		pin_poid_destroy(pdp, ep); 				\
	}								\
}

#define PIN_POID_COPY(src_pdp, ebufp)					\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_poid_copy(src_pdp, ebufp)) :			\
	(NULL))

#define PIN_POID_COMPARE(pdp1, pdp2, check_rev, ebufp)			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_poid_compare(pdp1, pdp2, check_rev, ebufp)) :	\
	(0))

#define PIN_POID_COMPARE_WITH_LP(pdp1, pdp2, check_rev, ebufp)                        \
        (((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
        ((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,    \
                pin_poid_compare_with_lp(pdp1, pdp2, check_rev, ebufp)) :     \
        (0))



#define PIN_POID_PRINT(pp, fi, ebufp)					\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) { 				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_poid_print(pp, fi, ebufp);				\
	}								\
}

#define PIN_POID_TO_STR(pp, strpp, lenp, ebufp)				\
{									\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_poid_to_str(pp, strpp, lenp, ebufp);		\
	}								\
}

#define PIN_POID_FROM_STR(strp, end_cpp, ebufp)				\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_poid_from_str(strp, end_cpp, ebufp)) :		\
	(NULL))

/* POID LIST specific wrappers */

#define PIN_POID_LIST_CREATE(ebufp)					\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_poid_list_create(ebufp)) :				\
	(NULL))

#define PIN_POID_LIST_DESTROY(pldp, ebufp)				\
{									\
	pin_errbuf_t*		ep = (pin_errbuf_t*)ebufp;		\
	if((ep) == (pin_errbuf_t*)NULL) {				\
		pin_poid_list_destroy(pldp, ep);			\
	} else if((ep)->pin_err == PIN_ERR_NONE) {			\
		(ep)->line_no = __LINE__; 				\
		(ep)->filename = PIN_FILE_SOURCE_ID;				\
		pin_poid_list_destroy(pldp, ep); 			\
	}								\
}

#define PIN_POID_LIST_COPY(src_pldp, ebufp)				\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_poid_list_copy(src_pldp, ebufp)) :			\
	(NULL))

#define PIN_POID_LIST_ADD_POID(strpp, pp, check_rev, ebufp)		\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_poid_list_add(strpp, pp, check_rev, ebufp);		\
	}								\

#define PIN_POID_LIST_REMOVE_POID(strpp, pp, check_rev, ebufp)		\
	if((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) {				\
		(ebufp)->line_no = __LINE__;				\
		(ebufp)->filename = PIN_FILE_SOURCE_ID;				\
		pin_poid_list_remove(strpp, pp, check_rev, ebufp);	\
	}								\

#define PIN_POID_LIST_COPY_POID(strp, vp, flags, ebufp)			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_poid_list_get_poid(strp, vp, flags, ebufp)) :	\
	(NULL))

#define PIN_POID_LIST_COMPARE(strp, pp, flags, ebufp)			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_poid_list_compare(strp, pp, flags, ebufp)) :	\
	(NULL))

#define PIN_POID_LIST_COPY_NEXT_POID(strp, optional, cookiep, ebufp)	\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_poid_list_get_next(strp, optional, 			\
				cookiep, ebufp)) :			\
	(NULL))

#define PIN_POID_LIST_TAKE_NEXT_POID(strpp, optional, ebufp)		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_poid_list_take_next(strpp, optional, 		\
				ebufp)) :				\
	(NULL))

#define PIN_POID_IS_IT_TRANSLATABLE(db_no)						\
	(pin_poid_is_it_translatable(db_no))

#define PIN_POID_IS_NULL(pp)						\
	(pin_poid_is_null(pp))

#define PIN_POID_IS_TYPE_ONLY(pp)					\
	(pin_poid_get_id(pp) == (int64)-1)

#define PIN_POID_GET_DB(pp)						\
	(pin_poid_get_db(pp))

#define PIN_POID_SET_DB(pp, db_no)					\
	(pin_poid_set_db(pp, db_no))

#define PIN_POID_GET_ID(pp)						\
	(pin_poid_get_id(pp))

#define PIN_POID_GET_TYPE(pp)						\
	(pin_poid_get_type(pp))

#define PIN_POID_GET_REV(pp)						\
	(pin_poid_get_rev(pp))

#define PIN_BEID_IS_CURRENCY(beid)						\
	(pin_beid_is_currency(beid))

#ifdef _MBCS

#define PIN_CONVERT_STR_TO_UTF8(locale_p, src_p, src_len, dest_p, dest_len, ebufp) \
	PIN_CONVERT_MBCS_TO_UTF8(locale_p, src_p, src_len, dest_p, dest_len, ebufp)

#define PIN_CONVERT_UTF8_TO_STR(locale_p, src_p, src_len, dest_p, dest_len, ebufp) \
	PIN_CONVERT_UTF8_TO_MBCS(locale_p, src_p, src_len, dest_p, dest_len, ebufp)

#elif defined _UNICODE 

#define PIN_CONVERT_STR_TO_UTF8(locale_p, src_p, src_len, dest_p, dest_len,ebufp) \
	PIN_CONVERT_UNICODE_TO_UTF8(src_p, src_len, dest_p, dest_len, ebufp)
 
#define PIN_CONVERT_UTF8_TO_STR(locale_p, src_p, src_len, dest_p, dest_len, ebufp) \
	PIN_CONVERT_UTF8_TO_UNICODE(src_p, src_len, dest_p, dest_len, ebufp)
 
#endif /* _UNICODE and _MBCS */

#define PIN_CONVERT_MBCS_TO_UTF8(locale_p, src_p, src_len, dest_p, dest_len,ebufp) \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
		pin_convert_mbcs_to_utf8(locale_p, src_p, src_len, dest_p, \
					dest_len, ebufp)):         \
	(0))

#define PIN_CONVERT_UTF8_TO_MBCS(locale_p, src_p, src_len, dest_p, dest_len, ebufp)	 \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
                pin_convert_utf8_to_mbcs(locale_p, src_p, src_len, dest_p, \
					dest_len, ebufp)):         \
	(0))

#define PIN_CONVERT_UTF8_TO_UNICODE(src_p, src_len, dest_p, dest_len, ebufp) \
        (((ebufp)->pin_err == PIN_ERR_NONE) ?                           \
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
		pin_convert_utf8_to_unicode(src_p, src_len, dest_p, dest_len, \
						ebufp)): \
	(0))

#define PIN_CONVERT_UNICODE_TO_UTF8(src_p, src_len, dest_p, dest_len, ebufp) \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
		pin_convert_unicode_to_utf8(src_p, src_len, dest_p, dest_len, \
						ebufp)): \
	(0))

#define PIN_SETLOCALE(lctype, locale_p, ebufp)	 \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
		pin_setlocale(lctype, locale_p, ebufp)): \
	(NULL))

#define PIN_MBSLEN(locale_p, src_p, ebufp)	 \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
		pin_mbslen(locale_p, src_p, ebufp)): \
	(0))

#define PIN_FLIST_ELEM_POID_INDEX_INIT(idxpp, flistp, ebufp) \
{ \
        if ((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) { \
                (ebufp)->line_no = __LINE__; \
                (ebufp)->filename = PIN_FILE_SOURCE_ID; \
                pin_flist_elem_poid_index_init(idxpp, flistp, \
                                               PIN_FLD_EXTRA_RESULTS, ebufp); \
        } \
}

#define PIN_FLIST_ELEM_POID_INDEX_DESTROY(idxp, ebufp) \
{ \
        pin_errbuf_t* ep = (pin_errbuf_t*)ebufp; \
        if ((ep) == (pin_errbuf_t*)NULL) {       \
                pin_flist_elem_poid_index_destroy(idxp, ep); \
        } else if ((ep)->pin_err == PIN_ERR_NONE) { \
                (ep)->line_no = __LINE__;           \
                (ep)->filename = PIN_FILE_SOURCE_ID;          \
                pin_flist_elem_poid_index_destroy(idxp, ep); \
        } \
}

#define PIN_FLIST_ELEM_POID_INDEX_GET_NEXT(idxp, magicp, flistp, ebufp) \
        (((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ? \
        ((ebufp)->line_no == __LINE__, (ebufp)->filename = PIN_FILE_SOURCE_ID, \
                pin_flist_elem_poid_index_get(idxp, magicp, flistp, \
                                              PIN_FLD_LINKED_OBJ, ebufp)) : \
        (NULL))
        
/***************************************************************************
 *    Locale Mapping Macros
 *
 *    The following macros map an input locale string to its equivalent
 *    string in another set, for example, from the set of Win32 locale
 *    strings to the set of Solaris locale strings.
 *
 *    The return value of each macro is a string pointer to a locale
 *    string, or a null pointer if the input cannot be mapped or an
 *    error occured.  The return value is dynamically allocated and
 *    should be freed via pin_free() when no longer needed.
 **************************************************************************/

#define PIN_MAP_INFRANET_TO_PLATFORM_LOCALE( locale_str, ebufp )	\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( INFRANET_LOCALE, locale_str,		\
                PLATFORM_LOCALE, 0, ebufp)) :                           \
	(NULL))


#define PIN_MAP_INFRANET_TO_INFRANET_LOCALE( locale_str, ebufp )        \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( INFRANET_LOCALE, locale_str,		\
                INFRANET_LOCALE, 0, ebufp)) :                           \
	(NULL))

#define PIN_MAP_INFRANET_TO_UNIX_LOCALE( locale_str, ebufp )            \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( INFRANET_LOCALE, locale_str,		\
                UNIX_LOCALE, 0, ebufp)) :                               \
	(NULL))

#define PIN_MAP_INFRANET_TO_HPUX_LOCALE( locale_str, ebufp )            \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( INFRANET_LOCALE, locale_str,		\
                HPUX_LOCALE, 0, ebufp)) :                               \
	(NULL))

#define PIN_MAP_INFRANET_TO_AIX_LOCALE( locale_str, ebufp )            \
        (((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
        ((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
                pin_map_locale( INFRANET_LOCALE, locale_str,            \
                AIX_LOCALE, 0, ebufp)) :                               \
        (NULL))

#define PIN_MAP_INFRANET_TO_WIN32_LOCALE( locale_str, ebufp )           \
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( INFRANET_LOCALE, locale_str,		\
                WIN32_LOCALE, 0, ebufp)) :                              \
	(NULL))


#define PIN_MAP_UNIX_TO_INFRANET_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( UNIX_LOCALE, locale_str,		\
                INFRANET_LOCALE, 0, ebufp)) :                           \
	(NULL))

#define PIN_MAP_UNIX_TO_UNIX_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( UNIX_LOCALE, locale_str,		\
                UNIX_LOCALE, 0, ebufp)) :                               \
	(NULL))

#define PIN_MAP_UNIX_TO_HPUX_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( UNIX_LOCALE, locale_str,		\
                HPUX_LOCALE, 0, ebufp)) :                               \
	(NULL))
 
#define PIN_MAP_UNIX_TO_AIX_LOCALE( locale_str, ebufp )         \
        (((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
        ((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
                pin_map_locale( UNIX_LOCALE, locale_str,                \
                AIX_LOCALE, 0, ebufp)) :                               \
        (NULL))

#define PIN_MAP_UNIX_TO_WIN32_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( UNIX_LOCALE, locale_str,		\
                WIN32_LOCALE, 0, ebufp)) :                              \
	(NULL))


#define PIN_MAP_HPUX_TO_INFRANET_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( HPUX_LOCALE, locale_str,		\
                INFRANET_LOCALE, 0, ebufp)) :                           \
	(NULL))
 
#define PIN_MAP_HPUX_TO_UNIX_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( HPUX_LOCALE, locale_str,		\
                UNIX_LOCALE, 0, ebufp)) :                               \
	(NULL))

#define PIN_MAP_AIX_TO_UNIX_LOCALE( locale_str, ebufp )         \
        (((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
        ((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
                pin_map_locale( AIX_LOCALE, locale_str,         \
                UNIX_LOCALE, 0, ebufp)) :                               \
        (NULL))

#define PIN_MAP_HPUX_TO_HPUX_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( HPUX_LOCALE, locale_str,		\
                HPUX_LOCALE, 0, ebufp)) :                               \
	(NULL))

#define PIN_MAP_AIX_TO_AIX_LOCALE( locale_str, ebufp )          \
        (((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
        ((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
                pin_map_locale( AIX_LOCALE, locale_str,         \
                AIX_LOCALE, 0, ebufp)) :                               \
        (NULL))

#define PIN_MAP_HPUX_TO_WIN32_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( HPUX_LOCALE, locale_str,		\
                WIN32_LOCALE, 0, ebufp)) :                              \
	(NULL))

#define PIN_MAP_AIX_TO_WIN32_LOCALE( locale_str, ebufp )                \
        (((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
        ((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
                pin_map_locale( HPUX_LOCALE, locale_str,                \
                AIX_LOCALE, 0, ebufp)) :                              \
        (NULL))

#define PIN_MAP_WIN32_TO_INFRANET_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( WIN32_LOCALE, locale_str,		\
                INFRANET_LOCALE, 0, ebufp)) :                           \
	(NULL))

#define PIN_MAP_AIX_TO_INFRANET_LOCALE( locale_str, ebufp )             \
        (((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?                           \
        ((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,      \
                pin_map_locale( AIX_LOCALE, locale_str,         \
                INFRANET_LOCALE, 0, ebufp)) :                           \
        (NULL))

#define PIN_MAP_WIN32_TO_UNIX_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( WIN32_LOCALE, locale_str,		\
                UNIX_LOCALE, 0, ebufp)) :                               \
	(NULL))

#define PIN_MAP_WIN32_TO_HPUX_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( WIN32_LOCALE, locale_str,		\
                HPUX_LOCALE, 0, ebufp)) :                               \
	(NULL))

#define PIN_MAP_WIN32_TO_WIN32_LOCALE( locale_str, ebufp )		\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_map_locale( WIN32_LOCALE, locale_str,		\
                WIN32_LOCALE, 0, ebufp)) :                              \
	(NULL))


/***************************************************************************
 *    Locale Test Macros
 *
 *    The following macros determine if an input locale string is an
 *    element of the locale set.  The macros look for an exact match
 *    with case sensitivity.
 *
 *    The return value of each macro is a string pointer to a locale
 *    string if the input is an element of the set, or a null pointer
 *    if the input is not an element or an error occured.  The return
 *    value is dynamically allocated and should be freed via pin_free()
 *    when no longer needed.
 **************************************************************************/

#define PIN_TEST_INFRANET_LOCALE( locale_str, ebufp )			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_test_locale( INFRANET_LOCALE, locale_str,		\
                ebufp)) : (0))

#define PIN_TEST_UNIX_LOCALE( locale_str, ebufp )			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_test_locale( UNIX_LOCALE, locale_str,		\
                ebufp)) : (0))

#define PIN_TEST_HPUX_LOCALE( locale_str, ebufp )			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_test_locale( HPUX_LOCALE, locale_str,		\
                ebufp)) : (0))

#define PIN_TEST_WIN32_LOCALE( locale_str, ebufp )			\
	(((ebufp != (pin_errbuf_t *)0) && (ebufp)->pin_err == PIN_ERR_NONE) ?				\
	((ebufp)->line_no = __LINE__,(ebufp)->filename = PIN_FILE_SOURCE_ID,	\
		pin_test_locale( WIN32_LOCALE, locale_str,		\
                ebufp)) : (0))

/***************************************************************************
 * Validity Encoding/Decoding Macros
 *
 * Macros to store/retrieve validity details for start and end time. 
 * The validity field stores the start/end time mode and also it stores 
 * the offset and unit in case the mode is relative.
 **************************************************************************/

#define PIN_VALIDITY_UNIT_MASK    0x00000F00
#define PIN_VALIDITY_OFFSET_MASK  0xFFFFF000
#define PIN_VALIDITY_MODE_MASK    0x000000FF

#define PIN_VALIDITY_OFFSET_SHIFT  12
#define PIN_VALIDITY_UNIT_SHIFT    8

#define PIN_VALIDITY_MAX_OFFSET_SIZE   0xFFFFF
#define PIN_VALIDITY_MAX_UNIT_SIZE     0xF
#define PIN_VALIDITY_MAX_MODE_SIZE     0xFF

typedef enum pin_validity_modes {
	PIN_VALIDITY_ABSOLUTE     = 0,
	PIN_VALIDITY_IMMEDIATE    = 1,
	PIN_VALIDITY_NEVER        = 2,
	PIN_VALIDITY_FIRST_USAGE  = 3,
	PIN_VALIDITY_RELATIVE     = 4,
	PIN_VALIDITY_SPLIT_ALIGN_BUCKET = 5,
	PIN_VALIDITY_SPLIT_ALIGN_TOTAL  = 6,
	PIN_VALIDITY_ALIGN_BASE = 7,
	PIN_VALIDITY_ALIGN_ANY_BASE_EARLIEST = 8,
	PIN_VALIDITY_ALIGN_ANY_BASE_LATEST = 9,
	PIN_VALIDITY_ALIGN_ANY_EARLIEST = 10,
	PIN_VALIDITY_ALIGN_ANY_LATEST = 11
} pin_validity_modes_t;

/* Get unit macro definition
 * Input arguments- u_int32 encoded_value
 * Return - u_int32 unit 
 */
#define PIN_VALIDITY_GET_UNIT(encoded_value) \
   ((u_int32)(((encoded_value) & PIN_VALIDITY_UNIT_MASK) \
   >> PIN_VALIDITY_UNIT_SHIFT))

/* Get offset macro definition
 * Input arguments- u_int32 encoded_value
 * Return - u_int32 offset
 */
#define PIN_VALIDITY_GET_OFFSET(encoded_value) \
   ((u_int32)(((encoded_value) & PIN_VALIDITY_OFFSET_MASK) \
   >> PIN_VALIDITY_OFFSET_SHIFT))

/* Get mode macro definition
 * Input arguments- u_int32 encoded_value
 * Return - pin_validity_modes_t mode
 */
#define PIN_VALIDITY_GET_MODE(encoded_value) \
   ((pin_validity_modes_t)((encoded_value) & \
   PIN_VALIDITY_MODE_MASK))

/* Set unit macro definition
 * Input arguments- u_int32 encoded_value,
 *                  u_int32 unit 
 * Return - u_int32 encoded_value
 */
#define PIN_VALIDITY_SET_UNIT(encoded_value, unit) \
   ((u_int32)(((encoded_value) & (~PIN_VALIDITY_UNIT_MASK)) \
   | (((unit) & PIN_VALIDITY_MAX_UNIT_SIZE) << PIN_VALIDITY_UNIT_SHIFT)))

/* Set offset macro definition
 * Input arguments- u_int32 encoded_value,
 *                  u_int32 offset
 * Return - u_int32 encoded_value
 */
#define PIN_VALIDITY_SET_OFFSET(encoded_value, offset) \
   ((u_int32)(((encoded_value) & (~PIN_VALIDITY_OFFSET_MASK)) \
   | (((offset) & PIN_VALIDITY_MAX_OFFSET_SIZE) << PIN_VALIDITY_OFFSET_SHIFT)))

/* Set mode macro definition
 * Input arguments- u_int32 encoded_value,
 *                  pin_validity_modes_t mode
 * Return - u_int32 encoded_value
 */
#define PIN_VALIDITY_SET_MODE(encoded_value, mode)  \
   ((u_int32)(((encoded_value) & (~PIN_VALIDITY_MODE_MASK)) | \
   ((mode) & PIN_VALIDITY_MAX_MODE_SIZE)))

/* Decode macro definition
 * Input arguments- u_int32 encoded_value,
 *                  pin_validity_modes_t mode,
 *                  u_int32 unit,
 *                  u_int32 offset
 * Return - void
 */
#define PIN_VALIDITY_DECODE_FIELD(encoded_value, mode, unit, offset) \
{                                                                    \
    (mode)   = PIN_VALIDITY_GET_MODE(encoded_value);                 \
    (offset) = PIN_VALIDITY_GET_OFFSET(encoded_value);               \
    (unit)   = PIN_VALIDITY_GET_UNIT(encoded_value);                 \
}

/* Encode macro definition
 * Input arguments - pin_validity_modes_t mode,
 *                   u_int32 unit,
 *                   u_int32 offset
 * Return - u_int32 encoded_value
 */
#define PIN_VALIDITY_ENCODE_FIELD(mode, unit, offset) \
   ((u_int32) ( (((offset) & PIN_VALIDITY_MAX_OFFSET_SIZE) << \
   PIN_VALIDITY_OFFSET_SHIFT ) | (((unit) & PIN_VALIDITY_MAX_UNIT_SIZE) \
   << PIN_VALIDITY_UNIT_SHIFT) | ((mode) & PIN_VALIDITY_MAX_MODE_SIZE)))


#define PIN_CONVERT_SIZET_TO_UINT32(val, ebufp) ({\
       if (val > UINT32_MAX) {  \
                PIN_SET_ERR(ebufp, \
                (int32)PIN_ERRLOC_PCM, \
	        (int32)PIN_ERRCLASS_SYSTEM_DETERMINATE, \
	        (int32)PIN_ERR_DATA_OVERFLOW, \
	        (int32)0, (int32)0, (int32)0 ); \
       } \
       (u_int32)val; \
})

#define PIN_CONVERT_SIZET_TO_INT(val, ebufp) ({\
       if (val > INT32_MAX) {  \
                PIN_SET_ERR(ebufp, \
                (int32)PIN_ERRLOC_PCM, \
	        (int32)PIN_ERRCLASS_SYSTEM_DETERMINATE, \
	        (int32)PIN_ERR_DATA_OVERFLOW, \
	        (int32)0, (int32)0, (int32)0 ); \
       } \
       (int)val; \
})

#define PIN_CONVERT_LONGINT_TO_INT(val, ebufp) ({\
       if (val > INT32_MAX) {  \
                PIN_SET_ERR(ebufp, \
                (int32)PIN_ERRLOC_PCM, \
	        (int32)PIN_ERRCLASS_SYSTEM_DETERMINATE, \
	        (int32)PIN_ERR_DATA_OVERFLOW, \
	        (int32)0, (int32)0, (int32)0 ); \
       } \
       (int)val; \
})

#define PIN_CONVERT_SSIZET_TO_INT(val, ebufp) ({\
       if (val > INT32_MAX) {  \
                PIN_SET_ERR(ebufp, \
                (int32)PIN_ERRLOC_PCM, \
                (int32)PIN_ERRCLASS_SYSTEM_DETERMINATE, \
                (int32)PIN_ERR_DATA_OVERFLOW, \
                (int32)0, (int32)0, (int32)0 ); \
       } \
       (int)val; \
})

#define PIN_CONVERT_UINT_TO_USHORT(val, ebufp) ({\
       if (val > USHRT_MAX) {  \
                PIN_SET_ERR(ebufp, \
                (int32)PIN_ERRLOC_PCM, \
                (int32)PIN_ERRCLASS_SYSTEM_DETERMINATE, \
                (int32)PIN_ERR_DATA_OVERFLOW, \
                (int32)0, (int32)0, (int32)0 ); \
       } \
       (unsigned short)val; \
})


/*
 * flags for XA TRANS_OPEN
 * used by opcode PCM_OP_TRANS_OPEN for XA in   PIN_FLD_FLAGS field.
 * This list can be expanded later on to support other TMFLAGS(like TMJOIN etc)
 */
#define XA_TRANS_NOFLAGS 0 /* TMNOFLAGS in xa.h */
#define XA_TRANS_FOR_RECOVERY_OPS 1 /* recovery txn, no new trans or join/resume trans */
#define XA_TRANS_TMJOIN 2 /* Join ongoing XA TXN */
#define XA_TRANS_TMRESUME 3 /* Resume a suspended XA TXN */

/*
 * flags for XA TRANS_COMMIT
 * used by opcode PCM_OP_TRANS_COMMIT for XA in   PIN_FLD_FLAGS field.
 *
 */
#define XA_TRANS_TWOPHASE 0 /* TWO_PHASE is assume default for XA in BRM */
#define XA_TRANS_ONEPHASE 1 /* TMONEPHASE in xa.h */

#undef EXTERN

#ifdef __cplusplus
}
#endif

#endif /* !_PCM_H */
