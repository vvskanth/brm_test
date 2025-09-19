/*
 * Copyright (c) 1999, 2024 Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef lint
static  char    Sccs_id[] = "@(#)$Id: fm_zonemap_pol_get_lineage.c /cgbubrm_7.5.0.portalbase/1 2023/07/18 03:57:44 visheora Exp $";
#endif

/*******************************************************************
 * Contains the PCM_OP_ZONEMAP_POL_GET_LINEAGE operation. 
 *******************************************************************/

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "pcm.h"
#include "ops/zonemap.h"
#include "pin_errs.h"
#include "pinlog.h"

#include "cm_fm.h"
#include "cm_cache.h"

#include "pin_zonemap.h"
#include "fm_zonemap_pol.h"


#define FILE_LOGNAME "fm_zonemap_pol_get_lineage.c(8)"

/***************************************************
 * Defaults
 ***************************************************/

/* PIN_DEFAULT_SEARCH_MODE defines whether to use 
 * a prefix or exact match search when the search 
 * mode isn't specified either on the input flist 
 * or in the matrix itself.
 */
#define PIN_DEFAULT_SEARCH_MODE	PIN_ZONEMAP_PREFIX

/***************************************************
 * Hash Table Defines/types
 ***************************************************/

/* TELEPHONY_ZONEMAPHASHTABLESIZE defines the size of  
 * the hash table used for caching the zone maps.
 */
#define TELEPHONY_ZONEMAPHASHTABLESIZE	11

/* ZonemapHashEntry_t is the struct used for the 
 * hash table buckets.
 */
typedef struct TAG_ZonemapHashEntry_t {
	const char		*pszZonemapName;
	poid_t			*pMatrixPoid;
	int32			nDefaultSearchMode;
	struct TAG_ZonemapHashEntry_t	*pNext;
	Blob_t			*pBuffer;
	time_t			lastUpdate;
} ZonemapHashEntry_t;

/* Zonemap hash table & semaphore */
static ZonemapHashEntry_t *ZonemapHashTable_ap[TELEPHONY_ZONEMAPHASHTABLESIZE];
static time_t interval;

/* Hash table init done? */
static int32 nInitDone = PIN_BOOLEAN_FALSE;

/***************************************************
 * Other Defines/types
 ***************************************************/
/* Global list of zonemaps for startup logic */
typedef enum {
        STARTUP_CACHE_NONE = 0,
        STARTUP_CACHE_HEAP,
        STARTUP_CACHE_SHARED
} startup_cache_type_t;
static startup_cache_type_t g_startup_cache_type = STARTUP_CACHE_NONE;
static pin_flist_t *g_zone_flp = NULL;
static cm_cache_t *g_zone_cachep = NULL;

/*******************************************************************
 * Routines contained within.
 *******************************************************************/

PIN_EXPORT void 
fm_zonemap_pol_get_lineage_init(int32 *errp);

EXPORT_OP void
op_zonemap_pol_get_lineage(
	cm_nap_connection_t	*connp,
	u_int			opcode,
	u_int			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp);

static ZonemapHashEntry_t *
fm_zonemap_pol_get_lineage_get_zonemap(
	pcm_context_t		*pCtx, 
	poid_t			*pRoutingPoid, 
	poid_t			*pBrandPoid, 
	ZonemapHashEntry_t	**apHashTable, 
	const char		*pszSearchTarget, 
	pin_errbuf_t		*ebufp);

static char *
fm_zonemap_pol_get_lineage_search_zonemap(
	Blob_t			*pBuffer, 
	char			*pszString,
	int32			nSearchMode, 
	pin_errbuf_t		*ebufp);

static int32 
fm_zonemap_pol_get_lineage_hash_zone_name(
	const char		*pszZoneName);

static ZonemapHashEntry_t *
fm_zonemap_pol_get_lineage_find_bucket(
	ZonemapHashEntry_t	*pBucket, 
	const char		*pszZoneName, 
	pin_errbuf_t		*ebufp);

static int32
fm_zonemap_pol_get_lineage_load_zonemap(
	pcm_context_t		*pCtx, 
	const char		*pszTarget, 
	poid_t			*pRoutingPoid, 
	poid_t			**ppZonemapPoid,
	poid_t			*pBrandPoid,
	Blob_t			**ppBuffer, 
	int32			*pnSearchMode, 
	pin_errbuf_t		*ebufp);

static int32 
fm_zonemap_pol_get_lineage_rev_changed(
	pcm_context_t		*pCtx, 
	const poid_t		*pMatrixPoid, 
	pin_errbuf_t		*ebufp);

static int32
fm_zonemap_pol_load_maps (
	pin_flist_t 		**z_flpp);

static pin_flist_t *
fm_zonemap_pol_search_startup_cache (
	pcm_context_t		*ctxp,
	char			*target,
	poid_t 			*bpdp,
	pin_zonemap_data_type_t dt);

static int
fm_zonemap_pol_pre_process_shm_blob(
	int32                   keytype,
	void                    *keyvalp,
	pin_flist_t             *flp,
	void                    *inp,
	void                    **outpp);

static int
fm_zonemap_pol_get_lineage_from_shm(
	int32                   keytype,
	void                    *keyvalp,
	pin_flist_t             *flp,
	void                    *inp,
	void                    **outpp);

static void
fm_zonemap_pol_load_map_into_shm(
        pcm_context_t           *ctxp,
        char                    *name,
        pin_errbuf_t            *ebufp);

/*******************************************************************
 * Function bodies.
 *******************************************************************/
void 
fm_zonemap_pol_get_lineage_init(int32 *errp)
{
	int32			*i_ptr = NULL;
	int32			error = 0;
	int32			i = 0; 
	char                    msg[1024];

	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
		"fm_zonemap_pol_get_lineage_init Enter");
	/*
	 * Init ProductHashArray
	 */
	if (nInitDone == PIN_BOOLEAN_FALSE) {
		nInitDone = PIN_BOOLEAN_TRUE;
		/*
		 * read the value of this variable from the pin.conf file. If it
		 * doesn't exist then the default value stays.
		 */
		pin_conf("fm_zonemap_pol", "update_interval", PIN_FLDT_INT, (caddr_t *)&i_ptr, &error);
		if (error == PIN_ERR_NONE) {
			interval = *i_ptr;
		} else {
			interval = 3600;
		}
		if (i_ptr) {
			free(i_ptr);
		}
		pin_snprintf(msg, sizeof(msg), "fm_zonemap_pol_get_lineage_init : update_interval[%d] seconds", interval);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);
	
	}
        /*
	 * Initialize ZonemapHashTable_ap
	 */
	for (i = 0 ; i < TELEPHONY_ZONEMAPHASHTABLESIZE ; i++) {
		ZonemapHashTable_ap[i] = NULL;
	}
	
       *errp = fm_zonemap_pol_load_maps(&g_zone_flp);

	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
		"fm_zonemap_pol_get_lineage_init Exit");
}


/*******************************************************************
 * Main routine for the PCM_OP_ZONEMAP_POL_GET_LINEAGE operation.
 *******************************************************************/
void
op_zonemap_pol_get_lineage(
	cm_nap_connection_t	*connp,
	u_int			opcode,
	u_int			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	ZonemapHashEntry_t	*ZonemapNode_p = NULL;
	poid_t			*RoutingPoid_p = NULL;
	char			*SearchTarget_psz = NULL;
	char			*ZoneMapName_psz = NULL;
	int32			*SearchMode_np = NULL;
	int32			DefaultSearchMode_n = PIN_DEFAULT_SEARCH_MODE;
	char			*Lineage_psz = NULL;
	poid_t			*pBrandPoid = NULL;
	int32			*i_ptr = NULL;
	int32			error = 0;
	int32                   err;
	int32                   lookup_attempts = 0;
	poid_t                  *pdp =  NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Sanity check.
	 */
	if (opcode != PCM_OP_ZONEMAP_POL_GET_LINEAGE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_zonemap_pol_get_lineage bad opcode error", 
			ebufp);
		return;
	}

	/*
	 * Debug: What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_zonemap_pol_get_lineage input flist", i_flistp);

	/*
	 * Create & init return flist
	 */
	*r_flistpp = PIN_FLIST_CREATE(ebufp);
	RoutingPoid_p = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_POID, RoutingPoid_p, ebufp);
	pBrandPoid = (poid_t *) PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ACCOUNT_OBJ, 1, ebufp);
	/*
	 * Grab required params from input flist
	 */
	ZoneMapName_psz = (char*) PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ZONEMAP_NAME, 0, ebufp);
	SearchTarget_psz = (char*) PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ZONEMAP_TARGET, 0, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		goto Done;
	}
	
	if ((SearchTarget_psz == NULL) || (strlen(SearchTarget_psz) == 0)) {
		/*
		 * Target string is empty,
		 * create an empty return flist and exit.
		 */
		Lineage_psz = "";
		/*
		 * Copy data to return flist
		 */
		PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_ZONEMAP_LINEAGE, Lineage_psz, ebufp);
	} else if (g_startup_cache_type == STARTUP_CACHE_SHARED) {
		/*
		 * Implementation of shared-memory caching. This uses callbacks into the cm_cache to
		 * avoid both expensive copying of large amounts of data, but also the fact that copying the
		 * Zonemap matrix renders all the addresses in the map invalid - so would not work correctly
		 * anyway.
		 */
retry_lookup:
		err = 0;
		error = cm_cache_exec_entry(g_zone_cachep, ZoneMapName_psz,
				fm_zonemap_pol_get_lineage_from_shm, (void *)i_flistp, (void **)r_flistpp, &err);
		switch (error) {
		case PIN_ERR_NOT_FOUND:         /* couldn't find zonemap in cache - try and load it */
			fm_zonemap_pol_load_map_into_shm(ctxp, ZoneMapName_psz, ebufp);
			if (lookup_attempts == 0 && !PIN_ERR_IS_ERR(ebufp)) {
				++lookup_attempts;
				goto retry_lookup;
			}
			break;
		case PIN_ERR_STALE_CONF:        /* check if a change has occurred - if so, retry */
			pdp = PIN_FLIST_FLD_TAKE(*r_flistpp, PIN_FLD_ZONEMAP_OBJ, 0, ebufp);
			if (fm_zonemap_pol_get_lineage_rev_changed(ctxp, pdp, ebufp)) {
				fm_zonemap_pol_load_map_into_shm(ctxp, ZoneMapName_psz, ebufp);
				goto retry_lookup;
			}
			PIN_POID_DESTROY(pdp, NULL);
			break;
		case PIN_ERR_NO_MATCH:          /* lineage didn't match - nothing to do */
			break;
		default:
			pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, error, PIN_FLD_ZONEMAP_NAME, 0, 0);
		}
		goto Done;
		/*********/
	} else {
		/*
		 * Try and load matrix, and if we get it, search for the lineage ...
		 */
		ZonemapNode_p = fm_zonemap_pol_get_lineage_get_zonemap(ctxp, RoutingPoid_p, 
				pBrandPoid, ZonemapHashTable_ap, ZoneMapName_psz, ebufp);
		if (ZonemapNode_p != NULL) {
			/*
			 * Find search mode (from input flist or from matrix). If it is not found,
			 * use the default ...
			 */	
			SearchMode_np = (int32*) PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ZONEMAP_SEARCH_TYPE, 1, ebufp);
			if (SearchMode_np == NULL) {
				SearchMode_np = &(ZonemapNode_p->nDefaultSearchMode);
				if (SearchMode_np == NULL) {
					SearchMode_np = &DefaultSearchMode_n;
				}
			}
	
			/*
			 * Do the search (allocates mem)
			 */
			Lineage_psz = fm_zonemap_pol_get_lineage_search_zonemap(ZonemapNode_p->pBuffer,
						SearchTarget_psz, *SearchMode_np, ebufp);
		
			if (Lineage_psz == NULL) {
				Lineage_psz = "";
				PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_ZONEMAP_LINEAGE, Lineage_psz, ebufp);
			} else {
				PIN_FLIST_FLD_SET(*r_flistpp, PIN_FLD_ZONEMAP_LINEAGE, Lineage_psz, ebufp);
			 	free(Lineage_psz);  
			}
		} else {
			pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION, PIN_ERR_NO_MATCH, PIN_FLD_ZONEMAP_NAME, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"op_zonemap_pol_get_lineage error: Can't find requested zonemap in DB", ebufp);
		}
	}
Done:
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_FLIST_DESTROY_EX(r_flistpp, NULL);
		*r_flistpp = (pin_flist_t *)NULL;
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, "op_zonemap_pol_get_lineage error", ebufp);
	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "op_zonemap_pol_get_lineage return flist", *r_flistpp);
	}
	return;
}


/*******************************************************************
 * fm_zonemap_pol_get_lineage_get_zonemap():
 *******************************************************************/
static ZonemapHashEntry_t *
fm_zonemap_pol_get_lineage_get_zonemap(
	pcm_context_t		*pCtx, 
	poid_t			*pRoutingPoid, 
	poid_t			*pBrandPoid, 
	ZonemapHashEntry_t	**apHashTable, 
	const char		*pszTarget, 
	pin_errbuf_t		*ebufp) 
{

	int32			nTargetHash = 0 ;
	int32			nLoadStatus = 0;
	ZonemapHashEntry_t	*pHashEntry = NULL;
	ZonemapHashEntry_t	*pBucket = NULL;
	ZonemapHashEntry_t	*pCurNode = NULL;
	Blob_t			**ppBuffer = NULL;
	poid_t			**ppMatrixPoid = NULL;
	int32			*pnSearchMode = NULL;
	int32			nLoadFromDB = 0;
	time_t			current_time = 0;

	pHashEntry = pBucket = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return NULL;
	}

	/* Debug */
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_get_zonemap starting");

	/*
	 * Hash zone name
	 */
	nTargetHash = fm_zonemap_pol_get_lineage_hash_zone_name(pszTarget);

	/* Get bucket */
	pBucket = apHashTable[nTargetHash];

	/* Assume we don't have to load from DB */
	nLoadFromDB = PIN_BOOLEAN_FALSE;

	/*
	 * Have we already loaded this matrix?
	 */
	pHashEntry = fm_zonemap_pol_get_lineage_find_bucket(pBucket, pszTarget, ebufp);
	if (pHashEntry != NULL) {
		/*
		 * Do a revision check only if a certain time has elapsed and if the revsion has
		 * changed, trigger a reload.
		 */	
		current_time = pin_virtual_time((time_t*)NULL);
		if(current_time > (pHashEntry->lastUpdate + interval)) {
			 pHashEntry->lastUpdate = current_time;
			if (fm_zonemap_pol_get_lineage_rev_changed(pCtx, pHashEntry->pMatrixPoid, ebufp)) {
				/* Remove node from bucket */
				for (pCurNode = pBucket; pCurNode != NULL; pCurNode = pCurNode->pNext) {
					if(pCurNode == pHashEntry){	
					 	/* This case will occur if the first element in the 	
						 * linked list will go for update
						 */
						apHashTable[nTargetHash] = pHashEntry->pNext;
						break;
					} else if (pCurNode->pNext == pHashEntry) {
						/* This case will occur if any element other than first
						 * one in the linked list will go for update
						 */
						pCurNode->pNext = pHashEntry->pNext;
						break;
					}
				}
				/* Free buffer */
				free(pHashEntry->pBuffer);
				pHashEntry->pBuffer = NULL;

				/* Free matrix poid */
				PIN_POID_DESTROY_EX(&(pHashEntry->pMatrixPoid),
					NULL);

				/* Indicate that we need to load from DB */
				nLoadFromDB = PIN_BOOLEAN_TRUE;
			}
		}
	} else {
		/*
		 * No: Add new node to bucket
		 */
		pHashEntry = (ZonemapHashEntry_t *) malloc(sizeof(ZonemapHashEntry_t));
		if (pHashEntry != NULL) {
			pHashEntry->pszZonemapName = malloc(strlen(pszTarget) + 1);
			if (pHashEntry->pszZonemapName == NULL) {
				pin_set_err(ebufp, PIN_ERRLOC_FM, 
					PIN_ERRCLASS_SYSTEM_DETERMINATE, PIN_ERR_NO_MEM, 0, 0, 0);
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
					"fm_zonemap_pol_get_lineage_get_zonemap: failed to allocate memory for zonemap name", ebufp);
				free(pHashEntry);
				pHashEntry = NULL;
				goto Done;
			}
			pin_strlcpy((char*) pHashEntry->pszZonemapName, pszTarget,strlen(pszTarget) + 1);
			pHashEntry->pNext = NULL;
			pHashEntry->pBuffer = NULL;
		}
		if (pHashEntry == NULL) {
			pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_SYSTEM_DETERMINATE, PIN_ERR_NO_MEM, 0, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				"fm_zonemap_pol_get_lineage_get_zonemap: failed to allocate memory for hash entry", ebufp);
			goto Done;
		}
		/* Indicate that we need to load from DB */
		nLoadFromDB = PIN_BOOLEAN_TRUE;
	}

	/* Do we need to load from the database? */
	if (nLoadFromDB == PIN_BOOLEAN_TRUE) {
		/* Copy trie address from global to local mem */
		ppBuffer = &(pHashEntry->pBuffer);
		ppMatrixPoid = &(pHashEntry->pMatrixPoid);
		pnSearchMode = &(pHashEntry->nDefaultSearchMode);
				
		/* Attempt to read matrix from DB */
		nLoadStatus = fm_zonemap_pol_get_lineage_load_zonemap(
			pCtx, pszTarget, pRoutingPoid, ppMatrixPoid,pBrandPoid, 
			ppBuffer, pnSearchMode, ebufp);

		/* Did we load a matrix? */
		if (nLoadStatus == PIN_BOOLEAN_FALSE) {
			free((char*)pHashEntry->pszZonemapName);
			pHashEntry->pszZonemapName = NULL;
			free(pHashEntry);
			pHashEntry = NULL;
		} else {
			pHashEntry->pNext = apHashTable[nTargetHash];
			apHashTable[nTargetHash] = pHashEntry;
			pHashEntry->lastUpdate = pin_virtual_time((time_t *)NULL);
		}
	}
Done:

	/* Debug */
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_get_zonemap returning");

	return pHashEntry;
}

/*******************************************************************
 * fm_zonemap_pol_get_lineage_rev_changed():
 *
 *	Determine whether the matrix has changed since we loaded it.
 *
 *******************************************************************/
static int32
fm_zonemap_pol_get_lineage_rev_changed(
	pcm_context_t		*pCtx, 
	const poid_t		*pMatrixPoid, 
	pin_errbuf_t		*ebufp) 
{
	pin_flist_t		*pReadPoidInFlist = NULL;
	pin_flist_t		*pReadPoidOutFlist = NULL;
	poid_t			*pNewPoid = NULL;
	int32			nRetVal = 0;
	int64			in_db_no = 0;

	/* Sanity check */
	if (PIN_ERR_IS_ERR(ebufp)) {
		return PIN_BOOLEAN_FALSE;
	}

	/* Debug */
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_rev_changed starting");

	/*
	 * Create flist for reading the zonemap poid
	 */
	pReadPoidInFlist = PIN_FLIST_CREATE(ebufp);

	/* In multi-db case the input context DB number may be different than the one earlier cached in pMatrixPoid.
         * So do READ_FLDS on the DB from input context for cached zonemap poid to avoid error from CM that
         * a transaction is already open.
         */

        pNewPoid = PIN_POID_COPY((poid_t *)pMatrixPoid, ebufp);
        in_db_no = cm_fm_get_current_db_no(pCtx);

        PIN_POID_SET_DB(pNewPoid, in_db_no);

        PIN_FLIST_FLD_PUT(pReadPoidInFlist, PIN_FLD_POID, pNewPoid, ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_zonemap_pol_get_lineage_rev_changed zonemap read_flds input flist", pReadPoidInFlist);

	/*
	 * Read current poid from DB
	 */
	PCM_OP(pCtx, PCM_OP_READ_FLDS, 0, pReadPoidInFlist, &pReadPoidOutFlist, ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_zonemap_pol_get_lineage_rev_changed zonemap read_flds output flist", pReadPoidOutFlist);

	/*
	 * Compare poid revs
	 */

	/* Get new poid from flist */
	pNewPoid = (poid_t*) PIN_FLIST_FLD_GET(pReadPoidOutFlist, PIN_FLD_POID, 0, ebufp);

	/* To compare poids for revision check and in multi-db case just set back the
         * pMatrixPoid cached zonemap poid DB number to the  newly read poid
         * to just compare poid_id, type and revision and not db number
         */
        in_db_no = PIN_POID_GET_DB((poid_t *)pMatrixPoid);
        PIN_POID_SET_DB(pNewPoid, in_db_no);

	/* Is db rev newer than stored rev? */
	if (PIN_POID_COMPARE(pNewPoid, (poid_t*)pMatrixPoid, PIN_BOOLEAN_TRUE, ebufp)) {
		nRetVal = PIN_BOOLEAN_TRUE;
	} else {
		nRetVal = PIN_BOOLEAN_FALSE;
	}

	/*
	 * Clean up
	 */
	PIN_FLIST_DESTROY_EX(&pReadPoidInFlist, NULL);
	PIN_FLIST_DESTROY_EX(&pReadPoidOutFlist, NULL);		

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_zonemap_pol_get_lineage_rev_changed() error", ebufp);
		nRetVal = PIN_BOOLEAN_FALSE;
	} else {
		if (nRetVal == PIN_BOOLEAN_TRUE) {
			PIN_ERR_LOG_POID(PIN_ERR_LEVEL_DEBUG,
				"fm_zonemap_pol_get_lineage_rev_changed returning - zmap changed", pNewPoid);
		} else {
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
				"fm_zonemap_pol_get_lineage_rev_changed returning - no change");
		}
	}
	return nRetVal;
}

/*******************************************************************
 * fm_zonemap_pol_get_lineage_load_zonemap():
 *
 *	Retrieve and process the named zonemap.
 *
 *******************************************************************/
static int32 
fm_zonemap_pol_get_lineage_load_zonemap(
	pcm_context_t		*pCtx, 
	const char		*pszTarget, 
	poid_t			*pRoutingPoid, 
	poid_t			**ppZonemapPoid,
	poid_t			*pBrandPoid, 
	Blob_t			**ppBuffer, 
	int32			*pnDefaultSearchMode, 
	pin_errbuf_t		*ebufp) 
{

	pin_flist_t		*pGetZonemapInFlist = NULL;
	pin_flist_t		*pGetZonemapOutFlist = NULL;
	pin_flist_t		*pZonemaps = NULL;
	poid_t			*pSearchPoid = NULL;
	pin_buf_t		*pBuf = NULL;
	Blob_t			*pTempBufAddr = NULL;
	int32			nRetVal = PIN_BOOLEAN_TRUE;
	pin_zonemap_data_type_t	dt = PIN_ZONEMAP_DATA_BINARY;
	int32			*pnSearchMode = NULL;
	pin_flist_t 		*z_flp = NULL;
	poid_t 			*zpdp = NULL;
		
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_load_zonemap starting");

	/*
	 * First we must search our startup cache in preference to reading from the DB. 
	 * We will, however, check the revision number so that if a change has occurred, 
	 * we will get the latest version. If the zonemap is not found, or the version has 
	 * changed, read from DB.
	 */
	if (g_startup_cache_type == STARTUP_CACHE_HEAP) {
		z_flp = fm_zonemap_pol_search_startup_cache (pCtx, (char *)pszTarget, pBrandPoid, dt);
		if (z_flp) {
			PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"fm_zonemap_pol_get_lineage_load_zonemap: found in startup cache", z_flp);
			/*
			 * Check revision - if it's changed since startup (unlikely) then
			 * toss this entry and get from DB.
			 */
			zpdp = PIN_FLIST_FLD_GET(z_flp, PIN_FLD_POID, 0, ebufp);
			if (fm_zonemap_pol_get_lineage_rev_changed (pCtx, zpdp, ebufp)) {
				PIN_FLIST_DESTROY_EX(&z_flp, NULL);
			} else {
				PIN_ERR_LOG_POID(PIN_ERR_LEVEL_DEBUG,
					"fm_zonemap_pol_get_lineage_load_zonemap: revision OK", zpdp);
			}
		}
	}

	/*
	 * Create flist for retrieving matrix, if we didn't find it in our startup cache ...
	 */
	if (z_flp) {
		pZonemaps = z_flp;
	} else {
		pGetZonemapInFlist = PIN_FLIST_CREATE(ebufp);
		pSearchPoid = PIN_POID_CREATE(PIN_POID_GET_DB((poid_t *) pRoutingPoid), "/zonemap", -1, ebufp);
		PIN_FLIST_FLD_PUT(pGetZonemapInFlist, PIN_FLD_POID, pSearchPoid, ebufp);
		PIN_FLIST_FLD_SET(pGetZonemapInFlist, PIN_FLD_NAME, (void *) pszTarget, ebufp);
		PIN_FLIST_FLD_SET(pGetZonemapInFlist, PIN_FLD_ZONEMAP_DATA_TYPE, (void *) &dt, ebufp);
		PIN_FLIST_FLD_SET(pGetZonemapInFlist, PIN_FLD_ACCOUNT_OBJ, (void *) pBrandPoid, ebufp);
		PCM_OP(pCtx, PCM_OP_ZONEMAP_GET_ZONEMAP, 0, pGetZonemapInFlist, &pGetZonemapOutFlist, ebufp);
		pZonemaps = PIN_FLIST_ELEM_GET(pGetZonemapOutFlist, PIN_FLD_ZONEMAPS, PIN_ELEMID_ANY, 1, ebufp);
	}

	/*
	 * Process matrix - we just TAKE the blob from the buffer field and keep
	 * the poid and default search mode, if any. Note that the buffer has already
	 * had its addresses remapped, so do NOT "move" this buffer anywhere.
	 */
	if (pZonemaps != NULL) {
		*ppZonemapPoid = PIN_FLIST_FLD_GET(pZonemaps,PIN_FLD_POID, 0, ebufp);
		pnSearchMode = (int32*) PIN_FLIST_FLD_GET(pZonemaps, PIN_FLD_ZONEMAP_SEARCH_TYPE, 1, ebufp);
		if (pnSearchMode != NULL) {
			*pnDefaultSearchMode = *pnSearchMode;
		}
		pBuf = (pin_buf_t *)PIN_FLIST_FLD_TAKE(pZonemaps, PIN_FLD_ZONEMAP_DATA_DERIVED, 1, ebufp);
		if (pBuf != NULL && (pBuf->data != NULL)) { 
			*ppBuffer = (Blob_t *)pBuf->data; 
			pBuf->data = NULL;
			pin_free(pBuf);
		} else {
			*ppBuffer = NULL;
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_WARNING, "fm_zonemap_pol_get_lineage_load_zonemap: zonemap is empty!", ebufp);
		}
	} else {
		nRetVal = PIN_BOOLEAN_FALSE;
	}
	
	/*
	 * Clean up
	 */
	PIN_FLIST_DESTROY_EX(&pGetZonemapInFlist, NULL);
	PIN_FLIST_DESTROY_EX(&pGetZonemapOutFlist, NULL);
	PIN_FLIST_DESTROY_EX(&z_flp, NULL); 
	if (PIN_ERR_IS_ERR(ebufp)) {
		nRetVal = PIN_BOOLEAN_FALSE;

		if (ebufp->pin_err == PIN_ERR_NO_MATCH) {
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
			   "fm_zonemap_pol_get_lineage_load_zonemap: no matrix");
			PIN_ERR_CLEAR_ERR(ebufp);

		} else {
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			    "fm_zonemap_pol_get_lineage_load_zonemap: error", ebufp);
		}

	}
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_load_zonemap returning");

	return nRetVal;
}

/*******************************************************************
 * fm_zonemap_pol_get_lineage_find_bucket():
 *
 *	Given a bucket in the hash table, this function returns 
 *	the matching entry in that bucket.
 *
 *******************************************************************/
static ZonemapHashEntry_t *
fm_zonemap_pol_get_lineage_find_bucket(
	ZonemapHashEntry_t	*pBucket, 
	const char		*pszTarget, 
	pin_errbuf_t		*ebufp) 
{
	ZonemapHashEntry_t	*pCurNode = NULL;

	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_find_poid_in_bucket starting");

	for (pCurNode = pBucket; pCurNode != NULL; 
		pCurNode = pCurNode->pNext) {
		if (!strcmp(pszTarget, pCurNode->pszZonemapName)) {
			break;
		}
	}
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_find_poid_in_bucket returning");

	return pCurNode;
}

/*******************************************************************
 * fm_zonemap_pol_get_lineage_hash_zone_name():
 *
 *	Returns the hash value for the given zone name (a string)
 *
 *******************************************************************/
static int32 
fm_zonemap_pol_get_lineage_hash_zone_name(
	const char		*pszTarget) 
{

	unsigned long		nHashVal = 0;
	unsigned long		nCount = 0;
	unsigned long		nMask = 0xF0000000;

	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_hash_zone_name starting");

	/*
	 * Use a basic ELF hash
	 */
	while (*pszTarget) {
		nHashVal = (nHashVal << 4) + *pszTarget++;
		if ((nCount = nHashVal & nMask)) {
			nHashVal ^= nCount >> 24;
		}
		nHashVal &= ~nCount;
	}

	nHashVal %= TELEPHONY_ZONEMAPHASHTABLESIZE;

	/* Debug */
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_hash_zone_name returning");

	return (int32) nHashVal;
}


/*******************************************************************
 * fm_zonemap_pol_get_lineage_search_zonemap():
 *
 *	Finds the lineage of the given string
 *
 *******************************************************************/
static char *
fm_zonemap_pol_get_lineage_search_zonemap(
	Blob_t			*pBuffer, 
	char			*pszString,
	int32			nSearchMode, 
	pin_errbuf_t		*ebufp)
{

	/* Even though it's less readable than a recursive approach, 
	 * an iterative approach is used for performance reasons.
	 * This function is entered twice for every call, and thus must
	 * be sensitive to performance.
	 */

	TrieNode_t		*pCurNode = NULL;
	TrieNode_t		*pLastTerminal = NULL;
	LeafData_t		*pCurData = NULL;
	child_index_t		*pChildIndex = NULL;
	char			*pCur = NULL;
	char			*pszLabel = NULL;
	char			*pszResult = NULL;
	int32			nDone = 0;
	int32			nIndex = 0;
	size_t			nDepth = 0;
	char			acLogBuf[512]="";
        size_t                  pszLength = 0;

	/* Debug: Enter function */
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_search_zonemap starting");

	/* Verify inputs */
	if (pBuffer == NULL) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
			"fm_zonemap_pol_get_lineage_search_zonemap called with no data: returning");
		return NULL;
	}
	/* Verify Blob version number */	
	if (pBuffer->version != ZONEMAP_CURRENT_VERSION) {
		pin_snprintf(acLogBuf,sizeof(acLogBuf), "fm_zonemap_pol_get_lineage_search_zonemap: "
			"Wrong version of Zonemap.  Found %i, "
		       	"expecting %i. Ignoring zonemap. Use Configuration Center IPT to fix this problem.", 
			pBuffer->version, ZONEMAP_CURRENT_VERSION);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, acLogBuf);
		return NULL;
	}

	/*
	 * Get a pointer to the target string as a cursor.
	 */
	pCur = pszString;
 
	/* Grab trie root & init flag */
	pCurNode = (TrieNode_t *) ((telephony_offset_t)pBuffer + (telephony_offset_t)pBuffer->trie_start);
	pLastTerminal = NULL;
	nDone = PIN_BOOLEAN_FALSE;
	while (!nDone) {
		/*
		 * Pickup any applicable parent impact categories
		 */

		/* Is the current node a terminator? */
		if (pCurNode && pCurNode->isTerminal == PIN_BOOLEAN_TRUE) {
			/* Yes: Place bookmark */
			pLastTerminal = pCurNode;		
			/*
			 * Get the length of string we have searched.
			 */
			nDepth = pCur - pszString;
		}

		/*
		 * Check for more work & move forward
		 */

#ifdef IPT_DEBUG
		if(pBuffer->version == ZONEMAP_VERSION_4) {
			pin_snprintf(acLogBuf,sizeof(acLogBuf), "String: '%s', Index: %d, Max %d", pCur, 
				EXTENDED_CHARSET_INDEX(*pCur), 
				EXTENDED_CHARSETSIZE);
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, acLogBuf);
		}
#endif
		if (pCurNode == NULL) {
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
				"fm_zonemap_pol_get_lineage_search_zonemap: "
				"illegal memory access! Exiting");
			return NULL;
		}

		/* Does the string have more input? */
		if (*pCur != '\0') {
			/* Does this node have children? If so find a match */
			pChildIndex = (child_index_t *)(pCurNode + 1);
			for (nIndex = 0; nIndex < pCurNode->nChildren; nIndex++, pChildIndex++) {
				if (*pCur == (char)pChildIndex->character) {
					break;
				}
			}
			if (nIndex < pCurNode->nChildren) {
				pCurNode = (TrieNode_t *)((telephony_offset_t)pBuffer + pChildIndex->offset);
				pCur++;

			} else {
				/* No: we're done */
				nDone = PIN_BOOLEAN_TRUE;
			}
		} else {
			/* No: we're done */
			nDone = PIN_BOOLEAN_TRUE;
		}
	}

	/* Are we in exact match mode? */
	if (nSearchMode == PIN_ZONEMAP_EXACT) {
		/* Yes: Are we at the end of the string & at a terminal node? */
		/* Note: DeMorgans laws applied to simplify boolean logic */
		if ((*pCur != '\0') || (pLastTerminal != pCurNode)) {
			/* No: set return value */
			pLastTerminal = NULL;
		}
	}

	/* Did we manage to find data? */
	if (pLastTerminal != NULL) {
		/* Yes: Grab data & set return value */
		pCurData = (LeafData_t*) ((telephony_offset_t)pBuffer + (telephony_offset_t)pLastTerminal->leafdata_p);
				
		/* Yes: For each row in the list... */
		fm_zonemap_pol_get_lineage_from_ancestors((telephony_offset_t) pBuffer,
			pCurData->ancestors_p, &pszLabel, ebufp);

		if (!PIN_ERR_IS_ERR(ebufp)) {
			/*
			 * Craft a return string that has both the lineage and 
			 * the data we just did the search based upon.
			 */
                        if (pszLabel != NULL) {
                                pszLength = strlen(pszLabel) + nDepth + 1;
			    	
			} else {
				/*
				 * Only get the part of string based on which
				 * we did a search.
				 */
				*pCur = '\0';
                                pszLength = nDepth + 1;
			}
                        pszResult = (char *)malloc(pszLength); 
				
			/*
			 * Check the memory allocation
			 */
			if (pszResult != NULL) {
				if (pszLabel != NULL) {
					pin_strlcpy(pszResult, pszLabel, pszLength);
					pin_strncat(pszResult, pszLength, pszString, nDepth);
				} else {
					pin_strncpy(pszResult, pszLength, pszString, nDepth);
				}
			} else {
				pin_set_err(ebufp, PIN_ERRLOC_FM,
					PIN_ERRCLASS_APPLICATION,
					PIN_ERR_NO_MEM, 0, 0, 0);
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"Memory allocation error in get "
					"lineage.", ebufp);
			}
		} else {
			/*
			 * Something went wrong.
			 */
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, "Search lineage error.", ebufp);
			pszResult = (char *)NULL;
		}
	} else {
		pszResult = NULL;
	}

	/*
	 * Clean up memory because it is allocated elsewhere.
	 */
	if (pszLabel) {
		free(pszLabel);
	}

	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
		"fm_zonemap_pol_get_lineage_search_zonemap returning");

	return pszResult;
}
/*----------------------------------------------------------------------------+
 | Function     : fm_zonemap_pol_pre_process_shm_blob
 |
 | Description  : Function to convert the data that's been inserted into the shared
 |                memory cache into host-order and resolve  all addresses in the zonemap.
 |
 | Input        : keytype   The cm_cache key type (should be string type)
 |                keyvalp   The cm_cache key value (zonemap name)
 |                flp       The shared-memory zonemap flist
 |                inp       Input data (not used)
 |                outpp     Output data (not used)
 |
 | Output       : None
 |
 | Return       : 0(PIN_ERR_NONE) if all OK, non-zero otherwise
 +----------------------------------------------------------------------------*/
static int
fm_zonemap_pol_pre_process_shm_blob (
	int32           keytype,
	void            *keyvalp,
	pin_flist_t     *flp,
	void            *inp,
	void            **outpp)
{
	pin_errbuf_t    ebuf;
	pin_buf_t       *bufp = NULL;

	PIN_ERRBUF_CLEAR(&ebuf);
	bufp = (pin_buf_t *)PIN_FLIST_FLD_GET(flp, PIN_FLD_ZONEMAP_DATA_DERIVED, 0, &ebuf);
	if (bufp) {
		fm_zonemap_pol_pre_process_blob((Blob_t *)bufp->data);
	}
	return ebuf.pin_err;
}

/*----------------------------------------------------------------------------+
 | Function     : fm_zonemap_pol_get_lineage_from_shm
 |
 | Description  : Function evaluate the lineage for a zonemap that is stored in the
 |                shared memory cache.
 |
 | Input        : keytype   The cm_cache key type (should be string type)
 |                keyvalp   The cm_cache key value (zonemap name)
 |                flp       The shared-memory zonemap flist
 |                inp       Input data (the PCM_OP_ZONEMAP_POL_GET_LINEAGE input)
 |                outpp     Output data the (allocated) return flist
 |
 | Output       : None
 |
 | Return       : 0(PIN_ERR_NONE) if all OK, non-zero otherwise
 |                3 (PIN_ERR_NOT_FOUND) if the Zonemap is not found in the cache
 |                29 (PIN_ERR_STALE_CONF) if the zonemap is 'due' for a refresh ...
 |                >0 Other technical errors
 +----------------------------------------------------------------------------*/
static int
fm_zonemap_pol_get_lineage_from_shm (
	int32           keytype,
	void            *keyvalp,
	pin_flist_t     *flp,
	void            *inp,
	void            **outpp)
{
	pin_errbuf_t    ebuf;
	pin_buf_t       *bufp = NULL;
	pin_flist_t     *in_flp = (pin_flist_t *)inp;
	pin_flist_t     **out_flpp = (pin_flist_t **)outpp;
	int32           *search_modep = NULL;
	int32           search_mode = 0;
	char            *lineagep = NULL;
	char            *targetp = NULL;
	time_t          *mod_tp = NULL;
	time_t          next_refresh_t = 0;


	PIN_ERRBUF_CLEAR(&ebuf);
	if (flp == NULL) {
		ebuf.pin_err = PIN_ERR_NOT_FOUND;
	} else {
		bufp = (pin_buf_t *)PIN_FLIST_FLD_GET(flp, PIN_FLD_ZONEMAP_DATA_DERIVED, 0, &ebuf);
	}
	if (bufp) {
                /*
                 * We have a buffer, do zonemap lookup ...
                 */
		targetp = (char *)PIN_FLIST_FLD_GET(in_flp, PIN_FLD_ZONEMAP_TARGET, 0, &ebuf);
		search_modep = (int32 *)PIN_FLIST_FLD_GET(in_flp, PIN_FLD_ZONEMAP_SEARCH_TYPE, 1, &ebuf);
		if (search_modep == NULL) {
			search_mode = PIN_DEFAULT_SEARCH_MODE;
		} else {
			search_mode = *search_modep;
		}
		lineagep = fm_zonemap_pol_get_lineage_search_zonemap((Blob_t *)bufp->data, targetp, search_mode, &ebuf);
		PIN_FLIST_FLD_PUT(*out_flpp, PIN_FLD_ZONEMAP_LINEAGE, lineagep, &ebuf);
		/*
		 * Check if we have a stale entry that will need to be checked for refresh. We just return
		 * that refresh needs to happen and leave it to the caller - we'll give them the POID as well
		 * for convenience.
		 */
		if (ebuf.pin_err == PIN_ERR_NONE) {
			next_refresh_t = pin_virtual_time(NULL);
			mod_tp = (time_t *)PIN_FLIST_FLD_GET(flp, PIN_FLD_WHEN_T, 1, &ebuf);
			if (mod_tp && *mod_tp < next_refresh_t) {
				next_refresh_t += interval;
				*mod_tp = next_refresh_t;
				PIN_FLIST_FLD_COPY(flp, PIN_FLD_POID, *out_flpp, PIN_FLD_ZONEMAP_OBJ, &ebuf);
				ebuf.pin_err  = PIN_ERR_STALE_CONF;
			}
		}
	}
	return ebuf.pin_err;
}

/*----------------------------------------------------------------------------+
 | Function     : fm_zonemap_pol_load_maps
 |
 | Description  : Loads all zonemaps into memory at startup so they do not need to be
 |                loaded on-demand by each CM child when starting. This will just load
 |                the list of zonemaps via a search and then call '' to do the actual
 |                load of the matrix.
 |
 | Input        : None
 |
 | Output       : z_flpp    Pointer to flist pointer (updated)
 |
 | Return       : 0(PIN_ERR_NONE) if all OK, non-zero otherwise
 +----------------------------------------------------------------------------*/
static int32
fm_zonemap_pol_load_maps (
	pin_flist_t 	**z_flpp)
{
	char msg[512];
	pcm_context_t 	*ctxp = NULL;
	pin_flist_t	*ri_flp = NULL;
	pin_flist_t 	*ro_flp = NULL;
	pin_flist_t 	*zi_flp = NULL;
	pin_flist_t 	*zo_flp = NULL;
	pin_flist_t 	*a_flp = NULL;
	poid_t 		*pdp = NULL;
	char 		*name = NULL;
	int64 		dbno = 0;
	pin_errbuf_t 	ebuf;
	int32 		sflags = 0;
	pin_flist_t 	*z_flp = NULL;
	pin_cookie_t 	cookie = NULL;
	int32 		elemid = 0;
	int32 		*ip = NULL;
	int32 		rv = 0;
	int32 		bufsize = 0;
	int32 		nmaps = 0;
	pin_buf_t 	*bufp = NULL;
	int32 		err = 0;
	Blob_t 		*blobp = NULL;
	u_int32 	blob_ver = 0;
	time_t 		next_refresh_t = 0;

	*z_flpp = NULL;

	/*
	 * Check pin.conf for caching of zonemaps.
	 *  0 = none
	 *  1 = load into regular memory
	 *  2 = load into shared memory
	 */
	g_startup_cache_type = STARTUP_CACHE_NONE;
	pin_conf("fm_zonemap_pol", "startup_cache_type", PIN_FLDT_INT, (caddr_t *)&ip, &rv);
	if (rv == PIN_ERR_NONE) {
		g_startup_cache_type = *ip;
		pin_free(ip); ip = NULL;
		if (g_startup_cache_type != STARTUP_CACHE_NONE &&
			g_startup_cache_type != STARTUP_CACHE_HEAP &&
			g_startup_cache_type != STARTUP_CACHE_SHARED)
		{
			pin_snprintf(msg, sizeof(msg),
				"fm_zonemap_pol_load_maps: fm_zonemap_pol startup_cache has bad value[%d] - must be 0, 1, 2",
				g_startup_cache_type);
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_WARNING, msg);
			g_startup_cache_type = STARTUP_CACHE_NONE;
		}
	} else {
		rv = 0;
	}
	pin_snprintf(msg, sizeof(msg),
		"fm_zonemap_pol_load_maps: startup_cache set to %d", g_startup_cache_type);
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);

	if (g_startup_cache_type == STARTUP_CACHE_NONE) {
		return PIN_ERR_NONE;
	}

	/*
	 * we're caching ... connect to BRM ...
	 */
	PIN_ERRBUF_CLEAR(&ebuf);
	PCM_CONTEXT_OPEN(&ctxp, 0, &ebuf);
	if (PIN_ERR_IS_ERR(&ebuf)) {
		goto finish;
	}
	pdp = PCM_GET_USERID(ctxp);
	dbno = PIN_POID_GET_DB(pdp);
	PIN_POID_DESTROY(pdp, NULL); pdp = NULL;

	/*
	 * Find all zonemaps for all brands ... Don't read the zonemap buffer itself now, as they can be
	 * large and we don't want a single big search to be too greedy on DMO memory.
	 */
	zi_flp = PIN_FLIST_CREATE(&ebuf);
	pdp = PIN_POID_CREATE(dbno, "/search", -1, &ebuf);
	PIN_FLIST_FLD_PUT(zi_flp, PIN_FLD_POID, pdp, &ebuf);
	pdp = NULL;
	PIN_FLIST_FLD_SET(zi_flp, PIN_FLD_TEMPLATE, "select X from /zonemap where F1 like V1 ", &ebuf);
	PIN_FLIST_FLD_SET(zi_flp, PIN_FLD_FLAGS, &sflags, &ebuf);
	a_flp = PIN_FLIST_ELEM_ADD(zi_flp, PIN_FLD_ARGS, 1, &ebuf);
	pdp = PIN_POID_CREATE(dbno, "/zonemap", -1, &ebuf);
	PIN_FLIST_FLD_PUT(a_flp, PIN_FLD_POID, pdp, &ebuf);
	pdp = NULL;

        a_flp = PIN_FLIST_ELEM_ADD(zi_flp, PIN_FLD_RESULTS, 0, &ebuf);
        PIN_FLIST_FLD_SET(a_flp, PIN_FLD_POID, NULL, &ebuf);
        PIN_FLIST_FLD_SET(a_flp, PIN_FLD_ACCOUNT_OBJ, NULL, &ebuf);
        PIN_FLIST_FLD_SET(a_flp, PIN_FLD_NAME, NULL, &ebuf);
        PIN_FLIST_FLD_SET(a_flp, PIN_FLD_DESCR, NULL, &ebuf);
        PIN_FLIST_FLD_SET(a_flp, PIN_FLD_ZONEMAP_SEARCH_TYPE, NULL, &ebuf);
        PIN_FLIST_FLD_SET(a_flp, PIN_FLD_ZONEMAP_DATA_TYPE, NULL, &ebuf);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"fm_zonemap_pol_load_maps: search zonemaps input flist", zi_flp);

	PCM_OP(ctxp, PCM_OP_SEARCH, 0, zi_flp, &zo_flp, &ebuf);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_zonemap_pol_load_maps: search zonemaps output flist", zo_flp);

	/*
	 * If we need to initialize the shared-memory cache for zonemaps, do so ...
	 */
	if (g_startup_cache_type == STARTUP_CACHE_SHARED) {
		err = PIN_ERR_NONE;
		g_zone_cachep = cm_cache_init_byname("fm_zonemap_cache", CM_CACHE_KEY_STR, &err);
		if (err != PIN_ERR_NONE) {
			pin_snprintf(msg, sizeof(msg),
				"fm_zonemap_pol_load_maps: failed creating zonemap cache err[%d]", err);
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
			pin_set_err(&ebuf, PIN_ERRLOC_FM, PIN_ERRCLASS_SYSTEM_DETERMINATE, PIN_ERR_NO_MEM, 0, 0, err);
			goto finish;
		}
	}
        /*
         * Get each zonemap and load it into cache ... If we're using the shared-memory cache, then
         * load things into local memory, set in the mmap cache and we're done. If we're loading into
         * local CM cache, then load directly into the local hash map.
         */

	while ((z_flp = PIN_FLIST_ELEM_GET_NEXT(zo_flp, PIN_FLD_RESULTS, &elemid, 1, &cookie, &ebuf))) {
		pdp = PIN_FLIST_FLD_GET(z_flp, PIN_FLD_POID, 0, &ebuf);
		name = PIN_FLIST_FLD_GET(z_flp, PIN_FLD_NAME, 0, &ebuf);

		if (g_startup_cache_type == STARTUP_CACHE_HEAP) {
			/*
			 * We will load the derived buffer data into cache. This, will remain in network byte order and,
			 * potentially, even a wrong version, but will be converted on first use. This corner case will
			 * give less advantage, but is simple - as we cannot call policy logic to do this work from
			 * an initialization function.
			 */
			ri_flp = PIN_FLIST_CREATE(&ebuf);
			PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_POID, pdp, &ebuf);
			PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, NULL, &ebuf);

			PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, ri_flp, &ro_flp, &ebuf);

			bufp = (pin_buf_t *) PIN_FLIST_FLD_TAKE(ro_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, 0, &ebuf);
			bufsize= bufp->size;
			PIN_FLIST_FLD_PUT(z_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, bufp, &ebuf);

			if (!PIN_ERR_IS_ERR(&ebuf)) {
				pin_snprintf(msg, sizeof(msg), "fm_zonemap_pol_load_maps: loaded map[%.255s] of %d bytes into cache",
					name, bufsize);
				PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, msg, z_flp);
			}


			PIN_FLIST_DESTROY_EX(&ri_flp, NULL);
			PIN_FLIST_DESTROY_EX(&ro_flp, NULL);
			++nmaps;
		} else if (g_startup_cache_type == STARTUP_CACHE_SHARED) {
			/*
			 * We cannot call functions to transform and rewrite the BLOB at startup so if we find
			 * an old version, then ignore it so that it can be converted dynamically at run-time  (which
			 * would be a one-off operation. We'll log an error in this case so people are aware of it).
			 */
			ri_flp = PIN_FLIST_CREATE(&ebuf);
			PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_POID, pdp, &ebuf);
			PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_NAME, NULL, &ebuf);
			PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_DESCR, NULL, &ebuf);
			PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_ZONEMAP_SEARCH_TYPE, NULL, &ebuf);
			PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_ZONEMAP_DATA_TYPE, NULL, &ebuf);
			PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, NULL, &ebuf);

			PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, ri_flp, &ro_flp, &ebuf);

			bufp = (pin_buf_t *)PIN_FLIST_FLD_GET(ro_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, 0, &ebuf);
			blobp = (Blob_t *)bufp->data;
			blob_ver = (u_int32)ntohl((u_int32)blobp->version);

			if (blob_ver == ZONEMAP_CURRENT_VERSION) {
				rv = cm_cache_add_entry_post_exec(g_zone_cachep, (void *)name, ro_flp,
				fm_zonemap_pol_pre_process_shm_blob, ro_flp, NULL, &err);
				switch (err) {
					case PIN_ERR_NONE:
						break;
					case PIN_ERR_OP_ALREADY_DONE:
						pin_snprintf(msg, sizeof(msg), "fm_zonemap_pol_load_maps: map[%.255s] already cached", name);
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_WARNING, msg);
						break;
					default:
						pin_snprintf(msg, sizeof(msg), "fm_zonemap_pol_load_maps: map[%.255s] failed cache insert", name);
						PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
						pin_set_err(&ebuf, PIN_ERRLOC_FM, PIN_ERRCLASS_SYSTEM_DETERMINATE, PIN_ERR_NO_MEM, 0, 0, err);
						break;
				}
				++nmaps;
			} else {
				pin_snprintf(msg, sizeof(msg),
					"fm_zonemap_pol_load_maps: map[%.255s] has version[%d] rather than version[%d] - defer transform",
					name, blob_ver, ZONEMAP_CURRENT_VERSION);
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
			}
			PIN_FLIST_DESTROY_EX(&ri_flp, NULL);
			PIN_FLIST_DESTROY_EX(&ro_flp, NULL);
		}

	}

finish:
        /*
         * If all is well, set the output flist value ...
         */
        if (PIN_ERR_IS_ERR(&ebuf)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_zonemap_pol_load_maps: error loading zonemaps into cache", &ebuf);
                rv = ebuf.pin_err;
        } else {
                *z_flpp = zo_flp;
                zo_flp = NULL;
                pin_snprintf(msg, sizeof(msg), "fm_zonemap_pol_load_maps: loaded %d maps into cache", nmaps);
                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);
        }
	/*
	 * Cleanup ...
	 */
	PIN_FLIST_DESTROY_EX(&zi_flp, NULL);
	PIN_FLIST_DESTROY_EX(&zo_flp, NULL);
	PCM_CONTEXT_CLOSE(ctxp, 0, &ebuf);

	return rv;
}

/*----------------------------------------------------------------------------+
 | Function     : fm_zonemap_pol_search_startup_cache
 |
 | Description  : Searches the startup zonemap cache for the particular map.
 |                If found, the zonemap will be removed from the startup cache
 |                and returned. The calling application will be responsible for
 |                the destruction of this.
 |
 | Input        : ctxp     PCM context
 |                target   Zonemap name
 |                bpdp     Brand poid
 |                dt       Zonemap data type
 |
 | Output       : None
 |
 | Return       : zonemap flist, if found, NULL otherwise.
 +----------------------------------------------------------------------------*/
static pin_flist_t *
fm_zonemap_pol_search_startup_cache (
	pcm_context_t 		*ctxp,
	char 			*target,
        poid_t 			*bpdp,
	pin_zonemap_data_type_t	dt)
{
	pin_flist_t *z_flp = NULL;
	pin_cookie_t cookie = NULL;
	int32 elemid = 0;
	char *name = NULL;
	pin_errbuf_t ebuf;
	poid_t *cbpdp = NULL;
	pin_flist_t *pz_flp = NULL;
	pin_zonemap_data_type_t *dtp = NULL;
	pin_zonemap_data_type_t *odtp = NULL;
	void *vp = NULL;
	int32 isAccExists = 1;

	PIN_ERRBUF_CLEAR(&ebuf);

	while ((z_flp = PIN_FLIST_ELEM_GET_NEXT(g_zone_flp, PIN_FLD_RESULTS, &elemid, 1, &cookie, &ebuf)))
	{
		name = PIN_FLIST_FLD_GET(z_flp, PIN_FLD_NAME, 0, &ebuf);
		cbpdp = PIN_FLIST_FLD_GET(z_flp, PIN_FLD_ACCOUNT_OBJ, 0, &ebuf);

		isAccExists = bpdp ? (cbpdp && (PIN_POID_COMPARE(cbpdp, bpdp, 0, &ebuf) == 0) ? 1 : 0 ) : 1;

		if (name && strcmp (name, target) == 0  && isAccExists) {
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
					"fm_zonemap_pol_search_startup_cache: found zonemap");
			break; /* found it! */
		}
	}

	if (PIN_ERR_IS_ERR(&ebuf)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_zonemap_pol_search_startup_cache: error searching cache", &ebuf);
		z_flp = NULL;
	}

	if (z_flp) {
		/*
		 * Take it from our startup cache - it will now be managed using the
		 * normal caching logic. Before returning the data, call the policy to
		 * pre-process the zonemap data into host network order.
		 */
		z_flp = PIN_FLIST_ELEM_TAKE(g_zone_flp, PIN_FLD_RESULTS, elemid, 0, &ebuf);
		odtp = PIN_FLIST_FLD_TAKE(z_flp, PIN_FLD_ZONEMAP_DATA_TYPE, 0, &ebuf);
		PIN_FLIST_FLD_SET(z_flp, PIN_FLD_ZONEMAP_DATA_TYPE, &dt, &ebuf);

		PCM_OP(ctxp, PCM_OP_ZONEMAP_POL_GET_ZONEMAP, 0, z_flp, &pz_flp, &ebuf);

		/*
		 * Replace derived data on z_flp ...
		 */
		PIN_FLIST_FLD_PUT(z_flp, PIN_FLD_ZONEMAP_DATA_TYPE, odtp, &ebuf);
		vp = PIN_FLIST_FLD_TAKE(pz_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, 0, &ebuf);
		PIN_FLIST_FLD_PUT(z_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, vp, &ebuf);

		PIN_FLIST_DESTROY_EX(&pz_flp, NULL);
	}

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"fm_zonemap_pol_search_startup_cache: z_flp", z_flp);

	return z_flp;
}

/*----------------------------------------------------------------------------+
 | Function     : fm_zonemap_pol_load_map_into_shm
 |
 | Description  : Finds a zonemap and loads it into the shared memory cache (if it exists).
 |                If it is the wrong version it will be converted 'on the fly'.
 |
 | Input        : ctxp        PCM context
 |                name        The name of the zonemap
 |                ebufp       Error buffer.
 |
 | Output       : None
 |
 | Return       : None
 +----------------------------------------------------------------------------*/
static void
fm_zonemap_pol_load_map_into_shm(
	pcm_context_t           *ctxp,
	char                    *name,
	pin_errbuf_t            *ebufp)
{
	char msg[1024];
	pin_flist_t 	*si_flp = NULL;
	pin_flist_t 	*so_flp = NULL;
	pin_flist_t 	*a_flp = NULL;
	pin_flist_t 	*zmap_flp = NULL;
	pin_flist_t 	*new_blob_flp = NULL;
	pin_flist_t 	*pdp = NULL;
	int32 		rv = 0;
	int32 		err = 0;
	int32 		sflags = 0;
	u_int32 	blob_ver = 0;
	pin_buf_t 	*bufp = NULL;
	Blob_t 		*blobp = NULL;
	time_t 		next_refresh_t = 0;

	pin_snprintf(msg, sizeof(msg),
		"fm_zonemap_pol_load_map_into_shm : load zonemap name[%.256s]", name);
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, msg);

	pdp = PCM_GET_USERID(ctxp);
	si_flp = PIN_FLIST_CREATE(ebufp);
	pdp = PIN_POID_CREATE(PIN_POID_GET_DB(pdp), "/search", -1, ebufp);
	PIN_FLIST_FLD_PUT(si_flp, PIN_FLD_POID, pdp, ebufp);
	PIN_FLIST_FLD_SET(si_flp, PIN_FLD_FLAGS, &sflags, ebufp);
	PIN_FLIST_FLD_SET(si_flp, PIN_FLD_TEMPLATE, "select X from /zonemap where F1 = V1", ebufp);
	a_flp = PIN_FLIST_ELEM_ADD(si_flp, PIN_FLD_ARGS, 1, ebufp);
	PIN_FLIST_FLD_SET(a_flp, PIN_FLD_NAME, name, ebufp);
	a_flp = PIN_FLIST_ELEM_ADD(si_flp, PIN_FLD_RESULTS, 0, ebufp);
	PIN_FLIST_FLD_SET(a_flp, PIN_FLD_POID, NULL, ebufp);
	PIN_FLIST_FLD_SET(a_flp, PIN_FLD_NAME, NULL, ebufp);
	PIN_FLIST_FLD_SET(a_flp, PIN_FLD_DESCR, NULL, ebufp);
	PIN_FLIST_FLD_SET(a_flp, PIN_FLD_ZONEMAP_SEARCH_TYPE, NULL, ebufp);
	PIN_FLIST_FLD_SET(a_flp, PIN_FLD_ZONEMAP_DATA_TYPE, NULL, ebufp);
	PIN_FLIST_FLD_SET(a_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, NULL, ebufp);

	pin_snprintf(msg, sizeof(msg),
		"fm_zonemap_pol_load_map_into_shm : load zonemap name[%.256s] search flist", name);
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, msg, si_flp);

	PCM_OP(ctxp, PCM_OP_SEARCH, 0, si_flp, &so_flp, ebufp);

	/*
	 * Check the derived buffer ... if it's not the correct version, fix it on the fly.
	 */
	zmap_flp = PIN_FLIST_ELEM_GET(so_flp, PIN_FLD_RESULTS, 0, 0, ebufp);
	bufp = PIN_FLIST_FLD_GET(zmap_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, 0, ebufp);
	if (bufp && bufp->data) {
		blobp = (Blob_t *)bufp->data;
		blob_ver = (u_int32)ntohl((u_int32)blobp->version);
		if (blob_ver != ZONEMAP_CURRENT_VERSION) {
			fm_zonemap_pol_convert_zonemap(ctxp, zmap_flp, &new_blob_flp, ebufp);
			bufp = PIN_FLIST_FLD_TAKE(new_blob_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, 0, ebufp);
			PIN_FLIST_FLD_PUT(zmap_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, bufp, ebufp);
		}
	}
	if (PIN_ERR_IS_ERR(ebufp) || bufp == NULL || bufp->data == NULL) {
		goto cleanup;
	}

	/*
	 * We've got a good buffer, so pop it into shared memory ...
	 */
	next_refresh_t = pin_virtual_time(NULL) + interval;
	PIN_FLIST_FLD_SET(zmap_flp, PIN_FLD_WHEN_T, &next_refresh_t, ebufp);

	rv = cm_cache_update_entry_post_exec(g_zone_cachep, (void *)name, zmap_flp,
	fm_zonemap_pol_pre_process_shm_blob, zmap_flp, NULL, &err);
	switch (err) {
	case PIN_ERR_NONE:
	case PIN_ERR_OP_ALREADY_DONE:
		break;
	default:
		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_SYSTEM_DETERMINATE, PIN_ERR_NO_MEM, 0, 0, err);
		break;
	}
cleanup:
	if (PIN_ERR_IS_ERR(ebufp)) {
		pin_snprintf(msg, sizeof(msg),
			"fm_zonemap_pol_load_map_into_shm: map[%.255s] failed update", name);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, msg, ebufp);
	} else {
		pin_snprintf(msg, sizeof(msg),
		"fm_zonemap_pol_load_map_into_shm: map[%.255s] succesfully updated", name);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);
	}
	PIN_FLIST_DESTROY_EX(&si_flp, NULL);
	PIN_FLIST_DESTROY_EX(&so_flp, NULL);
	PIN_FLIST_DESTROY_EX(&new_blob_flp, NULL);
	return;
}
