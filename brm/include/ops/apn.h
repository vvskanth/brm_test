/*	
 *	@(#)%Portal Version: apn.h:CommonIncludeInt:3:2006-Sep-11 05:23:04 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_APN_OPS_H_
#define _PCM_APN_OPS_H_

/*
 * This file contains the opcode definitions for the APN PCM API.
 */

/*
     NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
     ====================================================================
     PCM_APN_OPS: 9021..9030; ; 9021..9030; fm_apn,
     PCM_APN_POL_OPS: 9031..9040; 9031..9036; 9037..9040; fm_pol_apn,
 */
#include "ops/base.h"

        /* opcodes for APN  Management*/

        /* policy opcodes for APN management */
#define PCM_OP_APN_POL_DEVICE_CREATE    9031
#define PCM_OP_APN_POL_DEVICE_ASSOCIATE 9032
#define PCM_OP_APN_POL_DEVICE_DELETE    9033
#define PCM_OP_APN_POL_DEVICE_SET_ATTR  9034
#define PCM_OP_APN_POL_DEVICE_SET_BRAND 9035
#define PCM_OP_APN_POL_DEVICE_SET_STATE 9036

#endif /* _PCM_APN_OPS_H_ */
