/*	
 *@(#)$Id: loan.h /cgbubrm_commoninclude/1 2021/07/21 14:44:00 udg Exp $
 *	
* Copyright (c) 2021,2022, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_LOAN_OPS_H_
#define _PCM_LOAN_OPS_H_

/*
 * This file contains the opcode definitions for the Loan Management PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_LOAN_OPS:       9351..9380;9351..9359;9360..9380; fm_loan
 */

#include "ops/base.h"

	/* opcodes for loan */
#define PCM_OP_LOAN_APPLY_LOAN                  9351
#define PCM_OP_LOAN_GET_LOAN                    9352
#define PCM_OP_LOAN_ELIGIBILITY                 9353
#define PCM_OP_LOAN_POL_RESET_CYCLE             9354
#define PCM_OP_LOAN_POL_ELIGIBILITY		9355
#define PCM_OP_LOAN_NOTIFY_THRESHOLD            9356
#define PCM_OP_LOAN_RECOVER_LOAN                9357
#define PCM_OP_LOAN_POL_PRE_APPLY_LOAN		9358
#define PCM_OP_LOAN_PULLBACK_LOAN               9359
#define PCM_OP_LOAN_POL_PRE_RECOVER_LOAN        9360

        /* Reserved 9361 - 9380 */

#endif /* _PCM_LOAN_OPS_H_ */

