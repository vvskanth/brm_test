/*
 *	@(#)%Portal Version: PinTstampWrapper.h:RWSmod7.3.1Int:1:2007-Sep-12 23:54:12 %
 * 
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 * 
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinTstampWrapper_H_
#define _PinTstampWrapper_H_

#ifndef   _PinAnyTypeWrapper_H_
  #include "PinAnyTypeWrapper.h"
#endif

extern void throwNullPointerException();

class PCMCPP_CLASS PinTstampWrapper : public PinAnyTypeWrapper {
public:
#if defined(PCMCPP_CONST_SAFE)
	const PinTstamp* get() const
	{
		return static_cast< const PinTstamp* >(PinAnyTypeWrapper::get());
	}
#else
	PinTstamp* get() const
	{
	  	void *vp = const_cast< void* >(PinAnyTypeWrapper::get());
		return static_cast< PinTstamp* >(vp);
	}
#endif
	PinTstamp* get()
	{
		return static_cast< PinTstamp* >(PinAnyTypeWrapper::get());
	}

#if defined(PCMCPP_CONST_SAFE)
	PinTstamp value() const
	{
  		if (! get()) {
		  throwNullPointerException();
		}
	  	return (*(get()));
	}
#else
	PinTstamp value() const
	{
  		if (! get()) {
		  throwNullPointerException();
		}
	  	return *(const_cast< PinTstamp* >(get()));
  	}
#endif
	PinTstamp value()
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
	PinTstampWrapper()
	{}
	void grab(PinTstamp *pointee, int owns)
	{
	  	PinAnyTypeWrapper::grab(pointee, owns);
	}
	void takeFrom(PinTstampWrapper &other)
	{
	  	PinAnyTypeWrapper::takeFrom(other);
	}
	void copyFrom(const PinTstampWrapper &other)
	{
		PinAnyTypeWrapper::copyFrom(other);
	}
	PinTstamp* release()
	{
		return static_cast<PinTstamp*>(PinAnyTypeWrapper::release());
	}
};

#endif /* _PinTstampWrapper_H_ */
