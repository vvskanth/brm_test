/*
 * @(#)% %
 *
 * Copyright (c) 2002 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)% %";
#endif

/*******************************************************************
 * Contains the PCM_OP_ORDER_POL_PROCESS operation.
 *******************************************************************/

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#define FILE_LOGNAME "fm_order_pol_process.c(1)"

#include "ops/order.h"
#include "pin_order.h"
#include "ops/voucher.h"
#include "pin_voucher.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "fm_utils.h"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_order_pol_process(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_order_pol_process(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);


/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_ORDER_POL_CREATE operation.
 *******************************************************************/
void
op_order_pol_process(
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
	 * Null out results
	 ***********************************************************/
	*r_flistpp = NULL;

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_ORDER_POL_PROCESS) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_order_pol_process opcode error", ebufp);

		return;
	}
	
	/***********************************************************
	 * Debut what we got.
         ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_order_pol_process input flist", i_flistp);

	/***********************************************************
	 * Main rountine for this opcode
	 ***********************************************************/
	fm_order_pol_process(ctxp, flags, i_flistp, r_flistpp, ebufp);
	
	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(r_flistpp, ebufp);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_order_pol_process error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_order_pol_process output flist", *r_flistpp);
	}

	return;
}

/*******************************************************************
 * fm_order_pol_process:
 *
 *******************************************************************/
static void
fm_order_pol_process(
	pcm_context_t           *ctxp,
	int32                   flags,
	pin_flist_t             *i_flistp,
	pin_flist_t             **r_flistpp,
	pin_errbuf_t            *ebufp)
{
	int32			state = PIN_ORDER_DEFAULT_STATE;
	pin_flist_t		*tmp_flistp = NULL;

        poid_t          *order_poidp = NULL;
        char            *poid_type = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * ### Check why it has to be in default state. commented for now.
	 *PIN_FLIST_FLD_SET(tmp_flistp, PIN_FLD_STATE_ID, &state, ebufp);
	 */
		
        /*
         * Get the order poid
         */
        order_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

        /*
         * Get the order poid type
         */
        poid_type = (char *) PIN_POID_GET_TYPE(order_poidp);

	if (strcmp(poid_type, PIN_OBJ_TYPE_ORDER_VOUCHER) == 0) {
		/*
		 * call voucher policy to do the work we want
		 */
		PCM_OP(ctxp, PCM_OP_VOUCHER_POL_ORDER_PROCESS, flags, i_flistp,
                        r_flistpp, ebufp);
        }
        else
        {
		*r_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_order_pol_process error", ebufp);
	}
	return;
}
