#ifndef _pin_os_error_h_
#define _pin_os_error_h_

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


/** Get the text for an error code
  *
  * This function will load up the text for the passed system error code. Note
  * that some system error messages might have replacement tokens, which will
  * not be filled in by this function, since there is no platform portable way
  * for you to provide the correct replacement parameters.
  *
  * @param  err_code    The error code whose text is to be loaded
  * @param  to_fill     The text buffer to fill in
  * @param  max_chars   The max chars (including the null) that the buffer can
  *                     hold.
  */
void pin_get_err_text
(
    const u_int32 err_code,
    char* const to_fill,
    const u_int32 max_chars
);

#endif

