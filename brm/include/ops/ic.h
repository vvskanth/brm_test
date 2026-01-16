/*
 * (#)$Id: $ 
 *
 * Copyright (c) 2001, 2009, Oracle and/or its affiliates.All rights reserved. 
 * This material is the confidential property of Oracle Corporation. 
 * or its subsidiaries or licensors and may be used, reproduced, stored
 * or transmitted only in accordance with a valid Oracle license or 
 * sublicense agreement.
 */

#ifndef _PCM_IC_OPS_H_
#define _PCM_IC_OPS_H_

/*
 * This file contains the opcode definitions for the IC PCM API.
 */

/*
	NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
	====================================================================
	PCM_IC_OPS: 9141..9160; 9141..9142, 9143..9160; fm_ic
 */

#include "ops/base.h"

	/* opcodes for Interconnect */
#define PCM_OP_IC_DAILY_LOADER                  9141
#define PCM_OP_IC_LOAD_SMS_REPORT               9142

	/* policy opcodes for Interconnect */

	/* reserved through 9160 */

#endif /* _PCM_IC_OPS_H_ */
