/*	
 *	@(#)%Portal Version: gprs.h:CommonIncludeInt:1:2006-Sep-11 05:26:18 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_GPRS_OPS_H_
#define _PCM_GPRS_OPS_H_

/*
 * This file contains the opcode definitions for the GRPS PCM API.
 */

/* =====================================================================
 *   NAME            : PCM_GPRS_OPS
 *   TOTAL RANGE     : 2000..2099
 *   USED RANGE      : 2000..2014
 *   RESERVED RANGE  : 2015..2099
 *   ASSOCIATED FM   : fm_gprs, fm_gprs_pol
 * =====================================================================
 */

#include "ops/base.h"

	/* opcodes for GPRS */
#define PCM_OP_GPRS_AUTHENTICATE		2000
#define PCM_OP_GPRS_AUTHORIZE			2001
#define PCM_OP_GPRS_POL_AUTHORIZE		2002
#define PCM_OP_GPRS_POL_RESOLVE_ACCT		2003
#define PCM_OP_GPRS_GET_CALL_FEATURES		2004
#define PCM_OP_GPRS_SET_CALL_FEATURES		2005
#define PCM_OP_GPRS_START_ACCOUNTING		2006
#define PCM_OP_GPRS_UPDATE_ACCOUNTING		2007
#define PCM_OP_GPRS_STOP_ACCOUNTING		2008
#define PCM_OP_GPRS_ACCOUNTING_ON		2009
#define PCM_OP_GPRS_ACCOUNTING_OFF		2010
#define PCM_OP_GPRS_UPDATE_LOCATION		2011
#define PCM_OP_GPRS_POL_ACCOUNTING		2012
#define PCM_OP_GPRS_REAUTHORIZE			2013
#define PCM_OP_GPRS_POL_REAUTHORIZE		2014
	/* reserved  through 2099 */

#endif /* _PCM_GPRS_OPS_H_ */
