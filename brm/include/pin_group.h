/* file information --- @(#)%Portal Version: pin_group.h:CommonIncludeInt:% */
/*******************************************************************
 *
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef _PIN_GROUP_H
#define	_PIN_GROUP_H

/*******************************************************************
 * General defines
 *******************************************************************/
#define DONT_SET_FLIST_ERRORS		1
#define DO_SET_FLIST_ERRORS		0
#define MATCH_FOUND			PIN_BOOLEAN_TRUE
#define NO_MATCH_FOUND 			PIN_BOOLEAN_FALSE

/*******************************************************************
 * Group FM Definitions.
 *******************************************************************/

/*******************************************************************
 * Object Name Strings
 *******************************************************************/
#define PIN_OBJ_NAME_GROUP		"Group Object"
#define PIN_OBJ_NAME_EVENT_GROUP	"Group Management Event Log"

/*******************************************************************
 * Object Type Strings
 *******************************************************************/
#define PIN_OBJ_TYPE_EVENT_GROUP_MEMBER "/event/group/member"
#define PIN_OBJ_TYPE_EVENT_GROUP_PARENT "/event/group/parent"
#define PIN_OBJ_TYPE_GROUP		"/group"
#define PIN_OBJ_TYPE_GROUP_SHARING           "/group/sharing"
#define PIN_OBJ_TYPE_GROUP_SHARING_DISCOUNTS "/group/sharing/discounts"
#define PIN_OBJ_TYPE_GROUP_SHARING_PRODUCTS "/group/sharing/products"
#define PIN_OBJ_TYPE_GROUP_SHARING_CHARGES   "/group/sharing/charges"
#define PIN_OBJ_TYPE_GROUP_SHARING_PROFILES  "/group/sharing/profiles"
#define PIN_OBJ_TYPE_GROUP_SHARING_MONITOR   "/group/sharing/monitor"

/*******************************************************************
 * Event Description Strings
 *******************************************************************/
#define PIN_EVENT_DESCR_GROUP_MEMBER_DELETE	"Delete Member(s)"
#define PIN_EVENT_DESCR_GROUP_MEMBER_ADD	"Add Member(s)"
#define PIN_EVENT_DESCR_BILL_GROUP_MEMBER_DELETE	"Billing Group Member(s) Deleted"
#define PIN_EVENT_DESCR_BILL_GROUP_MEMBER_ADD		"Billing Group Member(s) Added"
#define PIN_EVENT_DESCR_SPONSOR_GROUP_MEMBER_DELETE	"Sponsor Group Member(s) Deleted"
#define PIN_EVENT_DESCR_SPONSOR_GROUP_MEMBER_ADD	"Sponsor Group Member(s) Added"
#define PIN_EVENT_DESCR_GROUP_PARENT		"Set Parent"

/*******************************************************************
 * Group delete member related
 *******************************************************************/
#define PIN_GROUP_DELETE_MEMBER_FLAG_ANY        0x0001	/* Use this flag to   */
							/* avoid 'subordinate */
							/* check' - for       */							/* internal use only! */

#endif	/*_PIN_GROUP_H*/
