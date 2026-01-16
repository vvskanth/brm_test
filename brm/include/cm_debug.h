/*
 *      @(#) % %
 *    
* Copyright (c) 1996, 2010, Oracle and/or its affiliates. All rights reserved. 
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef CM_DEBUG_H
#define CM_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif


/*
 * for cm_debug - cm_main.c
 */
#define CM_MAIN_DEBUG_TRACE	0x0001

/*
 * for cmap_debug - in cm_child, cm_cache, and cm_login_xxx
 */
#define CMAP_DEBUG_TRACE                 0x0001
#define CMAP_DEBUG_CMBUF                 0x0002
#define CMAP_DEBUG_TRACE_FM              0x0004  /* trace FM calls (no flists) */
#define CMAP_DEBUG_TIME_COUNTER          0x0008  /* time multi node_search */
#define CMAP_DEBUG_TRANS_OUT_PR          0x0010
#define CMAP_DEBUG_TRANS_IN_PR           0x0020
#define CMAP_DEBUG_PRINT_OUT_OP          0x0040
#define CMAP_DEBUG_PRINT_IN_OP           0x0080
#define CMAP_DEBUG_ERROR                 0x0100
#define CMAP_DEBUG_TXN_CACHE             0x0200
#define CMAP_DEBUG_CM_AUTH               0x0400
#define CMAP_DEBUG_PRINT_TO_DM           0x1000
#define CMAP_DEBUG_PRINT_FROM_DM         0x2000
#define CMAP_DEBUG_RFLDS_CACHE           0x4000  /* hits, fills */
#define CMAP_DEBUG_RFLDS_CACHE_DETAILS   0x8000
/* 4 bits for the cm_cache.c */
#define CMAP_DEBUG_CACHE_INIT           0x10000
#define CMAP_DEBUG_CACHE_USE            0x20000
#define CMAP_DEBUG_CACHE_DETAIL         0x40000
/* a bit of a security hole, but so is loglevel DEBUG */
#define CMAP_DEBUG_PRINT_LOGIN         0x100000


#ifdef __cplusplus
}
#endif

#endif /*CM_DEBUG_H*/
