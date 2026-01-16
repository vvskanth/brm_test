/*	
 *	@(#)%Portal Version: term.h:CommonIncludeInt:1:2006-Sep-11 05:27:02 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_TERM_OPS_H_
#define _PCM_TERM_OPS_H_


/*
 * This file contains the opcode definitions for the Term Server PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_TERM_OPS:     350..374; 350..359; 360..374; fm_term
   PCM_TERM_POL_OPS: 375..399; 375..379; 380..399; fm_term_pol
 */

#include "ops/base.h"


	/* opcodes for term FM */
#define PCM_OP_TERM_IP_DIALUP_LOGIN		350	/* obsolete */
#define PCM_OP_TERM_IP_DIALUP_LOGOUT		351	/* obsolete */
#define PCM_OP_TERM_IP_DIALUP_AUTHENTICATE	352
#define PCM_OP_TERM_IP_DIALUP_START_SESSION	353	/* obsolete */
#define PCM_OP_TERM_IP_DIALUP_AUTHORIZE		354
#define PCM_OP_TERM_IP_DIALUP_START_ACCOUNTING	355
#define PCM_OP_TERM_IP_DIALUP_UPDATE_ACCOUNTING	356
#define PCM_OP_TERM_IP_DIALUP_STOP_ACCOUNTING	357
#define PCM_OP_TERM_IP_DIALUP_ACCOUNTING_ON	358
#define PCM_OP_TERM_IP_DIALUP_ACCOUNTING_OFF	359
	/* reserve 360 - 374 */

	/* opcodes for term policy FM */
#define PCM_OP_TERM_POL_IP_DIALUP_SPEC_RATES	375	/* obsolete */
#define PCM_OP_TERM_POL_GET_DYN_IPADDR		376	/* obsolete */
#define PCM_OP_TERM_POL_REVERSE_IP		377
#define PCM_OP_TERM_POL_AUTHORIZE		378
#define PCM_OP_TERM_POL_ACCOUNTING		379
	/* reserve 380 - 399 */

#endif /* PCM_TERM_OPS_H_ */

