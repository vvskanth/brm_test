/*	
 *	@(#)%Portal Version: delivery.h:CommonIncludeInt:1:2006-Sep-11 05:26:08 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_DELIVERY_OPS_H_
#define _PCM_DELIVERY_OPS_H_


/*
 * This file contains the opcode definitions for the Delivery PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_DELIVERY_OPS:     831..860; 831..831; 832..860; fm_delivery
 */

#include "ops/base.h"

        /* opcodes for delivery FM */
#define PCM_OP_DELIVERY_MAIL_SENDMSGS		831
	/* reserved 832 - 860 */


#endif /* _PCM_DELIVERY_OPS_H_ */
