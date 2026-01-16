/*
 *      @(#)%Portal Version: pinlog.h:CUPmod7.3PatchInt:1:2006-Nov-29 16:05:46 %
 *      
* Copyright (c) 1996, 2023, Oracle and/or its affiliates.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */


#ifndef _PINLOG_H
#define _PINLOG_H

#ifndef _PCM_H
  #include "pcm.h"
#endif

#ifndef _PIN_TYPE_H_
  #include "pin_type.h"
#endif
#ifndef _pin_sys_h_
  #include "pin_sys.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Setup the declaration stuff correctly */
#ifdef WINDOWS
  #if  defined(PINLOG_DLL) || defined(_PINLOG_)
    #define PINLOG_EXPORT              __declspec(dllexport)
  #else
    #ifdef _NO_DLLIMPORT
      #define PINLOG_EXPORT            extern
    #else
      #define PINLOG_EXPORT            __declspec(dllimport)
    #endif /* _NO_DLLIMPORT */
  #endif /* PINLOG_DLL */
#else
  #define PINLOG_EXPORT                extern
#endif

    /* Error codes.
    */

#define LOG_NO_ERROR        68001
#define LOG_NO_FILE         68002
#define LOG_CANT_ACCESS     68003
#define LOG_NOT_INITIALIZED 68004
#define PIN_VIRT_TIME_ERR   68005


    /*  This file is used ONLY if we cannot open the
        file specified in the configuration file.
    */


#define LOG_FILE  "default.pinlog"

#define LOG_HOSTNAME_LENGTH     64
#define LOG_NAME_LENGTH         64
#define LOG_MACHINE_LENGTH      64
#define LOG_INSTANCE_LENGTH     64
/* Context info details added to correlation info = 255, 128*3, 4 colons */
#define CONTEXT_INFO_LENGTH 	664 
/* define length of correlation id +60 bytes for more stuff */
#define PINLOG_CORRELATION_SIZE	LOG_NAME_LENGTH+LOG_MACHINE_LENGTH+CONTEXT_INFO_LENGTH+60

#define LOG_SEVERITY_ERROR          'E'
#define LOG_SEVERITY_ASSERTION      'A'
#define LOG_SEVERITY_WARNING        'W'
#define LOG_SEVERITY_MESSAGE        'M'
#define LOG_SEVERITY_DEBUG          'D'
#define LOG_SEVERITY_UNKNOWN        '?'
#define LOG_SEVERITY_BACKGROUND     'B'
#define LOG_SEVERITY_CLIENT_CONTEXT  'A'

#define LOG_FLAG_ASSERTION   0x00000001
#define LOG_FLAG_ERROR       0x00000002
#define LOG_FLAG_WARNING     0x00000004
#define LOG_FLAG_DEBUG       0x00000008
#define LOG_FLAG_ERRNO       0x00000010
#define LOG_FLAG_MESSAGE     0x00000020
#define LOG_FLAG_DUMP        0x00000040
#define LOG_FLAG_SECURITY    0x00000080
#define LOG_FLAG_CLIENT_CONTEXT 0x00000100

#define LOG_FLAG_ALL (LOG_FLAG_ASSERTION | LOG_FLAG_ERROR | LOG_FLAG_WARNING \
    | LOG_FLAG_DEBUG | LOG_FLAG_MESSAGE)
#define LOG_FLAG_SERIOUS     (LOG_FLAG_ASSERTION | LOG_FLAG_ERROR | LOG_FLAG_SECURITY)

#define LOG_DATE_TIME_FORMAT_0  "%c"
#define LOG_DATE_TIME_FORMAT_1  "%Y-%m-%d %H:%M:%S"

/* size of built-in buffers */
#define LOG_BUFFER_LENGTH       1024

/* size of overflow message */
#define LOG_OVERFLOW_LENGTH     128

/*
 * this buffer, of LOG_BUFFER_LENGTH, is available for anyone to use
 * to sprintf() their message into before calling pinlog().
 * (instead of putting something on your stack - for example).
 */
PINLOG_EXPORT char PinLog_buffer[LOG_BUFFER_LENGTH];

/* for use with pinlog_buf(), offset your data starting here in your buffer */
/* We need to have space for new correlation id fields so 256 is not enough */ 
/* is approx 256+256+832 so let us round to 1.5K  1536 */
#define PINLOG_BUF_USER_OFFSET  1536

/* REVISIT: Flags should be available via pinlog_flags_get() */
PINLOG_EXPORT int32 PinLog_flags;

PINLOG_EXPORT int32 PinLog_format_version;

/*
 * You can use the following defines to reduce the overhead in making certain
 * pinlog calls where you need to pass data that needs to be constructed.
 *
 * For example, instead of this:
 *   sprintf(msg, "value 1 is %d, name is %s, value 2 is %d", val1, name, val2);
 *   pinlog(__FILE__, __LINE__, LOG_FLAG_DEBUG, "%s\n", emsg);
 *
 * you can do this instead:
 *   if (PINLOG_LOGLEVEL_IS_DEBUG) {
 *     sprintf(msg, "value 1 is %d, name is %s, value 2 is %d", val1, name, val2);
 *     pinlog(__FILE__, __LINE__, LOG_FLAG_DEBUG, "%s\n", emsg);
 *   }
 *
 */
#define PINLOG_LOGLEVEL_IS_DEBUG       (PinLog_flags & LOG_FLAG_DEBUG)
#define PINLOG_LOGLEVEL_IS_WARNING     (PinLog_flags & LOG_FLAG_WARNING)
#define PINLOG_LOGLEVEL_IS_ERROR       (PinLog_flags & LOG_FLAG_ERROR)

PINLOG_EXPORT char* pinlog_gen_correlation(
                pin_opcode_t         opcode,
                pin_err_t*           errp);

PINLOG_EXPORT char* pinlog_gen_correlation_ctxinfo(
                pin_opcode_t         opcode,
                pin_err_t*           errp,
                char *           ctxinfo);

PINLOG_EXPORT char* pinlog_set_correlation(
                char*                correlation,
                pin_err_t*           errp);

PINLOG_EXPORT char* pinlog_get_correlation();

PINLOG_EXPORT int64 pinlog_get_start_time();

PINLOG_EXPORT int32 pinlog_get_timeout();

PINLOG_EXPORT int64 pinlog_get_start_time_from_correlation (char * corid);

PINLOG_EXPORT int32 pinlog_get_timeout_from_correlation (char * corid);

PINLOG_EXPORT int pinlog_op_timeout_occured ();

PINLOG_EXPORT void pinlog_free_child();

PINLOG_EXPORT void pinlog_name_set(
                char*                name,
                pin_err_t*           error_code);

PINLOG_EXPORT void pinlog_path_set(
                char*                path,
                pin_err_t*           error_code);

PINLOG_EXPORT void pinlog_flags_set(
                int32                flags,
                pin_err_t*           error_code);

PINLOG_EXPORT int32 pinlog_flags_get();

PINLOG_EXPORT void pinlog_nmgr_setup(
                int                  argc,
                char**               argv,
                pin_err_t*           errp);

PINLOG_EXPORT void pinlog_nmgrlog_path_set(
                char path[],
                pin_err_t*           error_code);

PINLOG_EXPORT void pinlog_set_format_version(
                int32                format_version,
                pin_err_t*           error_code);
#ifndef WINDOWS
  PINLOG_EXPORT void pinlog_set_terminal_log_state(
                int32                flag,
                pin_err_t*           error_code);
#endif
PINLOG_EXPORT void pinlog_background_set(
                char*                format,
                ...);

PINLOG_EXPORT void pinlog_background_clear();

PINLOG_EXPORT void pinlog(
                const char*          src_file,
                int32                line,
                int32                flags,
                const char*          format,
                ...);

PINLOG_EXPORT void pinlog_buf(
                const char*          src_file,
                int32                line,
                int32                flags,
                char*                buf,
                int32                length);

PINLOG_EXPORT int pin_gethostname(char *hostname,
                           size_t len,
                           pin_err_t*   errp) ;


	/* NOTE: user data goes at PINLOG_BUF_USER_OFFSET from start of buf */
	/* NOTE: length is just length of valid user data */

#undef	PINLOG_EXPORT

#ifdef __cplusplus
}
#endif

/*added to take care of sigsend which is not present in AIX */
/* pidsig is used instead which does the same thing but the first parameter
is absent */
/* pidsig is to be called if the first parameter to sigsend is P_PID which h
as the value of 0 */
/* P_PID defined in AIX in <sys/wait.h>, it is defined to have the value 1,
in solaris it has the value 0 */
/* P_PID is part of enumeration idtype_t. if the enumeration is redefined */
/* things may go wrong??? */

#endif	/* ! _PINLOG_H */
