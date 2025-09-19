/**********************************************************************
*
*	C Source:		fm_zonemap_pol_config.c
*	Instance:		1
*	Description:	
*	%created_by:	lalita %
*	%date_created:	Thu Apr 05 16:00:30 2001 %
*
* Copyright (c) 1999, 2014, Oracle and/or its affiliates. All rights reserved.
*
*	This material is the confidential property of Oracle Corporation
*	or its subsidiaries or licensors and may be used, reproduced, stored
*	or transmitted only in accordance with a valid Oracle license or
*	sublicense agreement.
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#)$Id: fm_zonemap_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 03:57:42 nishahan Exp $";
#endif

#include <stdio.h>			/* for FILE * in pcm.h */
#include "ops/zonemap.h"
#include "pcm.h"
#include "cm_fm.h"

PIN_EXPORT void * fm_zonemap_pol_custom_config_func();

struct cm_fm_config fm_zonemap_pol_custom_config[] = {
	/* opcode as a u_int, function name (as a string) */
/*    
	{ PCM_OP_ZONEMAP_POL_GET_ZONEMAP,     "op_zonemap_pol_get_zonemap" },
	{ PCM_OP_ZONEMAP_POL_SET_ZONEMAP,     "op_zonemap_pol_set_zonemap" },
	{ PCM_OP_ZONEMAP_POL_GET_LINEAGE,     "op_zonemap_pol_get_lineage" },
*/    
	{ 0,	(char *)0 }
};

void *
fm_zonemap_pol_custom_config_func()
{
	return ((void *) (fm_zonemap_pol_custom_config));
}

