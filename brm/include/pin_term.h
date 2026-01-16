/* continuus file information --- %full_filespec: pin_term.h~4:incl:3 % */
/*******************************************************************
 *
 *  @(#) %full_filespec: pin_term.h~4:incl:3 %
 *
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef _PIN_TERM_H
#define	_PIN_TERM_H

/*   
 * defines for PIN_FLD_ACTION for opcode
 * PCM_OP_TERM_DIALUP_ACCOUNTING_{ON,OFF}
 */

#define PIN_TERM_ACCOUNTING_CHARGE "C"
#define PIN_TERM_ACCOUNTING_NO_CHARGE "NC"

/*
 * PCM_OP_TERM_IP_DIALUP_LOGOUT fields.
 */
/*
 * PIN_FLD_STATUS - The status of this logout re the PIN system,
 * *not* the NAS status.
 */
typedef enum pin_term_status {
		/* not set yet (implies session is open) */
	PIN_TERM_STATUS_NOT_SET =	0,
		/* OK - login/logout is OK */
	PIN_TERM_STATUS_OK =		1,
		/* LOGIN found an orphan session */
	PIN_TERM_STATUS_ORPHAN =	2,
		/* the NAS was found to have been reset */
	PIN_TERM_STATUS_NAS_RESET =	3,
		/* the session was forced close */
	PIN_TERM_STATUS_FORCED =	4
} pin_term_status_t;

/*
 * PIN_FLD_TERMINATE_CAUSE - The reason for the termination of this
 * session.
 */
typedef enum pin_term_terminate_cause {
	/* Default - specific cause not recorded */ 
	PIN_TERM_TERMINATE_DEFAULT = 0,
	/* User Request */
	PIN_TERM_TERMINATE_USER_REQUEST = 1,
	/* Lost Carrier */
	PIN_TERM_TERMINATE_LOST_CARRIER = 2,
	/* Lost Service */
	PIN_TERM_TERMINATE_LOST_SERVICE = 3,
	/* Idle Timeout */
	PIN_TERM_TERMINATE_IDLE_TIMEOUT = 4,
	/* Session Timeout */
	PIN_TERM_TERMINATE_SERVICE_TIMEOUT = 5,
	/* Admin Reset */
	PIN_TERM_TERMINATE_ADMIN_RESET = 6,
	/* Admin Reboot */
	PIN_TERM_TERMINATE_ADMIN_REBOOT = 7,
	/* Port Error */
	PIN_TERM_TERMINATE_PORT_ERROR = 8,
	/* NAS Error */
	PIN_TERM_TERMINATE_NAS_ERROR = 9,
	/* NAS Request */
	PIN_TERM_TERMINATE_NAS_REQUEST = 10,
	/* NAS Reboot */
	PIN_TERM_TERMINATE_NAS_REBOOT = 11,
	/* Port Unneeded */
	PIN_TERM_TERMINATE_PORT_UNNEEDED = 12,
	/* Port Preempted */
	PIN_TERM_TERMINATE_PORT_PREEMPTED = 13,
	/* Port Suspended */
	PIN_TERM_TERMINATE_PORT_SUSPENDED = 14,
	/* Service Unavaliable */
	PIN_TERM_TERMINATE_SERVICE_UNAVAILABLE = 15,
	/* callback */
	PIN_TERM_TERMINATE_CALLBACK = 16,
	/* User Error */
	PIN_TERM_TERMINATE_USER_ERROR = 17,
	/* Host Request */
	PIN_TERM_TERMINATE_HOST_REQUEST = 18
} pin_term_terminate_cause_t;


#endif	/*_PIN_TERM_H*/
