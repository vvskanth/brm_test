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

#define FILE_LOGNAME "fm_voucher_pol_order_associate.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_voucher_pol_order_associate(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_voucher_pol_order_associate(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);

static void
fm_voucher_pol_order_associate_validate(
	pcm_context_t		*ctxp,
	poid_t			*i_poidp,
	pin_errbuf_t		*ebufp);


/*******************************************************************************
 * Entry routine for the PCM_OP_VOUCHER_POL_ORDER_ASSOCIATE opcode
 ******************************************************************************/
void
op_voucher_pol_order_associate(
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
	if (opcode != PCM_OP_VOUCHER_POL_ORDER_ASSOCIATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_associate opcode error", ebufp);

		*o_flistpp = NULL;
		return;
	}

	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_voucher_pol_order_associate input flist", i_flistp);

	/*
	 * Prepare the return flist - copy the input to the output 
	 */
	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);


	/*
	 * Main rountine for this opcode
	 */
	fm_voucher_pol_order_associate(ctxp, i_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_voucher_pol_order_associate error", ebufp);
		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_voucher_pol_order_associate return flist", *o_flistpp);
	}

	return;
}


/*******************************************************************************
 * fm_voucher_pol_order_associate()
 *
 * Inputs: flist with /order/voucher poid 
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function will call the validate function to validate the  
 * main and sub orders poid 
 ******************************************************************************/
static void
fm_voucher_pol_order_associate(
	pcm_context_t           *ctxp,
	pin_flist_t             *i_flistp,
	pin_errbuf_t            *ebufp)
{
	pin_flist_t             *sub_flistp = NULL;		/* orders flist */
	poid_t			*s_poidp = NULL;		/* sub order poid */
	poid_t			*m_poidp = NULL;		/* main order poid */
	int32			eid  =  0;              	/* array element id */
	pin_cookie_t		cookie = NULL;          	/* to iterate through array */


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Validate the main order poid
	 */
	 m_poidp = PIN_FLIST_FLD_GET (i_flistp, PIN_FLD_POID, 0, ebufp);

	fm_voucher_pol_order_associate_validate(ctxp, m_poidp, ebufp);

	/*
	 * Validate sub orders
	 */
	while ((sub_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
                PIN_FLD_ORDERS_DATA, &eid, 1, &cookie, ebufp))
                != (pin_flist_t *)NULL) {
	
		s_poidp = PIN_FLIST_FLD_GET(sub_flistp, PIN_FLD_ORDER_OBJ, 0, 
			ebufp);
		
		fm_voucher_pol_order_associate_validate(ctxp, s_poidp, 
			ebufp);
	}
	
	return;
}

/*******************************************************************************
 * fm_voucher_pol_order_associate_validate()
 *
 * Inputs: flist with /order/voucher poid 
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function validates the input flist has a poid of type '/order/voucher',
 * then validates the order states 
 ******************************************************************************/
static void
fm_voucher_pol_order_associate_validate(
	pcm_context_t           *ctxp,
	poid_t			*i_poidp,
	pin_errbuf_t            *ebufp)
{
	void			*vp = NULL;			/* flist gets */
	pin_flist_t             *flistp = NULL;			/* read flist */
	pin_flist_t             *r_flistp = NULL;		/* result flist */
	char			*poid_type = NULL;		/* poid type */
	int32			status = 0;			/* order state */

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Validate the object is of type /order/voucher
	 */

	if(i_poidp != NULL){
		poid_type = (char *)PIN_POID_GET_TYPE(i_poidp);
		if ((strncmp(poid_type, PIN_OBJ_TYPE_ORDER_VOUCHER
			,14) != 0 ) ||
		    (strcmp(poid_type, PIN_OBJ_TYPE_ORDER_VOUCHER)) != 0) {
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
		       "fm_voucher_pol_order_associate_validate invalid poid type");

			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_APPLICATION,
               		        PIN_ERR_BAD_POID_TYPE, PIN_FLD_POID, 0, 0);

		}
	}

	flistp = PIN_FLIST_CREATE(ebufp);
	/*
	* Get the values of the order state id from the database
	*/
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, i_poidp, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_STATUS, NULL, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
       	        "fm_voucher_pol_order_associate_validate rd flds input flist", 
			flistp);
	PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, flistp, 
		&r_flistp, ebufp);
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
        	"fm_voucher_pol_order_associate_validate rd flds return flist", 
			r_flistp);

	/*
	 * Associating a suborder with main order is allowed 
	 * only if the status of:
	 *
	 *   PIN_VOUCHER_ORDER_NEW
	 *
	 * States outside of this list will generate an error.
	 */
	vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_STATUS, 0, 
		ebufp);
	if(vp != NULL){
		status = *(int32 *)vp;

		if(status != PIN_VOUCHER_ORDER_NEW ) 
		{
			/*
			 * validation fails 
			 */
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_PROHIBITED_ARG, PIN_FLD_POID,
				 0, 0);
			PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR, 
				"cannot associate an order if the "
				"order is in request or partial receive state ",
				 ebufp);
		}
	}
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	return;
}
