/*******************************************************************
 *
 * Copyright (c) 1999, 2023, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: fm_act_pol_spec_glid.c /cgbubrm_mainbrm.portalbase/1 2019/02/14 22:51:15 agangrad Exp $";
#endif

/*******************************************************************
 * Contains the PCM_OP_ACT_POL_SPEC_GLID operation. 
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 
 
#include "pcm.h"
#include "ops/act.h"
#include "pin_act.h"
#include "pin_cust.h"
#include "pin_rate.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "fm_utils.h"
#include "fm_bill_utils.h"
#include "pin_bill.h"
#include "pin_pymt.h"

/*******************************************************************
 * Prototypes.
 *******************************************************************/
static void
fm_act_pol_spec_glid (
        cm_nap_connection_t	*connp,
        pin_flist_t		*i_flistp,
	const char			*event_type,
        pin_flist_t		*s_flistp,
        pin_errbuf_t		*ebufp);

void
fm_act_pol_add_event_bal_impacts (
	pcm_context_t		*ctxp,
        pin_flist_t		*e_flistp,
        pin_errbuf_t		*ebufp);

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_act_pol_spec_glid(
        cm_nap_connection_t	*connp,
	int			opcode,
        int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**r_flistpp,
        pin_errbuf_t		*ebufp);


/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_ACT_POL_SPEC_GLID operation.
 *******************************************************************/
void
op_act_pol_spec_glid(
        cm_nap_connection_t	*connp,
	int			opcode,
        int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**r_flistpp,
        pin_errbuf_t		*ebufp)
{
	pin_cookie_t		cookie = NULL;
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*e_flistp = NULL;
	pin_flist_t		*s_flistp = NULL;
	pin_flist_t		*flistp = NULL;
	const char			*event_type = NULL;
	poid_t			*e_pdp = NULL;

	int			rec_id = 0;
	int			*glid = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);
	*r_flistpp = (pin_flist_t *)NULL;

	/*******************************************************************
	 * Insanity check.
 	 *******************************************************************/
	if (opcode != PCM_OP_ACT_POL_SPEC_GLID) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_act_pol_spec_glid bad opcode error", ebufp);
		return;
	}

	/*******************************************************************
	 * Debug: What we got.
 	 *******************************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_act_pol_spec_glid input flist", i_flistp);

	/*******************************************************************
	 * Our action will depend on the type of event
	 * Get the event substruct to get all the needed data
 	 *******************************************************************/
	*r_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
	e_flistp = PIN_FLIST_SUBSTR_GET(*r_flistpp, PIN_FLD_EVENT, 0, ebufp);
	e_pdp = (poid_t *)PIN_FLIST_FLD_GET(e_flistp, PIN_FLD_POID, 0, ebufp);
	event_type = PIN_POID_GET_TYPE(e_pdp);

	/******************************************************************
	 * Check PIN_FLD_RATES_USED array, add a dummy one if not.
	 ******************************************************************/
	if (PIN_FLIST_ELEM_COUNT(e_flistp, PIN_FLD_BAL_IMPACTS, ebufp) == 0) {

		/**********************************************************
		 * Add the bal impacts array for this pre-rates event.
 	 	 **********************************************************/
		fm_act_pol_add_event_bal_impacts(ctxp, e_flistp, ebufp);
	}

	/******************************************************************
	 * Now walk the PIN_FLD_RATES_USED array and check for G/L ID. 
	 ******************************************************************/
	while ((flistp = PIN_FLIST_ELEM_GET_NEXT(e_flistp, PIN_FLD_BAL_IMPACTS, 
		&rec_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {

		/**********************************************************
	 	 * G/L id is inside the PIN_FLD_SUBTOTAL array.
 	 	 **********************************************************/
		glid = (int *)PIN_FLIST_FLD_GET(flistp, 
				PIN_FLD_GL_ID, 1, ebufp);

		/**********************************************************
		 * We need to fill in the G/L id now.
 	 	 **********************************************************/
		if (glid == (int *)NULL) {
			 fm_act_pol_spec_glid(connp, i_flistp, 
				event_type, flistp, ebufp);
		}
	}

	/*******************************************************************
	 * Error?
 	 *******************************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_act_pol_spec_glid error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_act_pol_spec_glid return flist", *r_flistpp);
	}

	return;
}

/*******************************************************************
 * fm_act_pol_spec_glid
 * Routine that assigns the G/L id for pre/partially rated events.
 *******************************************************************/
static void
fm_act_pol_spec_glid (
        cm_nap_connection_t	*connp,
        pin_flist_t		*i_flistp,
	const char			*event_type,
        pin_flist_t		*s_flistp,
        pin_errbuf_t		*ebufp)
{
	int			glid = 0;
        pcm_context_t           *ctxp = connp->dm_ctx;
	pin_flist_t		*p_flistp = NULL;
        pin_flist_t		*e_flistp = NULL;
	int32			*cmdp = NULL;

	/*******************************************************************
	 * get glid from /config/map_glid based on event_type and
	 * string version and string id of the Debit/Credit account
	 * adjustment reason code, if any.
 	 *******************************************************************/

	e_flistp = PIN_FLIST_SUBSTR_GET(i_flistp, PIN_FLD_EVENT,
			0, ebufp);

	glid = fm_utils_get_glid(ctxp, e_flistp, ebufp);

	if (!strstr(event_type, "/event/billing/payment") &&
		!strstr(event_type, "/event/billing/refund")) {

		PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_GL_ID, 
				(void *)&glid, ebufp);
	} else {
	
		/**********************************************************
		 * Check the command to see if it was PIN_CHARGE_CMD_REFUND
		 **********************************************************/
		if (strstr(event_type, "/event/billing/payment")) {
			p_flistp = PIN_FLIST_SUBSTR_GET(e_flistp, 
				PIN_FLD_PAYMENT, 0, ebufp);
		} else {
			p_flistp = PIN_FLIST_SUBSTR_GET(e_flistp, 
				PIN_FLD_REFUND, 0, ebufp);
		}
		cmdp = (int32 *)PIN_FLIST_FLD_GET(p_flistp,
				PIN_FLD_COMMAND, 0, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			 PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_act_pol_spec_glid error - no command", 
				ebufp);
			return;
		}

		if (cmdp && *cmdp == PIN_CHARGE_CMD_REFUND) {
			/***********************************************
			 * Important - This is where a GLID for the 
			 * refund should be specified, it is set to a
			 * default of the payment GLID.
			 ***********************************************/
			PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_GL_ID, 
					(void *)&glid, ebufp);
		} else {
			PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_GL_ID,
					(void *)&glid, ebufp);
		}
	}
	return;
}

/*******************************************************************
 * fm_act_pol_add_event_bal_impacts():
 *******************************************************************/
void
fm_act_pol_add_event_bal_impacts(
	pcm_context_t		*ctxp,
	pin_flist_t		*a_flistp,
	pin_errbuf_t		*ebufp)
{
	pin_cookie_t		cookie = NULL;
	pin_flist_t		*bi_flistp = NULL;
	pin_flist_t		*flistp = NULL;
	pin_flist_t		*bia_flistp = NULL;
	poid_t			*r_pdp = NULL;
	void			*vp = NULL;

	int			impact_type = 0;
	int			count = 0;
	int			rec_id = 0;
	pin_decimal_t 		*dummyp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	dummyp = pbo_decimal_from_str("0.0", ebufp);
	/***********************************************************
	 * Set the balance impact type as pre-rated.
	 ***********************************************************/
	impact_type = PIN_IMPACT_TYPE_PRERATED;

	bia_flistp = PIN_FLIST_CREATE(ebufp);

	/***********************************************************
	 * Walk the PIN_FLD_TOTALS array in the original event and
	 * add a corresponding balance impact arary for that.
	 ***********************************************************/
	while ((flistp = PIN_FLIST_ELEM_GET_NEXT(a_flistp, PIN_FLD_TOTAL, 
		&rec_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {

		/***************************************************
	 	 * Add a new element for this balance impact.
		 ***************************************************/
		count = (int)PIN_FLIST_ELEM_COUNT(bia_flistp, 
				PIN_FLD_BAL_IMPACTS, ebufp);
		bi_flistp = PIN_FLIST_ELEM_ADD(bia_flistp, 
				PIN_FLD_BAL_IMPACTS, count, ebufp);

		/***************************************************
	 	 * Start filling in the standard field values now.
		 ***************************************************/
		PIN_FLIST_FLD_SET(bi_flistp, PIN_FLD_IMPACT_TYPE,
				(void *)&impact_type, ebufp);
		PIN_FLIST_FLD_SET(bi_flistp, PIN_FLD_RESOURCE_ID,
				(void *)&rec_id, ebufp);
		PIN_FLIST_FLD_SET(bi_flistp, PIN_FLD_TAX_CODE, 
				(void *)"", ebufp);
		PIN_FLIST_FLD_SET(bi_flistp, PIN_FLD_RATE_TAG, 
				(void *)"", ebufp);

		/***************************************************
		 * Get and Set the account and item object poids.
		 ***************************************************/
		vp = PIN_FLIST_FLD_GET(a_flistp, 
				PIN_FLD_ACCOUNT_OBJ, 0, ebufp);
		PIN_FLIST_FLD_SET(bi_flistp, 
				PIN_FLD_ACCOUNT_OBJ, vp, ebufp);

		vp = PIN_FLIST_FLD_GET(a_flistp, 
				PIN_FLD_ITEM_OBJ, 1, ebufp);
		if (vp) {
			PIN_FLIST_FLD_SET(bi_flistp, 
				PIN_FLD_ITEM_OBJ, vp, ebufp);
		}

		/***************************************************
		 * Fill in the dummy rate object poid.
		 ***************************************************/
		r_pdp = PIN_POID_CREATE(0, "", 0, ebufp);
		PIN_FLIST_FLD_PUT(bi_flistp, PIN_FLD_RATE_OBJ, 
				r_pdp, ebufp);

		/***************************************************
		 * set the 0 for quantity, discount and percentage
		 ***************************************************/
		PIN_FLIST_FLD_SET(bi_flistp, PIN_FLD_DISCOUNT, 
				(void *)dummyp, ebufp);
		PIN_FLIST_FLD_SET(bi_flistp, PIN_FLD_PERCENT, 
				(void *)dummyp, ebufp);
		PIN_FLIST_FLD_SET(bi_flistp, PIN_FLD_QUANTITY, 
				(void *)dummyp, ebufp);

		/***************************************************
		 * Set the amount deferred to 0.0
		 ***************************************************/
		PIN_FLIST_FLD_SET(bi_flistp, PIN_FLD_AMOUNT_DEFERRED, 
				(void *)dummyp, ebufp);

		/***************************************************
		 * Get the amount and set in the bal impact.
		 ***************************************************/
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_AMOUNT, 0, ebufp);
		PIN_FLIST_FLD_SET(bi_flistp, PIN_FLD_AMOUNT, vp, ebufp);

	}

	PIN_FLIST_CONCAT(a_flistp, bia_flistp, ebufp);

	PIN_FLIST_DESTROY_EX(&bia_flistp, NULL);
	pbo_decimal_destroy(&dummyp);
	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_act_pol_add_event_bal_impacts error", ebufp);
	}

	return;
}
