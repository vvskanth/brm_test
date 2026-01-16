/* Copyright (c) 2012, 2013, Oracle and/or its affiliates. 
All rights reserved. */
/*  *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_OFFER_PROFILE_OPS_H_
#define _PCM_OFFER_PROFILE_OPS_H_

/*
 * This file contains the opcode definitions for the OFFER PROFILE.
 */

/* =====================================================================
 *   NAME            : PCM_OFFER_PROFILE_OPS
 *   TOTAL RANGE     : 4801..4820
 *   USED RANGE      : 4801..4803
 *   RESERVED RANGE  : 
 *   ASSOCIATED FMs  : fm_offer_profile
 * =====================================================================
 */

#include "ops/base.h"

/* Opcodes for Offer Profile  */
#define PCM_OP_OFFER_PROFILE_SET_OFFER_PROFILE 4801
#define PCM_OP_OFFER_PROFILE_GET_OFFER_PROFILE 4802
#define PCM_OP_OFFER_PROFILE_CHECK_POLICY_THRESHOLD 4803


#endif /* _PCM_OFFER_PROFILE_OPS_H_ */
