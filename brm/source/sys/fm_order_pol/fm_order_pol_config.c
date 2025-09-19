/*
* Copyright (c) 2002, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_order_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:09:40 nishahan Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/order.h"
#include "cm_fm.h"

#define FILE_LOGNAME "fm_order_pol_custom_config.c"

PIN_EXPORT void * fm_order_pol_custom_config_func();


/*******************************************************************
 *******************************************************************/
struct cm_fm_config fm_order_pol_custom_config[] = {
	/* opcode as a u_int, function name (as a string) */
/*    
	{ PCM_OP_ORDER_POL_ASSOCIATE,		"op_order_pol_associate" },
	{ PCM_OP_ORDER_POL_CREATE,		"op_order_pol_create" },
	{ PCM_OP_ORDER_POL_DELETE,		"op_order_pol_delete" },
	{ PCM_OP_ORDER_POL_PROCESS,		"op_order_pol_process" },
	{ PCM_OP_ORDER_POL_SET_ATTR,		"op_order_pol_set_attr" },
	{ PCM_OP_ORDER_POL_SET_BRAND,		"op_order_pol_set_brand" },
	{ PCM_OP_ORDER_POL_SET_STATE,		"op_order_pol_set_state" },
*/    
	{ 0,	(char *)0 }
};

void *
fm_order_pol_custom_config_func()
{
  return ((void *) (fm_order_pol_custom_config));
}
