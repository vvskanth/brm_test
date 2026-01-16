/*
 *	@(#) % %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef PIN_POP_H
#define PIN_POP_H

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

/*******************************************************************
 * (Temp) Portal Defines
 *******************************************************************/
#define POP_CONFIG_FILE		"./server.conf"

#define PIN_PHONE_LENGTH	26

#define POP_ERR_NONE		0
#define POP_ERR_BAD_ARG		(POP_ERR_NONE + 1)
#define POP_ERR_MALLOC		(POP_ERR_NONE + 2)
#define POP_ERR_LONG_DIST	(POP_ERR_NONE + 3)
#define POP_ERR_AREA_CODE	(POP_ERR_NONE + 4)
#define POP_ERR_PREFIX		(POP_ERR_NONE + 5)
#define POP_ERR_EXCHANGE	(POP_ERR_NONE + 6)
#define POP_ERR_LOOKUP		(POP_ERR_NONE + 7)
#define POP_ERR_CONFIG		(POP_ERR_NONE + 8)
/*
** POP Flags
*/
#define POP_TOLL_FREE   0x00000001      /* Is pop toll free from ani?   */
#define POP_DIAL_ONE    0x00000002      /* Is a 1 required when dialed? */
#define POP_PRIMARY     0x00000004      /* Primary pop i.e cheapest     */
#define POP_SECONDARY   0x00000008      /* Secondary pop i.e. 2nd cheap */

#endif /* POP_POP_H */
