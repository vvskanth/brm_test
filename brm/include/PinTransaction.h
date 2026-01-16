/*
 *	@(#) % %
 * 
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 * 
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinTransaction_H_
#define _PinTransaction_H_

#ifndef    _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif

class PCMCPP_CLASS PinTransaction {
public:
	PinTransaction(PinContextBase &ctx);
	PinTransaction(PinContextBase &ctx, PinTransFlags flags, 
			const PinPoidBase &acctPdp);
	~PinTransaction();
	
	void begin(PinTransFlags flags, const PinPoidBase &acctPdp);
	void commit();
	void abort();

	PinBool	isLocal();
	PinBool	isInTrans();
	
private:
	friend class PinContext;
	
	void operator=(const PinTransaction&);
	PinTransaction(PinTransaction&);
	
	PinBool			m_local;
	PinBool			m_inTrans;
	PinContextBase 		&m_ctx;
};

#endif /* _PinTransaction_H_ */
