/*
 *      
 * Copyright (c) 1996, 2023, Oracle and/or its affiliates.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

/*
 * include file for machine specific types of PIN things
 * should never include directly - via pcm.h or pcp.h
 */

#ifndef _PIN_TYPE_H_
#define _PIN_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include<time.h>
#if defined(sparc) || defined(mc68000) || defined(__hp9000s800) || defined(__linux) || defined(BSDI) || defined (rs6000) || defined(__ia64)

  #define INT_32 32
  #define HAS_LONGLONG

#elif defined(WINDOWS)

  #define INT_32 32
  #define HAS_INT64

#else
  #error Do not know the int size on your archicture.
#endif

#if defined(WINDOWS)
  #include <sys/types.h>
#endif

#if defined(__linux)
  #include <inttypes.h>
#endif /* defined(__linux) */

/* On Solaris, include <sys/int_limits.h> so we can avoid
 * the error -> macro redefined: INT64_MAX */
#if defined(sparc)
  #include <sys/int_limits.h>
#endif
#if defined(WINDOWS) || defined(__linux)
  #ifndef _CADDR_T
    #define _CADDR_T
    typedef char* caddr_t;
  #endif /* _CADDR_T */
#endif /* defined(__linux) */

#if !defined(WINDOWS)
  #if defined(INT_32)   /* Platform with 32 bit ints */
    /*
     * ASSUMES native long = 32 bits
     * ASSUMES native int = 32 bits
     * ASSUMES native short = 16 bits
     */
      typedef short int16;                  /* must be 16 bits */
      typedef unsigned short u_int16;       /* be 16 bits */
      typedef int int32;                    /* must be 32 bits */
      /* NOTE: u_int32 defined this was in <rpc/auth.h> */
      typedef unsigned int u_int32;         /* must be 32 bits */
  #elif defined(INT_16)   /* platform with 16 bit ints */
    /*
     * ASSUMES native long = 32 bits
     * ASSUMES native int = 16 bits
     * ASSUMES native short = 16 bits
     */
    typedef int int16;                      /* must be 16 bits */
    typedef unsigned int u_int16;           /* be 16 bits */
    typedef int int32;                      /* must be 32 bits */
    typedef unsigned int u_int32;           /* must be 32 bits */
  #endif
  #if defined (PIN_64_BIT)
    #define atoi64 atol                       /* define atoi64 as atol for non-windows 64-bit */
  #else
    #define atoi64 atoll                      /* define atoi64 as atoll for non-windows 32-bit */
  #endif
#else /*WINDOWS*/
  /*
   * ASSUMES native long = 32 bits
   * ASSUMES native int = 32 bits
   * ASSUMES native short = 16 bits
   */
  typedef __int16 int16;
  typedef unsigned short u_int16;
  typedef __int32 int32;                       /* must be 32 bits */
  typedef unsigned __int32 u_int32;            /* must be 32 bits */
  typedef unsigned long uint;
  typedef char* caddr_t;
  typedef __int64 int64;
  typedef unsigned __int64 u_int64;
  typedef unsigned char u_char;
  typedef unsigned short u_short;
  typedef unsigned int u_int;
  typedef unsigned long u_long;
  typedef _off_t  offset_t;
  #define atoi64 _atoi64                    /* define atoi64 as _atoi64 for windows */

#endif  /* !defined(WINDOWS) */

#if defined(HAS_LONGLONG)	/* really platform with 64 bit long long */
     #if defined (PIN_64_BIT)
       typedef long int64;
       typedef unsigned long u_int64;
     #else
       typedef long long int64;
       typedef unsigned long long u_int64;
     #endif
  #if defined(__linux)
     #define TIME_T_PRINTF_PATTERN "l"
     #define TIME_T_SCANF_PATTERN  "l"
     #ifdef PIN_64_BIT
       #define I64_PRINTF_PATTERN  "l"
       #define I64_SCANF_PATTERN   "l"
     #else
       #define I64_PRINTF_PATTERN  "ll"
       #define I64_SCANF_PATTERN   "ll"
     #endif
  #else
     #define I64_PRINTF_PATTERN  "ll"
     #define I64_SCANF_PATTERN   "ll"
     #ifdef PIN_64_BIT
       #define TIME_T_PRINTF_PATTERN         I64_PRINTF_PATTERN
       #define TIME_T_SCANF_PATTERN          I64_SCANF_PATTERN
     #else
       #define TIME_T_PRINTF_PATTERN
       #define TIME_T_SCANF_PATTERN
     #endif
  #endif
#elif defined(HAS_INT64)        /* probably a windowsy platform with __int64 */
  typedef __int64 int64;
  typedef unsigned __int64 u_int64;
  #define I64_PRINTF_PATTERN    "I64"
  #define I64_SCANF_PATTERN     "I64"
#else                       /* build this ourselves */
  typedef union {
    int32	l[2];
    double	d;
  } int64;
  typedef union {
    u_int32 l[2];
    double	d;
  } u_int64;
  /* Cheat here a bit. The scanf won't really work */
  #define I64_PRINTF_PATTERN    "ld,%l"
  #define I64_SCANF_PATTERN     "l"
#endif  /* HAS_LONGLONG */

/* General union to allow breaking apart a 64-bit number */
typedef union {
  char                    as_char[8];
  unsigned char           as_u_char[8];
  int16                   as_int16[4];
  u_int16                 as_u_int16[4];
  int32                   as_int32[2];
  u_int32                 as_u_int32[2];
  int64                   as_int64;
  u_int64                 as_u_int64;
} int64_union;


#ifndef INT32_MAX
  #define INT32_MAX             2147483647
#endif

#ifndef UINT32_MAX
  #define UINT32_MAX            4294967295U
#endif

#if !defined (sparc)
  #ifndef INT64_MAX
    #define  INT64_MAX          9223372036854775807LL
  #endif

  #ifndef UINT64_MAX
    #define  UINT64_MAX         18446744073709551615ULL
  #endif
#endif

/**************************************************************************
 * some Portal types that are simple and needed in pin_errs.h or elsewhere
 * besides pcm.h.
 * (ie. public typedefs of opaque datatypes).
 *************************************************************************/
typedef int32 pin_beid_t;          /* the beid type */
typedef int64 pin_db_no_t;         /* the db number type (e.g. 0.0.0.1) */
typedef int32 pin_err_t;           /* the error type */
typedef int32 pin_err_class_t;     /* the error class type */
typedef int32 pin_err_facility_t;  /* the error facility type */
typedef int32 pin_err_line_no_t;   /* the error line number type */
typedef int32 pin_err_location_t;  /* the error location type */
typedef int32 pin_err_msg_id_t;    /* the error message id type */
typedef int32 pin_err_reserved_t;  /* the error reserved type */
typedef time_t pin_err_time_sec_t;  /* the error time in seconds type */
typedef time_t pin_err_time_usec_t; /* the error time in micro-seconds type */
typedef int32 pin_fld_num_t;       /* the field number type */
typedef short pin_fld_type_t;      /* the field type type */
typedef int32 pin_opcode_t;        /* the opcode type */
typedef int32 pin_opcode_flags_t;  /* the opcode flags type */
typedef int64 pin_poid_id_t;       /* the poid id type */
typedef int32 pin_poid_rev_t;      /* the poid rev type */
typedef char* pin_poid_type_t;     /* the poid type type */
typedef const char* pin_const_poid_type_t;     /* the poid type type */
typedef int32 pin_rec_id_t;        /* the flist record id type */

/*
 * pin_flist_t is an opaque structure, always used as a cookie/pointer
 */
typedef struct pin_flist {
        int32   dummy;
} pin_flist_t;

typedef void pin_poid_idx_t;

/*
 * error/status structure for pcm commands. Needed to pinpoint an error.
 */
typedef struct pin_errbuf {
    pin_err_location_t   location;      /* module that generated error */
    pin_err_class_t      pin_errclass;  /* type of error */
    pin_err_t            pin_err;       /* PIN specific error number */
    pin_fld_num_t        field;         /* field number that caused error (if known) */
    pin_rec_id_t         rec_id;        /* record id for previous field (if known) */
    pin_err_reserved_t   reserved;      /* field for internal debugging info */
        /* following is not sent on wire - valid locally only */
    pin_err_line_no_t    line_no;       /* line number (pcm local only) */
#if defined(__STDC__) || defined(PIN_USE_ANSI_HDRS) || defined(__cplusplus)
    const char*          filename;      /* file name (pcm local only) */
#else
    char*                filename;      /* file name (pcm local only) */
#endif /* __cplusplus */
    pin_err_facility_t   facility;      /* facility code (which sub-system) */
    pin_err_msg_id_t     msg_id;        /* unique with facility error id */
    pin_err_time_sec_t   err_time_sec;  /* when error happened - time_t secs */
    pin_err_time_usec_t  err_time_usec; /*   " " " - microsecond part */
    int32                version;       /* version of args */
    void*                argsp;         /* optional args flist */
    struct pin_errbuf*   nextp;         /* FUTURE: for optional chaining */
    int32                reserved2;     /* round out to power-of-two, future? */
} pin_errbuf_t;

/*
 * opaque typedef for C/C++ BigDecimal, ie PIN_FLDT_DECIMAL datatype
 */
typedef void pin_decimal_t;

/*
 * opaque typedef for the PIN_FLDT_TIME datatype
 */
typedef void pin_time_t;

/* Define a type for File Descriptors */
#if defined(WINDOWS)
  typedef void* pin_fd_t;
#else
  typedef int pin_fd_t;
#endif

/* REVISIT: These are not types and should not be here */
#if defined(BSDI)
  #define strtoll strtoq
#endif

/* FIONREAD exists in different locations on different platforms */
#if defined(__linux) || defined(BSDI)
  #include <sys/ioctl.h>
#elif defined(__SVR4)
  #include <sys/filio.h>
#endif

/*
 * Define a common way to suggest to the compiler to inline a function
 * Only needed for C code.  C++ code should use the standard keyword "inline".
 */
#ifndef __cplusplus
  #if defined(sparc)
    #define PIN_INLINE inline
  #else
    #define PIN_INLINE __inline
  #endif
#endif

/* Use EXPORT to make a function or global variable visible externally
**
*/
#if defined(WINDOWS)
  #ifdef _NO_DLLIMPORT
    #define IMPORT        extern
    #define EXPORT        extern
  #else
    #define IMPORT        __declspec(dllimport)
    #define EXPORT        __declspec(dllexport)
  #endif /* _NO_DLLIMPORT */
#else
  #define EXPORT
  #define IMPORT          extern
#endif

/* REVISIT: These are not types and should not be here */
#ifdef WINDOWS
  #ifndef strcasecmp
    #define strcasecmp(a,b)     stricmp(a,b)
    #define strncasecmp(a,b,c)  strnicmp(a,b,c)
  #endif  /* ! strcasecmp */

  /* For some reason Microsoft doesn't define ssize_t */
  #ifndef _SSIZE_T_DEFINED
    #ifdef PIN_64_BIT
      typedef signed __int64    ssize_t;
    #else
      typedef signed int        ssize_t;
    #endif
    #define _SSIZE_T_DEFINED
  #endif
#endif  /* WINDOWS */

/* REVISIT: These are not types and should not be here */
#include <stddef.h>    /* for size_t */

/* To avoid problems on Windows with redefining the malloc functions,
 * I will include the standard include files first before the DEFINEs
 * change their signature.  This avoids errors like:
 *   error C2375: 'heap_alloc' : redefinition; different linkage
 * This also applies to the strdup function.
 */
/* Actually pin_os_string.h includes stdlib.h and string.h
 * so do that for ALL platforms to avoid builds that fail on
 * one platform but succeed on another.
 */
#ifndef _pin_os_string_h_
  #include "pin_os_string.h"
#endif

#ifndef PIN_MALLOC
  #define pin_malloc(x)      malloc(x)
  #define pin_free(x)        free(x)
  #define pin_realloc(x,y)   realloc((x),(y))
  #define pin_strdup(x)      strdup(x)
#else  /*PIN_MALLOC*/
    #if defined(__STDC__) || defined(__cplusplus) || defined (PIN_USE_ANSI_HDRS)
      void* pin_malloc(size_t);
      void  pin_free(void*);
      void* pin_realloc(void*, size_t);
      char* pin_strdup(const char*);
    #else
      extern char* pin_malloc();
      extern int   pin_free();
      extern char* pin_realloc();
      extern char* pin_strdup();
    #endif
#endif  /*PIN_MALLOC*/

/* REVISIT: This REALLY shouldn't be here */
#if defined(WINDOWS) && defined(FOR_CM)
  #define malloc heap_alloc
  #define free   heap_free
  #define realloc heap_realloc
  #define calloc heap_calloc
  #define strdup heap_strdup
  #ifdef __CMPIN__
    EXPORT void* heap_alloc(size_t size);
    EXPORT void  heap_free(void* p);
    EXPORT void* heap_realloc(void* p, size_t size);
    EXPORT void* heap_calloc(size_t nitems, size_t size);
    EXPORT char* heap_strdup(const char*p);
  #else
    IMPORT void* heap_alloc(size_t size);
    IMPORT void  heap_free(void* p);
    IMPORT void* heap_realloc(void* p, size_t size);
    IMPORT void* heap_calloc(size_t nitems, size_t size);
    IMPORT char* heap_strdup(const char* p);
  #endif
#endif  /* WINDOWS and FOR_CM */

#if defined(WINDOWS)
  #if defined(FOR_DM) || defined(SM_ORA_DD_USE_DD)
    #define pin_malloc(x)       malloc(x)
    #define pin_free(x)	        free(x)
    #define pin_realloc(x,y)    realloc((x),(y))
    #define pin_strdup(x)       strdup(x)

    #define malloc heap_alloc
    #define free   heap_free
    #define realloc heap_realloc
    #define calloc heap_calloc
    #define strdup heap_strdup

    #if defined(_DM_UTILS_) || defined(__CMPIN__)
      EXPORT void* heap_alloc(size_t size);
      EXPORT void  heap_free(void* p);
      EXPORT void* heap_realloc(void* p, size_t size);
      EXPORT void* heap_calloc(size_t nitems, size_t size);
      EXPORT char* heap_strdup(const char* p);
    #else
      IMPORT void* heap_alloc(size_t size);
      IMPORT void  heap_free(void* p);
      IMPORT void* heap_realloc(void* p, size_t size);
      IMPORT void* heap_calloc(size_t nitems, size_t size);
      IMPORT char* heap_strdup(const char* p);
    #endif  /* _DM_UTILS_ */
  #endif
#endif  /* WINDOWS */

#ifdef __cplusplus
}
#endif


#endif /* _PIN_TYPE_H_ */
