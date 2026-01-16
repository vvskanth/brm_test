/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinElemDroppingIterator_H_
#define _PinElemDroppingIterator_H_

#ifndef    _PinIterator_H_
  #include "PinIterator.h"
#endif
#ifndef    _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif

class PCMCPP_CLASS PinElemDroppingIterator : public PinIterator {
public:
	PinElemDroppingIterator()
	{ m_prevCookie = NULL; }

	~PinElemDroppingIterator()
	{}
	
	PinElemDroppingIterator(const  PinElemDroppingIterator &i);

	PinElemDroppingIterator& operator=(
		  const PinElemDroppingIterator &other);

	PinFlistObserver next();

	void drop(PinArrayTypeField fldNum, PinRecId recId);
	
private:
	friend class PinFlist;
	PinElemDroppingIterator(  pin_flist_t *flistp,
				   const u_int32 fldnum,   const int count, 
				   const PinBool optional=PIN_BOOLEAN_FALSE);

	pin_cookie_t	m_prevCookie;
};

#endif /* _PinElemDroppingIterator_H_ */
