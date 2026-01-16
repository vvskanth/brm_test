/*
 *      @(#) % %
 *    
* Copyright (c) 1996, 2023, Oracle and/or its affiliates. 
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _DM_DEBUG_H
#define _DM_DEBUG_H

/*
 * defines for the DM debug variables
 */

/*
 * dm_debug is the top level, used for flow tracing of operations
 */
/* 1st nibble, gross transaction trace */
#define DM_DEBUG_TRANS_IN	0x00000001
#define DM_DEBUG_TRANS_OUT	0x00000002
#define DM_DEBUG_MAIN		0x00000004	/* main loop */
#define DM_DEBUG_SNMP		0x00000008	/* snmp process */

/* 2nd nibble, detailed transaction trace */
#define DM_DEBUG_TRANS_IN_PR	0x00000010
#define DM_DEBUG_TRANS_OUT_PR	0x00000020
#define DM_DEBUG_TRANS_TRACE	0x00000040	/* trace transaction stuff */

/* 2nd and 3rd bytes - trace ops */
#define DM_DEBUG_OP_CREATE	0x00000100	/* create object */
#define DM_DEBUG_OP_DELETE	0x00000200	/* delete object */
#define DM_DEBUG_OP_ROBJ	0x00000400	/* read entire object */
#define DM_DEBUG_OP_RFLDS	0x00000800	/* read list of fields */
#define DM_DEBUG_OP_WFLDS	0x00001000	/* write list of fields */
#define DM_DEBUG_OP_DFLDS	0x00002000	/* delete list of fields */
#define DM_DEBUG_OP_SEARCH	0x00004000	/* search */
#define DM_DEBUG_OP_INCFLDS	0x00008000	/* increment field(s) */
#define DM_DEBUG_OP_DECFLDS	0x00010000	/* decrement field(s) */
#define DM_DEBUG_OP_LOGIN	0x00020000	/* "login" */
#define DM_DEBUG_OP_LOGOFF	0x00040000	/* "logoff" */
#define DM_DEBUG_OP_STEPSRCH	0x00080000	/* "step searching" */
#define DM_DEBUG_OP_DD		0x00100000	/* GET_DD/SET_DD */

/* 4th byte - error printf enable */
#define DM_DEBUG_OP_ERROR	0x01000000	/* if error, print */
#define DM_DEBUG_ERROR_LOSTCONN	0x02000000	/* details on lost connects */
#define DM_DEBUG_ERROR_LOSTCONN2	0x04000000	/* more ... on lost connects */

#define DM_DEBUG_MULTIDB        0x20000000      /* trace MultiDB stuff */
#define DM_DEBUG_AUDITABILITY	0x40000000
#define DM_DEBUG_SPROC 		0x80000000	/* Stored Procedure execution */




/*
 * dm_debug2 is details of mapping fields to sql
 */
/* lower 16 bits is gross stuff */
#define DM_DEBUG2_MPTTI_MATCH_TYPE_ENTRY	0x00000001
#define DM_DEBUG2_MAP_POID_TO_TABLE_INFO	0x00000002
#define DM_DEBUG2_MAP_POID_TYPE_TO_CWL_FLAG	0x00000004
#define DM_DEBUG2_CONS_DEFAULT_FIELD		0x00000008	/* NYD */
#define DM_DEBUG2_VERIFY_CREATE_FIELDS		0x00000010
#define DM_DEBUG2_BINDING_INFO_OF_FIELD		0x00000020
#define DM_DEBUG2_MAKE_SELECT_COLUMN_LIST	0x00000040
#define DM_DEBUG2_MAKE_INSERT_COLUMN_LISTS	0x00000080
#define DM_DEBUG2_MAKE_UPDATE_COLUMN_LIST	0x00000100
#define DM_DEBUG2_MAKE_INCDEC_COLUMN_LIST	0x00000200
#define DM_DEBUG2_UNICODE_CONVERSION		0x00000400
#define DM_DEBUG2_GET_TABLE_TYPE_ID             0x00000800

/*
 * dm_debug3 is details of doing sql and misc. (dm_subr) and dd_locks
 */
/* lower 16 bits is tracing stuff */
#define DM_DEBUG3_ORACLE_ERR			0x00000001
#define DM_DEBUG3_GET_NEW_POID_ID		0x00000002
#define DM_DEBUG3_DO_SQL			0x00000004
#define DM_DEBUG3_DO_SQL_SELECT			0x00000008
#define DM_DEBUG3_DO_SQL_INSERT			0x00000010
#define DM_DEBUG3_DO_SQL_UPDATE			0x00000020
#define DM_DEBUG3_CONS_TYPE_AND_DATA_FROM_NAME	0x00000040
#define DM_DEBUG3_CONS_FIELD_LIST		0x00000080
#define DM_DEBUG3_GET_N_ROWS			0x00000100
#define DM_DEBUG3_MAKE_TOP_LEVEL_FIELDS		0x00000200
#define DM_DEBUG3_DM_READ_SUBROW		0x00000400
#define DM_DEBUG3_DM_READ_SUBROWS_WILD		0x00000800
#define DM_DEBUG3_GET_FIELD_VALUE		0x00001000
#define DM_DEBUG3_DD_LOCKS			0x00002000
#define DM_DEBUG3_SQL_COL_VALS			0x00004000
#define DM_DEBUG3_DO_SQL_MR_SELECT		0x00008000

/* defined to log the DM Time Info */
#define DM_DEBUG3_TIME_INFO                     0x00010000

/* upper 16 bits is errors stuff */
#define DM_DEBUG3_GET_NEW_POID_ID_ERR		0x00020000
#define DM_DEBUG3_DO_SQL_ERR			0x00040000
#define DM_DEBUG3_DO_SQL_SELECT_ERR		0x00080000
#define DM_DEBUG3_DO_SQL_INSERT_ERR		0x00100000
#define DM_DEBUG3_DO_SQL_UPDATE_ERR		0x00200000
#define DM_DEBUG3_CONS_TYPE_AND_DATA_FROM_NAME_ERR	0x00400000
#define DM_DEBUG3_CONS_FIELD_LIST_ERR		0x00800000
#define DM_DEBUG3_GET_N_ROWS_ERR		0x01000000
#define DM_DEBUG3_MAKE_TOP_LEVEL_FIELDS_ERR	0x02000000
#define DM_DEBUG3_DM_READ_SUBROW_ERR		0x04000000
#define DM_DEBUG3_DM_READ_SUBROWS_WILD_ERR	0x08000000
#define DM_DEBUG3_GET_FIELD_VALUE_ERR		0x10000000
#define DM_DEBUG3_TXN_RECORD			0x20000000
#define DM_DEBUG3_ORACLE_ERR_ERR                0x40000000

/* dm_debug4 for Timesten */
#define DM_DEBUG4_TIMESTEN			0x00000001
#define DM_DEBUG4_SINGLE_ROW_EVENT		0x00000002
#define DM_DEBUG4_DUAL_TRANSACTION		0x00000004
#define DM_DEBUG4_HYBRID_EVENT_SEARCH		0x00000008
#define DM_DEBUG4_GLOBAL_PROCESSING		0x00000010
#define DM_DEBUG4_UNION_SEARCH			0x00000020

/* dm_debug5 for AQ enqueuing */
#define DM_DEBUG5_AQUEUE                        0x00000001
#define DM_DEBUG5_ADD_MESSAGE                   0x00000002
#define DM_DEBUG5_ENQUEUE_MESSAGE               0x00000004

/* Symbolic constants defined for die_on_exception */

#define DM_DIE_NOT_SET		-1 /* Indicates it hasn't been specified */
#define DM_DIE_CONTINUE		0  /* Indicates the DM should continue */
#define DM_DIE_DIE		1  /* Indicates the DM should die */

#ifdef WIN32
/* export in dmpin.dll, import in dm.exe and dm_oracle8(i).dll */
#if defined(_DM_UTILS_) && !defined(_DM_DEBUG_IMPORT_) 
EXPORT int dm_debug;
EXPORT int dm_debug2;
EXPORT int dm_debug3;
EXPORT int dm_debug4;
EXPORT int dm_debug5;
EXPORT int cfg_dna_sat;
EXPORT int nt_service;
EXPORT int die_on_exception;
#else
IMPORT int dm_debug;
IMPORT int dm_debug2;
IMPORT int dm_debug3;
IMPORT int dm_debug4;
IMPORT int dm_debug5;
IMPORT int cfg_dna_sat;
IMPORT int nt_service;
IMPORT int die_on_exception;
#endif /*_DM_UTILS_ and _DM_DEBUG_IMPORT_*/

#else /* UNIX */

/* defined in dm.main.c, used in dm.exe and dm_oracle8(i) */
extern int dm_debug;
extern int dm_debug2;
extern int dm_debug3;
extern int dm_debug4;
extern int dm_debug5;
extern int cfg_dna_sat;

#endif /*WIN32*/

#endif /*_DM_DEBUG_H*/
