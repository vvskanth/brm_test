/*	
 *	@(#)%Portal Version: bulkacct.h:CommonIncludeInt:1:2006-Sep-11 05:26:01 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_BULKACCT_OPS_H_
#define _PCM_BULKACCT_OPS_H_

/*
 * This file contains the opcode definitions for the Bulk Acct PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_BULKACCT_OPS: 1200..1249; 1200..1201; 1202..1249; None
 */

#include "ops/base.h"

	/* opcodes for Bulkacct FM */
#define PCM_OP_BULKACCT_SET_BATCH 		1200 
#define PCM_OP_BULKACCT_GET_BATCH 		1201 
	/* reserved 1202 - 1249 */

#endif /* _PCM_BULKACCT_OPS_H_ */
