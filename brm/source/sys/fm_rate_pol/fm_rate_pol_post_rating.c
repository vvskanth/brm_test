/*******************************************************************
 *
 * @(#)%Portal Version: fm_rate_pol_post_rating.c:IDCmod7.3PatchInt:1:2007-Mar-26 14:15:36 %
 *
 *      Copyright (c) 1999 - 2023, Oracle.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its subsidiaries or licensors and may be used, reproduced, stored
 *      or transmitted only in accordance with a valid Oracle license or
 *      sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_rate_pol_post_rating.c:IDCmod7.3PatchInt:1:2023-May-18 16:41:36 %";
#endif

/*******************************************************************
 * Contains the PCM_OP_RATE_POL_POST_RATING operation. 
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 

#include "pcm.h"
#include "ops/rate.h"
#include "pin_bill.h"
#include "pin_cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "pin_act.h"

#define FILE_SOURCE_ID  "fm_rate_pol_post_rating.c(5)"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_rate_pol_post_rating(
        cm_nap_connection_t	*connp,
	u_int			opcode,
        u_int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp);

/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/
PIN_IMPORT void
fm_rate_utils_drop_non_matching_rps_rows(
		pin_flist_t     *e_flistp,
		pin_errbuf_t    *ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_RATE_POL_POST_RATING operation.
 *******************************************************************/
void
op_rate_pol_post_rating(
        cm_nap_connection_t	*connp,
	u_int			opcode,
        u_int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*r_flistp = NULL;
	void                    *vp = NULL;
	
	pin_decimal_t 		*quantity = NULL;
	poid_t                  *e_pdp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_RATE_POL_POST_RATING) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_rate_pol_post_rating opcode error", ebufp);
		return;
	}

	/***********************************************************
	 * Debug: What we got.
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_rate_pol_post_rating input flist", i_flistp);

	/***********************************************************
	 * Just a straight copy of the input flist. 
	 ***********************************************************/
	r_flistp = PIN_FLIST_COPY(i_flistp, ebufp);
	
	/***********************************************************
	 * Drop the Dropped call specific fields if present.
	 *
	 * - PIN_FLD_DROPPED_CALL_QUANTITY
	 * - PIN_FLD_CALL_TYPE
	 ***********************************************************/
	vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_DROPPED_CALL_QUANTITY,
				1, ebufp);
	if (vp != NULL) {

		PIN_FLIST_FLD_DROP(r_flistp,
			PIN_FLD_DROPPED_CALL_QUANTITY, ebufp);
	}

	vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_CALL_TYPE, 1, ebufp);
	if (vp != NULL) {
		PIN_FLIST_FLD_DROP(r_flistp, PIN_FLD_CALL_TYPE, ebufp);
	}

	/***********************************************************
	 * Drop the EXTENDED_INFO field if present.
	 ***********************************************************/

	vp = PIN_FLIST_SUBSTR_GET(r_flistp, PIN_FLD_EXTENDED_INFO, 1, ebufp);
	if(vp) {
		PIN_FLIST_SUBSTR_DROP(r_flistp, PIN_FLD_EXTENDED_INFO, ebufp);
	}

	/***********************************************************
	 * Drop the non matching rps rows if present.
	 ***********************************************************/ 
	vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_RATE_PLAN_SELECTOR_OBJ, 1, ebufp);
	if (vp) {
		fm_rate_utils_drop_non_matching_rps_rows(r_flistp, ebufp);
	}

	/***********************************************************
	 * Results.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {

		/***************************************************
		 * Log something and return nothing.
		 ***************************************************/
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_rate_pol_post_rating error", ebufp);
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		*o_flistpp = NULL;

	} else {

		/***************************************************
		 * Point the real return flist to the right thing.
		 ***************************************************/
		PIN_ERR_CLEAR_ERR(ebufp);
	
		/**************************************************************************
                  * Check if the event poid is of the type /event/session or its sub type, then remove the 
                  * PIN_FLD_QUANTITY field from the flist 
                  **************************************************************************/

		e_pdp = (poid_t *)PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_POID, 0, ebufp);

	
		if (fm_utils_is_subtype(e_pdp, PIN_OBJ_TYPE_EVENT_SESSION) == 1) {
		
			quantity = (pin_decimal_t *)PIN_FLIST_FLD_TAKE(r_flistp, 
				PIN_FLD_QUANTITY, 1, ebufp);
			pbo_decimal_destroy(&quantity);
		}
		
		if (PIN_ERR_IS_ERR(ebufp)) {
                            
                    /***************************************************
                     * Log something and return nothing.
                     ***************************************************/
                    PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                            "op_rate_pol_post_rating error while\
				 removing PIN_FLD_QUANTITY field", ebufp);

                    PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
                    *o_flistpp = NULL;                    
                 }
		
	
		*o_flistpp = r_flistp;

		/***************************************************
		 * Debug: What we're sending back.
		 ***************************************************/
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_rate_pol_post_rating return flist", *o_flistpp);

	}

	return;
}
