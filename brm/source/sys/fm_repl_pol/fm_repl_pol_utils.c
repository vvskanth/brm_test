/*******************************************************************
 *
 * @(#) %full_filespec: fm_repl_pol_utils.c~10:csrc:1 %
 *
 *      Copyright (c) 1999 - 2024 Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation or
 *      its licensors and may be used, reproduced, stored or transmitted only
 *      in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)%full_filespec: fm_repl_pol_utils.c~10:csrc:1 %";
#endif

#include <stdio.h>

#include "pcm.h"
#include "ops/repl.h"
#include "pinlog.h"
#include "fm_repl_pol.h"

#define FILE_LOGNAME "fm_repl_pol_utils.c(1.6)"

int64
fm_repl_pol_get_ldap_db()
{
	static int64    db_id = 0;
	poid_t		*a_pdp = NULL;
	int32		err = 0;

	if (db_id != 0)
		return db_id;


	pin_conf("fm_repl_pol", "ldap_db", PIN_FLDT_POID,
		 (caddr_t *)&a_pdp, &err);

	if (a_pdp == NULL) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
				"ldap_db parameter error in pin.conf");
		db_id = 6;
	}
	else {
		db_id = pin_poid_get_db(a_pdp);
		PIN_POID_DESTROY_EX(&a_pdp, NULL);
	}

	return db_id;
}

int64
fm_repl_pol_trnsform_ldap_db(int64 db_id)
{
	/*
	 * DM_LDAP does not currently have a way to generate POID_IDs.
	 * We need to compose a POID and pass it in. We do this by
	 * transforming the POID of a storable object from dm_oracle
	 * or dm_odbc in some fashion.
	 *
	 * Prior to Infranet 6.0, this was done as follows:
	 * The DB portion of the storable object POID was replaced
	 * with the DB portion of ldap_db CM pin.conf entry.
	 * For example, if the storable object POID was
	 * 0.0.0.1 /account 42, and ldap_db was 0.0.0.6 / 0
	 * then the resulting POID would be 0.0.0.6 /account 42.
	 *
	 * In Infranet 6.0, the database number partitioning is getting
	 * redefined to accommodate support for multi-db. The last piece
	 * (lower order 16 bits) is reserved for dm_oracle and dm_odbc
	 * datastores. To accommodate this, and to keep the POID generation
	 * simpler, the following is done:
	 *
	 * The ldap_db is assumed to be of the form X.X.X.0 / 0.
	 * The DB portion of the above is ORed with the DB portion of the
	 * storable object POID.
	 * For example, using the cases above, the resulting POID from
	 * this function would be X.X.X.1 /account 42.
	 *
	 * Please consult with Portal Technical Support if changing this
	 * mapping algorithm.
	 *
	 */
	int64   ldapdb_id = fm_repl_pol_get_ldap_db();
	return ((ldapdb_id & ~0xFFFFL) | db_id);
}
int
fm_repl_pol_get_user_scheme()
{
	static int	user_scheme = -1;
	int32		err = 0;
	int		*scheme = NULL;
	char		msg[BUFSIZ];

	if (user_scheme != -1)
		return user_scheme;

	pin_conf("fm_repl_pol", "user_scheme", PIN_FLDT_INT,
		 (caddr_t *)&scheme, &err);

	/*
	 * We use user scheme by default
	 */
	if ((scheme != NULL) && (*scheme != 0))
		user_scheme = 1;
	else
		user_scheme = 0;

	pin_snprintf(msg, sizeof(msg), "value of scheme is:%d\n", user_scheme);
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);

	return user_scheme;
}

char *
fm_repl_pol_get_ldap_type(
	pin_errbuf_t	*ebufp)
{
	static char	*ldap_type = NULL;
	int32		err = 0;

	if (ldap_type != NULL)
		return ldap_type;

	pin_conf("fm_repl_pol", "ldap_env", PIN_FLDT_STR,
		 (caddr_t *)&ldap_type, &err);


	if (ldap_type == NULL) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			    PIN_ERRCLASS_SYSTEM_DETERMINATE,
			    err, 0, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"ldap_type parameter error in pin.conf",
				 ebufp);
	}

	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, "Value of ldap_type is:");
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, ldap_type);

	return ldap_type;
}
