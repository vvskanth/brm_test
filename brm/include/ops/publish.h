/*	
 *	@(#)%Portal Version: publish.h:CommonIncludeInt:1:2006-Sep-11 05:26:42 %
 *	
* Copyright (c) 1996, 2016, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_PUBLISH_OPS_H_
#define _PCM_PUBLISH_OPS_H_

/*
 * This file contains the opcode definitions for the Publish PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_PUBLISH_OPS: 1300..1302; 1300..1302; 1303..1305; fm_publish
   PCM_PUBLISH_POL_OPS: 1306..1309; 1306; 1307..1309; fm_publish_pol
 */

#include "ops/base.h"

#define PCM_OP_PUBLISH_EVENT 1300
#define PCM_OP_PUBLISH_GEN_PAYLOAD 1301
#define PCM_OP_PUBLISH_CREATE_PAYLOAD 1302
        /* Reserved - 1303-1305*/
       /*opcode for fm_publish_pol*/
#define PCM_OP_PUBLISH_POL_PREP_EVENT 1306
       /* Reserved - 1307-1309*/

#endif /* _PCM_PUBLISH_OPS_H_ */

