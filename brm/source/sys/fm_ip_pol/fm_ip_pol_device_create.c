/*
 * @(#)$Id: fm_ip_pol_device_create.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:06:44 nishahan Exp $
 *
 * Copyright (c) 2004, 2023, Oracle and/or its affiliates. 
 * All rights reserved. 
 *
 * This material is the confidential property of Oracle Corporation or its
 * licensors and may be used, reproduced, stored or transmitted only in
 * accordance with a valid Oracle license or sublicense agreement.
 * sublicense agreement.
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_ip_pol_device_create.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:06:44 nishahan Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <pcm.h>
#include <pinlog.h>
#include "ops/ip.h"
#include "ops/apn.h"
#include "ops/device.h"
#include "pin_ip.h"
#include "pin_apn.h"
#include "cm_fm.h"
#include "pin_errs.h"

#define FILE_LOGNAME "fm_ip_pol_device_create.c"


/*******************************************************************************
 * Functions provided within this source file
 ******************************************************************************/
EXPORT_OP void
op_ip_pol_device_create(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp);


/*******************************************************************************
 * Functions provided outside of this source file
 ******************************************************************************/
static void
fm_ip_pol_device_id_exists(
        pcm_context_t           *ctxp,
	int32                   flags,
        pin_flist_t             *i_flistp,
        pin_errbuf_t            *ebufp);

static void
fm_ip_pol_apn_change_state(
        pcm_context_t   *ctxp,
        pin_flist_t     *i_flistp,
        int32           flags,
        pin_errbuf_t    *ebufp);
/*******************************************************************************
 * Entry routine for the PCM_OP_IP_POL_DEVICE_CREATE opcode
 ******************************************************************************/
void
op_ip_pol_device_create(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*dn_flistp = NULL;            
	pin_flist_t		*tmp_flistp = NULL;            
	char            	*end_dev_ipp = NULL;
	void			*vp = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_IP_POL_DEVICE_CREATE) {

		pin_set_err(ebufp, 
				PIN_ERRLOC_FM,
				PIN_ERRCLASS_SYSTEM_DETERMINATE,
				PIN_ERR_BAD_OPCODE, 
				0, 
				0, 
				opcode);

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"op_ip_pol_device_create opcode error", 
				ebufp);

		*o_flistpp = NULL;
		return;
	}
	
	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"op_ip_pol_device_create input flist", 
				i_flistp);

	/*
	 * Check if the given device_id already exists in this database.
	 * If ebuf is set, then the device_id already exists.
	 */
	fm_ip_pol_device_id_exists(ctxp, 
				flags,
				i_flistp, 
				ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF( PIN_ERR_LEVEL_ERROR,
				"op_ip_pol_device_create error", 
				ebufp);

		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		*o_flistpp = PIN_FLIST_COPY(i_flistp, ebufp);
		tmp_flistp = PIN_FLIST_SUBSTR_GET(*o_flistpp, PIN_FLD_DEVICE_IP, 0, ebufp);
		end_dev_ipp = PIN_FLIST_FLD_GET(tmp_flistp, PIN_FLD_END_ADDRESS, 1, ebufp);
		if (end_dev_ipp != NULL) {
                	PIN_FLIST_FLD_DROP(tmp_flistp, PIN_FLD_END_ADDRESS, ebufp);
		}

		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_ip_pol_device_create output flist", 
			*o_flistpp);
	}

	return;
}


/*******************************************************************************
 * fm_ip_pol_device_id_exists()
 *
 * Inputs: flist with device_id and poid of a /device along with APN object
 * Outputs: void; ebuf set if device_id already exists
 *
 * Description:
 * This function searches the database for an existing object with the
 * device_id and APN object as the input argument.
 * If one or more records are found, the
 * ebuf is set indicating the device_id already exists.
 ******************************************************************************/
static void
fm_ip_pol_device_id_exists(
        pcm_context_t   *ctxp,
	int32		flags,
        pin_flist_t     *i_flistp,
        pin_errbuf_t    *ebufp)
{
        int32           cred = 0;               /* used for scopeing */
        int32           s_flags = 0;            /* for searching */
        int64           database = 0;           /* route the search */
        pin_flist_t     *search_flistp = NULL;  /* search input */
        pin_flist_t     *args_flistp = NULL;    /* search args */
        pin_flist_t     *r_flistp = NULL;       /* search output */
        pin_flist_t     *dip_flistp = NULL;     /* device_ip flist */
        poid_t          *srchpp = NULL;         /* for routing */
        poid_t          *dn_poidp = NULL;       /* for searching */
        char            *template = NULL;       /* search template */
        char            *poid_type = NULL;      /* for searches */
	char		*end_dev_ipp = NULL;
        void            *vp = NULL;             /* for flist gets */
	int32		elemid = 0;		/* for results check */
	static int32	*ip_rangep = NULL;		/* for range search */
	static int32	ip_range = 0;		/* for range search */
	int32		err = 0;		/* for range search error */
	pin_cookie_t    cookie = NULL;		/* for results check */


        if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
	PIN_ERR_CLEAR_ERR(ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_ip_pol_device_id_exists input flist", 
		i_flistp);

        /*
         * Setup search flist for uniqueness check
         */
        search_flistp = PIN_FLIST_CREATE(ebufp);

        vp = PIN_FLIST_FLD_GET(i_flistp, 
			PIN_FLD_POID, 
			0, 
			ebufp);

        database = PIN_POID_GET_DB((poid_t *)vp);
        poid_type = (char *)PIN_POID_GET_TYPE((poid_t *)vp);

        srchpp = PIN_POID_CREATE(database, 
			"/search", 
			-1, 
			ebufp);
        PIN_FLIST_FLD_PUT(search_flistp, 
			PIN_FLD_POID, 
			srchpp, 
			ebufp);

        PIN_FLIST_FLD_SET(search_flistp, 
			PIN_FLD_FLAGS, 
			&s_flags, 
			ebufp);

	dip_flistp = (pin_flist_t *) PIN_FLIST_SUBSTR_GET(i_flistp,
			PIN_FLD_DEVICE_IP,
			0,
			ebufp);

	end_dev_ipp = PIN_FLIST_FLD_GET(dip_flistp, 
			PIN_FLD_END_ADDRESS, 
			1, 
			ebufp);

	if (ip_rangep == NULL) {
		pin_conf("fm_ip", "ip_device_dup_check_type", PIN_FLDT_INT, (caddr_t *)&ip_rangep, &err);

		if (ip_rangep != NULL) {
			ip_range = *ip_rangep;
			(void) free (ip_rangep);
		}
	}

	if (ip_range == 0) {
		
		template = "select X from /device/ip where "
				" ( F1 = V1 and "
				"   F2 = V2 and "
				"   F3 = V3 ) ";
	}
	else if (end_dev_ipp != NULL) {
		template = "select X from /device/ip where "
				" ( F1 = V1 and "
				"   F2 = V2 and "
				"   F3 >= V3 and "
				"   F4 <= V4 ) ";
	}
	else {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG," As the range of ip address is verified "
					"there is no need to verify for individual  ip address");
		goto cleanup;
	}

        PIN_FLIST_FLD_SET(search_flistp, 
			PIN_FLD_TEMPLATE, 
			(void *)template, 
			ebufp);

        /*
         * Add the first argument to the flist - the poid type
         */
        args_flistp = PIN_FLIST_ELEM_ADD(search_flistp,
			PIN_FLD_ARGS, 
			1, 
			ebufp);

        dn_poidp = PIN_POID_CREATE(database, poid_type, -1, ebufp);
        PIN_FLIST_FLD_PUT(args_flistp, 
			PIN_FLD_POID, 
			dn_poidp, 
			ebufp);

        /*
         * Add the second argument to the flist - the APN poid
	 * SUBSTR PIN_FLD_DEVICE_IP
	 * 	STR	PIN_FLD_APN_OBJ
         */
	args_flistp = PIN_FLIST_ELEM_ADD(search_flistp,
			PIN_FLD_ARGS, 
			2, 
			ebufp);

        args_flistp = PIN_FLIST_SUBSTR_ADD(args_flistp, 
			PIN_FLD_DEVICE_IP,
			ebufp);

	vp = PIN_FLIST_FLD_GET(dip_flistp, 
			PIN_FLD_APN_OBJ, 
			0, 
			ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
             		 "fm_ip_pol_device_id_exists():APN_OBJ is Mandatory");
                goto cleanup;
        }

	PIN_FLIST_FLD_SET(args_flistp, 
			PIN_FLD_APN_OBJ, 
			vp, 
			ebufp);
        /*
         * Add the third argument to the flist - the poid type
         */
        args_flistp = PIN_FLIST_ELEM_ADD(search_flistp,
			PIN_FLD_ARGS, 
			3, 
			ebufp);

        vp = PIN_FLIST_FLD_GET(i_flistp, 
			PIN_FLD_DEVICE_ID, 
			0, 
			ebufp);

        PIN_FLIST_FLD_SET(args_flistp, 
			PIN_FLD_DEVICE_ID, 
			vp, 
			ebufp);

	if (end_dev_ipp != NULL && ip_range != 0) {
        /*
         * Add the fourth argument to the flist - the end ip address
         */
        args_flistp = PIN_FLIST_ELEM_ADD(search_flistp,
			PIN_FLD_ARGS, 
			4, 
			ebufp);

        PIN_FLIST_FLD_SET(args_flistp, 
			PIN_FLD_DEVICE_ID, 
			end_dev_ipp, 
			ebufp);
	}

        /*
         * Add the null results array
         */
	PIN_FLIST_ELEM_SET(search_flistp, 
			NULL, 
			PIN_FLD_RESULTS, 
			0, 
			ebufp);

         PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                  "fm_ip_pol_device_id_exists search flist",
                  search_flistp);

        PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, 
			PCM_OPFLG_COUNT_ONLY | flags, 
			search_flistp,
			&r_flistp, 
			ebufp);
        /*
         * If /device with given device_id already exists, set ebuf
         */
	PIN_FLIST_ELEM_GET_NEXT(r_flistp, 
			PIN_FLD_RESULTS, 
			&elemid, 
			0, 
			&cookie, 
			ebufp);

	if (elemid > 0) {

         	pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_DUPLICATE, 
			PIN_FLD_DEVICE_ID, 
			0, 
			0);

        	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
                        "fm_ip_pol_device_id_exists new device_id "
                        "already exists");
        }
	else {
		fm_ip_pol_apn_change_state(ctxp, 
			i_flistp,
			flags,
			ebufp);
	}

cleanup:

        PIN_FLIST_DESTROY_EX(&search_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
        return;
}

/*
 * fm_ip_pol_apn_change_state()
 *
 *      Function to change the state of APN
 *
 *      Input :
 *              ctxp -> context
 *              i_flistp  -> Input flist
 *              apn_poid  -> APN poid
 *              flags     -> opcode flag
 *              state_id  -> Current state of APN
 *              ebufp     -> Error Buffer pointer
 *      Output :
 *              ebufp will be set incase of error
 */

static void
fm_ip_pol_apn_change_state(
        pcm_context_t   *ctxp,
        pin_flist_t     *i_flistp,
        int32           flags,
        pin_errbuf_t    *ebufp)
{

        pin_flist_t     *r_flistp = NULL;       /* flist for read   */
        pin_flist_t     *s_flistp = NULL;       /* flist for set   */
        pin_flist_t     *ro_flistp = NULL;      /* flist for output */
        pin_flist_t     *dip_flistp = NULL;      /* flist for output */
        poid_t          *vp=NULL;
	poid_t		*apn_poidp=NULL;
        int32           new_state=PIN_APN_USABLE_STATE;
        int32           *state_id=NULL;

        if (PIN_ERR_IS_ERR(ebufp)) {
                return;
        }
        PIN_ERR_CLEAR_ERR(ebufp);

        PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
              "fm_ip_pol_apn_change_state()");

        r_flistp = PIN_FLIST_CREATE(ebufp);

	dip_flistp = PIN_FLIST_SUBSTR_GET(i_flistp,
			PIN_FLD_DEVICE_IP,
			0,
			ebufp);
	apn_poidp = (void *)PIN_FLIST_FLD_GET(dip_flistp,
			PIN_FLD_APN_OBJ,
			0,
			ebufp);

        PIN_FLIST_FLD_SET(r_flistp,
                        PIN_FLD_POID,
                        apn_poidp,
                        ebufp);
        PIN_FLIST_FLD_SET(r_flistp,
                        PIN_FLD_STATE_ID,
                        (void *) NULL,
                        ebufp);

        /*
         * We're going to read the service object directly
         */
        PCM_OP(ctxp, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE,
                r_flistp, &ro_flistp, ebufp);

        if ((PIN_ERR_IS_ERR(ebufp)) || ro_flistp == NULL ) {
		 goto fm_ip_pol_apn_change_state_Error;
	}

        state_id = (int32 *)PIN_FLIST_FLD_GET(ro_flistp,
                        PIN_FLD_STATE_ID,
                        0,
                        ebufp);

	if (state_id == NULL) {
		 goto fm_ip_pol_apn_change_state_Error;
	}

        if( *state_id == PIN_APN_UNUSABLE_STATE ) {
                pin_set_err(ebufp, PIN_ERRLOC_FM,
                        PIN_ERRCLASS_APPLICATION,
                        PIN_ERR_INVALID_STATE,
                        PIN_FLD_APN_OBJ,
                        0,
                        0);
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_ip_pol_apn_change_state : invalid "
                        "state id",
                        ebufp);
        }
        if( *state_id != PIN_APN_NEW_STATE ) {
                 goto fm_ip_pol_apn_change_state_Error;
        }

	s_flistp = PIN_FLIST_CREATE(ebufp);

        PIN_FLIST_FLD_SET(s_flistp,
                        PIN_FLD_POID,
                        apn_poidp,
                        ebufp);

        PIN_FLIST_FLD_SET(s_flistp,
                                PIN_FLD_NEW_STATE,
                                &new_state,
                                ebufp);

        vp = PIN_FLIST_FLD_GET(i_flistp,
                                PIN_FLD_PROGRAM_NAME,
                                0,
                                ebufp);
        PIN_FLIST_FLD_SET(s_flistp,
                                PIN_FLD_PROGRAM_NAME,
                                vp,
                                ebufp);

	PIN_FLIST_DESTROY_EX(&ro_flistp, NULL);

        PCM_OP(ctxp, PCM_OP_DEVICE_SET_STATE, flags,
                        s_flistp, &ro_flistp, ebufp);

fm_ip_pol_apn_change_state_Error:
        PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&ro_flistp, NULL);
  return;
}
