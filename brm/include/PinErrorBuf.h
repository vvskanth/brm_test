//
// 	(#)%Portal Version: PinErrorBuf.h:PlatformR2Int:3:2006-Jul-16 15:38:04 %
// 
//	Copyright (c) 1996, 2023, Oracle and/or its affiliates.
// 
//	This material is the confidential property of Oracle Corporation or its
//	licensors and may be used, reproduced, stored or transmitted only in
//	accordance with a valid Oracle license or sublicense agreement.
//

#ifndef _PinErrorBuf_H_
#define _PinErrorBuf_H_

#ifndef _PcmCpp_H_
  #include "PcmCpp.h"
#endif
#ifndef _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif

#ifndef PIN_NOT_USING_OSTREAM
#ifndef _RWSTD_IOSTREAM_INCLUDED
  #include <iostream>
#endif
#endif


//////////////////////////////////////////////////////////////////////
// PinErrorBuf
//////////////////////////////////////////////////////////////////////

class PCMCPP_CLASS PinErrorBuf : public pin_errbuf_t
{

// Construction/destruction
public:
        PinErrorBuf();
// Attributes
public:
        static PinErrorBufObserver createAsObserved(pin_errbuf_t* pval);
        static PinErrorBufOwner    createAsOwned(pin_errbuf_t* pval);

	int32         getLocation() const;
        int32         getErrClass() const;
        int32         getError() const;
        int32         getField() const;
        int32         getRecordID() const;
        int32         getReserved() const;
        int32         getLineNo() const;
        const char*   getFilename() const;
        pin_fld_num_t getFacility() const;
        int32         getMessageID() const;
        time_t        getTimeSec() const;
        time_t        getTimeUSec() const;
        int32         getVersion() const;
        pin_errbuf_t* getNextBufPtr() const;
        int32         getReserved2() const;


	PinErrorBuf(const PinErrorBuf&, PinBool owns = PIN_BOOLEAN_TRUE);
	PinErrorBuf(const char* , int32);

	/** This makes a copy of the ebuf. Not the usual use-case.
	 * @deprecated Use createAsObserved() or createAsOwned() instead.
	 */
	PinErrorBuf(const pin_errbuf_t&, PinBool owns = PIN_BOOLEAN_TRUE);

	/** This makes a copy of the ebuf. Not the usual use-case.
	 * @deprecated Use createAsObserved() instead.
	 */
	PinErrorBuf(const pin_errbuf_t*, PinBool owns = PIN_BOOLEAN_TRUE);

	void grab(pin_errbuf_t* pointee, PinBool owns);
	void takeFrom(PinErrorBuf &other);
	void copyFrom(const PinErrorBuf &other);

	// This can't be virtual because the base class doesn't have a virtual
	// destructor!!
        ~PinErrorBuf();


private:
	PinBool m_owns;

// Operations
public:
#if defined(PCMCPP_CONST_SAFE)
	const pin_errbuf_t* get() const;
#else
	pin_errbuf_t* get() const;
#endif
	pin_errbuf_t* get();

        PinErrorBuf&	operator=(const PinErrorBuf&);
	void			clear();
	void			Clear();
	static pin_errbuf_t*	copy(const pin_errbuf_t *srcp, pin_errbuf_t *destp);
};

#ifndef PIN_NOT_USING_OSTREAM
PCMCPP_API std::ostream& operator<<(std::ostream& os,
				    const PinErrorBuf& ebuf);
#endif


//////////////////////////////////////////////////////////////////////
// PinErrorBuf Inline Implementation
//////////////////////////////////////////////////////////////////////

inline int32
PinErrorBuf::getLocation() const
{
        return location;
}

inline int32
PinErrorBuf::getErrClass() const
{
        return pin_errclass;
}

inline int32
PinErrorBuf::getError() const
{
        return pin_err;
}

inline int32 PinErrorBuf::getField() const
{
        return field;
}

inline int32 PinErrorBuf::getRecordID() const
{
        return rec_id;
}

inline int32 PinErrorBuf::getReserved() const
{
        return reserved;
}

inline int32 PinErrorBuf::getLineNo() const
{
        return line_no;
}

inline const char* PinErrorBuf::getFilename() const
{
        return filename;
}

inline pin_fld_num_t PinErrorBuf::getFacility() const
{
        return facility;
}

inline int32 PinErrorBuf::getMessageID() const
{
        return msg_id;
}

inline time_t PinErrorBuf::getTimeSec() const
{
        return err_time_sec;
}

inline time_t PinErrorBuf::getTimeUSec() const
{
        return err_time_usec;
}

inline int32 PinErrorBuf::getVersion() const
{
        return version;
}

inline pin_errbuf_t* PinErrorBuf::getNextBufPtr() const
{
        return nextp;
}

inline int32 PinErrorBuf::getReserved2() const
{
        return reserved2;
}

inline void PinErrorBuf::Clear() 
{
  	clear();
}

#if defined(PCMCPP_CONST_SAFE)
inline const pin_errbuf_t*
PinErrorBuf::get() const
{
    return this;
}
#else
inline pin_errbuf_t*
PinErrorBuf::get() const
{
    return const_cast< PinErrorBuf* >(this);
}
#endif

inline pin_errbuf_t*
PinErrorBuf::get()
{
    return this;
}

#endif
