/* (#)$Id: $
 *
 * Copyright (c) 2005, 2009, Oracle and/or its affiliates.All rights reserved. 
 *  This material is the confidential property of Oracle Corporation.
 *  or its subsidiaries or licensors and may be used, reproduced, stored
 *  or transmitted only in accordance with a valid Oracle license or
 *  sublicense agreement.
 */

#ifndef _PCM_PDC_OPS_H_
#define _PCM_PDC_OPS_H_

/*
 * This file contains the opcode definitions for the PDC PCM API.
 */

/*
     NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
     ====================================================================
     PCM_PDC_POL_OPS: 9136..9140; 9136..9136; 9137..9140; fm_pdc_pol,
 */
#include "ops/base.h"

        /* opcodes for PDC  Management*/

        /* policy opcodes for PDC management */
#define PCM_OP_PDC_POL_APPLY_PARAMETER  9136

#endif /* _PCM_PDC_OPS_H_ */
