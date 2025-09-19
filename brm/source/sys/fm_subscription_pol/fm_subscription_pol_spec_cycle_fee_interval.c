/**********************************************************************
 *
 * Copyright (c) 1999, 2009, Oracle and/or its affiliates.All rights reserved. 
 *      
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 **********************************************************************/

#ifndef lint
static char Sccs_id[] = "@(#)$Id: fm_subscription_pol_spec_cycle_fee_interval.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 06:17:47 nishahan Exp $";
#endif

/*******************************************************************
 * Contains the PCM_OP_SUBSCRIPTION_POL_SPEC_CYCLE_FEE_INTERVAL operation. 
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 
 
#include "pcm.h"
#include "ops/bill.h"
#include "ops/subscription.h"
#include "pin_bill.h"
#include "pin_rate.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_subscription_pol_spec_cycle_fee_interval(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_SUBSCRIPTION_POL_SPEC_CYCLE_FEE_INTERVAL operation.
 *******************************************************************/
void
op_subscription_pol_spec_cycle_fee_interval(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	void			*vp = NULL;

	*r_flistpp = NULL;
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_SUBSCRIPTION_POL_SPEC_CYCLE_FEE_INTERVAL) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_subscription_pol_spec_cycle_fee_interval bad opcode error", 
			ebufp);
		return;
	}

	/*
	 * Debug: What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_subscription_pol_spec_cycle_fee_interval input flist", i_flistp);

	/*
	 * Prepare return flist.
	 */
	*r_flistpp = PIN_FLIST_CREATE(ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, vp, ebufp);

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_CHARGED_FROM_T,
		0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_CHARGED_FROM_T, 
		vp, ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_CHARGED_TO_T,
		0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_CHARGED_TO_T, 
		vp, ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_SCALE, 1, ebufp);
	if (vp) {
        	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_SCALE,
                				vp, ebufp);
	}
        /*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"op_subscription_pol_spec_cycle_fee_interval error", ebufp);
	} else {

		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_subscription_pol_spec_cycle_fee_interval output flist", *r_flistpp);
	}
	return;
}
