/*
 * @(#)% %
 *
 * Copyright (c) 2001 - 2024 Oracle.
 *
 * This material is the confidential property of Oracle Corporation or
 * its licensors and may be used, reproduced, stored or transmitted only
 * in accordance with a valid Oracle license or sublicense agreement.
 *   
 ********************************************************************/ 
#ifndef lint 
static char Sccs_id[] = 
	"@(#)% %"; 
#endif 
/*******************************************************************  
* Contains the (example) PCM_OP_CONTENT_POL_AUTHORIZE operation.  *  
* 
*******************************************************************/ 
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>   
#include "pcm.h" 
#include "pin_act.h" 
#include "cm_fm.h" 
#include "pin_errs.h" 
#include "pin_content.h" 
#include "pin_currency.h" 
#include "pin_wireless.h" 
#include "pinlog.h" 
#include "fm_utils.h"

#define FILE_SOURCE_ID	"fm_content_pol_authorize.c( % version 7 %)" 

#define ONEDAYINSECS	60*60*24

/*******************************************************************  
* Routines needed from elsewhere.  
*******************************************************************/ 

extern void fm_content_util_find_maximum(); 

/*******************************************************************  
* Routines 
 contained within.  
*******************************************************************/ 
EXPORT_OP void op_content_pol_authorize(); 

static void fm_content_pol_authorize(pcm_context_t	*ctxp, 
				    int32		flags, 
				    pin_flist_t *i_flistp, 
				    pin_flist_t **o_flistpp,			 
				    pin_errbuf_t *ebufp); 
/*******************************************************************  
* Main routine for the PCM_OP_CONTENT_POL_AUTHORIZE operation.  
*******************************************************************/ 
void op_content_pol_authorize(        
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
		"op_content_pol_authorize input flist", i_flistp);	 
	/*	 
	* do the real work	 
	*/	 

	fm_content_pol_authorize(ctxp, flags, i_flistp, &r_flistp, ebufp);	 
	
	/*	 
	* Cleanup.  If error, we return NULL output flist.	 
	*/	 
	
	if (PIN_ERR_IS_ERR(ebufp)) {		 
		/*		 
		* Log something and return nothing.		 
		*/

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,			 
			"op_content_authorize error: ", ebufp);		 
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
		"op_content_pol_authorize return flist", r_flistp);	 
	} 
} 
 
/*******************************************************************  
* fm_content_pol_authorize():  
*******************************************************************/ 
static void fm_content_pol_authorize(	
			pcm_context_t	*ctxp,	 
			int32		flags,	 
			pin_flist_t	*i_flistp,	 
			pin_flist_t	**o_flistpp,	 
			pin_errbuf_t	*ebufp) 
{
	pin_flist_t	*r_flistp = NULL;   /* return flist ptr */	 
	void		*vp = NULL;	 
	pin_flist_t	*flistp = NULL;	 
	int32		reason, status, currency;	 
	int32		mode;	
	int32		validCat =  PIN_BOOLEAN_FALSE;	 
	int32		hasProfiles = PIN_BOOLEAN_FALSE; 
	static int32    use_srvc_profiles, initdone= PIN_BOOLEAN_FALSE; 
	int     	*use_srvc_profiles_valp; 
	char    	buf[512];
	time_t          exp_time = 0;

	/* sanity check */ 
	if (PIN_ERR_IS_ERR(ebufp)) {		 
		return;		 
		/******/	 
	}	 
	/*	  
	 * Initialize the return flist	  
	 */	

	r_flistp = PIN_FLIST_CREATE(ebufp); 
	
	*o_flistpp = r_flistp; 
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp); 
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_POID, vp, ebufp); 
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACCOUNT_OBJ, 0, ebufp); 
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_ACCOUNT_OBJ, vp, ebufp); 
	 
	/* Read the configuration entry(content_use_profiles) from the cm's 
	 * pin.conf file 
	 */ 
	/* Have we already read the pin.conf? */ 
	if (!initdone) { 
		/* No: try & grab the value for user_srvc_profiles */ 
		int32   err; 
		pin_conf("-", "content_use_profiles", PIN_FLDT_INT, 
			(caddr_t *)&use_srvc_profiles_valp, &err); 
		if (err == PIN_ERR_NONE) { 
			use_srvc_profiles = *use_srvc_profiles_valp; 
			free(use_srvc_profiles_valp); 
			pin_snprintf(buf, sizeof(buf), "use_srvc_profiles set to %d\n", 
				use_srvc_profiles); 
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, buf); 
		} else { 
			/* Set default */ 
			use_srvc_profiles = PIN_BOOLEAN_FALSE; 
		} 
		initdone = PIN_BOOLEAN_TRUE; 
	}
	if (use_srvc_profiles == PIN_BOOLEAN_FALSE) { 
		status = PIN_CONTENT_STATUS_OK; 
		PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_STATUS, &status, ebufp);
	} else { 
		/* 
		 * Put the serivce obj POID and CONTENT_CATEGORY_NAME  
		 */ 
		flistp = PIN_FLIST_CREATE(ebufp); 
		vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp); 
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, vp, ebufp); 
		vp = PIN_FLIST_FLD_GET(i_flistp,  
			PIN_FLD_CONTENT_CATEGORY_NAME, 0, ebufp); 
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_CONTENT_CATEGORY_NAME,  
			vp, ebufp); 
		mode = PIN_WIRELESS_MODE_ALLOWED;  
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_MODE, &mode, ebufp); 
		
		/* 
		 * Call validate content category with the flist created 
		 * with the service poid, the requested content category, 
		 * mode (allow/deny), and a pointer to a compare function. 
		 * This function returns PIN_BOOLEAN_TRUE or 
		 * PIN_BOOLEAN_FALSE depending on whether a user is 
		 * allowed to access the requested content category or not. 
		 */ 
	
		validCat = fm_utils_content_validate_content_cat(ctxp, 
			flistp, flags, mode,  
			&fm_utils_content_pol_compare_content_cat_names, 
			ebufp);

		PIN_FLIST_DESTROY_EX(&flistp, NULL); 
		
		if (validCat == PIN_BOOLEAN_FALSE) { 
			status = PIN_CONTENT_STATUS_NOK; 
			PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_STATUS,  
				&status, ebufp);
			if (PIN_ERR_IS_ERR(ebufp)) {
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"Problem in setting Status", ebufp);
				return;
			}
			/*
			 * set reason and if verification is required set it
			 * in the return flist
			 */

 			reason = PIN_CONTENT_DENIED_ACCESS_TO_CONTENT_CATEGORY;
			PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_REASON,  
				&reason, ebufp);
			return; 
		} else {
			/* else set the status to OK */
			status = PIN_CONTENT_STATUS_OK;
			PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_STATUS, 
					  &status, ebufp);
		}
		
	}

	/* Add reservation defaults if necessary */

	/* Set the expiration time 
	 * The interval passed in is ignored by default.
	 * The Service Provider controls the expiration time in
	 * most cases. The input has the extended data for special cases,
	 * where the Service Provider may use the interval in the extended 
	 * data to determine the extime.
	 */
	exp_time = pin_virtual_time((time_t *)NULL) + ONEDAYINSECS;
	vp       = (void *)&exp_time;

	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_EXPIRATION_T, vp, ebufp);

	/* Set the min qty
	 * 
	 * If the input does not have a minimum qty then the
	 * requested qty is the minimum. Hence the policy does not
	 * return a minimum.
	 */
	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_MINIMUM, 1, ebufp); 
	if (vp != NULL) {
		PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_MIN_QUANTITY, vp, ebufp);
	}

	/* set the default system currency
	 * This is hard coded but can be read from the cm's configuration
	 * if it is the same for all user's in the system or from the
	 * user's account etc.
	 */
	currency = PIN_CURRENCY_USD;
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_CURRENCY, &currency, ebufp);


}
