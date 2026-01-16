 /*******************************************************************
 * @(#)%Portal Version: dm_eai_plugin.h:UelEaiVelocityInt:1:2006-Sep-05 06:52:57 %
 * Copyright (c) 2000 - 2006 Oracle. All rights reserved.This material is the confidential property of Oracle Corporation
 * or its licensors and may be used,reproduced, stored or transmitted only in accordance with a valid Oracle license or sublicense
 * agreement.
 *
 *******************************************************************/


#ifndef _DM_EAI_PLUGIN_H__
#define _DM_EAI_PLUGIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#define EAI_EXTERN 

#define EAI_IMPORT 

#define TYPE_XML 1
#define TYPE_FLIST 2

/*
 * Initialize is called when the dm_eai starts up.  The return values
 * from this function can be PIN_RESULT_PASS or PIN_RESULT_FAIL.  For
 * payload in Flist format the no_xml parameter has to be set to 
 * PIN_BOOLEAN_TRUE.
 */
EAI_EXTERN int Initialize (void **context, int *output_type);

/*
 * OpenTransaction is called when a transaction is initiated with the 
 * dm_eai.  The return values from this function can be PIN_RESULT_PASS
 * or PIN_RESULT_FAIL.
 */
EAI_EXTERN int OpenTransaction (void *ctx);

/*
 * PublishEvent is called when there is a complete business object to
 * be published.  The payload with be an xml document (char *) or an
 * flist (pin_flist_t *) based on the setting of the no_xml parameter
 * on return of the Initialize call.  The srvc_poidp indicates the 
 * service type associated with the session that generated this 
 * business object.
 */
EAI_EXTERN int PublishEvent (void *context, void *payload, char *srvc_poidp);

/*
 * PrepareCommit is called when Infranet is about to commit the 
 * the transaction to its database.  The return values from this
 * function can be PIN_RESULT_PASS or PIN_RESULT_FAIL.
 */
EAI_EXTERN int PrepareCommit (void *ctx);

/*
 * CommitTransaction is called after the transaction has been committed
 * to Infranet database.  The result values from this fucntion can be
 * PIN_RESULT_PASS or PIN_RESULT_FAIL.  A return of PIN_RESULT_FAIL 
 * does not abort the transaction within Infranet.
 */
EAI_EXTERN int CommitTransaction (void *ctx);

/*
 * AbortTransaction is called when the transaction was aborted within
 * Infranet.
 */
EAI_EXTERN int AbortTransaction (void *ctx);

/*
 * Shutdown is called when the dm_eai terminates normally.  
 */
EAI_EXTERN void Shutdown (void *ctx);

/*
 * This function should be call  to set the identifier that is to be sent back
 * in response to a PublishEvent Call
 */

EAI_IMPORT void SetIdentifier(void *, int);
#ifdef __cplusplus
}
#endif

#endif 
