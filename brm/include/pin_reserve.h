/* file information --- @(#)%Portal Version: pin_reserve.h:BillingVelocityInt:1:2006-Mar-03 21:23:14 % */
/*
 * Copyright (c) 2004, 2023, Oracle and/or its affiliates.
 *      This material is the confidential property of Oracle Corporation
 *      or its subsidiaries or licensors and may be used, reproduced, stored
 *      or transmitted only in accordance with a valid Oracle license or
 *      sublicense agreement.
 */

#ifndef _PIN_RESERVE_H
#define	_PIN_RESERVE_H


/* reservation object status */	
#define PIN_RESERVATION_RESERVED		0
#define PIN_RESERVATION_RELEASED		1
#define PIN_RESERVATION_PUTBACKED		2
#define PIN_RESERVATION_STATUS_ANY		-1

/* reservation return action 
 * PCM_OP_ACT_MULTI_AUTH uses the return codes from 101 - 120
 * Reservation opcodes should not use codes between 101-120 
 * to avoid clashes.
 */   
#define PIN_RESERVATION_FAIL			0
#define PIN_RESERVATION_SUCCESS			1
#define PIN_RESERVATION_DUPLICATE		2
#define PIN_RESERVATION_INSUFFICIENT_FUNDS	3
#define PIN_RESERVATION_NO_FUNDS		4
#define PIN_RESERVATION_INSUFFICIENT_RATED_QTY	5
#define PIN_RESERVATION_INVALID_REQUESTED_QTY	6

/* reservation extension */
#define PIN_RESERVE_AGGREGATED_AMOUNT		0
#define PIN_RESERVE_INCREMENTAL_AMOUNT		1

/* ignore previous reservation */
#define PIN_RESERVATION_FLG_IGNORE_PREVIOUS_RESERVATION 0x1

#define PIN_OBJ_TYPE_RESERVATION                "/reservation"
#define PIN_OBJ_TYPE_RESERVATION_ACTIVE         "/reservation/active"
#endif	/*_PIN_RESERVE_H*/
