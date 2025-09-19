/*
 * @(#)%Portal Version: fm_num_pol_util.c:WirelessVelocityInt:1:2006-Sep-14 11:27:48 %
 *
 * Copyright (c) 2001 - 2023 Oracle. 
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#) %full_filespec: fm_num_pol_util.c~2:csrc:1 %";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "cm_fm.h"
#include "pin_errs.h"
#include "ops/num.h"

#define FILE_LOGNAME "fm_num_pol_util.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
void
fm_num_pol_util_device_id_exists(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp);

void
fm_num_pol_util_canonicalize_number(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp);


/*******************************************************************************
 * fm_num_pol_util_device_id_exists()
 *
 * Inputs: flist with device_id and poid of a /device
 * Outputs: void; ebuf set if device_id already exists
 *
 * Description:
 * This function searches the database for an existing object with the same
 * device_id as the input argument.  If one or more records are found, the
 * ebuf is set indicating the device_id already exists.
 ******************************************************************************/
void
fm_num_pol_util_device_id_exists(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp)
{
        int32                   cred = 0;               /* used for scopeing */
        int32                   s_flags = 0;            /* for searching */
        int64                   database = 0;           /* route the search */
        pin_flist_t             *search_flistp = NULL;  /* search input */
        pin_flist_t             *args_flistp = NULL;    /* search args */
        pin_flist_t             *r_flistp = NULL;       /* search output */
        poid_t                  *srchpp = NULL;         /* for routing */
        poid_t                  *dn_poidp = NULL;       /* for searching */
        char                    *template = NULL;       /* search template */
        char                    *poid_type = NULL;      /* for searches */
        void                    *vp = NULL;             /* for flist gets */
	int32			elemid = 0;		/* for results check */
	pin_cookie_t            cookie = NULL;		/* for results check */


        if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
	PIN_ERR_CLEAR_ERR(ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_num_pol_util_device_id_exists input flist", i_flistp);

        /*
         * Setup search flist for uniqueness check
         */
        search_flistp = PIN_FLIST_CREATE(ebufp);

        vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);

        database = PIN_POID_GET_DB((poid_t *)vp);
        poid_type = (char *)PIN_POID_GET_TYPE((poid_t *)vp);

        srchpp = PIN_POID_CREATE(database, "/search", -1, ebufp);
        PIN_FLIST_FLD_PUT(search_flistp, PIN_FLD_POID, 
		srchpp, ebufp);

        PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_FLAGS, &s_flags, ebufp);

        template = "select X from /device where ( F1 = V1 and F2 = V2 ) ";
        PIN_FLIST_FLD_SET(search_flistp, PIN_FLD_TEMPLATE, 
		(void *)template, ebufp);

        /*
         * Add the first argument to the flist - the device_id
         */
        args_flistp = PIN_FLIST_ELEM_ADD(search_flistp,
                PIN_FLD_ARGS, 1, ebufp);

	/* Make sure the number is canonicalized before adding it */
	fm_num_pol_util_canonicalize_number(ctxp, i_flistp, ebufp);

        vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_DEVICE_ID, 0, ebufp);
        PIN_FLIST_FLD_SET(args_flistp, PIN_FLD_DEVICE_ID, vp, ebufp);

        /*
         * Add the second argument to the flist - the poid type
         */
        args_flistp = PIN_FLIST_ELEM_ADD(search_flistp,
                PIN_FLD_ARGS, 2, ebufp);

        dn_poidp = PIN_POID_CREATE(database, poid_type, -1, ebufp);
        PIN_FLIST_FLD_PUT(args_flistp, PIN_FLD_POID, 
		dn_poidp, ebufp);

        /*
         * Add the null results array
         */
	PIN_FLIST_ELEM_SET(search_flistp, NULL, PIN_FLD_RESULTS, 0, ebufp);

        /*
         * Perform the search (system-wide)
         */
        cred = CM_FM_BEGIN_OVERRIDE_SCOPE(CM_CRED_SCOPE_OVERRIDE_ROOT);

        PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, PCM_OPFLG_COUNT_ONLY, search_flistp,
                &r_flistp, ebufp);

        CM_FM_END_OVERRIDE_SCOPE(cred);

        /*
         * If /device with given device_id already exists, set ebuf
         */
	PIN_FLIST_ELEM_GET_NEXT(r_flistp, PIN_FLD_RESULTS, &elemid, 0, 
		&cookie, ebufp);

	if (elemid > 0) {

                pin_set_err(ebufp, PIN_ERRLOC_FM,
                        PIN_ERRCLASS_SYSTEM_DETERMINATE,
                        PIN_ERR_DUPLICATE, PIN_FLD_DEVICE_ID, 0, 0);

                PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                        "fm_num_pol_util_device_id_exists new device_id "
                        "already exists");
        }

        PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
        return;
}

/*******************************************************************************
 * fm_num_pol_util_canonicalize_number()
 *
 * Input: flist with device_id
 * Output: input flist with number in canon form;
 *      ebuf set if errors encountered.
 *
 * Description:
 * This function calls the number canonicalization policy on the device_id
 * and returns the results.
 ******************************************************************************/
void
fm_num_pol_util_canonicalize_number(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp)
{
        void                    *vp = NULL;
        int64                   db_no;
        poid_t                  *pp = NULL;
        poid_t                  *i_poidp = NULL;
        pin_flist_t             *s_flistp = NULL;
        pin_flist_t             *sub_flistp = NULL;
        pin_flist_t             *co_flistp = NULL;
        pin_flist_t             *canon_flistp = NULL;

        if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
        PIN_ERR_CLEAR_ERR(ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_num_pol_util_canonicalize_number input flist", i_flistp);

        s_flistp = PIN_FLIST_CREATE(ebufp);

        i_poidp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
        db_no = PIN_POID_GET_DB(i_poidp);
        pp = PIN_POID_CREATE(db_no, "/dummy", -1, ebufp);
        PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, pp, ebufp);

        /*
         * Set the device_id
         */
        sub_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_NUMBERS,
                0, ebufp);
        vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_DEVICE_ID, 0, ebufp);
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_NUMBER, vp, ebufp);

        /*
         * Call the policy opcode
         */
        PCM_OP(ctxp, PCM_OP_NUM_POL_CANONICALIZE, 0, s_flistp, &co_flistp,
                ebufp);

	/*
	 * Get the canonicalized number and update the device_id from
	 * the input flist.
	 */
        canon_flistp = PIN_FLIST_ELEM_GET(co_flistp, PIN_FLD_NUMBERS, 0, 0,
                        ebufp);
        vp = PIN_FLIST_FLD_GET(canon_flistp, PIN_FLD_NUMBER, 0,
                ebufp);

        /*
         * Replace outbound device id with this canonicalized one
         */
        PIN_FLIST_FLD_SET(i_flistp, PIN_FLD_DEVICE_ID, vp, ebufp);

        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_num_pol_util_canonicalize_number error", ebufp);
        }

        PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&co_flistp, NULL);
        return;
}

/*******************************************************************
 * Fct used to add or delete an alias to/from the service alias list
 * that are passed into the i_flist
 * This will be firstly used by GSM manager but possibly by other
 * manager later
 * This is used by Integrate integration and also by webkit as possible
 * login
 *
 **** PCM_OP_CUST_SET_LOGIN version: will be removed when               *****
 **** PETS 40155 will be fixed. Quest use need this one in the meantime *****
 * input:
 *  i_flist: an flist, where we get services poid and account poid
 *           from the PIN_FLD_SERVICES array
 *           An assumption is that only the services of one account
 *           are are listed here
 *  id: alias element id in the service alias list
 *  alias : Add: alias to add
 *          Delete: NULL, indicates a delete
 *
 * output:
 *  ebuf: set in case of error
 *******************************************************************/
void
fm_utils_wireless_alias_to_services(
        pcm_context_t           *ctxp,
        pin_flist_t             *i_flistp,
        char                    *alias,
        int32                   id,
        pin_errbuf_t            *ebufp)
{
        pin_flist_t             *s_flistp = NULL;      /* set flist     */
        pin_flist_t             *log_flistp = NULL;    /* login flist   */
        pin_flist_t             *al_flistp = NULL;     /* alias flist   */
        pin_flist_t             *r_flistp = NULL;      /* return flist  */
        pin_flist_t             *svc_flistp = NULL;    /* service list flist*/
        poid_t                  *svc_poid = NULL;      /* service poid */
        poid_t                  *acct_poid = NULL;     /* account poid */
        int32                   elem_id = 0;    /* array element        */
        pin_cookie_t            cookie = NULL;  /* to loop through array */
        void                    *vp=NULL;
        if (PIN_ERR_IS_ERR(ebufp)) {
                return;
                /*********/
        }

        PIN_ERR_CLEAR_ERR(ebufp);

        /*
         * create and set the update_services input flist
         */
        s_flistp = PIN_FLIST_CREATE(ebufp);

        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_PROGRAM_NAME, FILE_LOGNAME,
                          ebufp);
        log_flistp = PIN_FLIST_CREATE(ebufp);
        /*
         * is it an add or a delete
         */
        if (alias == NULL) {
                /*
                 * Remove the alias from the service alias list
                 * set the flist to NULL for the id
                 */
			PIN_FLIST_ELEM_SET(log_flistp, (void*)NULL,
                                   PIN_FLD_ALIAS_LIST, id, ebufp);
        }
        else
        {
                /*
                 * Set the alias in ALIAS_LIST[id] (id is important)
                 */
                al_flistp = PIN_FLIST_ELEM_ADD(log_flistp, PIN_FLD_ALIAS_LIST,
                                               id, ebufp);
                PIN_FLIST_FLD_SET(al_flistp, PIN_FLD_NAME, alias, ebufp);
        }

        /*
         * loop through the array of all services and
         * build SET_LOGIN input flist and call the opcode
         */
        /* if END_T is passed by caller */
        vp=PIN_FLIST_FLD_GET(i_flistp,PIN_FLD_END_T,1,ebufp);

        while((svc_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, PIN_FLD_SERVICES,
                &elem_id, 1, &cookie, ebufp)) != (pin_flist_t *)NULL) {
                svc_poid = PIN_FLIST_FLD_GET(svc_flistp, PIN_FLD_SERVICE_OBJ,
                                             0, ebufp);

                /*
                 * No need to call SET_LOGIN opcode if service poid doesnt exist
                 */
                if ( PIN_POID_IS_NULL(svc_poid) ) {
                        continue;
                }
		acct_poid = PIN_FLIST_FLD_GET(svc_flistp,
                                              PIN_FLD_ACCOUNT_OBJ,
                                              0, ebufp);
                PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_POID,
                                  acct_poid, ebufp);
                PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_SERVICE_OBJ,
                                  svc_poid, ebufp);
                PIN_FLIST_ELEM_SET(s_flistp, log_flistp,
                                   PIN_FLD_LOGINS, 0 , ebufp);
                /* if END_T is not NULL set it */
                if(vp)  {
                PIN_FLIST_FLD_SET(s_flistp,PIN_FLD_END_T,vp,ebufp);
                }

                PCM_OP(ctxp, PCM_OP_CUST_SET_LOGIN, 0, s_flistp,
                       &r_flistp, ebufp);
                PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
        }
        PIN_FLIST_DESTROY_EX(&log_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&s_flistp, NULL);

}
