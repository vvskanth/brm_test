/*
 *	@(#) % %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PIN_TERMCAUSE_H_
#define _PIN_TERMCAUSE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_CREDIT_MAX		2147482647

/*****************************************************************
 * A list of call terminate cause code 
 *****************************************************************/
typedef enum pin_termcause {
	PIN_TERM_NORMAL	= 0, /* Normal termination */
	PIN_TERM_ABNORMAL = 1, /* Termination due to connection error */
	PIN_TERM_NOCONNECTION = 2,	 /* Never got connected */
	PIN_TERM_UNKNOWN = 3	/* Unknown termination cause */
} pin_termcause_t;

/*****************************************************************
 * Status of credit based on call terminate cause in the 
 * configuration, indicating whether the credit set for 
 * this terminate cause will take effect.
 *****************************************************************/
typedef enum pin_credit_termcause_status {
	PIN_CREDIT_TERMCAUSE_STATUS_NOT_SET	= 0, 
	PIN_CREDIT_TERMCAUSE_STATUS_ACTIVE		= 1,
	PIN_CREDIT_TERMCAUSE_STATUS_INACTIVE	= 2,
	PIN_CREDIT_TERMCAUSE_STATUS_CANCELLED	= 3
} pin_credit_termcause_status_t;

#ifdef __cplusplus
}
#endif

#endif /*_PIN_TERMCAUSE_H_*/
