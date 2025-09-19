/*
* Copyright (c) 1996, 2024, Oracle and/or its affiliates. 
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: sample_search.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:17:34 nishahan Exp $";
#endif

/*******************************************************************
 * Simple test program to demonstrate searches.
 * Two examples are included:
 *	- a read object search with single result expected
 *	- a read fields search with multiple results expected
 *******************************************************************/
#include <stdio.h>
#include <libgen.h>
#include "pinlog.h"
#include "pin_errs.h"
#include "pcm.h"
#include "ops/base.h"
#include "pin_cust.h"

#define FILE_SOURCE_ID "sample_search.c (1.16)"


/*******************************************************************
 * Configuration
 *
 *	Reads the pin.conf file to determine the database we want.
 *
 *	Not actually needed here since we use PCM_CONNECT to
 *	open our database connection and get the database #
 *	returned as a by product. But this does illustrate
 *	how to use pin_conf to retrieve conf'ed info.
 *
 *******************************************************************/
void
sample_read_pin_conf(
	u_int64		*databasep,
	pin_errbuf_t	*ebufp)
{
	poid_t		*db_pdp;
	int32		err;

	/***********************************************************
	 * Clear the error buffer.
	 ***********************************************************/
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Get the config userid to determine DB.
	 *
	 * Note: This allocates a poid that we must free.
	 ***********************************************************/
        pin_conf("-", "userid", PIN_FLDT_POID, (caddr_t *)&(db_pdp), &err);
        if (err != PIN_ERR_NONE) {
		ebufp->pin_err = err;
                return;
        }
	*databasep = PIN_POID_GET_DB(db_pdp);
	PIN_POID_DESTROY_EX(&db_pdp, NULL);

	return;
}

/*******************************************************************
 * Simple search.
 *
 *	Performs a read-object search for the main account
 *	object and displays the results with PIN_FLIST_PRINT.
 *
 *	The "one arg equal" search for /account is 201.
 *
 *	Note: In real life, this isn't particularly useful
 *	since we could use the poid of the account to do a
 *	read-object op directly.
 *
 *******************************************************************/
void
sample_read_obj_search(
	pcm_context_t	*ctxp,
	u_int64		database,
	pin_errbuf_t	*ebufp)
{
	pin_flist_t	*flistp = NULL;
	pin_flist_t	*a_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;

	poid_t		*objp = NULL;
	u_int64		id;


	/***********************************************************
	 * Check the error buffer.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Allocate the flist for searching.
	 ***********************************************************/
	flistp = PIN_FLIST_CREATE(ebufp);

	/***********************************************************
	 * Allocate the search poid and give it to the flist.
	 ***********************************************************/
	id = (u_int64)201;

	objp = PIN_POID_CREATE(database, "/search", id, ebufp);
	PIN_FLIST_FLD_PUT(flistp, PIN_FLD_POID, objp, ebufp);

	/***********************************************************
	 * Add a search arguments array.
	 ***********************************************************/
	a_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_ARGS, 1, ebufp);

	/***********************************************************
	 * Allocate the main account poid and add as the arg.
	 ***********************************************************/
	id = (u_int64)1;

	objp = PIN_POID_CREATE(database, "/account", id, ebufp);
	PIN_FLIST_FLD_PUT(a_flistp, PIN_FLD_POID, objp, ebufp);

	/***********************************************************
	 * Add a search results marker.
	 *
	 * Making this NULL forces the read-object result we want.
	 ***********************************************************/
	a_flistp = (pin_flist_t *)NULL;
	PIN_FLIST_ELEM_PUT(flistp, a_flistp, PIN_FLD_RESULTS, 0, ebufp);

	/***********************************************************
	 * Call the DM to do the search.
	 ***********************************************************/
	PCM_OP(ctxp, PCM_OP_SEARCH, 0, flistp, &r_flistp, ebufp);

	/***********************************************************
	 * Get (and show) the results.
	 ***********************************************************/
	a_flistp = PIN_FLIST_ELEM_GET(r_flistp, PIN_FLD_RESULTS, 0, 0, ebufp);

	fprintf(stdout,"\nSimple Search Result Flist:\n");
	PIN_FLIST_PRINT(a_flistp, stdout, ebufp);

	/***********************************************************
	 * Clean up.
	 ***********************************************************/
	/* Free local memory */
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&flistp, NULL);

	/* Error? */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_read_obj_search error", ebufp);
	}

	return;
}

/*******************************************************************
 * Multi search.
 *
 *	Performs a read-fields search to get the poid, merchant
 *	and status of all non-billable accounts in the database.
 *	We walk	the results array and display each individually.
 *
 *	The "one arg equal" search for /account is 201.
 *
 *******************************************************************/
void
sample_read_flds_search(
	pcm_context_t	*ctxp,
	u_int64		database,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	pin_flist_t	*flistp = NULL;
	pin_flist_t	*a_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;

	poid_t		*objp = NULL;
	u_int64		id;
	u_int		vp;
	int32		rec_id;

	/***********************************************************
	 * Check the error buffer.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Allocate the flist for searching.
	 ***********************************************************/
	flistp = PIN_FLIST_CREATE(ebufp);

	/***********************************************************
	 * Allocate the search poid and give it to the flist.
	 ***********************************************************/
	id = (u_int64)201;

	objp = PIN_POID_CREATE(database, "/search", id, ebufp);
	PIN_FLIST_FLD_PUT(flistp, PIN_FLD_POID, objp, ebufp);

	/***********************************************************
	 * Add a search arguments array.
	 ***********************************************************/
	a_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_ARGS, 1, ebufp);

	/***********************************************************
	 * Add non-billable (= undefined) as the argument.
	 ***********************************************************/
	vp = PIN_BILL_TYPE_UNDEFINED;
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_BILL_TYPE, (void *)&vp, ebufp);

	/***********************************************************
	 * Add a search results array.
	 ***********************************************************/
	a_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_RESULTS, 0, ebufp);

	/***********************************************************
	 * Add markers for the fields we want to read.
	 ***********************************************************/
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_POID, (void *)NULL, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_MERCHANT, (void *)NULL, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_STATUS, (void *)NULL, ebufp);

	/***********************************************************
	 * Call the DM to do the search.
	 ***********************************************************/
	PCM_OP(ctxp, PCM_OP_SEARCH, 0, flistp, &r_flistp, ebufp);

	/***********************************************************
	 * Walk the results array.
	 ***********************************************************/
	a_flistp = PIN_FLIST_ELEM_GET_NEXT(r_flistp, PIN_FLD_RESULTS,
		&rec_id, 1, &cookie, ebufp);
	while (a_flistp != (pin_flist_t *)NULL) {

		/***************************************************
		 * Show this result.
		 ***************************************************/
		fprintf(stdout,"\nMulti Search Result(%d) Flist:\n", rec_id);
		PIN_FLIST_PRINT(a_flistp, stdout, ebufp);

		/***************************************************
		 * Get the next result.
		 ***************************************************/
		a_flistp = PIN_FLIST_ELEM_GET_NEXT(r_flistp,
			PIN_FLD_RESULTS, &rec_id, 1, &cookie, ebufp);
	}

	/***********************************************************
	 * Clean up.
	 ***********************************************************/
	/* Free local memory */
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&flistp, NULL);

	/* Error? */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_read_flds_search error", ebufp);
	}

	return;
}


/*******************************************************************
 * Step search.
 *
 *	Performs a read-object step search for services with
 *	md5 encrypted passwords. The first 10 such services are
 *	retrieved in two blocks of 5 services each.
 *
 *	The "one arg like" search for /service is 236.
 *
 *******************************************************************/
void
sample_step_search(
	pcm_context_t	*ctxp,
	u_int64		database,
	pin_errbuf_t	*ebufp)
{
	pin_flist_t	*flistp = NULL;
	pin_flist_t	*a_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;

	poid_t		*objp = NULL;
	u_int64		id;
	char		*authCode = "";
	pin_cookie_t	cookie = NULL;
	int32		rec_id;


	/***********************************************************
	 * Check the error buffer.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Allocate the flist for searching.
	 ***********************************************************/
	flistp = PIN_FLIST_CREATE(ebufp);

	/***********************************************************
	 * Allocate the search poid and give it to the flist.
	 ***********************************************************/
	id = (u_int64)236;

	objp = PIN_POID_CREATE(database, "/search", id, ebufp);
	PIN_FLIST_FLD_PUT(flistp, PIN_FLD_POID, objp, ebufp);

	/***********************************************************
	 * Add a search arguments array.
	 ***********************************************************/
	a_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_ARGS, 1, ebufp);

	/***********************************************************
	 * Put the password value in element 1 of the args array.
	 ***********************************************************/
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_PASSWD, (void *)authCode, ebufp);

	/***********************************************************
	 * Add a search results marker.
	 *
	 * Making this NULL forces the read-object result we want.
	 * The key here is the value 5, which says give me the
	 * first 5 results or objects.
	 ***********************************************************/
	a_flistp = (pin_flist_t *)NULL;
	PIN_FLIST_ELEM_PUT(flistp, a_flistp, PIN_FLD_RESULTS, 5, ebufp);

	/***********************************************************
	 * Call the DM to initiate the step search.
	 ***********************************************************/
	PCM_OP(ctxp, PCM_OP_STEP_SEARCH, 0, flistp, &r_flistp, ebufp);

        /***********************************************************
         * Walk the first results array.
         ***********************************************************/
	fprintf(stdout,"\n** The first 5 objects:\n");
        a_flistp = PIN_FLIST_ELEM_GET_NEXT(r_flistp, PIN_FLD_RESULTS,
                &rec_id, 1, &cookie, ebufp);
        while (a_flistp != (pin_flist_t *)NULL) {

                /***************************************************
                 * Show this result.
                 ***************************************************/
                fprintf(stdout,"\nStep Search Result(%d) Flist:\n", rec_id);
                PIN_FLIST_PRINT(a_flistp, stdout, ebufp);

                /***************************************************
                 * Get the next result.
                 ***************************************************/
                a_flistp = PIN_FLIST_ELEM_GET_NEXT(r_flistp,
                        PIN_FLD_RESULTS, &rec_id, 1, &cookie, ebufp);
        }

	/***********************************************************
	 * Recycle the results flist.
	 ***********************************************************/
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	/***********************************************************
	 * Call the DM to get the next 5 objects using step next.
	 ***********************************************************/
	PCM_OP(ctxp, PCM_OP_STEP_NEXT, 0, flistp, &r_flistp, ebufp);

        /***********************************************************
         * Walk the next results array.
         ***********************************************************/
	fprintf(stdout,"\n** The next 5 objects:\n");
	cookie = NULL;
        a_flistp = PIN_FLIST_ELEM_GET_NEXT(r_flistp, PIN_FLD_RESULTS,
                &rec_id, 1, &cookie, ebufp);
        while (a_flistp != (pin_flist_t *)NULL) {

                /***************************************************
                 * Show this result.
                 ***************************************************/
                fprintf(stdout,"\nStep Search Result(%d) Flist:\n", rec_id);
                PIN_FLIST_PRINT(a_flistp, stdout, ebufp);

                /***************************************************
                 * Get the next result.
                 ***************************************************/
                a_flistp = PIN_FLIST_ELEM_GET_NEXT(r_flistp,
                        PIN_FLD_RESULTS, &rec_id, 1, &cookie, ebufp);
        }

	/***********************************************************
	 * Recycle the results flist.
	 ***********************************************************/
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	/***********************************************************
	 * Call the DM to end step searching.
	 ***********************************************************/
	PCM_OP(ctxp, PCM_OP_STEP_END, 0, flistp, &r_flistp, ebufp);

	/***********************************************************
	 * Clean up.
	 ***********************************************************/
	/* Free local memory */
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&flistp, NULL);

	/* Error? */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_step_search error", ebufp);
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
	pin_errbuf_t	ebuf;
	int64		database;
	char		logfile[256];
	char		*c_ptr = (char *)NULL;
	char		*program;
	int32		err;

	/***********************************************************
	 * Clear the error buffer (for safety).
	 ***********************************************************/
	PIN_ERR_CLEAR_ERR(&ebuf);

	/***********************************************************
	 * Logging initialization
	 ***********************************************************/
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

	/***********************************************************
	 * Open the database context
	 ***********************************************************/
        PCM_CONNECT(&ctxp, &database, &ebuf);

	/***********************************************************
	 * Simple Read Object Search.
	 ***********************************************************/
	sample_read_obj_search(ctxp, database, &ebuf);

	/***********************************************************
	 * Create the service.
	 ***********************************************************/
	sample_read_flds_search(ctxp, database, &ebuf);

	/***********************************************************
	 * Step search sample.
	 ***********************************************************/
	sample_step_search(ctxp, database, &ebuf);

	/***********************************************************
	 * See if all went well.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(&ebuf)) {
		fprintf(stderr, "\nTest Failed, See Log File\n\n");
	} else {
		fprintf(stderr, "\nTest Suceeded\n\n");
	}

	/***********************************************************
	 * Close the database channel (a formality since we exit).
	 ***********************************************************/
        PCM_CONTEXT_CLOSE(ctxp, 0, &ebuf);

	return(0);
}
