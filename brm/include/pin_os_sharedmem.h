/*
 *	@(#)%Portal Version: pin_os_sharedmem.h:PlatformR2Int:2:2006-Sep-12 03:29:53 %
 *       
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *       
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_sharedmem_h_
#define _pin_os_sharedmem_h_

#ifndef WINDOWS
  #include <fcntl.h>
  #include <stdio.h>
  #include <sys/mman.h>
  #ifdef __sun
    extern int shm_open(const char *, int, mode_t);
    extern int shm_unlink(const char *);
  #endif
  #include <unistd.h>
  #define EXTERN extern
#else /* windows */
  #include <windows.h>
  #ifdef _NO_DLLIMPORT
    #define EXTERN __declspec(dllexport)
  #else
    #define EXTERN extern
  #endif
#endif

#if defined(__cplusplus)
extern "C" {
#endif
/*
 * return 1 is equivalent to true;
 * return 0 is equivalent to false;
 * pin_shm_remove() function removes the named object on unix
 * and does nothing on Windows
 */
EXTERN int 
pin_shm_remove(const char * name);

/*
 * pin_shm_create( )function opens (or optionally creates) 
 * a shared memory object named path.
 */
EXTERN void * 
pin_shm_create(const char * name,void * base_addr, size_t
size, long offset);


EXTERN void * 
pin_shm_open(const char * name, void * base_addr, size_t
size,long offset);


EXTERN int 
pin_shm_close(void * base_addr);
#if defined(__cplusplus)
}
#endif

#endif /* _pin_os_sharedmem_h_ */

