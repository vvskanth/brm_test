/* Copyright (c) 2016,2020 Oracle and/or its affiliates. All rights reserved.*/
 
/* 
   NAME 
     fm_utils_bparams_cache.h - <one-line expansion of the name> 

   DESCRIPTION 
     <short description of facility this file declares/defines> 

   RELATED DOCUMENTS 
     <note any documents related to this facility>
 
   EXPORT FUNCTION(S) 
     <external functions declared for use outside package - one-line descriptions>

   INTERNAL FUNCTION(S)
     <other external functions declared - one-line descriptions>

   EXAMPLES

   NOTES
     <other useful comments, qualifications, etc.>

   MODIFIED   (MM/DD/YY)
   anirbisw    02/02/16 - for BCC business params caching .
   anirbisw    02/02/16 - Creation

*/

#ifndef FM_UTILS_BPARAMS_CACHE_H
# define FM_UTILS_BPARAMS_CACHE_H
#include <pcm.h>

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                     PRIVATE TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                           EXPORT FUNCTIONS
  ---------------------------------------------------------------------------*/
/*
 * Method to init the business params cache.
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   database 	 - The database number.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 * Returns: The FLIST representation of the value associated with the specified
 *   parameter.  If the parameter does not exist then NULL will be returned.
 */
extern void
fm_utils_init_bparams_cache(
				pcm_context_t 	*ctxp,
				int64 			database,
				pin_errbuf_t 	*ebufp);

/*
 * Get the specified parameter as a FList value from a /config/business_param
 * configuration object using the specified Infranet connection.
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.) to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the value for.
 *   param_must_exist - A flag to check whether to turn on displaying errors or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 * Returns: The FLIST representation of the value associated with the specified
 *   parameter.  If the parameter does not exist then NULL will be returned.
 *
 *       PIN_FLD_DESCR           STR [0] "Description"
 *       PIN_FLD_PARAM_NAME      STR [0] "billing_cycle_offset"
 *       PIN_FLD_PARAM_TYPE      INT [0] 1
 *       PIN_FLD_PARAM_VALUE     STR [0] "0"
 */
extern pin_flist_t* 
fm_utils_bparams_cache_get_param (        
				pcm_context_t*  contextp,
				const char*     group_namep,
				const char*     param_namep,
				int32           param_must_exist,
				pin_errbuf_t*   ebufp);

/*
 * Get the specified parameter as a FList value from the CM
 * transaction cache using the specified Infranet connection.
 * This function will maintain parameter consistency in a transaction boundary
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   param_must_exist - A flag to check whether to turn on displaying errors
 *                      or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 * Returns:
 *   The FLIST representation of the value associated with the specified
 *   parameter.  If the parameter does not exist then NULL will be returned.
 *
 *       PIN_FLD_DESCR           STR [0] "Description"
 *       PIN_FLD_PARAM_NAME      STR [0] "billing_cycle_offset"
 *       PIN_FLD_PARAM_TYPE      INT [0] 1
 *       PIN_FLD_PARAM_VALUE     STR [0] "0"
 */
extern pin_flist_t* 
fm_utils_bparams_txn_cache_get_param (        
		pcm_context_t*  contextp,
        	const char*     group_namep,
        	const char*     param_namep,
        	int32           param_must_exist,
        	pin_errbuf_t*   ebufp);
/*
 * Get the specified parameter as an integer value from a /config/business_param
 * configuration object using the specified Infranet connection.
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   param_must_exist - A flag to check whether to turn on displaying errors
 *                      or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 * Returns:
 *   The integer representation of the value associated with the specified
 *   parameter.  If the parameter does not exist or the cannot be converted
 *   to an integer then a value of zero will be returned.
 */

extern int32
fm_utils_bparams_cache_get_int (
                                pcm_context_t*  contextp,
                                const char*     group_namep,
                                const char*     param_namep,
                                int32           param_must_exist,
                                pin_errbuf_t*   ebufp);

/*
 * Get the specified parameter as an integer value from a /config/business_param
 * configuration object using the specified Infranet connection.
 * This function will maintain parameter consistency in a transaction boundary
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   param_must_exist - A flag to check whether to turn on displaying errors
 *                      or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 * Returns:
 *   The integer representation of the value associated with the specified
 *   parameter.  If the parameter does not exist or the cannot be converted
 *   to an integer then a value of zero will be returned.
 */

extern int32
fm_utils_bparams_txn_cache_get_int (
                                pcm_context_t*  contextp,
                                const char*     group_namep,
                                const char*     param_namep,
                                int32           param_must_exist,
                                pin_errbuf_t*   ebufp);
/*
 * Get the specified parameter as a decimal value from a /config/business_param
 * configuration object using the specified Infranet connection.
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   param_must_exist - A flag to check whether to turn on displaying errors
 *                      or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 * Returns:
 *   The decimal representation of the value associated with the specified
 *   parameter.  If the parameter does not exist or the cannot be converted
 *   to an decimal value then a null pointer will be returned.
 *
 *  It will be the responsibility of the caller to free the decimal value.
 */
extern pin_decimal_t*
fm_utils_bparams_cache_get_decimal (
                                pcm_context_t*  contextp,
                                const char*     group_namep,
                                const char*     param_namep,
                                int32           param_must_exist,
                                pin_errbuf_t*   ebufp);

/*
 * Get the specified parameter as a decimal value from a /config/business_param
 * configuration object using the specified Infranet connection.
 * This function will maintain parameter consistency in a transaction boundary
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   param_must_exist - A flag to check whether to turn on displaying errors
 *                      or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 * Returns:
 *   The decimal representation of the value associated with the specified
 *   parameter.  If the parameter does not exist or the cannot be converted
 *   to an decimal value then a null pointer will be returned.
 *
 *  It will be the responsibility of the caller to free the decimal value.
 */
extern pin_decimal_t*
fm_utils_bparams_txn_cache_get_decimal (
                                pcm_context_t*  contextp,
                                const char*     group_namep,
                                const char*     param_namep,
                                int32           param_must_exist,
                                pin_errbuf_t*   ebufp);

/*
 * Get the specified parameter as a character array from a /config/business_param
 * configuration object using the specified Infranet connection.
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   bufp        - Pointer to the buffer array to contain the string.
 *   buf_size    - The size of the buffer array (bufp) to contain the string
 *                 representation of the parameter.  The size is in characters
 *                 and includes the null terminator.  If buf_size is 10 then
 *                 bufp points to a buffer that contain a string with a
 *                 maximum of 9 characters.
 *   param_must_exist - A flag to check whether to turn on displaying errors
 *                      or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 *
 * Returns:
 *   Pointer to the character buffer passed in to contain the string
 *   representation of the value associated with the specified parameter.  If
 *   the parameter does not exist or cannot be converted to a string then an
 *   "empty string" will be returned.
 */
extern char*
fm_utils_bparams_cache_get_str (
                                pcm_context_t*  contextp,
                                const char*     group_namep,
                                const char*     param_namep,
                                int32           param_must_exist,
                                pin_errbuf_t*   ebufp);
/*
 * Get the specified parameter as a character array from a /config/business_param
 * configuration object using the specified Infranet connection.
 * This function will maintain parameter consistency in a transaction boundary
 *
 * Parameters:
 *   contextp    - A connection to Infranet.
 *   group_namep - The name of the group ("billing", "rating", "vse_ipt", etc.)
 *                 to retrieve
 *                 the parameter from.
 *   param_namep - The name of the configuration parameter to retrieve the
 *                 value for.
 *   bufp        - Pointer to the buffer array to contain the string.
 *   buf_size    - The size of the buffer array (bufp) to contain the string
 *                 representation of the parameter.  The size is in characters
 *                 and includes the null terminator.  If buf_size is 10 then
 *                 bufp points to a buffer that contain a string with a
 *                 maximum of 9 characters.
 *   param_must_exist - A flag to check whether to turn on displaying errors
 *                      or NOT.
 *   ebufp       - If the parameter does not exist or there was a problem with
 *                 the connection then ebufp will contain an error.
 *
 *
 * Returns:
 *   Pointer to the character buffer passed in to contain the string
 *   representation of the value associated with the specified parameter.  If
 *   the parameter does not exist or cannot be converted to a string then an
 *   "empty string" will be returned.
 */
extern char*
fm_utils_bparams_txn_cache_get_str (
                                pcm_context_t*  contextp,
                                const char*     group_namep,
                                const char*     param_namep,
                                int32           param_must_exist,
                                pin_errbuf_t*   ebufp);


void
fm_utils_bparam_cache_update_from_pinconf(
                                const char*     group_namep,
                                const char*     param_namep,
                                char*           valuep,
                                pin_errbuf_t*   ebufp);


/*---------------------------------------------------------------------------
                          INTERNAL FUNCTIONS
  ---------------------------------------------------------------------------*/
void
fm_utils_bparams_cache_create(
		pcm_context_t * ctxp,
             	int64           database,
        	pin_errbuf_t    *ebufp);


void
fm_utils_refresh_bparams_cache(
        pcm_context_t   *ctxp,
        pin_errbuf_t    *ebufp);

time_t
fm_utils_bparams_get_last_modified(
	const char*	config_type,
	pin_errbuf_t	*ebufp);
#ifdef __cplusplus
}
#endif

#endif                                              /* FM_UTILS_BPARAMS_CACHE_H */
