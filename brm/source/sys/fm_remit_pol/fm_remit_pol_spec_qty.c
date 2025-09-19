/*
 *      Copyright (c) 2000 - 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)%Portal Version: fm_remit_pol_spec_qty.c:BillingVelocityInt:2:2006-Sep-08 04:11:21 %";
#endif

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "pcm.h"
#include "ops/remit.h"
#include "pinlog.h"
#include "pin_flds.h"
#include "cm_fm.h"

#define FILE_LOGNAME "fm_remit_pol_spec_qty.c(2)"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_remit_pol_spec_qty(
        cm_nap_connection_t	*connp,
	int32			opcode,
        int32			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**r_flistpp,
        pin_errbuf_t		*ebufp);


/*******************************************************************
 * Main routine for the PCM_OP_REMIT_POL_SPEC_QTY operation.
 *******************************************************************/
void
op_remit_pol_spec_qty(
        cm_nap_connection_t	*connp,
	int32			opcode,
        int32			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**r_flistpp,
        pin_errbuf_t		*ebufp)
{
        void                    *vp = NULL;
        pin_flist_t             *flistp = NULL;
        int                     rec_id;
        pin_cookie_t            cookie = NULL;
        int                     bal_impact_elem_id;
        
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);
        
	/*******************************************************************
	 * Insanity check.
 	 *******************************************************************/
	if (opcode != PCM_OP_REMIT_POL_SPEC_QTY) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_remit_pol_spec_qty bad opcode error", ebufp);
		return;
	}

	/*******************************************************************
	 * Debug: What we got.
 	 *******************************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_remit_pol_spec_qty input flist", i_flistp);
        
        /*
         * initialize return flist with the quantity of current processed
         * balance impact
         */
        *r_flistpp = PIN_FLIST_CREATE(ebufp);
        vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
        PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, vp, ebufp);
        flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_ARGS, &rec_id, 0,
            &cookie, ebufp);
	bal_impact_elem_id = -1;

	if (flistp != NULL) {
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_ELEMENT_ID, 0, ebufp);
		if (vp) {
			bal_impact_elem_id = *((int *)vp);
		}
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                    "op_remit_pol_spec_qty error", ebufp);
	}

	if( bal_impact_elem_id != -1 )
	{
		if( (flistp = PIN_FLIST_ELEM_GET(i_flistp, PIN_FLD_BAL_IMPACTS,
		            bal_impact_elem_id, 0, ebufp)) != NULL )
		{
        		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_QUANTITY, 0, ebufp);
        		PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_QUANTITY, vp, ebufp);
		}
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                    "op_remit_pol_spec_qty error", ebufp);
	} else {
                PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                    "op_remit_pol_spec_qty return flist", *r_flistpp);
	}
}
