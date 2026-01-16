/*
 *	@(#) % %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _QM_DEBUG_H
#define _QM_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * defines for the QM debug variables
 */
/*
 * these are global variable, one from the frontend, one for the backend.
 * they are set via pin.conf.
 * They should be considered READ-ONLY.
 */
extern int32	qm_debug_front;
extern int32	qm_debug_back;

/*
 * qm_debug is the top level, used for flow tracing of operations
 */
/* 1st nibble, gross transaction trace */
#define QM_DEBUG_OP_ERROR	0x00000001	/* print errors in detail */
#define QM_DEBUG_MAIN		0x00000002	/* main loop */
#define QM_DEBUG_TRANS_TRACE	0x00000004	/* trace transaction stuff */


/*
 * qm_debug_front is for frontend stuff
 *	low byte is reserved for generic stuff, upper 3 for app
 */
#define	DEBUG_FE_NEW_CONNECTION		0x0001
#define	DEBUG_FE_INPUT			0x0002
#define	DEBUG_FE_OP_DONE		0x0004
#define	DEBUG_FE_OUTPUT			0x0008

/*
 * dm_debug_back is for backend stuff
 * the values are all application specific
 */

#ifdef __cplusplus
}
#endif

#endif /*_QM_DEBUG_H*/
