/*
 *	@(#) % %
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_dynload_h_
#define _pin_os_dynload_h_

#if defined(__cplusplus)
extern "C" {
#endif

typedef void *pin_dynload_function_type(void);

/** Load a dynamic library by name
  *
  * This method will load up a dynamic library. You provide the name of the
  * library to load. You just provide the base name part, not any file name
  * extension, i.e. if the library was foobar.dll, you would provide just
  * "foobar". The correct platform extension will be provided for you by this
  * call.
  *
  * Note that there are still platform specific semantics at play here. In
  * particular, what type of search paths are done and such. But these are
  * assumed to be dealt with correctly by external installation and system
  * configuration tools and that the libraries to load will be in the
  * appropriate places.
  *
  * Also note that this API increases the reference count of the library
  * loaded, even if its already loaded. Not until it is unloaded as many
  * times as it was loaded will the library be dropped from memory.
  *
  * @param  module_name The name of the module to load
  *
  * @return The handle to the library, or zero if it failed. This is a void
  *         pointer which really points to some platform specific handle
  *         type.
  */
void* pin_dynload_load_module(const char* const module_name);

/** Get a dynamic library by name
  *
  * This method will get a dynamic library. You provide the name of the
  * library to get. You just provide the base name part, not any file name
  * extension, i.e. if the library was foobar.dll, you would provide just
  * "foobar". The correct platform extension will be provided for you by this
  * call.
  *
  * Note that there are still platform specific semantics at play here. In
  * particular, what type of search paths are done and such. But these are
  * assumed to be dealt with correctly by external installation and system
  * configuration tools and that the libraries to get will be in the
  * appropriate places.
  *
  * @param  module_name The name of the module to get
  *
  * @return The handle to the library, or zero if it failed. This is a void
  *         pointer which really points to some platform specific handle
  *         type.
  */
void* pin_dynload_get_module(const char* const module_name);


/** Unload a dynamic library by handle
  *
  * This method will unload a previous loaded library. You pass in the handle
  * returned from a previous call to pin_dynload_load_module(). This API will
  * reduce the reference count of the library and, if that count goes to zerok
  * may unload the module from memory (though whether it really does is still
  * platform specific.)
  *
  * @param  module  The hande to the module to unload.
  */
void pin_dynload_unload_module(void* const module);


/** Get the address of a named entry point in a loaded library module.
  *
  * This function will find a named entry point in the library module represented
  * by the passed module handle and return the address.
  *
  * Note that such entry points must use (in the module being loaded, not in
  * your code which is loading it) the extern C calling convention, such
  * as in this example:
  *
  *     extern "C" void foobar();
  *
  * i.e. the function MUST be a C style calling convention. Otherwise, the
  * C++ compiler will mangle the name.
  *
  * @param  module  The handle to the module to load the entry point from It
  *                 must have been obtained via a previous call to
  *                 pin_dynload_load_module().
  * @param  function_name   The name of the function to load.
  *
  * @return If it fails, the return is a zero pointer. If it works the return
  *         is the address of the entry point. Since this API has to work
  *         for many forms of entry points, it is returned as a generic
  *         function pointer type. You must case this to the real type.
  */

pin_dynload_function_type*
pin_dynload_get_function(void* const module, const char* const function_name);

/*
** pin_dynload_get_pcm_function()
**
** Return a pointer to function 'function_name' in either the cmpin, dmpin, qmpin or
** portal module.
** (You will need to cast the result).
** Returns 0 on failure.
*/
pin_dynload_function_type *pin_dynload_get_pcm_function(const char* const function_name);

/** Get the last error from a previous dynamic load call
  *
  * If an error occurs in any of the other pin_dynload functions, this function
  * may be called immediately to return a string describing the error.
  *
  * @return A pointer to a string containing the error. You must use this
  *         string before calling any other pinos services, because it might
  *         get overwritten if you call another pinos API. You don't own the
  *         returned string, so don't try to delete it.
  */
const char* pin_dynload_get_error(void);

#if defined(__cplusplus)
}
#endif

#endif

