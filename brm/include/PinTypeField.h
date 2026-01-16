/*
 *	@(#) % %
 * 
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 * 
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinTypeField_H_
#define _PinTypeField_H_

#ifndef    _PcmCpp_H_
  #include "PcmCpp.h"
#endif

class PCMCPP_CLASS PinAnyTypeField {
protected:
	PinAnyTypeField(const u_int32 fld)
		: num(fld)
	{}
	PinAnyTypeField(const char *fldname )
		: num( pin_field_of_name( fldname ) )
	{}
public:
	u_int32	num;
};

class PCMCPP_CLASS PinSimpleTypeField : public PinAnyTypeField {
protected:
	PinSimpleTypeField(const u_int32 fld)
		: PinAnyTypeField(fld)
	{}
	PinSimpleTypeField(const char *fldname )
		: PinAnyTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinComplexTypeField : public PinAnyTypeField {
protected:
	PinComplexTypeField(const u_int32 fld)
		: PinAnyTypeField(fld)
	{}
	PinComplexTypeField(const char *fldname )
		: PinAnyTypeField( fldname )
	{}
};



class PCMCPP_CLASS PinIntTypeField : public PinSimpleTypeField {
public:
	PinIntTypeField(const u_int32 fld)
		: PinSimpleTypeField(fld)
	{}
	PinIntTypeField(const char *fldname )
		: PinSimpleTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinUintTypeField : public PinSimpleTypeField {
public:
	PinUintTypeField(const u_int32 fld)
		: PinSimpleTypeField(fld)
	{}
	PinUintTypeField(const char *fldname )
		: PinSimpleTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinNumTypeField : public PinSimpleTypeField {
public:
        PinNumTypeField(const u_int32 fld)
                : PinSimpleTypeField(fld)
        {}
	PinNumTypeField(const char *fldname )
		: PinSimpleTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinEnumTypeField : public PinSimpleTypeField {
public:
	PinEnumTypeField(const u_int32 fld)
		: PinSimpleTypeField(fld)
	{}
	PinEnumTypeField(const char *fldname )
		: PinSimpleTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinTstampTypeField : public PinSimpleTypeField {
public:
	PinTstampTypeField(const u_int32 fld)
		: PinSimpleTypeField(fld)
	{}
	PinTstampTypeField(const char *fldname )
		: PinSimpleTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinInt64TypeField : public PinSimpleTypeField {
public:
	PinInt64TypeField(const u_int32 fld)
		: PinSimpleTypeField(fld)
	{}
	PinInt64TypeField(const char *fldname )
		: PinSimpleTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinStrTypeField : public PinSimpleTypeField {
public:
	PinStrTypeField(const u_int32 fld)
		: PinSimpleTypeField(fld)
	{}
	PinStrTypeField(const char *fldname )
		: PinSimpleTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinBinstrTypeField : public PinSimpleTypeField {
public:
	PinBinstrTypeField(const u_int32 fld)
		: PinSimpleTypeField(fld)
	{}
	PinBinstrTypeField(const char *fldname )
		: PinSimpleTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinBufTypeField : public PinComplexTypeField {
public:
	PinBufTypeField(const u_int32 fld)
		: PinComplexTypeField(fld)
	{}
	PinBufTypeField(const char *fldname )
		: PinComplexTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinPoidTypeField : public PinComplexTypeField {
public:
	PinPoidTypeField(const u_int32 fld)
		: PinComplexTypeField(fld)
	{}
	PinPoidTypeField(const char *fldname )
		: PinComplexTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinBigDecimalTypeField : public PinComplexTypeField {
public:
        PinBigDecimalTypeField(const u_int32 fld)
		: PinComplexTypeField(fld)
	{}
	PinBigDecimalTypeField(const char *fldname )
		: PinComplexTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinArrayTypeField : public PinComplexTypeField {
public:
	PinArrayTypeField(const u_int32 fld)
		: PinComplexTypeField(fld)
	{}
	PinArrayTypeField(const char *fldname )
		: PinComplexTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinSubTypeField : public PinComplexTypeField {
public:
	PinSubTypeField(const u_int32 fld)
		: PinComplexTypeField(fld)
	{}
	PinSubTypeField(const char *fldname )
		: PinComplexTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinErrTypeField : public PinAnyTypeField {
public:
	PinErrTypeField(const u_int32 fld)
		: PinAnyTypeField(fld)
	{}
	PinErrTypeField(const char *fldname )
		: PinAnyTypeField( fldname )
	{}
};

class PCMCPP_CLASS PinObjTypeField : public PinAnyTypeField {
public:
	PinObjTypeField(const u_int32 fld)
		: PinAnyTypeField(fld)
	{}
	PinObjTypeField(const char *fldname )
		: PinAnyTypeField( fldname )
	{}
};

#endif /* _PinTypeField_H_ */
