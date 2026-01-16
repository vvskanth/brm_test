/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinElemObservingIteratorConst_H_
#define _PinElemObservingIteratorConst_H_

#ifndef    _PinIterator_H_
  #include "PinIterator.h"
#endif
#ifndef    _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif

class PCMCPP_CLASS PinElemObservingIteratorConst : public PinIterator {
public:
	PinElemObservingIteratorConst()
	{}

	~PinElemObservingIteratorConst()
	{}
	
	PinElemObservingIteratorConst(const PinElemObservingIteratorConst &)
	{}

	PinElemObservingIteratorConst& operator = (
		const PinElemObservingIteratorConst &other)
	{
		PinIterator::operator=(other);
		return *this;
	}

	PinFlistObserverConst next() const;
	
private:
	friend class PinFlist;
	PinElemObservingIteratorConst(const pin_flist_t *flistp,
				 const u_int32 fldnum, const int count, 
				 const PinBool optional=PIN_BOOLEAN_FALSE);
};

#endif /* _PinElemObservingIteratorConst_H_ */
