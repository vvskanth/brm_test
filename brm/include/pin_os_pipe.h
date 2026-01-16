/*
 *      @(#)%Portal Version: pin_os_pipe.h:PortalBase7.3ModularInt:1:2007-Mar-06 13:44:34 %
 *      
 *      Copyright (c) 1996 - 2007 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_pipe_h_
#define _pin_os_pipe_h_

#ifndef _PIN_TYPE_H_
  #include "pin_type.h"
#endif

#if defined(WINDOWS)
	#include <windows.h>
	/*
	 * The pin_pipe_t for NT
	 * This structure contains the fields needed to use the pipe.
	 */
	typedef struct pin_pipe {
		HANDLE		in_pipe;
		HANDLE		out_pipe;
		int		err_val;
	} pin_pipe_t;


#else
	/* Unix */
	#include <unistd.h>
#ifdef __linux
#include <errno.h>
#else
	extern int errno;
#endif

	/*
	 * The pin_pipe_t for Unix
	 * This structure contains the fields needed to use the pipe.
	 */
	typedef struct pin_pipe {
		int		in_pipe;     /* pipe file descriptors */
		int		out_pipe;
		int		err_val;
	} pin_pipe_t;

#endif
	
#if defined(__cplusplus)
extern "C" {
#endif

/* 
 * pin_pipe_open - Upon successful completion, 0 is returned.
 * Otherwise, -1  is returned and errno is set to indicate the error.
 */
int pin_pipe_open(pin_pipe_t* pipe_p);
/* 
 * pin_pipe_close - Upon successful completion, 0 is returned.
 * Otherwise, -1  is returned and errno is set to indicate the error.
 */
int pin_pipe_close(pin_pipe_t* pipe_p);
/* 
 * pin_pipe_read - Upon successful completion, the number of bytes read is returned.
 * Otherwise, -1  is returned and errno is set to indicate the error.
 */
ssize_t pin_pipe_read(
	pin_pipe_t*	pipe_p,
	char*		buf_p,
	size_t		max_bytes_to_read);
/* 
 * pin_pipe_write - Upon successful completion, the number of bytes written is returned.
 * Otherwise, -1  is returned and errno is set to indicate the error.
 */
ssize_t pin_pipe_write(
	pin_pipe_t*	pipe_p,
	const char*	buf_p,
	size_t		num_bytes_to_write);

#if defined(__cplusplus)
}
#endif
#endif

