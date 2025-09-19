/*
 * @(#)%Portal Version: fm_content_pol_post_accounting.c:OptMgrsVelocityInt:1:2006-Sep-20 13:06:12 %
 *
 * Copyright (c) 2002 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or
 * its licensors and may be used, reproduced, stored or transmitted only
 * in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_content_pol_post_accounting.c:OptMgrsVelocityInt:1:2006-Sep-20 13:06:12 %";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>
#include "ops/content.h"
#include "cm_fm.h"
#include "fm_utils.h"
#include "pin_errs.h"
#include "pin_decimal.h"
#include "pin_content.h"

#define FILE_LOGNAME "fm_content_pol_post_accounting.c(1)"

/*******************************************************************
 * Contains the (example) PCM_OP_CONTENT_POL_POST_ACCOUNTING operation. 
 *
 * NOOP for now.
 *******************************************************************/
 

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void op_content_pol_post_accounting();

static void
fm_content_pol_post_accounting(pcm_context_t	*ctxp, 
			int32		flags,
			pin_flist_t *i_flistp, 
			pin_flist_t **o_flistpp,
			pin_errbuf_t *ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_CONTENT_POL_POST_ACCOUNTING operation.
 *******************************************************************/
void
op_content_pol_post_accounting(
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
		"op_content_pol_post_accounting input flist", i_flistp);

	/*
	 * do the real work
	 */
	fm_content_pol_post_accounting(ctxp, flags, i_flistp, &r_flistp, ebufp);

	/*
	 * Cleanup.  If error, we return NULL output flist.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		/*
		 * Log something and return nothing.
		 */
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_content_pol_post_accounting error", ebufp);
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
			"op_content_pol_post_accounting return flist", r_flistp);

	}
}

/*******************************************************************
 * fm_content_pol_post_accounting():
 *******************************************************************/
static void
fm_content_pol_post_accounting(
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

	/* This policy implementation simply returns 
	 * the poid from the input flist.
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
	 *
	 * To return  Advice of charge information in the extended data
	 * array, the information in the PIN_FLD_TOTAL of the 
	 * PIN_FLD_OP_OUT_FIELDS substruct passed to this opcode
	 * can be used.
	 *
	 * pin_flist_t             *ex_data_flistp     = NULL;
	 * pin_flist_t             *out_flistp     = NULL;
	 * pin_flist_t             *temp_flistp     = NULL;
	 * pin_flist_t             *result_flp     = NULL;
	 * char			   resource_name[64];
	 * pin_decimal_t	   *dec_amountp;
	 * char	   		   *amountp;
	 * int32		   eid		   = 0;	
	 * int32		   eid2		   = 0;	
	 * pin_cookie_t	           cookie          = NULL:
	 *
	 * 
	 * return the TOTAL array for advice-of-charge use 
	 * out_flistp = PIN_FLIST_SUBSTR_GET (i_flistp, 
	 * PIN_FLD_OP_OUT_FIELDS, 0, ebufp);
	 * result_flp  = PIN_FLIST_SUBSTR_GET(
         *              out_flistp, PIN_FLD_RESULTS, 0, ebufp);
	 *
	 * eid2 = 0;
         * while ((temp_flistp = PIN_FLIST_ELEM_GET_NEXT(result_flp,
         *        PIN_FLD_TOTAL, &eid, 1, &cookie, ebufp)) != NULL) {
	 *
	 * ex_data_flistp = PIN_FLIST_ELEM_ADD(*o_flistpp,
         *                              PIN_FLD_EXTENDED_DATA, eid2++,
	 *				ebufp);

	 *  Add the resource id 
	 * fm_utils_beid_name(eid, resource_name, 64);
	 * PIN_FLIST_FLD_SET(ex_data_flistp, PIN_FLD_NAME,
         *                           (void *)resource_name, ebufp);
         * dec_amountp = (pin_decimal_t *)PIN_FLIST_FLD_GET(temp_flistp, 
	 *			PIN_FLD_AMOUNT, 
	 *		        0, ebufp);
	 * amountp = pbo_decimal_to_str(dec_amountp, ebufp);
	 * PIN_FLIST_FLD_SET(ex_data_flistp, PIN_FLD_VALUE,
         *                   		(void *)amountp, ebufp);
         * }
	 **********************************************************************/
}

