/*	
 *	@(#)%Portal Version: sat.h:CommonIncludeInt:1:2006-Sep-11 05:26:52 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_SAT_OPS_H_
#define _PCM_SAT_OPS_H_

/*
 * This file contains the opcode definitions for the Satellite PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_SAT_OPS: 1250..1260; 1250..1252; 1253..1260; fm_sat
 */

#include "ops/base.h"

	/* opcodes for satellite policy */
#define PCM_OP_SAT_POL_PRE_STORE		1250
#define PCM_OP_SAT_POL_POST_STORE		1251
#define PCM_OP_SAT_POL_MASTER_DOWN		1252
	/* reserved - 1260 */

#endif /* _PCM_SAT_OPS_H_ */

