/*******************************************************************
 *
* Copyright (c) 2001, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or
 * its licensors and may be used, reproduced, stored or transmitted only
 * in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = 
		"@(#)$Id: fm_content_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2014/12/17 11:11:55 vivilin Exp $";
#endif

#include <stdio.h>	/* for FILE * in pcm.h */
#include "ops/content.h"
#include "pcm.h"
#include "cm_fm.h"

PIN_EXPORT void * fm_content_pol_custom_config_func();

/*******************************************************************
 *******************************************************************/
struct cm_fm_config fm_content_pol_custom_config[] = {
	/* opcode as a u_int, function name (as a string) */
/*    
	{ PCM_OP_CONTENT_POL_ACCOUNTING,	"op_content_pol_accounting" }, 
	{ PCM_OP_CONTENT_POL_AUTHORIZE,		"op_content_pol_authorize" }, 
	{ PCM_OP_CONTENT_POL_RESOLVE_EVENT_EXTENSIONS,
					"op_content_pol_resolve_event_extensions" }, 
	{ PCM_OP_CONTENT_POL_RESOLVE_USER,
					"op_content_pol_resolve_user" },
	{ PCM_OP_CONTENT_POL_POST_AUTHORIZE,	
					"op_content_pol_post_authorize" }, 
	{ PCM_OP_CONTENT_POL_POST_ACCOUNTING,	
					"op_content_pol_post_accounting" }, 
*/                    
	{ 0,	(char *)0 }
};

void *
fm_content_pol_custom_config_func()
{
  return ((void *) (fm_content_pol_custom_config));
}

