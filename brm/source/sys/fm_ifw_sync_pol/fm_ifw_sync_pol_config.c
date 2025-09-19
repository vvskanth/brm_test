/*******************************************************************
 *
 *
* Copyright (c) 2002, 2014, Oracle and/or its affiliates. All rights reserved.
 * or its licensors and may be used, reproduced, stored or transmitted only in accordance with a valid Oracle
 * license or sublicense  agreement.
 *
 *
 *******************************************************************/

#ifndef lint
static  char Sccs_Id[] = "@(#)$Id: fm_ifw_sync_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2014/12/17 11:11:55 vivilin Exp $";
#endif

#include <stdio.h>			/* for FILE * in pcm.h */
#include "pcm.h"
#include "cm_fm.h"
#include "ops/ifw_sync.h"

PIN_EXPORT void * fm_ifw_sync_pol_custom_config_func();

/*******************************************************************
 *******************************************************************/

struct cm_fm_config fm_ifw_sync_pol_custom_config[] = {
	/* opcode as a u_int, function name (as a string) */
/*    
	{ PCM_OP_IFW_SYNC_POL_PUBLISH_EVENT,	"op_ifw_sync_pol_publish_event" },
*/    
	{ 0,	(char *)0 }
};

void *
fm_ifw_sync_pol_custom_config_func()
{
  return ((void *) (fm_ifw_sync_pol_custom_config));
}

