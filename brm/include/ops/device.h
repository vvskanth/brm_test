/*	
 *	@(#)%Portal Version: device.h:CommonIncludeInt:2:2006-Sep-11 05:25:24 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_DEVICE_OPS_H_
#define _PCM_DEVICE_OPS_H_

/*
 * This file contains the opcode definitions for the Device PCM API.
 */

/*
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_OP_DEVICE:      2700..2749; 2700..2708; 2709..2749 
   PCM_OP_DEVICE_POL:  2750..2799; 2750..2756; 2757..2799
 */


#include "ops/base.h"

	/*
	 * Opcodes for Device Managment Framework
	 */
#define PCM_OP_DEVICE_CREATE			2700
#define PCM_OP_DEVICE_SET_STATE			2701
#define PCM_OP_DEVICE_SET_ATTR			2702
#define PCM_OP_DEVICE_ASSOCIATE			2703
#define PCM_OP_DEVICE_DELETE			2704
#define	PCM_OP_DEVICE_REPLACE			2705
#define PCM_OP_DEVICE_NOTIFY			2706
#define PCM_OP_DEVICE_SET_BRAND			2707
#define PCM_OP_DEVICE_UPDATE			2708

        /* 
	 * Policy opcodes for Device Management Framework 
	 */
#define PCM_OP_DEVICE_POL_CREATE		2750
#define PCM_OP_DEVICE_POL_SET_STATE		2751
#define PCM_OP_DEVICE_POL_SET_ATTR		2752
#define PCM_OP_DEVICE_POL_ASSOCIATE		2753
#define PCM_OP_DEVICE_POL_DELETE		2754
#define PCM_OP_DEVICE_POL_REPLACE		2755
#define PCM_OP_DEVICE_POL_SET_BRAND		2756

#endif /* _PCM_DEVICE_OPS_H_ */
