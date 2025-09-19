/*
 *
* Copyright (c) 2007, 2012, Oracle and/or its affiliates. All rights reserved. 
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: fm_bal_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 05:25:50 nishahan Exp $";
#endif

#include <stdio.h>      /* for FILE * in pcm.h */
#include "ops/bal.h"
#include "pcm.h"
#include "cm_fm.h"

/********************************************************************
 * If you want to customize any of the op-codes commented below, you
 * need to uncomment it.
 *
 *******************************************************************/
struct cm_fm_config fm_bal_pol_custom_config[] = {
        /* opcode as a u_int, function name (as a string) */
/*      { PCM_OP_BAL_POL_CHECK_LIFECYCLE_STATE, "op_bal_pol_check_lifecycle_state" },*/
/*      { PCM_OP_BAL_POL_APPLY_MULTI_BAL_IMPACTS, "op_bal_pol_apply_multi_bal_impacts" },*/
        { 0,    (char *)0 }
};
