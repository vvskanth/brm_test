/*	
 *	@(#)%Portal Version: gsm.h:CommonIncludeInt:2:2006-Sep-11 05:25:26 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_GSM_OPS_H_
#define _PCM_GSM_OPS_H_

/*
 * This file contains the opcode definitions for the GSM PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
 *   NAME            : PCM_GSM_OPS
 *   TOTAL RANGE     : 4001..4100
 *   USED RANGE      : 4001..4053,4080..4081
 *   RESERVED RANGE  : 4053..4100
 *   ASSOCIATED FM   : fm_gsm
   =====================================================================
 */

#include "ops/base.h"

	/* opcodes for gsm FM */
#define PCM_OP_GSM_SVC_LISTENER                 2525
#define PCM_OP_GSM_APPLY_PARAMETER		4080
#define PCM_OP_GSM_POL_APPLY_PARAMETER		4081
#define PCM_OP_GSM_PROPAGATE_STATUS		2527
	/*  reserved 2528 - 2574 */

#endif
