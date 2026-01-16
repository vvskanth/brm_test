/* 
 *	Copyright (c) 2023 Oracle.
 * 
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinInt64Wrapper_H_
#define _PinInt64Wrapper_H_

#ifndef   _PinAnyTypeWrapper_H_
  #include "PinAnyTypeWrapper.h"
#endif

extern void throwNullPointerException();

class PCMCPP_CLASS PinInt64Wrapper : public PinAnyTypeWrapper {
public:
#if defined(PCMCPP_CONST_SAFE)
	const int64* get() const
	{
		return static_cast< const int64* >(PinAnyTypeWrapper::get());
	}
#else
	int64* get() const
	{
	  	void *vp = const_cast< void* >(PinAnyTypeWrapper::get());
		return static_cast< int64* >(vp);
	}
#endif
	int64* get()
	{
		return static_cast< int64* >(PinAnyTypeWrapper::get());
	}

#if defined(PCMCPP_CONST_SAFE)
	int64 value() const
	{
  		if (! get()) {
		  throwNullPointerException();
		}
	  	return (*(get()));
	}
#else
	int64 value() const
	{
  		if (! get()) {
		  throwNullPointerException();
		}
	  	return *(const_cast< int64* >(get()));
  	}
#endif
	int64 value()
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
	PinInt64Wrapper()
	{}
	void grab(int64 *pointee, int owns)
	{
	  	PinAnyTypeWrapper::grab(pointee, owns);
	}
	void takeFrom(PinInt64Wrapper &other)
	{
	  	PinAnyTypeWrapper::takeFrom(other);
	}
	void copyFrom(const PinInt64Wrapper &other)
	{
		PinAnyTypeWrapper::copyFrom(other);
	}
	int64* release()
	{
		return static_cast<int64*>(PinAnyTypeWrapper::release());
	}
};

#endif /* _PinInt64Wrapper_H_ */
