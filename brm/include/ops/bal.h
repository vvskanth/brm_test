/*	
 *@(#)$Id: bal.h /cgbubrm_commoninclude/3 2018/06/19 21:42:24 thakuppu Exp $ Portal Version: bal.h:CommonIncludeInt:14:2007-Aug-29 11:51:23 %
 *	
* Copyright (c) 1996, 2018, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_BAL_OPS_H_
#define _PCM_BAL_OPS_H_

/*
 * This file contains the opcode definitions for the Multi-Bal PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_BAL_OPS:       3701..3725; 3701..3716; 3717..3725; fm_bal
 */

#include "ops/base.h"

	/* opcodes for multi-bal */
#define PCM_OP_BAL_GET_BALANCES			3701
#define PCM_OP_BAL_GET_BAL_GRP_AND_SVC	        3702
#define PCM_OP_BAL_CHANGE_VALIDITY		3703
#define PCM_OP_BAL_GET_ACCT_BILLINFO     	3704
#define PCM_OP_BAL_GET_ACCT_BAL_GRP_AND_SVC	3705
#define PCM_OP_BAL_APPLY_MULTI_BAL_IMPACTS	3706
#define PCM_OP_BAL_GET_MONITOR_BAL              3707
#define PCM_OP_BAL_GET_ACCT_MONITORS            3708
#define PCM_OP_BAL_APPLY_MONITOR_IMPACTS        3709
#define PCM_OP_BAL_GET_PREPAID_BALANCES         3710
#define PCM_OP_BAL_POL_GET_BAL_GRP_AND_SVC      3711
#define PCM_OP_BAL_LOCK_RESERVATION_LIST        3712
#define PCM_OP_BAL_SET_FIRST_USAGE_VALIDITY     3713
#define PCM_OP_BAL_CHANGE_VALIDITY_FROM_STRING	3714
#define PCM_OP_BAL_GET_CREDIT_PROFILE           3715
#define PCM_OP_BAL_POL_CHECK_LIFECYCLE_STATE    3716
#define PCM_OP_BAL_POL_APPLY_MULTI_BAL_IMPACTS  3717
	/*opcode to support CMT balance migration */
#define PCM_OP_BAL_MERGE_MIGRATED_BALANCES	3719
	/* opcode to support Balance Transfer*/
#define PCM_OP_BAL_TRANSFER_BALANCE      	3720
#define PCM_OP_BAL_POL_SET_SUB_BALANCES 	3721
#define PCM_OP_BAL_NOTIFY_BALANCE_EXPIRY	3722  
#define PCM_OP_BAL_POL_VAL_BAL_TRANSFER		3723  

        /* Reserved 3723 - 3725 */

#endif /* _PCM_BAL_OPS_H_ */

