/*******************************************************************
 *
* Copyright (c) 2003, 2023, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char Sccs_Id[] = "@#$Id: fm_cust_pol_prep_acctinfo.c /cgbubrm_mainbrm.portalbase/1 2023/06/13 12:55:36 ampoonia Exp $"; 
#endif

#include <stdio.h>
#include <time.h>
#include "pcm.h"
#include "ops/cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_cust.h"
#include "pin_bill.h"
#include "pinlog.h"
#include "fm_utils.h"
#include "pin_currency.h"
#include "psiu_currency.h"
#include "pin_os.h"

#define MAX_SEGMENT_SIZE 255
/*******************************************************************
 * Routines contained within.
 *******************************************************************/
EXPORT_OP void
op_cust_pol_prep_acctinfo(
	cm_nap_connection_t 	*connp, 
	int32               	opcode, 
	int32               	flags, 
	pin_flist_t         	*in_flistp, 
	pin_flist_t         	**ret_flistpp,
	pin_errbuf_t        	*ebufp);

static void
fm_cust_pol_prep_acctinfo(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp);
 
static void
fm_cust_pol_prep_acctinfo_new(
	pcm_context_t		*ctxp,
	pin_flist_t       	*in_flistp, 
	pin_flist_t       	*ac_flistp, 
	pin_errbuf_t      	*ebufp);

static void
fm_cust_pol_assign_brand_gl_segment(
	pcm_context_t		*ctxp,
	pin_account_type_t	*typep,
	char			*namep,
	poid_t			*account_poidp,
	char			*gl_segmentp,
	pin_errbuf_t		*ebufp);


/*******************************************************************
 * Routines needed from elsewhere.
 *******************************************************************/
 

/*******************************************************************
 * Main routine for the PCM_OP_CUST_POL_PREP_ACCTINFO  command
 *******************************************************************/
void
op_cust_pol_prep_acctinfo(
	cm_nap_connection_t *connp, 
	int32               opcode, 
	int32               flags, 
	pin_flist_t         *in_flistp, 
	pin_flist_t         **ret_flistpp,
	pin_errbuf_t        *ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*r_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;

	/***********************************************************
	 * Null out results until we have some.
	 ***********************************************************/
	*ret_flistpp = NULL;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_CUST_POL_PREP_ACCTINFO) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "bad opcode in op_cust_pol_prep_acctinfo",
				 ebufp);
		return;
	}

	/***********************************************************
	 * Debug: What did we get?
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_cust_pol_prep_acctinfo input flist", in_flistp);

	/***********************************************************
	 * Call main function to do it
	 ***********************************************************/
	fm_cust_pol_prep_acctinfo(ctxp, flags, in_flistp, &r_flistp, ebufp);

	/***********************************************************
	 * Results.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		*ret_flistpp = (pin_flist_t *)NULL;
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_cust_pol_prep_acctinfo error", ebufp);
	} else {
		*ret_flistpp = r_flistp;
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_cust_pol_prep_acctinfo return flist", r_flistp);
	}

	return;
}

/*******************************************************************
 * fm_cust_pol_prep_acctinfo()
 *
 *	Prep the acctinfo to be ready for on-line registration.
 *
 *******************************************************************/
static void
fm_cust_pol_prep_acctinfo(
	pcm_context_t		*ctxp,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp)
{
	pin_flist_t		*ac_flistp = NULL;
	pin_cookie_t		cookie = NULL;
	int32			elemid = 0;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);	


	/*
	 * Initialize the outgoing flist
	 */
	*out_flistpp = PIN_FLIST_COPY(in_flistp, ebufp);

	ac_flistp = PIN_FLIST_ELEM_GET_NEXT(*out_flistpp,
		PIN_FLD_ACCTINFO, &elemid, 0, &cookie, ebufp);

	/*
	 * Perform 'first time' prep (during registration)
	 */
	if (flags & PCM_OPFLG_CUST_REGISTRATION) {
		fm_cust_pol_prep_acctinfo_new(ctxp, in_flistp, 
			ac_flistp, ebufp);
	}

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_cust_pol_prep_acctinfo error", ebufp);
	}

	return;
}

/*******************************************************************
 * fm_cust_pol_prep_acctinfo_new()
 *
 *	Given the current and new values for accounting close time, 
 *      determine if the resulting first cycle should be short or 
 *      long.
 *
 *******************************************************************/
static void
fm_cust_pol_prep_acctinfo_new(
	pcm_context_t	*ctxp,
	pin_flist_t	*in_flistp, 
	pin_flist_t	*ac_flistp, 
	pin_errbuf_t	*ebufp)
{
	int32		err = 0;
	char		*gl_segmentp = NULL;
	char		gl_segment[MAX_SEGMENT_SIZE] = "";
	int32		*businesstypep = NULL;
	int32		*currencyp = NULL;
	int32		*currency_secp = NULL;
	poid_t		*pdp = (poid_t *)NULL;
	poid_t		*a_pdp = (poid_t *)NULL;
	void		*vp = NULL;

	int32		businesstype = PIN_BUSINESS_TYPE_UNKNOWN;
	int32		currency = 0;
	int32		currency_sec = 0;
	int64		poid_db = 0;
	int64		poid_id = 0;
	pin_flist_t	*t_flistp = (pin_flist_t *)NULL;
	pin_flist_t	*r_flistp = (pin_flist_t *)NULL;
	pin_account_type_t	*typep = (pin_account_type_t *)NULL;
	char			*namep = (char *)NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	typep = (pin_account_type_t *)PIN_FLIST_FLD_GET(ac_flistp, 
			PIN_FLD_ACCOUNT_TYPE, 1, ebufp);
	namep = (char *)PIN_FLIST_FLD_GET(ac_flistp, PIN_FLD_NAME, 1, ebufp);

	/***********************************************************
	 * Set the G/L segment for this account. This information
	 * will be used during the ledger report posting to identify
	 * which G/L segment this account belongs.
	 ***********************************************************/
	gl_segmentp = (char *)PIN_FLIST_FLD_GET(ac_flistp, 
			PIN_FLD_GL_SEGMENT, 1, ebufp);
	if (((int32 *)gl_segmentp != (int32 *)NULL) && (strlen(gl_segmentp) != 0)) {
		pin_strlcpy(gl_segment, gl_segmentp,sizeof(gl_segment));

	} else if(cm_fm_is_scoping_enabled()) {
		poid_t			*poidp;

		poidp = (poid_t *)PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID,
			0, ebufp);
		fm_cust_pol_assign_brand_gl_segment(ctxp, typep, namep, poidp, 
			gl_segment, ebufp);

	} else {
		pin_conf("fm_cust_pol", "gl_segment", PIN_FLDT_STR,
			&gl_segmentp, &err);

		if (gl_segmentp != (char *) NULL) {
			pin_strlcpy(gl_segment, gl_segmentp, sizeof(gl_segment));
			free(gl_segmentp);
		} else {
			pin_strlcpy(gl_segment, ".",sizeof(gl_segment));
		}
	}

	PIN_FLIST_FLD_SET(ac_flistp, PIN_FLD_GL_SEGMENT,
		(void *)gl_segment, ebufp);

	/***********************************************************
	 * Set the default currency for the account
	 * First check the input flist, if not specified in the input 
	 * flist take the one from the pin.conf file. If not specified
	 * in both places, use the default value.
	 ***********************************************************/
	currencyp = (int32 *)PIN_FLIST_FLD_GET(ac_flistp,
		PIN_FLD_CURRENCY, 1, ebufp);

	if ((currencyp != (int32 *)NULL) && (*currencyp != 0)) {
		currency = *currencyp;
	} else {
		pin_conf("fm_cust_pol", "currency", PIN_FLDT_INT,
			 (caddr_t *)&currencyp, &(err));
		if (currencyp != (int32 *) NULL) {
			currency = *currencyp;
			free(currencyp);
		}
	}

	/***********************************************************
	 * If currency not found above, get system currency from the
	 * 'root account' object.
	 ***********************************************************/

	if (currency == 0) {
		t_flistp = PIN_FLIST_CREATE(ebufp);
		a_pdp = (poid_t *)PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID, 0, ebufp);
		poid_db = PIN_POID_GET_DB(a_pdp);
		a_pdp = PIN_POID_CREATE(poid_db, "/account", (int64)1, ebufp);
		PIN_FLIST_FLD_PUT(t_flistp, PIN_FLD_POID, a_pdp, ebufp);
		PIN_FLIST_FLD_SET(t_flistp, PIN_FLD_CURRENCY, (void *)NULL, ebufp);
		PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, t_flistp, &r_flistp, ebufp);
		if (PIN_ERR_IS_ERR(ebufp)) {
			PIN_FLIST_DESTROY_EX(&t_flistp, NULL);
			PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
			goto cleanup;
		}

		currencyp = (int32 *)PIN_FLIST_FLD_GET(r_flistp, 
			PIN_FLD_CURRENCY, 0, ebufp);
		if (currencyp) {
			currency = *currencyp;
		}

		PIN_FLIST_DESTROY_EX(&t_flistp, NULL);
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	}

	currency_secp = (int32 *)PIN_FLIST_FLD_GET(ac_flistp,
		PIN_FLD_CURRENCY_SECONDARY,1, ebufp);
	/***********************************************************
	 * Do we have secondary currency for this currency?
	 ***********************************************************/
	if (((currency_secp == (int32 *)NULL) ||
		(*currency_secp == 0)) && 
		(psiu_currency_is_emu_currency(currency))) {

		psiu_currency_get_default_secondary(currency, 
			&currency_sec, ebufp);
	}
	else if (currency_secp) {
		currency_sec = *(int32 *)currency_secp;
	}

	/***********************************************************
	 * Set both currency and secondary currency.
	 ***********************************************************/
	PIN_FLIST_FLD_SET(ac_flistp, PIN_FLD_CURRENCY_SECONDARY, 
		(void *)&currency_sec, ebufp);
	PIN_FLIST_FLD_SET(ac_flistp, PIN_FLD_CURRENCY, 
		(void *)&currency, ebufp);

#ifdef NEVER

	/**************************************************************
	 * Set the PIN_FLD_ACCOUNT_NO based on the customer sequence
	 **************************************************************/
	vp = PIN_FLIST_FLD_GET( ac_flistp, PIN_FLD_ACCOUNT_NO, 1, ebufp);
	if ((vp == (void *)NULL) || (strlen((char *)vp) <= 0)) {
		/**************************************************************
	 	* May be used to set PIN_FLD_ACCOUNT_NO based on the 
	 	* customer sequence instead of seting it according to poid.
	 	**************************************************************/
		int64	account_seq = 0;
		char	separator[64] = "";
		char	sequence_id[64] = "";
		char	acctno[sizeof(sequence_id)+sizeof(separator)+40] = "";
		pcm_context_t	*new_ctxp = NULL;


		pdp = (poid_t *)PIN_FLIST_FLD_GET(in_flistp, 
			PIN_FLD_POID, 0, ebufp);
		
		/* Open a new context */
		PCM_CONTEXT_OPEN(&new_ctxp, (pin_flist_t *)0, ebufp);

		if(PIN_ERR_IS_ERR(ebufp)) {
			PIN_FLIST_LOG_ERR("fm_cust_pol_prep_acctinfo_new "
				"pcm_context_open error", ebufp);
			PCM_CONTEXT_CLOSE(new_ctxp, 0, ebufp);
			return;
		}
		
		fm_utils_get_tids(new_ctxp, PIN_POID_GET_DB(pdp), 1,
			PIN_SEQUENCE_TYPE_ACCOUNT, &account_seq,
			&sequence_id[0], &separator[0], ebufp);
		
		/* Close the context */		
		PCM_CONTEXT_CLOSE(new_ctxp, 0, ebufp);

		pin_snprintf(acctno,sizeof(acctno),"%s%s%ld", sequence_id, separator, account_seq);
		PIN_FLIST_FLD_SET(ac_flistp, PIN_FLD_ACCOUNT_NO,
			(void *)acctno, ebufp);
	} /* if... */
#endif

        /***********************************************************
         * Initialize BUSINESS_TYPE to UNKNOWN
         * First check the input flist, if not specified in the input
         * flist take the one from the pin.conf file. If not specified
         * in both places, use the default value.
         ***********************************************************/
        businesstypep = (int32 *)PIN_FLIST_FLD_GET(ac_flistp,
                        PIN_FLD_BUSINESS_TYPE, 1, ebufp);

        if ( businesstypep != (int32 *)NULL ) {
                businesstype = *businesstypep;
        } else {
                pin_conf("fm_cust_pol", "business_type", PIN_FLDT_ENUM,
                         (caddr_t *)&businesstypep, &(err));
                if (businesstypep != (int32 *) NULL) {
                        businesstype = *businesstypep;
                        free(businesstypep);
                }
        }

	PIN_FLIST_FLD_SET(ac_flistp, PIN_FLD_BUSINESS_TYPE,
		(void *)&businesstype, ebufp);

cleanup:
	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_cust_pol_prep_acctinfo_new error", ebufp);
	}

	return;
}

static void
fm_cust_pol_assign_brand_gl_segment(
	pcm_context_t		*ctxp,
	pin_account_type_t	*typep,
	char			*namep,
	poid_t			*account_poidp,
	char			*gl_segmentp,	/* Passed back on success */
	pin_errbuf_t		*ebufp)
{

	pin_flist_t		*flistp = (pin_flist_t *)NULL;
	pin_flist_t		*rlistp = (pin_flist_t *)NULL;
	poid_t			*poidp = NULL;
	char			*current_segmentp = (char *)NULL;
	size_t			len = 0;

	/* For pending errors, bail right away */
	if(PIN_ERR_IS_ERR(ebufp))
		return;
		/******/

	/*
	 * By default, if branding is enabled, go ahead and build
	 * the glid based on the current GL Segment + name of sub-brand (if
	 * we're creating a brand).
	 */
	*gl_segmentp = '\0';

	/* 
	 * First, snag the existing gl segment (of the operating account).
	 * This is necessary, as the gl segment will be based on this.
	 */
	poidp = cm_fm_perm_get_account(ebufp);

	/* 
	 * In the case where the current brand is == ROOT, you could 
	 * conceivably in the middle of a transaction to a database, and
	 * want to fetch the GL Segment in another database (ie, login
	 * as root.0.0.0.1, and the account is being created in 0.0.0.2). 
	 * This is a no-no in the multi-db world; you cannot read from a
	 * disjoint DB from where a transaction is already open. 
	 * 
	 * To get around this problem, if the databases of the current
	 * operational account poid and the "in progress" account poid are
	 * different, assume that glid is '.'
	 *
	 * NOTE: This is only a problem for the root account, as accounts
	 * (and brands) that get created from there can be distributed 
	 * across any db's. For brands within brands, this will not be a
	 * problem.
	 */
	if(PIN_POID_GET_DB(poidp) == PIN_POID_GET_DB(account_poidp)) {
		flistp = PIN_FLIST_CREATE(ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, (void *)poidp, ebufp);
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_GL_SEGMENT, (void *)NULL, 
			ebufp);
		PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, flistp , &rlistp, ebufp);
		if(PIN_ERR_IS_ERR(ebufp)) {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_cust_pol_assign_brand_gl_segment: problems "
				"getting GL segment of currently operational "
				"account",
			 ebufp);
			goto cleanup;
			/********/
		}
		current_segmentp = (char *)PIN_FLIST_FLD_TAKE(rlistp, 
			PIN_FLD_GL_SEGMENT, 0, ebufp);
	} else {
		/* 
		 * Assume root GL Segment --> '.' 
		 * 
		 * If this is not the case, this default should be changed.
		 */
		current_segmentp = strdup(".");
	}

	/* 
	 * Finally, we're ready to do something! Calculate new segment ID.
	 */
	if((typep != (pin_account_type_t *)NULL) && 
		(*typep == PIN_ACCOUNT_TYPE_BRAND)) {
		/*
		 * Brand GLID will look like:
		 * <Other brand(s) before it>.Brandname.
		 */

		/* current_segmentp should always be there but it */
		/* doesn't hurt too check */
		if ( current_segmentp ) {
			pin_strlcpy(gl_segmentp, current_segmentp, MAX_SEGMENT_SIZE);
		}
		
		/*
		 * Ensure that there's a '.' at the end before we tack the 
		 * additional stuff on.
		 */
		len = strlen(gl_segmentp);
		if((len == 0) || (*(gl_segmentp + len - 1) != '.'))
			pin_strlcat(gl_segmentp, ".", MAX_SEGMENT_SIZE);
		if ( namep ) {
			pin_strlcat(gl_segmentp, namep, MAX_SEGMENT_SIZE);
		}
		/* apparantly we don't want a '.' at the end. */
		/*strcat(gl_segmentp, ".");*/
	} else {
		if((current_segmentp != (char *)NULL) && 
			(strlen(current_segmentp) > 0)) {
			pin_strlcpy(gl_segmentp, current_segmentp, MAX_SEGMENT_SIZE);
		} else {
			pin_strlcpy(gl_segmentp, ".", MAX_SEGMENT_SIZE);
		}
	}

cleanup:
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	PIN_FLIST_DESTROY_EX(&rlistp, NULL);
	if(current_segmentp != (char *)NULL)
		free(current_segmentp);
}
