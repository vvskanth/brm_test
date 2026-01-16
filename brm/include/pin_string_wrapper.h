/*
* Copyright (c) 2022, 2022, Oracle and/or its affiliates.
 *
 *  This material is the confidential property of Oracle Corporation or its
 *  licensors and may be used, reproduced, stored or transmitted only in
 *  accordance with a valid Oracle license or sublicense agreement.
 *
 *
 *  this file contains  prototype of wrapper functions for String to number conversions
 */


#ifndef _PIN_STRING_WRAPPER_H

#define _PIN_STRING_WRAPPER_H

unsigned long long int  pin_strtoull ( char *str_input,int base,int32 is_ebuf_mandatory,const char *func_loc_name,
                   pin_errbuf_t *ebufp);

unsigned long int  pin_strtoul  ( char *str_input,int base,int32 is_ebuf_mandatory,const char *func_loc_name,
                   pin_errbuf_t *ebufp);

long long int  pin_strtoll ( char *str_input,int base,int32 is_ebuf_mandatory,const char *func_loc_name,
                   pin_errbuf_t *ebufp);

double pin_strtod( char *str_input,int32 is_ebuf_mandatory,const char *func_loc_name,
                   pin_errbuf_t *ebufp);

long int  pin_strtol  ( char *str_input,int base,int32 is_ebuf_mandatory,const char *func_loc_name,
                   pin_errbuf_t *ebufp);

float pin_strtof  ( char *str_input,int32 is_ebuf_mandatory,const char *func_loc_name,
                   pin_errbuf_t *ebufp);

int  pin_strtoi  ( char *str_input,int base,int32 is_ebuf_mandatory,const char *func_loc_name,
                   pin_errbuf_t *ebufp);

//Macros for above pin_functions

//base value
#define PIN_DECIMAL_BASE 10
#define PIN_OCTAL_BASE 8
#define PIN_HEX_BASE 16
#define PIN_BINARY_BASE 2
#define PIN_AUTO_BASE 0

//macro for optional values
#define PIN_MANDATORY_EBUF 1
#define PIN_OPTIONAL_EBUF 0

#endif //for _PIN_STRING_WRAPPER_H
