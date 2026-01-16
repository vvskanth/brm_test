/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PIN_ACCESS_LEVEL_H
#define _PIN_ACCESS_LEVEL_H

/* Define valid levels for read/write */
#define PIN_ACCESS_LEVEL_SELF            	'S'
#define PIN_ACCESS_LEVEL_BRAND_LINEAGE   	'L'
#define PIN_ACCESS_LEVEL_BRAND_GROUP     	'B'
#define PIN_ACCESS_LEVEL_ANCESTRAL_LINEAGE 	'A'
#define PIN_ACCESS_LEVEL_GLOBAL          	'G'

/* Defines token strings valid for read/write */
#define PIN_ACCESS_LEVEL_SELF_STR		"Self"
#define PIN_ACCESS_LEVEL_BRAND_LINEAGE_STR	"BrandLineage"
#define PIN_ACCESS_LEVEL_BRAND_GROUP_STR	"Brand"
#define PIN_ACCESS_LEVEL_ANCESTRAL_LINEAGE_STR	"AncestralLineage"
#define PIN_ACCESS_LEVEL_GLOBAL_STR		"Global"

/* Define valid levels for creation */
#define PIN_ACCESS_LEVEL_BRAND_OWNER     	'O'
#define PIN_ACCESS_LEVEL_ROOT            	'R'
#define PIN_ACCESS_LEVEL_ANY             	'N'

/* String flavors of the above create levels */
#define PIN_ACCESS_LEVEL_BRAND_OWNER_STR	"BrandOwner"
#define PIN_ACCESS_LEVEL_ROOT_STR		"Root"
#define PIN_ACCESS_LEVEL_ANY_STR		"Any"

#endif
