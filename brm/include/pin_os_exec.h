/*
 *	@(#) % %
 *  
 *	Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *  
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_exec_h_
#define _pin_os_exec_h_

#ifndef _pin_os_types_h_
#include <pin_os_types.h>
#endif

#ifndef _pin_os_thread_h_
#include <pin_os_thread.h>
#endif


#if defined(__cplusplus)
extern "C" {
#endif

#define PIN_EXEC_FOREVER    ((u_int32)-1)
#define PIN_EXECFLAG_NONE   0x0000


/** A magic cookie representing an external process
  *
  * When a new process is created, you get back a pointer to one of these
  * structures. It actually will be bigger than this since it will have
  * trailing, platform specific, information.
  */
typedef struct extproc_cookie_type
{
    pid_t   pid;
}   extproc_cookie;


/** Just invoke a script via a system shell
  *
  * This method does the simple invocation possible, of an external script
  * that you want to run within the current command shell. You just provide
  * the base part of the name, no extension. It can have a path if required.
  *
  * This is a synchronous call, so it waits for the script to complete and
  * returns the return value.
  *
  * @param  script_name The name of the script to invoke. It should not have
  *                     any extension, just the base name of the script. The
  *                     extension (if any) will be provided automatically.
  *
  * @param  params      An array of pointers to parameters. It can be null
  *                     if none. The last one should be a null pointer to
  *                     indicate the end of the list.
  *
  * @param  flags       One or more of the exec flags.
  *
  * @param  result_code Filled with the return value of the script.
  *
  * @return 1 if the script was found and executed, else 0
  */
int pin_os_run_script(  const char* const script_name,
                        const char** parm_list,
                        const u_int32 flags,
                        u_int32* const result_code);


/** Invoke an executable
  *
  * This method just invokes an executable. You provide the name of the
  * executable to execute. Don't provide an extension. You can provide
  * a path if it will not be findable via the standard means.
  *
  * You will be given back a cookie which represents the process. This is
  * NOT a process id, it is a platform specific representation of the process.
  * However, one of the fields is always the process id, so you can get it
  * via the ->pid field.
  *
  * @param  exe_name    The name of the executable to invoke. It should not
  *                     have any extension, just the base name of the app. The
  *                     extension (if any) will be provided automatically.
  *
  * @param  params      An array of pointers to parameters. It can be null
  *                     if none. The last one should be a null pointer to
  *                     indicate the end of the list.
  *
  * @param  flags       One or more of the exec flags.
  *
  * @return If successful, a pointer to an external process cookie, which you
  *         become responsible for, and must eventually free with a call to
  *         pin_os_exec_freecookie(). If it fails, the return is a null pointer.
  */
extproc_cookie* pin_os_exec(const char* const exe_name,
                            const char* const * parm_list,
                            const u_int32 flags);



/** Invoke an executable, and wait for it to terminate
  *
  * This method basically does pin_os_exec() and pin_os_exec_wait_for_death()
  * for you, so you don't have to do the work yourself. You get back the
  * termination code of the app.
  *
  * @param  exe_name    The name of the executable to invoke. It should not
  *                     have any extension, just the base name of the app. The
  *                     extension (if any) will be provided automatically.
  *
  * @param  params      An array of pointers to parameters. It can be null
  *                     if none. The last one should be a null pointer to
  *                     indicate the end of the list.
  *
  * @param  flags       One or more of the exec flags.
  *
  * @param  result_code Filled with the return value of the app.
  *
  * @return 1 if the successful, else 0.
  */
int pin_os_exec_wait(   const char* const exe_name,
                        const char** parm_list,
                        const u_int32 flags,
                        u_int32* const result_code);


/** Wait for a previously executed process to die
  *
  * If you execute an external process, and want to know when it has completed,
  * you can either use pin_os_exec_wait() or you can use pin_os_exec() and
  * at some point later come back and make sure its died.
  *
  * @param  cookie      A cookie obtained by a previous call to pin_os_exec().
  *
  * @param  result_code Filled with the return value of the app.
  *
  * @param  wait_millis Milliseconds to wait for it to die. Use PIN_EXEC_FOREVER
  *                     to wait forever.
  *
  * @return 1 if it was successful, else 0.
  */
int pin_os_exec_wait_for_death( extproc_cookie* const cookie,
                                u_int32* const result_code,
                                const u_int32 wait_millis);

/** Frees a previously obtrained process cookit
  *
  * After a call to pin_os_exec(), the caller recievers a pointer to a process
  * cookie. This cook it used to control the process or get information about it.
  * Once the cookie is no longer needed, this function should be called to clean
  * up the cookit.
  *
  * @param  to_clean    The cookie to clean up.
  *
  */
void pin_os_exec_free_cookie(extproc_cookie* const to_clean);

#if defined(__cplusplus)
}
#endif

#endif


