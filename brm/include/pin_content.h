/*******************************************************************
 *
 *      @(#)$Id: pin_content.h /cgbubrm_7.5.0.portalbase/1 2015/11/27 05:52:40 nishahan Exp $
 *
 * Copyright (c) 2001, 2009, Oracle and/or its affiliates. 
 * All rights reserved. 
 *      
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *
 *******************************************************************/

#ifndef _PIN_CONTENT_H
#define	_PIN_CONTENT_H

#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************
 * Common Constants
  *******************************************************************/
  #define CONTENT_ACTIVITY_EVENT       "/event/activity/content"
  #define CONTENT_ACTIVITY_SUB_EVENT   "/content"

/*******************************************************************
 * CONTENT FM Definitions.
 *******************************************************************/
/*
 * Warning codes used by PCM_OP_CONTENT_SET_SRVC_FEATURES
 */
#define	PIN_CONTENT_NO_CONFLICT				0x00
#define	PIN_CONTENT_CONFLICT				0x01
#define	PIN_CONTENT_BOTH_ALLOW_AND_DENY_LIST_SPECIFIED	0x02
#define	PIN_CONTENT_DUPLICATE_CATEGORY_ENTRIES		0x03
#define	PIN_CONTENT_DUPLICATE_CATEGORIES		0x04
#define PIN_CONTENT_INVALID_MODE_SPECIFIED              0x05

/*
 * Error codes used by PCM_OP_CONTENT_AUTHORIZE
 */

/*
 *  Other codes used by PCM_OP_CONTENT_AUTHORIZE and
 *  PCM_OP_CONTENT_POL_AUTHORIZE
 */
#define	PIN_CONTENT_NO_CALC_MAX				0x00
#define	PIN_CONTENT_CALC_MAX				0x01

/* Possible return codes for PCM_OP_CONTENT_POL_CHECK_ACCESS_PRIVILEGE
 */
#define	PIN_CONTENT_DENY_ACCESS				0x00
#define	PIN_CONTENT_GRANT_ACCESS			0x01
#define	PIN_CONTENT_INSUFFICIENT_PRIVILEGE		0x02
#define	PIN_CONTENT_CP_UNKNOWN				0x03
#define	PIN_CONTENT_USER_UNKNOWN			0x04
#define	PIN_CONTENT_DUPLICATE_RECORD			0x05
#define	PIN_CONTENT_DENIED_ACCESS_TO_CONTENT_CATEGORY   0x06
#define	PIN_CONTENT_BALANCE_VERIFICATION_REQUIRED	0x07
#define	PIN_CONTENT_BALANCE_VERIFICATION_NOT_REQUIRED   0x08
#define	PIN_CONTENT_CANNOT_PURCHASE_CONTENT_CATEGORY	0x09
#define	PIN_CONTENT_INVALID_AUTHORIZATION_ID		0x0A
#define PIN_CONTENT_STORAGE_DISCONNECT 			0x0B

/*
 * These are the return codes that are returned by the 
 * act find and hence by the authentication opcode.
 */
#define PIN_ACT_CHECK_UNDEFINED 			0x00
#define PIN_ACT_CHECK_ACCT_TYPE				0x01
#define PIN_ACT_CHECK_ACCT_STATUS			0x02
#define PIN_ACT_CHECK_ACCT_PASSWD			0x03
#define PIN_ACT_CHECK_SRVC_TYPE				0x04
#define PIN_ACT_CHECK_SRVC_STATUS			0x05 
#define PIN_ACT_CHECK_SRVC_PASSWD			0x06
#define PIN_ACT_CHECK_CREDIT_AVAIL			0x07 

/* Values for SUCCESS and FAILURE to be returned from
 * aaccounting opcodes
  */
#define         PIN_CONTENT_STATUS_NOK            0
#define         PIN_CONTENT_STATUS_OK             1


#ifdef __cplusplus
}
#endif

#endif	/*_PIN_CONTENT_H*/
