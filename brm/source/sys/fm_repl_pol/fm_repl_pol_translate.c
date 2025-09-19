/*******************************************************************
 *
* Copyright (c) 1999, 2024, Oracle and/or its affiliates. 
 *
 *      This material is the confidential property of Oracle Corporation or
 *      its licensors and may be used, reproduced, stored or transmitted only
 *      in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: fm_repl_pol_translate.c portalbase satkp Exp $";
#endif

#define FILE_SOURCE_ID "fm_repl_pol_translate.c(16)"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcm.h"
#include "ops/repl.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "fm_utils.h"
#include "pin_cust.h"
#include "fm_repl_pol.h"

/*
 * PROTOTYPES
 */
static void
fm_repl_pol_set_replica_fields(
	int			is_creation,
	pcm_context_t		*ctxp,
	int32			opflags,
	poid_t			*a_pdp,
	poid_t			*cons_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_repl_pol_get_account_flds(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_repl_pol_get_service_flds(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_repl_pol_prep_account_flds(
	int			is_creation,
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	poid_t			*cons_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_repl_pol_prep_service_flds(
	int			is_creation,
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*s_pdp,
	poid_t			*cons_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_repl_pol_prep_service_email_flds(
	int			is_creation,
	int			user_scheme,
	pin_flist_t		*rep_flistp,
	pin_errbuf_t		*ebufp);

static void
fm_repl_pol_prep_service_ip_flds(
	int			is_creation,
	int			user_scheme,
	pin_flist_t		*rep_flistp,
	pin_errbuf_t		*ebufp);

static void
fm_repl_pol_find_ldap_login (
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_repl_pol_find_service_login(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	poid_t			*s_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static void
fm_repl_pol_find_ldap_profile(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static char *
fm_repl_pol_get_billtype_desc(
	pin_bill_type_t		bill_type);

static char*
fm_repl_pol_get_status_str(
	pin_status_t		status);

void search_billinfo(
        pcm_context_t           *ctxp,
        poid_t                  *acct_obj,
        pin_flist_t             **r_flistpp,
        pin_errbuf_t            *ebufp);

/*
 * fm_repl_pol_create_obj:
 */
void
fm_repl_pol_create_obj(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	poid_t			*cons_pdp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*rep_flistp 	= NULL;
	pin_flist_t		*r_flistp	= NULL;
	int			is_creation	= 1; /* Create object */

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_POID(PIN_ERR_LEVEL_DEBUG,
			 "Input poid for fm_repl_pol_create_obj", a_pdp);

	/*
	 * Set the fields of the replica object.
	 */
	fm_repl_pol_set_replica_fields(is_creation, ctxp, flags, a_pdp, cons_pdp,
					&rep_flistp, ebufp);

	/*
	 * Log the input flist that we ship to DM_LDAP.
	 */
	if (! PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"fm_repl_pol_create_obj:PCM_OP_CREATE_OBJ input",
			rep_flistp);
	}

	/*
	 * Invoke the CREATE_OBJ opcode on DM_LDAP. Remember to use the
	 * the USE_POID_GIVEN opflag because DM_LDAP cannot create poid ids.
	 */
	PCM_OP(ctxp, PCM_OP_CREATE_OBJ, (flags | PCM_OPFLG_USE_POID_GIVEN),
	       rep_flistp, &r_flistp, ebufp);

	/*
	 * Log the output from DM_LDAP
	 */
	if (! PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"fm_repl_pol_create_obj:PCM_OP_CREATE_OBJ output",
			r_flistp);
	}

	PIN_FLIST_DESTROY_EX(&rep_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "fm_repl_pol_create_obj flistp error", ebufp);
	}

	return;
}

void
fm_repl_pol_write_flds(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	poid_t			*cons_pdp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*rep_flistp 	= NULL;
	pin_flist_t		*r_flistp	= NULL;
	int			is_creation	= 0; /* WRITE FIELDS */

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_POID(PIN_ERR_LEVEL_DEBUG,
			 "Input poid for fm_repl_pol_write_flds", a_pdp);

	/*
	 * Set the fields for the replica object.
	 */
	fm_repl_pol_set_replica_fields(is_creation, ctxp, flags, a_pdp, cons_pdp,
					&rep_flistp, ebufp);

	/*
	 * Log the input flist that we ship to DM_LDAP.
	 */
	if (! PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"fm_repl_pol_write_flds:PCM_OP_WRITE_FLDS input",
			rep_flistp);
	}

	/*
	 * Invoke the WRITE_FLDS opcode on DM_LDAP
	 */
	PCM_OP(ctxp, PCM_OP_WRITE_FLDS, flags, rep_flistp, &r_flistp, ebufp);

	/*
	 * Log the output from DM_LDAP
	 */
	if (! PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			  "fm_repl_pol_write_flds:PCM_OP_WRITE_FLDS output",
			  r_flistp);
	}

	PIN_FLIST_DESTROY_EX(&rep_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "fm_repl_pol_write_flds flistp error", ebufp);
	}

	return;
}

/*
 * fm_repl_pol_set_replica_fields:
 */
static void
fm_repl_pol_set_replica_fields(
	int			is_creation,
	pcm_context_t		*ctxp,
	int32			opflags,
	poid_t			*a_pdp,
	poid_t			*cons_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	if (fm_utils_is_subtype(a_pdp, "/account")) {

		fm_repl_pol_prep_account_flds(is_creation, ctxp, opflags,
					      a_pdp, cons_pdp, r_flistpp, ebufp);

	} else if (fm_utils_is_subtype(a_pdp, "/service/email")	||
		   fm_utils_is_subtype(a_pdp, "/service/ip")	||
		   fm_utils_is_subtype(a_pdp, "/service/ldap")) {

		fm_repl_pol_prep_service_flds(is_creation, ctxp, opflags,
					      a_pdp, cons_pdp, r_flistpp, ebufp);
	} else {
		PIN_ERR_LOG_POID(PIN_ERR_LEVEL_DEBUG,
				 "Unhandled object type:", a_pdp);
	}
}

static void
fm_repl_pol_get_account_flds(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*read_flistp 	= NULL;
	pin_flist_t		*name_flistp 	= NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	*r_flistpp = NULL;

	/*
	 * Read in the list of account fields we are interested in
	 */
	read_flistp = PIN_FLIST_CREATE(ebufp);
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_POID, (void *)a_pdp, ebufp);
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_NAME, NULL, ebufp);
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_STATUS, NULL, ebufp);
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_ACCOUNT_NO, NULL, ebufp);
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_CURRENCY, NULL, ebufp);

	name_flistp = PIN_FLIST_ELEM_ADD(read_flistp, PIN_FLD_NAMEINFO,
			   PIN_NAMEINFO_BILLING, ebufp);
	PIN_FLIST_FLD_SET(name_flistp, PIN_FLD_LAST_NAME, NULL, ebufp);
	PIN_FLIST_FLD_SET(name_flistp, PIN_FLD_FIRST_NAME, NULL, ebufp);
	PIN_FLIST_FLD_SET(name_flistp, PIN_FLD_MIDDLE_NAME, NULL, ebufp);
	PIN_FLIST_FLD_SET(name_flistp, PIN_FLD_ADDRESS, NULL, ebufp);
	PIN_FLIST_FLD_SET(name_flistp, PIN_FLD_CITY, NULL, ebufp);
	PIN_FLIST_FLD_SET(name_flistp, PIN_FLD_STATE, NULL, ebufp);
	PIN_FLIST_FLD_SET(name_flistp, PIN_FLD_ZIP, NULL, ebufp);
	PIN_FLIST_FLD_SET(name_flistp, PIN_FLD_COUNTRY, NULL, ebufp);

	/*
	 * Read the account fields from DM_ORACLE.
	 */
	PCM_OP(ctxp, PCM_OP_READ_FLDS, flags, read_flistp, r_flistpp, ebufp);

	if (fm_utils_is_subtype(a_pdp, "/account")) {
           search_billinfo(ctxp, a_pdp, r_flistpp, ebufp);
         }

	/*
	 * Log the output from DM_ORACLE
	 */
	if (! PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"fm_repl_pol_get_account_flds:PCM_OP_READ_FLDS output",
			*r_flistpp);
	}

	PIN_FLIST_DESTROY_EX(&read_flistp, NULL);
}

static void
fm_repl_pol_get_service_flds(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*read_flistp 	= NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}

	PIN_ERR_CLEAR_ERR(ebufp);
	*r_flistpp = NULL;

	/*
	 * Read in the list of service fields we are interested in
	 */
	read_flistp = PIN_FLIST_CREATE(ebufp);
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_POID, (void *)a_pdp, ebufp);
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_ACCOUNT_OBJ, NULL, ebufp);
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_LOGIN, NULL, ebufp);
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_STATUS, NULL, ebufp);
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_PASSWD, NULL, ebufp);

	if (fm_utils_is_subtype(a_pdp, "/service/email")) {
		PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_SERVICE_EMAIL,
				  NULL, ebufp);
	} else if (fm_utils_is_subtype(a_pdp, "/service/ip")) {
		PIN_FLIST_ELEM_SET(read_flistp, NULL, PIN_FLD_ARGS,
				   PIN_ELEMID_ANY, ebufp);
	}

	/*
	 * Read the service object fields from DM_ORACLE.
	 */
	PCM_OP(ctxp, PCM_OP_READ_FLDS, flags, read_flistp, r_flistpp, ebufp);

	/*
	 * Log the output flist.
	 */
	if (! PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"fm_repl_pol_get_service_flds:PCM_OP_READ_FLDS output",
			*r_flistpp);
	}

	PIN_FLIST_DESTROY_EX(&read_flistp, NULL);
}

void search_billinfo( pcm_context_t *ctxp,  poid_t *acct_obj, pin_flist_t **r_flistpp, pin_errbuf_t *ebufp)
{
        pin_flist_t     *s_flistp   = NULL;
        pin_flist_t     *a_flistp = NULL;
        pin_flist_t     *r_flistp = NULL;
	poid_t          *objp     = NULL;
        int              id       = 204;
        u_int64         database;
        int32           s_flags   = 0;
        int32           rec_id;
        void            *vp;
        /***********************************************************
        * Check the error buffer.
        ***********************************************************/

	if (PIN_ERR_IS_ERR(ebufp))
                return;
        PIN_ERR_CLEAR_ERR(ebufp);


        /*******************************************************
         * Create the search flist
         *******************************************************/
        s_flistp = PIN_FLIST_CREATE(ebufp);
        objp = PIN_POID_CREATE(PIN_POID_GET_DB(acct_obj), "/search", id, ebufp);
        PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, objp, ebufp);
        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_TEMPLATE, (void *)"select X from /billinfo where F1 = V1", ebufp);
        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_FLAGS,(void *)&s_flags, ebufp);


        a_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);
        PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_ACCOUNT_OBJ, acct_obj, ebufp);

        a_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_RESULTS, 0, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_PAY_TYPE, (void *)NULL, ebufp);

        /******************************************************
         * Log the Input Flist
         ******************************************************/
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                        "search_billinfo :input flist",
                        s_flistp);

        PCM_OP(ctxp, PCM_OP_SEARCH, 0,  s_flistp, &r_flistp, ebufp);

        /******************************************************
         * Log the Output Flist
         ******************************************************/
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                                  "search_billinfo :output flist", r_flistp);

        a_flistp = PIN_FLIST_ELEM_GET(r_flistp,
                    PIN_FLD_RESULTS, PIN_ELEMID_ANY, 0, ebufp);
        vp = NULL;

	if (a_flistp != NULL)
        {
              vp = PIN_FLIST_FLD_GET(a_flistp, PIN_FLD_PAY_TYPE, 0, ebufp);
              PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_PAY_TYPE, vp, ebufp);
        }
        else
        {
              PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_PAY_TYPE, vp, ebufp);
        }
        /***********************************************************
         * Clean up.
         ***********************************************************/
        /* Free local memory */
        PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&s_flistp, NULL);

        /* Error? */
        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "billinfo_read_obj_search error", ebufp);
        }

        return;
}

static void
fm_repl_pol_prep_account_flds(
	int			is_creation,
	pcm_context_t		*ctxp,
	int32			opflags,
	poid_t			*a_pdp,
	poid_t			*cons_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*rep_flistp	= NULL;
	pin_flist_t		*name_flistp 	= NULL;
	char			*ldap_type	= NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Initialize the output flist.
	 */
	*r_flistpp = NULL;

	/*
	 * Determine the env we are operating in (LDAP)
	 */
	ldap_type = fm_repl_pol_get_ldap_type(ebufp);
	if (PIN_ERR_IS_ERR(ebufp) || ldap_type == NULL) {
		return;
	}

	/*
	 * Read in the list of account fields we are interested in
	 */
	fm_repl_pol_get_account_flds(ctxp, opflags, a_pdp,
				     &rep_flistp, ebufp);

	/*
	 * Keep the original account poid as PIN_FLD_ACCOUNT_OBJ
	 */
	if (is_creation) {
		PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_ACCOUNT_OBJ,
				  a_pdp, ebufp);
	}

	/*
	 * Get the PIN_FLD_DN
	 */
	if (is_creation) {
		pin_flist_t	*ret_flistp = NULL;
		pin_flist_t	*l_flistp = NULL;
		char		*dn = NULL;

		fm_repl_pol_find_ldap_profile(ctxp, opflags, a_pdp,
					     &ret_flistp, ebufp);
		if (ret_flistp != NULL) {
			/*
			 * Pass the DN only if not valid or empty.
			 */
			l_flistp = PIN_FLIST_FLD_GET(ret_flistp,
					PIN_FLD_LDAP_INFO, 0, ebufp);
			dn = PIN_FLIST_FLD_GET(l_flistp, PIN_FLD_DN,
					       0, ebufp);
			if ((dn) && (*dn != '\0')) {
				PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_DN,
						  dn, ebufp);
			}
			PIN_FLIST_DESTROY_EX(&ret_flistp, NULL);
		}
	}

	/*
	 * Compose the replica poid.
	 */
	if (1) {
		poid_t	*ract_pdp 	= NULL;
		int	user_scheme 	= fm_repl_pol_get_user_scheme();
		int64	a_id 		= pin_poid_get_id(a_pdp);
		int64   check_db	= pin_poid_get_db(a_pdp);
		int64	db_id 		= pin_poid_get_db(cons_pdp);

		/* If the consumer obj db number is equal to the db number
		 * in the source obj , assume it is old scheme and so call
		 * transform db
		 */
		if (db_id == check_db) {
			int64   ldap_db         = fm_repl_pol_trnsform_ldap_db(db_id);
			db_id = ldap_db;
		}

		/*
		 * Create the r_account object using the information we got
		 * Reuse ret_flistp as the input flist
		 */
		if (user_scheme) {
			ract_pdp = PIN_POID_CREATE(db_id, "/r_user",
						   a_id, ebufp);
		} else {
			ract_pdp = PIN_POID_CREATE(db_id, "/r_account",
						   a_id, ebufp);
		}

		PIN_FLIST_FLD_PUT(rep_flistp, PIN_FLD_POID, ract_pdp, ebufp);
	}

	/*
	 * Set Account Status.
	 */
	if (1) {
		pin_status_t 	*status = NULL;
		char		*status_str = NULL;
		status = (pin_status_t *)PIN_FLIST_FLD_GET(rep_flistp,
						    PIN_FLD_STATUS, 0, ebufp);
		if (status) {
			status_str = fm_repl_pol_get_status_str(*status);
			PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_ACCOUNT_STATUS,
				  		(void *)status_str, ebufp);
		}

		PIN_FLIST_FLD_DROP(rep_flistp, PIN_FLD_STATUS, ebufp);
	}

	/*
	 * Set Name/address.
	 */
	name_flistp = PIN_FLIST_ELEM_TAKE(rep_flistp, PIN_FLD_NAMEINFO,
					  PIN_NAMEINFO_BILLING, 0, ebufp);

	/*
	 * Composing the common name
	 */
	if (1) {
		char *fn = NULL;
		char *sn = NULL;
		char *mn = NULL;
		char buf[512];

		fn = PIN_FLIST_FLD_GET(name_flistp, PIN_FLD_FIRST_NAME, 0,
				       ebufp);
		mn = PIN_FLIST_FLD_GET(name_flistp, PIN_FLD_MIDDLE_NAME, 0,
				       ebufp);
		sn = PIN_FLIST_FLD_GET(name_flistp, PIN_FLD_LAST_NAME, 0,
				       ebufp);
		if (fn && sn && mn && (strcmp(mn, ""))) {
			pin_snprintf(buf, sizeof(buf), "%s %s %s", fn, mn, sn);
		}
		else if (fn && sn) {
			pin_snprintf(buf, sizeof(buf), "%s %s", fn, sn);
		}

		PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_NAME, buf, ebufp);
		PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_FIRST_NAME, fn,ebufp);
		PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_LAST_NAME, sn, ebufp);
	}

	/*
	 * Composing the full address
	 */
	if (1) {
		char *street = NULL;
		char *city   = NULL;
		char *state  = NULL;
		char *zip    = NULL;
		char *country = NULL;
		char buf[1024];

		street = PIN_FLIST_FLD_GET(name_flistp, PIN_FLD_ADDRESS, 0,
					   ebufp);
		city = PIN_FLIST_FLD_GET(name_flistp, PIN_FLD_CITY, 0, ebufp);
		state = PIN_FLIST_FLD_GET(name_flistp, PIN_FLD_STATE, 0,
					  ebufp);
		zip = PIN_FLIST_FLD_GET(name_flistp, PIN_FLD_ZIP, 0, ebufp);
		country = PIN_FLIST_FLD_GET(name_flistp, PIN_FLD_COUNTRY, 0,
					    ebufp);
		if (street && city && state && zip && country) {
			pin_snprintf(buf, sizeof(buf),"%s, %s, %s %s, %s",
				street, city, state, zip, country);
		}
		PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_ADDRESS, buf, ebufp);
	}

	/*
	 * Get the login for the service associated with ldap
	 */
	if (1) {
		pin_flist_t 	*s_flistp = NULL;
		char		*login = NULL;
		char		*passwd = NULL;
		char		*buf;

		fm_repl_pol_find_ldap_login(ctxp, opflags, a_pdp, &s_flistp,
						ebufp);

		/*
		 * PIN_FLD_LOGIN is used as the RDN piece for the /r_user
		 * object in the dm_ldap's mapping file. In some Directory
		 * Servers you cannot write the RDN attribute even if the
		 * value is identical.
		 */
		if (is_creation) {
			login = PIN_FLIST_FLD_GET(s_flistp, PIN_FLD_LOGIN, 0,
						ebufp);
			PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_LOGIN, login,
						ebufp);
		}

		/*
		 * Trim the header from PIN_FLD_PASSWD
		 * "clear|XXX" ==> "XXX"
		 */
		passwd = PIN_FLIST_FLD_GET(s_flistp, PIN_FLD_PASSWD, 0, ebufp);
		if (passwd) {
			buf = strchr(passwd, '|');
			if (buf) {
				buf++;
				PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_PASSWD,
						  buf, ebufp);
			}
			/* Add the error case later */
		}

		PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
	}

	/*
	 * Translating PIN_FLD_PAY_TYPE to its description.
	 */
	if (1) {
		pin_bill_type_t		*bill_type;
		char			*billtype_desc = NULL;

		bill_type = PIN_FLIST_FLD_GET(rep_flistp, PIN_FLD_PAY_TYPE,
					      0, ebufp);
		if (bill_type) {
			billtype_desc =
				fm_repl_pol_get_billtype_desc(*bill_type);
		}
		PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_BILL_TYPE_NAME,
				  billtype_desc, ebufp);
		PIN_FLIST_FLD_DROP(rep_flistp, PIN_FLD_PAY_TYPE, ebufp);
	}

	/*
	 * Translating PIN_FLD_CURRENCY to its description
	 */
	if (1) {
		int32	*be_idp	= NULL;
		char	*currency_desc = NULL;
		char	name_buf[1024];

		be_idp = PIN_FLIST_FLD_GET(rep_flistp, PIN_FLD_CURRENCY, 0,
					   ebufp);
		if (be_idp != NULL) {
			fm_utils_beid_name(ctxp, *be_idp, name_buf, sizeof(name_buf));
			currency_desc = name_buf;
		}
		PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_CURRENCY_NAME,
				  currency_desc, ebufp);
		PIN_FLIST_FLD_DROP(rep_flistp, PIN_FLD_CURRENCY, ebufp);
	}


	PIN_FLIST_DESTROY_EX(&name_flistp, NULL);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(&rep_flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "fm_repl_pol_prep_account_flds error",
				 ebufp);
	} else {
		*r_flistpp = rep_flistp;
	}
}

static void
fm_repl_pol_prep_service_flds(
	int			is_creation,
	pcm_context_t		*ctxp,
	int32			opflags,
	poid_t			*s_pdp,
	poid_t			*cons_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*rep_flistp	= NULL;
	int			is_email	= 0;
	int			is_ip		= 0;
	int			is_ldap		= 0;
	int64			ldap_db		= 0;
	int64			db_id		= 0;
	int			user_scheme	= 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	is_email 	= fm_utils_is_subtype(s_pdp, "/service/email");
	is_ip 		= fm_utils_is_subtype(s_pdp, "/service/ip");
	is_ldap		= fm_utils_is_subtype(s_pdp, "/service/ldap");
	db_id		= pin_poid_get_db(cons_pdp);
	user_scheme 	= fm_repl_pol_get_user_scheme();

	/*
	 * Read in the list of account fields we are interested in
	 */
	fm_repl_pol_get_service_flds(ctxp, opflags, s_pdp,
				     &rep_flistp, ebufp);

	/*
	 * We don't push the passwd for email and ip service
	 */
	if (is_email || is_ip) {
		PIN_FLIST_FLD_DROP(rep_flistp, PIN_FLD_PASSWD, ebufp);
	}

	/*
	 * We don't push the login and status for ldap service here.
	 * Login is pushed only once during account creation.
	 */
	if (is_ldap) {
		char	*passwd	= NULL;
		char	*buf	= NULL;

		PIN_FLIST_FLD_DROP(rep_flistp, PIN_FLD_STATUS, ebufp);
		PIN_FLIST_FLD_DROP(rep_flistp, PIN_FLD_LOGIN, ebufp);

		/*
		 * Trim the header from PIN_FLD_PASSWD
		 * "clear|XXX" ==> "XXX"
		 */
		passwd = PIN_FLIST_FLD_GET(rep_flistp, PIN_FLD_PASSWD,
					   0, ebufp);
		if (passwd) {
			char	temp_buf[512];

			buf = strchr(passwd, '|');
			if (buf) {
				buf++;
				pin_strlcpy(temp_buf, buf, sizeof(temp_buf));
				PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_PASSWD,
						  temp_buf, ebufp);
			}
			/* Add the error case later */
		}
	}

	/*
	 * Compose the replica poid.
	 */
	{
		poid_t	*r_pdp 		= NULL;

		/*
		 * Create the r_account object using the information we got
		 */
		if (user_scheme) {
			int64	a_id = 0;
			poid_t	*acct_pdp = NULL;

			acct_pdp = (poid_t *)PIN_FLIST_FLD_GET(rep_flistp,
					PIN_FLD_ACCOUNT_OBJ, 0, ebufp);
			a_id = pin_poid_get_id(acct_pdp);

			r_pdp = PIN_POID_CREATE(db_id, "/r_user", a_id,
						ebufp);
		} else {
			int64	s_id = 0;
			s_id = pin_poid_get_id(s_pdp);
			if (is_email) {
				r_pdp = PIN_POID_CREATE(db_id,
							"/r_service/email",
							s_id, ebufp);
			} else if (is_ip) {
				r_pdp = PIN_POID_CREATE(db_id,
							"/r_service/ip",
							s_id, ebufp);
			}
		}

		PIN_FLIST_FLD_PUT(rep_flistp, PIN_FLD_POID, r_pdp, ebufp);
	}

	/*
	 * Compose ACCOUNT_OBJ_POID if necessary.
	 */
	if (user_scheme) {
		PIN_FLIST_FLD_DROP(rep_flistp, PIN_FLD_ACCOUNT_OBJ, ebufp);
	} else {
		poid_t	*acct_pdp = NULL;
		poid_t	*ract_pdp = NULL;

		acct_pdp = PIN_FLIST_FLD_GET(rep_flistp, PIN_FLD_ACCOUNT_OBJ,
					    0, ebufp);
		ract_pdp = PIN_POID_CREATE(db_id, "/r_account",
					   PIN_POID_GET_ID(acct_pdp), ebufp);
		PIN_FLIST_FLD_PUT(rep_flistp, PIN_FLD_ACCOUNT_OBJ, ract_pdp,
				  ebufp);
	}

	if (is_email) {
		fm_repl_pol_prep_service_email_flds(is_creation, user_scheme,
							rep_flistp, ebufp);
	} else if (is_ip) {
		fm_repl_pol_prep_service_ip_flds(is_creation, user_scheme,
							rep_flistp, ebufp);
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(&rep_flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "fm_repl_pol_prep_service_flds error",
				 ebufp);
	} else {
		*r_flistpp = rep_flistp;
	}
}

static void
fm_repl_pol_prep_service_email_flds(
	int			is_creation,
	int			user_scheme,
	pin_flist_t		*rep_flistp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t	*e_flistp = NULL;

	if (user_scheme) {
		char		*login = NULL;
		pin_status_t 	*status = NULL;
		char		*status_str = NULL;
		/*
		 * Change LOGIN,STATUS to EMAIL_*
		 */
		login = PIN_FLIST_FLD_TAKE(rep_flistp, PIN_FLD_LOGIN, 0, ebufp);
		PIN_FLIST_FLD_PUT(rep_flistp, PIN_FLD_EMAIL_LOGIN, login,
					ebufp);

		status = PIN_FLIST_FLD_GET(rep_flistp, PIN_FLD_STATUS, 0,
					ebufp);
		if (status) {
			status_str = fm_repl_pol_get_status_str(*status);
			PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_EMAIL_STATUS,
				  		(void *)status_str, ebufp);
		}

		PIN_FLIST_FLD_DROP(rep_flistp, PIN_FLD_STATUS, ebufp);
	}

	/*
         * Transfer the rest of the email specific fields.
         */
	e_flistp = PIN_FLIST_SUBSTR_TAKE(rep_flistp, PIN_FLD_SERVICE_EMAIL,
					     0, ebufp);

	/* DROP MAIL SERVER ID FOR NOW...SHOULD STRINGIFY THIS */
	PIN_FLIST_FLD_DROP(e_flistp, PIN_FLD_MAIL_SERVER_ID, ebufp);

	/* Do not push PIN_FLD_PATH */
	PIN_FLIST_FLD_DROP(e_flistp, PIN_FLD_PATH, ebufp);

	PIN_FLIST_CONCAT(rep_flistp, e_flistp, ebufp);
	PIN_FLIST_DESTROY_EX(&e_flistp, NULL);
}

static void
fm_repl_pol_prep_service_ip_flds(
	int			is_creation,
	int			user_scheme,
	pin_flist_t		*rep_flistp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*a_flistp = NULL;
	pin_cookie_t		cookie = NULL;
	int32			element_id = 0;

	if (user_scheme) {
		char		*login = NULL;
		pin_status_t 	*status = NULL;
		char		*status_str = NULL;
		/*
		 * Change LOGIN,STATUS to IP_*
		 */
		login = PIN_FLIST_FLD_TAKE(rep_flistp, PIN_FLD_LOGIN, 0, ebufp);
		PIN_FLIST_FLD_PUT(rep_flistp, PIN_FLD_IP_LOGIN, login, ebufp);

		status = PIN_FLIST_FLD_GET(rep_flistp, PIN_FLD_STATUS, 0,
						ebufp);
		if (status) {
			status_str = fm_repl_pol_get_status_str(*status);
			PIN_FLIST_FLD_SET(rep_flistp, PIN_FLD_IP_STATUS,
				  		(void *)status_str, ebufp);
		}

		PIN_FLIST_FLD_DROP(rep_flistp, PIN_FLD_STATUS, ebufp);
	}

	/*
         * Walkr through the IP ARGS and join name/value.
         */
	cookie = NULL;
	element_id = 0;
	while ((a_flistp = PIN_FLIST_ELEM_GET_NEXT(rep_flistp, PIN_FLD_ARGS,
						   &element_id, 1, &cookie,
						   ebufp)) != NULL) {
		char pair[2048];
		char *name, *value;
		name = PIN_FLIST_FLD_GET(a_flistp, PIN_FLD_NAME, 0, ebufp);
		value = PIN_FLIST_FLD_GET(a_flistp, PIN_FLD_VALUE, 0, ebufp);
		if (name && value) {
			/*
			 * NO ENCODING IS DONE IN CASE VALUE HAS "=" IN IT
			 */
			pin_snprintf(pair, sizeof(pair),"%s=%s ", name, value);
		}

		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_ARG, (void *)pair, ebufp);
		PIN_FLIST_FLD_DROP(a_flistp, PIN_FLD_NAME, ebufp);
		PIN_FLIST_FLD_DROP(a_flistp, PIN_FLD_VALUE, ebufp);

	} /* while */
}


static void
fm_repl_pol_find_ldap_login(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	char		*ldap_type = NULL;
	poid_t		*serv_pdp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Now find the login specific to the LDAP type
	 */
	ldap_type = fm_repl_pol_get_ldap_type(ebufp);
	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}

	if (strcmp(ldap_type, "ldap") == 0) {
		serv_pdp = PIN_POID_CREATE(PIN_POID_GET_DB(a_pdp),
						"/service/ldap",
						(int64)-1, ebufp);
	} else {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			    PIN_ERRCLASS_SYSTEM_DETERMINATE,
			    PIN_ERR_INVALID_CONF, 0, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"invalid ldap_type parameter in pin.conf",
				 ebufp);
		return;
	}

	fm_repl_pol_find_service_login(ctxp, flags, a_pdp, serv_pdp,
					r_flistpp, ebufp);

	PIN_POID_DESTROY_EX(&serv_pdp, NULL);

	return;
}

static void
fm_repl_pol_find_service_login(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	poid_t			*s_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t	*s_flistp = NULL;
	pin_flist_t	*a_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	poid_t		*obj_pdp = NULL;
	int64		db_no = 0;
	int32		element_id = 0;
	pin_cookie_t    cookie = NULL;
	poid_t		*serv_pdp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	db_no = PIN_POID_GET_DB(a_pdp);

	/*
	 * Create the service poid
	 */
	serv_pdp = PIN_POID_CREATE(db_no, PIN_POID_GET_TYPE(s_pdp),
				   -1, ebufp);

	/*
	 * Create the search flist
	 */
	s_flistp = PIN_FLIST_CREATE(ebufp);
	obj_pdp = PIN_POID_CREATE(db_no, "/search", (int64)237, ebufp);
	PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, obj_pdp, ebufp);

	a_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);
	PIN_FLIST_FLD_PUT(a_flistp, PIN_FLD_POID, serv_pdp, ebufp);

	a_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 2, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_ACCOUNT_OBJ, a_pdp, ebufp);

	a_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_RESULTS, 0, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_POID, NULL, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_LOGIN, NULL, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_PASSWD, NULL, ebufp);

	/*
	 * Perform the search
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			  "Search flist for login:", s_flistp);
	PCM_OP(ctxp, PCM_OP_SEARCH, flags, s_flistp, &r_flistp, ebufp);
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
	                          "Return flist from search", r_flistp);

	/*
	 * Here we only look at the first return result
	 */
	*r_flistpp = PIN_FLIST_ELEM_TAKE_NEXT(r_flistp, PIN_FLD_RESULTS,
					   &element_id, 1, &cookie, ebufp);

	PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	return;
}

static void
fm_repl_pol_find_ldap_profile(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t	*s_flistp = NULL;
	pin_flist_t	*a_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	pin_flist_t	*l_flistp = NULL;
	poid_t		*obj_pdp = NULL;
	poid_t		*ldap_pdp = NULL;
	int64		db_no = 0;
	int32		element_id = 0;
	pin_cookie_t    cookie = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	db_no = PIN_POID_GET_DB(a_pdp);

	/*
	 * Create the /profile/ldap poid
	 */
	ldap_pdp = PIN_POID_CREATE(db_no, "/profile/ldap%", (int64)-1, ebufp);

	/*
	 * Create the search flist
	 */
	s_flistp = PIN_FLIST_CREATE(ebufp);
	obj_pdp = PIN_POID_CREATE(db_no, "/search", (int64)278, ebufp);
	PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, obj_pdp, ebufp);

	a_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_ACCOUNT_OBJ, a_pdp, ebufp);

	a_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 2, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_POID, ldap_pdp, ebufp);

	a_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_RESULTS, 0, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_POID, NULL, ebufp);
	l_flistp = PIN_FLIST_SUBSTR_ADD(a_flistp, PIN_FLD_LDAP_INFO, ebufp);
	PIN_FLIST_FLD_SET(l_flistp, PIN_FLD_DN, NULL, ebufp);

	/*
	 * Perform the search
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			  "Search flist for profile:", s_flistp);
	PCM_OP(ctxp, PCM_OP_SEARCH, flags, s_flistp, &r_flistp, ebufp);
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			  "Return flist from search", r_flistp);

	/*
	 * Here we only look at the first return result
	 */
	*r_flistpp = PIN_FLIST_ELEM_TAKE_NEXT(r_flistp, PIN_FLD_RESULTS,
					   &element_id, 1, &cookie, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			  "Return flist of find_ldap_profile", *r_flistpp);

	PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	return;
}

static char *
fm_repl_pol_get_status_str(
	pin_status_t		status)
{
	char		*name = NULL;
	switch (status) {
	case PIN_STATUS_DEFUNCT:
		name = "Defunct";
		break;
	case PIN_STATUS_ACTIVE:
		name = "Active";
		break;
	case PIN_STATUS_INACTIVE:
		name = "Inactive";
		break;
	case PIN_STATUS_CLOSED:
		name = "Closed";
		break;
	default:
		name = "Unknown";
		break;
	}

	return name;
}

static char *
fm_repl_pol_get_billtype_desc(
	pin_bill_type_t		bill_type)
{
	char		*billtype_desc = NULL;

	switch (bill_type){
	case PIN_BILL_TYPE_PREPAID:
		billtype_desc = "Prepaid";
		break;
	case PIN_BILL_TYPE_INVOICE:
		billtype_desc = "Invoice";
		break;
	case PIN_BILL_TYPE_DEBIT:
		billtype_desc = "Debit";
		break;
	case PIN_BILL_TYPE_CC:
		billtype_desc = "Credit Card";
		break;
	case PIN_BILL_TYPE_DDEBIT:
		billtype_desc = "Direct Debit";
		break;
	case PIN_BILL_TYPE_SMARTC:
		billtype_desc = "Smart Card";
		break;
	case PIN_BILL_TYPE_SUBORD:
		billtype_desc = "Subordinate";
		break;
	case PIN_BILL_TYPE_INTERNAL:
		billtype_desc = "Internal";
		break;
	case PIN_BILL_TYPE_GUEST:
		billtype_desc = "Guest";
		break;
	case PIN_BILL_TYPE_CASH:
		billtype_desc = "Cash";
		break;
	case PIN_BILL_TYPE_CHECK:
		billtype_desc = "Check";
		break;
	case PIN_BILL_TYPE_WTRANSFER:
		billtype_desc = "Wire Transfer";
		break;
	case PIN_BILL_TYPE_PAYORDER:
		billtype_desc = "Inter Bank Payment Order";
		break;
	case PIN_BILL_TYPE_POSTALORDER:
		billtype_desc = "Postal Order";
		break;
	default:
		billtype_desc = "Unknown";
		break;
	}

	return billtype_desc;
}

