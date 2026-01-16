/* continuus file information --- %full_filespec: psiu_inet.h~3:incl:3 % */
/*
 *  @(#) %full_filespec: psiu_inet.h~3:incl:3 %
 *
 *      Copyright (c) 1998 - 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PSIU_INET_H_
#define _PSIU_INET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pin_os.h"
#include "pcm.h"

#define PSIU_MAX_IPADDR_LEN	16

/*
 * Convert an ip address to a dotted string format
 * Parameters:
 *	addr		ip address in network byte order
 *	buf		string buffer
 *	buf_len		length of buffer
 */
extern void
psiu_inet_ip2str(
	struct in_addr	addr,
	char		*buf,
	int		buf_len,
	pin_errbuf_t	*ebufp);

/*
 * Convert an ip address in dotted string format to an ip address in
 * network byte order.
 * Parameters:
 *	ipstr		ip address in dotted string format
 *	addr		pointer to ip address
 */
extern void
psiu_inet_str2ip(
	char		*ipstr,
	struct in_addr	*addr,
	pin_errbuf_t	*ebufp);

/*
 * Check if an ip address in dotted string format is valid
 * Parameters:
 *	ipstr		ip address in dotted string format
 */
extern int
psiu_inet_isvalidip(
	char		*ipstr);

/*
 * Return the network address of an ip address
 * Parameters:
 *	addr		ip address in network byte order
 */
extern struct in_addr
psiu_inet_netof(
	struct in_addr	addr);

#ifdef __cplusplus
}
#endif

#endif /*_PSIU_INET_H_*/
