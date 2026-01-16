/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinAnyTypeWrapper_H_
#define _PinAnyTypeWrapper_H_

#ifndef    _PcmCpp_H_
  #include "PcmCpp.h"
#endif

class PCMCPP_CLASS PinAnyTypeWrapper {
public:
	PinBool isNull() const
	{
		return m_pointee == 0;
	}
protected:
	// CONSTRUCTORS
	PinAnyTypeWrapper()
		: m_pointee(0), m_owns(PIN_BOOLEAN_FALSE)
	{}
	~PinAnyTypeWrapper()
	{
		if (m_owns) {
			pin_free(m_pointee);
		}
	}

	// MAIN INTERFACE
	const void* get() const
	{
	  	return m_pointee;
	}
	void* get()
	{
		return m_pointee;
	}
	void grab(void *pointee, int owns)
	{
		if (m_owns) {
			pin_free(m_pointee);
		}

		 m_pointee = pointee;
		 m_owns = owns;
	}
	void takeFrom(PinAnyTypeWrapper &other)
	{
		if (m_owns) {
			pin_free(m_pointee);
		}

		m_owns = other.m_owns;
		m_pointee = other.m_pointee;

		other.m_owns = PIN_BOOLEAN_FALSE;
		other.m_pointee = 0;
	}

	// Used for operator = on observers
	void copyFrom(const PinAnyTypeWrapper &other)
	{
		if (m_owns) {
			pin_free(m_pointee);
		}

		m_owns = PIN_BOOLEAN_FALSE;
		m_pointee = other.m_pointee;
	}
	void* release()
	{
		void *oldpointee = m_pointee;
		m_pointee = 0;
		m_owns = PIN_BOOLEAN_FALSE;
		return oldpointee;
	}
private:
	void	*m_pointee;
	int     m_owns;
};


#endif /*_PinAnyTypeWrapper_H_*/
