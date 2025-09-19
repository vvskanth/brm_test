/*****************************************************************************
*
*      Copyright (c) 2007 Oracle. All rights reserved.
*
*      This material is the confidential property of Oracle Corporation or
*      its licensors and may be used, reproduced, stored or transmitted only
*      in accordance with a valid Oracle license or sublicense agreement.
*
*****************************************************************************/
#ifndef lint
static  char    Sccs_id[] = "@(#) %full_filespec: fm_ar_pol_get_event_rum_charge_details.c;%";
#endif

/*******************************************************************
 * Contains the PCM_OP_AR_POL_GET_EVENT_RUM_CHARGE_DETAILS operation.
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
op_ar_pol_get_event_rum_charge_detail(
        cm_nap_connection_t     *connp,
        u_int                   opcode,
        u_int                   flags,
        pin_flist_t             *i_flistp,
        pin_flist_t             **o_flistpp,
        pin_errbuf_t            *ebufp);

static void
fm_ar_pol_get_event_rum_charge_details(
    pcm_context_t         *ctxp,
    pin_flist_t             *i_flistp,
    pin_flist_t             **r_flistpp,
    pin_errbuf_t            *ebufp);


/****************************************************************************
 * Main routine for the PCM_OP_AR_POL_GET_EVENT_RUM_CHARGE_DETAIL operation.
 ****************************************************************************/
void
op_ar_pol_get_event_rum_charge_detail(
        cm_nap_connection_t     *connp,
        u_int                   opcode,
        u_int                   flags,
        pin_flist_t             *i_flistp,
        pin_flist_t             **ret_flistpp,
        pin_errbuf_t            *ebufp)
{
        pcm_context_t           *ctxp = connp->dm_ctx;
        pin_flist_t             *r_flistp = NULL;


        if (PIN_ERR_IS_ERR(ebufp))
                return;
        PIN_ERR_CLEAR_ERR(ebufp);
	/***********************************************************
         * Insanity check.
         ***********************************************************/
        if (opcode != PCM_OP_AR_POL_GET_EVENT_RUM_CHARGE_DETAIL) {
                pin_set_err(ebufp, PIN_ERRLOC_FM,
                        PIN_ERRCLASS_SYSTEM_DETERMINATE,
                        PIN_ERR_BAD_OPCODE, 0, 0, opcode);
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "op_ar_pol_get_event_rum_charge_details opcode error", ebufp);
                return;
        }

        /***********************************************************
         * Debug: What we got.
         ***********************************************************/
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "op_ar_pol_get_event_rum_charge_details input flist", i_flistp);

        fm_ar_pol_get_event_rum_charge_details(ctxp,i_flistp, &r_flistp, ebufp);

        if (PIN_ERR_IS_ERR(ebufp)) {

                *ret_flistpp = (pin_flist_t *) NULL;
                PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                "op_ar_pol_get_event_rum_charge_detail error",
                        ebufp);
        } else {

                *ret_flistpp = r_flistp;
                PIN_ERR_CLEAR_ERR(ebufp);

                PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "op_ar_pol_get_event_rum_charge_detail "\
                "return flist", r_flistp);
        }

    return;
}

/* -----------------------------------------------------------------------------
 *    Function that does the actual stuff.
 *    This can be used to customize to add More fields of RUM_MAP Array to event
 * --------------------------------------------------------------------------*/

static void
fm_ar_pol_get_event_rum_charge_details(
    pcm_context_t           *ctxp,
    pin_flist_t             *in_flistp,
    pin_flist_t             **out_flistpp,
    pin_errbuf_t            *ebufp)
{
    if (PIN_ERR_IS_ERR(ebufp))
        return;
    PIN_ERR_CLEAR_ERR(ebufp);
    /*
     * Create outgoing flist
     */

    *out_flistpp = PIN_FLIST_COPY(in_flistp, ebufp);

    /*
     * Error?
     */

    if (PIN_ERR_IS_ERR(ebufp)) {
        PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
            "fm_ar_pol_get_event_rum_charge_details error",
            ebufp);
    }

    return;
}
