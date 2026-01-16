/*	
 * @(#)$Id: tcf.h /cgbubrm_commoninclude/3 2012/07/31 23:32:10 souagarw Exp $ Portal Version: tcf.h:CommonIncludeInt:11:2007-Sep-13 03:49:30 %
 *	
* Copyright (c) 1996, 2012, Oracle and/or its affiliates. All rights reserved. 
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_TCF_OPS_H_
#define _PCM_TCF_OPS_H_

/*
 * This file contains the opcode definitions for the GENERIC TELCO PCM API.
 */

/* =====================================================================
 *   NAME            : PCM_TCF_OPS
 *   TOTAL RANGE     : 4001..4033, 4101..4199
 *   USED RANGE      : 4001..4033, 4101..4110
 *   RESERVED RANGE  : 4111..4199
 *   ASSOCIATED FMs  : fm_tcf, fm_tcf_aaa, fm_tcf_aaa_pol, fm_tcf_pol
 * =====================================================================
 */

#include "ops/base.h"

/* Opcodes for Telco framework - Will be deprecated in the future
 * because of the improper opcode naming convention.
 */
#define PCM_OP_TELCO_AUTHENTICATE                     4001
#define PCM_OP_TELCO_AUTHORIZE                        4002
#define PCM_OP_TELCO_REAUTHORIZE                      4003
#define PCM_OP_TELCO_CANCEL_AUTHORIZATION             4004
#define PCM_OP_TELCO_START_ACCOUNTING                 4005
#define PCM_OP_TELCO_UPDATE_ACCOUNTING                4006
#define PCM_OP_TELCO_STOP_ACCOUNTING                  4007
#define PCM_OP_TELCO_SVC_LISTENER                     4008
#define PCM_OP_TELCO_APPLY_PARAMETER                  4009
#define PCM_OP_TELCO_PROPAGATE_STATUS                 4010
#define PCM_OP_TELCO_POL_APPLY_PARAMETER              4011
#define PCM_OP_TELCO_ACCOUNTING                       4012
#define PCM_OP_TELCO_PROV_SERVICE_ORDER_SET_STATE     4013
#define PCM_OP_TELCO_PROV_SERVICE_ORDER_SET_ATTR      4014
#define PCM_OP_TELCO_PROV_SERVICE_ORDER_NOTIFY        4015
#define PCM_OP_TELCO_PROV_CREATE_SVC_ORDER            4016
#define PCM_OP_TELCO_PROV_HANDLE_SVC_ORDER            4017
#define PCM_OP_TELCO_PROV_UPDATE_SVC_ORDER            4018
#define PCM_OP_TELCO_PROV_UPDATE_PROV_OBJECT          4019
#define PCM_OP_TELCO_PROV_SET_ATTR                    4020
#define PCM_OP_TELCO_PROV_SIMULATE_AGENT              4021
#define PCM_OP_TELCO_ACCOUNTING_ON                    4022
#define PCM_OP_TELCO_ACCOUNTING_OFF                   4023

#define PCM_OP_TELCO_SEARCH_SESSION                   4024
#define PCM_OP_TELCO_PREP_INPUT                       4025
#define PCM_OP_TELCO_UPDATE_AND_REAUTHORIZE           4026

/* Opcodes for Telco framework. */
#define PCM_OP_TCF_AAA_AUTHENTICATE                   4001
#define PCM_OP_TCF_AAA_AUTHORIZE                      4002
#define PCM_OP_TCF_AAA_REAUTHORIZE                    4003
#define PCM_OP_TCF_AAA_CANCEL_AUTHORIZATION           4004
#define PCM_OP_TCF_AAA_START_ACCOUNTING               4005
#define PCM_OP_TCF_AAA_UPDATE_ACCOUNTING              4006
#define PCM_OP_TCF_AAA_STOP_ACCOUNTING                4007
#define PCM_OP_TCF_SVC_LISTENER                       4008
#define PCM_OP_TCF_APPLY_PARAMETER                    4009
#define PCM_OP_TCF_PROPAGATE_STATUS                   4010
#define PCM_OP_TCF_POL_APPLY_PARAMETER                4011
#define PCM_OP_TCF_AAA_ACCOUNTING                     4012
#define PCM_OP_TCF_PROV_SERVICE_ORDER_SET_STATE       4013
#define PCM_OP_TCF_PROV_SERVICE_ORDER_SET_ATTR        4014
#define PCM_OP_TCF_PROV_SERVICE_ORDER_NOTIFY          4015
#define PCM_OP_TCF_PROV_CREATE_SVC_ORDER              4016
#define PCM_OP_TCF_PROV_HANDLE_SVC_ORDER              4017
#define PCM_OP_TCF_PROV_UPDATE_SVC_ORDER              4018
#define PCM_OP_TCF_PROV_UPDATE_PROV_OBJECT            4019
#define PCM_OP_TCF_PROV_SET_ATTR                      4020
#define PCM_OP_TCF_PROV_SIMULATE_AGENT                4021
#define PCM_OP_TCF_AAA_ACCOUNTING_ON                  4022
#define PCM_OP_TCF_AAA_ACCOUNTING_OFF                 4023

#define PCM_OP_TCF_AAA_SEARCH_SESSION                 4024
#define PCM_OP_TCF_AAA_PREP_INPUT                     4025
#define PCM_OP_TCF_AAA_UPDATE_AND_REAUTHORIZE         4026

#define PCM_OP_TCF_AAA_ACCOUNTING_PREP_INPUT          4027
#define PCM_OP_TCF_AAA_AUTHORIZE_PREP_INPUT           4028
#define PCM_OP_TCF_AAA_REAUTHORIZE_PREP_INPUT         4029
#define PCM_OP_TCF_AAA_UPDATE_ACCOUNTING_PREP_INPUT   4030
#define PCM_OP_TCF_AAA_STOP_ACCOUNTING_PREP_INPUT     4031

/* Opcodes for Dropped Call.*/
#define PCM_OP_TCF_AAA_DETECT_CONTINUATION_CALL       4032
#define PCM_OP_TCF_AAA_POL_MATCH_CONTINUATION_CALL    4033

/* Policy opcode for PCM_OP_TCF_PROV_HANDLE_SVC_ORDER*/
#define PCM_OP_TCF_POL_PROV_HANDLE_SVC_ORDER          4101

#define PCM_OP_TCF_AAA_REFUND			      4102
#define PCM_OP_TCF_AAA_TOPUP_BALANCE		      4103
#define PCM_OP_TCF_AAA_QUERY_BALANCE		      4104
#define PCM_OP_TCF_AAA_SERVICE_PRICE_ENQUIRY	      4105
/* Opcodes used for Subscriber Life Cycle feature */
#define PCM_OP_TCF_AAA_VALIDATE_LIFECYCLE	      4106
#define PCM_OP_TCF_AAA_POL_VALIDATE_LIFECYCLE	      4107

/* Opcode used for Active Mediation feature */
#define PCM_OP_TCF_AAA_POL_POST_PROCESS               4108

#define PCM_OP_TCF_AAA_GET_SUBSCRIBER_PROFILE         4109
#define PCM_OP_TCF_AAA_POL_GET_SUBSCRIBER_PROFILE     4110


#endif /* _PCM_TCF_OPS_H_ */

