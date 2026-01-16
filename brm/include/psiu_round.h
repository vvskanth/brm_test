/* Continuus file information --- %full_filespec: psiu_round.h~4:incl:1 % */
/*
 * @(#)psiu_round.h 1.5 99/03/18
 *
 *      Copyright (c) 1998 - 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PSIU_ROUND_H_ 
#define _PSIU_ROUND_H_ 

#if defined(__STDC__) || defined(PIN_USE_ANSI_HDRS) || defined(__cplusplus)
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************
 * Rounding Functions
 * The function psiu_round_nearest() used to be called as
 * fm_utils_round() which was not rounding exactly. This function
 * has been rewritten here.
 * Parameters:
 * 	amountp		amount to be rounded
 *	precision	no. of digits to be rounded to.
 *******************************************************************/

extern void
psiu_round_nearest PROTO_LIST((
	double		*amountp,
	int		precision));

extern void
psiu_round_even PROTO_LIST((
	double		*amountp,
	int		precision));

extern void
psiu_round_up PROTO_LIST((
	double		*amountp,
	int		precision));

extern void
psiu_round_down PROTO_LIST((
	double		*amountp,
	int		precision));

extern int
dbl_cmp PROTO_LIST((
	double		d1,
	double		d2));

extern int
dbl_cmp_zero PROTO_LIST((
	double		d1));

extern int
dbl_equal PROTO_LIST((
	double		d1,
	double		d2));

extern int
dbl_is_zero PROTO_LIST((
	double		d1));

extern time_t
psiu_time_round_to_midnight_apply_offset PROTO_LIST((
	time_t		a_time,
	int		offset));

#ifdef __cplusplus
}
#endif

#endif /*_PSIU_ROUND_H_*/
