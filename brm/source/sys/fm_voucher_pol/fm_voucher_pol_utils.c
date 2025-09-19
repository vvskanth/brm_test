/*
 *
 * Copyright (c) 2002, 2024, Oracle and/or its affiliates. 
 *
 * This material is the confidential property of Oracle Corporation
 * or its licensors and may be used, reproduced, stored or transmitted
 * only in accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)$Id: fm_voucher_pol_utils.c /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:15:48 nishahan Exp $";
#endif

#include <stdio.h>
#include <string.h>

#include <pcm.h>
#include <pinlog.h>
#include "ops/voucher.h"
#include <pin_errs.h>
#include <cm_fm.h>
#include <pin_voucher.h>

#define FILE_SOURCE_ID "fm_voucher_pol_utils.c(2)"

/*****************************************************************************
 * Global routines contained within
 *****************************************************************************/
void 
fm_voucher_pol_utils_is_valid_fld (
	pcm_context_t	*ctxp,
	poid_t		*poidp,
	int32		fldnum,
	char		*fld_value,
	pin_decimal_t	*quantity,
	pin_errbuf_t	*ebufp
	);

void 
fm_voucher_pol_utils_is_valid_quantity (
	pcm_context_t	*ctxp,
	pin_decimal_t	*quantity,
	pin_decimal_t	*batch_total, 
	pin_decimal_t	*batch_qty,
	pin_decimal_t	*pack_qty,
	pin_errbuf_t	*ebufp
	);

void 
fm_voucher_pol_utils_get_voucher_numbers(
	pin_decimal_t	**start_code,
	pin_decimal_t	**end_code,
	pin_decimal_t	*quantity,
	char		*start_number,
	pin_errbuf_t	*ebufp
	);

void 
fm_voucher_pol_utils_read_order(
	pcm_context_t	*ctxp,
	poid_t		*poidp,
	pin_flist_t	**r_flistpp,
	pin_errbuf_t	*ebufp
	);

void
fm_voucher_pol_utils_read_device_state(
        pcm_context_t   *ctxp,
        poid_t          *poidp,
        pin_flist_t     **r_flistpp,
        pin_errbuf_t    *ebufp
        );

/******************************************************************************
 * fm_voucher_pol_utils_is_valid_fld()
 *
 * Validate the VOUCHER serial number to make sure that they are not duplicates
 *
 * input:
 *	: poid of the order or type only 
 *	: the field that is being validated
 *	: the value of the fields being validated
 *	: the quantity of the cards to get the end value of the range 
 *
 * output:
 *	: errbuf is set if there are duplicates
 ******************************************************************************/
void 
fm_voucher_pol_utils_is_valid_fld(
	pcm_context_t	*ctxp,
	poid_t		*poidp,
	int32		fldnum,
	char		*fld_value,
	pin_decimal_t	*quantity,
	pin_errbuf_t	*ebufp
	)
{
	pin_flist_t	*s_flistp = NULL;	/* search flist */
	pin_flist_t	*sr_flistp = NULL;	/* return flist from search */
	pin_flist_t	*sub_flistp = NULL;	/* flist for substruct */
	pin_flist_t	*ss_flistp = NULL;	/* another flist */
	pin_flist_t	*sss_flistp = NULL;	/* another flist */
	pin_flist_t	*r_flistp = NULL;	/* another flist */
	pin_flist_t	*res_flistp = NULL;	/* another flist */
	pin_flist_t	*tmp_flistp = NULL;	/* another flist */

	poid_t		*spoidp = NULL;		/* search poid */
	poid_t		*pdp = NULL;		/* poid of the order in db */
	pin_cookie_t	cookie = NULL;		/* cookie for search rslts */
	int32		count = 0;		/* number of duplicate rslts */
	int32		cred = 0;		/* scoping credentials */
	int32		flag = SRCH_DISTINCT;	/* flag for search opcode */
	int64		dbno = 0;		/* db no for creating poid */
	int32		eid = 0;		/* element id of array	*/
	int32		n_eid = 0;		/* element id of array	*/
	int32		status = PIN_VOUCHER_ORDER_CANCELLED; /* status to check */
	char		new_voucher [BUFSIZ] = "";
	pin_decimal_t	*ext_quantity = NULL;
	char		*ext_serial = NULL;
	pin_decimal_t	*new_start_code  = NULL;
	pin_decimal_t	*new_end_code  = NULL;
	pin_decimal_t	*ext_start_code = NULL;
	pin_decimal_t	*ext_end_code = NULL;

	if(PIN_ERR_IS_ERR(ebufp)){
		return;
		/********/
	}

	PIN_ERR_CLEAR_ERR(ebufp);
	
	/*
	 * create the flists
	 */
	s_flistp = PIN_FLIST_CREATE(ebufp);
	
	/*
	 * create the search poid
	 */
	dbno = PIN_POID_GET_DB(poidp);
	spoidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

	/*
	 * populate the search flist
	 */
	PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, spoidp, ebufp);
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp); 

	/*
 	 * Get the voucher batch and part number section of the
 	 * voucher start number
 	 */
	pin_strncpy(new_voucher, sizeof(new_voucher), fld_value, 10);
	pin_strlcat (new_voucher, "%", sizeof(new_voucher));

	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_TEMPLATE,
		(void *)"select X from /order/voucher where "
		"( F1 like V1 ) "
		"AND ( F2 != V2 ) ",
		 ebufp);
	
	
	ss_flistp = PIN_FLIST_ELEM_ADD( s_flistp, PIN_FLD_ARGS, 1, ebufp);
	
	sub_flistp = PIN_FLIST_ELEM_ADD( ss_flistp,
		PIN_FLD_VOUCHER_ORDERS, 1, ebufp);

	/*
	 * the fld being searched
	 */
	PIN_FLIST_FLD_SET(sub_flistp, fldnum, (void *)new_voucher, ebufp);
	
	/*
	 * make sure to ignore the cancelled orders
	 */
	sub_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 2, ebufp);
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_STATUS, &status,
		ebufp);



	ss_flistp = PIN_FLIST_ELEM_ADD( s_flistp, PIN_FLD_RESULTS, 0, ebufp);

	/*
	 * Get the poid
	 */
	PIN_FLIST_FLD_SET(ss_flistp, PIN_FLD_POID, (void *)NULL, ebufp);


	/*
	 * Get the start serial number
	 */
	sss_flistp = PIN_FLIST_ELEM_ADD (ss_flistp, PIN_FLD_VOUCHER_ORDERS, 0,
		ebufp);
	PIN_FLIST_FLD_SET(sss_flistp, PIN_FLD_START_SERIAL_NO, (void *)NULL, ebufp);

	/*
	 * Get the quantity
	 */
	PIN_FLIST_FLD_SET(sss_flistp, PIN_FLD_QUANTITY, (void *)NULL, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_voucher_pol_utils_is_valid_fld search input flist", s_flistp);

	/*
	 * turn off  scoping just before the search and on right after
	 */
	cred = CM_FM_BEGIN_OVERRIDE_SCOPE(CM_CRED_SCOPE_OVERRIDE_ROOT);
	
	
	/*
	 * do the search
	 */
	PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, 0, s_flistp, 
		&sr_flistp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp))
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, "after global search");

	/*
	 * turn the scoping back on
	 */
	CM_FM_END_OVERRIDE_SCOPE(cred);
	
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_voucher_pol_utils_is_valid_fld search return flist", sr_flistp);


	/*
	 * Find the start and end code of the new voucher serial number
	 */
	fm_voucher_pol_utils_get_voucher_numbers(&new_start_code, &new_end_code,
		 quantity, fld_value, ebufp);
 
	/*
	 * Validate the voucher start serial number and the quantity 
	 * does not exist in the database
	 */
	while ((res_flistp = PIN_FLIST_ELEM_GET_NEXT(sr_flistp,
		PIN_FLD_RESULTS, &eid, 1, &cookie, ebufp))
			!= (pin_flist_t *)NULL) {

		/*
		 * Get the start serial number of existing order
		 */
		tmp_flistp = PIN_FLIST_ELEM_GET(res_flistp, PIN_FLD_VOUCHER_ORDERS,
			PIN_ELEMID_ANY, 0, ebufp);


		ext_serial = PIN_FLIST_FLD_GET(tmp_flistp, 
			PIN_FLD_START_SERIAL_NO, 0, ebufp);
			
		/*
		 * Get the quantity of the existing orders
		 */
		ext_quantity = PIN_FLIST_FLD_GET(tmp_flistp, 
			PIN_FLD_QUANTITY, 0, ebufp);

		fm_voucher_pol_utils_get_voucher_numbers( &ext_start_code, &ext_end_code,
			ext_quantity, ext_serial, ebufp);

		if ((pin_decimal_compare(ext_start_code, new_start_code, ebufp) == -1 ||
			(pin_decimal_compare(ext_start_code, new_start_code, ebufp) == 0))  &&
			(pin_decimal_compare(ext_end_code, new_start_code, ebufp) == 1 ||
			(pin_decimal_compare(ext_end_code, new_start_code, ebufp) == 0))) {
			count = count +1;
			n_eid = eid;
		}

		else if ((pin_decimal_compare(ext_start_code, new_end_code, ebufp) == -1 ||
			(pin_decimal_compare(ext_start_code, new_end_code, ebufp) == 0))  &&
			(pin_decimal_compare(ext_end_code, new_end_code, ebufp) == 1 ||
			(pin_decimal_compare(ext_end_code, new_end_code, ebufp) == 0))) {
			count = count +1;
			n_eid = eid;
		}

		else if ((pin_decimal_compare(new_start_code, ext_start_code, ebufp) == -1 ||
			(pin_decimal_compare(new_start_code, ext_start_code, ebufp) == 0))  &&
			(pin_decimal_compare(new_end_code, ext_start_code, ebufp) == 1 ||
			(pin_decimal_compare(new_end_code, ext_start_code, ebufp) == 0))) {
			count = count +1;
			n_eid = eid;
		}

		else if ((pin_decimal_compare(new_start_code, ext_end_code, ebufp) == -1 ||
			(pin_decimal_compare(new_start_code, ext_end_code, ebufp) == 0))  &&
			(pin_decimal_compare(new_end_code, ext_end_code, ebufp) == 1 ||
			(pin_decimal_compare(new_end_code, ext_end_code, ebufp) == 0))) {
			count = count +1;
			n_eid = eid;
		}

		PIN_DECIMAL_DESTROY_EX(&ext_start_code);
		PIN_DECIMAL_DESTROY_EX(&ext_end_code);
		
        }
	if(count > 1){
		/*
		 * there are duplicates
		 */
		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_DUPLICATE, fldnum, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
			"fm_voucher_pol_utils_is_valid_fld - duplicate fld value",
			 ebufp);

	}
	else if(count == 1){
		/*
		 * check if it is the same order that we are trying to 
		 * update
		 */
		if(!PIN_POID_IS_TYPE_ONLY(poidp)){
			/*r_flistp = PIN_FLIST_ELEM_GET_NEXT(sr_flistp,
				PIN_FLD_RESULTS, &n_eid, 0, &cookie, ebufp);*/
			r_flistp = PIN_FLIST_ELEM_GET(sr_flistp,
				PIN_FLD_RESULTS, n_eid, 0, ebufp);
			pdp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_POID, 0,
				ebufp);
			if(PIN_POID_COMPARE(poidp, pdp, 0, ebufp) != 0){
				/*
				 * set an error
				 */
				pin_set_err(ebufp, PIN_ERRLOC_FM,
					PIN_ERRCLASS_APPLICATION,
					PIN_ERR_DUPLICATE, fldnum, 0, 0);
					PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
					"fm_voucher_pol_utils_is_valid_fld"
					"-duplicate field value", ebufp);
			}
		}
		else {
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_DUPLICATE, fldnum, 0, 0);
				PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
				"fm_voucher_pol_utils_is_valid_fld"
				"-duplicate field value", ebufp);
		}
	}

	PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
	PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);

	PIN_DECIMAL_DESTROY_EX(&new_start_code);
	PIN_DECIMAL_DESTROY_EX(&new_end_code);
}

/******************************************************************************
 * fm_voucher_pol_utils_is_valid_quantity()
 *
 * Validate the order quantity matches quantity defined in the batch
 *
 ******************************************************************************/
void 
fm_voucher_pol_utils_is_valid_quantity(
	pcm_context_t	*ctxp,
	pin_decimal_t	*quantity,
	pin_decimal_t	*batch_total, 
	pin_decimal_t	*batch_qty,
	pin_decimal_t	*pack_qty,
	pin_errbuf_t	*ebufp
	)
{

	pin_flist_t	*sub_flistp = NULL;
	pin_decimal_t	*invt_qty = NULL;
	pin_decimal_t	*batch_actual = NULL;
	pin_decimal_t	*diff_val = NULL;
	pin_decimal_t	*constp = NULL;
	pin_decimal_t	*const2p = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);


	if((!pin_decimal_is_NULL(batch_qty, ebufp)) && 
		(!pin_decimal_is_NULL(pack_qty, ebufp)) && 
		(!pin_decimal_is_NULL(batch_total, ebufp))) {

		invt_qty = pin_decimal_multiply(batch_qty, pack_qty, ebufp);

		if(!(pbo_decimal_is_zero(invt_qty, ebufp))){
			batch_actual = pbo_decimal_divide(quantity, invt_qty, ebufp); 
		}
		diff_val = pin_decimal_subtract(batch_total, batch_actual, ebufp);
		constp = pin_decimal("1.0", ebufp);
		const2p = pin_decimal("0.0", ebufp);
		
		if (pin_decimal_compare(diff_val, constp, ebufp) == 1 ||
			pin_decimal_compare(diff_val, constp, ebufp) == 0  || 
			pin_decimal_compare(diff_val, const2p, ebufp) == -1 ) {

			/*
		 	 *  Set an error code
			 */
			pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
				PIN_COMPARE_NOT_EQUAL, PIN_FLD_QUANTITY, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				"fm_voucher_pol_utils_is_valid_quantity - quantity does not match",
				 ebufp);
        	}
	}

	PIN_DECIMAL_DESTROY_EX(&invt_qty);
	PIN_DECIMAL_DESTROY_EX(&diff_val);
	PIN_DECIMAL_DESTROY_EX(&batch_actual);
	PIN_DECIMAL_DESTROY_EX(&constp);
	PIN_DECIMAL_DESTROY_EX(&const2p);
	return;
}


/******************************************************************************
 * fm_voucher_pol_utils_get_voucher_numbers()
 *
 * The voucher number consists of batch Number + Pack Number
 * + six incremented digits (code)
 *
 * This function will return the start code of the serial number and the end code
 * The end code is the start code + the quantity 
 * 
 ******************************************************************************/
void 
fm_voucher_pol_utils_get_voucher_numbers(
	pin_decimal_t	**start_code,
	pin_decimal_t	**end_code,
	pin_decimal_t	*quantity,
	char		*start_number,
	pin_errbuf_t	*ebufp
	)
{
	char		voucher_code[BUFSIZ] = "";
	pin_decimal_t	*max_valp = NULL; 
	pin_decimal_t	*tmp_quantityp= NULL;
	pin_decimal_t   *tmp_end_code= NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	if (strlen(start_number) < 10) {
		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_NOT_FOUND, PIN_FLD_START_SERIAL_NO, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
			"fm_voucher_pol_utils_get_voucher_numbers - serial"
			" number length to be more than 10",
			 ebufp);
	}

	pin_strlcpy(voucher_code, start_number + 10, sizeof(voucher_code)); 

	if (voucher_code[0] != '\0') {
		*start_code = pbo_decimal_from_str(voucher_code, ebufp);


		if (!pin_decimal_is_NULL(*start_code, ebufp) ) {
			tmp_end_code = pin_decimal_add(quantity, *start_code, 
				ebufp);
			/*
		 	 * Correction is required as Count include the starting number
			 */

			tmp_quantityp = (pin_decimal_t *)pin_decimal("1", ebufp); 

			*end_code = pin_decimal_subtract(tmp_end_code, tmp_quantityp,
				ebufp);
			PIN_DECIMAL_DESTROY_EX(&tmp_quantityp);
			PIN_DECIMAL_DESTROY_EX(&tmp_end_code);
		}
		max_valp = (pin_decimal_t *)pin_decimal("1000000.0", ebufp); 
		if (pin_decimal_compare(*end_code, max_valp, ebufp) == 1 ) {
			/*
			 * Set an error code
			 */
			pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
				PIN_ERR_BAD_ARG, PIN_FLD_START_SERIAL_NO, 0, 0);
			PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR, 
				"fm_voucher_pol_utils_is_valid_fld - bad quantity",
				 ebufp);
		}
	}
	PIN_DECIMAL_DESTROY_EX(&max_valp);

	return;
}


/******************************************************************************
 * fm_voucher_pol_utils_read_order()
 *
 * This function will read the order object from the database
 * 
 ******************************************************************************/
void 
fm_voucher_pol_utils_read_order(
	pcm_context_t	*ctxp,
	poid_t		*poidp,
	pin_flist_t	**r_flistpp,
	pin_errbuf_t	*ebufp
	)
{

	pin_flist_t	*read_flistp = NULL;
	pin_flist_t	*sub_flistp = NULL;
	void		*vp = NULL;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	read_flistp = PIN_FLIST_CREATE(ebufp);

	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_POID, poidp, ebufp);

	/*
	 * retreive the status
	 */
	PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_STATUS, (void *)vp, ebufp);

	sub_flistp = PIN_FLIST_ELEM_ADD(read_flistp, PIN_FLD_VOUCHER_ORDERS, 
		0, ebufp);
	/*
	 * we also need to retreive the quantity for voucher and serial
	 * number for validation
	 */


	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_START_SERIAL_NO,
		(void *)vp, ebufp);
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_QUANTITY, 
		(void *)vp, ebufp);
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_BATCH_QUANTITY,
		(void *)vp, ebufp);
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_PACK_QUANTITY,
		(void *)vp, ebufp);
	PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_BATCH_TOTAL,
		(void *)vp, ebufp);
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_voucher_pol_utils_read_order read flds input  flist", 
			read_flistp);

	/*
	 * call the read flds opcode
	 */
	PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, read_flistp, r_flistpp, ebufp);

	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"fm_voucher_pol_utils_read_order read flds return flist",
			*r_flistpp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_voucher_pol_utils_read_order error", ebufp);
	}

	/* Cleanup! */
	PIN_FLIST_DESTROY_EX(&read_flistp, NULL);

	return;
} 



void
fm_voucher_pol_utils_verify_deal(
        pcm_context_t   *ctxp,
        poid_t     	*poidp,
        poid_t     	*deal_poidp,
	pin_flist_t     **deal_flistpp,
	pin_errbuf_t	*ebufp
	)
{
        poid_t          *spoidp = NULL;         /* search poid */
        pin_flist_t     *s_flistp = NULL;       /* search flist */
        pin_flist_t     *sub_flistp = NULL;     /* sub flist */
        pin_flist_t     *ss_flistp = NULL;      /* sub sub flist */
        pin_flist_t     *res_flistp = NULL;     /* result flist */
	pin_cookie_t	cookie = NULL;		/* cookie for search rslts */
	int64		dbno = 0;		/* db no for creating poid */
	int32		cred = 0;		/* scoping credentials */
	int32		eid = 0;		/* number of duplicate rslts */
	int32		flag = 0;		/* flag for search opcode */

        if(PIN_ERR_IS_ERR(ebufp)){
                return;
                /********/
        }

        PIN_ERR_CLEAR_ERR(ebufp);

        /*
         * create the flists
         */
        s_flistp = PIN_FLIST_CREATE(ebufp);

        /*
         * create the search poid
         */
        dbno = PIN_POID_GET_DB(poidp);
        spoidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

        /*
         * populate the search flist
         */
        PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, spoidp, ebufp);
	PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp); 

        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_TEMPLATE, (void *)
		"select X from /deal where F1 = V1 ", ebufp);

        /*
         * search the deal object
         */
        sub_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_POID, (poid_t*)deal_poidp, ebufp);

        ss_flistp = PIN_FLIST_ELEM_ADD( s_flistp, PIN_FLD_RESULTS, 0, ebufp);
        PIN_FLIST_FLD_SET(ss_flistp, PIN_FLD_POID, (void *)NULL, ebufp);

        PIN_FLIST_FLD_SET(ss_flistp, PIN_FLD_END_T, (void *)NULL, ebufp);

        cred = CM_FM_BEGIN_OVERRIDE_SCOPE(CM_CRED_SCOPE_OVERRIDE_ROOT);

        PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, 0, s_flistp,
                deal_flistpp, ebufp);

        CM_FM_END_OVERRIDE_SCOPE(cred);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_voucher_pol_utils_verify_deal search return flist", 
		*deal_flistpp);

        if ((res_flistp = PIN_FLIST_ELEM_GET_NEXT(*deal_flistpp,
                PIN_FLD_RESULTS, &eid, 1, &cookie, ebufp))
                != (pin_flist_t *)NULL) {
		if( (PIN_FLIST_FLD_GET(res_flistp, 
			PIN_FLD_POID, 1, ebufp)) == NULL ) {
	                pin_set_err(ebufp, PIN_ERRLOC_FM, 
				PIN_ERRCLASS_APPLICATION,
       		                PIN_ERR_NOT_FOUND, PIN_FLD_DEAL_OBJ, 0, 0);
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
				"Deal object not found in db");
		}
	}
	else {
	        pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
       			PIN_ERR_NOT_FOUND, PIN_FLD_DEAL_OBJ, 0, 0);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, "Deal object not found "
			"in db");
	}
		
        PIN_FLIST_DESTROY_EX(&s_flistp, NULL);

	return;
}

void
fm_voucher_pol_utils_read_device_state(
        pcm_context_t   *ctxp,
        poid_t          *poidp,
        pin_flist_t     **r_flistpp,
        pin_errbuf_t    *ebufp
        )
{

        poid_t          *spoidp = NULL;         /* search poid */
        pin_flist_t     *s_flistp = NULL;       /* search flist */
        pin_flist_t     *sr_flistp = NULL;      /* search return flist */
        pin_flist_t     *sub_flistp = NULL;     /* sub flist */
        pin_flist_t     *ss_flistp = NULL;      /* sub sub flist */
        pin_flist_t     *res_flistp = NULL;     /* result flist */
        pin_cookie_t    cookie = NULL;          /* cookie for search rslts */
        int64           dbno = 0;               /* db no for creating poid */
        int32           cred = 0;               /* scoping credentials */
        int32           eid = 0;                /* number of duplicate rslts */
        int32           flag = 0;               /* flag for search opcode */

        if(PIN_ERR_IS_ERR(ebufp)){
                return;
                /********/
        }

        PIN_ERR_CLEAR_ERR(ebufp);

        /*
         * create the flists
         */
        s_flistp = PIN_FLIST_CREATE(ebufp);

        /*
         * create the search poid
         */
        dbno = PIN_POID_GET_DB(poidp);
        spoidp = PIN_POID_CREATE(dbno, "/search", -1, ebufp);

        /*
         * populate the search flist
         */
        PIN_FLIST_FLD_PUT(s_flistp, PIN_FLD_POID, spoidp, ebufp);
        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_FLAGS, (void *)&flag, ebufp);

        PIN_FLIST_FLD_SET(s_flistp, PIN_FLD_TEMPLATE, (void *)
                "select X from /device/voucher where F1 = V1 ", ebufp);

        /*
         * search the deal object
         */
        sub_flistp = PIN_FLIST_ELEM_ADD(s_flistp, PIN_FLD_ARGS, 1, ebufp);
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_POID, (poid_t*)poidp, ebufp);

        sub_flistp = PIN_FLIST_ELEM_ADD( s_flistp, PIN_FLD_RESULTS, 0, ebufp);
        PIN_FLIST_FLD_SET(sub_flistp, PIN_FLD_STATE_ID, (void *)NULL, ebufp);

        cred = CM_FM_BEGIN_OVERRIDE_SCOPE(CM_CRED_SCOPE_OVERRIDE_ROOT);

        PCM_OP(ctxp, PCM_OP_GLOBAL_SEARCH, 0, s_flistp,
                &sr_flistp, ebufp);

        CM_FM_END_OVERRIDE_SCOPE(cred);

        PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
                "fm_voucher_pol_utils_read_device_state search return flist",
                sr_flistp);
	if ((res_flistp = PIN_FLIST_ELEM_TAKE_NEXT(sr_flistp,
                PIN_FLD_RESULTS, &eid, 1, &cookie, ebufp))
                != (pin_flist_t *)NULL) {
		*r_flistpp = res_flistp;
	}
	else {
		*r_flistpp = NULL;
	}

        PIN_FLIST_DESTROY_EX(&s_flistp, NULL);
        PIN_FLIST_DESTROY_EX(&sr_flistp, NULL);
        return;
}
