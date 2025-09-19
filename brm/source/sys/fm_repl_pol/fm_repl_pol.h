/*
 * @(#) %full_filespec: fm_repl_pol.h~6:incl:1 %
 *
* Copyright (c) 1997, 2016, Oracle and/or its affiliates. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or
 *      its licensors and may be used, reproduced, stored or transmitted only
 *      in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _FM_REPL_POL_H_
#define _FM_REPL_POL_H_

#include "pcm.h"
#include "cm_fm.h"

#ifdef __cplusplus
extern "C" {
#endif

void
fm_utils_beid_name(
	pcm_context_t	*ctxp,
	int32           bal_id,
	char            *name_buf,
	int             name_len);

PIN_EXPORT void
op_repl_pol_push(
	cm_nap_connection_t     *connp,
	int32                   opcode,
	int32                   flags,
	pin_flist_t             *i_flistp,
	pin_flist_t             **r_flistpp,
	pin_errbuf_t            *ebufp);


void
fm_repl_pol_create_obj(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	poid_t			*cons_pdp,
	pin_errbuf_t		*ebufp);

void
fm_repl_pol_write_flds(
	pcm_context_t		*ctxp,
	int32			flags,
	poid_t			*a_pdp,
	poid_t			*cons_pdp,
	pin_errbuf_t		*ebufp);

int64
fm_repl_pol_get_ldap_db();

int64
fm_repl_pol_trnsform_ldap_db(int64 db_id);

int
fm_repl_pol_get_user_scheme();

char *
fm_repl_pol_get_ldap_type(
	pin_errbuf_t		*ebufp);

poid_t *
fm_repl_pol_get_suitespot_service(
	pin_errbuf_t	*ebufp);

#ifdef __cplusplus
}
#endif

#endif /*_XXX_FILE_H_*/
