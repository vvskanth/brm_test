/*
 *
 *      Copyright (c) 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static  char Sccs_id[] = "@(#)%Portal Version: fm_bill_pol_get_item_tag.cpp:BillingVelocityInt:3:2006-Sep-05 21:51:28 %";
#endif

/*******************************************************************
 * Contains the PCM_OP_BILL_POL_GET_ITEM_TAG operation. 
 *******************************************************************/

#include "pcm.h"
#include "ops/bill.h"
#include "cm_fm.h"
#include "Pin.h"
#include "pin_bill.h"

#define FILE_SOURCE_ID  "fm_bill_pol_get_item_tag.cpp"

#ifdef __cplusplus
extern "C" {

EXPORT_OP void
op_bill_pol_get_item_tag(
    cm_nap_connection_t	*connp,
    u_int		opcode,
    u_int		flags,
    pin_flist_t		*in_flistp,
    pin_flist_t		**out_flistpp,
    pin_errbuf_t	*ebufp);
}

#endif

static void fm_bill_pol_get_item_tag(
    PinContextObserver	&ctxp,
    PinOpFlags		flags,
    PinFlistObserver	&inFlist,
    PinFlistOwner	&outFlist,
    pin_errbuf_t	*ebufp);

/**
 * Main routine for the PCM_OP_BILL_POL_GET_ITEM_TAG operation. This
 * policy can be used to further customize the flexible item assignment
 * that allows customers to define the item types to use, depending on
 * service type and event type. The PCM_OP_BILL_ITEM_ASSIGN will call this
 * policy to determine the item tag to be used during item assignment. 
 * By default, it will return the same item tag it receives in its input.
 * @param connp The connection pointer.
 * @param opcode This opcode.
 * @param flags The opcode flags.
 * @param in_flistp The input flist from BILL_ITEM_ASSIGN that contains 
 *        the EVENT substruct and the chosen ITEM_TAG.
 * @param out_flistpp The output flist with possibly an updated ITEM_TAG. 
 * @param ebufp The error buffer.
 * @return nothing.
 */
void
op_bill_pol_get_item_tag(
    cm_nap_connection_t	*connp,
    u_int		opcode,
    u_int		flags,
    pin_flist_t		*in_flistp,
    pin_flist_t		**out_flistpp,
    pin_errbuf_t	*ebufp)
{
    try {
	PinContextObserver ctxp = PinContext::createAsObserved(connp->dm_ctx);
	PinOpFlags         opflags = flags;
	PinFlistObserver   inFlist = PinFlist::createAsObserved(in_flistp);
	PinFlistOwner      outFlist;
	PinErrorBuf        ebuf(ebufp);

	if (ebuf.getError()) {
		return;
	}
	ebuf.Clear();

	*out_flistpp = NULL;

	// Sanity check.
	if (opcode != PCM_OP_BILL_POL_GET_ITEM_TAG) {
	    PIN_SET_ERR(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_SYSTEM_DETERMINATE,
		PIN_ERR_BAD_OPCODE, 0, 0, opcode);
	    throw PinDeterminateExc(ebufp);
	}

	// Debug: What we got.
	PIN_LOG(inFlist, PIN_ERR_LEVEL_DEBUG,
	    "op_bill_pol_get_item_tag input flist");

	// Do the actual operation.
	fm_bill_pol_get_item_tag(ctxp, opflags, inFlist, outFlist, ebufp);

	// Log and return the output flist.
	PIN_LOG(outFlist, PIN_ERR_LEVEL_DEBUG,
	    "op_bill_pol_get_item_tag output flist");
	*out_flistpp = outFlist->release();
	return;

    } catch (PinDeterminateExc &exc) {
	PIN_LOG(exc, PIN_ERR_LEVEL_ERROR, "op_bill_pol_get_item_tag error");
	exc.copyInto(ebufp);
    }
}

/**
 * This function is the actual implemenatation of the policy.
 * @param connp The context pointer.
 * @param flags The opcode flags.
 * @param inFlist The input flist.
 * @param outFlistp The output flist. Contains the POID and ITEM_TAG from
 *        the inFlist.
 * @param ebufp The error buffer.
 * @return nothing.
 */
static void fm_bill_pol_get_item_tag(
    PinContextObserver	&ctxp,
    PinOpFlags		flags,
    PinFlistObserver	&inFlist,
    PinFlistOwner	&outFlist,
    pin_errbuf_t	*ebufp)
{
    try {

	// Create the output flist with required POID.
	outFlist = PinFlist::create();
	PinPoidObserver pd = inFlist->get(tsf_PIN_FLD_POID, 0);
	outFlist->set(tsf_PIN_FLD_POID, pd);

	// Your code here to assign a new item tag ...
	// Default implementation is just to return the same one.
	// Note that ITEM_TAG can be NULL if there was no match for
	// a given (event,service) from the /config/item_tags.
	PinStrObserver tag = inFlist->get(tsf_PIN_FLD_ITEM_TAG, 0);
	outFlist->set(tsf_PIN_FLD_ITEM_TAG, tag);

	return;

    } catch (PinDeterminateExc &exc) {
	PIN_LOG(exc, PIN_ERR_LEVEL_ERROR, "fm_bill_pol_get_item_tag error");
	exc.copyInto(ebufp);
    }
}

