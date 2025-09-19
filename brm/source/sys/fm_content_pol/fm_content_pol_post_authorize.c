/*
 * @(#)%Portal Version: fm_content_pol_post_authorize.c:OptMgrsVelocityInt:1:2006-Sep-20 13:06:15 %
 *
 * Copyright (c) 2002 - 2022 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or
 * its licensors and may be used, reproduced, stored or transmitted only
 * in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = 
	"@(#)%Portal Version: fm_content_pol_post_authorize.c:OptMgrsVelocityInt:1:2006-Sep-20 13:06:15 %";
#endif


#define FILE_LOGNAME "fm_content_pol_post_authorize.c ( % version: 1 %)"

/*******************************************************************
 * Contains the (example) PCM_OP_CONTENT_POL_POST_AUTHORIZE operation. 
 *
 * NOOP for now.
 *******************************************************************/

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
 
#include "pcm.h"
#include "ops/content.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_content.h"
#include "pinlog.h"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void op_content_pol_post_authorize();

static void
fm_content_pol_post_authorize(pcm_context_t	*ctxp, 
			int32		flags,
			pin_flist_t *i_flistp, 
			pin_flist_t **o_flistpp,
			pin_errbuf_t *ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_CONTENT_POL_POST_AUTHORIZE operation.
 *******************************************************************/
void
op_content_pol_post_authorize(
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
		"op_content_pol_post_authorize input flist", i_flistp);

	/*
	 * do the real work
	 */
	fm_content_pol_post_authorize(ctxp, flags, i_flistp, &r_flistp, ebufp);

	/*
	 * Cleanup.  If error, we return NULL output flist.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		/*
		 * Log something and return nothing.
		 */
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_content_pol_post_authorize error", ebufp);
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		*o_flistpp = (pin_flist_t *)NULL;

	} else {

		/*
		 * Point the real return flist to the right thing.
		 */
		*o_flistpp = r_flistp;

		/*
		 * Debug: What we're sending back.
		 */
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_content_pol_post_authorize return flist", r_flistp);

	}
}

/*******************************************************************
 * fm_content_pol_post_authorize():
 *******************************************************************/
static void
fm_content_pol_post_authorize(
	pcm_context_t	*ctxp,
        int32		flags,
	pin_flist_t	*i_flistp,
	pin_flist_t	**o_flistpp,
	pin_errbuf_t	*ebufp)
{
	void		*vp;
	int32		status = PIN_BOOLEAN_TRUE;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/******/
	}

	/* This policy implementation simply returns the copy of the
	 * input flist.
	 * This is a dummy implementation.
	 */

	/*
	 * Simply return the input flist
	 */
	 *o_flistpp = PIN_FLIST_CREATE(ebufp);
	 vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	 PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_POID, vp, ebufp);

	/***********************************************************************
	 * Sample code to show how to return extended data from this opcode
	 * pin_flist_t             *ex_data_flistp     = NULL;
	 * char			   name[64];
	 * char			   value[64];
	 * int32		   eid		   = 0;	
	 *
	 * ex_data_flistp = PIN_FLIST_ELEM_ADD(*o_flistpp, 
	 *				PIN_FLD_EXTENDED_DATA, eid,ebufp);
	 * pin_strlcpy(name, "Country", sizeof(name));
	 * pin_strlcpy(value, "USA"), sizeof(value);
	 * PIN_FLIST_FLD_SET(ex_data_flistp, PIN_FLD_NAME, 
	 *				(void *)name, ebufp);
	 * PIN_FLIST_FLD_SET(ex_data_flistp, PIN_FLD_VALUE, 
	 *				(void *)value, ebufp);
	 *
	 * ex_data_flistp = PIN_FLIST_ELEM_ADD(*o_flistpp, 
	 *				PIN_FLD_EXTENDED_DATA,
	 *					++eid,ebufp);
	 * pin_strlcpy(name, "ZipCode", sizeof(name));
	 * pin_strlcpy(value, "95129", sizeof(value));
	 * PIN_FLIST_FLD_SET(ex_data_flistp, PIN_FLD_NAME, 
	 *				(void *)name, ebufp);
	 * PIN_FLIST_FLD_SET(ex_data_flistp, PIN_FLD_VALUE, 
	 *				(void *)value, ebufp);
	 * 
	 * To return  Advice of charge information in the extended data
	 * array, the information in the PIN_FLD_TOTAL of the 
	 * PIN_FLD_OP_RESERVE_OUT_FIELDS substruct passed to this opcode
	 * can be used.
	 **********************************************************************/
		 
}

