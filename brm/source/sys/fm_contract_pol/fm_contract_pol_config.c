/*************************************************************************
* Copyright (c) 2015, 2020, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its subsidiaries or licensors and may be used, reproduced, 
 *      stored or transmitted only in accordance with a valid Oracle
 *      license or sublicense agreement.
 *
 *************************************************************************/ 

#ifndef lint
static char Sccs_id[] = "@(#)$Id: fm_contract_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2016/03/09 23:12:05 meiyang Exp $";
#endif

#include <stdio.h>
#include "pcm.h"
#include "cm_fm.h"
#include "ops/contract.h"


/********************************************************************
 * If you want to customize any of the op-codes commented below, you
 * need to uncomment it.
 *******************************************************************/
struct cm_fm_config fm_contract_pol_custom_config[] = {
	/* opcode as a u_int, function name (as a string) */
/*	{ PCM_OP_CONTRACT_POL_POST_CREATE_CONTRACT,	"op_contract_pol_post_create_contract", CM_FM_OP_OVERRIDABLE },
	{ PCM_OP_CONTRACT_POL_VALID_CONTRACT,		"op_contract_pol_valid_contract", CM_FM_OP_OVERRIDABLE },
	{ PCM_OP_CONTRACT_POL_PREP_CONTRACT,		"op_contract_pol_prep_contract", CM_FM_OP_OVERRIDABLE }, */
	{ 0,	(char *)0 }
};
