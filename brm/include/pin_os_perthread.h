#ifndef _pin_os_perthread_h_
#define _pin_os_perthread_h_

/*
 *	@(#) % %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_types_h_
#include <pin_os_types.h>
#endif

/* Define a handle for per-thread data */
typedef u_int32 PinPTSlotId;

#if defined(__cplusplus)
extern "C" {
#endif

u_int32 pin_get_pt_slot(PinPTSlotId* to_fill);
u_int32 pin_get_pt_data(const PinPTSlotId hand, void** to_fill);
u_int32 pin_set_pt_data(const PinPTSlotId hand, void* const to_set);

#if defined(__cplusplus)
}
#endif

#endif

