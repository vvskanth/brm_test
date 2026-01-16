//
//	@(#)%Portal Version: PcmCpp.h:PlatformR2Int:4:2006-Sep-12 03:33:06 %
//	
//	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
//	
//	This material is the confidential property of Oracle Corporation or its
//	licensors and may be used, reproduced, stored or transmitted only in
//	accordance with a valid Oracle license or sublicense agreement.
//

#ifndef _PcmCpp_H_
#define _PcmCpp_H_

extern "C" {
#include <memory.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#ifndef _PCM_H
  #include "pcm.h"
#endif
#ifndef _PINLOG_H
  #include "pinlog.h"
#endif
#ifndef _PIN_FLDS_H
  #include <pin_flds.h>
#endif
#ifndef _PIN_TYPE_H_
  #include "pin_type.h"
#endif
}

#ifdef PCMCPP_DLL
  #define PCMCPP_API            PIN_EXPORT
  #define PCMCPP_API_EXPORT     PIN_EXPORT
  #define PCMCPP_CLASS          PCMCPP_API
#else /*PCMCPP_DLL*/
  #define PCMCPP_API            PIN_IMPORT
  #ifdef WINDOWS
    #define PCMCPP_API_EXPORT   PIN_IMPORT
  #else /*WINDOWS*/
    #define PCMCPP_API_EXPORT
  #endif
  #define PCMCPP_CLASS
#endif /*PCMCPP_DLL*/

#ifdef WINDOWS
  #ifdef PCMCPP_STATIC_LIB
    #undef PCMCPP_API
    #undef PCMCPP_API_EXPORT
    #undef PCMCPP_CLASS
    #define PCMCPP_API
    #define PCMCPP_API_EXPORT
    #define PCMCPP_CLASS
  #endif /*PCMCPP_STATIC_LIB*/
  #define PCMCPP_API_EXTERN     extern PCMCPP_API
#else /*WINDOWS*/
  #define PCMCPP_API_EXTERN     extern
#endif /*WINDOWS*/


typedef int32                   PinInt;
typedef u_int32                 PinUint;
typedef int32                   PinEnum;
typedef double                  PinNum;
typedef char*                   PinStr;
typedef const char*             PinStrConst;
typedef pin_fld_tstamp_t        PinTstamp;
typedef pin_binstr_t*           PinBinstr;
typedef pin_binstr_t            PinBinstrStruct;

typedef pin_db_no_t             PinPoidDb;
typedef pin_poid_id_t           PinPoidId;
typedef pin_const_poid_type_t   PinPoidTypeConst;
typedef pin_const_poid_type_t   PinPoidType;
typedef pin_poid_rev_t          PinPoidRev;

typedef pin_rec_id_t            PinRecId;

typedef int                     PinBool;

typedef pin_opcode_t            PinOp;
typedef u_int32                 PinOpFlags;     // BitClass later

typedef u_int32                 PinTransFlags;  // BitClass later

#endif /*_PcmCpp_H_ */
