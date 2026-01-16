/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinAnyObservingIterator_H_
#define _PinAnyObservingIterator_H_

#ifndef    _PinIterator_H_
  #include "PinIterator.h"
#endif
#ifndef    _PinAnyBase_H_
  #include "PinAnyBase.h"
#endif

class PCMCPP_CLASS PinAnyObservingIterator : public PinIterator {
public:
	PinAnyObservingIterator()
		: m_vp(NULL)
	{}

	~PinAnyObservingIterator()
	{}
	
	PinAnyObservingIterator(const PinAnyObservingIterator &other)
		: m_vp(other.m_vp)
	{}

	PinAnyObservingIterator& operator =(
		const PinAnyObservingIterator &other)
	{
		PinIterator::operator=(other);
		m_vp = other.m_vp;
		return *this;
	}

	
	PinAnyBase* 	next();

	void		advance();
	PinAnyBase*	get() const;
	
private:
	friend class PinFlist;
	PinAnyObservingIterator(pin_flist_t *flistp, int count);

	void		*m_vp;
};


#endif /* _PinAnyObservingIterator_H_ */
