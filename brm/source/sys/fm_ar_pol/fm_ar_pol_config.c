/*
 *
* Copyright (c) 2007, 2023, Oracle and/or its affiliates. All rights reserved. 
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: fm_ar_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2023/07/17 05:22:21 visheora Exp $";
#endif

#include <stdio.h>      /* for FILE * in pcm.h */
#include "ops/ar.h"
#include "pcm.h"
#include "cm_fm.h"


/********************************************************************
 * If you want to customize any of the op-codes commented below, you
 * need to uncomment it.
 *
 *******************************************************************/
struct cm_fm_config fm_ar_pol_custom_config[] = {
        /* opcode as a u_int, function name (as a string) */
/*        { PCM_OP_AR_POL_REVERSE_WRITEOFF,               "op_ar_pol_reverse_writeoff" },               */
/*        { PCM_OP_AR_POL_GET_EVENT_RUM_CHARGE_DETAIL,    "op_ar_pol_get_event_rum_charge_detail"},     */
/*        { PCM_OP_AR_POL_PRE_EVENT_ADJUSTMENT,           "op_ar_pol_pre_event_adjustment"},            */
/*        { PCM_OP_AR_POL_GET_BILLS,                      "op_ar_pol_get_bills"},            */
/*	  { PCM_OP_AR_POL_FILL_TAXINFO,                   "op_ar_pol_fill_taxinfo", CM_FM_OP_OVERRIDABLE}, */
        { 0,    (char *)0 }
};

