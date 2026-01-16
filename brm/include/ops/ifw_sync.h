/*	
 *	"@#$Id: ifw_sync.h /cgbubrm_commoninclude/1 2018/06/18 04:36:34 sagaur Exp $";	
 *	
* Copyright (c) 1996, 2018, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_IFW_SYNC_OPS_H_
#define _PCM_IFW_SYNC_OPS_H_

/*
 * This file contains the opcode definitions for the Acct. Sync. PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_IFW_SYNC_OPS:     3626..3640; 3626..3627; 3628..3640; fm_ifw_sync
   PCM_IFW_SYNC_POL_OPS: 3641..3650; 3641; 3642..3650; fm_ifw_sync_pol
 */

#include "ops/base.h"

	/* opcodes for IntegRate Framework Synchronization FM */
#define PCM_OP_IFW_SYNC_PUBLISH_EVENT			3626
#define PCM_OP_ECE_SYNC_MIGRATED_DATA			3627

	/* opcodes for IntegRate Framework Synchronization policy FM */
#define PCM_OP_IFW_SYNC_POL_PUBLISH_EVENT		3641



#endif /* PCM_IFW_SYNC_OPS_H_ */
