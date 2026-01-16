/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinAnyDiscriminator_H_
#define _PinAnyDiscriminator_H_

#ifndef    _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif

class PCMCPP_CLASS PinAnyDiscriminator {
public:
	virtual void onInt(PinIntBase &, void *) = 0;
	virtual void onInt64(PinInt64Base &, void *) = 0;
	virtual void onUint(PinUintBase &, void *) = 0;
	virtual void onEnum(PinEnumBase &, void *) = 0;
	virtual void onNum(PinNumBase &, void *) = 0;
	virtual void onStr(PinStrBase &, void *) = 0;
	virtual void onPoid(PinPoidBase &, void *) = 0;
	virtual void onBigDecimal(PinBigDecimalBase &, void *) = 0;
	virtual void onTstamp(PinTstampBase &, void *) = 0;
	virtual void onFlist(PinFlistBase &, void *) = 0;
	virtual void onBinstr(PinBinstrBase &, void *) = 0;

//	virtual void onBuf(PinBufBase &, void *) = 0;
	virtual void onErrorBuf(PinErrorBufBase &, void *) = 0;
	virtual void onUnknown(void *) = 0;
};

#endif /* _PinAnyDiscriminator_H_ */
