/*******************************************************************
 *
* Copyright (c) 1999, 2019, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: fm_act_pol_spec_event_cache.c /cgbubrm_omc.19.main.portalbase/1 2019/08/14 17:10:49 mnarasim Exp $";
#endif

/*******************************************************************
 * Contains the PCM_OP_ACT_POL_SPEC_EVENT_CACHE operation. 
 *******************************************************************/

/*******************************************************************
 * The event cache is mainly used to improve the invoicing 
 * performance, i.e., the efficiency in the event search during
 * invoicing.  By default, fields from the BAL_IMPACTS array are
 * cached in a base table field PIN_FLD_INVOICE_DATA, so that 
 * the event search will only need to look into the base event table,
 * which saves time for not doing a table join.  
 * Note the PIN_FLD_INVOICE_DATA field has a database length limit
 * of 4000 bytes, if the cache size is greater than 4000 bytes, it 
 * will be ignored.
 * If you don't want to use the event cache for some reason, simply
 * return a NULL pointer to the caller of this opcode. 
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 
#include <time.h> 

#include "pcm.h"
#include "ops/act.h"
#include "pin_act.h"
#include "pin_cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "fm_bill_utils.h"
#include "psiu_business_params.h"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_act_pol_spec_event_cache(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

void
fm_act_pol_spec_event_cache(
	pcm_context_t	*ctxp,
	int32		flags,
	pin_flist_t	*i_flistp,
	pin_flist_t	**r_flistpp,
	pin_errbuf_t	*ebufp);

/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_ACT_POL_SPEC_EVENT_CACHE operation.
 *******************************************************************/
void
op_act_pol_spec_event_cache(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_ACT_POL_SPEC_EVENT_CACHE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_act_pol_spec_event_cache opcode error", ebufp);
		return;
	}
	/***********************************************************
	 * Debut what we got.
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_act_pol_spec_event_cache input flist", i_flistp);

	/***********************************************************
	 * Do the actual op in a sub.
	 ***********************************************************/
	fm_act_pol_spec_event_cache(ctxp, flags, i_flistp, r_flistpp, 
		ebufp);

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		*r_flistpp = (pin_flist_t *)NULL;
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_act_pol_spec_event_cache error", ebufp);

	} else {

	/***********************************************************
	 * Output flist.
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_act_pol_spec_event_cache output flist", 
			*r_flistpp);

	}
	return;
}

void
fm_act_pol_spec_event_cache(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*bal_flistp = NULL;
	pin_flist_t		*r_flistp = NULL;
	pin_flist_t		*flistp = NULL;
	pin_flist_t		*elem_flistp = NULL;
	pin_flist_t		*format_flistp = NULL;
	pin_cookie_t		cookie = NULL;
	int32			rec_id = 0;
	pin_cookie_t		cookie1 = NULL;
	int32			rec_id1 = 0;
	int32			extended_attributes = PIN_BOOLEAN_FALSE;
	void			*vp = NULL;
	poid_t          	*e_poid = NULL;
	poid_t          	*plan_poid = NULL;
	poid_t          	*deal_poid = NULL;
	pin_flist_t		*obj_flistp = NULL;
	pin_flist_t		*obj2_flistp = NULL;
	pin_fld_num_t 		fld_id = 0;
	int32 			fld_type = 0;
	int32 			gl_id_dummy = 0;


	/**********************************************************
	 * By default, the following fields in the BAL_IMPACTS
	 * array will be cached:
	 *    PIN_FLD_RESOURCE_ID
	 *	  PIN_FLD_QUANTITY
	 *	  PIN_FLD_RATE_TAG
	 *	  PIN_FLD_ITEM_OBJ
	 *	  PIN_FLD_AMOUNT 
	 *	  PIN_FLD_IMPACT_TYPE
	 *	  PIN_FLD_DISCOUNT
	 *	  PIN_FLD_TAX_CODE
	 * 
	 * If customers would like to put more fields from 
	 * the BAL_IMPACTS array to the cache, they need to keep 
	 * the rec_id unchanged in the output flist.
	 * Other fields from other objects or other arrays can be
	 * put in the output flist and will be cached later too.
	 **********************************************************/

	/* Add more OOB fields and customized fields base on configuration */
	extended_attributes = fm_bill_utils_get_cfg_include_extended_attr(
							ctxp, ebufp);

	e_poid = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	if (extended_attributes &&
		fm_utils_get_customized_event_format(ctxp, e_poid, 
			&format_flistp, ebufp)) {
		r_flistp = PIN_FLIST_CREATE(ebufp);

		/* Loop through format to get customized structures */
		cookie = NULL;
		rec_id = 0;
		while ((flistp = PIN_FLIST_ELEM_GET_NEXT(format_flistp,
			PIN_FLD_OBJ_DESC, &rec_id, 1, &cookie,
			ebufp)) != (pin_flist_t *)NULL) {
			cookie1 = NULL;
			rec_id1 = 0;
			while ((elem_flistp = PIN_FLIST_ELEM_GET_NEXT(flistp,
					PIN_FLD_OBJ_ELEM, &rec_id1, 1, &cookie1,
					ebufp)) != (pin_flist_t *)NULL) {

				/* Get name and type of field, only sub-structure needed for now */
				vp = PIN_FLIST_FLD_GET(elem_flistp, PIN_FLD_FIELD_TYPE, 
					0, ebufp);
				if (vp) {
					fld_type = *(int32*)vp;
				} else {
					fld_type = 0;
				}
				if (vp && fld_type == PIN_FLDT_SUBSTRUCT) {
					vp = PIN_FLIST_FLD_GET(elem_flistp, PIN_FLD_FIELD_NAME, 
						0, ebufp);
				    if (vp) {
					fld_id = pin_field_of_name((char *)vp);

					/* Get the sub-structure from event, set it into 
					   output
					 */
					vp = PIN_FLIST_FLD_GET(i_flistp, fld_id, 
							0, ebufp);
					PIN_FLIST_FLD_SET(r_flistp, fld_id,
							vp, ebufp);
				    }
				}
				
			}
		}
		
	}

	/***********************************************************
	 * Check if the required fields are on the input flist
	 * If not, just add them to the return flist.
	 * By default, these fields should be added here.
	 ***********************************************************/
	flistp = NULL;
	cookie = NULL;
	rec_id = 0;
	while ((flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp,
		PIN_FLD_BAL_IMPACTS, &rec_id, 1, &cookie,
		ebufp)) != (pin_flist_t *)NULL) {

		plan_poid = NULL;
		deal_poid = NULL;
		/* r_flistp will be NULL if extended attributes parameter is disabled */
		if (r_flistp == NULL) {
			r_flistp = PIN_FLIST_CREATE(ebufp);
		}
		bal_flistp = PIN_FLIST_ELEM_ADD(r_flistp,
				PIN_FLD_BAL_IMPACTS, rec_id, 
				ebufp);

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_RESOURCE_ID, 
				0, ebufp);
		PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_RESOURCE_ID,
				vp, ebufp);

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_QUANTITY, 
				1, ebufp);
		if (vp != NULL) {
			PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_QUANTITY,
				vp, ebufp);
		}

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_RATE_TAG, 1, ebufp);
		if (vp != NULL) {
			PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_RATE_TAG,
				vp, ebufp);
		}

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_ITEM_OBJ, 0, ebufp);
		PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_ITEM_OBJ,
			vp, ebufp);

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_AMOUNT, 1, ebufp);
		if (vp != NULL) {
			PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_AMOUNT,
				vp, ebufp);
		}
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_IMPACT_TYPE, 1, ebufp);
		if (vp != NULL) {
			PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_IMPACT_TYPE,
				vp, ebufp);
		}
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_DISCOUNT, 1, ebufp);
		if (vp != NULL) {
			PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_DISCOUNT,
				vp, ebufp);
		}

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_TAX_CODE, 1, ebufp);
		if (vp != NULL) {
			PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_TAX_CODE,
				vp, ebufp);
		}

		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_GL_ID, 1, ebufp);
		if (vp != NULL) {
			PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_GL_ID,
				vp, ebufp);
		}
		else{
			PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_GL_ID, &gl_id_dummy, ebufp);
		}


		if (extended_attributes) {
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
				"Adding extended fields and customized fields.");

			/* Base fields to be added 
			 * Product or Discount Name
			 * Deal Name
			 * Plan Name
			 * (Unit Price will be presented as Rate Tag, already present)
			 */
			/* Get product or discount */
			vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_PRODUCT_OBJ, 1, ebufp);
			if (vp != NULL && !PIN_POID_IS_NULL(vp)) {
				fm_bill_utils_read_object(ctxp, vp, &obj_flistp, ebufp);
				/* No need to distinguish /product or /discount here */
				if (obj_flistp) {
					vp = PIN_FLIST_FLD_GET(obj_flistp, PIN_FLD_NAME, 1, ebufp);
					if (vp != NULL) {
						PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_PRODUCT_NAME,
							vp, ebufp);
					}
					PIN_FLIST_DESTROY_EX(&obj_flistp, NULL);
				}
			}

			/* Get purchased product or discount */
			vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_OFFERING_OBJ, 1, ebufp);
			if (vp != NULL && !PIN_POID_IS_NULL(vp)) {
				fm_bill_utils_read_object(ctxp, vp, &obj_flistp, ebufp);
			}

			/* No need to distinguish /purchased_product or /purchased_discount here */
			if (obj_flistp) {
				vp = PIN_FLIST_FLD_GET(obj_flistp, PIN_FLD_PLAN_OBJ, 1, ebufp);
				if (vp != NULL) {
					plan_poid = vp;
				}
				vp = PIN_FLIST_FLD_GET(obj_flistp, PIN_FLD_DEAL_OBJ, 1, ebufp);
				if (vp != NULL) {
					deal_poid = vp;
				}
			}

			if (plan_poid != NULL && !PIN_POID_IS_NULL(plan_poid)) {
				fm_bill_utils_read_object(ctxp, plan_poid, &obj2_flistp, ebufp);
				if (obj2_flistp) {
					vp = PIN_FLIST_FLD_GET(obj2_flistp, PIN_FLD_NAME, 1, ebufp);
					if (vp != NULL) {
						PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_NAME,
							vp, ebufp);
					}
					PIN_FLIST_DESTROY_EX(&obj2_flistp, NULL);
				}
			}

			if (deal_poid != NULL && !PIN_POID_IS_NULL(deal_poid)) {
				fm_bill_utils_read_object(ctxp, deal_poid, &obj2_flistp, ebufp);
				if (obj2_flistp) {
					vp = PIN_FLIST_FLD_GET(obj2_flistp, PIN_FLD_NAME, 1, ebufp);
					if (vp != NULL) {
						PIN_FLIST_FLD_SET(bal_flistp, PIN_FLD_DEAL_NAME,
							vp, ebufp);
					}
					PIN_FLIST_DESTROY_EX(&obj2_flistp, NULL);
				}
			}

			PIN_FLIST_DESTROY_EX(&obj_flistp, NULL);
		}
	}

	*r_flistpp = r_flistp;
	PIN_FLIST_DESTROY_EX(&format_flistp, NULL);

	return;
}

