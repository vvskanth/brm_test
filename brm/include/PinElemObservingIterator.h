/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinElemObservingIterator_H_
#define _PinElemObservingIterator_H_

#ifndef    _PinIterator_H_
  #include "PinIterator.h"
#endif
#ifndef    _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif

class PCMCPP_CLASS PinElemObservingIterator : public PinIterator {
public:
	PinElemObservingIterator()
	{}

	~PinElemObservingIterator()
	{}
	
	PinElemObservingIterator(const PinElemObservingIterator &i)
	       	: PinIterator(i)
	{}

	PinElemObservingIterator& operator = (
		const PinElemObservingIterator &other)
	{
		PinIterator::operator=(other);
		return *this;
	}

	PinFlistObserver next();
	
private:
	friend class PinFlist;
	PinElemObservingIterator(const pin_flist_t *flistp,
				 const u_int32 fldnum, const int count, 
				 const PinBool optional=PIN_BOOLEAN_FALSE);
};

#endif /* _PinElemObservingIterator_H_ */
