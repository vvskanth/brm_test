/*
 *	@(#) % %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef __PIN_ARCHIVE_H
#define __PIN_ARCHIVE_H


/*
 *	pinlog() info for load_pin_archive application.
 */
#define LOAD_PIN_ARCHIVE_PC_PROG       "load_pin_archive"
#define LOAD_PIN_ARCHIVE_PC_LOGFILE    "logfile"
#define LOAD_PIN_ARCHIVE_PC_LOGLEVEL   "loglevel"

/*
 *	pinlog() info for pin_pre_archive application.
 */
#define PIN_PRE_ARCHIVE_PC_PROG       "pin_pre_archive"
#define PIN_PRE_ARCHIVE_PC_LOGFILE    "logfile"
#define PIN_PRE_ARCHIVE_PC_LOGLEVEL   "loglevel"

/*
 *	pin.conf options for pin_pre_archive application.
 */
#define PIN_PRE_ARCHIVE_PC_MIN_AGING		"min_aging"
#define PIN_PRE_ARCHIVE_PC_MAX_AGING		"max_aging"
#define PIN_PRE_ARCHIVE_PC_BATCH_SIZE		"batch_size"
#define PIN_PRE_ARCHIVE_PC_LAST_ITEM_POID	"last_item_poid"
#define PIN_PRE_ARCHIVE_PC_DB_FILTER_EVENTS	"db_filter_events"
#define PIN_PRE_ARCHIVE_PC_CHECK_BAL_IMPACTS	"check_bal_impacts"
#define PIN_PRE_ARCHIVE_PC_EVENT_PER_STEP	"event_per_step"

/*
 *	pin.conf options used by mta
 */
#define PIN_PRE_ARCHIVE_PC_MAX_TIME		"max_time"
#define PIN_PRE_ARCHIVE_PC_RETRY_MTA_SRCH	"retry_mta_srch"

/*
 *	"hidden" pin.conf options for pin_pre_archive application.
 */
#define PIN_PRE_ARCHIVE_PC_ITEM_FILTER		"item_filter"
#define PIN_PRE_ARCHIVE_PC_USE_DISTINCT		"use_distinct"

/*
 * Global state of data archiving.
 */
typedef enum pin_archive_state {
	PIN_ARCHIVE_STATE_SESSION_INACTIVE		= 0x00,
	PIN_ARCHIVE_STATE_LOAD_CFG_INPROG		= 0x01,
	PIN_ARCHIVE_STATE_LOADED_CFG			= 0x02,
	PIN_ARCHIVE_STATE_PRE_ARCHIVE_PREPARE_INPROG	= 0x04,
	PIN_ARCHIVE_STATE_PRE_ARCHIVE_PREPARED		= 0x08,
	PIN_ARCHIVE_STATE_PRE_ARCHIVE_ABORT_INPROG	= 0x10,
	PIN_ARCHIVE_STATE_ARCHIVING_INPROG		= 0x20,
	PIN_ARCHIVE_STATE_PAST_FE_ABORT_STAGE		= 0x40,
	PIN_ARCHIVE_STATE_UNKNOWN			= 0x80
} pin_archive_state_t;

/*
 * Useful info for doing global state transitions. 
 *
 * RESOLVE: eventually move all this into a static transition table
 * so that simple lookups can be done.
 */
typedef enum pin_archive_state_op {
	PIN_ARCHIVE_STATE_OP_LOAD_START			= 0x00,
	PIN_ARCHIVE_STATE_OP_LOAD_FINISH		= 0x01,
	PIN_ARCHIVE_STATE_OP_LOAD_ABORT			= 0x02,
	PIN_ARCHIVE_STATE_OP_PREPARE_START		= 0x04,
	PIN_ARCHIVE_STATE_OP_PREPARE_FINISH		= 0x08,
	PIN_ARCHIVE_STATE_OP_PREPARE_ABORT		= 0x10,
	PIN_ARCHIVE_STATE_OP_ABORT_START		= 0x20,
	PIN_ARCHIVE_STATE_OP_ABORT_FINISH		= 0x40,
	PIN_ARCHIVE_STATE_OP_ABORT_ABORT		= 0x80,
	PIN_ARCHIVE_STATE_OP_UNKNOWN			= 0x100
} pin_archive_state_op_t;


/*
 *
 */
#define PIN_ARCHIVE_CFG_BACKUP_STR		"backup"
#define PIN_ARCHIVE_CFG_PUSH_STR		"push"
#define PIN_ARCHIVE_CFG_DELETE_STR		"delete"

/*
 *
 */
typedef enum pin_archive_cfgtype {
	PIN_ARCHIVE_CFG_BACKUP		= 1,
	PIN_ARCHIVE_CFG_PUSH		= 2,
	PIN_ARCHIVE_CFG_DELETE		= 3
} pin_archive_cfgtype_t; 


/*
 *
 */
#define PIN_PRE_ARCHIVE_OP_PREPARE_STR		"-prepare"
#define PIN_PRE_ARCHIVE_OP_ABORT_STR		"-abort"
#define PIN_PRE_ARCHIVE_OP_VALIDATE_STR		"-validate"

/*
 *
 */
typedef enum pin_pre_archive_optype {
	PIN_PRE_ARCHIVE_OP_PREPARE		= 1,
	PIN_PRE_ARCHIVE_OP_ABORT		= 2,
	PIN_PRE_ARCHIVE_OP_VALIDATE		= 3
} pin_pre_archive_optype_t;


/*
 * Archiving state of an archivable object.
 *
 * WARNING: do not switch the order of these states, search template
 * 208 depends on the order!
 */
typedef enum pin_archive_objstate {
	PIN_ARCHIVE_OBJSTATE_NULL			= 0x00,
	PIN_ARCHIVE_OBJSTATE_TO_BE_BACKED_UP		= 0x01,
	PIN_ARCHIVE_OBJSTATE_TO_BE_PUSHED		= 0x02,
	PIN_ARCHIVE_OBJSTATE_TO_BE_DELETED		= 0x04,
	PIN_ARCHIVE_OBJSTATE_IN_STAGING_AREA		= 0x08,
	PIN_ARCHIVE_OBJSTATE_BACKED_UP			= 0x10,
	PIN_ARCHIVE_OBJSTATE_PUSHED			= 0x20,
	PIN_ARCHIVE_OBJSTATE_CONSUMED			= 0x40
} pin_archive_objstate_t;

/*
 * Archiving state of an archivable object's parent.
 *
 * WARNING: do not switch the order of these states, search template
 * 709 depends on the order!
 */
typedef enum pin_archive_item_status {
	PIN_ARCHIVE_ITEM_STATUS_NULL			= 0x00,
	PIN_ARCHIVE_ITEM_STATUS_ARCHIVED		= 0x01, 
	PIN_ARCHIVE_ITEM_STATUS_PREPARED		= 0x02,
	PIN_ARCHIVE_ITEM_STATUS_PREPARED_AND_ARCHIVED	= 0x04
} pin_archive_item_status_t;

#endif 
