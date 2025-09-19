/*
* Copyright (c) 1996, 2024, Oracle and/or its affiliates. 
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static	char	Sccs_id[] = "@(#)$Id: sample_who.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:17:36 nishahan Exp $";
#endif

/*******************************************************************
 * Simple program to demonstrate reverse mapping sessions to users.
 * Prints a list of all IP users currently 'logged in' to the system.
 *******************************************************************/

#include <stdio.h>
#include <libgen.h>
#include "pinlog.h"
#include "pin_errs.h"
#include "pcm.h"
#include "ops/base.h"
#include "pin_cust.h"


/*******************************************************************
 * sample_who_find_sessions():
 *
 *	Finds all active (end time = 0) dialup sessions that
 *	are in the database. Retrieves the whole session object
 *	so that we can report on whatever pieces we want.
 *
 *******************************************************************/
void
sample_who_find_sessions(
	pcm_context_t	*ctxp,
	u_int64		database,
	pin_flist_t	**o_flistpp,
	pin_errbuf_t	*ebufp)
{
	pin_flist_t	*flistp = NULL;
	pin_flist_t	*a_flistp = NULL;

	poid_t		*objp = NULL;
	u_int64		id;
	
	/*pin_binstr_t	my_binstr;*/
	u_int		my_uint;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);
	
	/*
	 * Allocate the flist for searching.
	 */
	flistp = PIN_FLIST_CREATE(ebufp);

	/*
	 * Allocate the search poid and give it to the flist.
	 *
	 * What we really want is a search with one equal and
	 * one like across event objects. Since that does not
	 * exist now, we use one that is close (ie some cheese)
	 * instead of creating another search object.
	 */
	id = (u_int64)230;	/* 3arg >=, <, like */

	objp = PIN_POID_CREATE(database, "/search", id, ebufp);
	PIN_FLIST_FLD_PUT(flistp, PIN_FLD_POID, objp, ebufp);

	/*
	 * Add a search arguments array.
	 */
	/*
	 * PIN_FLD_END_T = 0	(or, 0 <= end_t < 1)
	 */
	a_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_ARGS, 1, ebufp);
	my_uint = 0;
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_END_T, (void *)&my_uint, ebufp);

	a_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_ARGS, 2, ebufp);
	my_uint = 1;
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_END_T, (void *)&my_uint, ebufp);

	/*
	 * POID(type) = '/event/session/dialup' (like for flexabilty).
	 */
	a_flistp = PIN_FLIST_ELEM_ADD(flistp, PIN_FLD_ARGS, 3, ebufp);

	objp = PIN_POID_CREATE(database, "/event/session/dialup", 0, ebufp);
	PIN_FLIST_FLD_PUT(a_flistp, PIN_FLD_POID, objp, ebufp); 

	/*
	 * Add the results (robj) placeholder.
	 */
	PIN_FLIST_ELEM_SET(flistp, NULL, PIN_FLD_RESULTS, 0, ebufp);

	/*
	 * Call the DM to do the search.
	 */
	PCM_OP(ctxp, PCM_OP_SEARCH, 0, flistp, o_flistpp, ebufp);

	/*
	 * Clean up.
	 */
	/* Free local memory */
	PIN_FLIST_DESTROY_EX(&flistp, NULL);

	/* Error? */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_who_find_sessions error", ebufp);
	}

	return;
}

/*******************************************************************
 * sample_who_read_login():
 *
 *	Read the login from the given service obj.
 *******************************************************************/
void
sample_who_read_login(
	pcm_context_t	*ctxp,
	pin_flist_t	*o_flistp,
	pin_errbuf_t	*ebufp)
{
	pin_flist_t	*flistp = NULL;
	pin_flist_t	*r_flistp = NULL;

	void		*vp;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Construct the flist for reading the service login.
	 */
	flistp = PIN_FLIST_CREATE(ebufp);

	vp = PIN_FLIST_FLD_GET(o_flistp, PIN_FLD_SERVICE_OBJ, 0, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, vp, ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_LOGIN, (void *)NULL, ebufp);

	/*
	 * Read the login from the service.
	 */
	PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, flistp, &r_flistp, ebufp);

	/*
	 * Put the login on the input/output flist.
	 */
	vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_LOGIN, 0, ebufp);
	PIN_FLIST_FLD_SET(o_flistp, PIN_FLD_LOGIN, vp, ebufp);

	/*
	 * Clean up.
	 */
	/* Free local memory. */
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	/* Error? */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_who_read_login error", ebufp);
	}

	return;
}

/*******************************************************************
 * sample_who_find_logins():
 *
 *	Look up the login names for users with open sessions.
 *
 *******************************************************************/
void
sample_who_find_logins(
	pcm_context_t	*ctxp,
	pin_flist_t	*o_flistp,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	pin_flist_t	*flistp = NULL;

	int32		rec_id;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Walk the results array.
	 */
	flistp = PIN_FLIST_ELEM_GET_NEXT(o_flistp, PIN_FLD_RESULTS,
		&rec_id, 1, &cookie, ebufp);
	while (flistp != (pin_flist_t *)NULL) {

		/*
		 * Find this login.
		 */
		sample_who_read_login(ctxp, flistp, ebufp);

		/*
		 * Get the next result.
		 */
		flistp = PIN_FLIST_ELEM_GET_NEXT(o_flistp,
			PIN_FLD_RESULTS, &rec_id, 1, &cookie, ebufp);
	}

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_who_find_logins error", ebufp);
	}

	return;
}

/*******************************************************************
 * sample_who_show_one():
 *
 *	Present a single outstanding session to the user.
 *
 *******************************************************************/
void
sample_who_show_one(
	pin_flist_t	*o_flistp,
	pin_errbuf_t	*ebufp)
{
	pin_flist_t	*flistp = NULL;

	char		*login;
	time_t		start_t = 0;
	pin_binstr_t	*ipaddr;
	void		*vp = NULL;
	char		str_ctime[32] = "";

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Snarf the data we want to show.
	 */
	login = (char *)PIN_FLIST_FLD_GET(o_flistp,
		PIN_FLD_LOGIN, 0, ebufp);
	vp = PIN_FLIST_FLD_GET(o_flistp, PIN_FLD_START_T, 0, ebufp);
	if(vp) {
		start_t = *(time_t *)vp;
	}

	flistp = PIN_FLIST_SUBSTR_GET(o_flistp,
		PIN_FLD_DIALUP, 0, ebufp);
	ipaddr = (pin_binstr_t *)PIN_FLIST_FLD_GET(flistp,
		PIN_FLD_IPADDR, 0, ebufp);

	/*
	 * Print it out.
	 */
	if (!(PIN_ERR_IS_ERR(ebufp))) {

		if((login != NULL) && (ipaddr != NULL) && (ipaddr->data != NULL)){
			pin_ctime(&start_t, str_ctime, PIN_ERRLOC_APP, ebufp);
			fprintf(stdout, "\t%s\t\t%d.%d.%d.%d\t%s", login,
				(ipaddr->data[0] & 0xFF), (ipaddr->data[1] & 0xFF),
				(ipaddr->data[2] & 0xFF), (ipaddr->data[3] & 0xFF),
				str_ctime);
		}
	}

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_who_show_one error", ebufp);
	}

	return;
}

/*******************************************************************
 * sample_who_show_list():
 *
 *	Present the results to the user.
 *
 *******************************************************************/
void
sample_who_show_list(
	pin_flist_t	*o_flistp,
	pin_errbuf_t	*ebufp)
{
	pin_cookie_t	cookie = NULL;
	pin_flist_t	*flistp = NULL;

	int32		rec_id;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	fprintf(stdout, "\nOutstanding Sessions:\n\n");

	/*
	 * Walk the results array.
	 */
	flistp = PIN_FLIST_ELEM_GET_NEXT(o_flistp, PIN_FLD_RESULTS,
		&rec_id, 1, &cookie, ebufp);
	while (flistp != (pin_flist_t *)NULL) {

		/*
		 * Show this session.
		 */
		sample_who_show_one(flistp, ebufp);

		/*
		 * Get the next result.
		 */
		flistp = PIN_FLIST_ELEM_GET_NEXT(o_flistp,
			PIN_FLD_RESULTS, &rec_id, 1, &cookie, ebufp);
	}

	fprintf(stdout, "\n\n");

	/*
	 * Error?
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"sample_who_show_list error", ebufp);
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
	pin_errbuf_t	ebuf;
	int64		database;
        char            logfile[256];
        char            *c_ptr = (char *)NULL;
	char		*program;
        int32           err;


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

        pin_strlcpy(logfile, "default.pinlog",sizeof(logfile));
        pin_conf(program, "logfile", PIN_FLDT_STR, (caddr_t *)&(c_ptr), &err);
        if (c_ptr != (char *)NULL) {
                pin_strlcpy(logfile, c_ptr, sizeof(logfile));
                PIN_FREE_EX(&c_ptr);
        }
        PIN_ERR_SET_LOGFILE(logfile);

	/*
	 * Open the database context
	 */
        PCM_CONNECT(&ctxp, &database, &ebuf);

	/*
	 * Search for open login sessions.
	 */
	sample_who_find_sessions(ctxp, database, &flistp, &ebuf);

	/*
	 * Map the session to a login.
	 */
	sample_who_find_logins(ctxp, flistp, &ebuf);

	/*
	 * Output the results.
	 */
	sample_who_show_list(flistp, &ebuf);

	/*
	 * See if all went well.
	 */
	if (PIN_ERR_IS_ERR(&ebuf)) {
		fprintf(stderr, "\nAn Error Occurred, See Log File\n\n");
	}

	/*
	 * Clean up (a formality since we exit).
	 */
	/* Free local memory */
	PIN_FLIST_DESTROY_EX(&flistp, NULL);

	/* Close the database channel */
        PCM_CONTEXT_CLOSE(ctxp, 0, &ebuf);

	return(0);
}

