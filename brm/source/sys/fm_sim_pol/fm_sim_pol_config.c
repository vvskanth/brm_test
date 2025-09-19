/*
 * @(#)$Id: fm_sim_pol_custom_config.c /cgbubrm_7.5.0.portalbase/2 2014/12/17 11:11:56 vivilin Exp $
 *
* Copyright (c) 2013, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_sim_pol_custom_config.c /cgbubrm_7.5.0.portalbase/2 2014/12/17 11:11:56 vivilin Exp $";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>
#include "cm_fm.h"
#include "ops/sim.h"


/*
 * NOTE THAT THE DISPATCH ENTRIES ARE COMMENTED. WHEN YOU OVERRIDE
 * AN IMPLEMENTATION, UNCOMMENT THE LINE BELOW THAT MATCHES THE
 * OPCODE FOR WHICH YOU HAVE PROVIDED AN ALTERNATE IMPLEMENTATION.
 */


/****************************************************************************
 ****************************************************************************/
struct cm_fm_config fm_sim_pol_custom_config[] = {
	/* opcode (as an int), function name (as a string) */
      /*
    {PCM_OP_SIM_POL_DEVICE_ASSOCIATE,	"op_sim_pol_device_associate" },
	{ PCM_OP_SIM_POL_DEVICE_CREATE,		"op_sim_pol_device_create"  },
	{ PCM_OP_SIM_POL_DEVICE_SET_ATTR,	"op_sim_pol_device_set_attr" },
	{ PCM_OP_SIM_POL_DEVICE_SET_BRAND,	"op_sim_pol_device_set_brand" },
	{ PCM_OP_SIM_POL_ORDER_CREATE, "op_sim_pol_order_create"},*/
	{ 0,	(char *)0 }
};

PIN_EXPORT
void *
fm_sim_pol_custom_config_func() {
	return ((void *) (fm_sim_pol_custom_config));
}
