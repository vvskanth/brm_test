/*
 * Copyright (c) 1996, 2024, Oracle and/or its affiliates. 
 * 
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: sample_app.c /cgbubrm_7.5.0.portalbase/1 2023/08/07 04:17:29 visheora Exp $";
#endif

/*******************************************************************
 * Simple test program to create a customer (account & services).
 *
 * We perform several basic steps as a howto example:
 *	- open a database channel
 *	- retrieve the desired plan
 *	- add the customer info to the plan
 *	- create the customer
 *	- close the database channel
 *
 *******************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>

#include "pinlog.h"
#include "pin_errs.h"
#include "pcm.h"
#include "ops/cust.h"
#include "pin_cust.h"
#include "pin_currency.h"
#include "pin_os.h"
#include "pin_os_getopt.h"


#define USAGE "usage: sample_app [-dh] -l <login> -p <password> <plan>\n"

/*******************************************************************
 * Globals
 *******************************************************************/
extern int	opterr;		/* for getopt(3c) */
extern int	optind;		/* for getopt(3c) */
extern char	*optarg;	/* for getopt(3c) */

/*******************************************************************
 * sample_find_plan():
 *
 *	Get the plan flist corresponding to the given plan name.
 *	We bluff as 'admintool' when calling GET_PLANS so that
 *	we also have at our access the CSR plan (and others) that
 *	is not generally available.
 *
 *******************************************************************/
void
sample_find_plan(
	pcm_context_t		*ctxp,
	int64			database,
	char			*plan,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pin_cookie_t		cookie = NULL;
	pin_flist_t		*i_flistp = NULL;
	pin_flist_t		*p_flistp = NULL;
	pin_flist_t		*r_flistp = NULL;
	poid_t			*a_pdp = NULL;

	char			*access = NULL;
	char			*name = NULL;

	u_int			plan_found = 0;
	int32			element_id;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Allocate the flist for loading the plans
	 */
	i_flistp = PIN_FLIST_CREATE(ebufp);

	/*
	 * Create and add a type-only poid.
	 */
	a_pdp = PIN_POID_CREATE(database, "/account", (int64)-1, ebufp);
	PIN_FLIST_FLD_PUT(i_flistp, PIN_FLD_POID, a_pdp, ebufp);

	/*
         * Try to get the plans listed in the "CSR" plan list.
         */
	access = "CSR";
	PIN_FLIST_FLD_SET(i_flistp, PIN_FLD_AAC_ACCESS, (void *)access, ebufp);

	/*
	 * Call the GET_PLANS Customer Policy FM opcode
	 */
	PCM_OP(ctxp, PCM_OP_CUST_POL_GET_PLANS, 0, i_flistp, &r_flistp, ebufp);

	/*
	 * Walk the plans array looking for a match.
	 */
	p_flistp = PIN_FLIST_ELEM_GET_NEXT(r_flistp, PIN_FLD_PLAN,
		&element_id, 1, &cookie, ebufp);
	while (p_flistp != (pin_flist_t *)NULL) {

		/*
                 * See if the plan matches what we want
                 */
		name = (char *)PIN_FLIST_FLD_GET(p_flistp,
			PIN_FLD_NAME, 0, ebufp);

		if ((name != (char *)NULL) && (strcmp(name, plan) == 0)) {

			plan_found = 1;

			*o_flistpp = PIN_FLIST_COPY(p_flistp, ebufp);

			PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"plan flist found", p_flistp);

			break;

		}

		/*
		 * Get the next result.
		 */
		p_flistp = PIN_FLIST_ELEM_GET_NEXT(r_flistp, PIN_FLD_PLAN,
			&element_id, 1, &cookie, ebufp);
	}

	/*
	 * If we didn't find the plan, set an error
	 */
	if ((!PIN_ERR_IS_ERR(ebufp)) && (!plan_found)) {
		pin_set_err(ebufp, PIN_ERRLOC_APP,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_NOT_FOUND, PIN_FLD_PLAN, 0, 0);
	}

	/*
	 * Cleanup.
	 */
	PIN_FLIST_DESTROY_EX(&i_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	if(PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_cust_pol_get_plans error", ebufp);
	}

	return;
}

/*******************************************************************
 * sample_init_input():
 *
 *	Initialize the input flist with some basic information.
 *	This would typically be an interactive process.
 *
 *******************************************************************/
void
sample_init_input(
	pin_flist_t		*flistp,
	u_int64               	database,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*a_flistp = NULL;
	pin_bill_type_t		btype;
	u_int			dummy;
	poid_t          	*pdp = NULL;

	void			*vp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Add a billinfo array element to the plan flist.
	 */
	a_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_BILLINFO, 0, ebufp);
	
	pdp = PIN_POID_CREATE(database, "/billinfo", (int64)-1, ebufp);
	PIN_FLIST_FLD_PUT(a_flistp, PIN_FLD_POID, pdp, ebufp);
	
	vp = (void *)"BILL UNIT(1)";
        PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_BILLINFO_ID, vp, ebufp);

	dummy = 1;
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_BILL_WHEN, (void *)&dummy, ebufp);

        dummy = 10001;
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_PAY_TYPE, (void *)&dummy, ebufp);

	/*
	 * Add a nameaddr array element to the plan flist.
	 */
	a_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_NAMEINFO,
		PIN_NAMEINFO_BILLING, ebufp);

	vp = (void *)"Doe";
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_LAST_NAME, vp, ebufp);

	vp = (void *)"John";
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_FIRST_NAME, vp, ebufp);

	vp = (void *)"1234 Main Street";
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_ADDRESS, vp, ebufp);

	vp = (void *)"Hometown";
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_CITY, vp, ebufp);

	vp = (void *)"TX";
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_STATE, vp, ebufp);

	vp = (void *)"55555-4444";
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_ZIP, vp, ebufp);

	vp = (void *)"USA";
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_COUNTRY, vp, ebufp);

	vp = (void *)"doe@nowhere.com";
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_EMAIL_ADDR, vp, ebufp);

	/*
	 * Fill in some optional AAC fields.
	 */
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_AAC_SOURCE, (void *)"sample", ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_AAC_VENDOR, (void *)"A Inc.", ebufp);

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_init_input error", ebufp);
	}

	return;
}

void
sample_init_payinfo(
	pin_flist_t		*flistp,
	u_int64         database,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*a_flistp = NULL;
	u_int			dummy;
	poid_t          *pdp;
	void			*vp = NULL;
	pin_flist_t     *s_flistp = NULL;
	pin_flist_t     *c_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Add a Payinfo array element to the plan flist.
	 */
	a_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_PAYINFO, 0, ebufp);

	pdp = PIN_POID_CREATE(database, "/payinfo/invoice", -1, ebufp);
	PIN_FLIST_FLD_PUT(a_flistp, PIN_FLD_POID, pdp, ebufp);

	dummy = 10001;
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_PAY_TYPE, (void *)&dummy, ebufp);
	
	c_flistp = PIN_FLIST_SUBSTR_ADD(a_flistp, PIN_FLD_INHERITED_INFO,
				ebufp);
	s_flistp = PIN_FLIST_SUBSTR_ADD(c_flistp, PIN_FLD_INV_INFO, ebufp);
	
	vp = (void *)"sample_pay";
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_NAME, vp, ebufp);
	
	dummy = 1;
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_DELIVERY_PREFER, (void *)&dummy,
				ebufp);

	dummy = 0;
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_INV_TERMS, (void *)&dummy, ebufp);

	vp = (void *)"";
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_EMAIL_ADDR, vp, ebufp);

	vp = (void *)"";
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_DELIVERY_DESCR, vp, ebufp);

	vp = (void *)"55555-4444";
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_ZIP, vp, ebufp);
	
	vp = (void *)"TX";
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_STATE, vp, ebufp);

	vp = (void *)"Hometown";
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_CITY, vp, ebufp);

	vp = (void *)"1234 Main Street";
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_ADDRESS, vp, ebufp);

	vp = (void *)"USA";
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_COUNTRY, vp, ebufp);

	dummy = 1;
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_FLAGS, (void *)&dummy, ebufp);

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_init_input error", ebufp);
	}

	return;
}

/*******************************************************************
 * sample_add_service():
 *
 *	Add the requested login and password to each service
 *	array element found on the plan flist.
 *
 *******************************************************************/
void
sample_add_service(
	pin_flist_t		*p_flistp,
	char			*login,
	char			*password,
	pin_errbuf_t		*ebufp)
{
	pin_cookie_t		cookie = NULL;
	pin_flist_t		*flistp = NULL;
	int32			element_id;

	void			*vp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * For each service, we need to provide a login and password.
	 */
	flistp = PIN_FLIST_ELEM_GET_NEXT(p_flistp, PIN_FLD_SERVICES,
		&element_id, 1, &cookie, ebufp);
	while (flistp != (pin_flist_t *)NULL) {

		/*
		 * Add the login
		 */
		vp = (void *)login;
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_LOGIN, vp, ebufp);

		/*
		 * Add the password
		 */
		vp = (void *)password;
		PIN_FLIST_FLD_SET(flistp, PIN_FLD_PASSWD_CLEAR, vp, ebufp);

		/*
		 * Get the next service
		 */
		flistp = PIN_FLIST_ELEM_GET_NEXT(p_flistp, PIN_FLD_SERVICES,
			&element_id, 1, &cookie, ebufp);
	}

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_add_service error", ebufp);
	}

	return;
}

/*******************************************************************
 * sample_create_customer():
 *
 *	Create the customer in the Infranet database.
 *
 *******************************************************************/
void
sample_create_customer(
	pcm_context_t		*ctxp,
	pin_flist_t		*flistp,
	pin_errbuf_t		*ebufp)
{
	pin_flist_t		*r_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Call the CustMngmt FM's COMMIT CUSTOMER opcode
	 */
	PCM_OP(ctxp, PCM_OP_CUST_COMMIT_CUSTOMER, 0, flistp, &r_flistp, ebufp);

	/*
	 * Debug output
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"commit customer result flist", r_flistp);

	/*
	 * Cleanup.
	 */
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_create_customer error", ebufp);
	}

	return;
}

/*******************************************************************
 * MAIN
 *******************************************************************/
int
main(
	int		argc,
	char		*argv[])
{
	pcm_context_t	*ctxp = NULL;
	pin_flist_t	*flistp = NULL;
	int64		database;
	pin_errbuf_t	ebuf;

	char		c;
	char		*plan = NULL;
	char		login[256];
	char		password[256];
	char		logfile[256];
	char		*c_ptr = (char *)NULL;
	char		*program;
	int32		err;
	poid_t          *a_pdp = NULL;

	/*
	 * Clear the error buffer (for safety).
	 */
	PIN_ERR_CLEAR_ERR(&ebuf);

	/*
	 * Logging initialization
	 */
	program = basename(argv[0]);
	PIN_ERR_SET_PROGRAM(program);
	PIN_ERR_SET_LEVEL(PIN_ERR_LEVEL_WARNING);

	pin_strlcpy(logfile, "default.pinlog", sizeof(logfile));
	pin_conf(program, "logfile", PIN_FLDT_STR, (caddr_t *)&(c_ptr), &err);
	if (c_ptr != (char *)NULL) {
		pin_strlcpy(logfile, c_ptr, sizeof(logfile));
		PIN_FREE_EX(&c_ptr);
	}
	PIN_ERR_SET_LOGFILE(logfile);

	/*
	 * Parse the command line args.
	 */
	optind = 1;
	opterr = 0;

	while ((c = (char)getopt(argc, argv, "l:p:dh")) != (char)EOF) {

		switch (c) {
		case 'd':
			PIN_ERR_SET_LEVEL(PIN_ERR_LEVEL_DEBUG);
			break;
		case 'l':
			pin_strlcpy(login, optarg, sizeof(login));
			break;
		case 'p':
			pin_strlcpy(password, optarg, sizeof(password));
			break;
		case 'h':
			fprintf(stderr, USAGE);
			exit(0);
			/******/
		case '?':
			fprintf(stderr, USAGE);
			exit(1);
			/******/
		}
	}

	switch (argc - optind) {
	case 0:
		plan = "CSR";
		break;
	default:
		plan = (char *)argv[optind];
		break;
	}

	/*
	 * Open the database context
	 */
        PCM_CONNECT(&ctxp, &database, &ebuf);

	/*
	 * Locate the desired plan object
	 */
	sample_find_plan(ctxp, database, plan, &flistp, &ebuf);

	/*
	 * Add the account info to the plan flist
	 */
	a_pdp = PIN_POID_CREATE(database, "/account",(int64)-1, &ebuf);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, (void *)a_pdp, &ebuf);
	PIN_FLIST_FLD_PUT(flistp, PIN_FLD_ACCOUNT_OBJ, a_pdp, &ebuf);

	sample_init_input(flistp, database, &ebuf);
	
	/*
	*  Initialize payinfo
	*/

	sample_init_payinfo(flistp, database, &ebuf);

	/*
	 * Add the requested service login & password to the plan flist.
	 */
	sample_add_service(flistp, login, password, &ebuf);

	/*
	 * Create the customer.
	 */
	sample_create_customer(ctxp, flistp, &ebuf);

	/*
	 * Free the flists created here.
	 */
	PIN_FLIST_DESTROY_EX(&flistp, NULL);

	/*
	 * Close the database channel (a formality since we exit).
	 */
        PCM_CONTEXT_CLOSE(ctxp, 0, NULL);

	/*
	 * See if all went well.
	 */
	if (PIN_ERR_IS_ERR(&ebuf)) {
		fprintf(stderr, "\nTest Failed, See Log File\n\n");
	} else {
		fprintf(stderr, "\nTest Suceeded\n\n");
	}

	return(0);
}
