/*
 *      @(#) % %
 *      
 *      Copyright (c) 1996 - 2008 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_dir_h_
#define _pin_os_dir_h_

/* Define a few directory related APIs and constants */
#if defined(WINDOWS)
    #include <direct.h>
    #include <sys/stat.h>
    #define S_IFDIR _S_IFDIR
    #define S_ISDIR(mode) (((mode) & S_IFDIR) == S_IFDIR)
    #define mkdir(NAME, MODE)  _mkdir(NAME)
#elif defined(__unix)
    #include <sys/stat.h>
    #include <sys/param.h>
#endif


/* Define some file system constants */
#ifdef WINDOWS
#define PIN_MAXPATHLEN      260
#define PIN_PATHSEP         '\\'
#define PIN_PATHSEPSTR      "\\"
#define PIN_PATHLISTSEP     ';'
#else
#define PIN_MAXPATHLEN      256
#define PIN_PATHSEP         '/'
#define PIN_PATHSEPSTR      "/"
#define PIN_PATHLISTSEP     ':'
#endif
#define PIN_MAXHOSTNAMELEN  256


/* And provide some non-wrapper helper functions */
#if defined(__cplusplus)
extern "C" {
#endif

/** Split last component of a path/file name out in OS independent fashion.
  *
  * If in NT "\" will be used as the path delimiter. If in Solaris or HP,
  * "/" will be used.
  *
  * On buffer length too short or other errors, -1 will be returned, path_buf and
  * last_item_buf will be cleared with '\0'.
  *
  * The buffers path_buf & last_item_buf at least need to be long
  * enough to hold the parsed result. Recommend to have same size as the
  * input pathName.
  *
  * The slash seperating the remaining path name and the last path component
  * is NOT retained.
  *
  * Example : path1/path2/path3/file -> path1/path2/path3 , file
  *           path1/path2/path3 -> path1/path2 , path3
  *           file1 -> "" , file1
  *
  * @return  On sucessful parse, 0 is returned.
  */
int pin_parse_path
(
    const char *pathName,
    char* const path_buf,
    const size_t path_buf_len,
    char* const last_item_buf,
    const size_t last_item_buf_len
);

#if defined(__cplusplus)
}
#endif


/*  PIN_DEPRECATED_STUFF
 *
 *  These things are now deprecated. They are here still in order to make the
 *  transition easier (i.e. so that the whole code base doesn't need to be
 *  swatted at once in order to update pinos.
 *
 *  To check your code against the new stuff, define the PIN_NODEP_PINOS token
 *  for your build, which will force all deprecated PINOS stuff off.
 */
#ifndef PIN_NODEP_PINOS
    /* Temporarily define the old parse path to the new */
    #define parsePath   pin_parse_path

    /* Temporarily provide the old max length defines */
    #ifndef WINDOWS
        #ifndef MAXPATHLEN
        #define MAXPATHLEN 256
        #endif
    #endif

    #ifndef MAXHOSTNAMELEN
    #define MAXHOSTNAMELEN 256
    #endif

    /* And the old slashes */
    #ifdef WINDOWS
    #define PATH_SLASH '\\'
    #else
    #define PATH_SLASH '/'
    #endif
#endif

#endif

