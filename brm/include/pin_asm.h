/*
 * (#)% %
 *
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PIN_ASM_H
#define _PIN_ASM_H

#ifdef __cplusplus
extern "C" {
#endif


#define PIN_OBJ_TYPE_ACTIVE_SESSION "/active_session"

#define PIN_ASM_INDEX_NAME_ACTIVE_ID "active_session_active_id_i"


/* Service Order Status  */
typedef enum  pin_aso_status {

        PIN_ASO_STATUS_AVAILABLE                 = 1,
        PIN_ASO_STATUS_CREATED                   = 2,
        PIN_ASO_STATUS_STARTED                   = 3,
        PIN_ASO_STATUS_UPDATED                   = 4,
        PIN_ASO_STATUS_CLOSED                    = 5,
	PIN_ASO_STATUS_CANCELLED                 = 6,
	PIN_ASO_STATUS_RATED                     = 7

} pin_aso_status_t;

#ifdef __cplusplus
}
#endif

#endif  /*_PIN_ASM_H*/
