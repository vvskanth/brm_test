/*	
 *	@(#)%Portal Version: zonemap.h:CommonIncludeInt:1:2006-Sep-11 05:27:06 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_ZONEMAP_OPS_H_
#define _PCM_ZONEMAP_OPS_H_

/*
 * This file contains the opcode definitions for the Zonemap PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_ZONEMAP_OPS:     1001..1050; 1001..1002; 1003..1050; fm_zonemap
   PCM_ZONEMAP_POL_OPS: 1051..1100; 1051..1053; 1054..1100; fm_zonemap_pol
 */

#include "ops/base.h"

	/* opcodes for zonemap standard FM */
#define PCM_OP_ZONEMAP_COMMIT_ZONEMAP			1001
#define PCM_OP_ZONEMAP_GET_ZONEMAP			1002
	/* reserved - 1003 - 1050 */

	/* opcodes for zonemap policy FM */
#define PCM_OP_ZONEMAP_POL_SET_ZONEMAP			1051
#define PCM_OP_ZONEMAP_POL_GET_ZONEMAP			1052
#define PCM_OP_ZONEMAP_POL_GET_LINEAGE			1053
	/* reserved - 1054 - 1100 */


#endif /* _PCM_ZONEMAP_OPS_H_ */

