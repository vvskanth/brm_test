/* Copyright (c) 2022, 2022 Oracle and/or its affiliates.*/
/*
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_NOTIFICATION_OPS_H_
#define _PCM_NOTIFICATION_OPS_H_

/*
 * This file contains the opcode definitions for NOTIFICATION 
 */

/* =====================================================================
 *   NAME            : PCM_NOTIFICATION_OPS
 *   TOTAL RANGE     : 5921..5940
 *   USED RANGE      : 5921..5933
 *   RESERVED RANGE  :
 *   ASSOCIATED FMs  : fm_notification
 *
 *   NAME            : PCM_NOTIFICATION_POL_OPS
 *   RESERVED RANGE  :
 *   ASSOCIATED FMs  : fm_notification_pol
 * =====================================================================
 */

#include "ops/base.h"

/* Opcodes for notification  */
#define PCM_OP_NOTIFICATION_CREATE_SPECIFICATION         5921
#define PCM_OP_NOTIFICATION_MODIFY_SPECIFICATION         5922
#define PCM_OP_NOTIFICATION_GET_SPECIFICATION            5923
#define PCM_OP_NOTIFICATION_POL_PREP_SPECIFICATION       5924
#define PCM_OP_NOTIFICATION_POL_VALID_SPECIFICATION      5925
#define PCM_OP_NOTIFICATION_POL_MODIFY_SPECIFICATION     5926
#define PCM_OP_NOTIFICATION_POL_GET_SPECIFICATION        5927
#define PCM_OP_NOTIFICATION_DELETE_SPECIFICATION	 5928
#define PCM_OP_NOTIFICATION_POL_DELETE_SPECIFICATION     5929
#define PCM_OP_NOTIFICATION_VERIFY_PUBLISH_REQUIRED	 5930
#define PCM_OP_NOTIFICATION_CALC_DELIVERY_TIME		 5931
#define PCM_OP_NOTIFICATION_PROCESS_NOTIFICATION	 5932
#define PCM_OP_NOTIFICATION_GET_LASTNOTIFY_TSTAMP        5933
#endif /* _PCM_NOTIFICATION_OPS_H_ */

