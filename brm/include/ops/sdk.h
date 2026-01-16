/*	
 *	@(#)%Portal Version: sdk.h:CommonIncludeInt:1:2006-Sep-11 05:26:54 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_SDK_OPS_H_
#define _PCM_SDK_OPS_H_

/*
 * This file contains the opcode definitions for the SDK PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_SDK_OPS:      575..649; 575..585; 586..649; fm_sdk
 */

#include "ops/base.h"

        /* opcodes for SDK FM */
#define PCM_OP_SDK_GET_FLD_SPECS                575
#define PCM_OP_SDK_SET_FLD_SPECS                576
#define PCM_OP_SDK_GET_OBJ_SPECS                577
#define PCM_OP_SDK_SET_OBJ_SPECS                578
#define PCM_OP_SDK_GET_OP_SPECS                 579	/* Obsolete */
#define PCM_OP_SDK_SET_OP_SPECS                 580	/* Obsolete */
#define PCM_OP_SDK_GET_SEARCH                   581	/* Obsolete */
#define PCM_OP_SDK_SET_SEARCH                   582	/* Obsolete */
#define PCM_OP_SDK_DEL_OBJ_SPECS                583
#define PCM_OP_SDK_DEL_OP_SPECS                 584	/* Obsolete */
#define PCM_OP_SDK_DEL_FLD_SPECS                585
	/* reserve 586 - 649 */


#endif /* _PCM_SDK_OPS_H_ */



