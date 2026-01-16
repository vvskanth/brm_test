/*
 *      @(#) % %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinObserverConst_H_
#define _PinObserverConst_H_

#ifndef    _PinBase_H_
  #include "PinBase.h"
#endif

template < class T, class U >
class PCMCPP_CLASS PinObserverConst : virtual public PinBase< T,U > {
public:
	PinObserverConst(const T pointee=0)
		: PinBase< T,U >(pointee, PIN_BOOLEAN_FALSE)
	{}

	PinObserverConst(const PinObserverConst< T,U > &other)
		: PinBase< T,U >(other)
	{
		PinBase< T, U >::copyFrom(other);
	}

	PinObserverConst(const PinObserver< T,U > &other)
		: PinBase< T,U >(other)
	{
		PinBase< T, U >::copyFrom(other);
	}

	~PinObserverConst()
	{}

	PinObserver< T,U >& unconst() const
	{
		return *this;
	}

	const U* operator->() const
	{
		return PinBase< T,U >::operator->();
	}
	const U& operator*() const
	{
		return PinBase< T,U >::operator*();
	}

	const U* operator->()
	{
		return PinBase< T,U >::operator->();
	}
	const U& operator*()
	{
		return PinBase< T,U >::operator*();
	}

	PinObserverConst &operator=(const PinObserverConst< T,U > &other)
	{
		if (this != &other) {
			PinBase< T,U >::copyFrom(other);
		}
		return *this;
	}

	virtual	void discriminate(PinAnyDiscriminator &disc, void *vp);
};

#endif /*_PinObserverConst_H_*/
