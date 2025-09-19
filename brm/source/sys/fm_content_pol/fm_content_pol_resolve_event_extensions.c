/*******************************************************************
 *
 * @(#)% %
 *
 * Copyright (c) 2001 - 2024 Oracle.
 *
 * This material is the confidential property of Oracle Corporation or
 * its licensors and may be used, reproduced, stored or transmitted only
 * in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)% %";
#endif

/*******************************************************************
 * Contains the (example) PCM_OP_CONTENT_POL_RESOLVE_EVENT_EXTENSIONS operation. 
 *
 * NOOP for now.
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
 
#include "pcm.h"
#include "ops/content.h"
#include "pin_act.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_content.h"
#include "pinlog.h"

#define FILE_SOURCE_ID	"fm_content_pol_resolve_extensions.c(1.17)"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void op_content_pol_resolve_event_extensions();

static void
fm_content_pol_resolve_event_extensions(	pcm_context_t	*ctxp, 
			int32		flags,
			pin_flist_t *i_flistp, 
			pin_flist_t **o_flistpp,
			pin_errbuf_t *ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_CONTENT_POL_RESOLVE_EVENT_EXTENSIONS operation.
 *******************************************************************/
void
op_content_pol_resolve_event_extensions(
        cm_nap_connection_t	*connp,
	int32			opcode,
        int32			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*r_flistp = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/******/
	}

	/*
	 * Debug: What did we get?
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_content_pol_resolve_extensions input flist", i_flistp);

	/*
	 * do the real work
	 */
	fm_content_pol_resolve_event_extensions(ctxp, flags, i_flistp, &r_flistp, ebufp);

	/*
	 * Cleanup.  If error, we return NULL output flist.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		/*
		 * Log something and return nothing.
		 */
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_content_pol_resolve_eevent_xtensions error", ebufp);
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		*o_flistpp = (pin_flist_t *)NULL;

	} else {

		/*
		 * Point the real return flist to the right thing.
		 */
		PIN_ERR_CLEAR_ERR(ebufp);
		*o_flistpp = r_flistp;

		/*
		 * Debug: What we're sending back.
		 */
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_content_pol_resolve_event_extensions return flist", r_flistp);

	}
}

/*******************************************************************
 * fm_content_pol_resolve_extensions():
 *******************************************************************/
static void
fm_content_pol_resolve_event_extensions(
	pcm_context_t	*ctxp,
        int32		flags,
	pin_flist_t	*i_flistp,
	pin_flist_t	**o_flistpp,
	pin_errbuf_t	*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/******/
	}

	/* This policy implementation simply returns the poid passed in.
	 * This is a dummy implementation.
	 * The opcode is passed in an array of name value pairs:
	 * PIN_FLD_EXTENDED_DATA
	 *	PIN_FLD_NAME
	 *      PIN_FLD_VALUE
	 * This information can be user to resolve the name value pairs to 
	 * Infranet specific field name/value pairs to be used to calulate the
	 * cost of content activty.
	 */

	/*
	 * Simply return the input flist
	 */
	 *o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
		 
}

