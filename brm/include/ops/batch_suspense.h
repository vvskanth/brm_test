/*
 * 	(#)$Id: $
 *
 *      Copyright (c) 2005, 2009, Oracle. All rights reserved.
 *	
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef _PCM_BATCH_SUSPENSE_OPS_H_
#define _PCM_BATCH_SUSPENSE_OPS_H_

/*
 * This file contains the opcode definitions for the Batch Suspense PCM API.
 */

/*
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_BATCH_SUSPENSE_OPS:      9101..9120;		fm_batch_suspense
   PCM_BATCH_SUSPENSE_POL_OPS:  9121..9129;		fm_batch_suspense_pol
 */


#include "ops/base.h"

/*
 * Opcodes for Batch Suspense Management
 */

#define PCM_OP_BATCH_SUSPENSE_RESUBMIT_BATCHES          9101
#define PCM_OP_BATCH_SUSPENSE_DELETE_BATCHES            9102
#define PCM_OP_BATCH_SUSPENSE_WRITE_OFF_BATCHES         9103

/*
 * Policy opcodes
 */

#define PCM_OP_BATCH_SUSPENSE_POL_RESUBMIT_BATCHES_ADJUST  9121

#endif /* _PCM_BATCH_SUSPENSE_OPS_H_ */
