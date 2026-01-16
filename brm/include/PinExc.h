//
//	@(#)%Portal Version: PinExc.h:PlatformR2Int:3:2006-Sep-12 03:33:37 %
//	
//	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
//	
//	This material is the confidential property of Oracle Corporation or its
//	licensors and may be used, reproduced, stored or transmitted only in
//	accordance with a valid Oracle license or sublicense agreement.
//
//=============================================================================
// Change Log
//-----------------------------------------------------------------------------
// Date         Name                Description
// -----------  ------------------  -------------------------------------------
// 10-Feb-04    Giles Douglas       Added output operators
// 28-May-04    Steve Schow         exported dummyFlist
// 09-Jun-04    Steve Schow         exported dummyFlist with new macro properly
//=============================================================================

#ifndef _PinExc_H_
#define _PinExc_H_

#ifndef    _PinErrorBuf_H_
  #include "PinErrorBuf.h"
#endif
#ifndef    _PcmCpp_H_
  #include "PcmCpp.h"
#endif
#ifndef    _PinBase_H_
  #include "PinBase.h"
#endif

class PinEbufExc {
public:
        u_int32 errClass() const
	{
		return m_ebuf.pin_errclass;
	}
        u_int32 location() const
	{
		return m_ebuf.location;
	}
        u_int32 errNum()  const
	{
		return m_ebuf.pin_err;
	}
        u_int32 field() const
	{
		return m_ebuf.field;
	}
        u_int32 recId() const
	{
		return m_ebuf.rec_id;
	}
        u_int32 reserved() const
	{
		return m_ebuf.reserved;
	}

        void copyInto(pin_errbuf_t *ebufp) const
        {
                if (ebufp != NULL) {
                	PinErrorBuf::copy(&m_ebuf, ebufp);
                }
	}

        void copyInto(PinErrorBuf *ebufp) const
	{
                copyInto( (pin_errbuf_t*) ebufp );
        }

        //
	// The intention is to create the derived subtype of PinEbufEcv
	// and catch PinEbufExc&. But unfortunately, the compiler misbehaves
	// (on NT at least) if the  constructors in this base class are made
	// protected....sigh...	
	//
        PinEbufExc(const pin_errbuf_t *ebufp) : m_ebuf()
	{
                PinErrorBuf::copy(ebufp, &m_ebuf);
	}

        PinEbufExc(const PinErrorBuf *ebufp) : m_ebuf()
        {
                PinErrorBuf::copy(ebufp, &m_ebuf);
        }

        PinEbufExc(const pin_errbuf_t *ebufp, PinFlistOwner &flist)
                : m_flist(flist), m_ebuf()
	{
		PinErrorBuf::copy(ebufp, &m_ebuf);

	}

        PinEbufExc(const PinErrorBuf *ebufp, PinFlistOwner &flist)
                : m_flist(flist), m_ebuf()
        {
                PinErrorBuf::copy(ebufp, &m_ebuf);
        }

	PinEbufExc(const PinEbufExc &other)
		: m_flist(const_cast<PinFlistOwner &>(other.m_flist)), m_ebuf()
	{
		PinErrorBuf::copy(&other.m_ebuf, &m_ebuf);
	}
	
	PinEbufExc(u_int32 a_errclass, u_int32 a_location, u_int32 a_pin_err,
		   u_int32 a_field, u_int32 a_rec_id, u_int32 a_reserved, 
		   PinFlistOwner &a_flist)
		: m_flist(a_flist),
		  m_ebuf()
	{
		m_ebuf.location 	= a_location;
		m_ebuf.pin_errclass	= a_errclass;
		m_ebuf.pin_err 		= a_pin_err;
		m_ebuf.field 		= a_field;
		m_ebuf.rec_id 		= a_rec_id;
		m_ebuf.reserved 	= a_reserved;
	}
	
	PinFlistBase& getFlistRef() {
		return m_flist;
	}

protected:

	void operator = (const PinEbufExc &other);
	   
        PinErrorBuf     m_ebuf;
	PinFlistOwner	m_flist;
	
	friend class PCMCPP_CLASS PinLog;
};

#ifndef PIN_NOT_USING_OSTREAM
PCMCPP_API std::ostream& operator<<(std::ostream& os,
				    const PinEbufExc& ebuf);
#endif

class PinDeterminateExc : public PinEbufExc {
public:
	PinDeterminateExc(const pin_errbuf_t *ebufp) 
		: PinEbufExc(ebufp)
	{}

        PinDeterminateExc(const PinErrorBuf *ebufp)
                : PinEbufExc(ebufp)
        {}

	PinDeterminateExc(u_int32 location, u_int32 pin_err,
			  u_int32 field = 0, u_int32 rec_id = 0,
			  u_int32 reserved = 0, 
			  PinFlistOwner flist = dummyFlist)
		: PinEbufExc(PIN_ERRCLASS_SYSTEM_DETERMINATE,
			     location, pin_err, field, rec_id, reserved,flist)
	{}

	PinDeterminateExc(const PinDeterminateExc& other)
		: PinEbufExc(other)
	{}

	PinDeterminateExc(const pin_errbuf_t *ebufp,
			  PinFlistOwner &flist)
		: PinEbufExc(ebufp, flist)
	{}

        PinDeterminateExc(const PinErrorBuf *ebufp,
                          PinFlistOwner &flist)
                : PinEbufExc(ebufp, flist)
        {}

private:
	void operator = (const PinEbufExc &other);
	PCMCPP_API_EXPORT static PinFlistOwner dummyFlist;
};

#endif /*_PinExc_H_ */

