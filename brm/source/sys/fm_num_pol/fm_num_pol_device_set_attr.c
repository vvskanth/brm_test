/*
 * @(#)%Portal Version: fm_num_pol_device_set_attr.c:WirelessVelocityInt:2:2006-Sep-14 11:27:35 %
 *
* Copyright (c) 2001, 2015, Oracle and/or its affiliates. All rights reserved.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_num_pol_device_set_attr.c:WirelessVelocityInt:2:2006-Sep-14 11:27:35 %";
#endif

#include <stdio.h> 
#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/num.h"
#include "pin_num.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "fm_utils.h"

#define FILE_LOGNAME "fm_num_pol_device_set_attr.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_num_pol_device_set_attr(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_num_pol_validate_device_id(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);	


/*******************************************************************************
 * Functions provided outside this source file
 ******************************************************************************/
extern void
fm_num_pol_util_device_id_exists(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp);

extern void
fm_num_pol_util_canonicalize_number(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp);


/*******************************************************************************
 * Entry routine for the PCM_OP_NUM_POL_DEVICE_SET_ATTR opcode
 ******************************************************************************/
void
op_num_pol_device_set_attr(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	void			*vp = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_NUM_POL_DEVICE_SET_ATTR) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_num_pol_device_set_attr opcode error", ebufp);

		*o_flistpp = NULL;
		return;
	}

	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_num_pol_device_set_attr input flist", i_flistp);

	/*
	 * Validate the object is of type /device/num
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

	if (strcmp((char *)PIN_POID_GET_TYPE((poid_t *)vp), 
		PIN_OBJ_TYPE_DEVICE_NUM) != 0) {

		pin_set_err(ebufp, PIN_ERRLOC_FM, 
			PIN_ERRCLASS_APPLICATION,
                        PIN_ERR_BAD_POID_TYPE, PIN_FLD_POID, 0, 0);

		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
			"fm_num_pol_device_set_attr invalid poid type");

		return;
	}

	/*
	 * Validate the device_id
	 */
	fm_num_pol_validate_device_id(ctxp, i_flistp, ebufp);

	/*
	 * Prepare the successful return flist
	 */
	*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_num_pol_device_set_attr error", ebufp);

		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_num_pol_device_set_attr return flist", *o_flistpp);
	}

	return;
}


/*******************************************************************************
 * fm_num_pol_validate_device_id()
 *
 * Inputs: flist with (optionally) new device_id
 * Outputs: void; ebuf set if errors encountered
 *
 * Description:
 * This function inspects the new device_id (if provided) and validates it
 * against a set of rules.  By default, the only change supported is changing
 * the NPA for North American numbers.  In adddition, it ensures the new
 * device_id does not already exist in the database.
 * 
 * Possible Customizations:
 * This function can be customized to support changing specific portions of the
 * device_id, such as the national destination code, etc.
 ******************************************************************************/
static void
fm_num_pol_validate_device_id(
	pcm_context_t		*ctxp,
	pin_flist_t		*i_flistp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*db_flistp = NULL;	/* for db values */
	pin_flist_t		*canon_flistp = NULL;	/* for return flist */
	char			errmsg[BUFSIZ];		/* error message */
	char			*new_device_id = NULL;	/* input device_id */
	char			*current_device_id = NULL;  /* from db copy */
	int			ndi_len = 0;		/* new id length */
	int			cdi_len = 0;		/* current id length */
	int			i = 0;			/* for looping */

	char			*north_american_cc = "001"; /* cntry code */
	int			north_american_len = 13;    /* id length */


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * If the device_id is not provided, return
	 */
	if (PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_DEVICE_ID, 1, ebufp) == NULL) {
		return;
	}

	/*
	 * Canonicalize the new device id
	 */
        fm_num_pol_util_canonicalize_number(ctxp, i_flistp, ebufp);

	/*
	 * Get the values of the number from the database
	 */
        PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, 
		i_flistp, &db_flistp, ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_num_pol_validate_device_id return flist",
		db_flistp);

	/*
	 * Get the current device_id value
	 */
	current_device_id = (char *)PIN_FLIST_FLD_GET(db_flistp,
                PIN_FLD_DEVICE_ID, 0, ebufp);

        /*
         * Get the canonicalized device id
         */
        new_device_id = (char *)PIN_FLIST_FLD_GET(i_flistp, 
		PIN_FLD_DEVICE_ID, 0, ebufp);

	if (	current_device_id == NULL ||
		new_device_id == NULL ||
		strcmp(new_device_id, current_device_id) == 0 ) {

		PIN_FLIST_DESTROY_EX(&db_flistp, NULL);
                return;
        }

	/*
	 * Prepare an error message for reporting
	 */
	pin_snprintf(errmsg, sizeof(errmsg), "fm_num_pol_validate_device_id unsupported number "
		"change: %s -> %s", current_device_id, new_device_id);

	/*
	 * The input flist contains a new device_id which is different
	 * than the existing device_id for this number.  So the new
	 * device_id will be evaluated to see if this change is
	 * supported.
	 */

	/*
	 * Grab the lengths of the id's for performing comparisons
	 */
	ndi_len = strlen(new_device_id);
	cdi_len = strlen(current_device_id);

	/*
	 * Support changing North American area-codes, or NPA.
	 *
	 * If the value starts with '001' and the length is '13',
	 * support changing the 4th, 5th, and or 6th characters.
	 * But if any other characters are being modified, log an error.
	 */
	if (strncmp(new_device_id, north_american_cc, 
		strlen(north_american_cc)) == 0) {

		/*
		 * If the length of both nums is 13 (ex '0014085723333')
		 * and the current num is also North American, continue
		 */
		if ( 	ndi_len != north_american_len || 
			cdi_len != north_american_len || 
			strncmp(current_device_id, north_american_cc, 
			strlen(north_american_cc)) != 0 ) {

			/*
			 * This number does not currently support
			 * being modified
			 */
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, errmsg);
			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_SYSTEM_DETERMINATE,
      				PIN_ERR_BAD_VALUE, 
				PIN_FLD_DEVICE_ID, 0, 0);

			PIN_FLIST_DESTROY_EX(&db_flistp, NULL);
			return;
		}

		/*
		 * If the least significant 7 characters are the same,
		 * then it must be the 4th, 5th, and/or 6th characters
		 * which have changed.  This is acceptable.
		 */
		for (i = ndi_len; i >= ndi_len - 7; --i) {

			if (new_device_id[i] != current_device_id[i]) {

				/*
				 * This number does not currently 
				 * support being modified
				 */
				PIN_ERR_LOG_MSG(
					PIN_ERR_LEVEL_ERROR, errmsg);
				pin_set_err(ebufp, PIN_ERRLOC_FM, 
					PIN_ERRCLASS_SYSTEM_DETERMINATE,
       					PIN_ERR_BAD_VALUE, 
					PIN_FLD_DEVICE_ID, 0, 0);

				PIN_FLIST_DESTROY_EX(&db_flistp, NULL);
				return;
			}
		} 

	} else {

		/*
		 * This number does not currently support being modified
		 */
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, errmsg);
		pin_set_err(ebufp, PIN_ERRLOC_FM, 
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
       			PIN_ERR_BAD_VALUE, PIN_FLD_DEVICE_ID, 0, 0);

		PIN_FLIST_DESTROY_EX(&db_flistp, NULL);
		return;
	}

	/*
	 * Check if this new device_id already exists
	 */
	fm_num_pol_util_device_id_exists(ctxp, i_flistp, ebufp);

	PIN_FLIST_DESTROY_EX(&db_flistp, NULL);
	return;
}

