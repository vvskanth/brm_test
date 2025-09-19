/*
 * @(#)%Portal Version: fm_tcf_pol_apply_parameter.c:WirelessVelocityInt:3:2006-Sep-14 11:22:46 %
 *
 * Copyright (c) 2004 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_tcf_pol_apply_parameter.c:WirelessVelocityInt:3:2006-Sep-14 11:22:46 %";
#endif

/*******************************************************************
 * Contains the PCM_OP_TELCO_POL_APPLY_PARAMETER operation.
 *******************************************************************/

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#define FILE_LOGNAME "fm_tcf_pol_apply_parameter.c(1)"

#include "ops/tcf.h"
#include "ops/gsm.h"
#include "ops/gprs_aaa.h"
#include "ops/imt.h"
#include "ops/pdc.h"
#include "pin_tcf.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_channel.h"
#include "fm_utils.h"

#define PIN_OBJ_TYPE_SERVICE_TELCO_GSM          "/service/telco/gsm"
#define PIN_OBJ_TYPE_SERVICE_TELCO_GPRS         "/service/telco/gprs"
#define PIN_OBJ_TYPE_SERVICE_TELCO_IMT          "/service/telco/imt"
#define PIN_OBJ_TYPE_SERVICE_TELCO_PDC          "/service/telco/pdc"

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_tcf_pol_apply_parameter(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_tcf_pol_apply_parameter(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);


/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/

/*******************************************************************
 * Main routine for the PCM_OP_TELCO_POL_APPLY_PARAMETER operation.
 *******************************************************************/
void
op_tcf_pol_apply_parameter(
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
	if (opcode != PCM_OP_TELCO_POL_APPLY_PARAMETER) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_tcf_pol_apply_parameter opcode error", ebufp);

		return;
	}
	
	/***********************************************************
	 * Debut what we got.
         ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_tcf_pol_apply_parameter input flist", i_flistp);

	/***********************************************************
	 * Main rountine for this opcode
	 ***********************************************************/
	fm_tcf_pol_apply_parameter(ctxp, flags, i_flistp, r_flistpp, ebufp);
	
	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(r_flistpp, ebufp);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_tcf_pol_apply_parameter error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_tcf_pol_apply_parameter output flist", *r_flistpp);
	}

	return;
}

/*******************************************************************
 * fm_tcf_pol_apply_parameter:
 *
 *******************************************************************/
static void
fm_tcf_pol_apply_parameter(
        pcm_context_t           *ctxp,
        int32                   flags,
        pin_flist_t             *i_flistp,
        pin_flist_t             **r_flistpp,
        pin_errbuf_t            *ebufp)
{

        pin_flist_t             *svc_info_flistp = NULL;
        pin_cookie_t            cookie = NULL;
        int32                   elem_id = 0;

        poid_t                  *svc_poid = NULL;
        char                    *svc_type = NULL;

        if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
        PIN_ERR_CLEAR_ERR(ebufp);

        svc_info_flistp = PIN_FLIST_ELEM_GET_NEXT (i_flistp, PIN_FLD_SERVICES, &elem_id, 0, &cookie, ebufp);
        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_tcf_pol_apply_parameter error getting services flist", ebufp);
        }

        svc_poid = PIN_FLIST_FLD_GET (svc_info_flistp, PIN_FLD_POID, 0, ebufp);
	if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_tcf_pol_apply_parameter error service object poid", ebufp);
		return;
        }

        svc_type = (char *) PIN_POID_GET_TYPE (svc_poid);
        if(svc_type && strstr(svc_type, PIN_OBJ_TYPE_SERVICE_TELCO_GSM) == svc_type)
        {
		PCM_OP(ctxp,PCM_OP_GSM_APPLY_PARAMETER, flags,
			i_flistp, r_flistpp, ebufp);

	}
	/*
	 * As we dont have the Standrd opcodes for IMT and PDC we directly calling policy opcodes
	 */
        else if(svc_type && strstr(svc_type,PIN_OBJ_TYPE_SERVICE_TELCO_PDC) == svc_type) {
		PCM_OP(ctxp,PCM_OP_PDC_POL_APPLY_PARAMETER, flags,
			i_flistp, r_flistpp, ebufp);

	}

        else if(svc_type && strstr(svc_type,PIN_OBJ_TYPE_SERVICE_TELCO_IMT) == svc_type) {
		PCM_OP(ctxp,PCM_OP_IMT_POL_APPLY_PARAMETER, flags,
			i_flistp, r_flistpp, ebufp);

	}

        else if(svc_type && strstr(svc_type,PIN_OBJ_TYPE_SERVICE_TELCO_GPRS) == svc_type) {

		PCM_OP(ctxp,PCM_OP_GPRS_APPLY_PARAMETER, flags,
			i_flistp, r_flistpp, ebufp);
	}
	else{
                /*
                 * Only service telco data. No vertical manager specific parameters.
                 * Send input back as output.
                 */
                *r_flistpp = PIN_FLIST_COPY (i_flistp, ebufp);

        }

        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_tcf_pol_apply_parameter error", ebufp);
        }
        return;
}
