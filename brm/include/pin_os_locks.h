/* 
 * 	@(#)%Portal Version: pin_os_locks.h:RWSmod7.3.1Int:2:2007-Oct-09 02:27:42 %
 *
* Copyright (c) 1996, 2023, Oracle and/or its affiliates. 
 * 
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_locks_h_
#define _pin_os_locks_h_

#if defined(__cplusplus)
extern "C" {
#endif

/******************
 * Solaris section
 ******************/
#if defined(__sun)
#include <sys/lwp.h>
#include <synch.h>
#include <pthread.h>

/** Spinlocks */
typedef int spin_lock_attr_t;	
typedef struct spin_lock
{
		unsigned long _spin_lock;
		spin_lock_attr_t _attr;
} spin_lock_type;
#define PIN_SPIN_ONLY -1
#define PIN_UNLOCKED 0
#define PIN_LOCKED 1

int spin_lock_set_spin_count(spin_lock_attr_t*, int);
int spin_lock_init(spin_lock_type*, const spin_lock_attr_t*);
int spin_lock_acquire(spin_lock_type*);
int spin_lock_release(spin_lock_type*);
int spin_lock_destroy(spin_lock_type*);

/** Real mutexes */
typedef lwp_mutex_t real_mutex_t;
typedef int real_mutex_attr_t;

#define real_mutex_acquire _lwp_mutex_lock
#define real_mutex_release _lwp_mutex_unlock

int real_mutex_init(real_mutex_t*, real_mutex_attr_t*);
int real_mutex_destroy(real_mutex_t*);

/** Latches */
typedef pthread_mutex_t latch_t;
typedef pthread_mutexattr_t latch_attr_t;

#define latch_init    pthread_mutex_init
#define latch_acquire pthread_mutex_lock
#define latch_release pthread_mutex_unlock
#define latch_destroy pthread_mutex_destroy

/******************
 * Linux/Aix section
 ******************/
#elif defined(__linux)
#include <pthread.h>

/** Spinlocks */
typedef int spin_lock_attr_t;
typedef struct spin_lock
{
                unsigned long _spin_lock;
                spin_lock_attr_t _attr;
} spin_lock_type;
#define PIN_SPIN_ONLY -1
#define PIN_UNLOCKED 0
#define PIN_LOCKED 1

int spin_lock_set_spin_count(spin_lock_attr_t*, int);
int spin_lock_init(spin_lock_type*, const spin_lock_attr_t*);
int spin_lock_acquire(spin_lock_type*);
int spin_lock_release(spin_lock_type*);
int spin_lock_destroy(spin_lock_type*);

/** Real mutexes */
typedef pthread_mutex_t real_mutex_t;
typedef pthread_mutexattr_t real_mutex_attr_t;

/* #int real_mutex_init(real_mutex_t*, real_mutex_attr_t*); */
#define real_mutex_init    pthread_mutex_init
#define real_mutex_acquire pthread_mutex_lock
#define real_mutex_release pthread_mutex_unlock
#define real_mutex_destroy pthread_mutex_destroy

/** Latches */
typedef pthread_mutex_t latch_t;
typedef pthread_mutexattr_t latch_attr_t;

#define latch_init    pthread_mutex_init
#define latch_acquire pthread_mutex_lock
#define latch_release pthread_mutex_unlock
#define latch_destroy pthread_mutex_destroy

/******************
 * HPUX section
 ******************/
#elif defined(WINDOWS)
#ifndef IA64
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0403
#endif
#include <windows.h>

typedef int spin_lock_attr_t;
typedef CRITICAL_SECTION spin_lock_type;
/* On NT, you can't say spin for ever. The input is a DWORD, and the max value in a DWORD
 * is 2^31 - 1 = 2147483647
 */
#define PIN_SPIN_ONLY 2147483647

int spin_lock_set_spin_count(spin_lock_attr_t*, int);
int spin_lock_init(spin_lock_type*, const spin_lock_attr_t*);

#define spin_lock_acquire EnterCriticalSection
#define spin_lock_release LeaveCriticalSection
#define spin_lock_destroy DeleteCriticalSection

typedef CRITICAL_SECTION real_mutex_t;
typedef int real_mutex_attr_t;

int real_mutex_init(real_mutex_t*, real_mutex_attr_t*);
#define real_mutex_acquire EnterCriticalSection
#define real_mutex_release LeaveCriticalSection
#define real_mutex_destroy DeleteCriticalSection

typedef CRITICAL_SECTION latch_t;
typedef int latch_attr_t;

int latch_init(latch_t*, latch_attr_t *);

#define latch_acquire EnterCriticalSection
#define latch_release LeaveCriticalSection
#define latch_destroy DeleteCriticalSection

// no other platform supported yet
#endif

#if defined(__cplusplus)
}
#endif			 

#endif
