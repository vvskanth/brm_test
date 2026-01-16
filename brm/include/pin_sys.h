/*
 *	@(#) % %
 *	
 *	Copyright (c) 1996 - 2023 Oracle.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_sys_h_
#define _pin_sys_h_

  #define PIN_EXPORT

  #define PIN_IMPORT extern

  #define PIN_THD_VAR

  /* tempnam */
  #define _tempnam(dir, pfx)		tempnam((dir), (pfx))

  /* other */
  #define pin_popen			popen
  #define pin_pclose			pclose


/* sockets */
#ifndef _pin_os_network_h_
  #include <pin_os_network.h>
#endif

/* threads */
#ifndef _pin_os_thread_h_
  #include <pin_os_thread.h>
#endif

#endif /* PIN_SYS_H */
