/*	
 *	@(#)%Portal Version: channel.h:CommonIncludeInt:1:2006-Sep-11 05:26:03 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_CHANNEL_OPS_H_
#define _PCM_CHANNEL_OPS_H_


/*
 * This file contains the opcode definitions for the Channel PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_CHANNEL_OPS:  725..744; 725..726; 727..744; fm_channel
 */

#include "ops/base.h"

	/* opcodes for channel FM */
#define PCM_OP_CHANNEL_PUSH			725
#define PCM_OP_CHANNEL_SYNC			726
	/* reserve  - 744 */

#endif /* _PCM_CHANNEL_OPS_H_ */

