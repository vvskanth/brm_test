/*	
 *	@(#)%Portal Version: lock_program.h:CommonIncludeInt:1:2006-Sep-11 05:25:31 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_LOCK_PROGRAM_OPS_H_
#define _PCM_LOCK_PROGRAM_OPS_H_

/*
 * This file contains the opcode definitions for the LOCK_PROGRAM PCM API.
 */

/* 
   NAME                : TOTAL RANGE   ; USED RANGE  ; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_LOCK_PROGRAM_OPS: 3050...3055   ; 3050...3050 ; 3051...3055   ; fm_lock_program
 */

#include "ops/base.h"

	/* opcodes for LOCK_PROGRAM FM */
#define PCM_OP_LOCK_PROGRAM			3050
	/* reserved 3050- 3055*/


#endif /* _PCM_LOCK_PROGRAM_OPS_H_ */

