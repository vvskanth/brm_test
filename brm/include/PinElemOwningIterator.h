/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinElemOwningIterator_H_
#define _PinElemOwningIterator_H_

#ifndef    _PinIterator_H_
  #include "PinIterator.h"
#endif
#ifndef    _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif

class PCMCPP_CLASS PinElemOwningIterator : public PinIterator {
public:
	PinElemOwningIterator()
	{}

	~PinElemOwningIterator()
	{}
	
	PinElemOwningIterator(const PinElemOwningIterator &i)
		: PinIterator(i)
	{}

	PinElemOwningIterator& operator = (const PinElemOwningIterator &other)
	{
		PinIterator::operator=(other);
		return *this;
	}

	PinFlistOwner	next();
	
private:
	friend class PinFlist;
	PinElemOwningIterator(pin_flist_t *flistp,
			      u_int32 fldnum, int count, 
			      PinBool optional=PIN_BOOLEAN_FALSE);
};

#endif /* _PinElemOwningIterator_H_ */
