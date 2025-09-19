/*
 * @(#)%Portal Version: fm_num_pol_device_create.c:WirelessVelocityInt:1:2006-Sep-14 11:27:29 %
 *
 * Copyright (c) 2001 - 2006 Oracle. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#) %full_filespec: fm_num_pol_device_create.c~7:csrc:1 %";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/num.h"
#include "pin_num.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "fm_utils.h"

#define FILE_LOGNAME "fm_num_pol_device_create.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_num_pol_device_create(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);


/*******************************************************************************
 * Functions provided outside of this source file
 ******************************************************************************/
extern void
fm_num_pol_util_device_id_exists(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp);


/*******************************************************************************
 * Entry routine for the PCM_OP_NUM_POL_DEVICE_CREATE opcode
 ******************************************************************************/
void
op_num_pol_device_create(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*dn_flistp = NULL;
	void			*vp = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_NUM_POL_DEVICE_CREATE) {

		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_num_pol_device_create opcode error", ebufp);

		*o_flistpp = NULL;
		return;
	}
	
	/*
	 * Log the input flist
         */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_num_pol_device_create input flist", i_flistp);

	/*
	 * Check if the given device_id already exists in this database.
	 * If ebuf is set, then the device_id already exists.
	 */
	fm_num_pol_util_device_id_exists(ctxp, i_flistp, ebufp);

	/*
	 * If the vanity fld was not provided on the device_num substruct,
	 * set the default.
	 */
	dn_flistp = PIN_FLIST_SUBSTR_GET(i_flistp, PIN_FLD_DEVICE_NUM,
		0, ebufp);

	vp = PIN_FLIST_FLD_GET(dn_flistp, PIN_FLD_VANITY, 1, ebufp);
	if (vp == NULL) {

		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
			"op_num_pol_device_create defaulting the vanity");

		PIN_FLIST_FLD_SET(dn_flistp, PIN_FLD_VANITY,
			(void *)PIN_NUM_VANITY_DEFAULT, ebufp);
	}

	/*
	 * Prepare the successful return flist
	 */
	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_num_pol_device_create error", ebufp);

		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_num_pol_device_create output flist", *o_flistpp);
	}

	return;
}
