/*	
 *	@(#)%Portal Version: content.h:CommonIncludeInt:1:2006-Sep-11 05:25:22 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_CONTENT_OPS_H_
#define _PCM_CONTENT_OPS_H_

/*
 * This file contains the opcode definitions for the CONTENT PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_CONTENT_OPS: 2200..2299; 2200..2262; 2263..2299; fm_content, fm_content_pol
 */

#include "ops/base.h"

	/* opcodes for content FM */
#define PCM_OP_CONTENT_AUTHENTICATE                 2250
#define PCM_OP_CONTENT_AUTHORIZE                    2251
#define PCM_OP_CONTENT_POL_AUTHORIZE                2252
#define PCM_OP_CONTENT_GET_SRVC_FEATURES            2253
#define PCM_OP_CONTENT_SET_SRVC_FEATURES            2254
#define PCM_OP_CONTENT_ACCOUNTING		    2255
#define PCM_OP_CONTENT_POL_ACCOUNTING	            2256	
#define PCM_OP_CONTENT_POL_RESOLVE_USER             2257	
#define PCM_OP_CONTENT_POL_RESOLVE_EVENT_EXTENSIONS 2258	
#define PCM_OP_CONTENT_FIND		            2259	
#define PCM_OP_CONTENT_CANCEL_AUTHORIZATION         2260
#define PCM_OP_CONTENT_POL_POST_AUTHORIZE           2261
#define PCM_OP_CONTENT_POL_POST_ACCOUNTING          2262

#endif
