/*	
 *	@(#)%Portal Version: rate.h:CommonIncludeInt:18:2007-Apr-08 02:05:13 %
 *	
* Copyright (c) 1996, 2013, Oracle and/or its affiliates. All rights reserved. 
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_RATE_OPS_H_
#define _PCM_RATE_OPS_H_

/*
 * This file contains the opcode definitions for the Rating PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_RATE_OPS:     500..549; 500..518; 519..549; fm_rate
   PCM_RATE_POL_OPS: 550..564; 550..559; 560..564; fm_rate_pol
 */

#include "ops/base.h"

	/* opcodes for rating FM */
#define PCM_OP_RATE_EVENT			500
#define PCM_OP_RATE_TAX_EVENT			501
#define PCM_OP_RATE_TAX_CALC			502
#define PCM_OP_RATE_TAX_DEFER			503	/* obsolete */
#define PCM_OP_RATE_GET_PRODLIST		504
#define PCM_OP_RATE_CACHE_PRODUCT		505	/* obsolete */
#define PCM_OP_RATE_CYCLE_ROLLOVER	    	506
#define PCM_OP_RATE_GET_CANDIDATE_DISCLIST      507
#define PCM_OP_RATE_DISCOUNT_EVENT              508
#define PCM_OP_RATE_GET_ZONEMAP_INFO            509
#define PCM_OP_RATE_GET_RATEPLAN_LIST           510
#define PCM_OP_RATE_GET_PRODUCT                 511
#define PCM_OP_RATE_GET_DISCOUNT                512

#define PCM_OP_RATE_AND_DISCOUNT_EVENT		513
#define PCM_OP_RATE_GET_MONITOR_IMPACTS     	514
#define PCM_OP_RATE_PIPELINE_EVENT	        515	
#define PCM_OP_RATE_GENERATE_MONITOR_IMPACTS    516
#define PCM_OP_RATE_GET_ERAS			517
#define PCM_OP_RATE_GET_SYSTEM_PRODUCTS		518
#define PCM_OP_RATE_GET_SYSTEM_DISCOUNTS	519
#define PCM_OP_RATE_ECE_EVENT		        520
#define PCM_OP_RATE_EVALUATE_SELECTOR           521
	/* reserve 522- 549 */

	/* opcodes for rating policy FM */
#define PCM_OP_RATE_POL_TAX_LOC			550
#define PCM_OP_RATE_POL_GET_TAX_SUPPLIER        551
#define PCM_OP_RATE_POL_MAP_TAX_SUPPLIER        552
#define PCM_OP_RATE_POL_POST_RATING        	553
#define PCM_OP_RATE_POL_PRE_TAX            	554
#define PCM_OP_RATE_POL_POST_TAX   	       	555
#define PCM_OP_RATE_POL_GET_TAXCODE	       	556
#define PCM_OP_RATE_POL_EVENT_ZONEMAP           557
#define PCM_OP_RATE_POL_PRE_RATING		558
#define PCM_OP_RATE_POL_PROCESS_ERAS		559
#define PCM_OP_RATE_POL_POST_EVALUATE_SELECTOR  560
	/* reserve 561 - 564 */



#endif /* PCM_RATE_OPS_H_ */

