/*
* Copyright (c) 2002, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_voucher_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:15:24 nishahan Exp $";
#endif

#include <stdio.h>

#include <pcm.h>
#include "cm_fm.h"
#include "ops/voucher.h"

#define FILE_LOGNAME "fm_voucher_pol_custom_config.c(1)"


/****************************************************************************
 ****************************************************************************/
struct cm_fm_config fm_voucher_pol_custom_config[] = {
	/* opcode (as an int), function name (as a string) */
/*    
	{ PCM_OP_VOUCHER_POL_ORDER_CREATE,	"op_voucher_pol_order_create" },
	{ PCM_OP_VOUCHER_POL_ORDER_SET_ATTR,	"op_voucher_pol_order_set_attr" },
	{ PCM_OP_VOUCHER_POL_ORDER_SET_BRAND,	"op_voucher_pol_order_set_brand" },
	{ PCM_OP_VOUCHER_POL_ORDER_DELETE,	"op_voucher_pol_order_delete" },
	{ PCM_OP_VOUCHER_POL_ORDER_ASSOCIATE,	"op_voucher_pol_order_associate" },
	{ PCM_OP_VOUCHER_POL_ORDER_PROCESS,	"op_voucher_pol_order_process" },
	{ PCM_OP_VOUCHER_POL_DEVICE_ASSOCIATE,	"op_voucher_pol_device_associate" },
	{ PCM_OP_VOUCHER_POL_DEVICE_CREATE,	"op_voucher_pol_device_create" },
	{ PCM_OP_VOUCHER_POL_DEVICE_SET_ATTR,	"op_voucher_pol_device_set_attr" },
	{ PCM_OP_VOUCHER_POL_DEVICE_SET_BRAND,	"op_voucher_pol_device_set_brand" },
*/    
	{ 0,	(char *)0 }
};

PIN_EXPORT
void *
fm_voucher_pol_custom_config_func() {
	return ((void *) (fm_voucher_pol_custom_config));
}
