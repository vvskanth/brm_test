/*
 *      @(#) % %
 *    
 *      Copyright (c) 1996 - 2023 Oracle. 
 *    
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PBO_DECIMAL_H
#define _PBO_DECIMAL_H

#include <string.h>

#ifndef _PCM_H
  #include <pcm.h>
#endif
#ifndef _PIN_DECIMAL_H
  #include <pin_decimal.h>
#endif

#define PBO_EPSILON    12        /* Maximum scale for comparisons */
#define PBO_MAX_SCALE  15        /* Maximum scale maintained during operations */

/* Setup the external declaration stuff correctly */
#define EXTERN				extern

/*
 * General notes:
 *
 *   Decimal values which are returned from pbo functions are always
 *   allocated and must be freed with pbo_decimal_destroy().
 *
 *   Series-style errbuf handling is used. This means:
 *   - If an error is already set in 'ebufp', no calculation is performed.
 *   - If an error occurs, it is set in 'ebufp'.
 *   - If the operation is successful, 'ebufp' is not affected.
 *
 *   If the result of an operation would exceed PBO_MAX_SCALE, the scale
 *   is truncated to PBO_MAX_SCALE, resulting in a possible loss of precision.
 *
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*
 *     Arithmetically compares two decimal values.
 *     Digits beyond EPSILON scale are ignored in comparison.
 *
 *     returns:
 *       -1 if (dec1 < dec2);
 *        0 if (dec1 = dec2);
 *        1 if (dec1 > dec2);
 *        0 on error 
 */
EXTERN int pbo_decimal_compare(const pin_decimal_t *dec1p, const pin_decimal_t *dec2p, pin_errbuf_t *ebufp);

/*
 *     Test the sign of a decimal value
 *     Digits beyond EPSILON scale are ignored in comparison to zero.
 *     Equivalent to "pbo_decimal_compare(dec, 0)"
 *     
 *     returns:
 *       -1 if (dec < 0);
 *        0 if (dec = 0);
 *        1 if (dec > 0);
 *        0 on error 
 */
EXTERN int pbo_decimal_sign(const pin_decimal_t *decp, pin_errbuf_t *ebufp);

/*
 *     Test if a decimal variable is zero.
 *     Digits beyond EPSILON scale are ignored in comparison to zero.
 *
 *     returns:
 *        1 if (dec = 0);
 *        0 if (dec != 0);
 *        0 on error 
 */
EXTERN int pbo_decimal_is_zero(const pin_decimal_t *decp, pin_errbuf_t *ebufp);

/*
 *     Test if a decimal variable is NULL
 *
 *     returns:
 *        1 if (dec is NULL);
 *        0 if (dec is not NULL);
 *        0 on error 
 */
EXTERN int pbo_decimal_is_null(const pin_decimal_t *decp, pin_errbuf_t *ebufp);

/*
 *     Create a decimal variable from an ascii string value
 *     Truncates decimal scale at PBO_MAX_SCALE if string has more digits
 *
 *     returns:
 *       pointer to allocated decimal variable on success
 *       NULL on error 
 */
EXTERN pin_decimal_t *pbo_decimal_from_str(const char *str, pin_errbuf_t *ebufp);

/*
 *     Create an ascii string variable from a decimal value
 *
 *     returns:
 *       pointer to allocated null-terminated string on success
 *       NULL on error 
 */
EXTERN char *pbo_decimal_to_str(const pin_decimal_t *decp, pin_errbuf_t *ebufp);

/*
 *     Create a decimal variable from a double value
 *     Truncates scale at PBO_MAX_SCALE if double contains more digits
 *
 *     returns:
 *       pointer to allocated decimal variable on success
 *       NULL on error
 */
EXTERN pin_decimal_t *pbo_decimal_from_double(double value, pin_errbuf_t *ebufp);

/**
 * Create a decimal variable from a double value, with specified rounding
 * method.
 * Allows the user to choose the rounding method rather than rely on the
 * default, which is ROUND_DOWN
 *
 * @param value double value to convert
 * @param rounding_mode ROUND_UP, ROUND_DOWN, ROUND_CEILING, ROUND_FLOOR,
 *        ROUND_HALF_UP, ROUND_HALF_DOWN, ROUND_HALF_EVEN, ROUND_UNNCESSARY are
 *        the choices
 * @param ebufp the error buffer to contain error
 * @return pin_decimal_t* pointer to newly allocated decimal value
 */
EXTERN pin_decimal_t* pbo_decimal_from_double_round(double value,
						    int rounding_mode,
						    pin_errbuf_t* ebufp);
/*
 *     Convert a decimal value to a double.
 *
 *     returns:
 *       double value representing the value of dec on success
 *       0 on error
 */
EXTERN double pbo_decimal_to_double(const pin_decimal_t *decp, pin_errbuf_t *ebufp);

/*
 *     Make a copy of a decimal value
 *     Truncates scale at MAX if dec contains more digits.
 *     'decp' may be NULL in which case the result is NULL, with no error set.
 *
 *     returns:
 *       pointer to allocated decimal variable on success
 *       NULL on error
 */
EXTERN pin_decimal_t *pbo_decimal_copy(const pin_decimal_t *decp, pin_errbuf_t *ebufp);

/*
 *     Destroy a decimal variable
 *     Frees dynamic memory for decimal variable and sets *decp to NULL
 *     Does nothing if decpp is NULL or *decpp is NULL.
 */
EXTERN void pbo_decimal_destroy(pin_decimal_t **decpp);

/*
 *     Returns the result of rounding a decimal value to a specific scale
 *     If specified scale exceeds EPSILON it's an error is set in 'ebufp'.
 *     If the specified scale is lower than the current scale of the
 *          variable, the value is rounded using the specified rounding mode.
 *     If the specified scale is higher than the current scale of the
 *          variable, trailing zeroes are added to the value to reach the
 *          specified scale.
 *     Specifying a negative scale is not allowed.
 *
 *     The rounding mode is one of ROUND_...
 *
 *     returns:
 *       pointer to allocated decimal variable on success
 *       NULL on error
 */
EXTERN pin_decimal_t *pbo_decimal_round(const pin_decimal_t *decp, int32 scale, int32 rounding_mode, pin_errbuf_t *ebufp);

/*
 *     Round a decimal variable to a specific scale
 *
 *     See pbo_decimal_round() for details.
 *
 *     The result is stored in decp.
 */
EXTERN void pbo_decimal_round_assign(pin_decimal_t *decp, int32 scale, int32 rounding_mode, pin_errbuf_t *ebufp);

/*
 *     Return the absolute value of a decimal value
 *     result = |dec|
 *
 *     returns:
 *       pointer to allocated decimal variable on success
 *       NULL on error
 */
EXTERN pin_decimal_t *pbo_decimal_abs(const pin_decimal_t *decp, pin_errbuf_t *ebufp);

/*
 *     Set a decimal variable to its absolute value
 *     dec = |dec|
 *
 *     The result is stored in decp.
 */
EXTERN void pbo_decimal_abs_assign(pin_decimal_t *decp, pin_errbuf_t *ebufp);

/*
 *     Return the negative of a decimal value
 *     result = -dec
 *
 *     returns:
 *       pointer to allocated decimal variable on success
 *       NULL on error
 */
EXTERN pin_decimal_t *pbo_decimal_negate(const pin_decimal_t *decp, pin_errbuf_t *ebufp);

/*
 *     Set a decimal variable to its negative
 *     dec = -dec
 *
 *     The result is stored in decp.
 */
EXTERN void pbo_decimal_negate_assign(pin_decimal_t *decp, pin_errbuf_t *ebufp);

/*
 *     Return the sum of two decimal values
 *     result = dec1 + dec2
 *
 *     returns:
 *       pointer to allocated decimal variable on success
 *       NULL on error
 */
EXTERN pin_decimal_t *pbo_decimal_add(const pin_decimal_t *dec1p, const pin_decimal_t *dec2p, pin_errbuf_t *ebufp);

/*
 *     Add a decimal value to a decimal variable.
 *     dec1 = dec1 + dec2
 *
 *     The result is stored in dec1p.
 */
EXTERN void pbo_decimal_add_assign(pin_decimal_t *dec1p, const pin_decimal_t *dec2p, pin_errbuf_t *ebufp);

/*
 *     Return the difference of two decimal values
 *     result = dec1 - dec2
 *
 *     returns:
 *       pointer to allocated decimal variable on success
 *       NULL on error
 */
EXTERN pin_decimal_t *pbo_decimal_subtract(const pin_decimal_t *dec1p, const pin_decimal_t *dec2p, pin_errbuf_t *ebufp);

/*
 *     Subtract a decimal value from a decimal variable
 *     dec1 = dec1 - dec2
 *
 *     The result is stored in dec1p.
 */
EXTERN void pbo_decimal_subtract_assign(pin_decimal_t *dec1p, const pin_decimal_t *dec2p, pin_errbuf_t *ebufp);

/*
 *     Return the product of two decimal values.
 *     result = dec1 * dec2
 *     result is truncated at PBO_MAX_SCALE if necessary
 *
 *     returns:
 *       pointer to allocated decimal variable on success
 *       NULL on error
 */
EXTERN pin_decimal_t *pbo_decimal_multiply(const pin_decimal_t *dec1p, const pin_decimal_t *dec2p, pin_errbuf_t *ebufp);

/*
 *     Multiplies a decimal variable by a decimal value.
 *     dec1 = dec1 * dec2
 *     dec1 is truncated at PBO_MAX_SCALE if necessary
 *
 *     The result is stored in dec1p.
 */
EXTERN void pbo_decimal_multiply_assign(pin_decimal_t *dec1p, const pin_decimal_t *dec2p, pin_errbuf_t *ebufp);

/*
 *     Returns the quotient of two decimal values.
 *     result = dec1 / dec2
 *     result is truncated at PBO_MAX_SCALE if necessary
 *
 *     returns:
 *       pointer to allocated decimal variable on success
 *       NULL on error
 */
EXTERN pin_decimal_t *pbo_decimal_divide(const pin_decimal_t *dec1p, const pin_decimal_t *dec2p, pin_errbuf_t *ebufp);

/*
 *     Divide a decimal variable by a decimal value
 *     dec1 = dec1 / dec2
 *     dec1 is truncated at PBO_MAX_SCALE if necessary
 *
 *     The result is stored in dec1p.
 */
EXTERN void pbo_decimal_divide_assign(pin_decimal_t *dec1p, const pin_decimal_t *dec2p, pin_errbuf_t *ebufp);

#undef EXTERN

#if defined(__cplusplus)
}
#endif

#endif
