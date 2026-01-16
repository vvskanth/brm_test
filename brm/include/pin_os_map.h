/*
 *	@(#) % %
 *
 *	Copyright (c) 1996 - 2023 Oracle.
 *	
 *	This material is the confidential property of Oracle Corporation or its
 *	licensors and may be used, reproduced, stored or transmitted only in
 *	accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _pin_os_map_h_
#define _pin_os_map_h_

#ifndef _RWSTD_MAP_INCLUDED
  #include <map>
#endif

// This macro should be used when looping through elements and 
// deleting some of them. Remove_if cannot be used easily for maps
// because the element value type (the key) is always const, which breaks 
// copy_remove_if used internally in remove_if.
// On hpux the current thread hangs if the return iterator is not
// collected and used in the next step of the loop.
#if defined(__sun) || defined(__linux) 
#define STL_ERASE(__erase_map__,__erase_iterator__,__iterator_type__) \
         __iterator_type__ __prev_iterator__ = __erase_iterator__; \
         __erase_iterator__##++; \
	 __erase_map__##.erase (__prev_iterator__)
#else
#define STL_ERASE(__erase_map__,__erase_iterator__,__iterator_type__) \
	__erase_iterator__ = __erase_map__.erase (__erase_iterator__) 
#endif

#endif
