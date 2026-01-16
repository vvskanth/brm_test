/*	
 * (#)$Id: num.h /cgbubrm_commoninclude/1 2009/11/05 05:05:13 gsivakum Exp $
 *	
 * Copyright (c) 1996, 2009, Oracle and/or its affiliates. 
 * All rights reserved. 
 *	
 * This material is the confidential property of Oracle Corporation or its
 * licensors and may be used, reproduced, stored or transmitted only in
 * accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_NUM_OPS_H_
#define _PCM_NUM_OPS_H_


/*
 * This file contains the opcode definitions for the NUM PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_NUM_OPS: 2576..2624; 2576..2587; 2588..2624; fm_num, fm_num_pol
 */

#include "ops/base.h"

	/*
	 * Opcodes for Number Managmer
	 */
#define PCM_OP_NUM_CREATE_BLOCK			2576
#define PCM_OP_NUM_MODIFY_BLOCK			2577
#define PCM_OP_NUM_QUARANTINE			2578
#define PCM_OP_NUM_POL_CANONICALIZE		2579
#define PCM_OP_NUM_POL_DEVICE_ASSOCIATE		2580
#define PCM_OP_NUM_POL_DEVICE_CREATE		2581
#define PCM_OP_NUM_POL_DEVICE_SET_ATTR		2582
#define PCM_OP_NUM_SPLIT_BLOCK			2583
#define PCM_OP_NUM_POL_DEVICE_SET_BRAND		2584
/* 
 * This opcode is used when porting the number from another service 
 * provider into the existing network 
 */
#define PCM_OP_NUM_PORT_IN                      2585  
/* 
 * This opcode is used when porting the number from your network to 
 * another service provider
 * The opcode sets the status of the specified telephone number as 
 * quarantine-portedout in the /device/num object
 */
#define PCM_OP_NUM_PORT_OUT                     2586
/*
 * This opcode is introduced for Number block shrinkage 
 * where block of numbers are deleted based on the range.
 */
#define PCM_OP_NUM_POL_DEVICE_DELETE            2587

#endif /* _PCM_NUM_OPS_H_ */
