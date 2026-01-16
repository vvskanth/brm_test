/*
 *	@(#) % %
 *       
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *       
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_resource_h_
#define _pin_os_resource_h_

/*
** Make getrlimit(), setrlimit() available.
** For systems which have these, HAS_RLIMIT is defined.
*/
#if defined(__unix)

#include <sys/resource.h>

#define HAS_RLIMIT

#endif

#endif
