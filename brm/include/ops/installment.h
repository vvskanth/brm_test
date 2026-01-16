/* Copyright (c) 2021,2022 Oracle and/or its affiliates. All rights reserved.*/
/* 
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_INSTALLMENT_OPS_H_
#define _PCM_INSTALLMENT_OPS_H_

/*
 * This file contains the opcode definitions for INSTALLMENTS
 */

/* =====================================================================
 *   NAME            : PCM_INSTALLMENT_OPS
 *   TOTAL RANGE     : 5821..5840
 *   USED RANGE      : 5821..5840
 *   RESERVED RANGE  : 
 *   ASSOCIATED FMs  : fm_installment
 *
 *   NAME            : PCM_INSTALLMENT_POL_OPS
 *   RESERVED RANGE  :
 *   ASSOCIATED FMs  : fm_installment_pol
 * =====================================================================
 */

#include "ops/base.h"

/* Opcodes for installment  */
#define PCM_OP_INSTALLMENT_CREATE_SCHEDULE_SPEC 	5821
#define PCM_OP_INSTALLMENT_MODIFY_SCHEDULE_SPEC 	5822
#define PCM_OP_INSTALLMENT_GET_SCHEDULE_SPEC    	5823
#define PCM_OP_INSTALLMENT_CREATE_INSTALLMENT 		5824
#define PCM_OP_INSTALLMENT_UPDATE_INSTALLMENT_STATUS 	5825
#define PCM_OP_INSTALLMENT_GET_INSTALLMENTS 		5826
#define PCM_OP_INSTALLMENT_APPLY_CHARGE 		5827
#define PCM_OP_INSTALLMENT_POL_VALID_INSTALLMENT 	5828
#define PCM_OP_INSTALLMENT_CANCEL_INSTALLMENT 		5829
#define PCM_OP_INSTALLMENT_UPDATE_INSTALLMENT 		5830
#define PCM_OP_INSTALLMENT_POL_VALID_SCHEDULE_SPEC 	5831
#define PCM_OP_INSTALLMENT_POL_PREP_SCHEDULE_SPEC 	5832
#define PCM_OP_INSTALLMENT_POL_PREP_INSTALLMENT 	5833
#define PCM_OP_INSTALLMENT_POL_GET_SCHEDULE_SPEC 	5834
#define PCM_OP_INSTALLMENT_PROPOSAL			5835
#define PCM_OP_INSTALLMENT_POL_PROPOSAL                 5836
#define PCM_OP_INSTALLMENT_GET_INSTALLMENT_DETAILS      5837
#define PCM_OP_INSTALLMENT_POL_GET_INSTALLMENT_DETAILS  5838
#define PCM_OP_INSTALLMENT_GET_BILL_ITEMS               5839
#define PCM_OP_INSTALLMENT_NOTIFY_INSTALLMENT_DUE	5840

#endif /* _PCM_INSTALLMENT_OPS_H_ */
