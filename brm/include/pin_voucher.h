/* Continuus file information --- %full_filespec: pin_voucher.h~4:incl:1 % */
/*
 * @(#) %full_filespec: pin_voucher.h~4:incl:1 %
 *
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */
#ifndef _1_pin_voucher_h_H
#define _1_pin_voucher_h_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
 * Object Type Strings
 *******************************************************************/

#define PIN_OBJ_TYPE_DEVICE_VOUCHER		"/device/voucher"
#define PIN_OBJ_TYPE_ORDER_VOUCHER		"/order/voucher"

/*******************************************************************
 * VOUCHER device initial default state 
 *******************************************************************/

#define PIN_VOUCHER_DEFAULT_PROGRAM_NAME	"VOUCHER Manager"

/****************************************************************************
 * VOUCHER Device state types
 ****************************************************************************/

typedef enum pin_voucher_state_type {
	PIN_VOUCHER_RAW_STATE			=	0,
        PIN_VOUCHER_NEW_STATE			=	1,
        PIN_VOUCHER_USED_STATE			=	2,
        PIN_VOUCHER_EXPIRED_STATE		=	3
} pin_voucher_state_type_t;

/****************************************************************************
 * Order object status types
 ****************************************************************************/

typedef enum pin_voucher_order_status {
	PIN_VOUCHER_ORDER_NEW			= 1,
	PIN_VOUCHER_ORDER_REQUEST		= 2,
	PIN_VOUCHER_ORDER_RECEIVED		= 3,
	PIN_VOUCHER_ORDER_PARTIAL_RECEIVE 	= 4,
	PIN_VOUCHER_ORDER_CANCELLED		= 5
}pin_voucher_order_status_t;

#ifdef __cplusplus
}
#endif

#endif
