/*	
 *	@(#)%Portal Version: remit.h:CommonIncludeInt:1:2006-Sep-11 05:26:47 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_REMIT_OPS_H_
#define _PCM_REMIT_OPS_H_


/*
 * This file contains the opcode definitions for the Remit PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_REMIT_OPS:    701..724, 1271..1280; 
                               1271..1274; 
		     701..724, 1275..1280;
		     fm_remit
 */

#include "ops/base.h"

	/* opcodes for remittance calculation */
	/* skip 700, was CHARGE_DDEBIT */
	   /* opcode PCM_OP_REMIT_REMIT	(701) is obsolete.
	      New opcodes are listed below. */
	/* reserve 701 - 724 */


       /* opcodes for 'new' remittance*/
#define PCM_OP_REMIT_GET_PROVIDER               1271
#define PCM_OP_REMIT_POL_SPEC_QTY               1272
#define PCM_OP_REMIT_REMIT_PROVIDER             1273
#define PCM_OP_REMIT_VALIDATE_SPEC_FLDS		1274
       /* reserved 1275 - 1280 */

#endif /* PCM_REMIT_OPS_H_ */


