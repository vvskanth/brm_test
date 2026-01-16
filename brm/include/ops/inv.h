/*	
 *	@(#)%Portal Version: inv.h:CommonIncludeInt:3:2006-Sep-11 05:26:25 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_INV_OPS_H_
#define _PCM_INV_OPS_H_

/*
 * This file contains the opcode definitions for the Inv PCM API.
 */

/*
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_INV_OPS: 951..974; 951..963; 964..974; fm_inv,fm_inv_pol
 */

#include "ops/base.h"


#define PCM_OP_INV_VIEW_INVOICE     		951
#define PCM_OP_INV_POL_FORMAT_VIEW_INVOICE	952
#define PCM_OP_INV_MAKE_INVOICE     		953
#define PCM_OP_INV_POL_PREP_INVOICE 		954
#define PCM_OP_INV_POL_FORMAT_INVOICE		955
#define PCM_OP_INV_POL_FORMAT_INVOICE_HTML	956
#define PCM_OP_INV_POL_FORMAT_INVOICE_DOC1	957
#define PCM_OP_INV_POL_FORMAT_INVOICE_XML	958
#define PCM_OP_INV_POL_FORMAT_INVOICE_XSLT	959
#define PCM_OP_INV_FORMAT_INVOICE   		960
#define PCM_OP_INV_DECODE_INVOICE_DATA		961
#define PCM_OP_INV_POL_SELECT        		962
#define PCM_OP_INV_POL_POST_MAKE_INVOICE	963

#endif /* _PCM_INV_OPS_H_ */

