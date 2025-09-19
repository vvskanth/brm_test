/*******************************************************************
 *
 *  @(#) %full_filespec: fm_repl_pol_config.c~6:csrc:1 %
 *
 *      Copyright (c) 1999 - 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or
 *      its licensors and may be used, reproduced, stored or transmitted only
 *      in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)%full_filespec: fm_repl_pol_config.c~6:csrc:1 %";
#endif

#include <stdio.h>	/* for FILE * in pcm.h */
#include "ops/repl.h"
#include "pcm.h"
#include "cm_fm.h"

PIN_EXPORT void * fm_repl_pol_config_func();

struct cm_fm_config fm_repl_pol_config[] = {
	/* opcode as a int32, function name (as a string) */
	{ PCM_OP_REPL_POL_PUSH, "op_repl_pol_push" },
	{ 0,	(char *)0 }
};

void *
fm_repl_pol_config_func()
{
	return ((void *) (fm_repl_pol_config));
}
