/*
 * (#)$Id: fm_device_pol_delete.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 03:59:05 nishahan Exp $ 
 *
* Copyright (c) 2001, 2010, Oracle and/or its affiliates. All rights reserved. 
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "(#)$Id: fm_device_pol_delete.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 03:59:05 nishahan Exp $";
#endif

/*******************************************************************
 * Contains the PCM_OP_DEVICE_POL_DELETE operation.
 *******************************************************************/

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#define FILE_LOGNAME "fm_device_pol_delete.c(2)"

#include "ops/device.h"
#include "ops/bel.h"
#include "ops/apn.h"
#include "ops/ip.h"
#include "ops/num.h"
#include "pin_device.h"
#include "pin_apn.h"
#include "pin_ip.h"
#include "pin_num.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_channel.h"
#include "fm_utils.h"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_device_pol_delete(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_device_pol_delete(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_DEVICE_POL_DELETE operation.
 *******************************************************************/
void
op_device_pol_delete(
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
	if (opcode != PCM_OP_DEVICE_POL_DELETE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_device_pol_delete opcode error", ebufp);

		return;
	}
	
	/***********************************************************
	 * Debut what we got.
         ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_device_pol_delete input flist", i_flistp);

	/***********************************************************
	 * Main rountine for this opcode
	 ***********************************************************/
	fm_device_pol_delete(ctxp, flags, i_flistp, r_flistpp, ebufp);
	
	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(r_flistpp, ebufp);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_device_pol_delete error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_device_pol_delete output flist", *r_flistpp);
	}

	return;
}

/*******************************************************************
 * fm_device_pol_delete:
 *
 *******************************************************************/
static void
fm_device_pol_delete(
	pcm_context_t           *ctxp,
	int32                   flags,
	pin_flist_t             *i_flistp,
	pin_flist_t             **r_flistpp,
	pin_errbuf_t            *ebufp)
{

	pin_flist_t             *t_flistp = NULL;
	pin_flist_t             *s_flistp = NULL;
	poid_t          *dev_pdp = NULL;
	char            *poid_type = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}

	PIN_ERR_CLEAR_ERR(ebufp);
	
	/*
	 * Get the device object poid
	 */
	dev_pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	/*******************************************************************
	* Read out the device object service array
	*******************************************************************/
	t_flistp = PIN_FLIST_CREATE(ebufp);
	PIN_FLIST_FLD_SET(t_flistp, PIN_FLD_POID, dev_pdp, ebufp);
	PIN_FLIST_ELEM_SET(t_flistp, NULL, PIN_FLD_SERVICES, PIN_ELEMID_ANY,
		ebufp);
	PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, t_flistp, &s_flistp, ebufp);

	/*******************************************************************
	 * If there is any element in  the service array, it means some service 
	 * object is associated with the device object. The error buf will be 
	 * set and the transaction will be aborted to prevent the device from 
	 * being mistakenly deleted. 
	 * However, if you still want to delete the device with some service 
	 * associated without aborting, then you can customize the section 
	 * below and flag a warning in the error buf. 
	 *******************************************************************/
	if (s_flistp != NULL && 
		PIN_FLIST_ELEM_COUNT(s_flistp, PIN_FLD_SERVICES, ebufp) ) {
		pin_set_err(ebufp,
			PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION,
			PIN_ERR_BAD_VALUE,
			PIN_FLD_SERVICES, 0, 0);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
			"fm_pol_device_delete error, service array is not null. "
			" Device is assoicated either with service or account. ", ebufp);

		goto Cleanup;
	}

	/*
	 * Get the device poid type
	 * this poid will be used in identifiing different Devices like sim,num..
	 */
	poid_type = (char *) PIN_POID_GET_TYPE(dev_pdp);
	if (strcmp(poid_type, PIN_OBJ_TYPE_DEVICE_APN) == 0) {
		/*
		 * call APN policy to do the work we want
		 */
		PCM_OP(ctxp, PCM_OP_APN_POL_DEVICE_DELETE, flags, i_flistp,
			r_flistpp, ebufp);
	}
	else if (strcmp(poid_type, PIN_OBJ_TYPE_DEVICE_IP) == 0) {
		/*
		 * call IP policy to do the work we want
		 */
		PCM_OP(ctxp, PCM_OP_IP_POL_DEVICE_DELETE, flags, i_flistp,
			r_flistpp, ebufp);
	}
	else if (strcmp(poid_type, PIN_OBJ_TYPE_DEVICE_NUM) == 0) {
		/*
		 * call NUM policy to do the work we want
		 */
		PCM_OP(ctxp, PCM_OP_NUM_POL_DEVICE_DELETE, flags, i_flistp,
			r_flistpp, ebufp);
	}

	else {
		/***********************************************************
		 * Prepare return flist
		 ***********************************************************/
		 *r_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
	}

Cleanup: 
	PIN_FLIST_DESTROY_EX(&t_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&s_flistp, NULL);

	return;
}

