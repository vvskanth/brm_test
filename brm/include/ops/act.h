/*	
 *	@(#)%Portal Version: act.h:CommonIncludeInt:14:2008-Aug-26 03:04:20 %
 *	
* Copyright (c) 1996, 2018, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_ACT_OPS_H_
#define _PCM_ACT_OPS_H_


/*
 * This file contains the opcode definitions for the Act  PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_ACT_OPS:     150..199; 151..180; 181..199; fm_act
   PCM_ACT_OPS:     4571..4580; 4571; 4572..4580; fm_act
   PCM_ACT_POL_OPS: 300..324; 300..318; 319..324; fm_act_pol
 */

#include "ops/base.h"

	/*
	 * Opcodes for Activity Tracking FM (151 - 176)
	 */
#define PCM_OP_ACT_ADMIN			150	/* Obsolete */
#define PCM_OP_ACT_ACTIVITY			151
#define PCM_OP_ACT_START_SESSION		152
#define PCM_OP_ACT_UPDATE_SESSION		153
#define PCM_OP_ACT_END_SESSION			154
#define PCM_OP_ACT_LOGIN			155
#define PCM_OP_ACT_LOGOUT			156
#define PCM_OP_ACT_VERIFY			157
#define PCM_OP_ACT_FIND_VERIFY			158
#define PCM_OP_ACT_FIND				159
#define PCM_OP_ACT_EVENT			160	/* obsolete */
#define PCM_OP_ACT_LOAD_SESSION			161
#define PCM_OP_ACT_LOG_VERIFY			162	/* Obsolete */
#define PCM_OP_ACT_USAGE			163
#define PCM_OP_ACT_VERIFY_SNAPSHOT		164
#define PCM_OP_ACT_VERIFY_MASTER		165
#define PCM_OP_ACT_CALC_MAX_USAGE		166
#define PCM_OP_ACT_SCHEDULE_CREATE		167
#define PCM_OP_ACT_SCHEDULE_MODIFY		168
#define PCM_OP_ACT_SCHEDULE_EXECUTE		169
#define PCM_OP_ACT_SCHEDULE_DELETE		170
#define PCM_OP_ACT_GET_CANDIDATE_RUMS		171
#define PCM_OP_ACT_AUTHORIZE                    172
#define PCM_OP_ACT_REAUTHORIZE                  173
#define PCM_OP_ACT_CANCEL_AUTHORIZE             174
#define PCM_OP_ACT_MULTI_AUTHORIZE              175
#define PCM_OP_ACT_CHECK_RESOURCE_THRESHOLD     176
#define PCM_OP_ACT_HANDLE_OOD_EVENT             177
#define PCM_OP_ACT_FIND_FROM_STRING             178
#define PCM_OP_ACT_LOG_USER_ACTIVITY		179
/*Opcode for processing newsfeed*/
#define PCM_OP_ACT_PROCESS_EVENTS 		180
/*Wrapper opcode for PCM_OP_ACT_SCHEDULE_CREATE to facilitate passing flist as 
 *     string from webservice */
#define PCM_OP_ACT_SCHEDULE_CREATE_REQ          181
   /*Wrapper opcode for PCM_OP_ACT_SCHEDULE_EXECUTE to facilitate passing flist as 
 *      string from webservice */
#define PCM_OP_ACT_SCHEDULE_EXECUTE_REQ         182

	/* reserve 181 - 199 */

	/* Opcodes for Activity Tracking FM (4571 - 4580) */
#define PCM_OP_ACT_CALC_REMITTANCE		4571
#define PCM_OP_ACT_REQUEST_CREATE		4572
#define PCM_OP_ACT_REQUEST_RETRIEVE		4573
	/* reserve 4574 - 4580 */

	/* opcodes for pol_act FM */
#define PCM_OP_ACT_POL_SPEC_VERIFY		300
#define PCM_OP_ACT_POL_EVENT_NOTIFY		301
#define PCM_OP_ACT_POL_SPEC_RATES		302
#define PCM_OP_ACT_POL_SPEC_GLID		303
#define PCM_OP_ACT_POL_EVENT_LIMIT		304
#define PCM_OP_ACT_POL_SPEC_IMPACT_CATEGORY	305	/* obsolete */
#define PCM_OP_ACT_POL_VALIDATE_SCHEDULE	306
#define PCM_OP_ACT_POL_SPEC_EVENT_CACHE 	307
#define PCM_OP_ACT_POL_SPEC_CANDIDATE_RUMS	308
#define PCM_OP_ACT_POL_CONFIG_BILLING_CYCLE	309
#define PCM_OP_ACT_POL_PRE_AUTHORIZE		310
#define PCM_OP_ACT_POL_POST_AUTHORIZE		311
#define PCM_OP_ACT_POL_PRE_REAUTHORIZE		312
#define PCM_OP_ACT_POL_POST_REAUTHORIZE		313
#define PCM_OP_ACT_POL_SCALE_MULTI_RUM_QUANTITIES 314
#define PCM_OP_ACT_POL_SET_RESOURCE_STATUS      315
#define PCM_OP_ACT_POL_LOCK_SERVICE		316
#define	PCM_OP_ACT_POL_LOG_USER_ACTIVITY	317

/*Policy opcode realted to newsfeed*/
#define PCM_OP_ACT_POL_PROCESS_EVENTS 		318

/*Policy opcode realted to newsfeed*/
#define PCM_OP_ACT_POL_REQUEST_CREATE		319

	/* reserve 320 - 324 */

#endif /* _PCM_ACT_OPS_H_ */
