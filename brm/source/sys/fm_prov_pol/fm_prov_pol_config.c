/*******************************************************************
 *
 * @(#)$Id: fm_prov_pol_custom_config.c /cgbubrm_7.5.0.portalbase/1 2014/12/17 11:11:56 vivilin Exp $
 *
* Copyright (c) 2001, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 *
 *******************************************************************/

#include <stdio.h>	/* for FILE * in pcm.h */
#include "ops/prov.h"
#include "pcm.h"
#include "cm_fm.h"

PIN_EXPORT void * fm_prov_pol_custom_config_func();

struct cm_fm_config fm_prov_pol_custom_config[] = {
	/* opcode as an int,			function name (as a string) */
/*    
	{ PCM_OP_PROV_POL_UPDATE_SVC_ORDER,	"op_prov_pol_update_svc_order" },
*/    
	{ 0,					(char *)0 }
};

void *
fm_prov_pol_custom_config_func()
{
	return ((void *) (fm_prov_pol_custom_config));
}
