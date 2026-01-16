/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PinAnyBase_H_
#define _PinAnyBase_H_

//
// IMPORTANT: Keep the following classes as "abstract" classes (interfaces)
// to avoid multiple inheritance issues.
//		PinAnyBase
//		PinAnyDiscriminator
//

/////////////////////////////////////////////////////////////////////////////
//
//
//                    +-------------+
//                    |  PinXXXBase |
//                    +-------------+
//                           |
//                          \|/
//                           V
//                    +-------------+        +-----------------------------+
//                    |  PinXXXBase | .....> | PinXXXWrapper (simple types)|
//                    +-------------+        |      (or)                   |
//                           |               | PinPoid/PinContext/etc.     |
//                           |               +-----------------------------+
//               .-----------------------.
//               |                       |
//              \|/                     \|/
//               V                       V
//      +-----------------+        +-------------+   
//      |  PinXXXObserver |        | PinXXXOwner |
//      +-----------------+        +-------------+
//
/////////////////////////////////////////////////////////////////////////////

#ifndef    _PcmCpp_H_
  #include "PcmCpp.h"
#endif

class PinAnyDiscriminator;


class PCMCPP_CLASS PinAnyBase {
public:
	virtual ~PinAnyBase()
	{}
	virtual	void discriminate(PinAnyDiscriminator &disc, void *vp) = 0;
};

#endif /*_PinAnyBase_H_ */
