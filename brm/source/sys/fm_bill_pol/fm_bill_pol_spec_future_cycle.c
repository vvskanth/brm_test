/*
 *
 * Copyright (c) 1996, 2022, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static char Sccs_id[] = "@#$Id: fm_bill_pol_spec_future_cycle.c /portalbase/1 2022/03/14 05:33:27 mkothari Exp $";
#endif

#pragma GCC diagnostic ignored "-Wunused-variable"

/*******************************************************************
 * Contains the PCM_OP_BILL_POL_SPEC_FUTURE_CYCLE operation. 
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 
#include <time.h> 
#include "pcm.h"
#include "ops/bill.h"
#include "pin_bill.h"
#include "pin_pymt.h"
#include "fm_utils.h"
#include "fm_bill_utils.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "pin_cust.h"
#include "psiu_business_params.h"
#include "fm_utils_bparams_cache.h"

/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/
extern time_t fm_utils_cycle_actgfuturet();

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_bill_pol_spec_future_cycle(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_bill_pol_spec_future_cycle_default();

static void
fm_bill_pol_spec_future_cycle_advance();

/*******************************************************************
 * Main routine for the PCM_OP_BILL_POL_SPEC_FUTURE_CYCLE operation.
 *******************************************************************/
void
op_bill_pol_spec_future_cycle(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	

	*r_flistpp = NULL;
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_BILL_POL_SPEC_FUTURE_CYCLE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_bill_pol_spec_future_cycle bad opcode error", 
			ebufp);
		return;
	}

	/*
	 * Debug: What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_bill_pol_spec_future_cycle input flist", i_flistp);


	if (pin_conf_advance_bill_cycle) {
	/*
	 * The following example demonstrates a specific logic of setting of 
	 * the next_t and future_t fields.
	 * At the account creation, next_t is taken from the next day after 
	 * creation. Future_t is taken from the month after next_t.
	 * If next_t = Jan 28, and the year are not leap, then future_t = Mar 1.
	 */

		fm_bill_pol_spec_future_cycle_advance(i_flistp,
			r_flistpp, ebufp);
	} else {
	/*
	 * The code bellow is a default implementation
	 */

		fm_bill_pol_spec_future_cycle_default(connp, i_flistp,
			r_flistpp, ebufp);
	}

	/*
	 * Error?
	 */

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"op_bill_pol_spec_future_cycle error", ebufp);
	} else {

		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_bill_pol_spec_future_cycle output flist", *r_flistpp);
	}

	return;
}

/*******************************************************************
 * fm_bill_pol_spec_future_cycle_default():
 *
 *     Calculate fields PIN_FLD_ACTG_NEXT_T and PIN_FLD_ACTG_FUTURE_T 
 *
 *******************************************************************/
static void
fm_bill_pol_spec_future_cycle_default(
	cm_nap_connection_t     *connp,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	void			*vp = NULL;

	time_t			event_end_t = (time_t)0;
	time_t			next_t = (time_t)0;
	time_t			future_t = (time_t)0;

	int32			dom = 0;
	int32			cycle_len = 0;
	
	int			cycle_len_specified = 0;
	int			creation = 1;
	int32           	short_bill_cycle = 0 ;
	int32			curr_dom = 0;
	int32           	config_billing_delay = 0;
	int32           	config_billing_cycle = 0;
	int32			delay = 0;
	int32			no_of_hours = 60 * 60 * 24;
	time_t                  delay_end_t = 0;
	struct tm		*tm = 0;

	pcm_context_t	*ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	*r_flistpp = PIN_FLIST_CREATE(ebufp);

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, vp, ebufp);

	/*
	 * Get fields which are needed to calculate next_t and future_t
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_END_T, 1, ebufp);
        if (vp) {
		event_end_t = *((time_t *)vp);
        }

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACTG_CYCLE_DOM, 1, ebufp);
        if (vp) {
		dom = *((int32 *)vp);
        }

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACTG_CYCLE_LEN, 1, ebufp);
        if (vp) {
		cycle_len = *((int32 *)vp);
		cycle_len_specified = 1;
        }

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACTG_NEXT_T, 1, ebufp);
        if (vp) {
		next_t = *((time_t *)vp);
		creation = 0;
		curr_dom = fm_utils_cycle_get_dom(next_t, ebufp);
        }

	/*
	 * Calculate next_t
	 */
	if (creation) {
		next_t = fm_utils_cycle_actgfuturet(dom, cycle_len,
			event_end_t, ebufp);
	} 
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_ACTG_NEXT_T, (void *)&next_t, 
		ebufp);

	/*
	 * Calculate future_t
	 */
	if (creation || !cycle_len_specified) {
	/* 
	 * This is a case of account creation or call from make_bill
	 */
		future_t = fm_utils_cycle_actgnextt(next_t, dom, ebufp);

		/* Align future_t with new dom during billing */
		tm = localtime(&future_t);
		if(tm && ((u_int)tm->tm_mday != dom)
			&& (curr_dom != 0)
			&& (fm_utils_op_is_ancestor(connp->coip, PCM_OP_BILL_MAKE_BILL))) {

			short_bill_cycle = fm_utils_bparams_cache_get_int(ctxp,PSIU_BPARAMS_BILLING_PARAMS,
					PSIU_BPARAMS_SHORT_CYCLE,0, ebufp);
			if (short_bill_cycle) {
				cycle_len = PIN_ACTG_CYCLE_SHORT;
			} else {
				cycle_len = PIN_ACTG_CYCLE_SHORT;
				if ((dom > curr_dom) && ((dom - curr_dom) < 15)) {
					cycle_len = PIN_ACTG_CYCLE_LONG;
				}
				if ((dom < curr_dom) && ((curr_dom - dom) > 15)) {
					cycle_len = PIN_ACTG_CYCLE_LONG;
				}
			}
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,"update future_t");
			future_t = fm_utils_cycle_actgfuturet(dom, cycle_len, next_t,
				ebufp);
		}
		future_t = fm_utils_time_round_to_midnight(future_t);
	} else {
	/* This is a case when account already exists,
	 * and we are changing billing cycle
	 */
		future_t = fm_utils_cycle_actgfuturet(dom, cycle_len, next_t, 
			ebufp);
		if((curr_dom != 0) && (dom != curr_dom)) {

			fm_bill_get_config_billing_delay_and_cycle(ctxp,
				&config_billing_delay, &config_billing_cycle, ebufp);

			if(config_billing_delay) {
				delay_end_t = next_t;
				delay = config_billing_delay/no_of_hours;
				fm_utils_add_n_days(delay, &delay_end_t);
				if((event_end_t >= next_t) && (event_end_t < delay_end_t)) {
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,"do not update future_t in delay period");
						future_t = fm_utils_cycle_actgnextt(next_t, curr_dom, ebufp);
				}
			}
		}
	} 
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_ACTG_FUTURE_T, (void *)&future_t, 
		ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
		"fm_bill_pol_spec_future_cycle_default", ebufp);
        }

	return;
}

/*******************************************************************
 * fm_bill_pol_spec_future_cycle_advance():
 *
 *     Calculate fields PIN_FLD_ACTG_NEXT_T and PIN_FLD_ACTG_FUTURE_T 
 *
 *******************************************************************/
static void
fm_bill_pol_spec_future_cycle_advance(
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	void			*vp = NULL;

	time_t			event_end_t = (time_t)0;
	time_t			next_t = (time_t)0;
	time_t			future_t = (time_t)0;

	int32			dom = 0;
	int32			cycle_len = 0;
	
	int			cycle_len_specified = 0;
	int			adjust_next_t = 0;
	int			adjust_future_t = 0;
	int			subord = 0;
	int			next_dom = 0;
	int			curr_dom = 0;
	int			creation = 1;
	int32                   dom_advanced = PIN_BILL_FLG_DOM_NOT_INCREMENTED;
	

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	*r_flistpp = PIN_FLIST_CREATE(ebufp);

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, vp, ebufp);

	/*
	 * Get fields which are needed to calculate next_t and future_t
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_END_T, 1, ebufp);
        if (vp) {
		event_end_t = *((time_t *)vp);
        }

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACTG_CYCLE_DOM, 1, ebufp);
        if (vp) {
		dom = *((int32 *)vp);
        }

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACTG_CYCLE_LEN, 1, ebufp);
        if (vp) {
		cycle_len = *((int32 *)vp);
		cycle_len_specified = 1;
        }

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACTG_NEXT_T, 1, ebufp);
        if (vp) {
		next_t = *((time_t *)vp);
		creation = 0;
        }

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_FLAGS, 1, ebufp);
	if (vp) {
		dom_advanced = *((int32 *)vp);
	}
	
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_PAY_TYPE, 1, ebufp);
        if (vp && (*((int32 *)vp) == PIN_PAY_TYPE_SUBORD)) {
		subord = 1;
	}

	/*
	 * Calculate next_t (only in the case of account creation)
	 */
	if (creation) {
		if (!subord) {
			curr_dom = fm_utils_cycle_get_dom(event_end_t, ebufp);
			if (dom >= curr_dom) {
			       next_t = event_end_t;
				if (dom_advanced == PIN_BILL_FLG_DOM_INCREMENTED) {
					fm_utils_add_n_days(dom - curr_dom, &next_t);
				} else {
					fm_utils_add_n_days(dom - curr_dom + 1, &next_t);
				}
			       next_t = fm_utils_time_round_to_midnight(next_t);
			} else {
				next_t = fm_utils_cycle_actgfuturet(dom, 
					PIN_ACTG_CYCLE_SHORT, event_end_t, ebufp);
				if (dom_advanced == PIN_BILL_FLG_DOM_NOT_INCREMENTED) {							
					fm_utils_add_n_days(1, &next_t);
				}
			}
		} else {  /* account is subordinate one */
		/*
		 * For subordinate account, dom is taken from parent.
		 * Therefore, dom can be equal to 29.
		 */	
			if (dom == 29) {
				adjust_next_t = 1;
				dom--;
				if (fm_utils_cycle_get_dom(event_end_t, 
					ebufp) > 1) {
			       		fm_utils_add_n_days(-1,
						&event_end_t);
				}
			}

			next_t = fm_utils_cycle_actgfuturet(dom, 
				PIN_ACTG_CYCLE_SHORT, event_end_t, ebufp);

			if (adjust_next_t) {
				fm_utils_add_n_days(1, &next_t);
			}

		} /* account is subordinate one */
	} /* (creation) */

	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_ACTG_NEXT_T, (void *)&next_t, 
				ebufp);

	/*
	 * Calculate future_t
	 */
	if (creation || !cycle_len_specified) {
	/* This is a case of account creation or call from make_bill */
		next_dom = fm_utils_cycle_get_dom(next_t, ebufp);
		if (next_dom > 28) {
			fm_utils_add_n_days(-1, &next_t);
			adjust_future_t = 1;
		}

		future_t = fm_utils_cycle_actgnextt(next_t, dom, ebufp);
		if (adjust_future_t) {
			fm_utils_add_n_days(1, &future_t);
		} 
	} else { /* This is a case when account already exists, 
		  * and we are changing billing cycle 
		  */
		if (!subord) {
			dom++;
		}
		next_dom = fm_utils_cycle_get_dom(next_t, ebufp);
		if (next_dom == 29) {
			fm_utils_add_n_days(-1, &next_t);
			if (dom > 1) {
				dom--;
			} 
			adjust_future_t = 1;
		} else if (dom == 29) {
			dom--;
			if (next_dom > 1) {
				fm_utils_add_n_days(-1, &next_t);
			}
			adjust_future_t = 1;
		}

		future_t = fm_utils_cycle_actgfuturet(dom, cycle_len, next_t, 
			ebufp);

		if (adjust_future_t) {
			fm_utils_add_n_days(1, &future_t);
		}
	}

	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_ACTG_FUTURE_T, (void *)&future_t, 
		ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
		"fm_bill_pol_spec_future_cycle_advance", ebufp);
        }

	return;
}
