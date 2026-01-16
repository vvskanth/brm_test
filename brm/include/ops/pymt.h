/*	
 *	@(#)$Id: pymt.h /cgbubrm_commoninclude/9 2018/02/21 01:52:09 jitkumar Exp $
 *	
* Copyright (c) 1996, 2018, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_PYMT_OPS_H_
#define _PCM_PYMT_OPS_H_

/*
 * This file contains the opcode definitions for the Multi-Payment/Top-ups PCM API.
 */

/* 
   NAME: 	TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_PYMT_OPS: 3726..3750; 3726..3748; 3749..3750; 3860..3868 4581..4600 fm_pymt
 */

#include "ops/base.h"

        /* opcodes for fm_pymt */
#define PCM_OP_PYMT_TOPUP                      		 	3726
#define PCM_OP_PYMT_FIND_TOPUP_EVENTS 				3741
#define PCM_OP_PYMT_GET_ACH_INFO                                3742
#define PCM_OP_PYMT_MBI_DISTRIBUTE				3745
		/* Moved from fm_bill */
#define PCM_OP_PYMT_CHARGE					101
#define PCM_OP_PYMT_DEBIT					105	/*PCM_OBSOLETE*/
#define PCM_OP_PYMT_COLLECT					113     
#define PCM_OP_PYMT_CHARGE_CC					114 
#define PCM_OP_PYMT_VALIDATE					115
#define PCM_OP_PYMT_ITEM_SEARCH					128
#define PCM_OP_PYMT_CHARGE_INVOICE              		134     /* obsolete */
#define PCM_OP_PYMT_SELECT_ITEMS				129 
#define PCM_OP_PYMT_RECOVER					135    
#define PCM_OP_PYMT_RECOVER_CC					136
#define PCM_OP_PYMT_VALIDATE_CC					137
#define PCM_OP_PYMT_CHARGE_DDEBIT				140
#define PCM_OP_PYMT_VALIDATE_DD					927
#define PCM_OP_PYMT_CHARGE_DD					928
#define PCM_OP_PYMT_RECOVER_DD					929

		/* Reserve - 3725 */

        /* opcodes for fm_pymt_pol */
#define PCM_OP_PYMT_POL_VALID_VOUCHER           		3727
#define PCM_OP_PYMT_POL_PURCHASE_DEAL           		3728
#define PCM_OP_PYMT_POL_MBI_DISTRIBUTE				3746

/*Refund*/
#define PCM_OP_PYMT_POL_PRE_REFUND                              3747
#define PCM_OP_PYMT_POL_POST_REFUND                             3748

		/* Moved from fm_bill_pol */
#define PCM_OP_PYMT_POL_COLLECT					400
#define PCM_OP_PYMT_POL_PRE_COLLECT				416
#define PCM_OP_PYMT_POL_VALIDATE				401
#define PCM_OP_PYMT_POL_SPEC_COLLECT				402
#define PCM_OP_PYMT_POL_SPEC_VALIDATE				403
#define PCM_OP_PYMT_POL_UNDER_PAYMENT				410
#define PCM_OP_PYMT_POL_OVER_PAYMENT				411
/*  Topup  */
#define PCM_OP_PYMT_POL_TOPUP_SET_VALIDITY                      440
#define PCM_OP_PYMT_POL_POST_TOPUP                              441
#define PCM_OP_PYMT_POL_CHARGE					3733

/*Payment Fees*/
#define PCM_OP_PYMT_APPLY_FEE					3729
#define PCM_OP_PYMT_POL_APPLY_FEE				3730

/*Payment Incentives*/
#define PCM_OP_PYMT_PROVISION_INCENTIVE				3734
#define PCM_OP_PYMT_REVERSE_INCENTIVE				3735
#define PCM_OP_PYMT_GRANT_INCENTIVE				3736
#define PCM_OP_PYMT_POL_PROVISION_INCENTIVE			3737
#define PCM_OP_PYMT_POL_GRANT_INCENTIVE				3738

/*Suspense Management*/
#define PCM_OP_PYMT_VALIDATE_PAYMENT				3731
#define PCM_OP_PYMT_POL_VALIDATE_PAYMENT			3732
#define PCM_OP_PYMT_RECYCLE_PAYMENT				3739
#define PCM_OP_PYMT_POL_SUSPEND_PAYMENT				3740
#define PCM_OP_PYMT_RECYCLED_PAYMENTS_SEARCH                    3743

/* Reterival opcodes */
#define PCM_OP_PYMT_MBI_ITEM_SEARCH				3744

/* SEPA request object  creation and updation opcodes */
#define PCM_OP_PYMT_CREATE_SEPA_REQUEST				3749
#define PCM_OP_PYMT_UPDATE_SEPA_REQUEST				3750

/* SEPA request and response processing opcodes */
#define PCM_OP_PYMT_GET_SEPA_REQUEST_INFO			4581
#define PCM_OP_PYMT_PROCESS_SEPA_RESPONSE			4582

/* Merging of cloud pymt */
#define PCM_OP_PYMT_GENERIC_CHARGE_CC                          3860
#define PCM_OP_PYMT_GENERIC_VALIDATE_CC                        3861
#define PCM_OP_PYMT_GENERIC_DELETE_CC                          3862
#define PCM_OP_PYMT_GENERIC_CAPTURE_CC                         3863
#define PCM_OP_PYMT_GENERIC_REFUND_CC                          3864
#define PCM_OP_PYMT_GENERIC_PROCESS_PAYMENT                    3865
#define PCM_OP_PYMT_GENERIC_NOTIFY_PAYMENT                     3866
#define PCM_OP_PYMT_GENERIC_RECOVER_CC                         3867
#define PCM_OP_PYMT_GENERIC_AUTHORIZE_CC                       3868

/* Policy opcodes for collections component */
#define PCM_OP_PYMT_POL_EXEC_COLLECTIONS_ACTION			4583

/*
enum pymt_req_trans_status {
        PYMT_REQ_TRANS_STATUS_PYMT_PENDING = 0,
        PYMT_REQ_TRANS_STATUS_PAID = 1,
        PYMT_REQ_TRANS_STATUS_FAILED = 2
  } ;
*/

#endif /* _PCM_PYMT_OPS_H_ */
