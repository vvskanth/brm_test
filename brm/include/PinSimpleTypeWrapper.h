/*
 * 	@(#) % %
 *
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 * 
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinSimpleTypeWrapper_H_
#define _PinSimpleTypeWrapper_H_

#ifndef    _PinAnyTypeWrapper_H_
  #include "PinAnyTypeWrapper.h"
#endif


// The template init code requires the body for value() method below.
// And that requires the definition of PinDeterminateExc. Define a helper 
// in order to resolve circular dependency between PinExc.h, 
// PinTypeHandle.h which gives the definition of PinFlistOwner.
// Admittedly, a hack.
// 
extern void throwNullPointerException();

template < class T >
class PCMCPP_CLASS PinSimpleTypeWrapper : public PinAnyTypeWrapper {
public:
#if defined(PCMCPP_CONST_SAFE)
	const T* get() const
	{
		return static_cast< const T* >(PinAnyTypeWrapper::get());
	}
#else
	T* get() const
	{
	  	void *vp = const_cast< void* >(PinAnyTypeWrapper::get());
		return static_cast< T* >(vp);
	}
#endif
	T* get()
	{
		return static_cast< T* >(PinAnyTypeWrapper::get());
	}

#if defined(PCMCPP_CONST_SAFE)
	const T value() const
	{
	    const T* val = get();
	    if (! val)
	    {
		throwNullPointerException();
	    }
	    return (*val);
	}
#else
	T value() const
	{
	    const T* val = get();
	    if (!val)
	    {
		throwNullPointerException();
	    }
	    return (*(const_cast< T* >(val)));
	}
#endif
	T value()
	{
	    T* val = get();
	    if (!val)
	    {
		throwNullPointerException();
	    }
	    return (*val);
	}

public:
	// The following methods are for use by PinAnyBase<T,U> only.
	// Ideally, without templates, these would be either embededded inside
	// PinAnyBase or this would declare PinAnyBase to be a friend.
	PinSimpleTypeWrapper()
	{}

	void grab(T* pointee, int owns)
	{
	  	PinAnyTypeWrapper::grab(pointee, owns);
	}

	void takeFrom(PinSimpleTypeWrapper< T > &other)
	{
		PinAnyTypeWrapper::takeFrom(other);
	}
	void copyFrom(const PinSimpleTypeWrapper< T > &other)
	{
		PinAnyTypeWrapper::copyFrom(other);
	}
	T* release()
	{
		return static_cast< T* >(PinAnyTypeWrapper::release());
	}
};
	
#endif /* _PinSimpleTypeWrapper_H_ */
