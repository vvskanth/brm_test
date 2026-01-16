/*
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef _PIN_NUM_H
#define _PIN_NUM_H

/*
 * Object type strings
 */
#define PIN_OBJ_TYPE_DEVICE_NUM		"/device/num"
#define PIN_OBJ_TYPE_BLOCK		"/block"

/*
 * States for /device/num
 */
typedef enum pin_num_state_type {
	PIN_NUM_STATE_RAW		= 0,
	PIN_NUM_STATE_NEW		= 1,
	PIN_NUM_STATE_ASSIGNED		= 2,
	PIN_NUM_STATE_QUARANTINED	= 3,
	PIN_NUM_STATE_UNASSIGNED	= 4,
        PIN_NUM_STATE_PORT_IN_NEW       = 5,        
	PIN_NUM_STATE_TEMPORARY_ASSIGN  = 6,        
	PIN_NUM_STATE_QUARANTINED_REPATRIATED   = 7,        
	PIN_NUM_STATE_REPATRIATED       = 8,        
	PIN_NUM_STATE_QUARANTINED_PORT_OUT      = 9,        
	PIN_NUM_STATE_PORT_OUT  = 10
} pin_num_state_type_t;

/*
 * Default values
 */
#define PIN_NUM_STATE_DEFAULT		1
#define PIN_NUM_VANITY_DEFAULT		0
#define PIN_NUM_CANON_MIN_LENGTH	10
#define PIN_NUM_CANON_MAX_LENGTH	20	
#define PIN_NUM_QUARANTINE_DEFAULT 	90*24*60*60	/* seconds */
#define PIN_NUM_QUARANTINE_NAME		"quarantine_period"	
#define PIN_NUM_QUARANTINE_ACTION_NAME	"device_num_unquarantine"

/*
 * Parsing support for canonicalization
 */
#define OPEN_PARAN '('
#define CLOSE_PARAN ')'
#define ENDOFLINE   '\0'
#define DASH	   '-'
#define DOT	   '.'
#define COMMA	   ','

#define ADDSYMBOL	0
#define SKIPSYMBOL	1
#define ERRSYMBOL	3
#define EOL 		4

#endif  /* _PIN_NUM_H */
