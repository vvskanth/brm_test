/*	
 *	@(#)%Portal Version: voucher.h:CommonIncludeInt:2:2006-Sep-11 05:24:57 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_VOUCHER_OPS_H_
#define _PCM_VOUCHER_OPS_H_

/*
 * This file contains the opcode definitions for the VOUCHER PCM API.
 */

/*
	NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
	====================================================================
	PCM_VOUCHER_OPS: 3651..3700; 3651..3661, 3662-3700; fm_voucher,
	 fm_voucher_pol
 */

#include "ops/base.h"

	/* opcodes for VOUCHER  Management*/
#define PCM_OP_VOUCHER_EXPIRATION		3651
#define PCM_OP_VOUCHER_ASSOCIATE_VOUCHER	3662

	/* policy opcodes for VOUCHER management */
#define PCM_OP_VOUCHER_POL_ORDER_CREATE		3652
#define PCM_OP_VOUCHER_POL_ORDER_SET_ATTR	3653
#define PCM_OP_VOUCHER_POL_ORDER_ASSOCIATE	3654
#define PCM_OP_VOUCHER_POL_ORDER_DELETE		3655
#define PCM_OP_VOUCHER_POL_ORDER_SET_BRAND	3656
#define PCM_OP_VOUCHER_POL_ORDER_PROCESS	3657
#define PCM_OP_VOUCHER_POL_DEVICE_CREATE	3658
#define PCM_OP_VOUCHER_POL_DEVICE_SET_ATTR	3659
#define PCM_OP_VOUCHER_POL_DEVICE_ASSOCIATE	3660
#define PCM_OP_VOUCHER_POL_DEVICE_SET_BRAND	3661	

#endif /* _PCM_VOUCHER_OPS_H_ */
