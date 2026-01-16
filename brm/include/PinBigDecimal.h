/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

/*
 * Notes: Here's the releationship between the rounding modes used in Gary's
 *        pin_decimal_t API and the BEID rounding modes.
 *
 *        PIN_BEID_ROUND_NEAREST: BEID mode - ROUND_HALF_UP
 *        PIN_BEID_ROUND_UP     : BEID mode - ROUND_UP
 *        PIN_BEID_ROUND_DOWN   : BEID mode - ROUND_DOWN
 *        PIN_BEID_ROUND_EVEN   : BEID mode - ROUND_HALF_EVEN
 */

#ifndef _PINBigDecimal_H_
#define _PINBigDecimal_H_

//////////////////////////////////////////////////////////////////////
// PINBigDecimal.h: Interface for the PinBigDecimal class.
//////////////////////////////////////////////////////////////////////

#include <wchar.h>
#ifndef _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif

//////////////////////////////////////////////////////////////////////
// PinBigDecimal
//////////////////////////////////////////////////////////////////////

class PCMCPP_CLASS PinBigDecimal {
// Declarations
public:
        #define DEF_DIV_DECIMAL_PLACES 10
        #define DEF_ROUNDING_MODE      ROUND_HALF_UP
        #define USE_CUR_DEC_PLACES     -1

// Factory Methods
public:
        static PinBigDecimalObserver createAsObserved(const pin_decimal_t* pval);
        static PinBigDecimalOwner    createAsOwned(pin_decimal_t* pval);

// Construction/destruction
public:
        // **Note: You must not use thsee constructors if you are using
        //         owners and observers.
        //
        // **Note: A NULL pointer for a character string or a pin_decimal_t*
        //         will cause a "null" pin_decimal_t to be created.
        //
        PinBigDecimal();
        PinBigDecimal(const char* pval);
        PinBigDecimal(const wchar_t* pval);
        PinBigDecimal(const pin_decimal_t* pval);
        PinBigDecimal(pin_decimal_t* pval, PinBool owns);
        PinBigDecimal(pin_decimal_t* pval, PinBool owns, PinBool for_zero);
        PinBigDecimal(double val, int decimalPlaces,
                      int mode=DEF_ROUNDING_MODE);
        PinBigDecimal(const PinBigDecimal& val);
        PinBigDecimal(const PinBigDecimal* pval);
        ~PinBigDecimal();

// Operations
public:
        // **Note: For the divide operators the number of decimal places
        //         is DEF_DIV_DECIMAL_PLACES and the rounding mode is
        //         DEF_ROUNDING_MODE.
        // **Note: Trying to perform an arithimic operation involving a
        //         pin_decimal_t that contains the "null" value will throw
        //         an exception.
        //
        PinBigDecimal operator+(const PinBigDecimal& val);
        PinBigDecimal operator-(const PinBigDecimal& val);
        PinBigDecimal operator/(const PinBigDecimal& val);
        PinBigDecimal operator*(const PinBigDecimal& val);

        PinBigDecimal& operator+=(const PinBigDecimal& val);
        PinBigDecimal& operator-=(const PinBigDecimal& val);
        PinBigDecimal& operator/=(const PinBigDecimal& val);
        PinBigDecimal& operator*=(const PinBigDecimal& val);

        PinBigDecimal& operator=(const PinBigDecimal* pval);
        PinBigDecimal& operator=(const PinBigDecimal& val);
        PinBigDecimal& operator=(const pin_decimal_t* pval);
        PinBigDecimal& operator=(const char* pval);
        PinBigDecimal& operator=(const wchar_t* pval);

#if defined(PCMCPP_CONST_SAFE)
        operator const pin_decimal_t*() const; // Hack for the PCM C API
#else
        operator pin_decimal_t*() const; // Hack for the PCM C API
#endif
        operator pin_decimal_t*(); // Hack for the PCM C API

        // **Note: The divide() and multiply() methods have the same result
        //         as operator/=() and operator*=().
        //
        PinBigDecimal& divide(const PinBigDecimal& val, int decimalPlaces,
                              int mode = DEF_ROUNDING_MODE);
        PinBigDecimal& multiply(const PinBigDecimal& val, int decimalPlaces,
                                int mode = DEF_ROUNDING_MODE);

        int     compare(const PinBigDecimal& val) const;
        PinBool isZero() const;
        PinBool isLessThanZero() const;
        PinBool isGreaterThanZero() const;
        PinBool isNull() const;

        PinBool operator==(const PinBigDecimal& val) const;
        PinBool operator!=(const PinBigDecimal& val) const;
        PinBool operator<(const PinBigDecimal& val) const;
        PinBool operator<=(const PinBigDecimal& val) const;
        PinBool operator>(const PinBigDecimal& val) const;
        PinBool operator>=(const PinBigDecimal& val) const;

        PinBigDecimal        abs() const;
        PinBigDecimal        negate() const;
        int                  getNumDecimalPlaces() const;
        PinBigDecimal&       setNumDecimalPlaces(int decimalPlaces,
                                                 int mode = DEF_ROUNDING_MODE);
        int                  sigNum();
#if defined(PCMCPP_CONST_SAFE)
        const pin_decimal_t* get() const;  // Use wisely
#else
        pin_decimal_t* get() const;
#endif
        pin_decimal_t* get();
        double               getDouble() const;
        PinBigDecimal&       setDouble(double val, int decimalPlaces,
                                       int mode = DEF_ROUNDING_MODE);
        int64                getInt64() const;
        char*                toString(char* pbuf, int bufSize,
                                      int decimalPlaces = USE_CUR_DEC_PLACES)
                                      const;

        // The following are required by PinBase.
        pin_decimal_t*       release();
        void grab(pin_decimal_t* pointee, PinBool owns);
        void takeFrom(PinBigDecimal &);
        void copyFrom(const PinBigDecimal &);

// Implementation
protected:
        void  constructMemberData(const char* pval);
        void  constructMemberData(const wchar_t* pval);
        void  constructMemberData(const pin_decimal_t* pval);
        void  constructMemberData(double val, int decimalPlaces, int mode);
        void  cleanup();

protected:
        pin_decimal_t* m_pnumber;
        static const PinBigDecimal& zero();
        int m_owns;
};

//////////////////////////////////////////////////////////////////////
// PinBigDecimal Inline Implementation
//////////////////////////////////////////////////////////////////////

inline PinBigDecimal&
PinBigDecimal::operator=(const PinBigDecimal* pval)
{
        constructMemberData(pval->m_pnumber);
        return *this;
}

inline PinBigDecimal&
PinBigDecimal::operator=(const PinBigDecimal& val)
{
        constructMemberData(val.m_pnumber);
        return *this;
}

inline PinBigDecimal&
PinBigDecimal::operator=(const pin_decimal_t* pval)
{
        constructMemberData(pval);
        return *this;
}

inline PinBigDecimal&
PinBigDecimal::operator=(const char* pval)
{
        constructMemberData(pval);
        return *this;
}

inline PinBigDecimal&
PinBigDecimal::operator=(const wchar_t* pval)
{
        constructMemberData(pval);
        return *this;
}

#if defined(PCMCPP_CONST_SAFE)
inline
PinBigDecimal::operator const pin_decimal_t*() const
{
        return get();
}
#else
inline
PinBigDecimal::operator pin_decimal_t*() const
{
        return get();
}
#endif

inline
PinBigDecimal::operator pin_decimal_t*()
{
        return get();
}

inline PinBool
PinBigDecimal::operator==(const PinBigDecimal& val) const
{
        return compare(val) == 0;
}

inline PinBool
PinBigDecimal::operator!=(const PinBigDecimal& val) const
{
        return compare(val) != 0;
}

inline PinBool
PinBigDecimal::operator<(const PinBigDecimal& val) const
{
        return compare(val) == -1;
}

inline PinBool
PinBigDecimal::operator<=(const PinBigDecimal& val) const
{
        return compare(val) <= 0;
}

inline PinBool
PinBigDecimal::operator>(const PinBigDecimal& val) const
{
        return compare(val) == 1;
}

inline PinBool
PinBigDecimal::operator>=(const PinBigDecimal& val) const
{
        return compare(val) >= 0;
}

#if defined(PCMCPP_CONST_SAFE)
inline const pin_decimal_t*
PinBigDecimal::get() const
{
        return m_pnumber;
}
#else
inline pin_decimal_t*
PinBigDecimal::get() const
{
        return m_pnumber;
}
#endif

inline pin_decimal_t*
PinBigDecimal::get()
{
        return m_pnumber;
}
/////////////////////////////////////////////////////////////////////////////

#endif
