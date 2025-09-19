/*******************************************************************
 *
 *      Copyright (c) 2007 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char Sccs_Id[] = "@(#)%Portal Version: fm_adu_pol_dump.c:RWSmod7.3.1Int:1:2007-Sep-07 06:16:08 %";
#endif

#include "pcm.h"
#include "cm_fm.h"
#include "ops/cust.h"

/*******************************************************************
 * Routines contained herein.
 *******************************************************************/
EXPORT_OP void
op_adu_pol_dump(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_adu_pol_dump(
	cm_nap_connection_t	*connp,
	pin_flist_t		*i_flistp,
	int32			flags,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_ADU_POL_DUMP command
 *******************************************************************/
void
op_adu_pol_dump(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*r_flistp = NULL;

	/***********************************************************
	 * Null out results until we have some.
	 ***********************************************************/
	*ret_flistpp = NULL;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_ADU_POL_DUMP) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_adu_pol_dump", ebufp);
		return;
	}

	/***********************************************************
	 * We will not open any transactions with Policy FM
	 * since policies should NEVER modify the database.
	 ***********************************************************/

	/***********************************************************
	 * Log input flist
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_adu_pol_dump input flist", in_flistp);

	/***********************************************************
	 * Call main function to do it 
	 ***********************************************************/
	fm_adu_pol_dump(connp, in_flistp, flags, &r_flistp, ebufp);

	/***********************************************************
	 * Results.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		*ret_flistpp = (pin_flist_t *)NULL;
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_adu_pol_dump error", ebufp);
	} else {
		*ret_flistpp = r_flistp;
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_adu_pol_dump return flist", r_flistp);
	}

	return;
}

/*******************************************************************
 * fm_adu_pol_dump()
 *
 *	This function will be used to convert the input flist into
 *	a format other than XML. The output should be stored in
 *	PIN_FLD_RESULTS.PIN_FLD_STRING of the output flist.
 *
 *******************************************************************/
static void
fm_adu_pol_dump(cm_nap_connection_t	*connp,
	pin_flist_t		*i_flistp,
	int32			flags,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*r_flistp = NULL;
	pin_flist_t		*res_flistp = NULL;
	poid_t			*acct_pdp = NULL;
	
	/***********************************************************
         * Return immediately if there's an error in ebufp.
         ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Create outgoing flist
	 ***********************************************************/
	r_flistp = PIN_FLIST_CREATE(ebufp);
	*o_flistpp = r_flistp;

	/***********************************************************
	 * Get the account poid.
	 ***********************************************************/
	acct_pdp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_POID, (void *)acct_pdp, ebufp);
	PIN_FLIST_ELEM_SET(r_flistp, NULL, PIN_FLD_RESULTS, 0, ebufp);

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_adu_pol_dump error", ebufp);
	}
	return;
}

