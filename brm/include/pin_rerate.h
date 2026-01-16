/*
 * @(#)$Id: pin_rerate.h /cgbubrm_mainbrm.portalbase/1 2017/01/29 23:15:42 ckm Exp $ 
 *
 *      
* Copyright (c) 1996, 2017, Oracle and/or its affiliates. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PIN_RERATE_H
#define	_PIN_RERATE_H

/*******************************************************************
 * Rerating FM Definitions.
 *
 * All values defined here are embedded in the database
 * and therefore *cannot* change!
 *******************************************************************/

/*******************************************************************
 * /job/rerate PIN_FIELD_STATUS Values
 * Don't change values, used by job_rerate_procedures.sql as well
 *******************************************************************/
#define PIN_RERATE_STATUS_UNINITIALIZED         0
#define PIN_RERATE_STATUS_NEW                   1
#define PIN_RERATE_STATUS_WAITING_ACCOUNT_LOCK  2
#define PIN_RERATE_STATUS_ACCOUNT_LOCKED        3
#define PIN_RERATE_STATUS_RERATED               4
#define PIN_RERATE_STATUS_READY_FOR_RECYCLE     5
#define PIN_RERATE_STATUS_RERATING_FAILED       6
#define PIN_RERATE_STATUS_UNSUCCESSFUL          7
#define PIN_RERATE_STATUS_UPDATING              8
#define PIN_RERATE_STATUS_IN_PROCESS            9
#define PIN_RERATE_STATUS_RECYCLE_IN_PROGRESS   10
#define PIN_RERATE_STATUS_ACCOUNT_LOCK_FAILED	11
#define PIN_RERATE_STATUS_COMPLETED             -1

/*******************************************************************
 * OP Flags specific to the Rerate FM.
 * (op specific flags go in high 16 bits)
 *******************************************************************/
#define PCM_OPFLG_REQUEST_FAILED		0x01000000

/*******************************************************************
 * fm_rerate #defines
 *******************************************************************/
/* Act Usage */
#define PIN_RERATE_HOLD_CDR_EVENT_NAME     "PrepareToRerate"
#define PIN_RERATE_HOLD_CDR_EVENT_TYPE     "/event/notification/rerating/PrepareToRerate"
#define PIN_RERATE_RESUME_CDR_EVENT_NAME   "ReratingCompleted"
#define PIN_RERATE_RESUME_CDR_EVENT_TYPE   "/event/notification/rerating/ReratingCompleted"

/* PCM_OP_EXEC_SPROC */
#define PIN_RERATE_PREPARE_TO_RERATE       "PrepareToRerate"
#define PIN_RERATE_UPDATE_RERATE_JOB_SPROC "RERATING.update_rerate_job"
#define PIN_RERATE_UPDATE_RERATE_JOB_SPROC_SQL "update_rerate_job"
#define PIN_RERATE_RERATING_COMPLETED      "ReratingCompleted"

/*******************************************************************
 * Flags used by rerating
 *******************************************************************/
#define PIN_RERATE_FLG_NO_BAL_UPDATE    0x10000

#endif	/*_PIN_RERATE_H*/

