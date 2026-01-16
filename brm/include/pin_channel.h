
/*
 *  @(#) %full_filespec: pin_channel.h~4:incl:3 %
 *
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PIN_CHANNEL_H
#define _PIN_CHANNEL_H

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_CHANNEL_ACCOUNT_CREATE	100
#define PIN_CHANNEL_ACCOUNT_MODIFY	101
#define PIN_CHANNEL_SERVICE_CREATE	102
#define PIN_CHANNEL_SERVICE_MODIFY	103

/*
 * PIN_FLD_STATUS - The status for /channel_event objects.
 */
typedef enum pin_channel_event_status {
	/* object created but not pushed */
	PIN_STATUS_NOT_PUSHED = 0,
	/*intermediate state not used currently  */
	PIN_STATUS_PUSH_IN_PROGRESS =   1,
	/* object has been successfully pushed */
	PIN_STATUS_PUSHED = 2,
	/* error in pushing data */
	PIN_STATUS_ERROR =  3,
	/* link error in pushing data */
	PIN_STATUS_LINK_ERROR = 4
} pin_channel_event_status_t;

#ifdef __cplusplus
}
#endif

#endif /* _PIN_CHANNEL_H */
