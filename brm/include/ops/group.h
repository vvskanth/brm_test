/*	
 *	@(#)%Portal Version: group.h:CommonIncludeInt:1:2006-Sep-11 05:26:21 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_GROUP_OPS_H_
#define _PCM_GROUP_OPS_H_

/*
 * This file contains the opcode definitions for the Group PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_GROUP_OPS:     450..474; 450..457; 458..474; fm_group
   PCM_GROUP_POL_OPS: 475..499; None;     475..499; (fm_group_pol)
 */

#include "ops/base.h"

	/* opcodes for group FM */
#define PCM_OP_GROUP_CREATE_GROUP		450
#define PCM_OP_GROUP_DELETE_GROUP		451
#define PCM_OP_GROUP_ADD_MEMBER			452
#define PCM_OP_GROUP_DELETE_MEMBER		453
#define PCM_OP_GROUP_UPDATE_INHERITED		454
	/* for temp back compatability */
#define PCM_OP_GROUP_UPDATE_INH			454	/* Obsolete */
#define PCM_OP_GROUP_SET_PARENT			455
#define PCM_OP_GROUP_INIT_GROUP			456	/* Obsolete */
	/* reserve 457 - 474 */

	/* opcodes for group policy FM */
	/* reserve 475 - 499 */


#endif /* PCM_GROUP_OPS_H_ */
