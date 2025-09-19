/*
 *
* Copyright (c) 2004, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted 
 * only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_apn_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 03:59:02 nishahan Exp $";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>
#include "cm_fm.h"
#include "ops/apn.h"

#define FILE_LOGNAME "fm_apn_pol_custom_config.c(2)"


/****************************************************************************
 ****************************************************************************/
struct cm_fm_config fm_apn_pol_custom_config[] = {
	/* opcode (as an int), function name (as a string) */
/*    
	{ PCM_OP_APN_POL_DEVICE_ASSOCIATE,	"op_apn_pol_device_associate" },
	{ PCM_OP_APN_POL_DEVICE_CREATE,		"op_apn_pol_device_create" },
	{ PCM_OP_APN_POL_DEVICE_DELETE,		"op_apn_pol_device_delete" },
	{ PCM_OP_APN_POL_DEVICE_SET_ATTR,	"op_apn_pol_device_set_attr" },
	{ PCM_OP_APN_POL_DEVICE_SET_BRAND,	"op_apn_pol_device_set_brand" },
	{ PCM_OP_APN_POL_DEVICE_SET_STATE,	"op_apn_pol_device_set_state" },
*/    
	{ 0,    (char *)0 }
};

PIN_EXPORT
void *
fm_apn_pol_custom_config_func() {
	return ((void *) (fm_apn_pol_custom_config));
}
