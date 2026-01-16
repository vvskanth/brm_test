/*
 *	@(#) % %
 *      
* Copyright (c) 1996, 2023, Oracle and/or its affiliates.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_h_
#define _pin_os_h_


#ifndef _pin_os_network_h_
  #include <pin_os_network.h>
#endif

#ifndef _pin_os_time_h_
  #include <pin_os_time.h>
#endif

#ifndef _pin_os_dir_h_
  #include <pin_os_dir.h>
#endif

#ifndef _pin_os_pthread_h_
  #include <pin_os_pthread.h>
#endif

#ifndef _pin_os_string_h_
  #include <pin_os_string.h>
#endif

#ifndef _pin_os_random_h_
  #include <pin_os_random.h>
#endif

#ifndef _pin_os_getopt_h_
  #include <pin_os_getopt.h>
#endif

#ifndef _pin_os_unixuid_h_
  #include <pin_os_unixuid.h>
#endif

/* Here are some capabilities which don't yet have modules */
#if defined(__unix)

#include <pwd.h>
#include <crypt.h>

#define HAS_PID

#ifndef O_BINARY
#define O_BINARY 0
#endif

#endif


#if defined(__cplusplus)

/* The prototype for the call-back function passed
 * to set_new_handler is different on WIN32.
 * The prototype for this function should be coded
 * as:
 *   PIN_NEWHANDLER_RETTYPE function_name(PIN_NEWHANDLER_ARG);
 */

#define PIN_NEWHANDLER_RETTYPE  void
#define PIN_NEWHANDLER_ARG
#define PIN_NEWHANDLER_RET(x) return


#endif

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

/**************************************************
 * System-dependent datatype definition
 ***************************************************/

/* POINTER defines a generic pointer type */
#ifndef _POINTER
  #define _POINTER
typedef unsigned char *POINTER;
#endif /* _POINTER */

/* UINT2 defines a two byte word */
#ifndef _UINT2
  #define _UINT2
typedef unsigned short int UINT2;
#endif /* _UINT2 */

/* UINT4 defines a four byte word */
#ifndef _UINT4
  #define _UINT4
  typedef u_int32 UINT4;
#endif /* _UINT4 */

#if defined (__linux)

#define MASKUPPER 0xFFFF000000000000LL
#define MASKSECOND 0x0000FFFF00000000LL
#define MASKTHIRD 0x00000000FFFF0000LL

#else

#define MASKUPPER 0xFFFF000000000000
#define MASKSECOND 0x0000FFFF00000000
#define MASKTHIRD 0x00000000FFFF0000

#endif

#endif
