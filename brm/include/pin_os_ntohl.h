/*
 *      @(#) % %
 *
 *      Copyright (c) 1996 - 2023 Oracle.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef PIN_OS_NTOHL_H
#define PIN_OS_NTOHL_H

#ifndef _pin_os_types_h_
  #include "pin_os_types.h"
#endif

  /* Unix */
  #include <sys/types.h>
  #include <netinet/in.h>
  #include <inttypes.h>

#if defined(__linux)
  inline int64
  htonll(int64 old)
  {
    const int32 poid_id1 = htonl(*(const int32 *)(&old));
    const int32 poid_id2 = htonl(*((const int32 *)(&old) + 1));
    int64 temp;
    *(int32*)(&temp) = poid_id2;
    *((int32*)(&temp) + 1)= poid_id1;
    return temp;
  }

  inline int64
  ntohll(int64 old)
  {
    const int32 poid_id1 = ntohl(*(const int32 *)(&old));
    const int32 poid_id2 = ntohl(*((const int32 *)(&old) + 1));
    int64 temp;
    *(int32*)(&temp) = poid_id2;
    *((int32*)(&temp) + 1)= poid_id1;
    return temp;
  }
#else
  #ifndef htonll
    #define htonll(a) a
  #endif
  #ifndef ntohll
    #define ntohll(a) a
  #endif
#endif

#endif
