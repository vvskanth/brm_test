 /* (#)$Id: fm_content_pol_accounting.c st_cgbubrm_gsivakum_bug-8542127/1 2009/05/26 00:52:47 gsivakum Exp $  
 *
 *
 * Copyright (c) 2001, 2024, Oracle and/or its affiliates. 
 *
 * This material is the confidential property of Oracle Corporation or
 * its licensors and may be used, reproduced, stored or transmitted only
 * in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static  char    Sccs_id[] = "(#)$Id: fm_content_pol_accounting.c st_cgbubrm_gsivakum_bug-8542127/1 2009/05/26 00:52:47 gsivakum Exp $";
#endif

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include "ops/content.h"
#include <pin_act.h>
#include <cm_fm.h>
#include <pin_errs.h>
#include <pinlog.h>
#include "pin_wireless.h"
#include "pin_content.h"
#include "fm_utils.h"
#include "pin_currency.h"
#include "pin_rate.h"

#define FILE_LOGNAME "fm_content_pol_accounting.c(1)"
#define PRE_RATED_MODE 0 
#define	RATING_MODE 1

/*******************************************************************
 * Contains the PCM_OP_CONTENT_POL_ACCOUNTING operation. This opcode is
 * called by fm_content_accounting before creating the activity
 *
 * Policy opcode will :
 * 1.   check if the given record is a duplicate record.
 * 2.	check if the given content category is a valid
 * 	category for the user.
 *
 *******************************************************************/

/*******************************************************************
 * Routines contained within.
 *******************************************************************/
static int 
fm_content_pol_check_for_duplicates(
	pcm_context_t	*ctxp,
	int32		opflags,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp
);

/*******************************************************************
 * Main routine for the PCM_OP_CONTENT_POL_ACCOUNTING operation.
 *******************************************************************/
EXPORT_OP void
op_content_pol_accounting(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			opflags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp);

void
op_content_pol_accounting(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			opflags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t	        *ctxp = connp->dm_ctx;
	int     		*use_srvc_profiles_valp;
	int			*content_skip_rating_mode_valp = NULL;
	static int32    	use_srvc_profiles, initdone = PIN_BOOLEAN_FALSE;
	static int32		rating_mode = PRE_RATED_MODE;
	int32    		status = PIN_CONTENT_STATUS_OK;
	int32    		reason,is_duplicate,mode,validCat;
	int32			*currency_valp = NULL;
	static int32            currency1 = 0;
	int32			impact_type = PIN_IMPACT_TYPE_PRERATED;
	int32			err  = PIN_ERR_NONE;
	void			*vp = NULL;
	pin_flist_t		*flistp = NULL;
	pin_flist_t		*bal_impact_flistp = NULL;
	char                    buf[PCM_MAXIMUM_STR] = "";
	char                    *amt = NULL;
	poid_t                  *acc_poidp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_CONTENT_POL_ACCOUNTING) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_content_pol_accounting opcode error", ebufp);
		return;
	}

	/*
	 * Debug what we got
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_content_pol_accounting input flist", i_flistp);


	/* default value for return flist */
	*o_flistpp = PIN_FLIST_CREATE(ebufp);
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_POID, vp, ebufp);


	/* check if profiles are used in this installation */
	/*
	* Attempt to read Content Use Profiles flag from pin.conf
	*/

	/* Have we already read the pin.conf? */
	if (!initdone) {
		/* No: try & grab the value for user_srvc_profiles */
		pin_conf("-", "content_use_profiles", PIN_FLDT_INT,
					(caddr_t *)&use_srvc_profiles_valp, &err);
		if (err == PIN_ERR_NONE) {

			use_srvc_profiles = *use_srvc_profiles_valp;
			PIN_FREE_EX(&use_srvc_profiles_valp);
			pin_snprintf(buf, sizeof(buf), "use_srvc_profiles set to %d\n", use_srvc_profiles);
				 PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, buf);
		} else {
			/* Set default */
			use_srvc_profiles = PIN_BOOLEAN_FALSE;
		}



		/* Read the system currency from pin.conf */
		
		err = PIN_ERR_NONE;

		pin_conf("fm_cust_pol", "currency", PIN_FLDT_INT,
                                        (caddr_t *)&currency_valp, &err);
		if (err == PIN_ERR_NONE) {

                        currency1 = *currency_valp;
			PIN_FREE_EX(&currency_valp);
                        pin_snprintf(buf, sizeof(buf), "System Currency is set to %d\n", currency1);
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, buf);
                } else {
                        /* Set default */
                        currency1 = PIN_CURRENCY_USD;
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
				"Assuming Default currency to be 840");
                }
	
		

		/* Read the value of mode (if pre-rated or rating mode ) */

		err = PIN_ERR_NONE;
		
		pin_conf("-", "content_prerated_mode", PIN_FLDT_INT,
					(caddr_t *)&content_skip_rating_mode_valp, &err);
		if (err == PIN_ERR_NONE) {
			rating_mode = *content_skip_rating_mode_valp;
			PIN_FREE_EX(&content_skip_rating_mode_valp);
	

			if (rating_mode == PRE_RATED_MODE || rating_mode == RATING_MODE){

				pin_snprintf(buf, sizeof(buf), " Mode is set to %d\n", rating_mode);
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, buf);
	
			} else {

				pin_set_err(ebufp, PIN_ERRLOC_APP,
					PIN_ERRCLASS_SYSTEM_DETERMINATE,
						PIN_ERR_INVALID_CONF, 0, 0, err);
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"Invalid Value in pin_conf for content_prerated_mode,"
					" should be either 0 or 1", ebufp);
				pin_snprintf(buf, sizeof(buf), " Mode is set to %d\n", rating_mode);
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, buf);
				goto finish;

			}
					
		} else {
			/* Set default mode to Pre_rated mode */

			rating_mode = PRE_RATED_MODE;
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
				"Defaulting to PRERATED mode");
		}
	
		initdone = PIN_BOOLEAN_TRUE;
		
	}

	/* Check if currentrecord is a duplicate or not. */
	is_duplicate = fm_content_pol_check_for_duplicates(ctxp, opflags,
								i_flistp, ebufp);
	if (is_duplicate) {
		status = PIN_CONTENT_STATUS_NOK;
		PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_STATUS, &status, ebufp);
 		reason = PIN_CONTENT_DUPLICATE_RECORD;
		PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_REASON, &reason, ebufp);
		goto finish;
	}
	if (use_srvc_profiles == PIN_BOOLEAN_FALSE) {
		status = PIN_CONTENT_STATUS_OK;
		PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_STATUS, &status, ebufp);
	} else {
		/*
		 * Put the serivce obj POID and CONTENT_CATEGORY_NAME 
		 */
		flistp = PIN_FLIST_CREATE(ebufp);
		vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, vp, ebufp);
		vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_CONTENT_CATEGORY_NAME, 0, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_CONTENT_CATEGORY_NAME, vp, ebufp);
	
		mode = PIN_WIRELESS_MODE_ALLOWED; 
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_MODE, &mode, ebufp);
	
		/*
		 * Call validate content category with the flist created with the
		 * service poid, the requested content category,  mode (allow/deny), 
		 * and a pointer to a compare function.
		 * This function returns PIN_BOOLEAN_TRUE or PIN_BOOLEAN_FALSE
		 * depending on whether a user is allowed to access the requested
		 * content category  or not.
		 */
	
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_content_pol_accounting validate cat input flist", flistp);
		validCat = fm_utils_content_validate_content_cat(ctxp, flistp, opflags, mode, 
					&fm_utils_content_pol_compare_content_cat_names, ebufp);
	
		PIN_FLIST_DESTROY_EX(&flistp, NULL);
		if (!validCat) {
			status = PIN_CONTENT_STATUS_NOK;
			PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_STATUS, &status, ebufp);
 			reason = PIN_CONTENT_DENIED_ACCESS_TO_CONTENT_CATEGORY;
			PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_REASON, &reason, ebufp);
			goto finish;
		} else {
			status = PIN_CONTENT_STATUS_OK;
			PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_STATUS, &status, ebufp);
		}
	}

	/* By default accounting mode will be Pre-rated .In this case,prepare BAL_IMPACTS
	 * array (For Amount based only) and pass on to the o/p flist of accounting policy for 
	 * the standard opcode to call op_act_usage with Pre-rated mode. */  

	amt = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_AMOUNT, 1, ebufp);

	if (rating_mode == PRE_RATED_MODE && amt != NULL ){


		bal_impact_flistp = PIN_FLIST_ELEM_ADD(*o_flistpp,
			PIN_FLD_BAL_IMPACTS, 0, ebufp);

		acc_poidp =  PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACCOUNT_OBJ,
				0, ebufp);

		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_content_pol_accounting_flist: missing "
					"reqd field PIN_FLD_ACCOUNT_OBJ", ebufp);
			goto finish;
		}


		/* Here we can have the currency hardcoded ,as this is a mandatory
		 * parameter or alternatively , system currency can be read from cm's pin.conf .*/ 


		PIN_FLIST_FLD_SET(bal_impact_flistp, PIN_FLD_AMOUNT,
			amt, ebufp);

		PIN_FLIST_FLD_SET(bal_impact_flistp, PIN_FLD_ACCOUNT_OBJ,
			acc_poidp, ebufp);

		PIN_FLIST_FLD_SET(bal_impact_flistp, PIN_FLD_IMPACT_TYPE,
			&impact_type, ebufp);

		PIN_FLIST_FLD_SET(bal_impact_flistp, PIN_FLD_RESOURCE_ID,
			&currency1, ebufp);
	}

		

	/*****************************************************************************
	 * Set the field values here to override the
	 * default values used  by the std opcode to calculate the
	 * cost. 
	 *
	 * NOTE: The only fields that can be overridden are:
	 * PIN_FLD_CONTENT_PROVIDER 
	 * PIN_FLD_CONTENT_CATEGORY_NAME 
	 * PIN_FLD_DESCR 
	 * PIN_FLD_QUANTITY 
	 * PIN_FLD_AMOUNT 
	 * PIN_FLD_OBJ_TYPE 
	 * PIN_FLD_EXTENDED_INFO 
	 *
	 * pin_flist_t         *e_flistp = NULL;
	 * pin_flist_t         *c_flistp = NULL;
	 * char                providerid[128];
	 * char                categoryName[128];
	 * char                categorydescr[128];
	 * char                *qty = NULL;
	 * char                objtype[64];
	 * pin_decimal_t       *newqtyp;
	 *
	 * pin_strlcpy(providerid, "excite", sizeof(providerid));
	 * pin_strlcpy(categoryName, "Weather", sizeof(categoryName));
	 * pin_strlcpy(categorydescr, "Weather from Policy", sizeof(categorydescr));
	 * pin_strlcpy(objtype, "extended", sizeof(objtype));
	 * PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_CONTENT_PROVIDER,
				  (void *)providerid, ebufp);
	 * PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_CONTENT_CATEGORY_NAME,
				  (void *)categoryName, ebufp);
	 * PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_DESCR,
				  (void *)categorydescr, ebufp);
	 * newqtyp = pin_decimal("2", ebufp);
	 * PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_QUANTITY,
					 (void *)newqtyp, ebufp);
	 * pin_decimal_destroy (newqtyp);
	 * PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_QUANTITY,
				  (void*)newqtyp, ebufp);
	 * PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_OBJ_TYPE,
				  (void *)objtype, ebufp);
	 *  e_flistp = PIN_FLIST_SUBSTR_ADD(*o_flistpp, PIN_FLD_EXTENDED_INFO, ebufp);
	 * c_flistp = PIN_FLIST_SUBSTR_ADD(e_flistp, PIN_FLD_CHARGE, ebufp);
	 *  status  = PIN_CONTENT_STATUS_OK;
	 * PIN_FLIST_FLD_SET(c_flistp, PIN_FLD_STATUS,
				  &status, ebufp);
	*****************************************************************************/
	 


finish:
	/*
	 * ERRORS
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {

		/*
		 * Log something and return nothing
		 */
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "op_content_pol_accounting error", ebufp);

	} else {

		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			  "op_content_pol_accounting return flist", *o_flistpp);
	}

	if (flistp) {
		PIN_FLIST_DESTROY_EX(&flistp, NULL);
	}
	return;
}

/*******************************************************************
 * fm_content_pol_check_for_duplicates
 *
 * get the service poid, cp id, start time, end time, quantity and
 * search the event table to see if an entry exists with the same
 * values. 
 *
 * input:
 *   i_flistp: opcode input flist (containing all the above info)
 *
 * output:
 *   ebufp: error if pb while cheking the status of the URL
 *   return: Duplicate record or not 
 *
 *******************************************************************/
static int 
fm_content_pol_check_for_duplicates(
	pcm_context_t	*ctxp,
	int32		opflags,
	pin_flist_t	*i_flistp,
	pin_errbuf_t	*ebufp
)
{
	pin_flist_t         	*flistp = NULL;
	pin_flist_t     	*search_flistp  = NULL;
	pin_flist_t     	*arg_flistp     = NULL;
	pin_flist_t     	*arg_sub_flistp     = NULL;
	pin_flist_t     	*s_flistp     = NULL;
	pin_flist_t     	*r_flistp     = NULL;
	pin_flist_t             *f_flistp = NULL;
	poid_t          	*search_pdp     = NULL;
	poid_t          	*srvc_pdp = NULL;
	/*int32         		s_flags = SRCH_DISTINCT;*/
	int32         		s_flags = 0;
	int64           	db_id       = 0;
	void			*vp;
	void			*end_t;
	int     		duplicate = PIN_BOOLEAN_TRUE;
	int     		*content_detect_duplicates_valp;
	static int32    	content_detect_duplicates, initdone = PIN_BOOLEAN_FALSE;
	char            	*template;

	

	if (PIN_ERR_IS_ERR(ebufp)) {
		return (PIN_BOOLEAN_FALSE);
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/* What did we get? */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
	  "fm_content_pol_check_for_duplicates input flist", i_flistp);

	/* check if profiles are used in this installation */
	/*
	* Attempt to read Use srvc Profiles flag from pin.conf
	*/

	/* Have we already read the pin.conf? */
	if (!initdone) {
		/* No: try & grab the value for content_detect_duplicates */
		int32   err;
		pin_conf("-", "content_detect_duplicates", PIN_FLDT_INT,
			(caddr_t *)&content_detect_duplicates_valp, &err);
		if (err == PIN_ERR_NONE) {
			char    buf[512];

		content_detect_duplicates = *content_detect_duplicates_valp;
			free(content_detect_duplicates_valp);
			pin_snprintf(buf, sizeof(buf), "content_detect_duplicates set to %d\n", 
				 content_detect_duplicates);
				 PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, buf);
		} else {
			/* Set default */
			content_detect_duplicates= PIN_BOOLEAN_TRUE;
		}

		initdone = PIN_BOOLEAN_TRUE;
	}

	if (content_detect_duplicates == PIN_BOOLEAN_FALSE) {
		return (PIN_BOOLEAN_FALSE);
	}

	/* create a serach flist */
	srvc_pdp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID,
					    0, ebufp);
	search_flistp = PIN_FLIST_CREATE(ebufp);

	/*********************************************************************
	* Create a 6 arg dynamic search template for event_act_content_t table.
	**********************************************************************/
	db_id = pin_poid_get_db(srvc_pdp);
	search_pdp = PIN_POID_CREATE(db_id, "/search",
					0, ebufp);
  	PIN_FLIST_FLD_PUT(search_flistp, PIN_FLD_POID,
			search_pdp,ebufp);
	PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_FLAGS,
			(void *)&s_flags, ebufp);

	template =    "select X from /event/activity/content "
	 "where  F1 = V1 and F2 = V2 and F3 = V3 and F4 = V4 and F5 = V5 and F6 = V6 ";

	 PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_TEMPLATE,
				 (void *)template, ebufp);

	/*
	 * Set up the search args
	 */
	 arg_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS,
					 1,ebufp);
	 PIN_FLIST_FLD_SET(arg_flistp, PIN_FLD_SERVICE_OBJ,
				 srvc_pdp,  ebufp);

	 arg_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS,
					 2,ebufp);
	 arg_sub_flistp = PIN_FLIST_ELEM_ADD(arg_flistp, PIN_FLD_CONTENT_INFO,
					 0,ebufp);
	 vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_CONTENT_PROVIDER, 0, ebufp);
	 PIN_FLIST_FLD_SET(arg_sub_flistp, PIN_FLD_CONTENT_PROVIDER, vp, ebufp);

	 arg_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS,
					 3,ebufp);
	 end_t = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_END_T, 0, ebufp);
	 PIN_FLIST_FLD_SET(arg_flistp, PIN_FLD_END_T, end_t, ebufp);

	 arg_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS,
					 4,ebufp);
	 vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_START_T, 1, ebufp);
	 if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(arg_flistp, PIN_FLD_START_T, vp, ebufp);
	 } else {
		PIN_FLIST_FLD_SET(arg_flistp, PIN_FLD_START_T, end_t, ebufp);
         }

	 arg_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS,
					 5,ebufp);
	 arg_sub_flistp = PIN_FLIST_ELEM_ADD(arg_flistp, PIN_FLD_CONTENT_INFO,
					 0,ebufp);
	 vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_CONTENT_CATEGORY_NAME, 0, ebufp);
	 PIN_FLIST_FLD_SET(arg_sub_flistp, PIN_FLD_CONTENT_CATEGORY_NAME, vp, ebufp);

	 arg_flistp = PIN_FLIST_ELEM_ADD(search_flistp, PIN_FLD_ARGS,
					 6,ebufp);
	 arg_sub_flistp = PIN_FLIST_ELEM_ADD(arg_flistp, PIN_FLD_CONTENT_INFO,
					 0,ebufp);
	 vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_QUANTITY, 0, ebufp);
	 PIN_FLIST_FLD_SET(arg_sub_flistp, PIN_FLD_QUANTITY, vp, ebufp);


	/*
	 * Construct the result array. We are only interested in
	 * the poid of the event object that was found.
	 */
	 s_flistp = PIN_FLIST_ELEM_ADD(search_flistp,
				   PIN_FLD_RESULTS, 0,ebufp);
	
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_POID, NULL, ebufp);


	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
	  "fm_content_pol_check_for_duplicates search flist", search_flistp);
	/*
	 * Do the search
	 */
	PCM_OP(ctxp, PCM_OP_SEARCH, 0, search_flistp,
				&r_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {

		/*
		 * Log something and return nothing
		 */
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "Search returned Error...........", ebufp);

	} 
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
	  "fm_content_pol_check_for_duplicates search Return flist", r_flistp);

	if (r_flistp != NULL) {
	
		f_flistp = PIN_FLIST_ELEM_GET(r_flistp, PIN_FLD_RESULTS,
						PIN_ELEMID_ANY, 1, ebufp);

		if (f_flistp == NULL) {
			duplicate = PIN_BOOLEAN_FALSE;
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
		  "fm_content_pol_check_for_duplicates returning false\n");
		}
	}
	if (PIN_ERR_IS_ERR(ebufp)) {

		/*
		 * Log something and return nothing
		 */
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "Error...........", ebufp);

	} 


	/* cleanup */
	PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	return (duplicate);
}

