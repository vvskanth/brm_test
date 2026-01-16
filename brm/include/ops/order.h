/*
 * (#)$Id: $
 *
 * Copyright (c) 2002, 2009, Oracle and/or its affiliates.All rights reserved. 
 * This material is the confidential property of Oracle Corporation 
 * or its subsidiaries or licensors and may be used, reproduced, stored
 * or transmitted only in accordance with a valid Oracle license or 
 * sublicense agreement.
 */

#ifndef _PCM_ORDER_OPS_H_
#define _PCM_ORDER_OPS_H_

/*
 * This file contains the opcode definitions for the Order PCM API.
 */

/*
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_OP_ORDER:      3576..3599; 3576..3583; 3584..3599 
   PCM_OP_ORDER_POL:  3600..3625; 3600..3606; 3607..3625
 */


#include "ops/base.h"

	/*
	 * Opcodes for Order Managment Framework
	 */
#define PCM_OP_ORDER_CREATE			3576
#define PCM_OP_ORDER_SET_STATE			3577
#define PCM_OP_ORDER_SET_ATTR			3578
#define PCM_OP_ORDER_ASSOCIATE			3579
#define PCM_OP_ORDER_DELETE			3580
#define	PCM_OP_ORDER_UPDATE			3581
#define PCM_OP_ORDER_SET_BRAND			3582
#define PCM_OP_ORDER_PROCESS			3583

        /* 
	 * Policy opcodes for Order Management Framework 
	 */
#define PCM_OP_ORDER_POL_CREATE			3600
#define PCM_OP_ORDER_POL_SET_STATE		3601
#define PCM_OP_ORDER_POL_SET_ATTR		3602
#define PCM_OP_ORDER_POL_ASSOCIATE		3603
#define PCM_OP_ORDER_POL_DELETE			3604
#define PCM_OP_ORDER_POL_SET_BRAND		3605
#define PCM_OP_ORDER_POL_PROCESS		3606

#endif /* _PCM_ORDER_OPS_H_ */
