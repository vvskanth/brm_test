/* Continuus file information --- %full_filespec: pin_gsm.h~5:incl:1 % */
/*
 * @(#) %full_filespec: pin_gsm.h~5:incl:1 %
 *
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PIN_GSM_H
#define _PIN_GSM_H

/*
 * pin_cust.h being included for the feature_service_era_status below.
 */
#include "pin_cust.h"

/*
 * String constants for service class/object.
 */
#define PIN_OBJ_TYPE_SERVICE_GSM_TEL            "/service/gsm/telephony"
#define PIN_OBJ_TYPE_SERVICE_GSM_SMS            "/service/gsm/sms"
#define PIN_OBJ_TYPE_SERVICE_GSM_FAX            "/service/gsm/fax"
#define PIN_OBJ_TYPE_SERVICE_GSM_DATA           "/service/gsm/data"

/*
 * String constants for config class/object, service names and keywords.
 */
#define PIN_OBJ_TYPE_CONFIG_GSMTAGS_TEL         "/config/gsmtags/telephony"
#define PIN_OBJ_TYPE_CONFIG_GSMTAGS_SMS         "/config/gsmtags/sms"
#define PIN_OBJ_TYPE_CONFIG_GSMTAGS_FAX         "/config/gsmtags/fax"
#define PIN_OBJ_TYPE_CONFIG_GSMTAGS_DATA        "/config/gsmtags/data"
#define PIN_OBJ_TYPE_CONFIG_ACC_ERA             "/config/account_era"

#define PIN_OBJ_TYPE_PROFILE_SERV_EXTRA         "/profile/serv_extrating"


typedef enum {
	PIN_ACTION_CANCEL       = 0,
	PIN_ACTION_ADD          = 1,
	PIN_ACTION_MODIFY       = 2
} provision_status_t;

typedef feature_service_era_status_t gsm_feature_service_era_status_t;

#endif
