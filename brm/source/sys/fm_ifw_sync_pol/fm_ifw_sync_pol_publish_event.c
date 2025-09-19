/*******************************************************************
 *
 *  @(#) %full_filespec: fm_ifw_sync_pol_publish_event.c~17:csrc:1 %
 *
* Copyright (c) 2002, 2023, Oracle and/or its affiliates.
 * This material is the confidential property of Oracle Corporation or
 * its licensors and may be used, reproduced, stored or transmitted only
 * in accordance with a valid Oracle license or sublicense agreement.        
 *
 *******************************************************************/

#ifndef lint
static  char Sccs_Id[] = "@(#)%Portal Version: fm_ifw_sync_pol_publish_event.c:PortalBase7.3.1Int:3:2009-Jan-12 21:46:41 %";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/timeb.h>
#include "pin_sys.h"
#include "pin_os.h"
#include "pcm.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "pin_rate.h"
#include "fm_utils.h"
#include "ops/ifw_sync.h"
#include "ops/subscription.h"

#define FILE_SOURCE_ID         "fm_ifw_sync_pol_publish_event.c(27)"
#define PIN_CYCLE_EVENT_TYPE   "/event/billing/product/fee/cycle/cycle_forward_"
#define PIN_ROLLOVER_EVENT_TYPE   "/event/billing/cycle/rollover/"
#define PIN_LOGIN_EVENT_TYPE   "/event/customer/login"
#define PIN_ADJUSTMENT_EVENT_TYPE   "/event/billing/adjustment/event"
#define PIN_CYCLE_EVENT_LENGTH 47

/*****************************************************************************
 *
 * Flags for apply balance impacts of cycle forward and adjustment events
 *
 * 1 = Apply balance impact to current period (month) only
 * 2 = Apply balance impact to specific periods (multiple months)
 * 4 = Allow impact of resource to result in a negative resource balance 
 *     (Cycle forward fees need this)
 *
 * Flag value of 5 would allow Integrate to apply the balance impact to the 
 * current month only and allow the current resource balance to go negative.
 ****************************************************************************/
#define PIN_FLAGS_APPLY_CURRENT_MONTH_ONLY 1 
#define PIN_FLAGS_APPLY_SPECIFIC_PERIODS   2
#define PIN_FLAGS_ALLOW_NEGATIVE_BALANCE   4

EXPORT_OP void
op_ifw_sync_pol_publish_event(
        cm_nap_connection_t	*connp,
        int32			opcode,
        int32			opflags,
        pin_flist_t		*in_flistp,
        pin_flist_t		**ret_flistpp,
        pin_errbuf_t		*ebufp);

static void
fm_ifw_sync_pol_publish_event(
        cm_nap_connection_t	*connp,
	pin_flist_t		*in_flistp,
        int32			flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp);

static void
process_make_bill(
        cm_nap_connection_t     *connp,
        pin_flist_t             *in_flistp,
        int32                   flags,
        pin_flist_t             **out_flistpp,
        pin_errbuf_t            *ebufp);

static void
process_purchase_action_event(
        cm_nap_connection_t     *connp,
        pin_flist_t             *in_flistp,
        int32                   flags,
        pin_flist_t             **out_flistpp,
        pin_errbuf_t            *ebufp);

static void
process_line_transfer(
        cm_nap_connection_t	*connp,
	pin_flist_t		*in_flistp,
        int32			flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp);

static void
process_discount_set_validity(
        cm_nap_connection_t     *connp,
        pin_flist_t             *in_flistp,
        int32                   flags,
        pin_flist_t             **out_flistpp,
        pin_errbuf_t            *ebufp);

static void
fm_ifw_sync_pol_process_profile_event(
        cm_nap_connection_t	*connp,
        pin_flist_t		*in_flistp,
        int32			flags,
        pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp);

static void
fm_ifw_sync_pol_process_noncurrency_event(
        cm_nap_connection_t	*connp,
        pin_flist_t		*in_flistp,
        int32			flags,
        pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp);

static void
fm_ifw_sync_pol_process_cycle_event(
        cm_nap_connection_t	*connp,
	pin_flist_t		*in_flistp,
        int32			flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp);

static void
fm_ifw_sync_pol_process_common_event(
        cm_nap_connection_t	*connp,
	char                    *event_type,
	pin_flist_t		*in_flistp,
        int32			flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp);

static void 
fm_ifw_sync_pol_push_to_eai(
        cm_nap_connection_t	*connp,
	pin_flist_t		*in_flistp,
        int32			flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp);

static void
fm_ifw_sync_pol_prep_logins(
	pin_flist_t		*in_flistp,
	pin_errbuf_t		*ebufp);

static void 
fm_ifw_sync_pol_prep_flags(
	pin_flist_t		*in_flistp,
	int                     cycle_forward_event,
	const char              *event_type,
	pin_errbuf_t		*ebufp);


/*******************************************************************
 * Main routine for the PCM_OP_IFW_SYNC_POL_PUBLISH_EVENT  command
 *******************************************************************/
void
op_ifw_sync_pol_publish_event(
        cm_nap_connection_t	*connp,
        int32			opcode,
        int32			opflags,
        pin_flist_t		*in_flistp,
        pin_flist_t		**ret_flistpp,
        pin_errbuf_t		*ebufp)
{
	pin_flist_t		*r_flistp = NULL;

	/***********************************************************
	 * Null out results until we have some.
	 ***********************************************************/
	*ret_flistpp = NULL;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_IFW_SYNC_POL_PUBLISH_EVENT) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_ifw_sync_pol_publish_event", ebufp);
		return;
	}

	/***********************************************************
	 * We will not open any transactions with Policy FM
	 * since policies should NEVER modify the database.
	 ***********************************************************/

	/***********************************************************
	 * Log input flist
	 ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_ifw_sync_pol_publish_event input flist", in_flistp);

	/***********************************************************
	 * Call main function to do it
	 ***********************************************************/
	fm_ifw_sync_pol_publish_event(connp, in_flistp, opflags, 
			&r_flistp, ebufp);

	/***********************************************************
	 * Results.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		*ret_flistpp = (pin_flist_t *)NULL;
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_ifw_sync_pol_publish_event error", ebufp);
	} else {
		*ret_flistpp = r_flistp;
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_ifw_sync_pol_publish_event output flist", r_flistp);
	}

	return;
}

/*******************************************************************
 * fm_ifw_sync_pol_publish_event()
 *
 *	Add fields to the event originally created by event notification.
 *
 *******************************************************************/
static void
fm_ifw_sync_pol_publish_event(
        cm_nap_connection_t	*connp,
	pin_flist_t		*in_flistp,
	int32                   flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp)
{
	poid_t			*poidp = NULL;
	pin_flist_t		*flistp = NULL;
	pin_flist_t             *tflistp = NULL;
	char			event_type[1024] = "";
	char			tmpstr[1152] = "";
	int			cycle_forward_event = 0;
	int32			flag = PIN_BOOLEAN_FALSE;
	void			*vp = NULL;
	char			**strp = NULL;
	char			*common_event[] = 
			{ "/event/billing/adjustment/event",
			"/event/billing/debit",
			"/event/customer/login",
			"/event/notification/service_item/make_bill",
			"/event/notification/billing/complete",
			NULL };
	pin_cookie_t            cookie = NULL;
        int32                   rec_id = 0;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

        /***********************************************************
         * Log the input flist 
         ***********************************************************/
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
		"fm_ifw_sync_pol_publish_event input flist", in_flistp);

	pin_snprintf(tmpstr, sizeof(tmpstr), "opflags = %d", flags);
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, tmpstr);
	poidp = (poid_t *)PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID, 0, ebufp);
	pin_snprintf(event_type, sizeof(event_type), "%s", PIN_POID_GET_TYPE(poidp)); 
	pin_snprintf(tmpstr, sizeof(tmpstr), "event_type = %s",event_type);
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, tmpstr);

	/******************************************************************
	 * Skip Adjustment event only if it is a part of rerating called
	 * from a rerating job.
	 ******************************************************************/
	if (strcmp(event_type, PIN_ADJUSTMENT_EVENT_TYPE) == 0) {
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, "It is an adjustment event.");
		if ((flags & PIN_RATE_FLG_RERATE) == PIN_RATE_FLG_RERATE) {
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, "It is an adjustment event generated from rerating.");
			if (fm_utils_op_is_ancestor(connp->coip,
				PCM_OP_SUBSCRIPTION_CALC_BEST_PRICING) &&
			    (flags & PCM_OPFLG_CALC_ONLY) != PCM_OPFLG_CALC_ONLY) {
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, "Publish rerating adjustment event.");
			} else {
				PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, "Skip rerating adjustment event.");
				return;
			}
		}
	}
	
	/******************************************************************
	 * Process Add/Modify Balance Group event.
	 ******************************************************************/
	if ((!strcmp(event_type,
			"/event/notification/bal_grp/create")) ||
		(!strcmp(event_type,
			"/event/notification/bal_grp/modify"))) {

		flistp = PIN_FLIST_ELEM_GET(in_flistp, PIN_FLD_BAL_INFO,
				PIN_ELEMID_ANY, 0, ebufp);
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_BAL_GRP_OBJ, 0, ebufp);
		if (vp == NULL)
		  return;

                if (!strcmp(PIN_POID_GET_TYPE(vp),"/balance_group/monitor")){
                  return;
                }
		PIN_FLIST_FLD_SET(in_flistp, PIN_FLD_BAL_GRP_OBJ, vp, ebufp);
		flag = PIN_BOOLEAN_FALSE;
		if (!strcmp(event_type,
                        "/event/notification/bal_grp/modify")) {
			flag = PIN_BOOLEAN_TRUE;
		}
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_BILLINFO_OBJ, flag, ebufp);
		if ( vp != NULL ) {
		    PIN_FLIST_FLD_SET(in_flistp, PIN_FLD_BILLINFO_OBJ, vp, ebufp);
		}
		vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_OBJECT_CACHE_TYPE, flag, ebufp);
		if ( vp != NULL ) {
			PIN_FLIST_FLD_SET(in_flistp, PIN_FLD_OBJECT_CACHE_TYPE, vp, ebufp);
		}
                while ((tflistp = PIN_FLIST_ELEM_GET_NEXT(flistp,
                        PIN_FLD_BALANCES, &rec_id, 1, &cookie, ebufp)) != NULL) {

                        PIN_FLIST_ELEM_SET(in_flistp, tflistp, PIN_FLD_BALANCES, rec_id, ebufp);
                }
	}
	
	/******************************************************************
	 * Process  MakeBill event.
	 ******************************************************************/
	if (!strcmp(event_type,
			"/event/notification/service_item/make_bill")) {
		process_make_bill(connp, in_flistp, flags, out_flistpp, ebufp);
	}

	/******************************************************************
         * Process  Purchase Action  event.
         ******************************************************************/
	if (!strcmp(event_type,
                        "/event/billing/product/action/purchase")) {
                process_purchase_action_event(connp, in_flistp, flags, out_flistpp, ebufp);
		goto Done;
        }

	/******************************************************************
	 * Process Add/Modify Ordered Balance Group event.
	 ******************************************************************/
	if ((!strcmp(event_type,
			"/event/billing/ordered_bal_grp/create")) ||
		(!strcmp(event_type,
			"/event/billing/ordered_bal_grp/modify"))) {

		vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_SERVICE_OBJ, 
				0, ebufp);
		PIN_FLIST_FLD_SET(in_flistp, PIN_FLD_SERVICE_OBJ, vp, ebufp);
	}
	
	/******************************************************************
	 * Process line transfer audit events and service transfer notification events
	 ******************************************************************/
	if (!strcmp(event_type, "/event/audit/subscription/transfer") ||
		!strcmp(event_type, "/event/notification/service_balgrp_transfer/data")) {
			process_line_transfer(connp, in_flistp, flags, 
				out_flistpp, ebufp);
	}

	/******************************************************************
	 * Process Discount SetValidity events.
	 ******************************************************************/
	if (!strcmp(event_type, "/event/billing/discount/action/set_validity")) {
		process_discount_set_validity(connp, in_flistp, flags, out_flistpp, ebufp);
	}

	cycle_forward_event = !strncmp(event_type, PIN_CYCLE_EVENT_TYPE,
		PIN_CYCLE_EVENT_LENGTH);

	pin_snprintf(tmpstr, sizeof(tmpstr), "\tcycle_forward_event = %d", cycle_forward_event);
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, tmpstr);

	/******************************************************************
	 * Second check:
	 *
	 * Do we have a common event?
	 ******************************************************************/
	strp = common_event;
	if (!cycle_forward_event)
	{
		for (strp = common_event; 
		      ((*strp != (char *)NULL) && (strcmp(*strp, event_type)));
			strp++);
	}

	/*******************************************************************
	 * Check if there is a PIN_FLD_BAL_IMPACTS array element.  If there
	 * is, add PIN_FLD_FLAGS before any further processing.
	 ******************************************************************/ 

	fm_ifw_sync_pol_prep_flags(in_flistp, cycle_forward_event, 
			event_type, ebufp);

	/******************************************************************
         *  Identify profile events and call fm_ifw_sync_pol_process_profile_event()
         *  This function publishes only "/profile/acct_extrating"
         *    and "/profile/serv_extrating" events.
         *****************************************************************/
        if ( (!strcmp(event_type, "/event/notification/profile/create") ) ||
                      (!strcmp(event_type, "/event/notification/profile/modify") ) ||
                      (!strcmp(event_type, "/event/notification/profile/delete") ) )
        {
                fm_ifw_sync_pol_process_profile_event(connp, in_flistp, flags, out_flistpp, ebufp);
                goto Done;
        }

        /******************************************************************
         *  Identify adjustment events and call fm_ifw_sync_pol_process_noncurrency_event()
         *  This function filters currency resource adjustments from being
         *  published.
         *****************************************************************/
        if( (!strcmp(event_type, "/event/billing/debit") ) ||
		(!strcmp(event_type, "/event/billing/product/fee/purchase") ) ||			
		(strstr(event_type, "/event/billing/product/fee/cycle/cycle_forward") ) ||			
		(!strcmp(event_type, "/event/billing/adjustment/event")) )
        {
                fm_ifw_sync_pol_process_noncurrency_event(connp, in_flistp, flags, out_flistpp, ebufp);
                goto Done;
        }

	/*****************************************************************
	 * Now process the different event types
	 *****************************************************************/
	if (cycle_forward_event) {

		fm_ifw_sync_pol_process_cycle_event(connp, in_flistp, flags, 
			out_flistpp, ebufp);

	} else if ((*strp != (char *)NULL)) {

		/* strp must equal event_type */
                fm_ifw_sync_pol_process_common_event(connp, event_type, 
			in_flistp, flags, out_flistpp, ebufp);

	} else {
		/******************************************************
	 	 * Push event to EAI. This will push all events 
		 * generated by event notification even if wrongly 
		 * configured to come through IFW. i.e, events that 
		 * could have gone directly to EAI from event 
		 * notification without the overhead of IFW.
	 	 ******************************************************/
		fm_ifw_sync_pol_push_to_eai(connp, in_flistp, flags,
			out_flistpp, ebufp);
	}

Done:
        /***********************************************************
         * Log the return flist 
         ***********************************************************/
        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
        	"fm_ifw_sync_pol_publish_event return flist", *out_flistpp);

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ifw_sync_pol_publish_event error", ebufp);
	}

	return;
}

static void
process_make_bill(
        cm_nap_connection_t	*connp,
	pin_flist_t		*in_flistp,
	int32                   flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp)
{
	pin_flist_t 		*fp1 = NULL;
	pin_flist_t 		*fp2 = NULL;
	int32 			*ts = NULL;

        fp1=PIN_FLIST_ELEM_ADD(in_flistp,PIN_FLD_ACTGINFO,1,ebufp);
	ts = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_ITEM_POID_LIST,0 , ebufp);
	PIN_FLIST_FLD_SET(fp1, PIN_FLD_ITEM_POID_LIST, ts, ebufp);
	ts = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_NEXT_ITEM_POID_LIST, 0, ebufp);
	PIN_FLIST_FLD_SET(fp1, PIN_FLD_NEXT_ITEM_POID_LIST, ts, ebufp);

}

static void fm_ifw_sync_pol_process_cycle_event( 
        cm_nap_connection_t	*connp,
	pin_flist_t		*in_flistp,
	int32                   flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t 		*ctxp = connp->dm_ctx;
	int			purchase_product_chain = 0;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ifw_sync_pol_process_cycle_event in_flistp", in_flistp);

	/**************************************************************
	 * push all the cycle forward events to EAI.
	 **************************************************************/
	fm_ifw_sync_pol_push_to_eai(connp, in_flistp, flags,
			    out_flistpp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp))
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
			"fm_ifw_sync_pol_process_cycle_event error", ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
		"fm_ifw_sync_pol_process_cycle_event return flist", 
		*out_flistpp);
			
	return;
}

static void fm_ifw_sync_pol_process_common_event(
        cm_nap_connection_t	*connp,
	char                    *event_type,
	pin_flist_t		*in_flistp,
	int32                   flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp)
{

	pcm_context_t *ctxp = connp->dm_ctx;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ifw_sync_pol_process_common_event input flist", in_flistp);

	if (strcmp(event_type, PIN_LOGIN_EVENT_TYPE) == 0)
	{

        	/* Add logins info */
		fm_ifw_sync_pol_prep_logins(in_flistp, ebufp);

		if (PIN_ERR_IS_ERR(ebufp))
		{
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_ifw_sync_pol_process_common_event error",
				ebufp);
			return;
		}
	}

	/**********************************************************************
	 * Push event to EAI
	 **********************************************************************/
	fm_ifw_sync_pol_push_to_eai(connp, in_flistp, flags, out_flistpp, 
				    ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
			"fm_ifw_sync_pol_process_common_event error", ebufp);
	}

	return;

}

static void fm_ifw_sync_pol_push_to_eai(
        cm_nap_connection_t	*connp,
	pin_flist_t		*in_flistp,
	int32                   flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t 		*ctxp = connp->dm_ctx;
	poid_t			*svc_pdp = NULL;
	pin_cookie_t    	cookie = NULL;
	int32			rec_id = 0;
	pin_flist_t		*flistp = NULL;
        
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ifw_sync_pol_push_to_eai in_flistp", in_flistp);

	/**********************************************************************
	 * Added for service level balances feature.
	 **********************************************************************/
	svc_pdp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_SERVICE_OBJ, 1, ebufp);
	while ((flistp = PIN_FLIST_ELEM_GET_NEXT(in_flistp,
		PIN_FLD_BAL_IMPACTS, &rec_id, 1, &cookie, ebufp)) != NULL) {

		PIN_FLIST_FLD_SET(flistp, PIN_FLD_SERVICE_OBJ, svc_pdp, ebufp);
	}

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ifw_sync_pol_push_to_eai in_flistp after SLB change", 
			in_flistp);

	/**********************************************************************
	 * Push event to EAI
	 **********************************************************************/
	PCM_OP(ctxp, PCM_OP_PUBLISH_GEN_PAYLOAD, flags, in_flistp, 
		out_flistpp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {

		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				 "fm_ifw_sync_pol_push_to_eai error", ebufp);

	}

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
		"fm_ifw_sync_pol_push_to_eai return flist", *out_flistpp);

	return;

}

static void fm_ifw_sync_pol_prep_logins(
	pin_flist_t  *in_flistp,
	pin_errbuf_t *ebufp)
{
	pin_flist_t  *flistp = NULL;
	void	     *vp     = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ifw_sync_pol_prep_logins input flist", in_flistp);

	/**********************************************************************
	 * Get PIN_FLD_LOGINS element # 1 (the new login)
	 * Add PIN_FLD_SERVICE_OBJ poid from the input flist to the 
	 * PIN_FLD_LOGINS flist
         * Also add PIN_FLD_EFFECTIVE_T
	 *********************************************************************/
	flistp = PIN_FLIST_ELEM_GET(in_flistp, PIN_FLD_LOGINS, 1, 0, ebufp);

	vp = PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_SERVICE_OBJ, 0, ebufp);

	PIN_FLIST_FLD_SET(flistp, PIN_FLD_SERVICE_OBJ, vp, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
			  "fm_ifw_sync_pol_prep_logins return flist", 
			  in_flistp);

	return;

}

static void fm_ifw_sync_pol_prep_flags(
	pin_flist_t	*in_flistp,
	int             cycle_forward_event,
	const char      *event_type,
	pin_errbuf_t    *ebufp)
{

	pin_flist_t	*flistp		= NULL;
	void		*vp 		= NULL;
	int32		elem_id 	= 0;
	int             flags		= 0;
	char            tmpstr[1024];

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ifw_sync_pol_prep_flags input flist", in_flistp);

	/**********************************************************************
	 * 
	 * Set up default flag values.  
	 * For all event types, set flag PIN_FLAGS_APPLY_CURRENT_MONTH_ONLY.
	 * If it's a cycle forward event, add PIN_FLAGS_ALLOW_NEGATIVE_BALANCE.
	 *
	 * PIN_FLAGS_APPLY_SPECIFIC_PERIODS will come later
	 *
	 ********************************************************************/
	flags = PIN_FLAGS_APPLY_CURRENT_MONTH_ONLY;
	if (cycle_forward_event)
		flags = flags + PIN_FLAGS_ALLOW_NEGATIVE_BALANCE;

	pin_snprintf(tmpstr, sizeof(tmpstr), "PIN_FLD_FLAGS = %d", flags);
	PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, tmpstr);

	flistp = PIN_FLIST_ELEM_GET(in_flistp, PIN_FLD_BAL_IMPACTS, 
				    elem_id, 1, ebufp);

	while (flistp != (pin_flist_t *)NULL) {

		elem_id++;

		PIN_FLIST_FLD_SET(flistp, PIN_FLD_FLAGS, &flags, ebufp);

		PIN_FLIST_FLD_SET(flistp, PIN_FLD_EVENT_NAME, (void *)event_type, ebufp);
                if (PIN_ERR_IS_ERR(ebufp)) {
                    PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                         "Error setting PIN_FLD_EVENT_NAME", ebufp);
                } 


		flistp = PIN_FLIST_ELEM_GET(in_flistp, PIN_FLD_BAL_IMPACTS, 
					    elem_id, 1, ebufp);

	}

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_ifw_sync_pol_prep_flags return flist", in_flistp);

	return;

}

static void
process_line_transfer(
        cm_nap_connection_t	*connp,
	pin_flist_t		*in_flistp,
        int32			flags,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp)
{
	pin_flist_t 		*svc_flistp = NULL;
	pin_flist_t		*si_flistp = NULL;
	pin_flist_t		*so_flistp = NULL;
	pin_flist_t 		*flistp = NULL;
	void			*vp = NULL;
	pin_cookie_t    	cookie = NULL;
	int32			rec_id = 0;
	pin_cookie_t    	cookie2 = NULL;
	int32			rec_id2 = 0;
	

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	si_flistp = PIN_FLIST_CREATE(ebufp);
	PIN_FLIST_FLD_SET(si_flistp, PIN_FLD_LOGIN, NULL, ebufp);
	PIN_FLIST_ELEM_SET(si_flistp, NULL, PIN_FLD_ALIAS_LIST, 
			PIN_ELEMID_ANY, ebufp);
	/* Walk the services array */
	while ((svc_flistp = PIN_FLIST_ELEM_GET_NEXT(in_flistp,
		PIN_FLD_SERVICES, &rec_id, 1, &cookie, ebufp)) != NULL) {

		vp = PIN_FLIST_FLD_GET(svc_flistp, PIN_FLD_SERVICE_OBJ, 
				0, ebufp);
		PIN_FLIST_FLD_SET(si_flistp, PIN_FLD_POID, vp, ebufp);
		PCM_OP(connp->dm_ctx, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE, 
				si_flistp, &so_flistp, ebufp);
		
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
			"READ SERVICE return flist", so_flistp);
		
		/* Add LOGIN and ALIAS_LIST to the original flist */
		vp = PIN_FLIST_FLD_TAKE(so_flistp, PIN_FLD_LOGIN, 0, ebufp);
		PIN_FLIST_FLD_PUT(svc_flistp, PIN_FLD_LOGIN, vp, ebufp);
		rec_id2 = 0;
		cookie2 = NULL;
		while ((flistp = PIN_FLIST_ELEM_TAKE_NEXT(so_flistp,
				PIN_FLD_ALIAS_LIST, &rec_id2, 1, &cookie2, 
				ebufp)) != NULL) {
			PIN_FLIST_ELEM_PUT(svc_flistp, flistp, 
				PIN_FLD_ALIAS_LIST, rec_id2, ebufp);
		}
		PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
	}
	PIN_FLIST_DESTROY_EX(&si_flistp, NULL);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "New input flist", in_flistp);
}

static void
process_discount_set_validity(
        cm_nap_connection_t     *connp,
        pin_flist_t             *in_flistp,
        int32                   flags,
        pin_flist_t             **out_flistpp,
        pin_errbuf_t            *ebufp)
{
        pin_flist_t             *si_flistp = NULL;
        pin_flist_t             *so_flistp = NULL;
        pin_flist_t             *disc_flistp = NULL;

        if (PIN_ERR_IS_ERR(ebufp))
                return;
        PIN_ERR_CLEAR_ERR(ebufp);

        si_flistp = PIN_FLIST_CREATE(ebufp);
        disc_flistp = PIN_FLIST_SUBSTR_GET(in_flistp, PIN_FLD_AC_DISCOUNT, 0, ebufp);
        PIN_FLIST_FLD_COPY(disc_flistp, PIN_FLD_OFFERING_OBJ, si_flistp, PIN_FLD_POID, ebufp);
        PIN_FLIST_FLD_SET(si_flistp, PIN_FLD_ACCOUNT_OBJ, NULL, ebufp);
        
        PCM_OP(connp->dm_ctx, PCM_OP_READ_FLDS, PCM_OPFLG_CACHEABLE,
                                si_flistp, &so_flistp, ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "READ account return flist", so_flistp);

        /* Take the ACCOUNT OBJ of the purchased object and add as OWNER OBJ to the original flist */
        PIN_FLIST_FLD_MOVE(so_flistp, PIN_FLD_ACCOUNT_OBJ, in_flistp, PIN_FLD_OWNER_OBJ, ebufp);

        PIN_FLIST_DESTROY_EX(&so_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&si_flistp, NULL);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, "New input flist", in_flistp);

}

static void fm_ifw_sync_pol_process_profile_event(
        cm_nap_connection_t     *connp,
        pin_flist_t             *in_flistp,
        int32                   flags,
        pin_flist_t             **out_flistpp,
        pin_errbuf_t            *ebufp)
{

        poid_t        *profile_poidp   = NULL;
        pcm_context_t *ctxp = connp->dm_ctx;

        if (PIN_ERR_IS_ERR(ebufp))
                return;
        PIN_ERR_CLEAR_ERR(ebufp);

        profile_poidp = (poid_t *)PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_PROFILE_OBJ, 0, ebufp);

        if (PIN_ERR_IS_ERR(ebufp)){
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_ifw_sync_pol_process_profile_event error", ebufp);
                return;
        }

        if ( (!(strcmp(PIN_POID_GET_TYPE(profile_poidp), "/profile/acct_extrating")) ||
             (!(strcmp(PIN_POID_GET_TYPE(profile_poidp), "/profile/serv_extrating")))) )
	{
		/**********************************************************************
		 * Push event to EAI
		 **********************************************************************/
		fm_ifw_sync_pol_push_to_eai(connp, in_flistp, flags, out_flistpp,
										ebufp);
        }
        else
        {
                /* Just copy the input to the output flist */
                *out_flistpp = PIN_FLIST_COPY(in_flistp, ebufp);
        }

        if (PIN_ERR_IS_ERR(ebufp)) {
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_ifw_sync_pol_process_profile_event error", ebufp);
        }

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_ifw_sync_pol_process_profile_event return flist", *out_flistpp);

        return;
}

static void fm_ifw_sync_pol_process_noncurrency_event(
        cm_nap_connection_t     *connp,
        pin_flist_t             *in_flistp,
        int32                   flags,
        pin_flist_t             **out_flistpp,
        pin_errbuf_t            *ebufp)
{

        pcm_context_t *ctxp = connp->dm_ctx;
        pin_cookie_t  cookie = NULL;
        int32         recid = 0;
        int32         resource_id = 0;
        pin_flist_t   *flistp = NULL;
        void          *vp = NULL;

        if (PIN_ERR_IS_ERR(ebufp))
                return;
        PIN_ERR_CLEAR_ERR(ebufp);

        if (PIN_ERR_IS_ERR(ebufp)){
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "fm_ifw_sync_pol_process_noncurrency_event error", ebufp);
                return;
        }

        /********************************************************************
         * Check for Resource Id > 10000. Otherwise it doesn't need to publish.
         * Loop through the arrays to find any noncurrency resource.
         *******************************************************************/
        while((flistp = PIN_FLIST_ELEM_GET_NEXT(in_flistp, PIN_FLD_SUB_BAL_IMPACTS, &recid, 
			1, &cookie, ebufp)) != (pin_flist_t *)NULL) {

                vp = PIN_FLIST_FLD_GET(flistp, PIN_FLD_RESOURCE_ID,
                                                                0, ebufp);
                if ( vp && ((*(u_int32*)vp) > 10000) ) {
			/**********************************************************************
			 * Push event to EAI
			 **********************************************************************/
			fm_ifw_sync_pol_push_to_eai(connp, in_flistp, flags, out_flistpp,
											ebufp);
			break;
		}
	}
	if(*out_flistpp == (pin_flist_t *)NULL) {
                /* Just copy the input to the output flist */
                *out_flistpp = PIN_FLIST_COPY(in_flistp, ebufp);
        }
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_ifw_sync_pol_process_noncurrency_event error", ebufp);
        }

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_ifw_sync_pol_process_noncurrency_event return flist", *out_flistpp);

}
static void 
process_purchase_action_event
(
        cm_nap_connection_t     *connp,
        pin_flist_t             *in_flistp,
        int32                   flags,
        pin_flist_t             **out_flistpp,
        pin_errbuf_t            *ebufp)
{
        pcm_context_t           *ctxp = connp->dm_ctx;
	pin_flist_t		*prod_flistp = NULL;
	poid_t			*off_pdp = NULL;

        if (PIN_ERR_IS_ERR(ebufp))
                return;
        PIN_ERR_CLEAR_ERR(ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "process_purchase_action_event in_flistp", in_flistp);

	prod_flistp = (pin_flist_t *) PIN_FLIST_SUBSTR_GET(in_flistp,
				PIN_FLD_PRODUCT, 0, ebufp);
	off_pdp = (poid_t *)PIN_FLIST_FLD_GET(prod_flistp, PIN_FLD_OFFERING_OBJ , 1, ebufp);

	/*In case of Item product, this will be null and there is no need to send
	 * the action event to usage platform like Pipeline or ECE*/
	if (!PIN_POID_IS_NULL(off_pdp))
	{
        	/**************************************************************
         	 * push event to EAI.
         	**************************************************************/
        	fm_ifw_sync_pol_push_to_eai(connp, in_flistp, flags,
                            	out_flistpp, ebufp);
	}

        if (PIN_ERR_IS_ERR(ebufp))
                PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
                        "process_purchase_action_event error", ebufp);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "process_purchase_action_event return flist",
                *out_flistpp);

        return;
}
