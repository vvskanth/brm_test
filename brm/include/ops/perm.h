/*	
 *	@(#)%Portal Version: perm.h:CommonIncludeInt:1:2006-Sep-11 05:26:37 %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_PERM_OPS_H_
#define _PCM_PERM_OPS_H_

/*
 * This file contains the opcode definitions for the Perm PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_PERM_OPS:  975..1000; 975..986; 987..1000; fm_perm
 */

#include "ops/base.h"

	/* opcodes for fm_perm */
#define PCM_OP_PERM_ACL_GROUP_CREATE		975
#define PCM_OP_PERM_ACL_GROUP_ADD_MEMBER	976
#define PCM_OP_PERM_ACL_GROUP_DELETE_MEMBER	977
#define PCM_OP_PERM_ACL_GROUP_MODIFY		978
#define PCM_OP_PERM_ACL_GROUP_DELETE		979
#define PCM_OP_PERM_FIND			980
#define PCM_OP_PERM_GET_CREDENTIALS		981
#define PCM_OP_PERM_SET_CREDENTIALS		982
#define PCM_OP_PERM_ACL_GET_SUBGROUPS		983
#define PCM_OP_PERM_SET_EFFECTIVE_CREDENTIAL_BY_OBJID 	984
#define PCM_OP_PERM_SET_EFFECTIVE_CREDENTIAL_BY_LOGIN 	985
#define PCM_OP_PERM_UNSET_EFFECTIVE_CREDENTIAL		986
#define PCM_OP_PERM_CHANGE_OBJECT_OWNER     987
	/* reserved 987 - 1000 */


#endif /* _PCM_PERM_OPS_H_ */
