/* @(#)%Portal Version: pin_apn.h:BillingVelocityInt:2:2006-Sep-14 18:47:20 % */
/*
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */
#ifndef _PIN_APN_H
#define _PIN_APN_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
 * Object Type Strings
 *******************************************************************/

#define PIN_OBJ_TYPE_DEVICE_APN		"/device/apn"

/*******************************************************************
 * APN device initial default state 
 *******************************************************************/

#define PIN_APN_DEFAULT_PROGRAM_NAME	"Static IP Manager"

/****************************************************************************
 * APN Device state types
 ****************************************************************************/

typedef enum pin_apn_state_type {
	PIN_APN_RAW_STATE		=	0,
	PIN_APN_NEW_STATE		=	1,
	PIN_APN_USABLE_STATE		=	2,
	PIN_APN_UNUSABLE_STATE		=	3
} pin_apn_state_type_t;

#ifdef __cplusplus
}
#endif

#endif
