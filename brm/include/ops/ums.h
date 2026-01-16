/*	
 *	@(#)%Portal Version: ums.h:CommonIncludeInt:4:2006-Sep-11 05:24:54 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _UMS_H_
#define _UMS_H_

/*
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_UMS_OPS:   3951..3975; 3951..3955; 3956..3975; fm_ums
 */

#include "ops/base.h"

        /* opcodes for universal message store */
#define PCM_OP_UMS_GET_MESSAGE_TEMPLATES		3951
#define PCM_OP_UMS_GET_MESSAGE_TEMPLATE			3952
#define PCM_OP_UMS_SET_MESSAGE				3953
#define PCM_OP_UMS_GET_MESSAGE				3954
#define PCM_OP_UMS_DELETE_MESSAGE			3955
        /* reserved through 3875 */

#endif /* _UMS_H_ */
