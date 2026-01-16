/*	
 *	@(#)%Portal Version: gl.h:CommonIncludeInt:3:2006-Sep-11 05:26:16 %
 *	
* Copyright (c) 1996, 2017, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_GL_OPS_H_
#define _PCM_GL_OPS_H_


/*
 * This file contains the opcode definitions for the GL PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_GL_OPS: 1325..1349; 1325..1326; 1327..1349; fm_gl
 */

#include "ops/base.h"

	/* opcodes for fm_gl */
#define PCM_OP_GL_VALIDATE_GL_ACCTS		1325
#define PCM_OP_GL_VALIDATE_SPEC_FLDS		1326
#define PCM_OP_GL_PROCESS_MILESTONE		1327
#define PCM_OP_GL_GET_MILESTONES		1328
        /* Reserve - 1349 */


#endif /* _PCM_GL_OPS_H_ */

