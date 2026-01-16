/*	
 *	
* Copyright (c) 1996, 2023, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

/*
 * This file contains the opcode definitions for the Base Opcode PCM API.
 */

#ifndef _PCM_BASE_OPS_H_
#define _PCM_BASE_OPS_H_

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_BASE_OPS: 0..49; 0..47; 49..49; None
 */

#define PCM_OP_INVALID		0	/* invalid operation */
#define PCM_OP_CREATE_OBJ	1	/* create object */
#define PCM_OP_DELETE_OBJ	2	/* delete object */
#define PCM_OP_READ_OBJ		3	/* read entire object */
#define PCM_OP_READ_FLDS	4	/* read list of fields */
#define PCM_OP_WRITE_FLDS	5	/* write list of fields */
#define PCM_OP_DELETE_FLDS	6	/* delete list of fields */
#define PCM_OP_SEARCH		7	/* search */
#define PCM_OP_INC_FLDS		8	/* increment field(s) */
#define PCM_OP_LOGIN		9	/* "login" */
#define PCM_OP_LOGOFF		10	/* "logoff" */
#define PCM_OP_TEST_LOOPBACK	11	/* dup input flist & return */
#define PCM_OP_TRANS_OPEN	12	/* transaction open */
#define PCM_OP_TRANS_ABORT	13	/* transaction abort */
#define PCM_OP_TRANS_COMMIT	14	/* transaction commit */
#define PCM_OP_PASS_THRU	15	/* invoke pass_thru function */
#define PCM_OP_GET_CM_STATS	16	/* get CM status from CM */
#define PCM_OP_STEP_SEARCH	17	/* invoke step searching 	*/
#define PCM_OP_STEP_NEXT	18	/* get the next set of results	*/
#define PCM_OP_STEP_END		19	/* get the rest of the results	*/
#define PCM_OP_GET_DD           20      /* get some DM schema config info */
#define PCM_OP_SET_DD           21      /* set some DM schema config info */
#define PCM_OP_TRANS_POL_OPEN	22	/* transaction open policy */
#define PCM_OP_TRANS_POL_ABORT	23	/* transaction abort policy */
#define PCM_OP_TRANS_POL_COMMIT	24	/* transaction commit policy */
#define PCM_OP_GLOBAL_SEARCH	25	/* Global search */
#define PCM_OP_GLOBAL_STEP_SEARCH 26	/* invoke Global step searching */
#define PCM_OP_GLOBAL_STEP_NEXT	27	/* get the next set of results	*/
#define PCM_OP_GLOBAL_STEP_END	28	/* get the rest of the results	*/
#define PCM_OP_SET_DM_CREDENTIALS 29	/* Set DM Credentials -- 
					   only for CM's use */
#define PCM_OP_TRANS_POL_PREP_COMMIT	30 /* pre trans commit policy */
#define PCM_OP_EXEC_SPROC       31      /* Execute stored procedure */
#define PCM_OP_LOCK_OBJ         32      /* Lock an object */
#define PCM_OP_GET_POID_IDS     33      /* Get a range of poid ids */
#define PCM_OP_GET_SERVER_TIME	34	/* Gets cm server time */

/* These are used for ROC object synchronization between dm and timos */

#define PCM_OP_SYNC_PRE_COMMIT	35	/* ROC Sync Pre-Commit operation */
#define PCM_OP_SYNC_POST_COMMIT	36	/* ROC Sync Post-Commit operation */
#define PCM_OP_SYNC_ROLLBACK	37	/* ROC Sync Rollback operation */

#define PCM_OP_GET_PIN_VIRTUAL_TIME 38	/* Get pin_virtual_time */
#define PCM_OP_LICENSE_VALIDATE_FEATURE 39 /* Validate feature */

#define PCM_OP_BULK_WRITE_FLDS        40      /* Bulk update */
#define PCM_OP_BULK_DELETE_OBJ        41      /* Bulk delete */
#define PCM_OP_BULK_CREATE_OBJ        42      /* Bulk create */

/* These are used for hot standby synchronization between TIMOS HA pair */
#define PCM_OP_HOT_SYNC_PRE_COMMIT    43      /* TIMOS HotStandby pre-commit operation*/
#define PCM_OP_HOT_SYNC_POST_COMMIT   44      /* TIMOS HotStandby post-commit operation*/

/* Used to initiate graceful switchover and switchover related operations */
#define PCM_OP_SWITCHOVER      45      /* TIMOS Graceful Switchover related operations */

/* Issued by Active Timos to migrate objects in payload data to passive  */
#define PCM_OP_BULK_DATA_SYNCHRONIZATION    46   /* Passive instance syncs it's memory with this data */

/* This is used for heart beats between TIMOS HA pair */
#define PCM_OP_HEART_BEAT             47      /* TIMOS HeartBeat operation*/
#define PCM_OP_ENQUEUE                48      /* AQ Enqueue */

/* This is used in XA Transactions before COMMIT */
#define PCM_OP_TRANS_PREPARE      49      /* XA PREPARE */

#define PCM_OP_CM_PRE_LOCK_OBJ	9201	/* pre policy */
#define PCM_OP_CM_PRE_PARTITION_PRUNE   9202    /* pre partition_prune */
#define PCM_OP_TRANS_RECOVER   9203    /* XA RECOVER */
#define PCM_OP_TRANS_FORGET   9204    /* XA FORGET */
#define PCM_OP_UPDATE_DATA_KEYS	9205
#define PCM_OP_SET_ROOT_KEYS	9206
#define PCM_OP_GET_ROOT_KEYS	9207
        /* reserve 9208 - 9300 */

#endif /* !_PCM_BASE_OPS_H */
