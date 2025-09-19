/*
 *
 * Copyright (c) 2003, 2024, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */
#ifndef lint
static char Sccs_id[] = "@(#)$Id: fm_ar_pol_reverse_writeoff.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 05:22:30 nishahan Exp $";
#endif

/*******************************************************************
 * Contains the PCM_OP_AR_POL_REVERSE_WRITEOFF operation. 
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 
 
#include "pcm.h"
#include "pin_bill.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "pin_ar.h"
#include "ops/ar.h"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_ar_pol_reverse_writeoff(
        cm_nap_connection_t	*connp,
	u_int			opcode,
        u_int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp);

static void fm_ar_pol_reverse_writeoff_read_profile(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	int			flags,
	pin_errbuf_t		*ebufp);
/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_AR_POL_REVERSE_WRITEOFF operation.
 *******************************************************************/
void
op_ar_pol_reverse_writeoff(
        cm_nap_connection_t	*connp,
	u_int			opcode,
        u_int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*r_flistp = NULL;


	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_AR_POL_REVERSE_WRITEOFF) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_ar_pol_reverse_writeoff opcode error", ebufp);
		return;
	}

	/***********************************************************
	 * Debug: What we got.
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_ar_pol_reverse_writeoff input flist", i_flistp);

	fm_ar_pol_reverse_writeoff_read_profile(ctxp, i_flistp, &r_flistp, flags, ebufp);
	/***********************************************************
	 * Results.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {

		/***************************************************
		 * Log something and return nothing.
		 ***************************************************/
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_ar_pol_reverse_writeoff error", ebufp);
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		*o_flistpp = NULL;

	} else {

		/***************************************************
		 * Point the real return flist to the right thing.
		 ***************************************************/
		*o_flistpp = r_flistp;

		/***************************************************
		 * Debug: What we're sending back.
		 ***************************************************/
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_ar_pol_reverse_writeoff return flist", r_flistp);
	}

	return;
}

/*******************************************************************
 * fm_ar_pol_reverse_writeoff_read_profile():
 * This policy op-code is used to fetch the writeoff items from 
 * /profile/writeoff object
 *******************************************************************/
static void
fm_ar_pol_reverse_writeoff_read_profile(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	int			flags,
	pin_errbuf_t		*ebufp)
{
	pin_cookie_t	 	cookie = NULL;
	pin_flist_t		*flistp = NULL;
	pin_flist_t		*p_flistp = NULL;
	pin_flist_t		*r_flistp = NULL;
	pin_flist_t		*w_flistp = NULL;
	pin_flist_t		*rev_flistp = NULL;
	poid_t			*a_pdp = NULL;
	poid_t			*p_pdp = NULL;
	int64			db = 0;
	int			flag = 0;
	int32			rec_id = 0;
	void			*vp = NULL;
	int32                   elem_id = 0;


	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	*r_flistpp = PIN_FLIST_CREATE(ebufp);
	a_pdp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, 
				PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, 
		(void *)a_pdp, ebufp);
	
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_PROGRAM_NAME, 0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_PROGRAM_NAME,vp, ebufp);
	
	db = PIN_POID_GET_DB(a_pdp);
	p_flistp = PIN_FLIST_CREATE(ebufp);
	p_pdp = PIN_POID_CREATE(db, "/profile", -1, ebufp);
	PIN_FLIST_FLD_PUT(p_flistp, PIN_FLD_POID, p_pdp, ebufp);
	PIN_FLIST_FLD_SET(p_flistp, PIN_FLD_ACCOUNT_OBJ,(void *)a_pdp, ebufp);
	PIN_FLIST_FLD_SET(p_flistp, PIN_FLD_TYPE_STR, 
				(void *)PIN_WRITEOFF_REV_PROFILE_TYPE, ebufp);
	flistp = PIN_FLIST_ELEM_ADD(p_flistp, PIN_FLD_RESULTS, 
				0, ebufp);
	w_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_WRITEOFF_INFO, 
				PIN_ELEMID_ANY, ebufp);
	PIN_FLIST_FLD_SET(w_flistp, PIN_FLD_FLAGS, NULL, ebufp);
	PIN_FLIST_FLD_SET(w_flistp, PIN_FLD_ITEM_OBJ, NULL, ebufp);
	/*********************************************************************
	* Search for the /profile/writeoff_reversal profile
	*********************************************************************/
	PCM_OP(ctxp, PCM_OP_CUST_FIND_PROFILE, flags, p_flistp, 
				&r_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ar_pol_reverse_writeoff_read_profile error", ebufp);
		goto cleanup;
		/***********/
	}

	cookie = NULL;
	flistp = PIN_FLIST_ELEM_GET_NEXT(r_flistp, PIN_FLD_RESULTS,
			&rec_id, 1, &cookie, ebufp);

	cookie = NULL;
	rec_id = 0;
	if (flistp) {
		while ((w_flistp = PIN_FLIST_ELEM_GET_NEXT(flistp, PIN_FLD_WRITEOFF_INFO,
		       &rec_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {
		       vp = (poid_t *)PIN_FLIST_FLD_GET(w_flistp, PIN_FLD_ITEM_OBJ, 0, ebufp);
		       if (vp && PIN_POID_IS_NULL(vp)) {
			      continue;
		       }
		       rev_flistp = PIN_FLIST_ELEM_ADD(*r_flistpp, PIN_FLD_REVERSALS, 
						       elem_id++, ebufp);
		       PIN_FLIST_FLD_SET(rev_flistp, PIN_FLD_ITEM_OBJ, 
					 (void *)vp, ebufp);
			/**************************************************************
			 * To generate the separate events for the writeoff tax
			 * Uncomment the below lines of code
			 **************************************************************/
			/*flag = PIN_BILL_WRITEOFF_TAX;
			PIN_FLIST_FLD_SET(rev_flistp, PIN_FLD_FLAGS, 
				(void *)&flag, ebufp); */
		}
	}

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ar_pol_reverse_writeoff return flist", *r_flistpp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ar_pol_reverse_writeoff error", ebufp);
		goto cleanup;
		/***********/
	}
cleanup:
	PIN_FLIST_DESTROY_EX(&p_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	return;
}
