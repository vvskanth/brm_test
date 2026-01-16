/*	
 *	@(#)% %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_FILTER_SET_OPS_H_
#define _PCM_FILTER_SET_OPS_H_


/*
 * This file contains the opcode definitions for the FilterSet  PCM API.
 */

/*
   NAME: TOTAL RANGE 9000...9020; USED RANGE 9000...9002; RESERVED RANGE 9008...9020; ASSOCIATED FM (if any)
   =========================================================================================================
   PCM_FILTER_SET_OPS:     9000...9002; fm_filter_set
 
 */

#include "ops/base.h"

        /*
         * Opcodes for FilterSet FM (9000 - 9003)
         */
#define PCM_OP_FILTER_SET_CREATE                    9000
#define PCM_OP_FILTER_SET_UPDATE                    9001
#define PCM_OP_FILTER_SET_DELETE                    9002


#endif
