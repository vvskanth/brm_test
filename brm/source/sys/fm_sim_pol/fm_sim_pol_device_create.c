/*
 * @(#)%Portal Version: fm_sim_pol_device_create.c:WirelessVelocityInt:2:2006-Sep-14 11:24:51 %
 *
 * Copyright (c) 2001 - 2023, Oracle and/or its affiliates.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_sim_pol_device_create.c:WirelessVelocityInt:2:2006-Sep-14 11:24:51 %";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>

#include "cm_fm.h"
#include "ops/sim.h"
#include "pin_errs.h"
#include "pin_sim.h"

#define FILE_LOGNAME "fm_sim_pol_device_create.c(9)"

EXPORT_OP void
op_sim_pol_device_create(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**ret_flistpp,
	pin_errbuf_t		*ebufp);


/**********************************************************************
 * Routines contained within
 **********************************************************************/
static void
fm_sim_pol_device_create(
	pcm_context_t		*ctxp,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistp,
	pin_errbuf_t		*ebufp);

static void
fm_sim_pol_device_exists(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	char		*buf,
	pin_errbuf_t	*ebufp);

/**********************************************************************
 * Main routine for the PCM_OP_SIM_POL_DEVICE_CREATE operation.
 **********************************************************************/
void
op_sim_pol_device_create(
	cm_nap_connection_t	*connp,
	int32			opcode,
	int32			flags,
	pin_flist_t		*i_flp,
	pin_flist_t		**o_flpp,
	pin_errbuf_t		*ebufp) {

	
	pcm_context_t	*ctxp = connp->dm_ctx;
	pin_flist_t	*r_flp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/*********/
	}

	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_SIM_POL_DEVICE_CREATE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_sim_pol_device_create", ebufp);
		return;
	}

	/*
	 * DEBUG: What did we get?
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_sim_pol_device_create input flist", i_flp);

	/*
	 * do the real work
	 */
	fm_sim_pol_device_create(ctxp, i_flp, &r_flp, ebufp);

	/*
	 * Results.
	 */
	if (PIN_ERR_IS_ERR(ebufp)) {
		*o_flpp = (pin_flist_t *)NULL;
		PIN_FLIST_DESTROY_EX(&r_flp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_sim_pol_device_create error", ebufp);
	}else {
		*o_flpp = r_flp;
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_sim_pol_device_create return flist", r_flp);
	}
	
	return;

}

/********************************************************************
 * fm_sim_pol_device_create():
 *
 *	The validation specific to sim devices goes here.
 *	This opcode ensures that the IMSI, SIM, KI and Network 
 *	elements are valid before creating the device. The only 
 *	mandatory fields to create a SIM device are the IMSI and SIM. 
 * 	Set an error if the device id which is the SIM, does not exist.
 *	Special characters put into these fields are removed.
 * input:
 *	i_flistp: the input flist passed in by the opcode
 * output:
 *	o_flistpp: the output flist which contains the formatted flds
 *		ebuf is set if there is an error
 *
 ********************************************************************/
static void
fm_sim_pol_device_create(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	pin_flist_t	**o_flistpp,
	pin_errbuf_t	*ebufp) {


	pin_flist_t	*sub_flistp = NULL; 	/* flist for formatting	*/
	pin_flist_t	*flistp = NULL;		/* a copy  input flist	*/
	
	char		*imsi = NULL;		/* IMSI of  sim device 	*/
	char		*sim = NULL;		/* the SIM of  device 	*/
	char		*ki = NULL;		/* the KI of  device 	*/
	char		*net_elem = NULL;	/* network element  	*/
	
	void		*vp = NULL;		/* for  optional flds 	*/
	int		i = 0;			/* length of string 	*/

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
		/********/
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * By default, we simply copy the input to the output..
	 */
	
	flistp = PIN_FLIST_COPY(i_flistp, ebufp);

	/*
	 * retreive the flds the we need to validate 
	 */

	sim = (char *)PIN_FLIST_FLD_TAKE(flistp, PIN_FLD_DEVICE_ID,
					0, ebufp);
	if (sim != NULL){
		/* 
	 	 * format the sim 
	 	 */
		for (i = strlen(sim) - 1; i >= 0; i--){
	
			if (sim[i] != ' ' && sim[i] != '\t' && sim[i] != 'F' &&
				sim[i] != 'f' && sim[i] != 'Z' && sim[i] != 'z') {
				/* break out of the loop */
				break;
			}
		}	
		sim[i+1] = '\0';
		PIN_FLIST_FLD_PUT(flistp, PIN_FLD_DEVICE_ID, sim, ebufp);
	}
	sim = (char *)PIN_FLIST_FLD_GET(flistp, PIN_FLD_DEVICE_ID, 0, ebufp);

	/* 
	 * search the db to make sure a device with this sim does not exist
	 */
	fm_sim_pol_device_exists(ctxp, i_flistp, sim, ebufp);



	/*
	 * now format the IMSI
	 */

	sub_flistp = PIN_FLIST_SUBSTR_GET(flistp, PIN_FLD_DEVICE_SIM, 1, ebufp);
	if( sub_flistp != NULL) {
		
		vp = PIN_FLIST_FLD_TAKE(sub_flistp, PIN_FLD_IMSI, 0, ebufp);
		if( vp != (void *)NULL) {

			imsi = (char *)vp;
			/* 
			 * validate the imsi
			 */
			for (i = strlen(imsi) - 1; i >= 0; i--){
				if (imsi[i] != ' ' && imsi[i] != '\t' && 
					imsi[i] != 'F' && imsi[i] != 'f') {
					/* break out of the loop */
					break;
				}
			}
			imsi[i+1] = '\0';
			PIN_FLIST_FLD_PUT(sub_flistp, PIN_FLD_IMSI, imsi, ebufp);
		}

		vp = PIN_FLIST_FLD_TAKE(sub_flistp, PIN_FLD_KI, 1, ebufp);
		if( vp != (void *)NULL) {
			ki = (char *)vp;
			/* 
			 * Remove trailing whitespaces from Ki
			 */

			for (i = strlen(ki) - 1; i >= 0; i--){
				if (ki[i] != ' ' && ki[i] != '\t') {
					break;
				}
			}
			ki[i+1] = '\0';
			PIN_FLIST_FLD_PUT(sub_flistp, PIN_FLD_KI, ki, ebufp);

		}

		vp = PIN_FLIST_FLD_TAKE(sub_flistp, PIN_FLD_NETWORK_ELEMENT,
					 1, ebufp);
		if( vp != (void *)NULL) {
			net_elem = (char *)vp;
			/* 
			 * Remove trailing whitespace from network element
			 */
			
			for (i = strlen(net_elem) - 1; i >= 0; i--){
				if (net_elem[i] != ' ' && net_elem[i] != '\t' ) {
					break;
				}
			}
			net_elem[i+1] = '\0';
			PIN_FLIST_FLD_PUT(sub_flistp, PIN_FLD_NETWORK_ELEMENT, 
				net_elem, ebufp);

		}
	}	
	*o_flistpp = flistp;
	
	return;
}

/***************************************************************************
 * fm_sim_pol_device_exists()
 *
 * This method will check the database to see if there is any sim device
 * that already exists in the db with the given SIM/device id. If there is
 * then the ebuf is set.
 *
 * input: i_flistp: the input flist sent in through the opcode call
 * 	 buf: the formatted imsi
 * output: ebuf is set if a sim device with the device id already exists
 ****************************************************************************/
static void
fm_sim_pol_device_exists(
	pcm_context_t	*ctxp,
	pin_flist_t	*i_flistp,
	char		*buf,
	pin_errbuf_t	*ebufp)
{

	pin_flist_t	*s_flistp = NULL; 	/* search flist		*/
	pin_flist_t	*sr_flistp = NULL;	/* return flistp search	*/
	pin_flist_t	*sub_flistp = NULL;	/* flist for substruct	*/

	poid_t		*pdp = NULL;		/* routing poid		*/
	poid_t		*dev_pdp = NULL;	/*  type only poid	*/
	poid_t		*spoidp = NULL;		/* search poid		*/
	int32		flag = SRCH_DISTINCT;	/* search flags		*/
	int64		dbno = 0;		/* database no		*/
	int32		cred = 0;		/* scoping credentials	*/


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	s_flistp = PIN_FLIST_CREATE(ebufp);

	pdp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	dbno = PIN_POID_GET_DB(pdp);

	spoidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

	/*
	 * create  a type only poid to narrow the search
	 */
	dev_pdp = PIN_POID_CREATE(dbno, PIN_OBJ_TYPE_DEVICE_SIM, -1, ebufp);	

	PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, spoidp, ebufp);
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);

	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_TEMPLATE, 
		(void *)"select X from /device/sim where  ( F1 = V1 and  "
		"F2 = V2 ) ", ebufp);
	sub_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);
	
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_DEVICE_ID, buf, ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 2, ebufp);

	PIN_FLIST_FLD_PUT(sub_flistp, PIN_FLD_POID, dev_pdp, ebufp);
	
	sub_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_RESULTS, 0, ebufp);
	
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_POID, 0, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
				"fm_sim_pol_dev srch input flist", s_flistp);

	/*
	 * turn off scoping just before the search and turn it on right after
	 */
	cred = CM_FM_BEGIN_OVERRIDE_SCOPE(CM_CRED_SCOPE_OVERRIDE_ROOT);

	PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, 0, s_flistp, &sr_flistp, ebufp);

	/* turn the scoping back on */
	CM_FM_END_OVERRIDE_SCOPE(cred);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG, 
				"fm_sim_pol_dev srch output flist", sr_flistp);

	if((PIN_FLIST_FLD_GET(sr_flistp, PIN_FLD_RESULTS,
					1, ebufp)) != NULL)
	{
		/* there are duplicates */
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_DUPLICATE, PIN_FLD_DEVICE_ID, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"duplicate device id in sim_pol_device_create", ebufp);
	}


	PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);

}
