/*	
 *	@(#)%Portal Version: sim.h:CommonIncludeInt:2:2006-Sep-11 05:25:43 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_SIM_OPS_H_
#define _PCM_SIM_OPS_H_

/*
 * This file contains the opcode definitions for the SIM PCM API.
 */

/*
	NAME: 		TOTAL        USED        RESERVED  
			RANGE        RANGE       RANGE
	====================================================================
	PCM_SIM_OPS: 	 2400..2424; 2400..2403, 2404-2424;  fm_sim
	PCM_SIM_POL_OPS: 2425..2449; 2425..2434, 2435-2449; fm_sim_pol
 */

#include "ops/base.h"

	/* opcodes for SIM  Management*/
#define	PCM_OP_SIM_CREATE_ORDER			2400
#define PCM_OP_SIM_UPDATE_ORDER			2401
#define PCM_OP_SIM_PROCESS_ORDER_RESPONSE	2402
#define PCM_OP_SIM_DEVICE_PROVISION		2403

	/* policy opcodes for SIM management */
#define PCM_OP_SIM_POL_DEVICE_CREATE	2425
#define PCM_OP_SIM_POL_DEVICE_SET_ATTR	2426
#define PCM_OP_SIM_POL_DEVICE_ASSOCIATE	2427
#define PCM_OP_SIM_POL_DEVICE_SET_BRAND	2428

/*Policy opcodes for Order Framework*/

#define PCM_OP_SIM_POL_ORDER_CREATE	2429
#define PCM_OP_SIM_POL_ORDER_DELETE	2430
#define PCM_OP_SIM_POL_ORDER_PROCESS	2431
#define PCM_OP_SIM_POL_ORDER_SET_ATTR	2432
#define PCM_OP_SIM_POL_ORDER_SET_BRAND	2433
#define PCM_OP_SIM_POL_ORDER_SET_STATE	2434

#endif /* _PCM_SIM_OPS_H_ */
