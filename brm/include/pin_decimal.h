/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996, 2023, Oracle and/or its affiliates.
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PIN_DECIMAL_H
#define _PIN_DECIMAL_H

#ifndef _PCM_H
  #include <pcm.h>
#endif

/* Setup the external declaration stuff correctly */
#define EXTERN				extern

/**************************************************************************
 * rounding modes for pin_decimal_divide() and pin_decimal_set_scale().
 * and pin_decimal_from_double().
 * Same names/functionality as Java BigDecimal.
 **************************************************************************/
/*
 * round up (away from zero).  If the discard part of the number is non-zero,
 * increment the new low order digit.
 */
#define ROUND_UP		1

/*
 * round down (towards zero).  This just truncates the number.
 */
#define ROUND_DOWN		2

/*
 * round ceiling (towards positive infinity).  Truncates negative numbers,
 * increment positive numbers if their discard part is non-zero.
 */
#define ROUND_CEILING	3

/*
 * round floor (towards negative infinity).   Truncates positive numbers,
 * increments low digit of negative number if discard part is non-zero. 
 */
#define ROUND_FLOOR		4

/*
 * round half up (towards nearest neighbor, and half goes up).
 * This is common rounding, if discard part is .5 or higher, round up;
 * if less, round down.
 */
#define ROUND_HALF_UP	5

/*
 * round half down (towards nearest neighbor, and half goes down).
 * If discard part is more than .5, round up; if .5 or less, round down.
 */
#define ROUND_HALF_DOWN	6

/*
 * round half even (towards nearest neighbor, and half goes to even).
 * If discard part is less than .5, round down; if more than .5, round up;
 * if .5 exactly, then round towards even digit.
 */
#define ROUND_HALF_EVEN	7


/*
 * ROUND_FLOOR_ALT is defined as follows -
 * if (number is positive)
 *	perform ROUND_DOWN_ALT (defined below)
 * if (number is negative)
 *	perform ROUND_UP
 */
#define ROUND_FLOOR_ALT 8


/*
 * ROUND_DOWN_ALT(decimal, scale) = ROUND_DOWN(ROUND_HALF_UP(decimal, scale+2), scale);
 */

#define ROUND_DOWN_ALT  9

/*
 * round unnecessary (do not round)
 * Is an error if used and rounding would happen.
 */
#define ROUND_UNNECESSARY	10

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * pin_decimal_t functions and macro - a subset of Java BigDecimal methods.
 *
 * Note: Direct use of these functions is deprecated in favour of the PBO interface.
 */
EXTERN pin_decimal_t *pin_decimal(const char *str, pin_errbuf_t *ebufp);
EXTERN pin_decimal_t *pin_decimal_alloc_result_zero(pin_errbuf_t *ebufp);
EXTERN pin_decimal_t *pin_decimal_from_double(double d, int32 scale,
	int roundingMode, pin_errbuf_t *ebufp);
EXTERN pin_decimal_t *pin_decimal_clone(const pin_decimal_t *p, pin_errbuf_t *ebufp);
EXTERN pin_decimal_t *pin_decimal_add(const pin_decimal_t *pdp1, const pin_decimal_t *pdp2,
	pin_errbuf_t *ebufp);
EXTERN pin_decimal_t *pin_decimal_abs(const pin_decimal_t *pdp, pin_errbuf_t *ebufp);
EXTERN void pin_decimal_abs_assign(pin_decimal_t *pdp, pin_errbuf_t *ebufp);
EXTERN int pin_decimal_compare(const pin_decimal_t *pdp1, const pin_decimal_t *pdp2,
	pin_errbuf_t *ebufp);
EXTERN pin_decimal_t *pin_decimal_divide(const pin_decimal_t *nump,
	const pin_decimal_t *byp, int32 scale, int roundingMode, pin_errbuf_t *ebufp);
EXTERN void pin_decimal_destroy(pin_decimal_t *pdp);
EXTERN int pin_decimal_is_NULL(const pin_decimal_t *pdp, pin_errbuf_t *ebufp);
EXTERN int pin_decimal_is_ZERO(const pin_decimal_t *pdp, pin_errbuf_t *ebufp);
EXTERN pin_decimal_t *pin_decimal_multiply(const pin_decimal_t *pdp1,
	const pin_decimal_t *pdp2, pin_errbuf_t *ebufp);
EXTERN pin_decimal_t *pin_decimal_negate(const pin_decimal_t *p, pin_errbuf_t *ebufp);
EXTERN void pin_decimal_negate_assign(pin_decimal_t *p, pin_errbuf_t *ebufp);
EXTERN int32 pin_decimal_scale(const pin_decimal_t *pdp1, pin_errbuf_t *ebufp);
EXTERN pin_decimal_t *pin_decimal_set_scale(const pin_decimal_t *pdp, int32 scale,
	int32 rounding_mode, pin_errbuf_t *ebufp);
EXTERN void pin_decimal_set_scale_assign(pin_decimal_t *pdp, int32 scale,
	int32 rounding_mode, pin_errbuf_t *ebufp);
EXTERN int pin_decimal_signum(const pin_decimal_t *pdp, pin_errbuf_t *ebufp);
EXTERN pin_decimal_t *pin_decimal_subtract(const pin_decimal_t *nump,
		const pin_decimal_t *byp, pin_errbuf_t *ebufp);
EXTERN void pin_decimal_to_str(const pin_decimal_t *pdp, const char *format,
		char **strpp, int *lenp, pin_errbuf_t *ebufp);
EXTERN double pin_decimal_to_double(const pin_decimal_t *pdp, pin_errbuf_t *ebufp);
EXTERN int64 pin_decimal_to_int64(const pin_decimal_t* pdpd, pin_errbuf_t *ebufp);

EXTERN void pin_decimal_add_assign(pin_decimal_t *pdp1, const pin_decimal_t *pdp2,
	pin_errbuf_t *ebufp);
EXTERN void pin_decimal_subtract_assign(pin_decimal_t *pdp1, const pin_decimal_t *pdp2,
	pin_errbuf_t *ebufp);
EXTERN void pin_decimal_multiply_assign(pin_decimal_t *pdp1, const pin_decimal_t *pdp2,
	pin_errbuf_t *ebufp);
EXTERN void pin_decimal_divide_assign(pin_decimal_t *nump,
	const pin_decimal_t *byp, int32 scale, int roundingMode, pin_errbuf_t *ebufp);
EXTERN long pin_decimal_cache_size(const pin_decimal_t *pdp);
EXTERN void *pin_decimal_cache_store(void *dest, const pin_decimal_t *pdp);

#define PIN_DECIMAL_DESTROY_EX( decpp)					\
{									\
	if(*(decpp)) {							\
		pin_decimal_destroy(*(decpp));				\
		*(decpp) = (pin_decimal_t *)NULL;			\
	}								\
}

#if defined(__cplusplus)
}
#endif

#undef EXTERN

#endif
