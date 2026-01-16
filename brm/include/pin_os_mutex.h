#ifndef _pin_os_mutex_h_
#define _pin_os_mutex_h_
/*
 *	@(#)%Portal Version: pin_os_mutex.h:PlatformR2Int:2:2006-Sep-12 03:30:27 %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

/* All OSs must define:
 *   PTHREAD_MUTEXATTR_DEFAULT
 *   PTHREAD_CONDATTR_DEFAULT
 *   PTHREAD_ATTR_DEFAULT
 *
 * Which are the default attributes to use when initialising/creating
 * mutexes, conds and threads respectively.
 */
#if defined(WINDOWS)

  typedef void* pthread_mutex_t;
  typedef void* pthread_cond_t;
  typedef int pthread_condattr_t;
  typedef int pthread_mutexattr_t;

  #define PTHREAD_MUTEXATTR_DEFAULT 0
  #define PTHREAD_CONDATTR_DEFAULT 0
  #define PTHREAD_ATTR_DEFAULT 0

  #if defined(__cplusplus)
    extern "C" {
  #endif

  /* Posix threads compatibility for NT - subset only
   * See POSIX for documentation
   */
  int  pthread_mutex_lock(pthread_mutex_t* mutex);
  int  pthread_mutex_unlock(pthread_mutex_t* mutex);
  int  pthread_mutex_init(pthread_mutex_t* mutex, pthread_mutexattr_t* attr);
  int  pthread_mutex_destroy(pthread_mutex_t *mutex);
  int  pthread_cond_init(pthread_cond_t* cond, pthread_condattr_t* attr);
  int  pthread_cond_destroy(pthread_cond_t* cond);
  int  pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);
  int  pthread_cond_signal(pthread_cond_t* cond);
  int  pthread_cond_broadcast(pthread_cond_t* cond);

  #if defined(__cplusplus)
    }
  #endif

#else

  #include <pthread.h>

  #define PTHREAD_MUTEXATTR_DEFAULT 0
  #define PTHREAD_CONDATTR_DEFAULT 0
  #define PTHREAD_ATTR_DEFAULT 0

#endif

#endif

