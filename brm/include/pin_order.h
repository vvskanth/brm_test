/*
 * @(#)% %
 *
 * Copyright (c) 2002 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef PIN_ORDER_H
#define PIN_ORDER_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Order state types.
 ***********************************************************************/
typedef enum pin_order_state_type {
	PIN_ORDER_RAW_STATE		= 		0,
	PIN_ORDER_INIT_STATE		=		1,
	PIN_ORDER_NORMAL_STATE		=		2,
	PIN_ORDER_TERM_STATE		=		3		
} pin_order_state_type_t;

/* used for opcode PCM_OP_ORDER_ASSOCIATE*/
#define PIN_ORDER_FLAG_ASSOCIATE	0x01

/* define the device type */
#define PIN_OBJ_TYPE_DEVICE		"/device"

/* define the default order state  */
#define PIN_ORDER_DEFAULT_STATE		1

/* define the default device state  */
#define PIN_DEVICE_DEFAULT_STATE	1

/************************************************************************
 * Object Name Strings
 ***********************************************************************/
#define PIN_OBJ_NAME_EVENT_ORDER         	"Order Event Log"
#define PIN_OBJ_NAME_EVENT_ORDER_CREATE   	"Order Creation Event Log"
#define PIN_OBJ_NAME_EVENT_ORDER_DELETE   	"Order Delete Event Log"
#define PIN_OBJ_NAME_EVENT_ORDER_ASSOCIATE   	"Order Association Event Log"
#define PIN_OBJ_NAME_EVENT_ORDER_DISASSOCIATE	"Order Disassociation Event Log"
#define PIN_OBJ_NAME_EVENT_ORDER_ATTRIBUTE	"Order Attribute Change Event Log"
#define PIN_OBJ_NAME_EVENT_ORDER_STATE		"Order State Change Event Log"
#define PIN_OBJ_NAME_EVENT_ORDER_BRAND		"Order Brand Event Log"
#define PIN_OBJ_NAME_EVENT_ORDER_PROCESS	"Order Process Event Log"


/************************************************************************
 * Order event type strings
 ***********************************************************************/
#define PIN_OBJ_TYPE_EVENT_ORDER		"/event/order"
#define PIN_OBJ_TYPE_EVENT_ORDER_CREATE		"/event/order/create"
#define PIN_OBJ_TYPE_EVENT_ORDER_DELETE		"/event/order/delete"
#define PIN_OBJ_TYPE_EVENT_ORDER_ASSOCIATE	"/event/order/associate"
#define PIN_OBJ_TYPE_EVENT_ORDER_DISASSOCIATE	"/event/order/disassociate"
#define PIN_OBJ_TYPE_EVENT_ORDER_ATTRIBUTE	"/event/order/attribute"
#define PIN_OBJ_TYPE_EVENT_ORDER_STATE		"/event/order/state"
#define PIN_OBJ_TYPE_EVENT_ORDER_BRAND		"/event/order/brand"
#define PIN_OBJ_TYPE_EVENT_ORDER_PROCESS	"/event/order/process"
#define PIN_OBJ_TYPE_EVENT_ORDER_STATE_NOTIFY   "/event/notification/order/state"
#define PIN_OBJ_TYPE_EVENT_ORDER_TRANSITION_NOTIFY "/event/notification/order/state/in_transition" 


#define PIN_EVENT_DESCR_ORDER_CREATE	"Order Create"
#define PIN_EVENT_DESCR_ORDER_DELETE	"Order Delete"
#define PIN_EVENT_DESCR_ORDER_ASSOCIATE	"Order Associate"
#define PIN_EVENT_DESCR_ORDER_DISASSOCIATE	"Order Disassociate"
#define PIN_EVENT_DESCR_ORDER_ATTRIBUTE	"Order Set Attribute"
#define PIN_EVENT_DESCR_ORDER_STATE	"Order Set State"
#define PIN_EVENT_DESCR_ORDER_BRAND	"Order Set Brand"
#define PIN_EVENT_DESCR_ORDER_PROCESS	"Order Process"
#define PIN_OBJ_NAME_EVENT_ORDER_STATE_NOTIFY     "Order State Notification Event"
#define PIN_OBJ_NAME_EVENT_ORDER_TRANSITION_NOTIFY "Order State Transition Notification Event" 

#ifdef __cplusplus
}
#endif

#endif /* PIN_ORDER_H */
