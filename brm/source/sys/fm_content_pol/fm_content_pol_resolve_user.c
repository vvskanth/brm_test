/*******************************************************************
 *
 * @(#)fm_content_pol_resolve_user.c 1.0 2001/01/29
 *
 * Copyright (c) 2001 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or
 * its licensors and may be used, reproduced, stored or transmitted only
 * in accordance with a valid Oracle license or sublicense agreement.
 *
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_content_pol_resolve_user.c:OptMgrsVelocityInt:1:2006-Sep-20 13:06:53 %";
#endif

/*******************************************************************
 * Contains the (example) PCM_OP_CONTENT_POL_RESOLVE_USER operation. 
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

#define FILE_SOURCE_ID	"fm_content_pol_resolve_user.c(1.17)"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void op_content_pol_resolve_user();

static void
fm_content_pol_resolve_user(	pcm_context_t	*ctxp, 
			int32		flags,
			pin_flist_t *i_flistp, 
			pin_flist_t **o_flistpp,
			pin_errbuf_t *ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_CONTENT_POL_RESOLVE_USER operation.
 *******************************************************************/
void
op_content_pol_resolve_user(
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
		"op_content_pol_resolve_user input flist", i_flistp);

	/*
	 * do the real work
	 */
	fm_content_pol_resolve_user(ctxp, flags, i_flistp, &r_flistp, ebufp);

	/*
	 * Cleanup.  If error, we return NULL output flist.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		/*
		 * Log something and return nothing.
		 */
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_content_pol_resolve_user error", ebufp);
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
			"op_content_pol_resolve_user return flist", r_flistp);

	}
}

/*******************************************************************
 * fm_content_pol_resolve_user():
 *******************************************************************/
static void
fm_content_pol_resolve_user(
	pcm_context_t	*ctxp,
        int32		flags,
	pin_flist_t	*i_flistp,
	pin_flist_t	**o_flistpp,
	pin_errbuf_t	*ebufp)
{
	pin_flist_t	*r_flistp = NULL;   /* return flist ptr */
	void		*vp;
	int32		status = PIN_BOOLEAN_TRUE;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/******/
	}

	/* This policy implementation simply returns the login passed in.
	 * This is a dummy implementation.
	 * The opcode is passed in an array of name value pairs:
	 * PIN_FLD_NETWORK_DATA
	 *	PIN_FLD_NAME
	 *      PIN_FLD_VALUE
	 * This information can be user to resolve the login set in (request id)
	 * to the actual login of the /service/content of the user in Infranet.
	 */

	/*
	 * Initialize the return flist
	 */
	r_flistp = PIN_FLIST_CREATE(ebufp);
	*o_flistpp = r_flistp;
		 

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_POID, vp, ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_LOGIN, 0, ebufp);
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_LOGIN, vp, ebufp);
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_STATUS, (void *)&status, ebufp);

}

