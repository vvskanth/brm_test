/* (#)$Id: $ 
 *
 * Copyright (c) 2005, 2009, Oracle and/or its affiliates.All rights reserved. 
 *  This material is the confidential property of Oracle Corporation.
 *  or its subsidiaries or licensors and may be used, reproduced, stored
 *  or transmitted only in accordance with a valid Oracle license or
 *  sublicense agreement.
 */

#ifndef _PCM_IMT_OPS_H_
#define _PCM_IMT_OPS_H_

/*
 * This file contains the opcode definitions for the IMT PCM API.
 */

/*
     NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
     ====================================================================
     PCM_IMT_POL_OPS: 9131..9135; 9131..9131; 9132..9135; fm_imt_pol,
 */
#include "ops/base.h"

        /* opcodes for IMT  Management*/

        /* policy opcodes for IMT management */
#define PCM_OP_IMT_POL_APPLY_PARAMETER  9131

#endif /* _PCM_IMT_OPS_H_ */
