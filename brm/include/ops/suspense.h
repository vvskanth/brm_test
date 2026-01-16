/*	
 *	@(#)%Portal Version: suspense.h:CommonIncludeInt:9:2006-Sep-11 05:23:31 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_SUSPENSE_OPS_H_
#define _PCM_SUSPENSE_OPS_H_

/*
 * This file contains the opcode definitions for the Suspense PCM API.
 */

/*
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_SUSPENSE_OPS:      4450..4475; fm_suspense
   PCM_SUSPENSE_POL_OPS:  4476..4490; fm_suspense
 */


#include "ops/base.h"

	/*
	 * Opcodes for Suspense Handling Manager 
	 */

#define PCM_OP_SUSPENSE_RECYCLE_USAGE		4450	
#define PCM_OP_SUSPENSE_EDIT_USAGE		4451	
#define PCM_OP_SUSPENSE_DELETE_USAGE		4452
#define PCM_OP_SUSPENSE_WRITTEN_OFF_USAGE	4453
#define PCM_OP_SUSPENSE_UNDO_EDIT_USAGE		4454	
#define PCM_OP_SUSPENSE_SEARCH_DELETE		4455	
#define PCM_OP_SUSPENSE_SEARCH_RECYCLE		4456
#define PCM_OP_SUSPENSE_SEARCH_WRITE_OFF	4457
#define PCM_OP_SUSPENSE_SEARCH_EDIT		4458
#define PCM_OP_SUSPENSE_DEFERRED_DELETE		4459

	/*
	 * Policy opcodes
	 */

#define PCM_OP_SUSPENSE_POL_RECYCLE_USAGE_ADJUST	4476

#endif /* _PCM_SUSPENSE_OPS_H_ */
