/*
 *	@(#)%Portal Version: pin_os_time.h:RWSmod7.3.1Int:1:2007-Sep-12 04:46:04 %
 *
* Copyright (c) 1996, 2023, Oracle and/or its affiliates. 
 *  
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_time_h_
#define _pin_os_time_h_

#ifndef _pin_os_types_h_
  #include <pin_os_types.h>
#endif

#include <time.h>

#if defined(__unix)
  #include <sys/time.h>
  #include <sys/timeb.h>
#endif
#define PIN_MAX_HRTIME_T                  9223372036854775807LL
#define PIN_NANOS_IN_A_MILLI              1000000LL
#define PIN_NANOS_IN_A_SECOND             1000000000LL

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(EXTERN)
  #if defined(PUBLIC)
    #undef PUBLIC
  #endif
  #define PUBLIC  EXTERN
#endif

#ifndef PUBLIC
  #define PUBLIC
#endif

#if defined(WINDOWS)
  #include <winsock2.h>     /* for struct timeval */
  #include <sys/timeb.h>

  struct tm;
  struct timezone;
  PUBLIC struct tm* localtime_r(
	const time_t*           clock,
	struct tm*              res);

  PUBLIC int gettimeofday(
	struct timeval*         timeval,
	struct timezone*        tz);

  #define pin_timezone                    _timezone

  typedef int64   hrtime_t;
  PUBLIC hrtime_t gethrtime();
#else
  #if defined(__linux)
    #ifndef hrtime_t
       #define hrtime_t long long
    #endif
    hrtime_t gethrtime(void);
  #endif

  #define pin_timezone                    timezone
#endif


#define pin_gettimeofday                gettimeofday

/** Convert a tm structure to a time_t structure
  *
  * This API will convert a tm style time structure into a time_t style
  * time structure, similar to the Unix mktime() call.
  *
  * @param  tm  A tm type structure to convert
  * @return The passed tm structure converted to the time_t format.
  */
PUBLIC time_t tm_to_time_t(
	const struct tm*        tm);


/** Sleep for an indicated number of milliseconds
  *
  * This API will put the calling thread to sleep for the indicated
  * number of milliseconds. Note that it is not guaranteed that the thread
  * will really stay asleep that long. It could be longer (if other high
  * priority threads need the CPU) or somewhat shorter, according to how
  * according the system is about such sleep periods.
  *
  * @param  to_sleep    The number of milliseconds to sleep
  */
PUBLIC void pin_msec_sleep(
	const u_int32           to_sleep);


/** Sleep for an indicated number of seconds
  *
  * This API will put the calling thread to sleep for the indicated
  * number of seconds. Note that it is not guaranteed that the thread
  * will really stay asleep that long. It could be longer (if other high
  * priority threads need the CPU) or somewhat shorter, according to how
  * according the system is about such sleep periods.
  *
  * @param  to_sleep    The number of seconds to sleep
  */
PUBLIC void pin_sleep(
	const u_int32           to_sleep);


/** Get the milliseconds since 01/01/1970 (UTC)
  *
  * This API provides a simple mechanism for storing time stamps in a format
  * that is reasonable compact but which does represent an absolute time.
  * In order to hold the required number, a 64 bit unsigned value is used. So
  * be aware of the performance implementations on 32 bit systems.
  *
  * @return The number of milliseconds since the base time.
  */
PUBLIC u_int64 pin_msec_offset();


/** Format a time_t string to a string
  *
  * This API will format a passed time_t structure into a string you provide.
  * You also provide a format string, which has replacement tokens for the
  * various fields of the time structure.
  *
  * @param  str_to_fill This is the string into which the formatted time
  *                     is placed.
  * @param  max_size    The max chars to place into the passed string. If the
  *                     string is not large enough to hold the formatted string,
  *                     and the terminating null, then a zero is returned to
  *                     indicate failure.
  * @param  format_str  This string indicates how you want the time formatted.
  * @param  to_format   This is the time_t structure to format
  *
  * @return The number of characters placed into the string, not including the
  *         terminating null. If zero, then it failed.
  */

#define PIN_STRFTIMET

PUBLIC size_t pin_strftimet
(
	char* const             str_to_fill,
	size_t                  max_size,
	char* const             format_str,
	time_t                  to_format
);


/** A portable format structure for pin_strftimet().
  *
  * Since %c is not portable, this format gives the same information and is
  * identical on all platforms. Use it in preference to "%c"
  */
#define STRFTIME_STD_FORMAT             "%a %b %d %X %Y"


/* pin_hrtime_to_str
 * This formats the passed hrtime_t to a char buffer in the form of
 *    HH:MM.ss.nnnnnnnnn
 * The caller must pass a pointer to a char buffer that is >= 19 bytes.
 * No overflow checks are made.
 * This function returns a pointer to the end of the buffer (the
 * terminating NULL.
 ****** NOTE: This code doesn't handle DST changes.
 *            This code is intended for a fast way to print the
 *            time in nano-seconds (mainly for performance debugging).
 *            It shouldn't be used for anything else.
 */
char* pin_hrtime_to_str(
	char*               buf,
	hrtime_t            hrtime_buffer);


/* pin_now_hrtime_to_str
 * This formats the current time to a char buffer in the form of
 *    HH:MM.ss.nnnnnnnnn
 * The caller must pass a pointer to a char buffer that is >= 19 bytes.
 * No overflow checks are made.
 * This function returns a pointer to the end of the buffer (the
 * terminating NULL.
 ****** NOTE: This code doesn't handle DST changes.
 *            This code is intended for a fast way to print the
 *            time in nano-seconds (mainly for performance debugging).
 *            It shouldn't be used for anything else.
 */
char* pin_now_hrtime_to_str(
	char*           buf);

#if defined(__cplusplus)
}
#endif

#endif
