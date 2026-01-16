/* @(#)%Portal Version: pin_ip.h:BillingVelocityInt:2:2006-Sep-14 18:46:44 % */
/*
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */
#ifndef _PIN_IP_H
#define _PIN_IP_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
 * Object Type Strings
 *******************************************************************/

#define PIN_OBJ_TYPE_DEVICE_IP		"/device/ip"

/*******************************************************************
 * IP device initial default state 
 *******************************************************************/

#define PIN_IP_DEFAULT_PROGRAM_NAME	"IP Manager"

/****************************************************************************
 * IP Device state types
 ****************************************************************************/

typedef enum pin_ip_state_type {
        PIN_IP_RAW_STATE               	= 	0,
        PIN_IP_NEW_STATE               	= 	1,
        PIN_IP_ALLOCATED_STATE          = 	2,
        PIN_IP_UNALLOCATED_STATE        = 	3,
        PIN_IP_RETURNED_STATE       	= 	4
} pin_ip_state_type_t;

typedef enum pin_ip_class_type {
	CLASS_A		= 	1,
	CLASS_B		= 	2,
	CLASS_C		= 	3
 } pin_ip_class_type;
 
#define PIN_IP_DEFAULT_STATE	1

#ifdef __cplusplus
}
#endif
#endif
