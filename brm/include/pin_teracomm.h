/* Continuus file information --- %objectname */
/*******************************************************************
 *
 * @(#)pin_teracomm.h 1.2 00/15/04
 *
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef _PIN_TERACOMM_H
#define _PIN_TERACOMM_H

/* Values for PIN_FLD_LED_STATUS: */
typedef enum {
	PIN_TERACOMM_LED_STATUS_GREEN = 2,
	PIN_TERACOMM_LED_STATUS_BLINK_GREEN = 3,
	PIN_TERACOMM_LED_STATUS_YELLOW = 4,
	PIN_TERACOMM_LED_STATUS_BLINK_YELLOW = 5,
	PIN_TERACOMM_LED_STATUS_OFF = 6
} pin_teracomm_LED_status_t;


/*
 * Default values for UI display
 */
#define PIN_TERACOMM_DEFAULT_LED_STATUS	PIN_TERACOMM_LED_STATUS_OFF
#define PIN_TERACOMM_DEFAULT_TERALINK_NAME	"*"
#define PIN_TERACOMM_DEFAULT_ASSOC_NW	"*"

/*
 * Validate LED value ranges
 */
#define PIN_TERACOMM_VALID_LED_STATUS(n) \
	((n) >= PIN_TERACOMM_LED_STATUS_GREEN && \
	(n) <= PIN_TERACOMM_LED_STATUS_OFF)


/* Value range for PIN_FLD_IP_LIMIT: */
#define PIN_TERACOMM_MIN_IP_LIMIT 0
#define PIN_TERACOMM_MAX_IP_LIMIT 100
#define PIN_TERACOMM_VALID_IP_LIMIT(n) \
	((n) >= PIN_TERACOMM_MIN_IP_LIMIT && \
	(n) <= PIN_TERACOMM_MAX_IP_LIMIT)

/*
 * Valid string length for PIN_FLD_MODEM_MAC_ADDR:
 * Note: This value must also have only hex. digits.
 */
#define PIN_TERACOMM_VALID_MODEM_MAC_ADDR_LEN 12

/*
 * Initial value for PIN_FLD_MODEM_MAC_ADDR:
 * When a /service/ip/cable/teracomm object is created,
 * the value for PIN_FLD_MODEM_MAC_ADDR is set to
 * PIN_TERACOMM_UNSPECIFIED_MODEM_MAC_ADDR, which is
 * not a valid modem mac. addr.  This value indicates
 * that the service object does not yet contain real
 * provisioning data.
 */
#define PIN_TERACOMM_UNSPECIFIED_MODEM_MAC_ADDR \
	"<Unspecified>"

/* Maximum string length of PIN_FLD_TERALINK_NAME: */
#define PIN_TERACOMM_MAX_TERALINK_NAME_LEN 32

/* Maximum string length of PIN_FLD_ASSOC_NETWORK: */
#define PIN_TERACOMM_MAX_ASSOC_NW_LEN 40

/* Maximum string length of PIN_FLD_MULTICAST_GRP_ALIAS: */
#define PIN_TERACOMM_MAX_MULTICAST_GRP_ALIAS_LEN 8

/* Bandwidth validity tests:
 * Bandwidths must be in the range [0 8192] and
 * be a multiple of 64.  A minimum bandwidth can
 * not be greater than the corresponding maximum
 * bandwidth.  The down stream minimum bandwidth must
 * be 0 or equal to the down stream maximum bandwidth.
 */
#define PIN_TERACOMM_MIN_BANDWIDTH 0
#define PIN_TERACOMM_MAX_BANDWIDTH 8192
#define PIN_TERACOMM_IS_VALID_BANDWIDTH(bw) \
	((bw) >= PIN_TERACOMM_MIN_BANDWIDTH && \
	(bw) <= PIN_TERACOMM_MAX_BANDWIDTH && \
	(((bw) % 64) == 0))
#define PIN_TERACOMM_VALID_DOWNSTREAM_COMBO(min_bw, max_bw) \
	((min_bw) == 0 || (min_bw) == (max_bw))
#define PIN_TERACOMM_VALID_UPSTREAM_COMBO(min_bw, max_bw) \
	((min_bw) <= (max_bw))

#endif /* _PIN_TERACOMM_H */
