/*	
 *	@(#)%Portal Version: passwd.h:CommonIncludeInt:2:2006-Sep-11 05:26:35 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_PASSWD_OPS_H_
#define _PCM_PASSWD_OPS_H_

/*
 * This file contains the opcode definitions for the Password PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_PASSWD_OPS: 175..178; Obsolete; Obsolete; None
 */

/*
 *  150..199 have been reserved and used by FM_ACT
 */

#include "ops/base.h"

	/*
	 * Opcodes for Password FM (175-199) OBSOLETE
	 *
	 * 5.1 - defined, but not used
	 * 5.2 - not defined
	 *
	 * PCM_OP_PASSWD_APPROVE		175
	 * PCM_OP_PASSWD_GENERATE		176
	 * PCM_OP_PASSWD_ENCRYPT		177
	 * PCM_OP_PASSWD_COMPARE		178
	 */

#endif /* _PCM_PASSWD_OPS_H_ */

