#ifndef _pin_os_printf_h_
#define _pin_os_printf_h_
/*
 *	@(#) % %
 *      
 *      Copyright (c) 1996 - 2023 Oracle. 
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#include <stdio.h>

/*
 *  Some of the platforms are incorrectly naming these. They are not standard
 *  and should therefore have a leading underscore. VC++ is doing it correctly
 *  but the other platforms are not. So we map the ANSI name to the non-ANSI
 *  name on the Unix platforms.
 */
#define _snprintf   snprintf

#endif
