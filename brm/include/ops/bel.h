/*	
 *	@(#)%Portal Version: bel.h:CommonIncludeInt:1:2006-Sep-11 05:25:19 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_BEL_OPS_H_
#define _PCM_BEL_OPS_H_


/*
 * This file contains the opcode definitions for the BEL PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_BEL_OPS:  2475..2524; 2475..2489; 2490..2524; fm_bel and fm_bel_pol
 */

#include "ops/base.h"

	/* opcodes for BEL and BEL_POL FMs */
#define PCM_OP_BEL_POL_HANDLE_SVC_ORDER		2475
#define PCM_OP_BEL_HANDLE_EVENTS		2476
#define PCM_OP_BEL_SCHEDULED_PROV		2477

#define PCM_OP_BEL_PURCHASE_PPV                 2478
#define PCM_OP_BEL_CANCEL_PPV                   2479

#define PCM_OP_BEL_DEV_REFRESH                  2480
#define PCM_OP_BEL_DEV_POLL                     2481
#define PCM_OP_BEL_DEV_PIN_ACT                  2482

#define PCM_OP_BEL_POL_DEVICE_SET_ATTR          2483
#define PCM_OP_BEL_POL_DEVICE_CREATE	        2484
#define PCM_OP_BEL_POL_DEVICE_ASSOCIATE         2485
#define PCM_OP_BEL_POL_DEVICE_DELETE            2486
#define PCM_OP_BEL_POL_DEVICE_REPLACE	        2487
#define PCM_OP_BEL_POL_DEVICE_SET_BRAND	        2488
#define PCM_OP_BEL_POL_DEVICE_SET_STATE         2489

#endif /* _PCM_BEL_OPS_H_ */

