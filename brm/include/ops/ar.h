/**************************************************************************
* Copyright (c) 1996, 2018, Oracle and/or its affiliates. All rights reserved.
*
*      This material is the confidential property of Oracle Corporation or its
*      licensors and may be used, reproduced, stored or transmitted only in
*      accordance with a valid Oracle license or sublicense agreement.
****************************************************************************/

#ifndef _PCM_AR_OPS_H_
#define _PCM_AR_OPS_H_

/*
 * This file contains the opcode definitions for the AR tool PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_AR_OPS:       1310..1320, 4500..4520; 1310..1315, 4500..4508; 1316..1320, 4509..4520; fm_ar
 */

#include "ops/base.h"

	/* opcodes for ar tool */
#define PCM_OP_AR_BILL_SEARCH			1310
#define PCM_OP_AR_BILL_ITEM_SEARCH		1311
#define PCM_OP_AR_HIERARCHY_SEARCH		1312
#define PCM_OP_AR_OTHER_ITEM_SEARCH		1313
#define PCM_OP_AR_HISTORY_SEARCH		1314
#define PCM_OP_AR_PAYMENT_SEARCH		1315
#define PCM_OP_AR_EVENT_SEARCH			1316
#define PCM_OP_AR_GET_ACTION_ITEMS		1317
#define PCM_OP_AR_GET_BILL_ITEMS		1318
#define PCM_OP_AR_GET_ITEM_DETAIL		1319
#define PCM_OP_AR_GET_BILLS			1320
#define PCM_OP_AR_GET_BAL_SUMMARY		1321
#define PCM_OP_AR_GET_DISPUTES			1322
#define PCM_OP_AR_BILL_ADJUSTMENT		1323
#define PCM_OP_AR_EVENT_SETTLEMENT		4500
#define PCM_OP_AR_BILL_SETTLEMENT		4501
#define PCM_OP_AR_BILL_DISPUTE			4502
#define PCM_OP_AR_EVENT_DISPUTE			4503
#define PCM_OP_AR_REVERSE_WRITEOFF              4504
#define PCM_OP_AR_POL_REVERSE_WRITEOFF          4505
#define PCM_OP_AR_GET_ACCT_ACTION_ITEMS		4506
#define PCM_OP_AR_GET_ACCT_BAL_SUMMARY          4507
#define PCM_OP_AR_GET_ACCT_BILLS                4508
#define PCM_OP_AR_GET_DISPUTE_DETAILS           4509
#define PCM_OP_AR_RESOURCE_AGGREGATION          4510
#define PCM_OP_AR_GET_ITEMS			4511
#define PCM_OP_AR_BILLINFO_WRITEOFF		4512
#define PCM_OP_AR_GET_EVENT_RUM_CHARGE_DETAIL   4513
#define PCM_OP_AR_POL_GET_EVENT_RUM_CHARGE_DETAIL   4514
#define PCM_OP_AR_POL_PRE_EVENT_ADJUSTMENT	4515
#define PCM_OP_AR_POL_GET_BILLS                 4516
#define PCM_OP_AR_BILL_CREDIT_TRANSFER          4517
#define PCM_OP_AR_REVERSE_ADJUSTMENT            4518
#define PCM_OP_AR_REVERSE_REFUND                4519

        /* Reserved - 1330 */
        /* Reserved -4500..4520 */

#endif /* _PCM_AR_OPS_H_ */
