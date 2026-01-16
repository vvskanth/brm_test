/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _1_pin_zonemap_h_H
#define _1_pin_zonemap_h_H

/*******************************************************************
 * Zonemap FM Definitions.
 *******************************************************************/

/*******************************************************************
 * Object Name Strings
 *******************************************************************/
#define PIN_OBJ_NAME_EVENT_ZONEMAP		"Zonemap Event Log"

/*******************************************************************
 * Object Type Strings
 *******************************************************************/
#define PIN_OBJ_TYPE_ZONEMAP			"/zonemap"

#define PIN_OBJ_TYPE_EVENT_ZONEMAP	"/event/zonemap/zonemap"

/*******************************************************************
 * Event Description Strings
 *******************************************************************/

#define PIN_EVENT_DESCR_ZONEMAP_CREATE_ZONEMAP	"Creating: Zonemap"
#define PIN_EVENT_DESCR_ZONEMAP_UPDATE_ZONEMAP	"Updating: Zonemap"
#define PIN_EVENT_DESCR_ZONEMAP_UPDATE_ZONEMAP_NAME	"Changing name: Zonemap"
#define PIN_EVENT_DESCR_ZONEMAP_DELETE_ZONEMAP	"Deleting: Zonemap"

/*************************************************************
 * Zonemap search types
**************************************************************/
typedef enum pin_zonemap_search_type {
	PIN_ZONEMAP_PREFIX		=		100,
	PIN_ZONEMAP_EXACT		=		101
} pin_zonemap_search_type_t;

/*************************************************************
 * Zonemap data types
**************************************************************/
typedef enum pin_zonemap_data_type {
	PIN_ZONEMAP_DATA_FLIST		=	150,
	PIN_ZONEMAP_DATA_XML		=	151,
	PIN_ZONEMAP_DATA_BINARY		=	152
} pin_zonemap_data_type_t;

/*******************************************************************
 * Enum to hold various zonemap fm failure codes
 *******************************************************************/
typedef enum pin_zonemap_err {
	PIN_ZONEMAP_ERR_NONE						= 200,
	PIN_ZONEMAP_ERR_NO_NAME					= 201,
	PIN_ZONEMAP_ERR_DUPLICATE				= 202,
	PIN_ZONEMAP_ERR_ILLEGAL_ZONENAME		= 203,
	PIN_ZONEMAP_ERR_ILLEGAL_DATA			= 204,
	PIN_ZONEMAP_ERR_ILLEGAL_RELATION		= 205,
	PIN_ZONEMAP_ERR_UNKNOWN					= 206
} pin_zonemap_err_t;

#define STEP_SEARCH_MAX		200

/*****************************************************************
 * Delimiter for zonemap lineages.
 * The same delimiter is used in rating engine, therefore,
 * this header file needs to be included in rating engine code.
 *****************************************************************/
#define PIN_LINEAGE_DELIMITER		";"

#endif
