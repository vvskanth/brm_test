/*
 *	@(#)%Portal Version: pin_os_unixuid.h:PlatformR2Int:2:2006-Sep-12 03:32:15 %
 *	
 *	Copyright (c) 1996 - 2023 Oracle. 
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_unixuid_h_
#define _pin_os_unixuid_h_

/*
** Provide the following functions for all platforms:
**   getuid()
**   getpwuid()
**
** Platforms which have real UNIX password databases and uids/gids
** define HAS_UNIX_PASSWORD and support:
**   pin_getpwnam()
**
** Platforms which have shadow passwords define HAS_UNIX_SHADOW_PASSWORD
** and support:
**  pin_getspnam()
**
** These are MT-safe.
*/

#if defined(__unix)

#include <pwd.h>
#include <crypt.h>

#define HAS_UNIX_PASSWORD

#define UNIX_GETPWNAM_LEN 256

/*
** pin_getpwnam()
**
** Like getpwnam() except:
**   Returns 0 if OK, or -1 if lookup failed or other error.
**   If OK, fills in '*pwd' with the results.
**   'buffer' must point to a buffer (of size at least UNIX_GETPWNAM_LEN).
**   'buflen' is the length of this buffer.
**
** This function is MT-safe.
*/
/*int pin_getpwnam(const char *name, struct passwd *pwd, char *buffer, size_t buflen);*/

/*
** pin_getspnam()
**
** MT-safe version of getspnam(). See pin_getpwnam() above for more details.
*/
/*int pin_getspnam(const char *name, struct spwd *spwd, char *buffer, size_t buflen);*/

#if defined(__sun)

#include <shadow.h>

#define HAS_UNIX_SHADOW_PASSWORD

#define pin_getpwnam(NAME, PWD, BUFFER, BUFLEN) ((getpwnam_r((NAME), (PWD), (BUFFER), (BUFLEN)) == 0) ? -1 : 0)
#define pin_getspnam(NAME, SPWD, BUFFER, BUFLEN) ((getspnam_r((NAME), (SPWD), (BUFFER), (BUFLEN)) == 0) ? -1 : 0)


#elif defined(__linux)

/* Note: HPUX has TRUST support for shadow passwords, but there doesn't seem to
         be an MT-safe interface to that, so we don't support pin_getpsnam() on HPUX
*/
#if defined(__cplusplus)
extern "C" {
#endif

#define DLL_SPEC (BUILDING_PINOS == 1)
#include <dlldef.h>
PUBLIC int pin_getpwnam(const char* name, struct passwd* pwd, char* buffer, size_t buflen);

#if defined(__cplusplus)
}
#endif

#else

#error Unix platform with no getpwnam_r() equivalent!

#endif

#endif

#endif
