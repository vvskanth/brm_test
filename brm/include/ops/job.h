/*	
 *	@(#)$Id: job.h /cgbubrm_commoninclude/1 2016/02/01 13:12:00 arcmisra Exp $
 *	
 * Copyright (c) 2023, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_JOB_OPS_H_
#define _PCM_JOB_OPS_H_

/*
 * This file contains the opcode definitions for the Job PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_JOB_OPS:    8000-8100,    -..- ;	fm_job
 */

#include "ops/base.h"

	/*
	 * Opcodes for Job FM (8000-8100)
	 */
#define PCM_OP_JOB_PROCESS_TEMPLATE   		8001 

#define PCM_OP_JOB_POL_VALIDATE_TEMPLATE 	8002

#define PCM_OP_JOB_EXECUTE 			8003

#define PCM_OP_JOB_PROCESS_DEFINITION   	8004

#define PCM_OP_JOB_POL_VALIDATE_DEFINITION 	8005

#define PCM_OP_JOB_POL_POST_PROCESS_DEFINITION 	8006


#endif /* _PCM_JOB_OPS_H_ */
