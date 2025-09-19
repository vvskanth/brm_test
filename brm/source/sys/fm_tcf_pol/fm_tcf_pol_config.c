/*
 * @(#)%Portal Version: fm_tcf_pol_custom_config.c:PortalBase7.3.1Int:1:2007-Nov-24 22:33:07 %
 *
 * Copyright (c) 2007 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_tcf_pol_custom_config.c:PortalBase7.3.1Int:1:2007-Nov-24 22:33:07 %";
#endif

#include <pcm.h>
#include "ops/tcf.h"
#include "cm_fm.h"


/*******************************************************************
 *******************************************************************/

/*
 * NOTE THAT THE DISPATCH ENTRIES ARE COMMENTED. WHEN YOU OVERRIDE
 * AN IMPLEMENTATION, UNCOMMENT THE LINE BELOW THAT MATCHES THE
 * OPCODE FOR WHICH YOU HAVE PROVIDED AN ALTERNATE IMPLEMENTATION.
 */

struct cm_fm_config fm_tcf_pol_custom_config[] = {
	/* opcode as a u_int, function name (as a string) */
/*	{ PCM_OP_TCF_POL_APPLY_PARAMETER, \
"op_tcf_pol_apply_parameter" }, */
/*	{ PCM_OP_TCF_POL_PROV_HANDLE_SVC_ORDER, \
"op_tcf_pol_prov_handle_svc_order"}, */
	{ 0,	(char *)0 }
};

PIN_EXPORT
void *
fm_tcf_pol_custom_config_func()
{
  return ((void *) (fm_tcf_pol_custom_config));
}
