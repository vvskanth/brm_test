/*	
 *	@(#)%Portal Version: mail.h:CommonIncludeInt:1:2006-Sep-11 05:26:30 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_MAIL_OPS_H_
#define _PCM_MAIL_OPS_H_


/*
 * This file contains the opcode definitions for the Mail PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_MAIL_OPS:     325..349; 325..326; 327..349; fm_mail
 */

#include "ops/base.h"


	/* opcodes for mail FM */
#define PCM_OP_MAIL_DELIV_VERIFY		325
#define PCM_OP_MAIL_LOGIN_VERIFY		326
	/* reserve 327 - 349 */

#endif /* PCM_MAIL_OPS_H_ */

