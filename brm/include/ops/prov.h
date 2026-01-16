/*	
 *	@(#)%Portal Version: prov.h:CommonIncludeInt:1:2006-Sep-11 05:25:36 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_PROV_OPS_H_
#define _PCM_PROV_OPS_H_


/*
 * This file contains the opcode definitions for the Prov PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_PROV_OPS:  2450..2462; 2450..2451; 2452..2462; fm_prov
   PCM_PROV_POL_OPS: 2463..2474; 2463..2463; 2464..2474; fm_prov_pol
 */

#include "ops/base.h"

	/* opcodes for fm_prov */
#define PCM_OP_PROV_PUBLISH_SVC_ORDER		2450
#define PCM_OP_PROV_UPDATE_SVC_ORDER		2451

	/* opcodes for fm_prov_pol */
#define PCM_OP_PROV_POL_UPDATE_SVC_ORDER	2463


#endif /* _PCM_PROV_OPS_H_ */

