/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinBase_H_
#define _PinBase_H_

#ifndef _PinAnyBase_H_
  #include "PinAnyBase.h"
#endif

template < class T, class U >
class PCMCPP_CLASS PinBase : virtual public PinAnyBase {
public:
#if defined(PCMCPP_CONST_SAFE)
  	const U* operator->() const 
	{
		return &m_wrapper;
	}
	const U& operator*() const
	{
		return m_wrapper;
	}
#else
  	U* operator->() const 
	{
		return const_cast< U* >(&m_wrapper);
	}
	U& operator*() const
	{
		return const_cast< U& >(m_wrapper);
	}
#endif
	U* operator->()
	{
		return &m_wrapper;
	}
	U& operator*()
	{
		return m_wrapper;
	}

protected:
	PinBase(T pointee, int owns)
	{
		m_wrapper.grab(pointee, owns);
	}

	// Responsibility on derived classes; provided here 
       // to make copy constructors in derived classes work
       PinBase(const PinBase< T,U > &other)
       {}

	void takeFrom(PinBase< T,U > &other)
	{
		m_wrapper.takeFrom(other.m_wrapper);
	}
	void copyFrom(const PinBase< T,U > &other)
	{
		m_wrapper.copyFrom(other.m_wrapper);
	}
	T release()
	{
	  	return m_wrapper.release();
	}

private:
	PinBase &operator=(PinBase< T,U > &other); // NO IMPL (HIDE)
private:
	U m_wrapper;
public:
	virtual ~PinBase()
	{}

public: //deprecated (preserved for compatibility)
	const U* getWrapperPtr() const
	{	
		return &m_wrapper;
	}

	PinBool isNullWrapperPtr() const
	{	
		return (PIN_BOOLEAN_FALSE);
	}
};

#endif /*_PinBase_H_ */
