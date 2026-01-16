/*
 *      @(#) % %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinIterator_H_
#define _PinIterator_H_

#ifndef    _PcmCpp_H_
  #include "PcmCpp.h"
#endif
#ifndef    _PinErrorBuf_H_
  #include "PinErrorBuf.h"
#endif

class PCMCPP_CLASS PinIterator {
public:
	virtual ~PinIterator()
	{}

	PinBool	hasMore() const;
	void 	reset();

	pin_fld_num_t getFieldNum() const
	{
		return m_fldnum;
	}

	u_int32	getFieldType() const
	{
		return PIN_GET_TYPE_FROM_FLD(m_fldnum);
	}

	PinRecId getRecId() const
	{
		return m_recid;
	}

protected:
	PinIterator();
	PinIterator(const PinIterator &other);
	void operator =(const PinIterator &other);

	pin_cookie_t		m_cookie;
	PinRecId		m_recid;
        int32                   m_fldnum;
	PinBool			m_optional;
	int			m_max;
	int			m_curr;
	pin_flist_t		*m_flistp;
};

#endif /* _PinIterator_H_ */
