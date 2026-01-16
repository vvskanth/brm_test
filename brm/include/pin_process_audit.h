/*******************************************************************
 *
 *      Copyright (c) 2004 - 2007 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef _PIN_PROCESS_AUDIT_H
#define _PIN_PROCESS_AUDIT_H

/**
 * Flag whether to create default links or not
**/
#define CREATE_DEFAULT_LINKS	1

/**
 * CM's pin.conf entry names
**/
#define PIN_PROCESS_AUDIT_MODULE_NAME						"fm_process_audit"
#define PIN_PROCESS_AUDIT_ARG_ENABLE_DEFAULT_LINKS			"enable_default_links"
#define PIN_PROCESS_AUDIT_ARG_ENABLE_OUTOFORDER_LINKCORRECTION		"enable_outoforder_linkcorrection"
#define PIN_PROCESS_AUDIT_ARG_WRITEOFF_CONTROL_POINT_ID		"writeoff_control_point_id"
#define PIN_PROCESS_AUDIT_ARG_WRITEOFF_BATCH_ID_PREFIX		"writeoff_batch_id_prefix"

/**
 * possible values for batch type
**/
#define BATCH_TYPE_UNKNOWN		-1
#define BATCH_TYPE_MEDIATION	0
#define BATCH_TYPE_RERATING		1
#define BATCH_TYPE_RECYCLING	2
#define BATCH_TYPE_WRITEOFF		3
#define BATCH_TYPE_MEDIATION_UPDATE	4
#define BATCH_TYPE_RERATING_UPDATE	5

/**
 * PIN_FLD_FLAGS for RA Search opcode
**/
#define PIN_RA_SRCH_FLAG_NO_VIEW_BY_BID_TOTAL_CNT 0x01

/**
 * possible values for RA search scope 
**/
#define RA_RATING	0
#define RA_RERATING	1

/**
 * possible action values
**/
#define ACTION_CREATE_NEW_PROCESS_AUDIT_OBJECT		"CREATE_NEW"
#define	ACION_UPDATE_EXISTING_PROCESS_AUDIT_OBJECT	"UPDATE_EXISTING"

/**
 * global flags and variables for functional control
**/
extern int32	g_enable_default_links;
extern int32	g_enable_outoforder_linkcorrection;
extern char		g_writeoff_control_point_id[81];
extern char		g_writeoff_batch_id_prefix[81];

typedef enum proc_status {
	NORMAL = 0,
	RECYCLE = 1,
	RECYCLE_TEST= 2
} proc_status_t;

#endif

