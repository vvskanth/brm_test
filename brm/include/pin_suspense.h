/*
 *	@(#) % %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef PIN_SUSPENSE_H
#define PIN_SUSPENSE_H

/************************************************************************
 * Suspense State 
 ***********************************************************************/
typedef enum pin_suspense_status {
        PIN_SUSPENSE_SUSPENDED		= 0,
        PIN_SUSPENSE_RECYCLING		= 1,
        PIN_SUSPENSE_SUCCEEDED		= 2,
        PIN_SUSPENSE_WRITTEN_OFF	= 3,
        PIN_SUSPENSE_DELETE_PENDING	= 4
}pin_suspense_status_t;

/************************************************************************
 * Suspense Action 
 ***********************************************************************/
typedef enum pin_suspense_action {
        PIN_SUSPENSE_ACTION_RECYCLE	= 0,
        PIN_SUSPENSE_ACTION_EDIT	= 1,
        PIN_SUSPENSE_ACTION_DELETE	= 2,
        PIN_SUSPENSE_ACTION_WRITE_OFF	= 3
}pin_suspense_action_t;

/************************************************************************
 * Maximum number of "undo edits" on suspended usage edit
 ***********************************************************************/
#define PIN_MAX_SUSP_UNDO_EDITS 	20

/************************************************************************
 * Constants for suspense admin action objects
 * Will be used for storing action_string in suspended usage object
 * for bulk actions. These constants are kept same as the dd type id for
 * these objects
 ***********************************************************************/
#define PIN_SUSP_ACTION_RYCYCLE_DD_TYPE_ID      1015
#define PIN_SUSP_ACTION_WRITEOFF_DD_TYPE_ID     1017
#define PIN_SUSP_ACTION_EDIT_DD_TYPE_ID         2102


/************************************************************************
 * Other constants 
 ***********************************************************************/
#define PIN_INIT_TOP_INDEX 		0
#define PIN_INIT_BOTTOM_INDEX 		0
#define PIN_SUSP_EDIT_OBJ_SUCCESS 	0
#define PIN_SUSP_UNDO_EDIT_SUCCESS 	0
#define PIN_SUSP_UNDO_EDIT_FAILED 	1


/************************************************************************
 * Suspense Admin Action Object Type 
 ***********************************************************************/
#define PIN_OBJ_TYPE_ACTION_RECYCLE	"/admin_action/suspended_usage/recycle"
#define PIN_OBJ_TYPE_ACTION_WRITE_OFF	"/admin_action/suspended_usage/writeoff"
#define PIN_OBJ_TYPE_ACTION_EDIT	"/admin_action/suspended_usage/edit"

/************************************************************************
 * Object Name Strings
 ***********************************************************************/
#define PIN_OBJ_NAME_EVENT_SUSPENSE         	"Suspense Event Log"
#define PIN_OBJ_NAME_EVENT_SUSPENSE_RECYCLE   	"Suspense Recycle Event Log"
#define PIN_OBJ_NAME_EVENT_SUSPENSE_DELETE   	"Suspense Delete Event Log"
#define PIN_OBJ_NAME_EVENT_SUSPENSE_EDIT   	"Suspense Edit Event Log"
#define PIN_OBJ_NAME_EVENT_SUSPENSE_WRITE_OFF	"Suspense Write Off Event Log"


/************************************************************************
 * Suspense event type strings
 ***********************************************************************/
#define PIN_OBJ_TYPE_EVENT_SUSPENSE		"/event/notification/suspense"
#define PIN_OBJ_TYPE_EVENT_SUSPENSE_RECYCLE	"/event/notification/suspense/recycle"
#define PIN_OBJ_TYPE_EVENT_SUSPENSE_DELETE	"/event/notification/suspense/delete"
#define PIN_OBJ_TYPE_EVENT_SUSPENSE_EDIT	"/event/notification/suspense/edit"
#define PIN_OBJ_TYPE_EVENT_SUSPENSE_WRITE_OFF	"/event/notification/suspense/writeoff"

#define PIN_EVENT_DESCR_SUSPENSE_RECYCLE	"Suspense Recycle"
#define PIN_EVENT_DESCR_SUSPENSE_DELETE		"Suspense Delete"
#define PIN_EVENT_DESCR_SUSPENSE_EDIT		"Suspense Edit"
#define PIN_EVENT_DESCR_SUSPENSE_WRITE_OFF	"Suspense Write Off"

/************************************************************************
 * Other Suspense Object Type 
 ***********************************************************************/
#define PIN_OBJ_TYPE_SUSP_USAGE_EDITS		"/suspended_usage_edits"

#endif /* PIN_SUSPENSE_H */
