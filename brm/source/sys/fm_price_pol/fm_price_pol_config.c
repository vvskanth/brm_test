/*******************************************************************
 *
 *  fm_price_pol_custom_config.c
 *
 * Copyright (c) 2023, Oracle and/or its affiliates.  
 *
 *	This material is the confidential property of Oracle Corporation
 *	or its subsidiaries or licensors and may be used, reproduced, stored
 *	or transmitted only in accordance with a valid Oracle license or
 *	sublicense agreement.
 *
 *******************************************************************/

#ifndef lint
static  char    Sccs_id[] = "@(#)% fm_price_pol_custom_config.c:ServerIDCVelocityInt:1:2023-Jul-18 16:47:12 %";
#endif

#include <stdio.h>			/* for FILE * in pcm.h */
#include "ops/price.h"
#include "pcm.h"
#include "cm_fm.h"

 /*
  * NOTE THAT THE DISPATCH ENTRIES ARE COMMENTED. WHEN YOU OVERRIDE
  * AN IMPLEMENTATION, UNCOMMENT THE LINE BELOW THAT MATCHES THE
  * OPCODE FOR WHICH YOU HAVE PROVIDED AN ALTERNATE IMPLEMENTATION.
  */


struct cm_fm_config fm_price_pol_custom_config[] = {
	/* opcode as a u_int, function name (as a string) */
/*	{ PCM_OP_PRICE_POL_DELETE_DEAL, 	"op_price_pol_delete_deal" }, */
/*	{ PCM_OP_PRICE_POL_DELETE_PRODUCT,	"op_price_pol_delete_product" }, */
/*	{ PCM_OP_PRICE_POL_DELETE_DISCOUNT,	"op_price_pol_delete_discount" }, */
/*	{ PCM_OP_PRICE_POL_DELETE_DEPENDENCY,	"op_price_pol_delete_dependency" }, */
/*	{ PCM_OP_PRICE_POL_DELETE_TRANSITION,	"op_price_pol_delete_transition" }, */
/*	{ PCM_OP_PRICE_POL_PREP_DEAL,		"op_price_pol_prep_deal" }, */
/*	{ PCM_OP_PRICE_POL_PREP_PRODUCT,	"op_price_pol_prep_product" }, */
/*	{ PCM_OP_PRICE_POL_PREP_DISCOUNT,	"op_price_pol_prep_discount" }, */
/*	{ PCM_OP_PRICE_POL_PREP_DEPENDENCY,	"op_price_pol_prep_dependency" }, */
/*	{ PCM_OP_PRICE_POL_PREP_TRANSITION,	"op_price_pol_prep_transition" }, */
/*	{ PCM_OP_PRICE_POL_VALID_DEAL,		"op_price_pol_valid_deal" }, */
/*	{ PCM_OP_PRICE_POL_VALID_PRODUCT,	"op_price_pol_valid_product" }, */
/*	{ PCM_OP_PRICE_POL_VALID_DISCOUNT,	"op_price_pol_valid_discount" }, */
/*	{ PCM_OP_PRICE_POL_VALID_DEPENDENCY,	"op_price_pol_valid_dependency" }, */
/*	{ PCM_OP_PRICE_POL_VALID_TRANSITION,	"op_price_pol_valid_transition" }, */
/*	{ PCM_OP_PRICE_POL_PREP_PIPELINE_DISCOUNTMODEL,	"op_price_pol_prep_pipeline_discountmodel" }, */
/*	{ PCM_OP_PRICE_POL_VALID_PIPELINE_DISCOUNTMODEL, "op_price_pol_valid_pipeline_discountmodel" }, */
	{ 0,	(char *)0 }
};

