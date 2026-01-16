/*
 * (#)$Id: pin_prov_wireless.h /cgbubrm_7.5.0.portalbase/1 2015/11/27 05:53:23 nishahan Exp $
 *
 * Copyright (c) 2001, 2009, Oracle and/or its affiliates.All rights reserved. 
 *
 * This material is the confidential property of Oracle Corporation 
 * or its licensors and may be used, reproduced, stored or transmitted
 * in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef _PIN_PROV_WIRELESS_H
#define _PIN_PROV_WIRELESS_H


/* Object Type Strings */
#define PIN_OBJ_TYPE_EVENT_GSM_SVC_ORDER "/event/provisioning/service_order/gsm"
#define PIN_OBJ_TYPE_SERVICE_GSM             "/service/gsm"

#define PIN_PROV_MSISDN_TAG                  "MSISDN"
#define PIN_PROV_IMSI_TAG                    "IMSI"
#define PIN_PROV_DEVICE_SIM_TAG              "SIM"
#define PIN_PROV_PIN1_TAG                    "PIN1"
#define PIN_PROV_PIN2_TAG                    "PIN2"
#define PIN_PROV_PUK1_TAG                    "PUK1"
#define PIN_PROV_PUK2_TAG                    "PUK2"
#define PIN_PROV_KI_TAG                      "KI"
#define PIN_PROV_ADM1_TAG                    "ADM1"
#define PIN_PROV_NETWORK_ELEMENT_TAG         "NET"


/* Service Order Status  */
typedef enum  pin_prov_gsm_so_status{

	PIN_SVC_ORDER_STATUS_SUCCESS            = 0,
	PIN_SVC_ORDER_STATUS_FAILED             = 1,
	PIN_SVC_ORDER_STATUS_PROVISIONING       = 2,
	PIN_SVC_ORDER_STATUS_PRE_PROVISIONING   = 3

} pin_prov_gsm_so_status_t;

#define PIN_PROV_WIRELESS_PROGRAM_NAME  "Wireless Provisioning"

/* Service Order Actions     */
#define PIN_ACTION_PRE_PROVISIONING     "P"
#define PIN_ACTION_ACTIVATE             "A"
#define PIN_ACTION_DEACTIVATE           "D"
#define PIN_ACTION_SUSPEND              "S"
#define PIN_ACTION_CHANGE               "C"
#define PIN_ACTION_REACTIVATE           "R"
#define PIN_ACTION_IGNORE               "I"

#endif
