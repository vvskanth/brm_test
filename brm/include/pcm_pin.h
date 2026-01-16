/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PCM_PIN_H
#define _PCM_PIN_H

/************************************************************************
 * Definitions for fields in pin specific objects.			*
 ************************************************************************/

/************************************************************************
 * Account/pin Object Fields						*
 ************************************************************************/

/*
 * PIN_FLD_ASERV_IP_FEATURES
 *
 * These values are embedded in the database, so they *cannot* change!
 */
/* XXX TBD XXX */
 
/*
 * PIN_FLD_ASERV_IP_PROTOCOL
 *
 * These values are embedded in the database, so they *cannot* change!
 */
typedef enum pcm_fld_pin_aserv_ip_protocol {
        PCM_ASERV_IP_PROTOCOL_SLIP =	30100,
        PCM_ASERV_IP_PROTOCOL_PPP =	30101
} pcm_fld_pin_aserv_ip_protocol_t;

/*
 * PIN_FLD_ASERV_IP_COMPRESS
 *
 * These values are embedded in the database, so they *cannot* change!
 */
/* XXX TBD XXX */
 
/*
 * PIN_FLD_ASERV_IP_MAILTYPE
 *
 * These values are embedded in the database, so they *cannot* change!
 */
typedef enum pcm_fld_pin_aserv_ip_mailtype {
        PCM_ASERV_IP_MAILTYPE_POP =		20100,
	PCM_ASERV_IP_MAILTYPE_SMTP =		20101,
        PCM_ASERV_IP_MAILTYPE_UNDEFINED =	20102
} pcm_fld_pin_aserv_ip_mailtype_t;

/*
 * PIN_FLD_ASERV_UUCP_FEATURES
 *
 * These values are embedded in the database, so they *cannot* change!
 */
/* XXX TBD XXX */
 
/*
 * PIN_FLD_ASERV_UUCP_REQUEST
 *
 * These values are embedded in the database, so they *cannot* change!
 */
/* XXX TBD XXX */
 
/*
 * PIN_FLD_ASERV_UUCP_SENDFILES
 *
 * These values are embedded in the database, so they *cannot* change!
 */
/* XXX TBD XXX */
 
/*
 * PIN_FLD_ASERV_UUCP_BATCH_BUILDER
 *
 * These values are embedded in the database, so they *cannot* change!
 */
/* XXX TBD XXX */
 
/*
 * PIN_FLD_ASERV_UUCP_BATCH_MUNCHER
 *
 * These values are embedded in the database, so they *cannot* change!
 */
/* XXX TBD XXX */
 
/*
 * PIN_FLD_ASERV_UUCP_BATCH_SENDER
 *
 * These values are embedded in the database, so they *cannot* change!
 */
/* XXX TBD XXX */
 
/************************************************************************
 * Bill/pin Object Fields						*
 ************************************************************************/

/*
 * PIN_FLD_BPUSE_TYPE
 *
 * These values are embedded in the database, so they *cannot* change!
 */
typedef enum pcm_fld_pin_bpuse_type {
        PCM_BPUSE_TYPE_POP =		2500,
        PCM_BPUSE_TYPE_X25 =		2501
} pcm_fld_pin_bpuse_type_t;

#endif /*_PCM_PIN_H*/
