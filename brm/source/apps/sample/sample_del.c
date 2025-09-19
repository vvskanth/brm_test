/*
* Copyright (c) 1996, 2024, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: sample_del.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:17:32 nishahan Exp $";
#endif

/*******************************************************************
 * Looks up an account by one of it's service logins and then
 * deletes the account and all its related objects.
 *
 * Usage: sample_del <service_type> <login>
 *
 *******************************************************************/

/*******************************************************************
 *******************************************************************/
	/***********************************************************
	 ***********************************************************/

#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <libgen.h>
#include "pcm.h"
#include "ops/cust.h"
#include "ops/act.h"
#include "pin_bill.h"
#include "pin_cust.h"
#include "pin_errs.h"
#include "pinlog.h"

/*******************************************************************
 * Print a usage statement.
 *******************************************************************/
void
usage(char    *prog)
{

	fprintf(stderr, "\nUsage: %s <service_type> <login>\n", prog);

	return;
}

/*******************************************************************
 * Main
 *
 *	This sets up the parameters and then calls the FM.
 *
 *******************************************************************/
int main(
	int		argc,
	char		*argv[])
{
	pcm_context_t	*ctxp;

	pin_flist_t	*flistp = NULL;
	pin_flist_t	*a_flistp = NULL;
	pin_flist_t	*r_flistp = NULL;

	poid_t		*acct_pdp = NULL;
	poid_t		*srvc_pdp = NULL;

	u_int		opcode;

	int64		database;

	void		*vp;

	char		logfile[256];
	char		*c_ptr = (char *)NULL;
	char		*program;
	int32		err;

	pin_errbuf_t	ebuf;

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
	 * Check the input.
	 */
	if (argc != 3) {
		usage(argv[0]);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, "bad # of arguments");
		exit(1);
	}

	/*
	 * Initialize PCM connection.
	 */
	PCM_CONNECT(&ctxp, &database, &ebuf);
	if (PIN_ERR_IS_ERR(&ebuf)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"pcm_connect error", &ebuf);
		exit(2);
	}

	/*
	 * Create the poid prototype.
	 */
	srvc_pdp = PIN_POID_CREATE(database, argv[1], 0, &ebuf);

	/*
	 * Look up the account.
	 */
	flistp = PIN_FLIST_CREATE(&ebuf);

	vp = (void *)srvc_pdp;
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_POID, vp, &ebuf);

	vp = (void *)argv[2];
	PIN_FLIST_FLD_SET(flistp, PIN_FLD_LOGIN, vp, &ebuf);

	opcode = PCM_OP_ACT_FIND;
	PCM_OP(ctxp, opcode, 0, flistp, &a_flistp, &ebuf);

	/*
	 * Did we find anything?
	 */
	acct_pdp = PIN_FLIST_FLD_GET(a_flistp, PIN_FLD_ACCOUNT_OBJ, 0, &ebuf);

	if (!PIN_POID_IS_NULL(acct_pdp)) {

		/*
		 * Advisory (or add confirm?)
		 */
		fprintf(stdout, "Deleting Account:\t");
		pin_poid_print(acct_pdp, 0, &ebuf);
		fprintf(stdout, "\n");

		/*
		 * Put the real account poid on the flist
		 * (The account_obj field will just be ignored)
		 */
		vp = (void *)acct_pdp;
		PIN_FLIST_FLD_SET(a_flistp, PIN_FLD_POID, vp, &ebuf);

		/*
		 * Delete the account.
		 */
		opcode = PCM_OP_CUST_DELETE_ACCT;
		PCM_OP(ctxp, opcode, 0, a_flistp, &r_flistp, &ebuf);

		/*
		 * Did we delete or not?
		 */
		if (PIN_ERR_IS_ERR(&ebuf)) {

			fprintf(stdout, "Account Deletion Error Occurred.\n");
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"op_cust_delete_acct error", &ebuf);

		} else {

			fprintf(stdout, "Account Deleted.\n");

		}

	} else {

		/*
		 * FIND error.
		 */
		fprintf(stdout, "Unable to locate Account.\n");
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_act_find error", &ebuf);

	}

	/*
	 * Close the PCM context.
	 */
	PCM_CONTEXT_CLOSE(ctxp, 0, &ebuf);

	/*
	 * Clean up.
	 */
	/* Free local memory. */
	PIN_FLIST_DESTROY_EX(&flistp, NULL);
	PIN_FLIST_DESTROY_EX(&a_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&r_flistp, NULL);

	/* No errors. */
	return(0);
}

