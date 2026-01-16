/*
 *      @(#) % %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinObserver_H_
#define _PinObserver_H_

#ifndef    _PinBase_H_
  #include "PinBase.h"
#endif

template < class T, class U >
class PCMCPP_CLASS PinObserver : virtual public PinBase< T, U > {
public:
	PinObserver(const T pointee=0)
		: PinBase< T,U >(pointee, PIN_BOOLEAN_FALSE)
	{}

	PinObserver(const PinObserver< T,U > &other)
	  : PinBase< T,U >(other)
	{
		PinBase<T, U>::copyFrom(other);
	}

	~PinObserver()
	{}

	PinObserver &operator=(const PinObserver< T,U > &other)
	{
		if (this != &other) {
			PinBase< T,U >::copyFrom(other);
		}
		return *this;
	}

	virtual	void discriminate(PinAnyDiscriminator &disc, void *vp);
};


#endif /*_PinObserver_H_*/
