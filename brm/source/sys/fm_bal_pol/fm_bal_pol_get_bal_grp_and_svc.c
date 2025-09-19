/*
 *
 *      Copyright (c) 2005 - 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_bal_pol_get_bal_grp_and_svc.c:BillingVelocityInt:3:2006-Sep-05 21:51:51 %";
#endif

/*******************************************************************
 * Contains the PCM_OP_BAL_POL_GET_BAL_GRP_AND_SVC operation. 
 *******************************************************************/

/*******************************************************************
*******************************************************************/

#include <stdio.h> 
#include <string.h> 
 
#include "pcm.h"
#include "ops/bal.h"
#include "pin_bal.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"


/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_bal_pol_get_bal_grp_and_svc(
	cm_nap_connection_t	*connp,
	int			opcode,
	int			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void 
fm_bal_pol_get_bal_grp_and_svc(
	pcm_context_t   	*ctxp,
	int 			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		*r_flistp,
	pin_errbuf_t		*ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_BAL_POL_GET_BAL_GRP_AND_SVC operation.
 *******************************************************************/
void
op_bal_pol_get_bal_grp_and_svc(
        cm_nap_connection_t	*connp,
	int			opcode,
        int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*r_flistp = NULL;
	void 			*vp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
	{
		return ;
	}
	PIN_ERR_CLEAR_ERR(ebufp);
	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_BAL_POL_GET_BAL_GRP_AND_SVC) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_bal_pol_get_bal_grp_and_svc opcode error", ebufp);
		return;
	}
	
	/***********************************************************
	 * Debug: The input Flist
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_bal_pol_get_bal_grp_and_svc input flist", i_flistp);

	/***********************************************************
	 * Prep the return flist.
	 ***********************************************************/
	r_flistp = PIN_FLIST_CREATE(ebufp);

	/***********************************************************
	 * Get the poid from the Input Flist.
	 ***********************************************************/
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_POID, vp, ebufp);
	fm_bal_pol_get_bal_grp_and_svc(ctxp, flags, i_flistp, r_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		/***************************************************
		 * Log Error Buuffer and return.
		 ***************************************************/
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_bal_pol_get_bal_grp_and_svc error", ebufp);
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		*o_flistpp = NULL;

	} 
	else
	{

		*o_flistpp = r_flistp;
		/***************************************************
		 * Debug: What we're sending back.
		 ***************************************************/
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_bal_pol_get_bal_grp_and_svc return flist", r_flistp);
	}

	return;
}

/********************************************************************
 * fm_bal_pol_get_bal_grp_and_svc()
 ********************************************************************/
static void 
fm_bal_pol_get_bal_grp_and_svc(
	pcm_context_t           *ctxp,
	int			flags,
	pin_flist_t 		*i_flistp,
	pin_flist_t 		*r_flistp,
	pin_errbuf_t            *ebufp)
{
	int             err = PIN_ERR_NONE;
	

	if (PIN_ERR_IS_ERR(ebufp))
	{
		return ;
	}
        PIN_ERR_CLEAR_ERR(ebufp);


	if (PIN_ERR_IS_ERR(ebufp)){
		/***************************************************
		 * Log Error Buffer and return.
		 ***************************************************/
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_bal_pol_get_bal_grp_and_svc error", ebufp);
	}
	return ;
}
