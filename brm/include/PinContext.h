/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2008 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinContext_H_
#define _PinContext_H_

#ifndef _PinErrorBuf_H_
  #include "PinErrorBuf.h"
#endif
#ifndef _PinTypeHandle_H_
    #include "PinTypeHandle.h"
#endif

/** Class to represent the portal context for executing opcodes
 */
class PCMCPP_CLASS PinContext
{
	class PinTransaction;
	
 public:
	// Factory Methods
	static PinContextOwner 	create();

	/** Create a context from a given file descriptor
	 * @param fd the file descriptor to use
	 * @param user the user poid for the connection
	 * @return PinContextOwner
	 */
	static PinContextOwner  createFromFileDescriptor(int fd,
							 PinPoidBase& user);

	static PinContextOwner	create(const PinFlistBase &inFlist);
	static PinContextObserver 	createAsObserved(pcm_context_t *ctxp);
	static PinContextOwner 	createAsOwned(pcm_context_t *ctxp);
	
	~PinContext();

    	PinPoidOwner		getUserId() const;
    	PinPoidOwner		getSession() const;

	/** Call the internal pcp send routine, if you wish to do your own
	 * connection management.
	 * @param opNum the opcode to call
	 * @param opflags the opcode flags
	 * @param pcp_flags the pcp flags
	 * @param flist the flist to send
	 * @param how the method to send with; either PCPXDR_BLOCKING or
	 * PCPXDR_NON_BLOCKING
	 */
	void  pcpSend(PinOp opNum, PinOpFlags opflags,
		       PinOpFlags pcp_flags, PinFlistBase& flist,
		       int32 how);

	/** Call the internal pcp receive routine, if you wish to do your
	 * own connection management.
	 * @param flist the flist returned from the call
	 * @param user the user object returned from the call
	 */
	void  pcpReceive(PinFlistOwner& flist, PinPoidOwner& user);
	
	void			op(PinOp opnum, PinOpFlags flags, 
				   const PinFlistBase  &input, 
				   PinFlistOwner &output);
	void			opref(PinOp opnum, PinOpFlags flags, 
				   const PinFlistBase  &input, 
				   PinFlistOwner &output);
	void			op(PinOp opnum, PinOpFlags flags, 
				   const PinFlistBase  &input, 
				   PinFlistOwner &output,
				   const char *filename, int32 line_no);
	void			opref(PinOp opnum, PinOpFlags flags, 
				   const PinFlistBase  &input, 
				   PinFlistOwner &output,
				   const char *filename, int32 line_no);
	void			close(int how=0);
	void			reconnect(char *passwd);


	// The following are required by PinBase.
	PinBool			isNull() const;
#if defined(PCMCPP_CONST_SAFE)
	const pcm_context_t*	get() const;
#else
	pcm_context_t*		get() const;
#endif
	pcm_context_t*		get();

    	pcm_context_t*		release();
	void grab(pcm_context_t* pointee, PinBool owns);
	void takeFrom(PinContext &);
	void copyFrom(const PinContext &);
	PinContext();
	
private:
	void operator=(const PinContext&);

	void			_op(PinOp opnum, PinOpFlags flags, 
				    const pin_flist_t *in_flistp, 
				    PinFlistOwner &output,
				    bool use_opref = false,
				    const char *filename = __FILE__,
				    int32 line_no = __LINE__);
private: // Data
	PinBool		ownsM;
	pcm_context_t	*ctxpM;
};

/////////////////////////////////////////////////////////////////////////////

inline PinBool
PinContext::isNull() const
{
	return (ctxpM == NULL);
}

#if defined(PCMCPP_CONST_SAFE)
inline const pcm_context_t*
PinContext::get() const
{
	return ctxpM;
}
#else
inline pcm_context_t*
PinContext::get() const
{
	return const_cast< pcm_context_t* >(ctxpM);
}
#endif

inline pcm_context_t*
PinContext::get()
{
	return ctxpM;
}

inline
PinContext::PinContext()
    : ctxpM(0), ownsM(PIN_BOOLEAN_FALSE)
{
}

inline void
PinContext::grab(pcm_context_t *ctxp, PinBool owns)
{
	if (ownsM) {
		close();
	}
  	ctxpM = ctxp;
  	ownsM = owns;
}

/////////////////////////////////////////////////////////////////////////////

#endif /*_PinContext_H_ */
