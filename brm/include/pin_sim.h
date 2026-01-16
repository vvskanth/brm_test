/*
 * @(#)%Portal Version: pin_sim.h:BillingVelocityInt:2:2006-Sep-14 18:45:59 %
 *
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */
#ifndef _1_pin_sim_h_H
#define _1_pin_sim_h_H


/*******************************************************************
 * Object Type Strings
 *******************************************************************/

#define PIN_OBJ_TYPE_CONFIG_SVC_PREPRO  "/config/pre_provisioning_sim"
#define PIN_OBJ_TYPE_DEVICE_SIM		"/device/sim"
#define PIN_OBJ_TYPE_ORDER_SIM		"/order/sim"

/*******************************************************************
 * SIM device initial default state 
 *******************************************************************/

#define PIN_SIM_DEFAULT_STATE		1

#define PIN_SIM_DEFAULT_PROGRAM_NAME	"SIM Manager"

/****************************************************************************
 * SIM Device state types
 ****************************************************************************/

typedef enum pin_sim_state_type {
	PIN_SIM_RAW_STATE			=	0,
        PIN_SIM_NEW_STATE			=	1,
        PIN_SIM_RELEASED_STATE			=	2,
        PIN_SIM_PROVISIONING_STATE		=	3,
        PIN_SIM_FAILED_PROVISIONING_STATE	=	4,
        PIN_SIM_ASSIGNED_STATE			=	5,
        PIN_SIM_UNASSIGNED_STATE		=	6
} pin_sim_state_type_t;

/****************************************************************************
 * Order object status types
 ****************************************************************************/

typedef enum pin_order_status {
	PIN_ORDER_NEW			= 1,
	PIN_ORDER_REQUEST		= 2,
	PIN_ORDER_RECEIVED		= 3,
	PIN_ORDER_PARTIAL_RECEIVE 	= 4,
	PIN_ORDER_CANCELLED		= 5
}pin_order_status_t;

typedef enum pin_order_card_type {
	PIN_CARD_TYPE_ISO		= 1,
	PIN_CARD_TYPE_PLUGIN		= 2 
}pin_order_card_type_t;


/****************************************************************************
 * Order object status types for Order framework
 ****************************************************************************/
typedef enum pin_sim_order_status {
	PIN_SIM_ORDER_RAW_STATE			= 0,
	PIN_SIM_ORDER_NEW_STATE			= 1,
	PIN_SIM_ORDER_REQUEST_STATE		= 2,
	PIN_SIM_ORDER_RECEIVED_STATE		= 3,
	PIN_SIM_ORDER_PARTIAL_RECEIVED_STATE	= 4,
	PIN_SIM_ORDER_CANCELLED_STATE 		= 5
}pin_sim_order_status_t;

#endif
