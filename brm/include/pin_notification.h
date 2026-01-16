/* Copyright (c) 2021,2022 Oracle and/or its affiliates. */
/*
*      This material is the confidential property of Oracle Corporation or its
*      licensors and may be used, reproduced, stored or transmitted only in
*      accordance with a valid Oracle license or sublicense agreement.
*/
#ifndef _PIN_NOTIFICATION_H
#define _PIN_NOTIFICATION_H
#define PIN_CONFIG_NOTIF_SPEC_TYPE 	"/config/notification_spec"
#define PIN_CONFIG_NOTIF_SPEC_NAME 	"config_notification_spec"
#define PIN_OBJ_TYPE_PROFILE  		"/profile"
#define PIN_CONFIG_NOTIF_DELIVERY       "/config/delivery_methods"
#define PIN_NOTIFY_POST_DUE_ENABLED_FLG 0x0001

#define PIN_CONFIG_NOTIFY_ALL   	0
#define PIN_CONFIG_NOTIFY_ADVANCE     	1
#define PIN_CONFIG_NOTIFY_REAL_TIME     2
#define PIN_CONFIG_NOTIFY_PREDEFINED  	3
#define PIN_CONFIG_NOTIFY_FIRST_TRANS   4
#define PIN_CONFIG_NOTIFY_POST_DUE	5

#define NOTIFY_OPT_NOTIFY_UNDEFINED		0
#define NOTIFY_OPT_NOTIFY_IF_OPTED_IN		1  /* Requires OptIn - Notify ONLY if OptedIn  */
#define NOTIFY_OPT_NOTIFY_IF_NOT_OPTED_OUT	2  /* Requires OptOut - Notify always but not when opted out */
#define NOTIFY_OPT_NOTIFY_ALWAYS		3  /* Always Notify - No option to OptOut */

#define NOTIFY_AGGR_MODE_SINGLE_MSG             0               
#define NOTIFY_AGGR_MODE_CLUB_MSG   1
#define NOTIFY_AGGR_MODE_CLUB_MSG_ACCOUNT_LEVEL 1   
#define NOTIFY_AGGR_MODE_CLUB_MSG_OWN_LEVEL     2  

#define CONFIG_NOTIFY_DEL_METHOD	"config_notification_spec"
#define PIN_NOTIFICATION_PROGRAM_NAME   "notification spec"

/* Defined to fetch the max search results to handle in get opcodes */
#define NOTIFICATION_GET_RESULTS_LIMIT               500
 
#define PIN_NOTIFICATION_NOTIFY_CREATE_EVT "/event/notification/create/notification_spec"
#define PIN_NOTIFICATION_NOTIFY_MODIFY_EVT "/event/notification/update/notification_spec"
#define PIN_NOTIFICATION_NOTIFY_DELETE_EVT "/event/notification/delete/notification_spec"
#define PIN_NOTIFY_POST_DUE_ENABLED_FLG 0x0001

#define NOTIFICATON_OPT_OUT_ALL_VALUE "ALL"
#define NOTIFICATON_OPT_IN_ALL_VALUE "ALL"

/* Each Bit in PIN_FLD_NOTIFY_SEARCH_LEVEL represents the Search options*/

#define	PIN_NOTIFY_SEARCH_LEVEL_OWN_SELF	0x0001
#define	PIN_NOTIFY_SEARCH_LEVEL_ACCOUNT		0x0002
#define	PIN_NOTIFY_SEARCH_LEVEL_BAL_GRP		0x0004
#define	PIN_NOTIFY_SEARCH_LEVEL_SERVICE		0x0008
#define	PIN_NOTIFY_SEARCH_LEVEL_BILLINFO	0x0010

#endif
