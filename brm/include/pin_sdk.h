/* continuus file information --- %full_filespec: pin_sdk.h~6:incl:3 % */
/*******************************************************************
 *
 *  @(#) %full_filespec: pin_sdk.h~6:incl:3 %
 *
 *      Copyright 1996 - 2009  Oracle Corporation. All rights reserved.
 *
 *      This material is the exclusive property and trade secret of
 *      Oracle Corporation. No part may be reproduced,
 *      disclosed, stored in a retrieval system, or otherwise
 *      transmitted in any form or by any means, electronic,
 *      mechanical, photocopying, optical, magnetic, chemical,
 *      biological, or otherwise, without prior written permission
 *      of Oracle Corporation.
 *
 *******************************************************************/

#ifndef _PIN_SDK_H
#define	_PIN_SDK_H


/*******************************************************************
 * Defines for PIN_FLD_POLICY to indicate if the opcode is a base
 * opcode or a policy opcode.
 *******************************************************************/
#define PIN_SDK_OP_NOT_POLICY	0
#define PIN_SDK_OP_POLICY	1

/*******************************************************************
 * Defines for PIN_FLD_BOUNDED to indicate if the input/output
 * field list for an opcode is bounded or not. 
 *******************************************************************/
#define PIN_SDK_OP_BOUNDED	0	/* fixed list of fields */
#define PIN_SDK_OP_UNBOUNDED	1	/* variable len list of fields */

/*******************************************************************
 * Defines for PIN_FLD_PERMISSIONS when describing a opcode's field.
 *******************************************************************/
#define PIN_SDK_OP_PERMS_REQUIRED	"Required"
#define PIN_SDK_OP_PERMS_OPTIONAL	"Optional"

/*******************************************************************
 * Defines for PIN_FLD_CREATE_PERMISSION when describing a object
 * type's field.
 *******************************************************************/
#define PIN_SDK_OBJ_CREATE_PERM_REQUIRED	"Required"
#define PIN_SDK_OBJ_CREATE_PERM_OPTIONAL	"Optional"
#define PIN_SDK_OBJ_CREATE_PERM_PROHIBITED	"Prohibited"
#define PIN_SDK_OBJ_CREATE_PERM_SYSTEM		"System"

/*******************************************************************
 * Defines for PIN_FLD_MOD_PERMISSION when describing a object
 * type's field.
 *******************************************************************/
#define PIN_SDK_OBJ_MOD_PERM_WRITEABLE	"Writeable"
#define PIN_SDK_OBJ_MOD_PERM_NOT_WRITEABLE	"Not_Writeable"
#define PIN_SDK_OBJ_MOD_PERM_SYSTEM	"System"

/*******************************************************************
 * STATUS (bit)flags defines used for PIN_FLD_STATUS field when reading
 * an obj spec.
 *******************************************************************/
#define PIN_SDK_STATUS_VALID		0x00000001
#define PIN_SDK_STATUS_SYSTEM		0x00000002

/*******************************************************************
 * General defines
 *******************************************************************/
#define GETSET_OPT_FLD_RESULTS( source_flp, destn_flp, field, ebufp )   \
        {                                                               \
                void*   vp=NULL;                                        \
                vp = PIN_FLIST_FLD_GET( source_flp, field, 1, ebufp );  \
                if( vp != NULL ) {                                      \
                        PIN_FLIST_FLD_SET( destn_flp, field,(void*)NULL,ebufp);\
                }                                                       \
        } /* GETSET_OPT_FLD_RESULTS */


#endif	/*_PIN_SDK_H*/
