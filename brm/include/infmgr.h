/*
 *	@(#) % %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _INFMGR_H
#define	_INFMGR_H

/*
 * Object types
 */
#define INFMGR_SERVER_TYPE	1
#define INFMGR_NODE_TYPE	2
#define INFMGR_CELL_TYPE	3
#define INFMGR_CLUSTER_TYPE	4

/*
 * Server States
 */
#define INFMGR_SERVER_STATE_NONE	0
#define INFMGR_SERVER_STATE_STARTED	1
#define INFMGR_SERVER_STATE_RUNNING	2
#define INFMGR_SERVER_STATE_STOPPED	3
#define INFMGR_SERVER_STATE_DOWN	4
#define INFMGR_SERVER_STATE_UNKNOWN	5
#define INFMGR_SERVER_STATE_MAINTENANCE	6
	

/*
 * OP FLAGS 
 */
#define	INFMGR_FLAG_UPDATE_DATA		0x00000001
#define	INFMGR_FLAG_IMMEDIATE_SHUTDOWN	0x00000002

#endif /* _INFMGR_H */
