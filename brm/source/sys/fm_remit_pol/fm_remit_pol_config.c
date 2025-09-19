/*
* Copyright (c) 2000, 2023, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: fm_remit_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2023/07/17 05:47:58 visheora Exp $";
#endif

#include <stdio.h>	/* for FILE * in pcm.h */
#include "ops/remit.h"
#include "pcm.h"
#include "cm_fm.h"

#define FILE_LOGNAME "fm_remit_pol_custom_config.c(1)"

/*******************************************************************
 *******************************************************************/
struct cm_fm_config fm_remit_pol_custom_config[] = {
	/* opcode as a int, function name (as a string) */
/*    
	{ PCM_OP_REMIT_POL_SPEC_QTY, "op_remit_pol_spec_qty" },
*/    
	{ 0,	(char *)0 }
};


