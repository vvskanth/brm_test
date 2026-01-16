/*******************************************************************
 *
 * 	@(#)$Id: pin_inv.h /cgbubrm_7.5.0.portalbase/2 2016/08/30 09:31:38 bmaturi Exp $ 
 *
* Copyright (c) 1999, 2016, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef _PIN_INV_H
#define	_PIN_INV_H

/********************************************************************
 * Format types recognized by Infranet.  Where possible, these are
 * MIME types
 ********************************************************************/

#define FM_INV_FORMAT_TYPE_HTML		"text/html"
#define FM_INV_FORMAT_TYPE_XML		"text/xml"
#define FM_INV_FORMAT_TYPE_DOC1		"text/doc1"
#define FM_INV_FORMAT_TYPE_PIN_FLIST	"text/pin_flist"

#define FM_INV_XML_ROOT_ELEMENT		"invoice"

/********************************************************************
 * Define the statuses used in invoicing.
 ********************************************************************/
#define PIN_INV_NONE			0
#define PIN_INV_EMAILED			1
#define PIN_INV_PRINTED			2

/********************************************************************
 * Define used in decoding the invoice_data cache
 ********************************************************************/
#define PIN_INV_DECODE_INVOICE_DATA_MARKER	'@'
#define PIN_INV_DECODE_CONFIG_OBJECT_TYPE	"/config/invoice_data_map%"
#define BUFSIZE 4096
#define IDSIZE	100
#define OPENBRACES '{'
#define CLOSEBRACES '}'
#define COMMA ','
#define PIPE '|'

#define BEGINARRAY      '<'
#define ENDARRAY        '>'
#define FIELDSEPARATOR  '#'
#define ELEMSEPARATOR   '|'
#define ESCAPENEXTCHAR  '\\'
#define ELEMIDPRESENT   '~'


/********************************************************************
 * Defines used in the load utility load_pin_invoice_data_map
 ********************************************************************/
#define LOAD_PIN_INVOICE_DATA_MAP_PROG		"load_pin_invoice_data_map"
#define LOAD_PIN_INVOICE_DATA_MAP_LOGLEVEL	"loglevel"

#define	LOAD_PIN_INVOICE_DATA_MAP_PARSE_ID		1
#define	LOAD_PIN_INVOICE_DATA_MAP_PARSE_FIELD		2
#define	LOAD_PIN_INVOICE_DATA_MAP_PARSE_ERROR		3
	
#define LOAD_PIN_INVOICE_DATA_MAP_DEFAULT_INPUT_FILE	"./pin_invoice_data_map"
#define LOAD_PIN_INVOICE_DATA_MAP_DEFAULT_CONFIG_OBJ 	"/config/invoice_data_map"

#define	LOAD_PIN_INVOICE_DATA_MAP_ID_KEYWORD			"ID"

/*******************************************************************
 * Defines for the bitmapped flags for perf_features_flags
 ******************************************************************/
#define INV_PERF_FEAT_NO_SUBSELECT              0x00000001
#define INV_PERF_FEAT_HARDCODE_EVENTS           0x00000002
#define INV_PERF_FEAT_CALC_ONLY                 0x00000004
#define INV_PERF_FEAT_USE_OPREF                 0x00000008
#define INV_PERF_FEAT_NO_RMEVENTS               0x00000010
#define INV_PERF_FEAT_SRCH                      0x00000020
#define INV_PERF_FEAT_ITEM                      0x00000040
#define INV_PERF_FEAT_EVENT                     0x00000080
#define INV_PERF_FEAT_MISC                      0x00000100
#define INV_PERF_FEAT_FIELDS_FROM_APP           0x00000200
#define INV_PERF_FEAT_KEEP_SPONSOR_BAL_IMPACTS	0x00000400
#define INV_PERF_FEAT_TELCO_INFO_FROM_CACHE     0x00000800
#define INV_PERF_FEAT_SKIP_DEVICES_INFO         0x00001000
#define INV_PERF_FEAT_SKIP_TAX_DETAILS          0x00002000
#define INV_PERF_FEAT_SKIP_PLAN_DETAILS         0x00004000
#define INV_PERF_FEAT_SKIP_BALANCE_DETAILS      0x00008000
#define INV_PERF_FEAT_SKIP_BALGRP_LOCK          0x00010000


/*******************************************************************
 * Result Type when invoice size is greater than the threshold
 ******************************************************************/
#define PIN_RESULT_BIG_INV_SIZE 		2

/*******************************************************************
 * Defines for the bitmapped flags for invoice types
 ******************************************************************/
#define PIN_INV_TYPE_DETAIL			0x00000001
#define PIN_INV_TYPE_SUMMARY			0x00000002
#define PIN_INV_TYPE_REGULAR			0x00000004
#define PIN_INV_TYPE_SUBORDINATE		0x00000008
#define PIN_INV_TYPE_PARENT			0x00000010
#define PIN_INV_TYPE_HIERARCHY			0x00000020
#define PIN_INV_TYPE_TRIAL_INVOICE		0x00000040
#define PIN_INV_TYPE_PARENT_WITH_SUBORDS	0x00000018
#define PIN_INV_TYPE_CORR_LETTER 		0x00000080
#define PIN_INV_TYPE_REPLACEMENT		0x00000100
#define PIN_INV_TYPE_RETRY			0x00000200

typedef enum pin_inv_status {
                /* Invoice document not generated */
        PIN_INV_STATUS_PENDING =    0x0,
                /* Final Invoice document  generated  */
        PIN_INV_STATUS_GENERATED=    0x1,
                /* Invoice regenerated  */
	PIN_INV_STATUS_REGENERATED = 0x2,
                /* Invoice document  scheduled for delivery */
	PIN_INV_STATUS_SCHEDULED = 0x3,
                /* Invoice document  generation error */
	PIN_INV_STATUS_ERRORED = 0x4,
                /* Invoice document  duplicated  */
	PIN_INV_STATUS_DUPLICATED = 0x5
} pin_inv_status_t;

/********************************************************************
 * Define the type of shadow events created by re-rating
 ********************************************************************/
#define INV_EVENT_SKIPPED		0
#define INV_EVENT_CYCLE_PURCHASE_CANCEL	1
#define INV_EVENT_USAGE			2

#endif	/*_PIN_INV_H*/
