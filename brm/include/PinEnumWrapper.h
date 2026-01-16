/*
 *      @(#)%Portal Version: PinEnumWrapper.h:RWSmod7.3.1Int:1:2007-Sep-12 23:54:16 %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinEnumWrapper_H_
#define _PinEnumWrapper_H_

#ifndef    _PcmCpp_H_
  #include "PcmCpp.h"
#endif
#ifndef    _PinAnyTypeWrapper_H_
  #include "PinAnyTypeWrapper.h"
#endif

extern void throwNullPointerException();

class PCMCPP_CLASS PinEnumWrapper : public PinAnyTypeWrapper {
public:
#if defined(PCMCPP_CONST_SAFE)
	const PinEnum* get() const
	{
		return static_cast< const PinEnum* >(PinAnyTypeWrapper::get());
	}
#else
	PinEnum* get() const
	{
	  	void *vp = const_cast< void* >(PinAnyTypeWrapper::get());
		return static_cast< PinEnum* >(vp);
	}
#endif

	PinEnum* get()
	{
	  	return static_cast< PinEnum* >(PinAnyTypeWrapper::get());
	}

#if defined(PCMCPP_CONST_SAFE)
	PinEnum value() const
	{
  		if (! get()) {
		  throwNullPointerException();
		}
	  	return (*(get()));
	}
#else
	PinEnum value() const
	{
  		if (! get()) {
		  throwNullPointerException();
		}
	  	return *(const_cast< PinEnum* >(get()));
  	}
#endif
	PinEnum value()
	{
  		if (! get()) {
		  throwNullPointerException();
		}
	  	return (*(get()));
  	}

public:	
	// The following methods are for use by PinAnyBase<T,U> only.
	// Ideally, without templates, these would be either embededded inside
	// PinAnyBase or this would declare PinAnyBase to be a friend.
	PinEnumWrapper()
	{}
	void grab(PinEnum *pointee, int owns)
	{
	  	PinAnyTypeWrapper::grab(pointee, owns);
	}
	void takeFrom(PinEnumWrapper &other)
	{
		PinAnyTypeWrapper::takeFrom(other);
	}
	void copyFrom(const PinEnumWrapper &other)
	{
		PinAnyTypeWrapper::copyFrom(other);
	}
	PinEnum* release()
	{
		return static_cast<PinEnum*>(PinAnyTypeWrapper::release());
	}
};

#endif /* _PinEnumWrapper_H_ */
