/*
 * @(#)%Portal Version: fm_device_pol_set_state.c:ServerIDCVelocityInt:4:2006-Sep-14 11:35:18 %
 *
 * Copyright (c) 2001 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_device_pol_set_state.c:ServerIDCVelocityInt:4:2006-Sep-14 11:35:18 %";
#endif

/*******************************************************************
 * Contains the PCM_OP_DEVICE_POL_SET_STATE operation.
 *******************************************************************/

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#define FILE_LOGNAME "fm_device_pol_set_state.c(1)"

#include "ops/device.h"
#include "pin_device.h"
#include "pin_apn.h"
#include "pin_ip.h"
#include "ops/bel.h"
#include "ops/apn.h"
#include "ops/ip.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_channel.h"
#include "fm_utils.h"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_device_pol_set_state(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_device_pol_set_state(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_DEVICE_POL_SET_STATE operation.
 *******************************************************************/
void
op_device_pol_set_state(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t	*r_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Null out results
	 ***********************************************************/
	*r_flistpp = NULL;

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_DEVICE_POL_SET_STATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_device_pol_set_state opcode error", ebufp);

		return;
	}
	
	/***********************************************************
	 * Debut what we got.
         ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_device_pol_set_state input flist", i_flistp);

	/***********************************************************
	 * Main rountine for this opcode
	 ***********************************************************/
	fm_device_pol_set_state(ctxp, flags, i_flistp, r_flistpp, ebufp);
	
	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(r_flistpp, ebufp);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_device_pol_set_state error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_device_pol_set_state output flist", *r_flistpp);
	}

	return;
}

/*******************************************************************
 * fm_device_pol_set_state:
 *
 *******************************************************************/
static void
fm_device_pol_set_state(
	pcm_context_t           *ctxp,
	int32                   flags,
	pin_flist_t             *i_flistp,
	pin_flist_t             **r_flistpp,
	pin_errbuf_t            *ebufp)
{

	poid_t          *dev_poidp = NULL;
	char            *poid_type = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}

	PIN_ERR_CLEAR_ERR(ebufp);
	
	/*
	 * Get the old device object poid
	 */
	dev_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	
	/*
	 * Get the device poid type
	 * this poid will be used in identifiing different Devices like sim,num..
	 */
	poid_type = (char *) PIN_POID_GET_TYPE(dev_poidp);

	if (strcmp(poid_type, PIN_OBJ_TYPE_DEVICE_IP) == 0) {
		/*
		 * call IP policy to do the work we want
		 */
		 PCM_OP(ctxp, PCM_OP_IP_POL_DEVICE_SET_STATE, flags, i_flistp,
			r_flistpp, ebufp);
	} else if (strcmp(poid_type, PIN_OBJ_TYPE_DEVICE_APN) == 0) {
		/*
		 * call APN policy to do the work we want
		 */
		 PCM_OP(ctxp, PCM_OP_APN_POL_DEVICE_SET_STATE, flags, i_flistp,
			r_flistpp, ebufp);
	}
	else {
		/***********************************************************
		 * Prepare return flist
		 ***********************************************************/
		*r_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
	}

	return;
}

