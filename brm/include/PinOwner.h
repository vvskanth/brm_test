/*
 * 	@(#) % %
 *
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 * 
 *	This material is the confidential property of Oracle Corporation or its
 * 	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinOwner_H_
#define _PinOwner_H_

#ifndef    _PinBase_H_
  #include "PinBase.h"
#endif

template < class T, class U >
class PCMCPP_CLASS PinOwner : virtual public PinBase< T,U > {
public:
	PinOwner(const T pointee=0)
		: PinBase< T,U >(pointee, PIN_BOOLEAN_TRUE)
	{}
	PinOwner(const PinOwner< T,U > &other)
		: PinBase< T,U >(other)
	{
		PinOwner< T,U > &o = const_cast< PinOwner< T,U > &>(other);
		if (this != &o) {
			PinBase< T,U >::takeFrom(o);
		}
	}
	PinOwner &operator=(const PinOwner< T,U > &other)
	{
		PinOwner< T,U > &o = const_cast< PinOwner< T,U > &>(other);
		
		if (this != &o) {
			PinBase< T,U >::takeFrom(o);
		}
		return *this;
	}
	~PinOwner()
	{}
	T release()
	{
		return PinBase< T,U >::release();
	}

	virtual	void discriminate(PinAnyDiscriminator &disc, void *vp);
};

#endif /*_PinOwner_H_*/
