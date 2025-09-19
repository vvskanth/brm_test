/*
 * @(#)% %
 *
 * Copyright (c) 2002 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)% %";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/voucher.h"
#include "pin_voucher.h"
#include "cm_fm.h"
#include "pin_errs.h"

#define FILE_LOGNAME "fm_voucher_pol_order_delete.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_voucher_pol_order_delete(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_voucher_pol_order_delete(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);


/*******************************************************************************
 * Entry routine for the PCM_OP_VOUCHER_POL_ORDER_DELETE opcode
 ******************************************************************************/
void
op_voucher_pol_order_delete(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_VOUCHER_POL_ORDER_DELETE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_delete opcode error", ebufp);

		*o_flistpp = NULL;
		return;
	}

	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_voucher_pol_order_delete input flist", i_flistp);

	/*
	 * Prepare the return flist - copy the input to the output 
	 */
	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);


	/*
	 * Main rountine for this opcode
	 */
	fm_voucher_pol_order_delete(ctxp, i_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_delete error", ebufp);
		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_voucher_pol_order_delete return flist", *o_flistpp);
	}

	return;
}


/*******************************************************************************
 * fm_voucher_pol_order_delete()
 *
 * Inputs: flist with /order/voucher poid 
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function validates the input flist has a poid of type '/order/voucher',
 * then validates the order states 
 ******************************************************************************/
static void
fm_voucher_pol_order_delete(
	pcm_context_t           *ctxp,
	pin_flist_t             *i_flistp,
	pin_errbuf_t            *ebufp)
{
	void			*vp = NULL;			/* flist gets */
	pin_flist_t             *flistp = NULL;			/* read flist */
	pin_flist_t             *r_flistp = NULL;		/* result flist */
	char			*poid_type = NULL;		/* poid type */
	int32			status = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	flistp = PIN_FLIST_CREATE(ebufp);

	/*
	 * Validate the object is of type /order/voucher
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	poid_type = (char *)PIN_POID_GET_TYPE((poid_t *)vp);

	if(vp != NULL){
		poid_type = (char *)PIN_POID_GET_TYPE((poid_t *)vp);
		if ((strncmp(poid_type, PIN_OBJ_TYPE_ORDER_VOUCHER
			, 14) != 0 ) ||
		    (strcmp(poid_type, PIN_OBJ_TYPE_ORDER_VOUCHER)) != 0) {

			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
			       "fm_voucher_pol_order_delete invalid poid type");

			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_APPLICATION,
               		        PIN_ERR_BAD_POID_TYPE, PIN_FLD_POID, 0, 0);

		}
	}

	/*
	 * Get the values of the order state id from the database
	 */
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, vp, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_STATUS, NULL, ebufp);


        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
               "fm_voucher_pol_order_delete rd flds input flist", flistp);
        PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, flistp, &r_flistp,
		ebufp);
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        	"fm_voucher_pol_order_delete rd flds return flist", r_flistp);

	/*
	 * Deleting the voucher order is not allowed 
	 * if the status of:
	 *
	 *   PIN_VOUCHER_ORDER_REQUEST
	 *   PIN_VOUCHER_ORDER_PARTIAL_RECEIVE
	 *
	 * States outside of this list will generate an error.
	 */
	vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATUS, 0, ebufp);
	if(vp != NULL){
		status = *(int32 *)vp;

		if(status == PIN_VOUCHER_ORDER_REQUEST || 
		   status == PIN_VOUCHER_ORDER_PARTIAL_RECEIVE )
		{
			/*
			 * validation fails 
			 */
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_PROHIBITED_ARG, PIN_FLD_POID,
				 0, 0);
			PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR, 
				"cannot delete an order if the "
				"order is in request or partial receive state ",
				 ebufp);
		}
	}
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	return;
}
