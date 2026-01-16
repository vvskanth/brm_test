/*	
 *	@(#)%Portal Version: gel.h:CommonIncludeInt:1:2006-Sep-11 05:26:13 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_GEL_OPS_H_
#define _PCM_GEL_OPS_H_

/*
 * This file contains the opcode definitions for the GEL PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_GEL_OPS:      1101..1120; 1101..1105; 1106..1120; fm_gel
 */

#include "ops/base.h"

	/* opcodes for GEL (Generic Event Loader) FM */
#define PCM_OP_GEL_TEMPLATE_CREATE     		1101 
#define PCM_OP_GEL_TEMPLATE_MODIFY     		1102
#define PCM_OP_GEL_TEMPLATE_READ     		1103
#define PCM_OP_GEL_TEMPLATE_DELETE     		1104
#define PCM_OP_GEL_FIND_TEMPLATES     		1105
        /* reserved - 1120 */

#endif /* _PCM_GEL_OPS_H_ */

