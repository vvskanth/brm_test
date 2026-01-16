/*	
 *	@(#)$Id: cust.h /cgbubrm_commoninclude/11 2018/01/26 15:05:07 mlling Exp $ 
 *	
* Copyright (c) 1996, 2018, Oracle and/or its affiliates. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_CUST_OPS_H_
#define _PCM_CUST_OPS_H_

/*
 * This file contains the opcode definitions for the Customer Mngmt PCM API.
 */

/* 
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_CUST_OPS:         50..89;     50..89;       -..-; fm_cust
                     3901..3950; 3901..3923; 3924..3950; 
   PCM_CUST_POL_OPS:   250..299;   250..299;       -..-; fm_cust_pol
                     2150..2199; 2150..2164; 2165..2199; 
 */

#include "ops/base.h"

/*
 * Opcodes for Customer Mngmt FM (50 - 89)
 */
#define PCM_OP_CUST_CREATE_ACCT			50
#define PCM_OP_CUST_CREATE_CUSTOMER		51
#define PCM_OP_CUST_CREATE_SERVICE		52
#define PCM_OP_CUST_DELETE_ACCT			53
#define PCM_OP_CUST_DELETE_CUSTOMER		54	/* Obsolete */
#define PCM_OP_CUST_INIT_ACCT			55	/* Obsolete */
#define PCM_OP_CUST_INIT_SERVICE		56
#define PCM_OP_CUST_PREP_CUSTOMER		57
#define PCM_OP_CUST_SET_BILLINFO		58
#define PCM_OP_CUST_SET_LOGIN			59
#define PCM_OP_CUST_SET_NAMEINFO		60
#define PCM_OP_CUST_SET_PASSWD			61
#define PCM_OP_CUST_SET_STATUS			62
#define PCM_OP_CUST_COMMIT_CUSTOMER		63
#define PCM_OP_CUST_MODIFY_CUSTOMER		64
#define PCM_OP_CUST_SET_ACTGINFO		65	/* Obsolete */
#define PCM_OP_CUST_SET_LOCALE			66
#define PCM_OP_CUST_FIND			67
#define PCM_OP_CUST_CREATE_PROFILE		68
#define PCM_OP_CUST_FIND_PROFILE		69
#define PCM_OP_CUST_MODIFY_PROFILE		70
#define PCM_OP_CUST_DELETE_PROFILE		71
#define PCM_OP_CUST_SET_PRODUCT_STATUS		72	/*PCM_OBSOLETE*/
#define PCM_OP_SUBSCRIPTION_SET_PRODUCT_STATUS	72
#define PCM_OP_CUST_CREATE_PAYINFO              73
#define PCM_OP_CUST_MODIFY_PAYINFO		74
#define PCM_OP_CUST_FIND_PAYINFO		75
#define PCM_OP_CUST_DELETE_PAYINFO		76
#define PCM_OP_CUST_SET_PAYINFO			77
#define PCM_OP_CUST_VALID_FLD			78
#define PCM_OP_CUST_MODIFY_SERVICE		79
#define PCM_OP_CUST_SET_TAXINFO			80
#define PCM_OP_CUST_READ_ACCT_PRODUCTS		81	/*PCM_OBSOLETE*/
#define PCM_OP_SUBSCRIPTION_READ_ACCT_PRODUCTS	81
#define PCM_OP_CUST_GET_HISTORY     		82	/*PCM_OBSOLETE*/
#define PCM_OP_SUBSCRIPTION_GET_HISTORY     	82
#define PCM_OP_CUST_VALIDATE_CUSTOMER     	83
#define PCM_OP_CUST_UPDATE_CUSTOMER     	84
#define PCM_OP_CUST_SET_BRANDINFO		85
#define PCM_OP_CUST_UPDATE_SERVICES             86
#define PCM_OP_CUST_SET_BAL_GRP             	87
#define PCM_OP_CUST_CREATE_BAL_GRP		88
#define PCM_OP_CUST_MODIFY_BAL_GRP		89

#define PCM_OP_CUST_SET_ACCTINFO		3901
#define PCM_OP_CUST_CREATE_BILLINFO		3902
#define PCM_OP_CUST_DELETE_BILLINFO		3903
#define PCM_OP_CUST_DELETE_BAL_GRP		3904
#define PCM_OP_CUST_SET_TOPUPRULES              3905	/*PCM_OBSOLETE*/
#define PCM_OP_CUST_CREATE_TOPUPRULES           3906	/*PCM_OBSOLETE*/
#define PCM_OP_CUST_DELETE_TOPUPRULES           3907	/*PCM_OBSOLETE*/
#define PCM_OP_CUST_MODIFY_TOPUPRULES           3908	/*PCM_OBSOLETE*/

/* OPCODEs for prepay feeder */
/* We are using the same opcode number PCM_OP_CUST_SET_TOPUP
 * PCM_OP_CUST_CREATE_TOPUP, PCM_OP_CUST_DELETE_TOPUP,
 * PCM_OP_CUST_MODIFY_TOPUP for backward compatibility.
 */
#define PCM_OP_CUST_SET_TOPUP                   3905
#define PCM_OP_CUST_CREATE_TOPUP                3906
#define PCM_OP_CUST_DELETE_TOPUP                3907
#define PCM_OP_CUST_MODIFY_TOPUP                3908

/* Opcodes for handling business profiles */
#define PCM_OP_CUST_GET_BUSINESS_PROFILE_INFO	3909
#define PCM_OP_CUST_CHANGE_BUSINESS_PROFILE	3910

/* Opcodes for Customer diagnostic utlity - AccountDumpUtility(ADU) */
#define PCM_OP_ADU_VALIDATE			3911

/* Opcodes for handling business profiles for invoicing */
#define PCM_OP_CUST_CREATE_ASSOCIATED_BUS_PROFILE 	3912
#define PCM_OP_CUST_SET_ASSOCIATED_BUS_PROFILE		3913
/* Opcode used by subscriber life cycle to get the config lifecycle */
#define PCM_OP_CUST_GET_LIFECYCLE_STATES	3914
/* Opcodes for subscriber preferences for Active Mediation */
#define PCM_OP_CUST_GET_SUBSCRIBER_PREFERENCES  3915
#define PCM_OP_CUST_SET_SUBSCRIBER_PREFERENCES  3916
/* Opcodes for creating and searching for notes */
#define PCM_OP_CUST_GET_NOTE  3917
#define PCM_OP_CUST_SET_NOTE  3918

/* Opcodes for handling SEPA mandates */
#define PCM_OP_CUST_REGISTER_MANDATE  3919
#define PCM_OP_CUST_AMEND_MANDATE  3920
#define PCM_OP_CUST_AMEND_CREDITOR_INFO  3921
#define PCM_OP_CUST_CANCEL_MANDATE  3922

/*Opcode to retrieve data for newsfeed.*/
#define PCM_OP_CUST_GET_NEWSFEED 3923

/*Personal Data*/
#define PCM_OP_CUST_DELETE_PERSONAL_DATA 3924

/*Opcode used for bill presentation*/
#define PCM_OP_CUST_SET_BILL_STRUCTURE 3925
/*Opcode used to convert to wholesale hierarchy*/
#define PCM_OP_CUST_CONVERT_WHOLESALE_HIERARCHY 3926

/* Opcodes for customer role creation/update/delete/associate */
#define PCM_OP_CUST_CREATE_ROLE                 3927
#define PCM_OP_CUST_ASSOCIATE_ROLE              3928
#define PCM_OP_CUST_UPDATE_ROLE                 3929
#define PCM_OP_CUST_DELETE_ROLE                 3930

	/* reserved through 3950 */


        /*  Opcodes for Pol_cust FM */
#define PCM_OP_CUST_POL_PRE_CREATE		250	/* Obsolete */
#define PCM_OP_CUST_POL_PRE_COMMIT		251
#define PCM_OP_CUST_POL_POST_COMMIT		252
#define PCM_OP_CUST_POL_GET_CONFIG		253
#define PCM_OP_CUST_POL_GET_INTRO_MSG		254
#define PCM_OP_CUST_POL_GET_PLANS		255
#define PCM_OP_CUST_POL_GET_POPLIST		256
#define PCM_OP_CUST_POL_VALID_ALL		257	/* Obsolete */
#define PCM_OP_CUST_POL_VALID_PLAN		258	/* Obsolete */
#define PCM_OP_CUST_POL_VALID_BILLINFO		259
#define PCM_OP_CUST_POL_VALID_NAMEINFO		260
#define PCM_OP_CUST_POL_VALID_AACINFO		261
#define PCM_OP_CUST_POL_VALID_INHERITED		262	/* Obsolete */
#define PCM_OP_CUST_POL_VALID_MISC		263	/* Obsolete */
#define PCM_OP_CUST_POL_VALID_PASSWD		264
#define PCM_OP_CUST_POL_VALID_LOGIN		265
#define PCM_OP_CUST_POL_PREP_ALL		266	/* Obsolete */
#define PCM_OP_CUST_POL_PREP_PLAN		267	/* Obsolete */
#define PCM_OP_CUST_POL_PREP_BILLINFO		268
#define PCM_OP_CUST_POL_PREP_NAMEINFO		269
#define PCM_OP_CUST_POL_PREP_AACINFO		270
#define PCM_OP_CUST_POL_PREP_INHERITED		271
#define PCM_OP_CUST_POL_PREP_MISC		272	/* Obsolete */
#define PCM_OP_CUST_POL_PREP_PASSWD		273
#define PCM_OP_CUST_POL_PREP_LOGIN		274
#define PCM_OP_CUST_POL_COMPARE_PASSWD		275
#define PCM_OP_CUST_POL_ENCRYPT_PASSWD		276
#define PCM_OP_CUST_POL_DECRYPT_PASSWD		277
#define PCM_OP_CUST_POL_GET_DEALS		278
#define PCM_OP_CUST_POL_GET_PRODUCTS		279
#define PCM_OP_CUST_POL_PREP_LIMIT		280
#define PCM_OP_CUST_POL_VALID_LIMIT		281
#define PCM_OP_CUST_POL_PREP_ACTGINFO		282	/* Obsolete */
#define PCM_OP_CUST_POL_VALID_ACTGINFO		283	/* Obsolete */
#define PCM_OP_CUST_POL_PREP_LOCALE		284
#define PCM_OP_CUST_POL_VALID_LOCALE		285
#define PCM_OP_CUST_POL_PREP_STATUS		286
#define PCM_OP_CUST_POL_VALID_STATUS		287
#define PCM_OP_CUST_POL_PREP_PROFILE		288
#define PCM_OP_CUST_POL_VALID_PROFILE		289
#define PCM_OP_CUST_POL_PREP_PAYINFO		290
#define PCM_OP_CUST_POL_VALID_PAYINFO		291
#define PCM_OP_CUST_POL_MODIFY_SERVICE		292
#define PCM_OP_CUST_POL_CANONICALIZE		293
#define PCM_OP_CUST_POL_VALID_TAXINFO		294
#define PCM_OP_CUST_POL_GET_DB_NO		295
#define PCM_OP_CUST_POL_READ_PLAN		296
#define PCM_OP_CUST_POL_TAX_INIT 		297
#define PCM_OP_CUST_POL_TAX_CALC 		298
#define PCM_OP_CUST_POL_GET_DB_LIST		299

	/* additional opcodes for customer policy FM */
#define PCM_OP_CUST_POL_SET_BRANDINFO		2150
#define PCM_OP_CUST_POL_POST_MODIFY_CUSTOMER	2151
#define PCM_OP_CUST_POL_PREP_BAL_GRP        	2152
#define PCM_OP_CUST_POL_VALID_BAL_GRP        	2153
#define PCM_OP_CUST_POL_PREP_ACCTINFO		2154
#define PCM_OP_CUST_POL_VALID_ACCTINFO		2155
#define PCM_OP_CUST_POL_GET_SUBSCRIBED_PLANS	2156	
#define PCM_OP_CUST_POL_TRANSITION_DEALS 	2157	
#define PCM_OP_CUST_POL_TRANSITION_PLANS	2158
#define PCM_OP_CUST_POL_EXPIRATION_PASSWD	2159

/* customer policy opcodes added for prepayfeeder */
#define PCM_OP_CUST_POL_PREP_TOPUP		2160
#define PCM_OP_CUST_POL_VALID_TOPUP		2161
#define PCM_OP_CUST_POL_DELETE_ACCT		2162

/* Opcodes for Customer diagnostic utlity - AccountDumpUtility(ADU) */
#define PCM_OP_ADU_POL_DUMP			2163
#define PCM_OP_ADU_POL_VALIDATE			2164

/* customer policy opcode added for delete payinfo */
#define PCM_OP_CUST_POL_PRE_DELETE_PAYINFO      2165

	/* reserved through 2199 */

#endif /* _PCM_CUST_OPS_H_ */

