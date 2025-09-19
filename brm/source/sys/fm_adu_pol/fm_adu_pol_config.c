/******************************************************************************
 *
* Copyright (c) 2007, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 ******************************************************************************/

#include "pcm.h"
#include "cm_fm.h"
#include "ops/cust.h"

PIN_EXPORT void * fm_adu_pol_custom_config_func();

struct cm_fm_config fm_adu_pol_custom_config[] = {
	/* opcode as a u_int, function name (as a string) */

/****************************************************************************
 *
 * If you want to customize any of the op-codes commented below, you need to
 * uncomment it. The op-codes that have the op-code name "XXXXX" need to be
 * written if you want to customize those op-codes.
 *
 ****************************************************************************/
 /* 	{ PCM_OP_ADU_POL_DUMP, "op_adu_pol_dump" },
	{ PCM_OP_ADU_POL_VALIDATE, "op_adu_pol_validate" }, */
	{ 0,	(char *)0 }
};

void *
fm_adu_pol_custom_config_func() {
	return ((void *) (fm_adu_pol_custom_config));
}

