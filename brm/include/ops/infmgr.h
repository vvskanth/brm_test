/*	
 *	@(#)%Portal Version: infmgr.h:CommonIncludeInt:2:2006-Sep-11 05:26:23 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_INFMGR_OPS_H_
#define _PCM_INFMGR_OPS_H_

/*
 * This file contains the opcode definitions for the Infranet Mngmt PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_INFMGR_OPS:     800..830; 800..813; 814..830; None
 */

#include "ops/base.h"


        /* opcodes for infranet Management */
#define PCM_OP_INFMGR_START_SERVER              800
#define PCM_OP_INFMGR_STOP_SERVER               801
#define PCM_OP_INFMGR_GET_STATUS                802
#define PCM_OP_INFMGR_ADD_OBJECT             	803
#define PCM_OP_INFMGR_DELETE_OBJECT             804
#define PCM_OP_INFMGR_ADD_LOG                   807
#define PCM_OP_INFMGR_GET_INFO                  808
#define PCM_OP_INFMGR_MODIFY_MONITOR_INTERVAL   809
#define PCM_OP_INFMGR_SCHEDULE_DOWNTIME         810
#define PCM_OP_INFMGR_CANCEL_DOWNTIME           811
#define PCM_OP_INFMGR_SATELLITE_CM_START_FORWARDING		812
#define PCM_OP_INFMGR_SATELLITE_CM_STOP_FORWARDING		813
#define PCM_OP_INFMGR_SET_LOGLEVEL 	814    /* Make cm log level as specified */
#define PCM_OP_INFMGR_GET_LOGLEVEL 	815    /* Get cm log level */
	/* 816 - 830 */


#endif /* _PCM_INFMGR_OPS_H_ */

