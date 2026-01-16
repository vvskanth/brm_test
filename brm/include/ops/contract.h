/*
 *      @(#)$Id: contract.h /cgbubrm_commoninclude/4 2017/12/21 21:27:17 arcmisra Exp $
 *
* Copyright (c) 2016, 2017, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_CONTRACT_OPS_H_
#define _PCM_CONTRACT_OPS_H_

/*
 * This file contains the opcode definitions for the Job PCM API.
 */

/*
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_CONTRACT_OPS:    9311,9330    -..- ; fm_contract
 */

#include "ops/base.h"

        /*
         * Opcodes for Contract FM (9311-9330)
         */
#define PCM_OP_CONTRACT_POL_PREP_CONTRACT_NO            9311
#define PCM_OP_CONTRACT_POL_POST_CREATE_CONTRACT        9312
#define PCM_OP_CONTRACT_POL_VALID_CONTRACT              9313
#define PCM_OP_CONTRACT_POL_PREP_CONTRACT               9314
#define PCM_OP_CONTRACT_POL_CANCEL_CONTRACT             9315
#define PCM_OP_CONTRACT_CREATE_CONTRACT                 9316
#define PCM_OP_CONTRACT_MODIFY_CONTRACT                 9317
#define PCM_OP_CONTRACT_CANCEL_CONTRACT                 9318
#define PCM_OP_CONTRACT_RENEW_CONTRACT                  9320
#define PCM_OP_CONTRACT_POL_NOTIFY_CONTRACT             9321
#define PCM_OP_CONTRACT_GET_SUBSCRIBER_CONTRACTS        9322
#define PCM_OP_CONTRACT_PROCESS_DELIVERABLE		9323
#define PCM_OP_CONTRACT_PROCESS_MILESTONE             	1327
#define PCM_OP_CONTRACT_GET_MILESTONES                	1328

#endif /* _PCM_CONTRACT_OPS_H_ */
