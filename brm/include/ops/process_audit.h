/*	
 *	@(#)%Portal Version: process_audit.h:CommonIncludeInt:7:2006-Sep-11 05:23:33 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_PROCESS_AUDIT_OPS_H_
#define _PCM_PROCESS_AUDIT_OPS_H_

/*
 * This file contains the opcode definitions for the process_audit PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_PROCESS_AUDIT_OPS: 4300..4309; 4300..4308; 4309..4309; fm_process_audit
 */

#include "ops/base.h"

#define PCM_OP_PROCESS_AUDIT_CREATE 4300
#define PCM_OP_PROCESS_AUDIT_POL_CREATE 4301
#define PCM_OP_PROCESS_AUDIT_CREATE_AND_LINK 4302
#define PCM_OP_PROCESS_AUDIT_POL_CREATE_AND_LINK 4303
#define PCM_OP_PROCESS_AUDIT_LINK 4304
#define PCM_OP_PROCESS_AUDIT_POL_ALERT 4305
#define PCM_OP_PROCESS_AUDIT_CREATE_WRITEOFF_SUMMARY 4306
#define PCM_OP_PROCESS_AUDIT_POL_CREATE_WRITEOFF_SUMMARY 4307
#define PCM_OP_PROCESS_AUDIT_SEARCH 4308
        /* Reserved - 4309 */

#endif /* _PCM_PROCESS_AUDIT_OPS_H_ */

