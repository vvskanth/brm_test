/*
* Copyright (c) 1996, 2024, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: sample_act.c /cgbubrm_7.5.0.portalbase/1 2023/11/24 04:17:27 sreejs Exp $";
#endif

/*******************************************************************
 * This file contains sample code that generates an activity
 * or session event for a service object. Specifics of the
 * events are taken from the user via command line options.
 *
 * TODO: Add parameter for setting the event (start) time.
 *******************************************************************/

/*******************************************************************
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <libgen.h>

#include "pcm.h"
#include "ops/act.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "pin_os.h"
#include "pin_os_getopt.h"
#include "pin_string_wrapper.h"


#define FILE_SOURCE_ID  "sample_act.c(7)"


/* Convenience macros */
#define MKTIME(tm)      (mktime(tm))


/*******************************************************************
 * Forward declarations.
 *******************************************************************/
void sample_act_find(
	pcm_context_t	*ctxp,
	int64		database,
	pin_flist_t	*c_flistp,
	pin_errbuf_t	*ebufp);
void sample_act_activity(
	pcm_context_t	*ctxp,
        pin_flist_t	*c_flistp,
        pin_errbuf_t	*ebufp);
void sample_act_session(
	pcm_context_t	*ctxp,
        pin_flist_t	*c_flistp,
        pin_errbuf_t	*ebufp);
void
sample_act_fill_telephony_call(
	pin_flist_t	*a_flistp, 
	pin_flist_t	*c_flistp, 
	pin_errbuf_t	*ebufp);
void sample_act_op(
	pcm_context_t	*ctxp,
	int32		opcode,
	int32		opflag,
	pin_flist_t	*i_flistp,
	pin_flist_t	**r_flistpp,
	pin_errbuf_t	*ebufp);
time_t sample_act_get_time(
	char 		*timestr,
	char 		*program);

/*******************************************************************
 * Global data.
 *******************************************************************/
extern int	opterr;		/* for getopt(3c) */
extern int	optind;		/* for getopt(3c) */
extern char	*optarg;	/* for getopt(3c) */

int32	 	flistdebug = 0;
int32	 	testmode = 0;
int32 		verbose = 0;

/*******************************************************************
 * Print a usage statement.
 *******************************************************************/
void
usage(prog)
	char	*prog;
{
	fprintf(stderr,
		"Usage: %s %s\n%s\n",
		prog, "[-f] [-h] [-t] [-v]",
		"\t-e activity|session [-c <custom_subtype>]\n"
		"\t-s <service_type> -l <service_login>\n"
		"\t[-n <RUM Name> -q <quantity> [-u <Unit>] ]\n"
		"\t[-r <rate_plan_name> -i <impact category> [-g <lineage>]]\n" 
		"\t[-b <event time (as MM:DD:YY:HH:MM:SS or unixtime)>]\n"
		"\t[-d <duration (in secs only for sessions)>]\n"
		"\t[-A <ANI>] [-D <DNIS>] [-Y <call_type>]\n"
		"\t[-X <int_indicator>] [-N <new Timezone ID String>]\n"
		"\t[-T <old timezone offset>]\n"
		"\tWhere unit is one of the following:\n"
		"\tSec: 1, Min: 2, Hour: 3, Day: 4, Byte: 11, kb: 12, mb: 13, gb: 14\n"
	);
}

/*******************************************************************
 * Main
 *******************************************************************/
int
main(argc, argv)
	int	argc;
	char	**argv;
{
	pcm_context_t		*ctxp;	
	pin_flist_t		*flistp = NULL;
	pin_errbuf_t		ebuf;

	int64			database;
	int32			opcode = 0;

	char			c;
	char			*program;
	char			logfile[256];
	char			*c_ptr = (char *)NULL;
	int32			err;

	int32			duration = 0;
	int			call_type = 0;
	int			int_indicator = 0;
	pin_decimal_t		*quantity = NULL;
	int32			unit = 0;
	double			orig_qty = 0;
	char			inp_qty[256];

	time_t			start_t = pin_virtual_time(NULL);
	int32			quantity_specified = 0;

	pin_flist_t		*rum_flistp = NULL;

	PIN_ERR_CLEAR_ERR(&ebuf);

	/*
	 * Logging init
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
	 * Initialize expected parameters.
	 */
	quantity = pin_decimal("1.0", &ebuf);
	flistp = PIN_FLIST_CREATE(&ebuf);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_PROGRAM_NAME, (void *)program, &ebuf);

	/*
	 * Parse the command line args
	 */
	while ((c = (char)getopt(argc, argv, 
		"c:e:n:q:u:r:i:g:d:s:l:b:A:D:Y:X:O:I:P:T:N:fhtv")) != (char)EOF) {

		switch (c) {
		case 'e':
			/* What event? */
			if (!strcmp(optarg, "session")) {
				opcode = PCM_OP_ACT_LOAD_SESSION;
			}else if (!strcmp(optarg, "activity")) {
				opcode = PCM_OP_ACT_ACTIVITY;
			} else {
				fprintf(stderr, "%s: unknown event type %s\n",
					program, optarg);
				usage(program);
				exit(1);
				/******/
			}
			break;
		case 'c':
			/* What event subtype? */
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_OBJ_TYPE,
				(void *)optarg, &ebuf);
			break;
		case 'n':
			/* Rum name? */
			rum_flistp = PIN_FLIST_ELEM_ADD(flistp,
				PIN_FLD_CANDIDATE_RUMS, 0, &ebuf);
			PIN_FLIST_FLD_SET(rum_flistp, PIN_FLD_RUM_NAME,
				(void *) optarg, &ebuf);	
			break;
		case 'q':
			/* Rate quantity? */
			quantity_specified = 1;
			sscanf((char *)optarg, "%s", inp_qty);
			quantity = pin_decimal(inp_qty, &ebuf);
			break;
		case 'u':
			/* Unit */
			unit = (int32)strtol(optarg, NULL, 0);
			break;
		case 'r':
			/* rate plan name */
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_RATE_PLAN_NAME, 
				(void *)optarg, &ebuf);	
			break;
		case 'i':
			/* impact category */
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_IMPACT_CATEGORY, 
				(void *)optarg, &ebuf);
			break;
		case 'g':
			/* lineage */
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_LINEAGE, 
				(void *)optarg, &ebuf);
			break;
		case 'd':
			/* Duration in seconds. */
			duration = (int32)strtol(optarg, NULL, 0);
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_COUNT,
				(void *)&duration, &ebuf);
			break;
		case 'b':
			/* Event time? */
			start_t = sample_act_get_time(optarg, program);
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_END_T,
				(void *)&start_t, &ebuf);
			break;
		case 's':
			/* Service type? */
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_TYPE_STR,
				(void *)optarg, &ebuf);
			break;
		case 'l':
			/* Login? */
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_LOGIN,
				(void *)optarg, &ebuf);
			break;
		case 'A':
			/* ANI? */
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_ANI,
				(void *)optarg, &ebuf);
			break;
		case 'D':
			/* DNIS? */
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_DNIS,
				(void *)optarg, &ebuf);
			break;
		case 'Y':
			/* Call Type? */
			call_type = pin_strtoi(optarg, PIN_DECIMAL_BASE,
				 PIN_MANDATORY_EBUF, "sample_act.c-main-1", &ebuf);
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_CALL_TYPE, 
				(void *)&call_type, &ebuf);
			break;
		
		case 'X':
			/* International Indicator? */
			int_indicator = pin_strtoi(optarg, PIN_DECIMAL_BASE,
				PIN_MANDATORY_EBUF, "sample_act.c-main-2", &ebuf);
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_INTERNATIONAL_IND, 
				(void *)&int_indicator, &ebuf);
			break;
		
		case 'T':
			/* OLD TIMEZONE OFFSET */
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_TIMEZONE,
				(void *)optarg, &ebuf);
			break;
		case 'N':
			/* NEW TIMEZONE_ID STRING */
			PIN_FLIST_FLD_SET(flistp, PIN_FLD_TIMEZONE_ID,
				(void *)optarg, &ebuf);
			break;
		case 'f':
			/* Flist debugging on? */
			PIN_ERR_SET_LEVEL(PIN_ERR_LEVEL_DEBUG);
			flistdebug = 1;
			break;
		case 't':
			/* Test mode on? */
			testmode = 1;
			break;
		case 'v':
			/* Verbose status on? */
			verbose = 1;
			break;
		case 'h':
			/* help = usage */
			usage(program);
			exit(0);
			/******/
		case '?':
			usage(program);
			exit(1);
			/******/
		}
	}



	if (duration != 0) {
		if (opcode == PCM_OP_ACT_ACTIVITY) {
			usage(program);
			exit(1);
			/******/
		}
	}
		
	if (quantity_specified) {
		if (opcode != PCM_OP_ACT_ACTIVITY
			|| !rum_flistp ) {
			fprintf(stderr, 
				"sample-act: Rum name or unit is missing\n\n");
			usage(program);
			exit(1);
			/******/
		}

		PIN_FLIST_FLD_SET(rum_flistp, PIN_FLD_QUANTITY,
				  (void *)quantity, &ebuf);
		PIN_FLIST_FLD_SET(rum_flistp, PIN_FLD_UNIT,
				  (void *)&unit, &ebuf);
	}

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "Configuration flist", flistp);

	/*
	 * Open pcm connection.
	 */
	PCM_CONNECT(&ctxp, &database, &ebuf);

	if (PIN_ERR_IS_ERR(&ebuf)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"error opening pcm_connection", &ebuf);
		if (verbose) {
			fprintf(stderr, "%s: error (see pinlog)\n", program);
		}
		exit(2);
		/******/
	}

	if (verbose) {
		fprintf(stdout, "%s: pcm_connection open\n", program);
	}

	/*
	 * Find the service.
	 */
	sample_act_find(ctxp, database, flistp, &ebuf);

	if (PIN_ERR_IS_ERR(&ebuf)) {
		PIN_FLIST_DESTROY_EX(&flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"error locating service object", &ebuf);
		if (verbose) {
			fprintf(stderr, "%s: error (see pinlog)\n", program);
		}
		exit(3);
		/******/
	}

	if (verbose) {
		fprintf(stdout, "%s: service object found\n", program);
	}

        /*
	 * The rest depends on whether this is a session or an activity
	 */
	switch (opcode) {
	case PCM_OP_ACT_ACTIVITY:
		sample_act_activity(ctxp, flistp, &ebuf);
		break;
	case PCM_OP_ACT_LOAD_SESSION:
		sample_act_session(ctxp, flistp, &ebuf);
		break;
	}

	if (PIN_ERR_IS_ERR(&ebuf)) {
		PIN_FLIST_DESTROY_EX(&flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"error generating event", &ebuf);
		if (verbose) {
			fprintf(stderr, "%s: error (see pinlog)\n", program);
		}
		exit(3);
		/******/
	}

	if (verbose) {
		fprintf(stdout, "%s: event generated successfully\n", program);
	}

	/*
	 * Clean up
	 */
	PIN_FLIST_DESTROY_EX(&flistp, NULL);

        /*
	 * Close the context.
	 */
        PCM_CONTEXT_CLOSE(ctxp, 0, &ebuf);

	if (verbose) {
		fprintf(stdout, "%s: pcm_connection closed\n", program);
	}

	return(0);
}

/*******************************************************************
 * sample_act_find()
 *
 *	Find the poids for the given login and service type.
 *
 *******************************************************************/
void
sample_act_find(
	pcm_context_t	*ctxp,
	int64		database,
	pin_flist_t	*c_flistp,
	pin_errbuf_t	*ebufp)
{
	pin_flist_t	*flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	poid_t		*pdp = NULL;
	char		*strp = NULL;
	int32		opcode;

	/*
	 * Create the PCM_OP_ACT_FIND input flist
	 */
	flistp = PIN_FLIST_CREATE(ebufp);

	strp = (char *)PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_TYPE_STR, 0, ebufp);
	pdp = PIN_POID_CREATE(database, strp, 0, ebufp);
	PIN_FLIST_FLD_PUT(flistp, PIN_FLD_POID, pdp, ebufp);

	strp = (char *)PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_LOGIN, 0, ebufp);
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_LOGIN, (void *)strp, ebufp);

	/*
	 * Call PCM_OP_ACT_FIND to find the service
	 */
        opcode = PCM_OP_ACT_FIND;
        sample_act_op(ctxp, opcode, 0, flistp, &r_flistp, ebufp);

        /*
         * Did we find anything?
         */
        pdp = (poid_t *)PIN_FLIST_FLD_GET(r_flistp,
		PIN_FLD_ACCOUNT_OBJ, 0, ebufp);

	if (!PIN_POID_IS_NULL(pdp)) {
		PIN_FLIST_FLD_SET(c_flistp, PIN_FLD_ACCOUNT_OBJ,
			(void *)pdp, ebufp);

        	pdp = (poid_t *)PIN_FLIST_FLD_GET(r_flistp,
			PIN_FLD_POID, 0, ebufp);

		PIN_FLIST_FLD_SET(c_flistp, PIN_FLD_SERVICE_OBJ,
			(void *)pdp, ebufp);

	} else {
		pin_set_err(ebufp, PIN_ERRLOC_APP,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_UNKNOWN_POID, PIN_FLD_LOGIN, 0, 0);
	}

	/*
	 * Cleanup
	 */
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	return;
}

/*******************************************************************
 * sample_act_activity()
 *
 *	Create an activity event for the specified service.
 *
 *******************************************************************/
void
sample_act_activity(
	pcm_context_t	*ctxp,
        pin_flist_t	*c_flistp,
        pin_errbuf_t	*ebufp)
{
	pin_flist_t	*a_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	pin_flist_t	*flistp = NULL;
	void 		*vp = NULL;
	int32		opcode;
	int32		opflag = 0;
	pin_flist_t		*rum_flistp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
        PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Create the PCM_OP_ACT_ACTIVITY input flist.
	 */
	a_flistp = PIN_FLIST_CREATE(ebufp);

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_ACCOUNT_OBJ, 0, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_POID, vp, ebufp);

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_SERVICE_OBJ, 0, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_SERVICE_OBJ, vp, ebufp);

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_PROGRAM_NAME, 0, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_PROGRAM_NAME, vp, ebufp);

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_OBJ_TYPE, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_OBJ_TYPE, vp, ebufp);
	}

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_END_T, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_START_T, vp, ebufp);
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_END_T, vp, ebufp);
	}

	/* TIMEZONE_ID */
	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_TIMEZONE_ID, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_TIMEZONE_ID, vp, ebufp);
	}

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_RATE_PLAN_NAME, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_RATE_PLAN_NAME, vp, ebufp);
	}

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_IMPACT_CATEGORY, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_IMPACT_CATEGORY, vp, ebufp);
	}

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_LINEAGE, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_LINEAGE, vp, ebufp);
	}

	rum_flistp = PIN_FLIST_ELEM_GET(c_flistp, PIN_FLD_CANDIDATE_RUMS, 
		0, 1, ebufp);

	if(rum_flistp) {
		PIN_FLIST_ELEM_SET(a_flistp, rum_flistp,
			PIN_FLD_CANDIDATE_RUMS, 0, ebufp);
	}

        /*
         * Call PCM_OP_ACT_ACTIVITY to generate the event.
         */
        opcode = PCM_OP_ACT_ACTIVITY;
        sample_act_op(ctxp, opcode, 0, a_flistp, &r_flistp, ebufp);

	/*
	 * Should we get the event poid for display?
	 */

	/*
	 * Clean up.
	 */
	PIN_FLIST_DESTROY_EX(&a_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

        return;
}

/*******************************************************************
 * sample_act_session()
 *
 *	Create a session event for the specified service.
 *
 *******************************************************************/
void
sample_act_session(
	pcm_context_t	*ctxp,
        pin_flist_t	*c_flistp,
        pin_errbuf_t	*ebufp)
{
	pin_flist_t	*a_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;
	pin_flist_t	*flistp = NULL;
	void 		*vp = NULL;
	time_t		now;
	int32		opcode;
	int32		opflag = 0;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
        PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Create the PCM_OP_ACT_LOAD_SESSION input flist.
	 */
	a_flistp = PIN_FLIST_CREATE(ebufp);

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_ACCOUNT_OBJ, 0, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_POID, vp, ebufp);

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_SERVICE_OBJ, 0, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_SERVICE_OBJ, vp, ebufp);

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_PROGRAM_NAME, 0, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_PROGRAM_NAME, vp, ebufp);

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_OBJ_TYPE, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_OBJ_TYPE, vp, ebufp);
		if (strcmp((char*)vp, "/call/telephony") == 0) {
			sample_act_fill_telephony_call(a_flistp, c_flistp, ebufp);
		}
	}

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_END_T, 1, ebufp);
	if (vp == (void *)NULL) {
		now = pin_virtual_time(NULL);
		vp = &now;
	} else {
		now = *(time_t*)vp;
	}

	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_START_T, vp, ebufp);
	PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_END_T, vp, ebufp);

	/* If duration was specified, adjust START_T */
	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_COUNT, 1, ebufp);
	if (vp != (void *)NULL) {
		now -= *((time_t *)vp);
		vp = &now;
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_START_T, vp, ebufp);
	}

	/* TIMEZONE_ID */
	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_TIMEZONE_ID, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_TIMEZONE_ID, vp, ebufp);
	}

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_RATE_PLAN_NAME, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_RATE_PLAN_NAME, vp, ebufp);
	}

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_IMPACT_CATEGORY, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_IMPACT_CATEGORY, vp, ebufp);
	}

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_LINEAGE, 1, ebufp);
	if (vp != (void *)NULL) {
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_LINEAGE, vp, ebufp);
	}

	/*
	 * Call PCM_OP_ACT_LOAD_SESSION to generate the event.
         */
        opcode = PCM_OP_ACT_LOAD_SESSION;
        sample_act_op(ctxp, opcode, 0, a_flistp, &r_flistp, ebufp);

	/*
	 * Should we get the event poid for display?
	 */

	/*
	 * Clean up.
	 */
	PIN_FLIST_DESTROY_EX(&a_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

        return;
}

void
sample_act_fill_telephony_call(
	pin_flist_t	*a_flistp, 
	pin_flist_t	*c_flistp, 
	pin_errbuf_t	*ebufp)
{
	pin_flist_t *call_flistp = NULL;
	void 	*vp = NULL;
	int	*ip = NULL;		
	int	call_type = 0;
	int 	int_indicator = 0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);
	

	call_flistp = PIN_FLIST_SUBSTR_ADD(a_flistp, PIN_FLD_INHERITED_INFO, 
		ebufp);
	call_flistp = PIN_FLIST_SUBSTR_ADD(call_flistp, PIN_FLD_TELEPHONY, 
		ebufp);

	/* Get the input values for ANI and DNIS */
	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_ANI, 1, ebufp);
	PIN_FLIST_FLD_SET(call_flistp, PIN_FLD_ANI, (void *)(vp ? vp : "14083434400"), 
		ebufp);

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_DNIS, 1, ebufp);
	PIN_FLIST_FLD_SET(call_flistp, PIN_FLD_DNIS, (void *)(vp ? vp : "14083434401"), 
		ebufp);  

	ip = (int *)PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_CALL_TYPE, 1, ebufp);
	if (ip == (int *)NULL) {
		call_type = 1;
	}
	else
		call_type = *ip;
	PIN_FLIST_FLD_SET(call_flistp, PIN_FLD_CALL_TYPE, (void *)&call_type, 
		ebufp);


	ip = (int *)PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_INTERNATIONAL_IND, 1, 
		ebufp);
	if (ip == (int *)NULL) {
		int_indicator = 1;
	}
	else
		int_indicator = *ip;
	PIN_FLIST_FLD_SET(call_flistp, PIN_FLD_INTERNATIONAL_IND, 
		(void *)&int_indicator, ebufp);

	vp = PIN_FLIST_FLD_GET(c_flistp, PIN_FLD_LOGIN, 0, ebufp);
	PIN_FLIST_FLD_SET(call_flistp, PIN_FLD_LOGIN, (void *)vp, ebufp);
}



/*******************************************************************
 * sample_act_op():
 *
 *	Wrapper to PCM_OP to handle flistdebug and testmode flags.
 *
 *******************************************************************/
void
sample_act_op(
	pcm_context_t	*ctxp,
	int32		opcode,
	int32		opflag,
	pin_flist_t	*i_flistp,
	pin_flist_t	**r_flistpp,
	pin_errbuf_t	*ebufp)
{
	pin_flist_t	*flistp = NULL;
	pin_errbuf_t	junk;
	char		msg[1024];

	PIN_ERR_CLEAR_ERR(&junk);

	/*
	 * Optional errbuf
	 */
	if (ebufp == (pin_errbuf_t *)NULL) {
		ebufp = &junk;
	}

	/*
	 * Debug... the input. 
	 */
	pin_snprintf(msg, sizeof(msg),  "%s input flist%c", pcm_opcode_to_opname(opcode),'\0');
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, msg, i_flistp);

	/*
	 * Testmode?
	 */
	if (testmode) {
		opflag |= PCM_OPFLG_CALC_ONLY;
	}

	/*
	 * Call PCM_OP
	 */
	PCM_OP(ctxp, opcode, opflag, i_flistp, &flistp, ebufp);

	/*
	 * Debug... the results. 
	 */
	pin_snprintf(msg, sizeof(msg), "%s result flist%c", pcm_opcode_to_opname(opcode),'\0');
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, msg, flistp);

	/*
	 * Optional results?
	 */
	if (r_flistpp != NULL) {
		*r_flistpp = flistp;
	} else {
		PIN_FLIST_DESTROY_EX(&flistp, NULL);
	}

	return;
}

/*******************************************************************
 * sample_act_get_time():
 *
 *	Funtion to convert time string to unix time value
 *
 *******************************************************************/
time_t
sample_act_get_time(
	char 	*timestr,
	char 	*program) 
{
    	struct tm               *tm;
    	time_t                  timeval, current_time;
    	int                     mm, dd, yy, hh, nn, ss, rv;
    	char                    delimiter[7];

	/* See if we have a valid date */
	rv = sscanf(timestr, 
		    "%d%6[/.:-]%d%6[:/.-]%d%6[:/.-]%d%6[:/.-]%d%6[:/.-]%d", 
		    &mm, delimiter, &dd, delimiter, &yy, delimiter,
		    &hh, delimiter, &nn, delimiter, &ss);
	switch(rv) {
	case 1:
		/* seems like we got unix time format */
		return ((time_t)mm);
	case 11:
		break;
	default:
	    	/* Unrecognized format! return without doing anything */
	    	fprintf(stderr, "%s: bad value for time\n", program);
		usage(program);
		exit(1);
		/******/
	}

	/* fprintf(stdout, "%d %d %d %d %d %d\n", mm, dd, yy, hh, nn, ss); */

    	/* Get current time and localtime (and fill in some defaults) */
    	time(&current_time);
    	tm = localtime(&current_time);
	tm->tm_mday	= dd;
	tm->tm_mon 	= mm - 1;
	/*
	 * Massage year for y2k compliance...
	 */
	if (yy < 70) {
		/* assume years less than 70 are after 2000 */
		yy += 100;
	} else if (yy > 1900) {
		yy -= 1900;
	}
	tm->tm_year 	= yy;
	tm->tm_sec 	= ss;
	tm->tm_min 	= nn;
	tm->tm_hour 	= hh;
	tm->tm_isdst	= -1;

	timeval = MKTIME(tm);

    	/* fprintf(stdout, "%d\n", (int)timeval); */

	return (timeval);
}
