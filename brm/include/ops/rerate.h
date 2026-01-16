/*	
 *	@(#)%Portal Version: rerate.h:CommonIncludeInt:4:2006-Sep-11 05:23:15 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_RERATE_OPS_H_
#define _PCM_RERATE_OPS_H_

/*
 * This file contains the opcode definitions for the Rating PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_RERATE_OPS:     565..567; 565..570; 568..570; fm_rerate
   PCM_RERATE_POL_OPS: 571..575; ---..---; 571..575; fm_rerate_pol
 */

#include "ops/base.h"

	/* opcodes for rerating FM */
#define	PCM_OP_RERATE_PROCESS_JOB		565
#define	PCM_OP_RERATE_PROCESS_QUEUE		566
#define	PCM_OP_RERATE_CREATE_JOB		567
	/* reserve 568 - 570 */
    /*
    Moved PCM_OP_SUBSCRIPTION_INSERT_RERATE_REQUEST opcode
    to PCM_OP_RERATE_INSERT_RERATE_REQUEST.
    Using same opcode number as before.
    */
#define PCM_OP_RERATE_INSERT_RERATE_REQUEST       3766

	/* opcodes for rating policy FM */
	/* reserve 571 - 575 */

#endif /* PCM_RERATE_OPS_H_ */

