/*
 *      @(#) % %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinLogger_H_
#define _PinLogger_H_

#ifndef    _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif

class PCMCPP_CLASS PinLog {
public:
        static void log(const PinEbufExc& exc, u_int32 level, 
			const char *msg=NULL,
			const char *filename=NULL, int32 line_no=0)
	{
		char *new_msg = const_cast<char *>(msg);
		pin_err_log_ebuf(level, new_msg, filename, line_no, 
                                 (pin_errbuf_t*)&exc.m_ebuf);
	}
        static void log(const PinFlistBase& flist, u_int32 level, 
			const char *msg=NULL,
			const char *filename=NULL, int32 line_no=0)
	{
		char *new_msg = const_cast<char *>(msg);
		pin_flist_t *fp = NULL;
		if (! flist.isNullWrapperPtr()) {
			fp = const_cast<pin_flist_t*>(flist->get());
		}
		pin_log_flist(level, new_msg, filename, line_no, fp);
	}
        static void log(const PinPoidBase& poid, u_int32 level, 
			const char *msg=NULL,
			const char *filename=NULL, int32 line_no=0)
	{
		char *new_msg = const_cast<char *>(msg);
		poid_t *pdp = NULL;
		if (! poid.isNullWrapperPtr()) {
			pdp = const_cast<poid_t*>(poid->get());
		}
		pin_log_poid(level, new_msg, filename, line_no, pdp);
	}
	static void log(u_int32 level, const char *msg=NULL, 
			const char *filename=NULL, int32 line_no=0)
	{
		char *new_msg = const_cast<char *>(msg);
		pin_err_log_msg(level, new_msg, filename, line_no);
	}
};

#define PIN_LOG(a,b,c)	PinLog::log((a), (b), (c), __FILE__, __LINE__)
#define PIN_MSG(a,b)	PinLog::log((a), (b), __FILE__, __LINE__)

#endif /*_PinLogger_H_*/

