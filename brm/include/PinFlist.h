//
//	@(#)%Portal Version: PinFlist.h:PlatformR2Int:7:2006-Sep-12 03:33:40 %
// 
// Copyright (c) 1996, 2008, Oracle and/or its affiliates.All rights reserved. 
// 
//	This material is the confidential property of Oracle Corporation or its
//	licensors and may be used, reproduced, stored or transmitted only in
//	accordance with a valid Oracle license or sublicense agreement.
//

#ifndef _PinFlist_H_
#define _PinFlist_H_

#ifndef _PinErrorBuf_H_
  #include "PinErrorBuf.h"
#endif
#ifndef _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif
#ifndef _PinPoid_H_
  #include "PinPoid.h"
#endif
#ifndef _PINBigDecimal_H_
  #include "PinBigDecimal.h"
#endif

#ifndef PIN_NOT_USING_OSTREAM
#ifndef _RWSTD_IOSTREAM_INCLUDED
  #include <iostream>
#endif
#endif

class PinElemObservingIterator;
class PinElemObservingIteratorConst;
class PinElemOwningIterator;
class PinElemDroppingIterator;
class PinAnyObservingIterator;

/** The PinFlist class manage a C flist pointer. This class is intended to be
 * used via a PinFlistObserver or PinFlistOwner wrapper class.
 */
class PCMCPP_CLASS PinFlist {
public:
	/** @name Public Factory Methods */

	//@{

	/** Create an empty flist
	 * @return PinFlistOwner : Instance of the flist owner wrapper
	 */
	static PinFlistOwner 	create();

	/** Create a C++ observer wrapper for a C-style flist. This means that
	 * the flist will not be deleted when the object goes out of scope
	 * @param flistp Pointer to flist to wrap
	 * @return PinFlistObserver Instance of the flist observer wrapper
	 */
	static PinFlistObserver createAsObserved(pin_flist_t *flistp);

	/** Create a C++ const observer wrapper for a C-style flist. 
	 * This means that the flist will not be deleted when the object 
	 * goes out of scope. Also, only const operations will be allowed.
	 * @param flistp Pointer to flist to wrap
	 * @return PinFlistObserverConst Instance of the flist observer wrapper
	 */
#if defined(PCMCPP_CONST_SAFE)
	static PinFlistObserverConst createAsObservedConst(
				   pin_flist_t *flistp);
#endif

	/** Create a C++ owner wrapper for a C-style flist. This means that
	 * the flist will be deleted when the object goes out of scope
	 * @param flist  Pointer to the flist to wrap
	 * @return PinFlistOwner Instance of the flist owner wrapper
	 */
	static PinFlistOwner 	createAsOwned(pin_flist_t *flistp);

	/** Create an flist object from the string that contains a serialized
	 * string of the flist
	 * @param flistStr pointer to the flist string represetnation
	 * @return PinFlistOwner Instance of the flist owner wrapper
	 */
	static PinFlistOwner    createFromString(const char* flistStr);

	//@}

	/** Destructor
	 */
	~PinFlist();

	/** @name Public Member Functions for INT fields */

	//@{

	/** Get access to the value of integer field from the flist and 
	 * leaves the flist unchanged.
	 * @param fld name of field to retrieve (eg tsf_PIN_FLD_XXXX)
	 * @param optional indicator of whether the field is optional or not,
	 * indicates the field is required if the param is not passed in
	 * @return PinIntObserverConst observer of the integer value
	 * @throw PinDeterminateException If the field is not present and
	 * the optional indicator is set to FALSE, then an exception occurs.
	 */
	PinIntObserverConst	get(const PinIntTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE) 
	  			const;

	/** Get access to the value of integer field from the flist and 
	 * leaves the flist unchanged.
	 * @param fld name of field to retrieve (eg tsf_PIN_FLD_XXXX)
	 * @param optional indicator of whether the field is optional or not,
	 * indicates the field is required if the param is not passed in
	 * @return PinIntObserver observer of the integer value
	 * @throw PinDeterminateException If the field is not present and
	 * the optional indicator is set to FALSE, then an exception occurs.
	 */
	PinIntObserver		get(const PinIntTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE); 

	/** Remove an integer field from the flist and take ownership of the
	 * value via an integer owner wrapper which manages the memory.
	 * @param fld name of field to retrieve (eg tsf_PIN_FLD_XXXX)
	 * @param optional indicator of whether the field is optional or not
	 * indicates the field is required if the param is not passed in
	 * @return PinIntOwner owner of the integer value
	 * @throw PinDeterminateException If the field is not present and
	 * the optional indicator is set to FALSE, then an exception occurs.
	 * @deprecated Since the flist uses its own internal heap in 6.7, this
	 * actually results in an unnecessary copy of the value. Use get()
	 * instead.
	 */
	PinIntOwner		take(const PinIntTypeField& fld, 
				     const PinBool optional=PIN_BOOLEAN_FALSE);

	/** Add a new integer field to the flist. If the field already 
	 * exists, just change the value. The value is copied into the flist.
	 * @param fld name of field to add or change (eg tsf_PIN_FLD_XXXX)
	 * @param val integer value of the field
	 * @throw PinDeterminateException Throw on errors, usually fatal.
	 */
	void			set(const PinIntTypeField& fld, 
				    const PinInt val);

	/** Add a new integer field to the flist. If the field already 
	 * exists, just change the value. The value is copied into the flist.
	 * Used when "getting" a value from one flist and "setting" it into
	 * another via a wrapper.
	 * @param fld name of field to add or change (eg tsf_PIN_FLD_XXXX)
	 * @param val int wrapper for the value.
	 * @throw PinDeterminateException Throw on errors, usually fatal.
	 */
	void			set(const PinIntTypeField& fld,
				    const PinIntBase& val);

	/** Add a new integer field to the flist. If the field already 
	 * exists, just change the value. The value is copied into the flist.
	 * Used when "taking" a value from one flist and "puting" it into
	 * another via a wrapper. The ownership is usurped over from te passed
	 * in owner wrapper "val". Note that "val" will no longer be valid
	 * after this method.
	 * @param fld name of field to add or change (eg tsf_PIN_FLD_XXXX)
	 * @param val int owner wrapper for the value.
	 * @throw PinDeterminateException Throw on errors, usually fatal.
	 */
	void			put(const PinIntTypeField& fld,
				    PinIntOwner&);

	/** rename an integer field on the flist if it exists to a 
	 * new field of same type
	 * Throws an exception if source field is not found
	 * @param fld name of field to rename(eg tsf_PIN_FLD_XXXX)
	 * @param new fld name.
	 * @throw PinDeterminateException Throw on errors, usually fatal.
	 */
	void			rename(const PinIntTypeField& srcFld,
					const PinIntTypeField& destFld);

	/** copy an integer field from the given src flist over to this
	 * flist, using destFld as the new field name
	 * If destFld is already present, overwrite it
	 * @param src flist to copy from
	 * @param fld name of field to copy(eg tsf_PIN_FLD_XXXX)
	 * @param fld name on this flist
	 * @return boolean indicator of whether src field was found. When
	 * not found it is not considered an error.
	 * @throw PinDeterminateException Throw on errors, usually fatal.
	 */
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinIntTypeField& srcFld,
					const PinIntTypeField& destFld);

	/** move an integer field from the given src flist over to this
	 * flist, using destFld as the new field name
	 * If destFld is already present, overwrite it
	 * @param src flist to move from
	 * @param fld name of field to move(eg tsf_PIN_FLD_XXXX)
	 * @param fld name on this flist
	 * @return boolean indicator of whether src field was found. When
	 * not found it is not considered an error.
	 * @throw PinDeterminateException Throw on errors, usually fatal.
	 */
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinIntTypeField& srcFld,
					const PinIntTypeField& destFld);
	//@}


	/** @name Public Member Functions for UINT fields 
	 * See methods dealing with INT fields.
	 */

	//@{

	PinUintObserverConst	get(const PinUintTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;
	PinUintObserver		get(const PinUintTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE);
	PinUintOwner		take(const PinUintTypeField& fld, 
				     const PinBool optional=PIN_BOOLEAN_FALSE);
	void			set(const PinUintTypeField& fld,
				    const PinUint);
	void			set(const PinUintTypeField& fld,
				    const PinUintBase&);
	void			put(const PinUintTypeField& fld,
				    PinUintOwner&);
	void			rename(const PinUintTypeField& srcFld,
					const PinUintTypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinUintTypeField& srcFld,
					const PinUintTypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinUintTypeField& srcFld,
					const PinUintTypeField& destFld);

	/** @name Public Member Functions for ENUM fields 
	 * See methods dealing with INT fields.
	 */

	//@{
        PinEnumObserverConst 	get(const PinEnumTypeField& fld,
				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;
        PinEnumObserver 	get(const PinEnumTypeField& fld,
				    const PinBool optional=PIN_BOOLEAN_FALSE);
	PinEnumOwner		take(const PinEnumTypeField& fld, 
				     const PinBool optional=PIN_BOOLEAN_FALSE);
	void			set(const PinEnumTypeField& fld,
				    const PinEnum);
	void			set(const PinEnumTypeField& fld,
				    const PinEnumBase&);
	void			put(const PinEnumTypeField& fld,
				    PinEnumOwner&);
	void			rename(const PinEnumTypeField& srcFld,
					const PinEnumTypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinEnumTypeField& srcFld,
					const PinEnumTypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinEnumTypeField& srcFld,
					const PinEnumTypeField& destFld);


	//@}

	/** @name Public Member Functions for NUM fields.
	 * See methods dealing with INT fields.
	 */

	//@{

        PinNumObserverConst  	get(const PinNumTypeField& fld,
				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;
        PinNumObserver  	get(const PinNumTypeField& fld,
				    const PinBool optional=PIN_BOOLEAN_FALSE);
        PinNumOwner     	take(const PinNumTypeField& fld,
				     const PinBool optional);
        void            	set(const PinNumTypeField& fld,
				    const PinNum);
        void            	set(const PinNumTypeField& fld,
				    const PinNumBase&);
        void            	put(const PinNumTypeField& fld, PinNumOwner&);
	void			rename(const PinNumTypeField& srcFld,
					const PinNumTypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinNumTypeField& srcFld,
					const PinNumTypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinNumTypeField& srcFld,
					const PinNumTypeField& destFld);


	//@}

	/** @name Public Member Functions for DECIMAL fields
	 * See methods dealing with INT fields.
	 */

	//@{

        PinBigDecimalObserverConst 	get(const PinBigDecimalTypeField& fld,
				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;
        PinBigDecimalObserver 	get(const PinBigDecimalTypeField& fld,
				    const PinBool optional=PIN_BOOLEAN_FALSE);
        PinBigDecimalOwner    	take(const PinBigDecimalTypeField& fld,
                                   const PinBool optional=PIN_BOOLEAN_FALSE);
        void                  	set(const PinBigDecimalTypeField& fld,
				    const PinBigDecimal&);
        void                  	set(const PinBigDecimalTypeField& fld,
				  const PinBigDecimalBase&);
	void			rename(const PinBigDecimalTypeField& srcFld,
					const PinBigDecimalTypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinBigDecimalTypeField& srcFld,
					const PinBigDecimalTypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinBigDecimalTypeField& srcFld,
					const PinBigDecimalTypeField& destFld);


	// XXX (JP): put is missing!
	//@}

	/** @name Public Member Functions for STR fields
	 * See methods dealing with INT fields.
	 */

	//@{

	PinStrObserverConst	get(const PinStrTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;
	PinStrObserver		get(const PinStrTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE);
	PinStrOwner		take(const PinStrTypeField& fld, 
				     const PinBool optional=PIN_BOOLEAN_FALSE);
	void			set(const PinStrTypeField& fld, PinStrConst);
	void			set(const PinStrTypeField& fld,
				    const PinStrBase&);
	void			put(const PinStrTypeField& fld,
				    PinStrOwner&);
	void			rename(const PinStrTypeField& srcFld,
					const PinStrTypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinStrTypeField& srcFld,
					const PinStrTypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinStrTypeField& srcFld,
					const PinStrTypeField& destFld);


	//@}

	/** @name Public Member Functions for BINSTR fields
	 * See methods dealing with INT fields.
	 */

	//@{

 	PinBinstrObserverConst	get(const PinBinstrTypeField& fld, 
 				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;
 	PinBinstrObserver	get(const PinBinstrTypeField& fld, 
 				    const PinBool optional=PIN_BOOLEAN_FALSE);
 	PinBinstrOwner		take(const PinBinstrTypeField& fld, 
				     const PinBool optional=PIN_BOOLEAN_FALSE);
	void			set(const PinBinstrTypeField& fld,
				    const PinBinstrStruct);
 	void			set(const PinBinstrTypeField& fld,
				    const PinBinstrBase&);
 	void			put(const PinBinstrTypeField& fld,
				    PinBinstrOwner&);
	void			rename(const PinBinstrTypeField& srcFld,
					const PinBinstrTypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinBinstrTypeField& srcFld,
					const PinBinstrTypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinBinstrTypeField& srcFld,
					const PinBinstrTypeField& destFld);


	//@}

// PIN_FLDT_BUF
//	PinBufObserver		get(const PinBufTypeField& fld, 
//		    		const PinBool optional=PIN_BOOLEAN_FALSE);
//	PinBufOwner		take(const PinBufTypeField& fld, 
//			     	const PinBool optional=PIN_BOOLEAN_FALSE);
//	void			set(const PinBufTypeField& fld, PinBuf);
//	void			set(const PinBufTypeField& fld, PinBufBase&);
//	void			put(const PinBufTypeField& fld, PinBufOwner&);


	/** @name Public Member Functions for POID fields.
	 * See methods dealing with INT fields.
	 */

	//@{

	PinPoidObserverConst	get(const PinPoidTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;
	PinPoidObserver		get(const PinPoidTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE);
	PinPoidOwner		take(const PinPoidTypeField& fld, 
				     const PinBool optional=PIN_BOOLEAN_FALSE);
	void			set(const PinPoidTypeField& fld,
				    const PinPoidBase&);
	void			put(const PinPoidTypeField& fld,
				    PinPoidOwner&);
	void			rename(const PinPoidTypeField& srcFld,
					const PinPoidTypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinPoidTypeField& srcFld,
					const PinPoidTypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinPoidTypeField& srcFld,
					const PinPoidTypeField& destFld);


	//@}

	/** @name Public Member Functions for TSTAMP fields
	 * See methods dealing with INT fields.
	 */

	//@{

	PinTstampObserverConst	get(const PinTstampTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;
	PinTstampObserver	get(const PinTstampTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE);
	PinTstampOwner		take(const PinTstampTypeField& fld, 
				     const PinBool optional=PIN_BOOLEAN_FALSE);
	void			set(const PinTstampTypeField& fld, PinTstamp);
	void			set(const PinTstampTypeField& fld,
				    const PinTstampBase&);
	void			put(const PinTstampTypeField& fld,
				    PinTstampOwner&);
	void			rename(const PinTstampTypeField& srcFld,
					const PinTstampTypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinTstampTypeField& srcFld,
					const PinTstampTypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinTstampTypeField& srcFld,
					const PinTstampTypeField& destFld);

	/** @name Public Member Functions for INT64 fields
	 * See methods dealing with INT fields.
	 */

	//@{

	PinInt64ObserverConst	get(const PinInt64TypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;
	PinInt64Observer	get(const PinInt64TypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE);
	PinInt64Owner		take(const PinInt64TypeField& fld, 
				     const PinBool optional=PIN_BOOLEAN_FALSE);
	void			set(const PinInt64TypeField& fld, int64);
	void			set(const PinInt64TypeField& fld,
				    const PinInt64Base&);
	void			put(const PinInt64TypeField& fld,
				    PinInt64Owner&);
	void			rename(const PinInt64TypeField& srcFld,
					const PinInt64TypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinInt64TypeField& srcFld,
					const PinInt64TypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinInt64TypeField& srcFld,
					const PinInt64TypeField& destFld);


	//@}

	/** @name Public Member Functions for EBUF fields
	 * See methods dealing with INT fields.
	 */

	//@{

	PinErrorBufObserverConst get(const PinErrTypeField& fld, 
				    PinBool optional=PIN_BOOLEAN_FALSE)
	  			const;
	PinErrorBufObserver	get(const PinErrTypeField& fld, 
				    PinBool optional=PIN_BOOLEAN_FALSE);
	PinErrorBufOwner	take(const PinErrTypeField& fld, 
				     PinBool optional=PIN_BOOLEAN_FALSE);
	void			set(const PinErrTypeField& fld, 
				    const PinErrorBuf&);
	void			set(const PinErrTypeField& fld, 
				    const PinErrorBufBase&);
	void			rename(const PinErrTypeField& srcFld,
					const PinErrTypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinErrTypeField& srcFld,
					const PinErrTypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinErrTypeField& srcFld,
					const PinErrTypeField& destFld);

	
	//@}

	/** @name Public Member Functions for ARRAY fields */

	//@{

	/** Return the number of array elements of the given name.
	 * @param fld name of array field
	 * @return int number of array elements.
	 */
	int 			count(const PinArrayTypeField& fld) const;

	/** Add an empty array element to the flist. Note that in the 
	 * flist world, there is no notion of just adding an array; 
	 * you must specify an element.
	 * @param fld name of array field
	 * @param id element id
	 */
	PinFlistObserver 	add(const PinArrayTypeField& fld, 
				    PinRecId id);

	/** Get a const flist observer wrapper to an array element in the
	 * flist. Note that the array element is itself an flist.
	 * @param fld name of array field
	 * @param id element id
	 * @param optional indicator of whether the field is optional or not,
	 * indicates the field is required if the param is not passed in
	 * @return PinFlistObserverConst flist observer instance
	 * @throw PinDeterminateException If the field is not present and
	 * the optional indicator is set to FALSE, then an exception occurs.
	 */
	PinFlistObserverConst 	get(const PinArrayTypeField& fld,
				    const PinRecId id,
				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;

	/** Get a non-const flist observer wrapper to an array element in the
	 * flist. Note that the array element is itself an flist.
	 * @note The flist observer allows you to modify the underlying
	 * C flist. If you want to prevent that, declare the return value
	 * to be a const observer so that the return value would be of 
	 * type PinFlistObserverConst.
	 * @param fld name of array field
	 * @param id element id
	 * @param optional indicator of whether the field is optional or not,
	 * indicates the field is required if the param is not passed in
	 * @return PinFlistObserver flist observer instance
	 * @throw PinDeterminateException If the field is not present and
	 * the optional indicator is set to FALSE, then an exception occurs.
	 */
	PinFlistObserver 	get(const PinArrayTypeField& fld,
				    const PinRecId id,
				    const PinBool optional=PIN_BOOLEAN_FALSE);

	/** Remove an array element from the flist and take ownership of the
	 * content (which is another flist) via a flist owner wrapper
	 * which manages the memory. To just drop the array element, use
	 * the drop method.
	 * @param fld name of array field
	 * @param id element id
	 * @param optional indicator of whether the field is optional or not,
	 * indicates the field is required if the param is not passed in
	 * @return PinFlistOwner flist owner instance
	 * @throw PinDeterminateException If the field is not present and
	 * the optional indicator is set to FALSE, then an exception occurs.
	 */
	PinFlistOwner		take(const PinArrayTypeField& fld, PinRecId id,
				     const PinBool optional=PIN_BOOLEAN_FALSE);

	/** Add a new array element to the flist. If the element already 
	 * exists, just change the value. The value is copied and the 
	 * original passed in parameter is untouched.
	 * @param fld name of array field
	 * @param val flist observer or owner instance for the source value
	 * @param id element id
	 */
	void			set(const PinArrayTypeField& fld,
				    const PinFlistBase& val,
				    const PinRecId id);

	/** Add a new array element to the flist. If the element already 
	 * exists, just change the value. The value is usurped from the
	 * input parameter "val".
	 * @param fld name of array field
	 * @param val flist owner instance for the source value
	 * @param id element id
	 */
	void			put(const PinArrayTypeField& fld,
				    PinFlistOwner& val,
				    const PinRecId id);

	/** Drop an array element from the flist.  If the field is not
	 * found, it is not considered an error.
	 * @param fld name of field to drop
	 * @param id element id
	 * @throw PinDeterminateException for any errors that occur
	 */
	void			drop(const PinArrayTypeField& fld,
				     const PinRecId id);

	/** Rename all elements of an array from one field name 
	 * to another
	 * @param src fld name of field to rename
	 * @param dest fld name to use
	 * @throw PinDeterminateException If the element is not found
	 * or any other error occurs.
	 */
	void			rename(const PinArrayTypeField& srcFld,
					const PinArrayTypeField& destFld);

	/** Copy all elements of an array from another flist to this
	 * flist.  The field name can be changed while performing 
	 * the copy
	 * @param src flist to copy array from
	 * @param src array fld name to copy from srcFlist
	 * @param dest array fld name to use here
	 * @return boolean indicator of whether any src array elems were
	 * found. When not found it is not considered an error.
	 * @throw PinDeterminateException If the element is not found
	 * or any other error occurs.
	 */
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinArrayTypeField& srcFld,
					const PinArrayTypeField& destFld);

	/** Copy single element of an array from another flist to this
	 * flist.  The field name and recid can be changed while performing 
	 * the copy
	 * @param src flist to copy array element from
	 * @param src array fld name to copy from srcFlist
	 * @param src element id to copy
	 * @param dest array fld name to use here
	 * @param dest element id to use here
	 * @return boolean indicator of whether src array element was
	 * found. When not found it is not considered an error.
	 * @throw PinDeterminateException if any error occurs, usually Fatal
	 */
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinArrayTypeField& srcFld,
					const PinRecId srcId,
					const PinArrayTypeField& destFld,
					const PinRecId destId);

	/** Move all elements of an array from another flist to this
	 * flist.  The field name can be changed while performing 
	 * the move
	 * @param src flist to move array from
	 * @param src array fld name to move from srcFlist
	 * @param dest array fld name to use here
	 * @return boolean indicator of whether any src array elements were
	 * found. When not found it is not considered an error.
	 * @throw PinDeterminateException if any error occurs, usually Fatal
	 */
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinArrayTypeField& srcFld,
					const PinArrayTypeField& destFld);

	/** Move single element of an array from another flist to this
	 * flist.  The field name and recid can be changed while performing 
	 * the move
	 * @param src flist to move array from
	 * @param src array fld name to move from srcFlist
	 * @param src element id
	 * @param dest array fld name to use here
	 * @param dest element id to use here
	 * @return boolean indicator of whether src array element was
	 * found. When not found it is not considered an error.
	 * @throw PinDeterminateException if any error occurs, usually Fatal
	 */
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinArrayTypeField& srcFld,
					const PinRecId srcId,
					const PinArrayTypeField& destFld,
					const PinRecId destId);


#if defined(PCMCPP_CONST_SAFE)
	/** Walk through the elements in an array.
	 * @param fld name of field
	 * @param optional indicator of whether the field is optional or not,
	 * indicates the field is required if the param is not passed in
	 * @return PinElemObservingIteratorConst iterator object
	 * @throw PinDeterminateException If the element is not present.
	 */
	PinElemObservingIteratorConst getElements(
				const PinArrayTypeField& fld,
				const PinBool optional=PIN_BOOLEAN_TRUE)
				const;
#else
	PinElemObservingIterator getElements(
				const PinArrayTypeField& fld,
				const PinBool optional=PIN_BOOLEAN_TRUE) const;
#endif

	/** Walk through the elements in an array.
	 * @param fld name of field
	 * @param optional indicator of whether the field is optional or not,
	 * indicates the field is required if the param is not passed in
	 * @return PinElemObservingIterator iterator object
	 * @throw PinDeterminateException If the element is not present.
	 */
	PinElemObservingIterator getElements(
				const PinArrayTypeField& fld,
				const PinBool optional=PIN_BOOLEAN_TRUE);
	
	/** Walk through the elements in an array.
	 * @param fld name of field
	 * @param optional indicator of whether the field is optional or not,
	 * indicates the field is required if the param is not passed in
	 * @return PinElemDroppingIterator iterator object
	 * @throw PinDeterminateException If the element is not present.
	 */
	PinElemDroppingIterator getDroppableElements(
				const PinArrayTypeField& fld,
				const PinBool optional=PIN_BOOLEAN_TRUE);
	
	/** Remove the elements in an array one by one via an iterator.
	 * @param fld name of field
	 * @param optional indicator of whether the field is optional or not,
	 * indicates the field is required if the param is not passed in
	 * @return PinElemOwningIterator iterator object
	 * @throw PinDeterminateException If the element is not present.
	 */
	PinElemOwningIterator	takeElements(
			     	const PinArrayTypeField& fld,
				const PinBool optional=PIN_BOOLEAN_TRUE);

	//@}

	/** @name Public Member Functions for SUBSTRUCT fields */

	//@{

	PinFlistObserver 	add(const PinSubTypeField& fld);	    
	void			drop(const PinSubTypeField& fld);

	PinFlistObserverConst 	get(const PinSubTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE)
				    const;
	PinFlistObserver      	get(const PinSubTypeField& fld, 
				    const PinBool optional=PIN_BOOLEAN_FALSE);
	PinFlistOwner		take(const PinSubTypeField& fld, 
				     const PinBool optional=PIN_BOOLEAN_FALSE);
	void			set(const PinSubTypeField& fld,
				    const PinFlistBase&);
	void			put(const PinSubTypeField& fld,
				    PinFlistOwner&);
	void			rename(const PinSubTypeField& srcFld,
					const PinSubTypeField& destFld);
	bool			copyFrom(const PinFlistBase& srcFlist,
					const PinSubTypeField& srcFld,
					const PinSubTypeField& destFld);
	bool			moveFrom(const PinFlistBase& srcFlist,
					const PinSubTypeField& srcFld,
					const PinSubTypeField& destFld);


	//@}

	/** @name Other Public Member Functions */

	//@{

	/** Drop a field from the flist. For ARRAY or SUBSTRUCT fields, 
	 * use other methods.
	 * @param fld name of field to drop
	 * @throw PinDeterminateException If the field is not present.
	 */
 	void			drop(const PinAnyTypeField& fld);

	/** Append (concatenate) the flist to the end of the current one.
	 * @param flist flist to append
	 */
	void appendFlist(PinFlistBase& flist);
	
	/** Return the number of fields in the flist at the top level. 
	 * Note that this does not traverse down array elements or 
	 * substructs. Also, each individual array element is counted.
	 * @return int number of fields in the flist
	 */
	int 			count() const;

	/** Walk through the fields in the flist. Has specialized uses
	 * in debuggers, generic flist translators etc.
	 * @return PinAnyObservingIterator iterator object
	 * @sa PinAnyDiscriminator
	 * @todo Technically, we need a const version of this iterator!
	 */
	PinAnyObservingIterator	getFields();

	/** Sort the flist. See PIN_FLIST_SORT description.
	 * @param sortlist sorting key described as flist.
	 * @param descending if 1, sort in descending order
	 * @param sortDefault
	 */
	void			sort(const PinFlistBase &sortlist,
				     const int32 descending=0,
				     const int32 sortDefault=0);
	/** Sort the flist. If there are ARRAY elements, each  of 
	 * them is sorted as well recursively. See PIN_FLIST_RECURSIVE_SORT 
	 * description.
	 * @param sortlist sorting key described as flist.
	 * @param descending if 1, sort in descending order
	 * @param sortDefault
	 */
	void			sortRecursively(const PinFlistBase &sortlist, 
						const int32 descending=0, 
						const int32 sortDefault=0);

	/** Make a clone of the flist (deep copy).
	 * @return PinFlistOwner Instance of the new flist owner handle
	 */
	PinFlistOwner		clone() const;

	/** Check if the underlying C flist is valid (not null pointer).
	 * @return PinBool True If the underlying C flist is a null pointer 
	 */
	PinBool			isNull() const;

	/** Get the underlying C flist pointer. 
	 * @warning Useful in situations when invoking safe C functions 
	 * that in C++ code using this class. Make sure that the flist is not 
	 * destroyed by the callee.
	 * @sa release()
	 * @return flist_t* the underlying C flist pointer
	 */
#if defined(PCMCPP_CONST_SAFE)
	const pin_flist_t*	get() const;
#else
	pin_flist_t*		get() const;
#endif

	/** Get the underlying C flist pointer. 
	 * @warning Useful in situations when invoking safe C functions 
	 * that in C++ code using this class. Make sure that the flist is not 
	 * destroyed by the callee.
	 * @sa release()
	 * @return flist_t* the underlying C flist pointer
	 */
	pin_flist_t*	get();

	/** Take ownership of the underlying C flist pointer.
	 * @warning Useful in situations when invoking  C functions that in 
	 * C++ code using this class, and there is no reason for this C++ 
	 * class to manage the C flist pointer. Make sure that the C flist 
	 * is properly destroyed.
	 * @sa get()
	 */
	pin_flist_t*		release();

	//@}


	/** @name Semi-public constructors and member Functions */

	//@{

	/** Make a new PinFlist.
	 * @warning This is intended for use by PinBase. Use the public 
	 * factory methods instead.
	 */
	PinFlist();

	/** Make a new PinFlist using the passed in C flist pointer.
	 * @param flistp Pointer to the C flist
	 * @param owns If 1, then this class takes ownership of the flistp
	 * @warning This is intended for use by PinBase. Use the public 
	 * factory methods instead.
	 */
	void grab(pin_flist_t *flistp, int owns);

	/** Takes over the C flist pointer from the source object.
	 * Releases or destroys the C flist it is currently holding.
	 * @warning This is intended for use by PinBase.
	 * @sa copyFrom()
	 */
	void 			takeFrom(PinFlist &);

	/** Points to the C flist pointer from the source object. 
	 * Releases or destroys the C flist it is currently holding.
	 * @warning This is intended for use by PinBase.
	 * @sa takeFrom()
	 */
	void 			copyFrom(const PinFlist &);

	//@}
	
private:
	void operator=(const PinFlist &); // no implementation

	void * 		_get(const u_int32 fldnum, 
			     const PinBool optional=PIN_BOOLEAN_FALSE) const;
	void * 		_take(const u_int32 fldnum, 
			      const PinBool optional=PIN_BOOLEAN_FALSE);
	void		_set(const u_int32 fldnum, const void *vp);
	void		_put(const u_int32 fldnum, const void *vp);
	void		_set(const u_int32 fldnum, const void *vp,
			     const PinRecId id);
	void		_put(const u_int32 fldnum, const void *vp,
			     const PinRecId id);
	void		_rename(const pin_fld_num_t srcfldnum,
				const pin_fld_num_t destfldnum);
	bool		_copyFrom(pin_flist_t* src_flistp,
				const pin_fld_num_t srcfldnum,
				const pin_fld_num_t destfldnum);
	bool		_copyFrom(pin_flist_t* src_flistp,
				const pin_fld_num_t srcfldnum,
				const PinRecId srcId,
				const pin_fld_num_t destfldnum,
				const PinRecId destId);
	bool		_moveFrom(pin_flist_t* src_flistp,
				const pin_fld_num_t srcfldnum,
				const pin_fld_num_t destfldnum);
	bool		_moveFrom(pin_flist_t* src_flistp,
				const pin_fld_num_t srcfldnum,
				const PinRecId srcId,
				const pin_fld_num_t destfldnum,
				const PinRecId destId);

private: // Data
	/** Indicator of whether or not the data is owned by the wrapper
	 */
	PinBool		m_owns;

	/** pointer to the underlying C-style flist
	 */
	pin_flist_t	*m_flistp;
};


#ifndef PIN_NOT_USING_OSTREAM
PCMCPP_API std::ostream & operator<<(std::ostream &os,
				     const PinFlistBase &flist);
#endif


///////////////////////////////////////////////////////////////////////////////


inline PinIntObserverConst
PinFlist::get(const PinIntTypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinInt*>(vp);
}

inline PinIntObserver
PinFlist::get(const PinIntTypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinInt*>(vp);
}

inline PinIntOwner
PinFlist::take(const PinIntTypeField& fld, const PinBool optional)
{
	void *vp = _take(fld.num, optional);
	return PinIntOwner(static_cast<PinInt*>(vp));

}

inline void		
PinFlist::set(const PinIntTypeField& fld, const PinInt i)
{
	PinInt	val = i;
	_set(fld.num, &val);
}

inline void		
PinFlist::set(const PinIntTypeField& fld, const PinIntBase& observer)
{
	_set(fld.num, observer->get());
}

inline void		
PinFlist::put(const PinIntTypeField& fld, PinIntOwner& owner)
{
	_put(fld.num, owner->release());
}

inline void		
PinFlist::rename(const PinIntTypeField& srcFld, const PinIntTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinIntTypeField& srcFld, const PinIntTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinIntTypeField& srcFld, const PinIntTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

/////////////////////////////////////////////////////////////////////////////

inline PinUintObserverConst
PinFlist::get(const PinUintTypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinUint*>(vp);
}

inline PinUintObserver
PinFlist::get(const PinUintTypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinUint*>(vp);
}

inline PinUintOwner
PinFlist::take(const PinUintTypeField& fld, const PinBool optional)
{
	void *vp = _take(fld.num, optional);
	return PinUintOwner(static_cast<PinUint*>(vp));
}

inline void		
PinFlist::set(const PinUintTypeField& fld, const PinUint i)
{
	PinUint	val = i;
	_set(fld.num, &val);
}

inline void		
PinFlist::set(const PinUintTypeField& fld, const PinUintBase& observer)
{
	_set(fld.num, observer->get());
}

inline void		
PinFlist::put(const PinUintTypeField& fld, PinUintOwner& owner)
{
	_put(fld.num, owner->release());
}
inline void		
PinFlist::rename(const PinUintTypeField& srcFld,
		const PinUintTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinUintTypeField& srcFld,
		const PinUintTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinUintTypeField& srcFld,
		const PinUintTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}


/////////////////////////////////////////////////////////////////////////////

inline PinEnumObserverConst
PinFlist::get(const PinEnumTypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinEnum*>(vp);
}

inline PinEnumObserver
PinFlist::get(const PinEnumTypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinEnum*>(vp);
}

inline PinEnumOwner
PinFlist::take(const PinEnumTypeField& fld, const PinBool optional)
{
	void *vp = _take(fld.num, optional);
	return PinEnumOwner(static_cast<PinEnum*>(vp));
}

inline void		
PinFlist::set(const PinEnumTypeField& fld, const PinEnum i)
{
	PinEnum	val = i;
	_set(fld.num, &val);
}

inline void		
PinFlist::set(const PinEnumTypeField& fld, const PinEnumBase& observer)
{
	_set(fld.num, observer->get());
}

inline void		
PinFlist::put(const PinEnumTypeField& fld, PinEnumOwner& owner)
{
	_put(fld.num, owner->release());
}
inline void		
PinFlist::rename(const PinEnumTypeField& srcFld, 
		const PinEnumTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinEnumTypeField& srcFld, 
		const PinEnumTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinEnumTypeField& srcFld, 
		const PinEnumTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}



/////////////////////////////////////////////////////////////////////////////

inline PinNumObserverConst
PinFlist::get(const PinNumTypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinNum*>(vp);
}

inline PinNumObserver
PinFlist::get(const PinNumTypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinNum*>(vp);
}

inline PinNumOwner
PinFlist::take(const PinNumTypeField& fld, const PinBool optional)
{
	void *vp = _take(fld.num, optional);
	return PinNumOwner(static_cast<PinNum*>(vp));
}

inline void		
PinFlist::set(const PinNumTypeField& fld, const PinNum i)
{
	PinNum	val = i;
	_set(fld.num, &val);
}

inline void		
PinFlist::set(const PinNumTypeField& fld, const PinNumBase& observer)
{
	_set(fld.num, observer->get());
}


inline void		
PinFlist::put(const PinNumTypeField& fld, PinNumOwner& owner)
{
	_put(fld.num, owner->release());
}

inline void		
PinFlist::rename(const PinNumTypeField& srcFld, 
		const PinNumTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinNumTypeField& srcFld, 
		const PinNumTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinNumTypeField& srcFld, 
		const PinNumTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}


/////////////////////////////////////////////////////////////////////////////

inline PinStrObserverConst
PinFlist::get(const PinStrTypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinStr>(vp);
}

inline PinStrObserver
PinFlist::get(const PinStrTypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinStr>(vp);
}

inline PinStrOwner
PinFlist::take(const PinStrTypeField& fld, const PinBool optional)
{
	void *vp = _take(fld.num, optional);
	return PinStrOwner(static_cast<PinStr>(vp));
}

inline void		
PinFlist::set(const PinStrTypeField& fld, PinStrConst i)
{
	PinStr val = const_cast<PinStr>(i);
	_set(fld.num, val);
}

inline void		
PinFlist::set(const PinStrTypeField& fld, const PinStrBase& observer)
{
	_set(fld.num, observer->get());
}

inline void		
PinFlist::put(const PinStrTypeField& fld, PinStrOwner& owner)
{
	_put(fld.num, owner->release());
}

inline void		
PinFlist::rename(const PinStrTypeField& srcFld, 
		const PinStrTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinStrTypeField& srcFld, 
		const PinStrTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinStrTypeField& srcFld, 
		const PinStrTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}


/////////////////////////////////////////////////////////////////////////////

inline PinBinstrObserverConst
PinFlist::get(const PinBinstrTypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinBinstr>(vp);
}

inline PinBinstrObserver
PinFlist::get(const PinBinstrTypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinBinstr>(vp);
}

inline PinBinstrOwner
PinFlist::take(const PinBinstrTypeField& fld, const PinBool optional)
{
	return PinBinstrOwner(static_cast<PinBinstr>(_take(fld.num,
							   optional)));
}

inline void		
PinFlist::set(const PinBinstrTypeField& fld, const PinBinstrStruct i)
{
	PinBinstrStruct val = i;
	_set(fld.num, &val);
}

inline void		
PinFlist::set(const PinBinstrTypeField& fld, const PinBinstrBase& observer)
{
	_set(fld.num, observer->get());
}

inline void		
PinFlist::put(const PinBinstrTypeField& fld, PinBinstrOwner& owner)
{
	_put(fld.num, owner->release());
}

inline void		
PinFlist::rename(const PinBinstrTypeField& srcFld, 
		const PinBinstrTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinBinstrTypeField& srcFld, 
		const PinBinstrTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinBinstrTypeField& srcFld, 
		const PinBinstrTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}


/////////////////////////////////////////////////////////////////////////////

inline PinTstampObserverConst
PinFlist::get(const PinTstampTypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinTstamp*>(vp);
}

inline PinTstampObserver
PinFlist::get(const PinTstampTypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
	return static_cast<PinTstamp*>(vp);
}

inline PinTstampOwner
PinFlist::take(const PinTstampTypeField& fld, const PinBool optional)
{
	void *vp = _take(fld.num, optional);
	return PinTstampOwner(static_cast<PinTstamp*>(vp));
}

inline void		
PinFlist::set(const PinTstampTypeField& fld, PinTstamp i)
{
	PinTstamp	val = i;
	_set(fld.num, &val);
}

inline void		
PinFlist::set(const PinTstampTypeField& fld, const PinTstampBase& observer)
{
	_set(fld.num, observer->get());
}

inline void		
PinFlist::put(const PinTstampTypeField& fld, PinTstampOwner& owner)
{
	_put(fld.num, owner->release());
}

inline void		
PinFlist::rename(const PinTstampTypeField& srcFld, 
		const PinTstampTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinTstampTypeField& srcFld, 
		const PinTstampTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinTstampTypeField& srcFld, 
		const PinTstampTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}


/////////////////////////////////////////////////////////////////////////////

inline PinInt64ObserverConst
PinFlist::get(const PinInt64TypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
	return static_cast<int64*>(vp);
}

inline PinInt64Observer
PinFlist::get(const PinInt64TypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
	return static_cast<int64*>(vp);
}

inline PinInt64Owner
PinFlist::take(const PinInt64TypeField& fld, const PinBool optional)
{
	void *vp = _take(fld.num, optional);
	return PinInt64Owner(static_cast<int64*>(vp));
}

inline void		
PinFlist::set(const PinInt64TypeField& fld, int64 i)
{
	int64	val = i;
	_set(fld.num, &val);
}

inline void		
PinFlist::set(const PinInt64TypeField& fld, const PinInt64Base& observer)
{
	_set(fld.num, observer->get());
}

inline void		
PinFlist::put(const PinInt64TypeField& fld, PinInt64Owner& owner)
{
	_put(fld.num, owner->release());
}

inline void		
PinFlist::rename(const PinInt64TypeField& srcFld, 
		const PinInt64TypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinInt64TypeField& srcFld, 
		const PinInt64TypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinInt64TypeField& srcFld, 
		const PinInt64TypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}


/////////////////////////////////////////////////////////////////////////////

inline PinBigDecimalObserverConst
PinFlist::get(const PinBigDecimalTypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
    	return static_cast<pin_decimal_t*>(vp);
}

inline PinBigDecimalObserver
PinFlist::get(const PinBigDecimalTypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
    	return static_cast<pin_decimal_t*>(vp);
}

inline PinBigDecimalOwner
PinFlist::take(const PinBigDecimalTypeField& fld, const PinBool optional)
{
	void *vp = _take(fld.num, optional);
    	return static_cast<pin_decimal_t*>(vp);
}

inline void
PinFlist::set(const PinBigDecimalTypeField& fld, const PinBigDecimal& rebel)
{
    	_set(fld.num, const_cast<pin_decimal_t*>(rebel.get()));
}

inline void
PinFlist::set(const PinBigDecimalTypeField& fld, 
              const PinBigDecimalBase& observer)
{
    if (observer.isNullWrapperPtr()) {
                _set(fld.num, 0);
	} else {
                _set(fld.num, const_cast<pin_decimal_t*>(observer->get()));
	}
}

inline void		
PinFlist::rename(const PinBigDecimalTypeField& srcFld,
		const PinBigDecimalTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinBigDecimalTypeField& srcFld,
		const PinBigDecimalTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinBigDecimalTypeField& srcFld,
		const PinBigDecimalTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}


/////////////////////////////////////////////////////////////////////////////

inline PinErrorBufObserverConst
PinFlist::get(const PinErrTypeField& fld, PinBool optional) const
{
	void *vp = _get(fld.num, optional);
    	return static_cast<pin_errbuf_t*>(vp);
}

inline PinErrorBufObserver
PinFlist::get(const PinErrTypeField& fld, PinBool optional)
{
	void *vp = _get(fld.num, optional);
    	return static_cast<pin_errbuf_t*>(vp);
}

inline PinErrorBufOwner
PinFlist::take(const PinErrTypeField& fld, PinBool optional)
{
	void *vp = _take(fld.num, optional);
    	return static_cast<pin_errbuf_t*>(vp);
}

inline void
PinFlist::set(const PinErrTypeField& fld, const PinErrorBuf& rebel)
{
    	_set(fld.num, const_cast<pin_errbuf_t*>(rebel.get()));
}

inline void
PinFlist::set(const PinErrTypeField& fld, const PinErrorBufBase& observer)
{
	if (observer.isNullWrapperPtr()) {
		_set(fld.num, 0);
	} else {
	        _set(fld.num, const_cast<pin_errbuf_t*>(observer->get()));
	}
}

inline void		
PinFlist::rename(const PinErrTypeField& srcFld, 
		const PinErrTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinErrTypeField& srcFld, 
		const PinErrTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinErrTypeField& srcFld, 
		const PinErrTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}


/////////////////////////////////////////////////////////////////////////////

inline PinPoidObserverConst
PinFlist::get(const PinPoidTypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
	return static_cast<poid_t*>(vp);
}

inline PinPoidObserver
PinFlist::get(const PinPoidTypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
	return static_cast<poid_t*>(vp);
}

inline PinPoidOwner
PinFlist::take(const PinPoidTypeField& fld, const PinBool optional)
{
	void *vp = _take(fld.num, optional);
	return static_cast<poid_t*>(vp);
}

inline void		
PinFlist::set(const PinPoidTypeField& fld, const PinPoidBase& observer)
{
    _set(fld.num, observer->get());
}

inline void		
PinFlist::put(const PinPoidTypeField& fld, PinPoidOwner& owner)
{
    _put(fld.num, owner->release());
}

inline void		
PinFlist::rename(const PinPoidTypeField& srcFld, 
		const PinPoidTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinPoidTypeField& srcFld, 
		const PinPoidTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinPoidTypeField& srcFld, 
		const PinPoidTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}


/////////////////////////////////////////////////////////////////////////////

inline void		
PinFlist::set(const PinArrayTypeField& fld, const PinFlistBase& sp, 
              const PinRecId id)
{
	_set(fld.num, sp->get(), id);
}

inline void		
PinFlist::put(const PinArrayTypeField& fld, PinFlistOwner& owner,
              const PinRecId id)
{
	_put(fld.num, owner->release(), id);
}

inline void		
PinFlist::rename(const PinArrayTypeField& srcFld, 
		const PinArrayTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinArrayTypeField& srcFld, 
		const PinArrayTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinArrayTypeField& srcFld, const PinRecId srcId,
		const PinArrayTypeField& destFld, const PinRecId destId)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, srcId, destFld.num, destId);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinArrayTypeField& srcFld,
		const PinArrayTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinArrayTypeField& srcFld, const PinRecId srcId,
		const PinArrayTypeField& destFld, const PinRecId destId)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, srcId, destFld.num, destId);
}


/////////////////////////////////////////////////////////////////////////////

inline PinFlistObserver
PinFlist::get(const PinSubTypeField& fld, const PinBool optional)
{
	void *vp = _get(fld.num, optional);
	return static_cast<pin_flist_t*>(vp);
}

inline PinFlistObserverConst
PinFlist::get(const PinSubTypeField& fld, const PinBool optional) const
{
	void *vp = _get(fld.num, optional);
	return static_cast<pin_flist_t*>(vp);
}

inline PinFlistOwner
PinFlist::take(const PinSubTypeField& fld, const PinBool optional)
{
	void *vp = _take(fld.num, optional);
	return static_cast<pin_flist_t*>(vp);
}

inline void		
PinFlist::set(const PinSubTypeField& fld, const PinFlistBase& observer)
{
    _set(fld.num, observer->get());
}

inline void		
PinFlist::put(const PinSubTypeField& fld, PinFlistOwner& owner)
{
    _put(fld.num, owner->release());
}
inline void		
PinFlist::rename(const PinSubTypeField& srcFld, 
		const PinSubTypeField& destFld)
{
	_rename(srcFld.num, destFld.num);
}

inline bool		
PinFlist::copyFrom(const PinFlistBase& srcFlist,
		const PinSubTypeField& srcFld, 
		const PinSubTypeField& destFld)
{
	return _copyFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}

inline bool		
PinFlist::moveFrom(const PinFlistBase& srcFlist,
		const PinSubTypeField& srcFld, 
		const PinSubTypeField& destFld)
{
	return _moveFrom(srcFlist->m_flistp, srcFld.num, destFld.num);
}


/////////////////////////////////////////////////////////////////////////////

inline PinBool		
PinFlist::isNull() const
{
	return (m_flistp == NULL);
}

#if defined(PCMCPP_CONST_SAFE)
inline const pin_flist_t*
PinFlist::get() const
{
	return m_flistp;
}
#else
inline pin_flist_t*
PinFlist::get() const
{
	return const_cast< pin_flist_t *>(m_flistp);
}
#endif

inline pin_flist_t*
PinFlist::get()
{
	return m_flistp;
}

inline
PinFlist::PinFlist()
    	: m_flistp(0), m_owns(PIN_BOOLEAN_FALSE)
{
}

inline PinFlistObserver
PinFlist::createAsObserved(pin_flist_t *flistp)
{
    	return flistp;
}

#if defined(PCMCPP_CONST_SAFE)
inline PinFlistObserverConst
PinFlist::createAsObservedConst(pin_flist_t *flistp)
{
    	return flistp;
}
#endif

inline PinFlistOwner 	
PinFlist::createAsOwned(pin_flist_t *flistp)
{
  	return flistp;
}

inline 
PinFlist::~PinFlist()
{
	if (m_owns) {
		PIN_FLIST_DESTROY_EX(&m_flistp, 0);
	}
}

inline pin_flist_t*	
PinFlist::release()
{
	pin_flist_t *flistp = m_flistp;
	m_flistp = NULL; 
	m_owns = 0;
	return flistp;
}

inline void
PinFlist::grab(pin_flist_t *flistp, PinBool owns)
{
	if (m_owns) {
		PIN_FLIST_DESTROY(m_flistp, 0);
	}
  	m_flistp = const_cast<pin_flist_t*>(flistp);
  	m_owns = owns;
}

inline void 
PinFlist::takeFrom(PinFlist &other)
{
	if (m_owns) {
		PIN_FLIST_DESTROY(m_flistp, 0);
	}
	
	m_owns = other.m_owns;
	m_flistp = other.m_flistp;
	
	other.m_owns = PIN_BOOLEAN_FALSE;
	other.m_flistp = 0;
}

inline void
PinFlist::copyFrom(const PinFlist &other)
{
	if (m_owns) {
		PIN_FLIST_DESTROY(m_flistp, 0);
	}
	
	m_owns = PIN_BOOLEAN_FALSE;
	m_flistp = other.m_flistp;
}
	
#endif /*_PinFlist_H_ */
