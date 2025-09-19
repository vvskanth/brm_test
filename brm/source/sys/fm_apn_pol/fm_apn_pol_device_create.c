/*
 * (#)$Id: fm_apn_pol_device_create.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 03:59:05 nishahan Exp $
 *
 * Copyright (c) 2004, 2024, Oracle and/or its affiliates.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "(#)$Id: fm_apn_pol_device_create.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 03:59:05 nishahan Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/apn.h"
#include "pin_apn.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "fm_utils.h"

#define FILE_LOGNAME "fm_apn_pol_device_create.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_apn_pol_device_create(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

/**********************************************************************
 * Routines contained within
 **********************************************************************/
static void
fm_apn_pol_device_create(
	pcm_context_t		*ctxp,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistp,
	pin_errbuf_t		*ebufp);

/**********************************************************************
 * Main routine for the PCM_OP_APN_POL_DEVICE_CREATE operation.
 **********************************************************************/
void
op_apn_pol_device_create(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**out_flistpp,
	pin_errbuf_t		*ebufp) {

	pcm_context_t   *ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/*********/
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */

	if (opcode != PCM_OP_APN_POL_DEVICE_CREATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"bad opcode in op_apn_pol_device_create", ebufp);
		return;
	}

	/*
	 * DEBUG: What did we get?
	 */

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_apn_pol_device_create input flist", i_flistp);

	/*
	 * do the real work
	 */
        fm_apn_pol_device_create(ctxp, i_flistp, out_flistpp, ebufp);

	/*
	 * Results.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(out_flistpp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_apn_pol_device_create error", ebufp);
	} 
	else {
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_apn_pol_device_create return flist", *out_flistpp);
	}

	return;
}

/***************************************************************************
 * fm_apn_pol_device_create()
 *
 * This method will check the database to see if there is any apn device
 * that already exists in the db with the given apn/device id. If there is
 * then the ebuf is set.
 *
 * input: i_flistp: the input flist sent in through the opcode call
 *
 * output: ebuf is set if a apn device with the device id already exists
 ****************************************************************************/
static void fm_apn_pol_device_create(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	**out_flistpp,
	pin_errbuf_t	*ebufp) {

	pin_flist_t	*srch_flistp = NULL;    /* Search flist         */
	pin_flist_t	*sr_flistp = NULL;      /* Return flistp search */
	pin_flist_t	*sub_flistp = NULL;     /* Flist for substruct  */
	poid_t		*pdp = NULL;            /* Routing poid         */
	poid_t		*dev_pdp = NULL;        /* Type only poid       */
	poid_t		*srch_poidp = NULL;     /* Search poid          */
	char		*dev_id = NULL;		/* APN_device_ID	*/
	int32		flag = SRCH_DISTINCT;   /* Search flags         */
	int64		dbno = 0;               /* Database no          */
	int32		cred = 0;               /* Scoping credentials  */
	size_t		dev_orig_len = 0;      /* Length of the Device id*/
	int32		dev_space_len = 0;      /* Length of start spaces*/
	size_t		dev_final_len = 0;    /* Final length after trimming space and tab*/

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	srch_flistp = PIN_FLIST_CREATE(ebufp);

	pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	dbno = PIN_POID_GET_DB(pdp);

	/*
	 * Copy the Input Flist into output Flist
	 */
	*out_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);

	/*
	 * Get the Device id from the input Flist
	 */
	dev_id = (char * )PIN_FLIST_FLD_TAKE(*out_flistpp, PIN_FLD_DEVICE_ID,
						0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_device_create Invalid APN Device",
			ebufp);
		goto Cleanup;
	}

	/*
	 * Remove the Left spaces
	 */
	dev_orig_len = strlen(dev_id);

	dev_space_len = strspn(dev_id, " \t");
	
	dev_orig_len = dev_orig_len - dev_space_len;
	dev_final_len = dev_orig_len;

	if( dev_final_len > 0 ) {
		pin_memmove(dev_id, dev_orig_len, dev_id + dev_space_len, dev_orig_len);

		/*
		 * Remove the Right SPace
		 */
		while (strchr (" \t\0",dev_id[dev_final_len])) {
			dev_final_len--;
		}
		dev_id[dev_final_len + 1 ] = '\0';
	}

	PIN_FLIST_FLD_SET(*out_flistpp, PIN_FLD_DEVICE_ID, dev_id, ebufp);

	if (dev_final_len <= 0 ) {
                pin_set_err(ebufp, PIN_ERRLOC_FM,
                                PIN_ERRCLASS_APPLICATION,
                                PIN_ERR_BAD_VALUE, 
				PIN_FLD_DEVICE_ID, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_apn_pol_device_create Invalid APN Device",
			ebufp);
		goto Cleanup;
	}

	srch_poidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);
	
	/*
	 * create  a type only poid to narrow the search
	*/

	dev_pdp = PIN_POID_CREATE(dbno, PIN_OBJ_TYPE_DEVICE_APN, -1, ebufp);

	PIN_FLIST_FLD_PUT(srch_flistp, PIN_FLD_POID, srch_poidp, ebufp);
	PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);

	PIN_FLIST_FLD_SET(srch_flistp, PIN_FLD_TEMPLATE,
		(void *)"select X from /device/apn where  ( F1 = V1 and  "
		"F2 = V2 ) ", ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_ARGS, 1, ebufp);
	PIN_FLIST_FLD_PUT(sub_flistp, PIN_FLD_DEVICE_ID, dev_id, ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_ARGS, 2, ebufp);

	PIN_FLIST_FLD_PUT(sub_flistp, PIN_FLD_POID, dev_pdp, ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD(srch_flistp, PIN_FLD_RESULTS, 0, ebufp);

	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_POID, 0, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"fm_apn_pol_dev srch input flist", srch_flistp);

	PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, 0, srch_flistp, &sr_flistp, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_apn_pol_dev srch output flist", sr_flistp);

	if((PIN_FLIST_FLD_GET(sr_flistp, PIN_FLD_RESULTS,
						1, ebufp)) != NULL)
	{
		/* there are duplicates */
		pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_DUPLICATE, PIN_FLD_DEVICE_ID, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"duplicate device id in apn_pol_device_create", ebufp);
	}
Cleanup:

	PIN_FREE_EX(&dev_id);
	PIN_FLIST_DESTROY_EX(&srch_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);
	return;
}
