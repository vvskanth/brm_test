/*
 *	@(#) % %
 * 
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 * 
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinTypeHandle_H_
#define _PinTypeHandle_H_

#ifndef    _PinObserver_H_
  #include "PinObserver.h"
#endif
#ifndef    _PinObserverConst_H_
  #include "PinObserverConst.h"
#endif
#ifndef    _PinOwner_H_
  #include "PinOwner.h"
#endif
#ifndef    _PinSimpleTypeWrapper_H_
  #include "PinSimpleTypeWrapper.h"
#endif
#ifndef    _PinSimplePtrTypeWrapper_H_
  #include "PinSimplePtrTypeWrapper.h"
#endif
#ifndef    _PinEnumWrapper_H_
  #include "PinEnumWrapper.h"
#endif
#ifndef    _PinTstampWrapper_H_
  #include "PinTstampWrapper.h"
#endif
#ifndef    _PinInt64Wrapper_H_
  #include "PinInt64Wrapper.h"
#endif

typedef PinSimpleTypeWrapper<  PinInt > 		PinIntWrapper;
typedef PinBase< PinInt*, PinIntWrapper > 		PinIntBase;
typedef PinObserver< PinInt*, PinIntWrapper >		PinIntObserver;
typedef PinOwner< PinInt*, PinIntWrapper >		PinIntOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< PinInt*, PinIntWrapper > 	PinIntObserverConst;
#else
  typedef PinIntObserver 	PinIntObserverConst;
#endif

typedef PinSimpleTypeWrapper< PinUint >   		PinUintWrapper;
typedef PinBase< PinUint*, PinUintWrapper >		PinUintBase;
typedef PinObserver< PinUint*, PinUintWrapper > 	PinUintObserver;
typedef PinOwner< PinUint*, PinUintWrapper > 		PinUintOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< PinUint*, PinUintWrapper > 	PinUintObserverConst;
#else
  typedef PinUintObserver 	PinUintObserverConst;
#endif

typedef PinSimpleTypeWrapper< PinNum >    		PinNumWrapper;
typedef PinBase< PinNum*, PinNumWrapper >		PinNumBase;
typedef PinObserver< PinNum*, PinNumWrapper > 		PinNumObserver;
typedef PinOwner< PinNum*, PinNumWrapper > 		PinNumOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< PinNum*, PinNumWrapper >     	PinNumObserverConst;
#else
  typedef PinNumObserver 	PinNumObserverConst;
#endif

typedef PinSimplePtrTypeWrapper< PinStr >    		PinStrWrapper;
typedef PinBase< PinStr, PinStrWrapper >		PinStrBase;
typedef PinObserver< PinStr, PinStrWrapper > 		PinStrObserver;
typedef PinOwner< PinStr, PinStrWrapper > 		PinStrOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< PinStr, PinStrWrapper > 	PinStrObserverConst;
#else
  typedef PinStrObserver 	PinStrObserverConst;
#endif

typedef PinSimplePtrTypeWrapper< PinBinstr >    	PinBinstrWrapper;
typedef PinBase< PinBinstr, PinBinstrWrapper >		PinBinstrBase;
typedef PinObserver< PinBinstr,PinBinstrWrapper > 	PinBinstrObserver;
typedef PinOwner< PinBinstr, PinBinstrWrapper > 	PinBinstrOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< PinBinstr,PinBinstrWrapper > 	PinBinstrObserverConst;
#else
  typedef PinBinstrObserver 	PinBinstrObserverConst;
#endif

typedef PinBase< PinEnum*, PinEnumWrapper >		PinEnumBase;
typedef PinObserver< PinEnum*, PinEnumWrapper > 	PinEnumObserver;
typedef PinOwner< PinEnum*, PinEnumWrapper > 		PinEnumOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< PinEnum*, PinEnumWrapper > 	PinEnumObserverConst;
#else
  typedef PinEnumObserver 	PinEnumObserverConst;
#endif

typedef PinBase< PinTstamp*, PinTstampWrapper >		PinTstampBase;
typedef PinObserver< PinTstamp*,PinTstampWrapper > 	PinTstampObserver;
typedef PinOwner< PinTstamp*, PinTstampWrapper > 	PinTstampOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< PinTstamp*,PinTstampWrapper > PinTstampObserverConst;
#else
  typedef PinTstampObserver 	PinTstampObserverConst;
#endif

typedef PinBase< int64*, PinInt64Wrapper >		PinInt64Base;
typedef PinObserver< int64*,PinInt64Wrapper > 	PinInt64Observer;
typedef PinOwner< int64*, PinInt64Wrapper > 	PinInt64Owner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< int64*,PinInt64Wrapper > PinInt64ObserverConst;
#else
  typedef PinInt64Observer 	PinInt64ObserverConst;
#endif

class PinBigDecimal;
typedef PinBase< pin_decimal_t*, PinBigDecimal >	PinBigDecimalBase;
typedef PinObserver< pin_decimal_t*, PinBigDecimal > 	PinBigDecimalObserver;
typedef PinOwner< pin_decimal_t*, PinBigDecimal > 	PinBigDecimalOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< pin_decimal_t*, PinBigDecimal > 
						PinBigDecimalObserverConst;
#else
  typedef PinBigDecimalObserver 	PinBigDecimalObserverConst;
#endif

class PinPoid;
typedef PinBase< poid_t*, PinPoid >			PinPoidBase;
typedef PinObserver< poid_t*, PinPoid > 		PinPoidObserver;
typedef PinOwner< poid_t*, PinPoid > 			PinPoidOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< poid_t*, PinPoid > 		PinPoidObserverConst;
#else
  typedef PinPoidObserver 	PinPoidObserverConst;
#endif

class PinFlist;
typedef PinBase< pin_flist_t*, PinFlist >	       	PinFlistBase;
typedef PinObserver< pin_flist_t*, PinFlist > 		PinFlistObserver;
typedef PinOwner< pin_flist_t*, PinFlist > 		PinFlistOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< pin_flist_t*, PinFlist > 	PinFlistObserverConst;
#else
  typedef PinFlistObserver 	PinFlistObserverConst;
#endif

class PinContext;
typedef PinBase< pcm_context_t*, PinContext >		PinContextBase;
typedef PinObserver< pcm_context_t*, PinContext > 	PinContextObserver;
typedef PinOwner< pcm_context_t*, PinContext > 		PinContextOwner;

class PinErrorBuf;
typedef PinBase< pin_errbuf_t*, PinErrorBuf >		PinErrorBufBase;
typedef PinObserver< pin_errbuf_t*, PinErrorBuf > 	PinErrorBufObserver;
typedef PinOwner< pin_errbuf_t*, PinErrorBuf > 		PinErrorBufOwner;
#if defined(PCMCPP_CONST_SAFE)
  typedef PinObserverConst< pin_errbuf_t*, PinErrorBuf > 	
			PinErrorBufObserverConst;
#else
  typedef PinErrorBufObserver 	PinErrorBufObserverConst;
#endif

#endif /* _PinTypeHandle_H_ */
