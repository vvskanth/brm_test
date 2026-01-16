/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */


#ifndef __DPARSE_H
#define __DPARSE_H


#include "pcm.h"

/**********************************************************************
 * API entry points.
 **********************************************************************/

extern int 
dparseDefFile(
	FILE *fp, 
	pin_flist_t *fld_flistp, 
	pin_flist_t *class_flistp, 
	pin_flist_t *impl_flistp );

extern int 
dparseDefFileExt( 
	FILE *fp, 
	pin_flist_t *fld_flistp, 
	pin_flist_t *class_flistp, 
	pin_flist_t *impl_flistp, 
	char *impl );

extern int 
dparseExportInterfaceFields( 
	FILE *fp, 
	pin_flist_t *fld_flistp, 
	pin_errbuf_t *ebufp );

extern int 
dparseExportInterfaceClasses( 
	FILE *fp, 
	pin_flist_t *obj_flistp, 
	pin_flist_t *ddfld_flistp, 
	pin_errbuf_t *ebufp );

extern int 
dparseExportImplClasses( 
	FILE *fp, 
	pin_flist_t *ddfld_flistp, 
	pin_flist_t *obj_flistp, 
	pin_errbuf_t *ebufp );

extern int dparseGetErrLine();

extern char * dparseGetErrMsg();

extern void dparseSetErrMsg( 
	char *msg);

extern int dparseIsEqual( 
	char *string1, 
	char *string2 );

extern void
dparseMergeClassFlist(
	char		*impl,
	pin_flist_t 	*class_flistp, 
	pin_flist_t 	*impl_flistp,
	pin_errbuf_t 	*ebufp);

extern int
dparseIsTopLevelClass(
	char		*name);

#endif 
