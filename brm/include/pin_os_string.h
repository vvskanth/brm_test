/*
 *	@(#)%Portal Version: pin_os_string.h:RWSmod7.3.1Int:1:2007-Sep-12 04:46:01 %
 *      
* Copyright (c) 1996, 2023, Oracle and/or its affiliates. 
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_string_h_
#define _pin_os_string_h_

#include <stdlib.h>
#include <string.h>

#ifndef _PIN_TYPE_H_
  #include "pin_type.h"
#endif

#define HAS_STRTOK

#if defined(WINDOWS)

  #ifndef strcasecmp
    #define strcasecmp(a,b)     stricmp(a,b)
    #define strncasecmp(a,b,c)  strnicmp(a,b,c)
  #endif

#endif /*WINDOWS*/


#if defined(__cplusplus)
extern "C" {
#endif

#if defined(WINDOWS)
  char* strtok_r(
	char*                   strp,
	const char*             tokensp,
	char**                  lastspp);

  #define pin_atoi64            atoi64
#else
  #define pin_atoi64            atoll
#endif


/*
 * Bit masks for the flags field to pin_itoa_formatted and
 * pin_uitoa_formatted.
 * NOTE: We only check the non-zero defines.  The
 * other defines are mainly for documentation purposes
 */
#define PIN_ITOA_LEFT_JUSTIFIED         0
#define PIN_ITOA_RIGHT_JUSTIFIED        1
#define PIN_ITOA_ZERO_FILL              2
#define PIN_ITOA_BLANK_FILL             0

/*************************************************
 * pin_strlcpy - is better replacement for strncpy
 * For Solaris, use built in strlcpy, for everyone
 * else use our own function.
 */
#if defined(__sun)
  #define pin_strlcpy( dest, src, size ) \
	(strlcpy( dest, src, size ))
  #define pin_strlcat( dest, src, size ) \
	(strlcat( dest, src, size ))
#else
  size_t pin_strlcpy(
	char*                   dst,
	const char*             src,
	size_t                  size );
  size_t pin_strlcat(
	char*                   dst,
	const char*             src,
	size_t                  size );
#endif

int pin_memset(
        void *                  v,
        size_t                  smax,
        int                     c,
        size_t                  n);
int pin_memcpy(
        void                    *dest,
        size_t                  dmax,
        const void              *src,
        size_t                  smax);
int pin_memmove (
	void 			*dest, 
	size_t 			dmax, 
	const void 		*src, 
	size_t 			smax);


/* pin_hex_str_to_u_int32
 * This function will convert a string of characters that 
 * is a base 16 number into an u_int32.
 * Parameter 1 is the pointer to the character string. This
 *             doesn't have to be null terminated.  See
 *             parameter 3.
 * Parameter 2 is the pointer to where an error value is
 *             to be set in case of error.  An error will
 *             be indicated if this value is 1.  Otherwise
 *             this value will be 0.
 * Parameter 3 is the number of characters of parameter 1 
 *             to process.
 *
 * Returns the converted value.
 */
u_int32 pin_hex_str_to_u_int32(
	char*                   str_ptr,
	int32*                  err_ptr,
	u_int32                 num_of_chars);

/*
 * These routines are the opposite versions of atoi.
 * Since they aren't part of the C standard, they aren't
 * available on all OS compilers.  So we have these.
 *
 * One main difference is that these return a pointer to
 * the NULL character at the end of the resulting string.
 */
char* pin_itoa(
	int32                   value,
	char*                   strp,
	int32                   radix);

char* pin_uitoa(
	u_int32                 value,
	char*                   strp,
	int32                   radix);

/*
 * pin_itoa_formatted
 * This function is an extended version of pin_itoa.
 * It can be used as a replacement for
 *   sprintf(buf, "%06d", n);
 * instead of this you would use:
 *   pin_itoa_formatted(n, buf, 10, 6, PIN_ITOA_ZERO_FILL | PIN_ITOA_RIGHT_JUSTIFIED);
 */
char* pin_itoa_formatted(
	int32                   value,
	char*                   str_ptr,
	int32                   radix,
	int32                   width,
	u_int32                 flags);

/*
 * pin_uitoa_formatted
 * This function is an extended version of pin_uitoa.
 * It can be used as a replacement for
 *   sprintf(buf, "%06d", n);
 * instead of this you would use:
 *   pin_uitoa_formatted(n, buf, 10, 6, PIN_ITOA_ZERO_FILL | PIN_ITOA_RIGHT_JUSTIFIED);
 */
char* pin_uitoa_formatted(
	u_int32                 value,
	char*                   str_ptr,
	int32                   radix,
	int32                   width,
	u_int32                 flags);

/*
 * This function is the int64 version of the pin_itoa function.
 */
char* pin_i64toa(
	int64                   value,
	char*                   strp,
	int32                   radix);

/*
 * This function is the int64 version of the pin_uitoa function.
 */
char* pin_ui64toa(
	u_int64                 value,
	char*                   strp,
	int32                   radix);

/*
 * pin_i64toa_formatted
 * This function is an extended version of pin_i64toa.
 * It can be used as a replacement for
 *   sprintf(buf, "%" I64_PRINTF_PATTERN "d", n);
 * instead of this you would use:
 *   pin_i64toa_formatted(n, buf, 10, 6, PIN_ITOA_ZERO_FILL | PIN_ITOA_RIGHT_JUSTIFIED);
 */
char* pin_i64toa_formatted(
	int64                   value,
	char*                   str_ptr,
	int32                   radix,
	int32                   width,
	u_int32                 flags);

/*
 * pin_ui64toa_formatted
 * This function is an extended version of pin_ui64toa.
 * It can be used as a replacement for
 *   sprintf(buf, "%" I64_PRINTF_PATTERN "d", n);
 * instead of this you would use:
 *   pin_uitoa_formatted(n, buf, 10, 6, PIN_ITOA_ZERO_FILL | PIN_ITOA_RIGHT_JUSTIFIED);
 */
char* pin_ui64toa_formatted(
	u_int64                 value,
	char*                   str_ptr,
	int32                   radix,
	int32                   width,
	u_int32                 flags);

/*
 * pin_strcpy_rtn_end_ptr
 *
 * This function is similar to strcpy except it returns a ptr to
 * the NULL character at the end of the resulting string.
 *
 * This can be used to replace code like this:
 *    strcpy(cp, sp);
 *    cp += strlen(sp);
 *
 * with this:
 *    cp = pin_strcpy_rtn_end_ptr(cp, sp);
 */

char* pin_strcpy_rtn_end_ptr(
	char*                   dst,
	const char*             src);

/*
 * pin_strcpy_upto_char_rtn_end_ptr
 *
 * This function is similar to strcpy except it returns a ptr to
 * the NULL character at the end of the resulting string.
 *
 * This can be used to replace code like this:
 *    strcpy(cp, sp);
 *    cp += strlen(sp);
 *
 * with this:
 *    cp = pin_strcpy_rtn_end_ptr(cp, sp);
 */

const char* pin_strcpy_upto_char_rtn_end_ptr(
	char*                   dst,
	const char*             src,
	char                    a_char);

char *pin_stristr(char *haystack, char *needle);

int pin_snprintf(
        char*                   buffp,
        size_t                  len,
        const char*             fmt_strp,
	...);


#if defined(__cplusplus)
}
#endif

#endif	/* header file */
