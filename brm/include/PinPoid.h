//
//	@(#)%Portal Version: PinPoid.h:PlatformR2Int:5:2006-Sep-12 03:33:54 %
// 
//	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
// 
//	This material is the confidential property of Oracle Corporation or its
//	licensors and may be used, reproduced, stored or transmitted only in
//	accordance with a valid Oracle license or sublicense agreement.
//

#ifndef _PinPoid_H_
#define _PinPoid_H_

#ifndef _PinErrorBuf_H_
  #include "PinErrorBuf.h"
#endif

#ifndef PIN_NOT_USING_OSTREAM
#ifndef _RWSTD_IOSTREAM_INCLUDED
  #include <iostream>
#endif
#endif

/** The PinPoid class manage a C poid pointer. This class is intended to be
 * used via a PinPoidObserver or PinPoidOwner wrapper class.
 */
class PCMCPP_CLASS PinPoid {
public:
	/** @name Public Factory Methods */

	//@{

	/** Create a new Poid and access it via the owner C++ wrapper.
	 * @param db Database number
	 * @param type Storable class type
	 * @param id Instance id
	 * @return PinPoidOwner Instance of the poid owner wrapper
	 */
	static PinPoidOwner 	create(PinPoidDb db, 
				       PinPoidTypeConst type, 
				       PinPoidId id);

	/** Create a C++ observer wrapper for a C-style Poid. When the 
	 * wrapper goes out of scope, the underlying poid will not be deleted.
	 * @param poidp Pointer to the poid to wrap
	 * @return PinPoidObserver Instance of the poid observer wrapper
	 */
	static PinPoidObserver 	createAsObserved(poid_t *pdp);

	static PinPoidObserverConst 	createAsObservedConst(poid_t *pdp);

	/** Create a C++ owner wrapper for a C-style Poid. When the wrapper 
	 * goes  out of scope, the underlying poid will be deleted.
	 * @param poidp Pointer to the poid to wrap
	 * @return PinPoidOwner Instance of the poid owner handle
	 */
	static PinPoidOwner 	createAsOwned(poid_t *pdp);

	//@}

	/** Destructor
	 */
	~PinPoid();

	/** @name Public Member Functions */

	//@{

	/** Get the database piece of the poid.
	 * @return PinPoidDb Database piece of the poid
	 */
	PinPoidDb	getDb() const;

	/** Get the instance id piece of the poid.
	 * @return PinPoidId Instance id piece of the poid
	 */
	PinPoidId	getId() const;

	/** Get the class name piece of the poid.
	 * @return PinPoidDb Class name piece of the poid
	 */
	PinPoidTypeConst getTypeConst() const;

	/** Get the class name piece of the poid.
	 * @deprecated This method is DEPRECATED in favor of getTypeConst().
	 * @return PinPoidDb Class name piece of the poid
	 */
	PinPoidType getType() const;

	/** Get the database piece of the poid.
	 * @return PinPoidRev Instance revision piece of the poid
	 */
	PinPoidRev	getRev() const;

	/** Check if the poid is a typeonly poid; in other words.
	 * @return Pinbool Returns PIN_BOOLEAN_TRUE if the instance id is 
	 * either 0 or -1
	 */
	PinBool		isTypeOnly() const;

	/** Make a clone of the poid.
	 * @return PinPoidOwner Instance of the new poid owner handle
	 */
	PinPoidOwner	clone() const;

	/** Compares this object with the poid that is passed in via an owner
	 * or observer wrapper.
	 * @param poid Poid to compare with
	 * @param checkRev Optional parameter to include the revision for 
	 * comparison
	 * @return int Returns 0 if they compare
	 * @sa isEqual()
	 */
	int		compare(const PinPoidBase &poid, int checkRev=0) const;

	/** Compares this object with the poid that is passed in via an owner
	 * or observer wrapper. This method returns a boolean unlike compare() 
	 * which acts like ::strcmp().
	 * @param poid Poid to compare with
	 * @param checkRev Optional parameter to include the revision for 
	 * comparison
	 * @return PinBool Returns PIN_BOOLEAN_TRUE if they compare
	 * @sa compare()
	 */
	PinBool		isEqual(const PinPoidBase &poid, int checkRev=0) const;

	/** Return a string representation of the poid.
	 * @param buf Sharacater buffer to hold the result
	 * @param bufsize Size of the buffer
	 * @param skiprev Optional parameter, revision is excluded if 
	 * skiprev is 1
	 */
	void		toString(char buf[], int bufsize, int skiprev=0) const;

	/** Check if the poid is a null poid. 
	 * @return PinBool True If the underlying C poid is a null pointer 
	 * or if the database portion of the poid is zero.
	 */
	PinBool		isNull() const;

	/** Get the underlying C poid pointer. 
	 * @warning Useful in situations when invoking safe C functions 
	 * that in C++ code using this class. Make sure that the poid is not 
	 * destroyed by the callee.
	 * @sa release()
	 * @return poid_t* the underlying C poid pointer
	 */
#if defined(PCMCPP_CONST_SAFE)
    	const poid_t*	get() const;
#else
    	poid_t*	get() const;
#endif
    	poid_t*	get();

	/** Take ownership of the underlying C poid pointer.
	 * @warning Useful in situations when invoking  C functions that in 
	 * C++ code using this class, and there is no reason for this C++ 
	 * class to manage the C poid pointer. Make sure that the C poid 
	 * is properly destroyed.
	 * @sa get()
	 */
    	poid_t*		release();

	//@}


	/** @name Semi-public constructors and member Functions */

	//@{

	/** Make a new PinPoid.
	 * @warning This is intended for use by PinBase. Use the public 
	 * factory methods instead.
	 */
	PinPoid();

	/** Make a new PinPoid using the passed in C poid pointer.
	 * @param poidp Pointer to the C poid
	 * @param owns If 1, then this class takes ownership of the poidp
	 * @warning This is intended for use by PinBase. Use the public 
	 * factory methods instead.
	 */
	void grab(poid_t *poidp, PinBool owns);

	/** Takes over the C poid pointer from the source object.
	 * Releases or destroys the C poid it is currently holding.
	 * @warning This is intended for use by PinBase.
	 * @sa copyFrom()
	 */
	void takeFrom(PinPoid &);

	/** Points to the C poid pointer from the source object. 
	 * Releases or destroys the C poid it is currently holding.
	 * @warning This is intended for use by PinBase.
	 * @sa takeFrom()
	 */
	void copyFrom(const PinPoid &);

	//@}

public:
	/** Character separating the various pieces of a stringified poid.
	 */
	static const char SEPARATOR_CHAR;

private:
	/** Assignment operator explicitly prohibited.
	 */
	void operator=(const PinPoid&);

private: 
	/** Indicator whether or not the C poid pointer is owned by this obj.
	 */
	PinBool		m_owns;

	/** pointer to the underlying C-style poid
	 */
	poid_t		*m_poidp;
};

#ifndef PIN_NOT_USING_OSTREAM
/**  @relates PinPoid
 * Write the string form of the poid to the output stream.
 */
PCMCPP_API std::ostream& operator<<(std::ostream &os, PinPoidBase &flist);
#endif


/////////////////////////////////////////////////////////////////////////////

inline PinPoidDb
PinPoid::getDb() const
{
	return PIN_POID_GET_DB(m_poidp);
}

inline PinPoidId
PinPoid::getId() const
{
	return PIN_POID_GET_ID(m_poidp);
}

inline PinPoidTypeConst
PinPoid::getTypeConst() const
{
	return PIN_POID_GET_TYPE(m_poidp);
}

inline PinPoidType
PinPoid::getType() const
{
	return PIN_POID_GET_TYPE(m_poidp);
}

inline PinPoidRev
PinPoid::getRev() const
{
	return PIN_POID_GET_REV(m_poidp);
}

inline PinBool
PinPoid::isTypeOnly() const
{
	return PIN_POID_IS_TYPE_ONLY(m_poidp);
}

inline PinBool
PinPoid::isEqual(
	const PinPoidBase 	&other, 
	int 			checkRev) const
{
	/* 
	 * NOTE: PIN_POID_COMPARE() acts like strcmp and returns 0
	 * if there is no difference. This is kinda non-intuitive. And
	 * compare() is left alone to mimic the underlying function and
	 * isEqual returns a boolean.
	 */
	return !(compare(other, checkRev));
}

inline PinBool
PinPoid::isNull() const
{
	return PIN_POID_IS_NULL(m_poidp);
}

#if defined(PCMCPP_CONST_SAFE)
inline const poid_t*
PinPoid::get() const
{
	return const_cast< poid_t* >(m_poidp);
}
#else
inline poid_t*
PinPoid::get() const
{
	return m_poidp;
}
#endif

inline poid_t*
PinPoid::get()
{
	return m_poidp;
}

inline PinPoidObserver 
PinPoid::createAsObserved(poid_t *poidp)
{
    return poidp;
}

inline PinPoidOwner 	
PinPoid::createAsOwned(poid_t *poidp)
{
    return PinPoidOwner(poidp);
}

inline poid_t*
PinPoid::release()
{
	poid_t *poidp = m_poidp;
	m_poidp = NULL; 
	m_owns = 0;
	return poidp;
}

inline
PinPoid::PinPoid()
        : m_poidp(0), m_owns(PIN_BOOLEAN_FALSE)
{
}

inline void
PinPoid::grab(poid_t *poidp, PinBool owns)
{
	if (m_owns) {
		PIN_POID_DESTROY(m_poidp, 0);
	}
	m_poidp = const_cast<poid_t*>(poidp);
	m_owns = owns;
}

inline void 
PinPoid::takeFrom(PinPoid &other)
{
	if (m_owns) {
		PIN_POID_DESTROY(m_poidp, 0);
	}
	
	m_owns = other.m_owns;
	m_poidp = other.m_poidp;
	
	other.m_owns = PIN_BOOLEAN_FALSE;
	other.m_poidp = 0;
}

inline void
PinPoid::copyFrom(const PinPoid &other)
{
	if (m_owns) {
		PIN_POID_DESTROY(m_poidp, 0);
	}
	
	m_owns = PIN_BOOLEAN_FALSE;
	m_poidp = other.m_poidp;
}

inline
PinPoid::~PinPoid()
{
	if (m_owns) {
		PIN_POID_DESTROY(m_poidp, 0);
	}
}


#endif /*_PinPoid_H_ */
