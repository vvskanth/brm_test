/*
 * @(#)$Id: fm_num_pol_custom_config.c /cgbubrm_7.3.2.portalbase/2 2010/05/11 22:15:59 mvenkate Exp $
 *
* Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved. 
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_num_pol_custom_config.c /cgbubrm_7.3.2.portalbase/2 2010/05/11 22:15:59 mvenkate Exp $";
#endif

#include <pcm.h>
#include "ops/num.h"
#include "cm_fm.h"


/*******************************************************************
 *******************************************************************/

/*
 * NOTE THAT THE DISPATCH ENTRIES ARE COMMENTED. WHEN YOU OVERRIDE
 * AN IMPLEMENTATION, UNCOMMENT THE LINE BELOW THAT MATCHES THE
 * OPCODE FOR WHICH YOU HAVE PROVIDED AN ALTERNATE IMPLEMENTATION.
 */

struct cm_fm_config fm_num_pol_custom_config[] = {
	/* opcode as a u_int, function name (as a string) */
/*        { PCM_OP_NUM_POL_CANONICALIZE, "op_num_pol_canonicalize", CM_FM_OP_OVERRIDABLE },
        { PCM_OP_NUM_POL_DEVICE_ASSOCIATE, "op_num_pol_device_associate", CM_FM_OP_OVERRIDABLE },
        { PCM_OP_NUM_POL_DEVICE_CREATE, "op_num_pol_device_create", CM_FM_OP_OVERRIDABLE },
        { PCM_OP_NUM_POL_DEVICE_SET_ATTR, "op_num_pol_device_set_attr", CM_FM_OP_OVERRIDABLE },
        { PCM_OP_NUM_POL_DEVICE_SET_BRAND, "op_num_pol_device_set_brand", CM_FM_OP_OVERRIDABLE },
        { PCM_OP_NUM_POL_DEVICE_DELETE, "op_num_pol_device_delete", CM_FM_OP_OVERRIDABLE },*/

	{ 0,	(char *)0 }
};

PIN_EXPORT
void *
fm_num_pol_custom_config_func()
{
  return ((void *) (fm_num_pol_custom_config));
}
