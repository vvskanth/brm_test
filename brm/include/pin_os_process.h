#ifndef _pin_os_process_h_
#define _pin_os_process_h_
/*
 *	@(#) % %
 *	
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

/*
 *  The thread stuff used to be in this header, so for a while, we'll keep
 *  backwards compabilitity by included that header here.
 */
#ifndef _pin_os_thread_h_
#include <pin_os_thread.h>
#endif


/** Get the current process' id
  *
  * This function will return the id of the calling process. The form of this
  * id can be different on different platforms, so treat it like a magic
  * cookie to the greatest degree possible, making no assumptions about its
  * range, uniqueness over reuse, etc...
  *
  * @return The current process id
  */
u_int32 pin_get_pid();


#endif

