/* 
 * (#)$Id: $ 
 *
 * Copyright (c) 2006, 2009, Oracle and/or its affiliates.All rights reserved. 
 *
 * This material is the confidential property of Oracle Corporation.
 * or its subsidiaries or licensors and may be used, reproduced, stored
 * or transmitted only in accordance with a valid Oracle license or
 * sublicense agreement.
 */

#ifndef _PCM_GPRS_AAA_OPS_H_
#define _PCM_GPRS_AAA_OPS_H_

/*
 * This file contains the opcode definitions for the GENERIC GPRS AAA PCM API.
 */

/* =====================================================================
 *   NAME            : PCM_GPRS_AAA_OPS
 *   TOTAL RANGE     : 9161..9200
 *   USED RANGE      : 9161..9169
 *   RESERVED RANGE  : 9170..9200
 *   ASSOCIATED FM   : fm_gprs_aaa_pol,fm_tcf_gprs,fm_tcf_gprs_pol
 * =====================================================================
 */

#include "ops/base.h"


/* Opcodes for GPRS Service  */
#define PCM_OP_GPRS_APPLY_PARAMETER				9161
#define PCM_OP_GPRS_POL_APPLY_PARAMETER				9162

/* Opcodes for GPRS AAA POL*/
#define PCM_OP_GPRS_AAA_POL_AUTHORIZE_PREP_INPUT		9163
#define PCM_OP_GPRS_AAA_POL_REAUTHORIZE_PREP_INPUT		9164
#define PCM_OP_GPRS_AAA_POL_UPDATE_ACCOUNTING_PREP_INPUT	9165
#define PCM_OP_GPRS_AAA_POL_STOP_ACCOUNTING_PREP_INPUT		9166
#define PCM_OP_GPRS_AAA_POL_AUTHORIZE  				9167
#define PCM_OP_GPRS_AAA_POL_ACC_ON_OFF_SEARCH			9168
#define PCM_OP_GPRS_AAA_POL_SEARCH_SESSION 			9169


#endif /* _PCM_GPRS_AAA_OPS_H_ */

