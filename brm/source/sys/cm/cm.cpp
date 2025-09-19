/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#include <stdio.h>
extern "C" int mainThread(int argc, char **argv);

/*********************************************************************
 * call mainThread() function from cmpin dynamic library, This source
 * file is only used in UNIX version of cm.  the main() is defined here.
 * NT version of CM.EXE gets its main() from nt_service.c.  They both
 * call mainThread() which is located in cmpin lib.
 */ 
int main(int argc, char **argv)
{
	return( mainThread( argc, argv));
}
