/*
 *      @(#)%Portal Version: pcm_ops_do_not_edit.h:CommonIncludeInt:2:2006-Sep-11 05:27:09 %
 *
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

/*
 * This file contains the opcode definitions for the PCM API.
 */

#ifndef _PCM_OPS_H
#define	_PCM_OPS_H

/*
 * Define the opcodes.
 *
 *
 * do NOT! change these values!
 *
 *
 * NOTE: the opcode number space is divided into space reserved for
 *       definition by Portal and space reserved for customer definition.
 *       The spaces are allocated as follows:
 *
 *              opcode number           reserved for
 *              ------------------------------------
 *              0 - 9999                Portal Only
 *              10,000 - 999,999        Customer Use
 *              1,000,000 - 9,999,999   Portal Only
 *              10,000,000+             Customer Use
 *
 * DO NOT define custom opcodes in the ranges reserved for Portal.
 *
 * DO NOT ADD ANY defines OTHER THAN OPCODES IN THIS FILE. THIS FILE IS USED
 * TO AUTOMATICALLY GENERATE OPCODE MAPPINGS.
 *
 */
	/*
	 * Generic Opcodes
	 */
#define PCM_OP_INVALID		0	/* invalid operation */
#define PCM_OP_CREATE_OBJ	1	/* create object */
#define PCM_OP_DELETE_OBJ	2	/* delete object */
#define PCM_OP_READ_OBJ		3	/* read entire object */
#define PCM_OP_READ_FLDS	4	/* read list of fields */
#define PCM_OP_WRITE_FLDS	5	/* write list of fields */
#define PCM_OP_DELETE_FLDS	6	/* delete list of fields */
#define PCM_OP_SEARCH		7	/* search */
#define PCM_OP_INC_FLDS		8	/* increment field(s) */
#define PCM_OP_LOGIN		9	/* "login" */
#define PCM_OP_LOGOFF		10	/* "logoff" */
#define PCM_OP_TEST_LOOPBACK	11	/* dup input flist & return */
#define PCM_OP_TRANS_OPEN	12	/* transaction open */
#define PCM_OP_TRANS_ABORT	13	/* transaction abort */
#define PCM_OP_TRANS_COMMIT	14	/* transaction commit */
#define PCM_OP_PASS_THRU	15	/* invoke pass_thru function */
#define PCM_OP_GET_CM_STATS	16	/* get CM status from CM */
#define PCM_OP_STEP_SEARCH	17	/* invoke step searching 	*/
#define PCM_OP_STEP_NEXT	18	/* get the next set of results	*/
#define PCM_OP_STEP_END		19	/* get the rest of the results	*/
#define PCM_OP_GET_DD           20      /* get some DM schema config info */
#define PCM_OP_SET_DD           21      /* set some DM schema config info */
#define PCM_OP_TRANS_POL_OPEN	22	/* transaction open policy */
#define PCM_OP_TRANS_POL_ABORT	23	/* transaction abort policy */
#define PCM_OP_TRANS_POL_COMMIT	24	/* transaction commit policy */
#define PCM_OP_GLOBAL_SEARCH		25	/* Global search */
#define PCM_OP_GLOBAL_STEP_SEARCH	26	/* invoke Global step searching 	*/
#define PCM_OP_GLOBAL_STEP_NEXT	27	/* get the next set of results	*/
#define PCM_OP_GLOBAL_STEP_END		28	/* get the rest of the results	*/
#define PCM_OP_SET_DM_CREDENTIALS	29	/* Set DM Credentials -- only for CM's use */
#define PCM_OP_TRANS_POL_PREP_COMMIT	30	/* pre trans commit policy */
	/* reserve 31 - 49 */

	/*
	 * Opcodes for Customer Mngmt FM (50 - 89)
	 */
#define PCM_OP_CUST_CREATE_ACCT			50
#define PCM_OP_CUST_CREATE_CUSTOMER		51
#define PCM_OP_CUST_CREATE_SERVICE		52
#define PCM_OP_CUST_DELETE_ACCT			53
#define PCM_OP_CUST_DELETE_CUSTOMER		54	/* Obsolete */
#define PCM_OP_CUST_INIT_ACCT			55
#define PCM_OP_CUST_INIT_SERVICE		56
#define PCM_OP_CUST_PREP_CUSTOMER		57
#define PCM_OP_CUST_SET_BILLINFO		58
#define PCM_OP_CUST_SET_LOGIN			59
#define PCM_OP_CUST_SET_NAMEINFO		60
#define PCM_OP_CUST_SET_PASSWD			61
#define PCM_OP_CUST_SET_STATUS			62
#define PCM_OP_CUST_COMMIT_CUSTOMER		63
#define PCM_OP_CUST_MODIFY_CUSTOMER		64
#define PCM_OP_CUST_SET_ACTGINFO		65
#define PCM_OP_CUST_SET_LOCALE			66
#define PCM_OP_CUST_FIND			67
#define PCM_OP_CUST_CREATE_PROFILE		68
#define PCM_OP_CUST_FIND_PROFILE		69
#define PCM_OP_CUST_MODIFY_PROFILE		70
#define PCM_OP_CUST_DELETE_PROFILE		71
#define PCM_OP_CUST_SET_PRODUCT_STATUS		72
#define PCM_OP_CUST_CREATE_PAYINFO              73
#define PCM_OP_CUST_MODIFY_PAYINFO		74
#define PCM_OP_CUST_FIND_PAYINFO		75
#define PCM_OP_CUST_DELETE_PAYINFO		76
#define PCM_OP_CUST_SET_PAYINFO			77
#define PCM_OP_CUST_VALID_FLD			78
#define PCM_OP_CUST_MODIFY_SERVICE		79
#define PCM_OP_CUST_SET_TAXINFO			80
#define PCM_OP_CUST_READ_ACCT_PRODUCTS		81
#define PCM_OP_CUST_GET_HISTORY     		82
#define PCM_OP_CUST_VALIDATE_CUSTOMER     	83
#define PCM_OP_CUST_UPDATE_CUSTOMER     	84
#define PCM_OP_CUST_SET_BRANDINFO		85

	/* reserve 86 - 89 */

	/*
	 * Opcodes for Billing FM (90 - 149)
	 */
#define PCM_OP_BILL_SPONSOR_GROUP_DELETE	97	
#define PCM_OP_BILL_CURRENCY_CONVERT_AMOUNTS		 98
#define PCM_OP_BILL_CURRENCY_QUERY_CONVERSION_RATES	 99
#define PCM_OP_BILL_CANCEL_PRODUCT		100
#define PCM_OP_BILL_CHARGE			101
#define PCM_OP_BILL_CYCLE_FORWARD		102
#define PCM_OP_BILL_CYCLE_FOLD			103
#define PCM_OP_BILL_CYCLE_ARREARS		104
#define PCM_OP_BILL_DEBIT			105
#define PCM_OP_BILL_SET_LIMIT			106
#define PCM_OP_BILL_PURCHASE_PRODUCT		107
#define PCM_OP_BILL_PURCHASE_DEAL		108
#define PCM_OP_BILL_RFR				109	/* Obsolete */
#define PCM_OP_BILL_MAKE_BILL			110
#define PCM_OP_BILL_MAKE_INVOICE		111	/* obsolete */
#define PCM_OP_BILL_ACCOUNTING			112	/* obsolete */
#define PCM_OP_BILL_COLLECT			113
#define PCM_OP_BILL_CHARGE_CC			114
#define PCM_OP_BILL_VALIDATE			115
#define PCM_OP_BILL_CYCLE_TAX			116
#define PCM_OP_BILL_GROUP_CREATE		117
#define PCM_OP_BILL_GROUP_DELETE		118
#define PCM_OP_BILL_GROUP_ADD_MEMBER            119
#define PCM_OP_BILL_GROUP_DELETE_MEMBER         120
#define PCM_OP_BILL_GROUP_GET_CHILDREN		121
#define PCM_OP_BILL_GROUP_GET_PARENT		122
#define PCM_OP_BILL_ITEM_DISPUTE		123
#define PCM_OP_BILL_ITEM_SETTLEMENT		124
#define PCM_OP_BILL_ITEM_ADJUSTMENT		125
#define PCM_OP_BILL_ITEM_TRANSFER		126
#define PCM_OP_BILL_ITEM_WRITEOFF		127
#define PCM_OP_BILL_ITEM_SEARCH			128
#define PCM_OP_BILL_SELECT_ITEMS		129
#define PCM_OP_BILL_RCV_PAYMENT			130
#define PCM_OP_BILL_FIND			131
#define PCM_OP_BILL_SET_PRODINFO		132
#define PCM_OP_BILL_LEDGER_REPORT		133
#define PCM_OP_BILL_CHARGE_INVOICE		134	/* obsolete */
#define PCM_OP_BILL_RECOVER			135
#define PCM_OP_BILL_RECOVER_CC			136
#define PCM_OP_BILL_VALIDATE_CC			137
#define PCM_OP_BILL_REVERSE_PAYMENT		138
#define PCM_OP_BILL_REVERSE			139
#define PCM_OP_BILL_CHARGE_DDEBIT		140
#define PCM_OP_BILL_EVENT_ADJUSTMENT		141
#define PCM_OP_BILL_SPONSOR_GROUP_CREATE	142
#define PCM_OP_BILL_SPONSOR_GROUP_ADD_MEMBER	143
#define PCM_OP_BILL_SPONSOR_GROUP_DELETE_MEMBER 144
#define PCM_OP_BILL_SPONSOR_GROUP_MODIFY	145
#define PCM_OP_BILL_ITEM_EVENT_SEARCH		146
#define PCM_OP_BILL_VIEW_INVOICE		147
#define PCM_OP_BILL_SET_INVOICE_STATUS		148	/* obsolete */
#define PCM_OP_BILL_GROUP_MOVE_MEMBER		149

	/*
	 * Opcodes for Activity Tracking FM (151 - 174)
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
	/* reserve 172 - 174 */

	/*
	 * Opcodes for Password FM (175-199) OBSOLETE
	 *
	 * 5.1 - defined, but not used
	 * 5.2 - not defined
	 *
	 * PCM_OP_PASSWD_APPROVE		175
	 * PCM_OP_PASSWD_GENERATE		176
	 * PCM_OP_PASSWD_ENCRYPT		177
	 * PCM_OP_PASSWD_COMPARE		178
	 */

        /* Opcodes for mecca FM */
#define PCM_OP_MECCA_INIT_SESSION		201
#define PCM_OP_MECCA_TERM_SESSION		202
#define PCM_OP_MECCA_SEND_STATUS		203
#define PCM_OP_MECCA_MIRROR_DATA		204
#define PCM_OP_MECCA_SEND_PARAMS		205
#define PCM_OP_MECCA_CREATE_ACCT		206
#define PCM_OP_MECCA_CONFIRM_SENT		207
#define PCM_OP_MECCA_SEND_POPLIST		208
	/* reserve 209 - 224 */

	/*  Opcodes for Pol_Mecca FM */
#define PCM_OP_MECCA_POL_MAP_CONFIG		225
#define PCM_OP_MECCA_POL_MAP_CREATE		226
#define PCM_OP_MECCA_POL_MAP_POPLIST		227
#define PCM_OP_MECCA_POL_MAP_INTRO_MSG		228
#define PCM_OP_MECCA_POL_MAP_VALID		229
#define PCM_OP_MECCA_POL_GET_PARAMS		230
	/* reserve 231 - 249 */

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
#define PCM_OP_CUST_POL_PREP_ACTGINFO		282
#define PCM_OP_CUST_POL_VALID_ACTGINFO		283
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
	/* reserve 297 - 299 */
        /* See 2150 for additional customer policy opcodes */

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
	/* reserve 308 - 324 */


	/* opcodes for mail FM */
#define PCM_OP_MAIL_DELIV_VERIFY		325
#define PCM_OP_MAIL_LOGIN_VERIFY		326

	/* reserve 327 - 349 */

	/* opcodes for term FM */
#define PCM_OP_TERM_IP_DIALUP_LOGIN		350	/* obsolete */
#define PCM_OP_TERM_IP_DIALUP_LOGOUT		351	/* obsolete */
#define PCM_OP_TERM_IP_DIALUP_AUTHENTICATE	352
#define PCM_OP_TERM_IP_DIALUP_START_SESSION	353	/* obsolete */
#define PCM_OP_TERM_IP_DIALUP_AUTHORIZE		354
#define PCM_OP_TERM_IP_DIALUP_START_ACCOUNTING	355
#define PCM_OP_TERM_IP_DIALUP_UPDATE_ACCOUNTING	356
#define PCM_OP_TERM_IP_DIALUP_STOP_ACCOUNTING	357
#define PCM_OP_TERM_IP_DIALUP_ACCOUNTING_ON	358
#define PCM_OP_TERM_IP_DIALUP_ACCOUNTING_OFF	359
	/* reserve 360 - 374 */

	/* opcodes for term policy FM */
#define PCM_OP_TERM_POL_IP_DIALUP_SPEC_RATES	375	/* obsolete */
#define PCM_OP_TERM_POL_GET_DYN_IPADDR		376	/* obsolete */
#define PCM_OP_TERM_POL_REVERSE_IP		377
#define PCM_OP_TERM_POL_AUTHORIZE		378
#define PCM_OP_TERM_POL_ACCOUNTING		379
	/* reserve 380 - 399 */

	/* opcodes for billing policy FM */
#define PCM_OP_BILL_POL_COLLECT			400
#define PCM_OP_BILL_POL_VALIDATE		401
#define PCM_OP_BILL_POL_SPEC_COLLECT		402
#define PCM_OP_BILL_POL_SPEC_VALIDATE		403
#define PCM_OP_BILL_POL_VALID_DISPUTE		404
#define PCM_OP_BILL_POL_VALID_SETTLEMENT	405
#define PCM_OP_BILL_POL_VALID_ADJUSTMENT	406
#define PCM_OP_BILL_POL_VALID_TRANSFER		407
#define PCM_OP_BILL_POL_VALID_WRITEOFF		408
#define PCM_OP_BILL_POL_SPEC_BILLNO		409
#define PCM_OP_BILL_POL_UNDER_PAYMENT		410
#define PCM_OP_BILL_POL_OVER_PAYMENT		411
#define PCM_OP_BILL_POL_FORMAT_INVOICE		412	/* obsolete */

/*
 * RESERVE THE OPCODE NUMBER FOR BILL_GROUP_AUTHORIZATION.
 */
/* #ifdef 	BILL_GROUP_AUTHORIZATION */
#define PCM_OP_BILL_POL_SPEC_RELATION		413	/* Obsolete */
/* #endif */	/* BILL_GROUP_AUTHORIZATION */

#define PCM_OP_BILL_POL_PRE_FOLD		414
#define PCM_OP_BILL_POL_BILL_PRE_COMMIT		415
#define PCM_OP_BILL_POL_PRE_COLLECT		416
#define PCM_OP_BILL_POL_PURCHASE_PROD_PROVISIONING	417
#define PCM_OP_BILL_POL_CANCEL_PROD_PROVISIONING	418
#define PCM_OP_BILL_POL_GET_PROD_PROVISIONING_TAGS	419
#define PCM_OP_BILL_POL_VALID_INVOICE_STATUS		420	/* obsolete */
#define PCM_OP_BILL_POL_SPEC_FUTURE_CYCLE	421
#define PCM_OP_BILL_POL_SPEC_CYCLE_FEE_INTERVAL	422
#define PCM_OP_BILL_POL_SPEC_CANCEL		423
#define PCM_OP_BILL_POL_POST_BILLING		424
	/* reserve 425 - 449 */

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

	/* opcodes for rating FM */
#define PCM_OP_RATE_EVENT			500
#define PCM_OP_RATE_TAX_EVENT			501
#define PCM_OP_RATE_TAX_CALC			502
#define PCM_OP_RATE_TAX_DEFER			503	/* obsolete */
#define PCM_OP_RATE_GET_PRODLIST		504
#define PCM_OP_RATE_CACHE_PRODUCT		505
	/* reserve 505 - 549 */

	/* opcodes for rating policy FM */
#define PCM_OP_RATE_POL_TAX_LOC			550
#define PCM_OP_RATE_POL_GET_TAX_SUPPLIER        551
#define PCM_OP_RATE_POL_MAP_TAX_SUPPLIER        552
#define PCM_OP_RATE_POL_POST_RATING        	553
	/* reserve 554 - 574 */

        /* opcodes for SDK FM */
#define PCM_OP_SDK_GET_FLD_SPECS                575
#define PCM_OP_SDK_SET_FLD_SPECS                576
#define PCM_OP_SDK_GET_OBJ_SPECS                577
#define PCM_OP_SDK_SET_OBJ_SPECS                578
#define PCM_OP_SDK_GET_OP_SPECS                 579
#define PCM_OP_SDK_SET_OP_SPECS                 580
#define PCM_OP_SDK_GET_SEARCH                   581
#define PCM_OP_SDK_SET_SEARCH                   582
#define PCM_OP_SDK_DEL_OBJ_SPECS                583
#define PCM_OP_SDK_DEL_OP_SPECS                 584
#define PCM_OP_SDK_DEL_FLD_SPECS                585
	/* reserve 586 - 649 */

	/* opcodes for pricelist FM */
#define PCM_OP_PRICE_COMMIT_PLAN		659
#define PCM_OP_PRICE_COMMIT_DEAL		660
#define PCM_OP_PRICE_COMMIT_PRODUCT		661
#define PCM_OP_PRICE_GET_PRICE_LIST		662
#define PCM_OP_PRICE_SET_PRICE_LIST		663
#define PCM_OP_PRICE_COMMIT_PLAN_LIST		666
#define PCM_OP_PRICE_SET_CAAR_MATRIX		667	/* Obsolete */
#define PCM_OP_PRICE_GET_CAAR_MATRIX		668	/* Obsolete */
#define PCM_OP_PRICE_DELETE_CAAR_MATRIX		669	/* Obsolete */
	/* reserve 670 - 674 */

	/* opcodes for pricelist policy FM */
#define PCM_OP_PRICE_POL_DELETE_DEAL		675
#define PCM_OP_PRICE_POL_DELETE_PRODUCT		676
#define PCM_OP_PRICE_POL_DELETE_RATE		677
#define PCM_OP_PRICE_POL_PREP_DEAL		678
#define PCM_OP_PRICE_POL_PREP_PRODUCT		679
#define PCM_OP_PRICE_POL_PREP_RATE		680
#define PCM_OP_PRICE_POL_VALID_DEAL		681
#define PCM_OP_PRICE_POL_VALID_PRODUCT		682
#define PCM_OP_PRICE_POL_VALID_RATE		683
#define PCM_OP_PRICE_POL_SET_CAAR_MATRIX	684	/* Obsolete */
#define PCM_OP_PRICE_POL_GET_CAAR_MATRIX	685	/* Obsolete */
	/* reserve 684 - 699 */

	/* opcode for Direct Debit payment */

	/* opcodes for remittance calculation */
	/* skip 700, was CHARGE_DDEBIT */
#define PCM_OP_REMIT_REMIT			701
	/* reserve 702 - 724 */


	/* opcodes for channel FM */
#define PCM_OP_CHANNEL_PUSH			725
#define PCM_OP_CHANNEL_SYNC			726
	/* reserve  - 744 */

	/* opcodes for replicable objects */
#define PCM_OP_REPL_POL_PUSH			745
	/* reserve - 774 */

        /* opcodes for infranet Management */
#define PCM_OP_INFMGR_START_SERVER              800
#define PCM_OP_INFMGR_STOP_SERVER               801
#define PCM_OP_INFMGR_GET_STATUS                802
#define PCM_OP_INFMGR_ADD_OBJECT             	803
#define PCM_OP_INFMGR_DELETE_OBJECT             804
#define PCM_OP_INFMGR_ADD_LOG                   807
#define PCM_OP_INFMGR_GET_INFO                  808
#define PCM_OP_INFMGR_MODIFY_MONITOR_INTERVAL   809
#define PCM_OP_INFMGR_SCHEDULE_DOWNTIME         810
#define PCM_OP_INFMGR_CANCEL_DOWNTIME           811
#define PCM_OP_INFMGR_SATELLITE_CM_START_FORWARDING		812
#define PCM_OP_INFMGR_SATELLITE_CM_STOP_FORWARDING		813
	/* reserved 810 - 830 */

        /* opcodes for delivery FM */
#define PCM_OP_DELIVERY_MAIL_SENDMSGS		831
	/* reserved 832 - 860 */

	/* opcodes for broadband FM */
#define PCM_OP_BROADBAND_GET_IPADDRS		861
#define PCM_OP_BROADBAND_SET_IPADDRS		862
#define PCM_OP_BROADBAND_FIND_BY_IPADDR		863
	/* reserved 863 - 879 */

	/* opcodes for ipt FM */
#define PCM_OP_IPT_AUTHENTICATE			880
#define PCM_OP_IPT_AUTHORIZE			881
#define PCM_OP_IPT_START_ACCOUNTING		882
#define PCM_OP_IPT_STOP_ACCOUNTING		883
#define PCM_OP_IPT_ACCOUNTING_ON		884
#define PCM_OP_IPT_ACCOUNTING_OFF		885
#define PCM_OP_IPT_GET_CALL_FEATURES		886
#define PCM_OP_IPT_SET_CALL_FEATURES		887

	/* reserved 886 - 889 */

	/* opcodes for ipt policy FM */
#define PCM_OP_IPT_POL_AUTHORIZE		890
#define PCM_OP_IPT_POL_ACCOUNTING		891
	/* reserved 891 - 899 */

	/* opcodes for NetFlow FM */
#define PCM_OP_NETFLOW_UPDATE_USAGE		901
#define PCM_OP_NETFLOW_RATE_USAGE		902
	/* reserved - 910 */

	/* opcodes for NetFlow Policy FM */
#define PCM_OP_NETFLOW_POL_CALC_BANDWIDTH	911
#define PCM_OP_NETFLOW_POL_SPEC_RATES		912
#define PCM_OP_NETFLOW_POL_SPEC_IMPACT_CATEGORY	913
	/* reserved - 920 */

	/* More opcodes for Billing FM */
#define PCM_OP_BILL_BILL_WRITEOFF		921
#define PCM_OP_BILL_ACCOUNT_WRITEOFF		922
#define PCM_OP_BILL_ACCOUNT_ADJUSTMENT		923
#define PCM_OP_BILL_PURCHASE_FEES		924
#define PCM_OP_BILL_ITEM_REFUND  		925
#define PCM_OP_BILL_CANCEL_DEAL			926
#define PCM_OP_BILL_VALIDATE_DD			927
#define PCM_OP_BILL_CHARGE_DD			928
#define PCM_OP_BILL_RECOVER_DD			929
#define PCM_OP_BILL_CHANGE_DEAL			930
#define PCM_OP_BILL_TRANSFER_BALANCE		931
	/* reserved - 932 - 950 */

/* Include the header file with the
 * customized op codes
 */

#define PCM_OP_INV_VIEW_INVOICE       		951 
#define PCM_OP_INV_POL_FORMAT_VIEW_INVOICE	952 
#define PCM_OP_INV_MAKE_INVOICE       		953 
#define PCM_OP_INV_POL_PREP_INVOICE     	954 
#define PCM_OP_INV_POL_FORMAT_INVOICE 		955 
#define PCM_OP_INV_POL_FORMAT_INVOICE_HTML	956 
#define PCM_OP_INV_POL_FORMAT_INVOICE_DOC1	957 
#define PCM_OP_INV_POL_FORMAT_INVOICE_XML 	958 

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
	/* reserved 987 - 1000 */

	/* opcodes for zonemap standard FM */
#define PCM_OP_ZONEMAP_COMMIT_ZONEMAP			1001
#define PCM_OP_ZONEMAP_GET_ZONEMAP			1002
	/* reserved - 1003 - 1050 */

	/* opcodes for zonemap policy FM */
#define PCM_OP_ZONEMAP_POL_SET_ZONEMAP			1051
#define PCM_OP_ZONEMAP_POL_GET_ZONEMAP			1052
#define PCM_OP_ZONEMAP_POL_GET_LINEAGE			1053
	/* reserved - 1054 - 1100 */

	/* opcodes for GEL (Generic Event Loader) FM */
#define PCM_OP_GEL_TEMPLATE_CREATE     		1101 
#define PCM_OP_GEL_TEMPLATE_MODIFY     		1102
#define PCM_OP_GEL_TEMPLATE_READ     		1103
#define PCM_OP_GEL_TEMPLATE_DELETE     		1104
#define PCM_OP_GEL_FIND_TEMPLATES     		1105
        /* reserved - 1120 */

	/* opcodes for On Demand Billing */
#define PCM_OP_BILL_MAKE_BILL_ON_DEMAND		1121 
#define PCM_OP_BILL_MAKE_BILL_NOW     		1122
        /* reserved - 1130 */

	/* opcodes for dm opstore */
#define PCM_OP_ALDM_REPLAY_LOGS			1131
	/* reserved - 1132 - 1150 */

	/* opcodes for Bulkacct FM */
#define PCM_OP_BULKACCT_SET_BATCH 		1200 
#define PCM_OP_BULKACCT_GET_BATCH 		1201 
	/* reserved 1202 - 1249 */

	/* opcodes for satellite policy */
#define PCM_OP_SAT_POL_PRE_STORE		1250
#define PCM_OP_SAT_POL_POST_STORE		1251
#define PCM_OP_SAT_POL_MASTER_DOWN		1252
	/* reserved - 1260 */

#define PCM_OP_PUBLISH_EVENT			1300
#define PCM_OP_PUBLISH_GEN_PAYLOAD		1301

        /* additional opcodes for customer policy FM */
#define PCM_OP_CUST_POL_SET_BRANDINFO           2150
        /* reserved through 2199 */

/* opcodes for ar tool Reserved till 1320 */

#define PCM_OP_AR_BILL_SEARCH			1310
#define PCM_OP_AR_BILL_ITEM_SEARCH		1311
#define PCM_OP_AR_HIERARCHY_SEARCH		1312
#define PCM_OP_AR_OTHER_ITEM_SEARCH		1313
#define PCM_OP_AR_HISTORY_SEARCH		1314
#define PCM_OP_AR_PAYMENT_SEARCH		1315
        /* opcodes for ar tool Reserved till 1320 */

#endif /* !_PCM_OPS_H */
