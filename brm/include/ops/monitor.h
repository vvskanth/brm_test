/*	
 *	@(#)%Portal Version: monitor.h:CommonIncludeInt:4:2006-Sep-11 05:22:56 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_MONITOR_OPS_H_
#define _PCM_MONITOR_OPS_H_

/*
 * This file contains the opcode definitions for the Monitor PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_MONITOR_OPS:     7850..7875; 7850..7861; 7862..7875; fm_MONITOR
*/

#include "ops/base.h"

/*
 * Opcodes for MONITOR Management Standard FM (7850 - 7875)
 */
#define PCM_OP_MONITOR_PROCESS_HIERARCHY_MONITORS	7850
#define PCM_OP_MONITOR_PROCESS_BILLING_MONITORS		7851
#define PCM_OP_MONITOR_PROCESS_SERVICE_MONITORS		7852
#define PCM_OP_MONITOR_SETUP_MEMBERS			7853
#define PCM_OP_MONITOR_ACCOUNT_HIERARCHY		7854
#define PCM_OP_MONITOR_BILLING_HIERARCHY		7855
#define PCM_OP_MONITOR_SERVICE_HIERARCHY		7856
#define PCM_OP_MONITOR_HIERARCHY_CLEANUP		7857
#define PCM_OP_MONITOR_UPDATE_MONITORS			7858
#define PCM_OP_MONITOR_VALIDATE_MONITOR_MEMBERS		7859
#define PCM_OP_MONITOR_ADD_RERATE_REQUEST		7860
#define PCM_OP_MONITOR_CREATE_MONITOR			7861


#endif /* _PCM_MONITOR_OPS_H_ */

