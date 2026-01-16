/*	
 *	@(#)%Portal Version: repl.h:CommonIncludeInt:1:2006-Sep-11 05:26:49 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_REPL_OPS_H_
#define _PCM_REPL_OPS_H_

/*
 * This file contains the opcode definitions for the Repl PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_REPL_POL_OPS: 745..774; 745..745; 746..774; fm_repl_pol
 */

#include "ops/base.h"

	/* opcodes for replicable objects */
#define PCM_OP_REPL_POL_PUSH			745
	/* reserve - 774 */


#endif /* PCM_REPL_OPS_H_ */

