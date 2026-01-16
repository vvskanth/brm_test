/*
(#)%full_filespec: asm.h~3:incl:1 %
 *
 *      Copyright (c) 2004 Portal Software, Inc. All rights reserved.
 *      This material is the confidential property of Portal Software, Inc.
 *      or its subsidiaries or licensors and may be used, reproduced, stored
 *      or transmitted only in accordance with a valid Portal license or
 *      sublicense agreement.
 */

#ifndef _PCM_ASM_OPS_H_
#define _PCM_ASM_OPS_H_


/*
 * This file contains the opcode definitions for the ASM PCM API.
 */

/*
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_ASM_OPS:     4550..4570; 4550..4555; 4556..4570; fm_asm
 */

#include "ops/base.h"

        /*
         * Opcodes for ASM FM (4550 - 4555)
         */
#define PCM_OP_ASM_CREATE_ACTIVE_SESSION        4550
#define PCM_OP_ASM_FIND_ACTIVE_SESSION          4551
#define PCM_OP_ASM_UPDATE_ACTIVE_SESSION        4552
#define PCM_OP_ASM_CANCEL_ACTIVE_SESSION        4553
#define PCM_OP_ASM_CLOSE_ACTIVE_SESSION         4554
#define PCM_OP_ASM_DELETE_ACTIVE_SESSION        4555

#endif /* _PCM_ASM_OPS_H_ */
