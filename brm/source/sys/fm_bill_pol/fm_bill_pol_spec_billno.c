/*
 *
 *      Copyright (c) 1999, 2024, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_bill_pol_spec_billno.c:RWSmod7.3.1Int:1:2007-Sep-07 06:26:05 %";
#endif

#pragma GCC diagnostic ignored "-Wunused-variable"

#include <stdio.h>
#include "pcm.h"
#include "ops/bill.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_bill.h"
#include "pin_cust.h"
#include "pinlog.h"
#include "pin_os.h"

EXPORT_OP void
op_bill_pol_spec_billno(
	cm_nap_connection_t	*connp,
	u_int			opcode,
	u_int			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_bill_pol_spec_billno(
	pcm_context_t		*ctxp,
	u_int			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp);
 
/*******************************************************************
 * Main routine for the PCM_OP_BILL_POL_PREP_BILLNO  command
 *******************************************************************/
void
op_bill_pol_spec_billno(
	cm_nap_connection_t	*connp,
	u_int			opcode,
	u_int			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*r_flistp = NULL;

	/***********************************************************
	 * Null out results until we have some.
	 ***********************************************************/
	*ret_flistpp = NULL;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_BILL_POL_SPEC_BILLNO) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_bill_pol_spec_billno", ebufp);
		return;
	}

	/***********************************************************
	 * Debug: What did we get?
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_bill_pol_spec_billno input flist", in_flistp);

	/***********************************************************
	 * Call main function to do it
	 ***********************************************************/
	fm_bill_pol_spec_billno(ctxp, flags, in_flistp, &r_flistp, ebufp);

	/***********************************************************
	 * Results.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		*ret_flistpp = (pin_flist_t *)NULL;
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_bill_pol_spec_billno error", ebufp);
	} else {
		*ret_flistpp = r_flistp;
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_bill_pol_spec_billno return flist", r_flistp);
	}

	return;
}

/*******************************************************************
 * fm_bill_pol_spec_billno()
 *
 *	Prep the billno for the given bill obj.
 *
 *******************************************************************/
static void
fm_bill_pol_spec_billno(
	pcm_context_t		*ctxp,
	u_int			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp)
{
	pin_flist_t	*b_flistp = NULL;
	poid_t		*pdp = NULL;
	void            *vp = NULL;

	int64		poid_db;
	int64		poid_id;
	char		billno[60];

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	b_flistp = PIN_FLIST_COPY(in_flistp, ebufp);

	/***********************************************************
	 * We don't need a bill_no if the bill type is subordinate.
	 ***********************************************************/
	vp = PIN_FLIST_FLD_GET(b_flistp, PIN_FLD_PAY_TYPE, 0, ebufp);

	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_DROP(b_flistp, PIN_FLD_PAY_TYPE, ebufp);
	}

	/***********************************************************
	 * Set the BILL_NO field according to poid.
	 ***********************************************************/
        vp = PIN_FLIST_FLD_GET(b_flistp, PIN_FLD_BILL_NO, 1, ebufp);
	if ((vp == (void *)NULL) || (strlen((char *)vp) <= 0)) {

		pdp = (poid_t *)PIN_FLIST_FLD_GET(in_flistp, 
			PIN_FLD_POID, 0, ebufp);

		poid_db = PIN_POID_GET_DB(pdp);
		poid_id = PIN_POID_GET_ID(pdp);

		pin_snprintf(billno, sizeof(billno), "%u.%u.%u.%u-%" I64_PRINTF_PATTERN "u",
		(u_int)((poid_db & (u_int64)MASKUPPER) >> 48),
		(u_int)((poid_db & (u_int64)MASKSECOND) >> 32),
		(u_int)((poid_db & (u_int64)MASKTHIRD) >> 16),
		(u_int)(poid_db & (u_int64)0xFFFF), poid_id);

		PIN_FLIST_FLD_SET(b_flistp, PIN_FLD_BILL_NO, 
			(void *)billno, ebufp);
	}

	*out_flistpp = b_flistp;

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_bill_pol_spec_billno error", ebufp);
	}

	return;
}
