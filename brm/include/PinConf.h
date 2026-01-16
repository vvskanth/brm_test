/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinConf_H_
#define _PinConf_H_

#ifndef _PinTypeHandle_H_
  #include "PinTypeHandle.h"
#endif

class PCMCPP_CLASS PinConf {
public:
	static PinIntOwner 	getInt(const char *prog_name, const char *token,
				       int optional=PIN_BOOLEAN_FALSE);
	static PinUintOwner 	getUint(const char *prog_name, const char *token,
					int optional=PIN_BOOLEAN_FALSE);
        static PinNumOwner      getNum(const char *prog_name, const char *token,
                                        int optional=PIN_BOOLEAN_FALSE);
	static PinStrOwner 	getStr(const char *prog_name, const char *token,
					int optional=PIN_BOOLEAN_FALSE);
	static PinPoidOwner 	getPoid(const char *prog_name, const char *token,
					int optional=PIN_BOOLEAN_FALSE);
				       

	// TODO: support pin_conf_multi() using an iterator style interface.
};

#endif /*_PinConf_H_ */
