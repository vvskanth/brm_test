/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2023 Oracle.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

/*
 * This file contains a simple minded opaque layer for exposing
 * certain credentials to the DM.
 */
#ifndef _DM_CREDENTIALS_H
#define _DM_CREDENTIALS_H

#include "pin_type.h"
#include "pcm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Credentials are opaque at this level */
typedef void dm_credentials_t;

#define EXTERN		extern

/* Semi-pulic interfaces into the dm credentials */
EXTERN void dm_credentials_from_flist(dm_credentials_t **credp, 
			pin_flist_t *init_flistp,
			pin_errbuf_t *ebufp);
EXTERN void dm_credentials_free(dm_credentials_t *credp);
EXTERN char *dm_credentials_get_lineage(dm_credentials_t *credp);
EXTERN poid_t *dm_credentials_get_brand(dm_credentials_t *credp);
EXTERN poid_t *dm_credentials_get_account(dm_credentials_t *credp);

#undef EXTERN

#ifdef __cplusplus
}
#endif

#endif /* _DM_CREDENTIALS_H */
