/*
 *	@(#) % %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */


#ifndef _pin_os_ioctl_h_
#define _pin_os_ioctl_h_

/* Who knows where ioctl's come from under WIN32.
   For now, assume that they come from some other pin_os header
*/
#if defined(__unix)

#include <sys/ioctl.h>

#if defined(__sun)
/* ioctls such as FIONREAD and FIONBIO aren't brought in by the above */
#include <sys/filio.h>
#endif

#endif

#endif
