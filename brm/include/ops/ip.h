/*	
 *	@(#)%Portal Version: ip.h:CommonIncludeInt:4:2006-Sep-11 05:23:07 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_IP_OPS_H_
#define _PCM_IP_OPS_H_

/*
 * This file contains the opcode definitions for the IP PCM API.
 */

/*
    NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
    ====================================================================
    PCM_IP_OPS: 9041..9050, 9041..9044, 9045..9050; fm_ip,
    PCM_IP_POL_OPS: 9051..9060, 9051..9056, 9057..9060; fm_ip_pol
 */

#include "ops/base.h"

        /* opcodes for IP  Management*/
#define PCM_OP_IP_DEVICE_CREATE		9041
#define PCM_OP_IP_DEVICE_DELETE		9042
#define PCM_OP_IP_DEVICE_SET_STATE	9043
#define PCM_OP_IP_DEVICE_SET_ATTR	9044

        /* policy opcodes for IP management */
#define PCM_OP_IP_POL_DEVICE_CREATE     9051
#define PCM_OP_IP_POL_DEVICE_ASSOCIATE  9052
#define PCM_OP_IP_POL_DEVICE_DELETE     9053
#define PCM_OP_IP_POL_DEVICE_SET_ATTR   9054
#define PCM_OP_IP_POL_DEVICE_SET_BRAND  9055
#define PCM_OP_IP_POL_DEVICE_SET_STATE  9056

#endif /* _PCM_IP_OPS_H_ */
