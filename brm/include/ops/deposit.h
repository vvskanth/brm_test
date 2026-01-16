/* Copyright (c) 2022, Oracle and/or its affiliates. All rights reserved.*/
/* 
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_DEPOSIT_OPS_H_
#define _PCM_DEPOSIT_OPS_H_

/*
 * This file contains the opcode definitions for DEPOSIT
 */

/* =====================================================================
 *   NAME            : PCM_DEPOSIT_OPS
 *   TOTAL RANGE     : 4821..4840
 *   USED RANGE      : 4821..4828
 *   RESERVED RANGE  : 
 *   ASSOCIATED FMs  : fm_deposit
 *
 *   NAME            : PCM_DEPOSIT_POL_OPS
 *   TOTAL RANGE     : 4841..4860
 *   USED RANGE      : 4841..4842
 *   RESERVED RANGE  :
 *   ASSOCIATED FMs  : fm_deposit_pol
 * =====================================================================
 */

#include "ops/base.h"

/* Opcodes for deposit  */
#define PCM_OP_DEPOSIT_PURCHASE_DEPOSIT 4821
#define PCM_OP_DEPOSIT_COLLECT_PAYMENT 4823

/* Opcodes for deposit  - Enhancement */
#define PCM_OP_DEPOSIT_CREATE_SPECIFICATION_PROFILE 		4844
#define PCM_OP_DEPOSIT_GET_SPECIFICATION_PROFILE    		4845
#define PCM_OP_DEPOSIT_MODIFY_SPECIFICATION_PROFILE 		4846
#define PCM_OP_DEPOSIT_CREATE_SPECIFICATION			4847
#define PCM_OP_DEPOSIT_GET_SPECIFICATION 	    		4848
#define PCM_OP_DEPOSIT_MODIFY_SPECIFICATION			4849
#define PCM_OP_DEPOSIT_GET_DEPOSITS				4850
#define PCM_OP_DEPOSIT_UPDATE_DEPOSIT				4851
#define PCM_OP_DEPOSIT_REVERSE_DEPOSIT				4852
#define PCM_OP_DEPOSIT_RELEASE_DEPOSIT				4853
#define PCM_OP_DEPOSIT_TRANSFER_DEPOSIT				4854
#define PCM_OP_DEPOSIT_ADD_INTEREST				4855
#define PCM_OP_DEPOSIT_REFUND_REQUEST				4856
#define PCM_OP_DEPOSIT_GET_REFUND_REQUEST			4857
#define PCM_OP_DEPOSIT_UPDATE_REFUND_REQUEST			4858
#define PCM_OP_DEPOSIT_UPDATE_DEPOSIT_RECEIVED			4859
#define PCM_OP_DEPOSIT_TRIGGER_DEPOSIT				4860
#define PCM_OP_DEPOSIT_DELETE_DRAFT				4869
#define PCM_OP_DEPOSIT_GET_TRANSACTIONS				4873
#define PCM_OP_DEPOSIT_HIERARCHY_DEPOSIT_TRANSFER               4874
/*Policy opcodes for deposit - Enhancement */
#define PCM_OP_DEPOSIT_POL_VALID_SPECIFICATION_PROFILE		4861
#define PCM_OP_DEPOSIT_POL_VALID_SPECIFICATION			4862
#define PCM_OP_DEPOSIT_POL_VALID_RELEASE_DEPOSIT		4863
#define PCM_OP_DEPOSIT_POL_VALID_TRANSFER_DEPOSIT		4864
#define PCM_OP_DEPOSIT_POL_VALID_PURCHASE_DEPOSIT		4865
#define PCM_OP_DEPOSIT_POL_VALID_REFUND_REQUEST			4866
#define PCM_OP_DEPOSIT_POL_PRE_ADD_INTEREST 			4867
#define PCM_OP_DEPOSIT_POL_VALID_REVERSE_DEPOSIT		4868
#define PCM_OP_DEPOSIT_POL_VALID_COLLECT_PAYMENT                4870
#define PCM_OP_DEPOSIT_POL_POST_ADD_INTEREST                    4871
#define PCM_OP_DEPOSIT_POL_VALID_UPDATE_DEPOSIT                 4872
#define PCM_OP_DEPOSIT_POL_PRE_COLLECT_PAYMENT                  4875
#define PCM_OP_DEPOSIT_POL_POST_COLLECT_PAYMENT                 4876

#endif /* _PCM_DEPOSIT_OPS_H_ */
