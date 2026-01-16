/* Continuus file information --- %full_filespec: pin_device.h~10:incl:1 % */
/*
 * @(#) %full_filespec: pin_device.h~10:incl:1 %
 *
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef PIN_DEVICE_H
#define PIN_DEVICE_H

/***********************************************************************
 * Device state types.
 ***********************************************************************/
typedef enum pin_device_state_type {
	PIN_DEVICE_RAW_STATE		= 		0,
	PIN_DEVICE_INIT_STATE		=		1,
	PIN_DEVICE_NORMAL_STATE		=		2,
	PIN_DEVICE_TERM_STATE		=		3		
} pin_device_state_type_t;

/* used for opcode PCM_OP_DEVICE_ASSOCIATE*/
#define PIN_DEVICE_FLAG_ASSOCIATE		0x01

/* used for opcode PCM_OP_DEVICE_DELETE*/
#define PIN_DEVICE_FLAG_ASSOCIATE_CHECK		0x01

/************************************************************************
 * Object Name Strings
 ***********************************************************************/
#define PIN_OBJ_NAME_EVENT_DEVICE         	"Device Event Log"
#define PIN_OBJ_NAME_EVENT_DEVICE_CREATE   	"Device Creation Event Log"
#define PIN_OBJ_NAME_EVENT_DEVICE_DELETE   	"Device Delete Event Log"
#define PIN_OBJ_NAME_EVENT_DEVICE_ASSOCIATE   	"Device Association Event Log"
#define PIN_OBJ_NAME_EVENT_DEVICE_DISASSOCIATE	"Device Disassociation Event Log"
#define PIN_OBJ_NAME_EVENT_DEVICE_ATTRIBUTE	"Device Attribute Change Event Log"
#define PIN_OBJ_NAME_EVENT_DEVICE_STATE		"Device State Change Event Log"
#define PIN_OBJ_NAME_EVENT_DEVICE_REPLACE	"Device Replace Event Log"
#define PIN_OBJ_NAME_EVENT_DEVICE_BRAND		"Device Brand Event Log"
#define PIN_OBJ_NAME_EVENT_DEVICE_STATE_NOTIFY	"Device State Notification Event"
#define PIN_OBJ_NAME_EVENT_DEVICE_TRANSITION_NOTIFY "Device State Transition Notification Event" 


/************************************************************************
 * Device event type strings
 ***********************************************************************/
#define PIN_OBJ_TYPE_EVENT_DEVICE		"/event/device"
#define PIN_OBJ_TYPE_EVENT_DEVICE_CREATE	"/event/device/create"
#define PIN_OBJ_TYPE_EVENT_DEVICE_DELETE	"/event/device/delete"
#define PIN_OBJ_TYPE_EVENT_DEVICE_ASSOCIATE	"/event/device/associate"
#define PIN_OBJ_TYPE_EVENT_DEVICE_DISASSOCIATE	"/event/device/disassociate"
#define PIN_OBJ_TYPE_EVENT_DEVICE_ATTRIBUTE	"/event/device/attribute"
#define PIN_OBJ_TYPE_EVENT_DEVICE_STATE		"/event/device/state"
#define PIN_OBJ_TYPE_EVENT_DEVICE_REPLACE	"/event/device/replace"
#define PIN_OBJ_TYPE_EVENT_DEVICE_BRAND		"/event/device/brand"
#define PIN_OBJ_TYPE_EVENT_DEVICE_STATE_NOTIFY  "/event/notification/device/state"
#define PIN_OBJ_TYPE_EVENT_DEVICE_TRANSITION_NOTIFY "/event/notification/device/state/in_transition" 

#define PIN_EVENT_DESCR_DEVICE_CREATE	"Device Create"
#define PIN_EVENT_DESCR_DEVICE_DELETE	"Device Delete"
#define PIN_EVENT_DESCR_DEVICE_ASSOCIATE	"Device Associate"
#define PIN_EVENT_DESCR_DEVICE_DISASSOCIATE	"Device Disassociate"
#define PIN_EVENT_DESCR_DEVICE_ATTRIBUTE	"Device Set Attribute"
#define PIN_EVENT_DESCR_DEVICE_STATE	"Device Set State"
#define PIN_EVENT_DESCR_DEVICE_REPLACE	"Device Replace"
#define PIN_EVENT_DESCR_DEVICE_BRAND	"Device Set Brand"

#endif /* PIN_DEVICE_H */
