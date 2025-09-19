/*
 * @(#)%Portal Version: fm_num_pol_canonicalize.c:WirelessVelocityInt:2:2006-Sep-14 11:27:09 %
 *
 * Copyright (c) 2001, 2023, Oracle and/or its affiliates.
 *
 * This material is the confidential property of Oracle Corporation or its 
 * licensors and may be used, reproduced, stored or transmitted only in 
 * accordance with a valid Oracle license or sublicense agreement.
 *
 */

#ifndef lint
static const char Sccs_id[] = "@(#)%Portal Version: fm_num_pol_canonicalize.c:WirelessVelocityInt:2:2006-Sep-14 11:27:09 %";
#endif

#define FILE_LOGNAME "fm_num_pol_canonicalize.c"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "pcm.h"
#include "ops/num.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_num.h"
#include "pin_cust.h"
#include "pinlog.h"


/*******************************************************************************
 * Functions provided within this source file 
 ******************************************************************************/
EXPORT_OP void
op_num_pol_canonicalize(
        cm_nap_connection_t	*connp,
	u_int			opcode,
        u_int			flags,
        pin_flist_t		*i_flistp,
        pin_flist_t		**o_flistpp,
        pin_errbuf_t		*ebufp);

static void
fm_num_pol_canonicalize_string(
	char			*i_stringp,
	char			**o_stringp,
	pin_errbuf_t		*ebufp);

static int32
fm_num_pol_get_next_symbol(
	char			*tempstr,
	char			*symbuf,
	int			*index);

static void
fm_num_pol_pad_and_check_length(
	char			**str,
	int			str_len,
	pin_errbuf_t		*ebufp);

 
/*******************************************************************************
 * Entry routine for the PCM_OP_NUM_POL_CANONICALIZE opcode.
 ******************************************************************************/
void
op_num_pol_canonicalize(
	cm_nap_connection_t	*connp,
	u_int			opcode,
	u_int			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**o_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	char			*instringp = NULL;
	char			*outstringp = NULL;
	int32			rec_id = 0;
	pin_cookie_t		cookie = NULL;
	pin_flist_t		*str_flistp = NULL;
	pin_flist_t		*nums_flistp = NULL;
	void			*vp = NULL;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_NUM_POL_CANONICALIZE) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_num_pol_canonicalize opcode error", ebufp);
		return;
	}

	/*
	 * Log the input flist
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_num_pol_canonicalize input flist", i_flistp);


	/*
	 * Create outgoing flist
	 */
	*o_flistpp = PIN_FLIST_CREATE(ebufp);

	vp = PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(*o_flistpp, PIN_FLD_POID, (poid_t *)vp, ebufp);

	/*
	 * Walk the numbers array and canonicalize the strings.
	 */
	while ((str_flistp = PIN_FLIST_ELEM_GET_NEXT(i_flistp, 
		PIN_FLD_NUMBERS, &rec_id, 1, &cookie, ebufp)) != NULL) {

		/*
		 * Get the number from the element of the array
		 */
		instringp = PIN_FLIST_FLD_GET(str_flistp,
			PIN_FLD_NUMBER, 0, ebufp);

		/*
		 * If the value is not null and not empty, canonicalize
		 * it, else just return null for this element.
		 */
        	if (instringp != NULL && strlen(instringp) > 0) {

			/*
			 * Add a numbers element on the return flist
			 */
			nums_flistp = PIN_FLIST_ELEM_ADD(*o_flistpp, 
					PIN_FLD_NUMBERS, rec_id, ebufp); 

			fm_num_pol_canonicalize_string(instringp,
				&outstringp, ebufp);

			PIN_FLIST_FLD_PUT(nums_flistp, PIN_FLD_NUMBER, 
				outstringp, ebufp);

			outstringp = NULL;

		} else { 

			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR,
				"fm_num_pol_canonicalize null number "
				"encountered");
			
			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_APPLICATION,
				PIN_ERR_BAD_VALUE, PIN_FLD_NUMBER, 0, 0);
		}
	}

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_num_pol_canonicalize error", ebufp);

		PIN_FLIST_DESTROY_EX(o_flistpp, NULL);
		*o_flistpp = NULL;

	} else {
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_num_pol_canonicalize return flist", *o_flistpp);
	}

	return;
}


/*******************************************************************************
 * fm_num_pol_canonicalize_string()
 *
 * Inputs: string to be canonicalized
 * Outputs: canonicalized string
 *
 * Description:
 * This function removes non-digits and left pads the number with two zero's.
 * In addition, it ensure the numbers are of a proper length.
 *
 * Possible Customizations:
 * This function could be customized to support non-digit characters, or default
 * specific country codes, or prefixes to the numbers, etc.
 ******************************************************************************/
static void
fm_num_pol_canonicalize_string(
	char			*instringp,
	char			**outstringpp,
	pin_errbuf_t		*ebufp)
{
	char			symbuf[2];
	char			msg[BUFSIZ];
	int			index = 0;
	int			symbol = -1;
	size_t			sizeOfOutString=0;

	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Allocate the outstring + 1 for \0 and 2 more for the "00" than may
	 * need to be added for left padding and clear the buffers
	 */
	sizeOfOutString=(strlen((char*)instringp) + 1 + 2) * sizeof(char) ;
	*outstringpp = (char *)pin_malloc(sizeOfOutString);
	if(*outstringpp == NULL)
	{
		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_SYSTEM_DETERMINATE, 
                    PIN_ERR_NO_MEM, 0, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_num_pol_canonicalize_string() unable to allocate memory()", ebufp);
   		 return;
	}
	
	pin_memset(*outstringpp, sizeOfOutString, '\0', sizeOfOutString);
	symbuf[0] = '\0';
	symbuf[1] = '\0';

	/*
	 * Loop through the string
	 */
	while ( (symbol = fm_num_pol_get_next_symbol(instringp, 
		(char *)&symbuf, &index)) != EOL ) {

		if (symbol == ADDSYMBOL) {

			pin_strlcat(*outstringpp, symbuf, sizeOfOutString);

		} else if (symbol == ERRSYMBOL) {

			pin_snprintf(msg, sizeof(msg), "fm_num_pol_canonicalize_string "
				"invalid number: '%s'", instringp);
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);

			pin_set_err(ebufp, PIN_ERRLOC_FM,
				PIN_ERRCLASS_SYSTEM_DETERMINATE,
				PIN_ERR_CANON_CONV, PIN_FLD_NUMBER, 0, 0);
			break;

		}
	}

	/*
	 * Pad the string and check its length
	 */
	fm_num_pol_pad_and_check_length(outstringpp, sizeOfOutString, ebufp);

	return;
}


/*******************************************************************************
 * fm_num_pol_get_next_symbol()
 *
 * Inputs: A string and the index of the character to be examined
 * Outputs: The category of the character
 *
 * Description:
 * This function classifies the character as: add it, skip it, or error.
 ******************************************************************************/
static int32
fm_num_pol_get_next_symbol(
	char		*tempstr,
	char		*symbuf,
	int		*index)
{
	char		c = '\0';


	if (tempstr == NULL) {
		return(EOL);
	}

	/*
	 * Read the character at position pointed to by index
	 * and increment the index
	 */
	c = tempstr[*index];
	*index = *index + 1;

 	switch (c) {
	case OPEN_PARAN:
	case CLOSE_PARAN:
	case DASH:
	case DOT:
	case COMMA:

		return(SKIPSYMBOL);

	case ENDOFLINE:

		return(EOL);

	default:

		/*
		 * Handle alphabet characters as invalid characters for a number
		 */
		if (isalpha(c)) {
			return(ERRSYMBOL);
		}

	   	if (isdigit(c)) {
			symbuf[0] = c; 
			return(ADDSYMBOL);
		}

		/*
		 * Skip the symbol if it fell through the checks above
		 */
		return(SKIPSYMBOL);
	}
}


/*******************************************************************************
 * fm_num_pol_pad_and_check_length()
 *
 * Inputs: string to be examined
 * Outputs: input string with possible prepended zero's; ebuf set if size is
 *	greater than or less than defined max and min values.
 *
 * Description:
 * This function ensures the string is left-padded with two zeros and the
 * length is with in the defined range.
 ******************************************************************************/
static void
fm_num_pol_pad_and_check_length(
	char			**str,
	int			str_len,
	pin_errbuf_t		*ebufp)
{
	char			tempstr[BUFSIZ];
	char			msg[BUFSIZ];
	char			*localstr = *str;
	int32			length = 0;


	if (PIN_ERR_IS_ERR(ebufp)) {
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * If the string starts with two zero's do nothing.
	 * Else if the string begins with a single zero, then prepend another 0.
	 * If the string does not begin with a zero prepend two zero's.
	 */
	pin_memset(tempstr, sizeof(tempstr), '\0', sizeof(tempstr));

	if (*localstr == '0') {
		if (*(++localstr) != '0') {
			pin_strlcpy(tempstr, "0",sizeof(tempstr));
		}
	} else {
		pin_strlcpy(tempstr, "00",sizeof(tempstr));
	}

	/*
	 * Concatenate the input string to the temporary string,
	 * and then copy the temporary string back to the input string.
	 */
	pin_strlcat(tempstr, *str,sizeof(tempstr));
	pin_strlcpy(*str, tempstr, str_len);

	/*
	 * Check that the length of the padded string is within bounds
	 */
	length = strlen(*str);

	if (length < PIN_NUM_CANON_MIN_LENGTH) {

		pin_snprintf(msg, sizeof(msg), "fm_num_pol_pad_and_check_length "
			"length of '%s' < PIN_NUM_CANON_MIN_LENGTH (%d)", 
			*str, PIN_NUM_CANON_MIN_LENGTH);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);

		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_CANON_CONV, PIN_FLD_NUMBER, 0, 0);

	} else if (length > PIN_NUM_CANON_MAX_LENGTH) {

		pin_snprintf(msg, sizeof(msg), "fm_num_pol_pad_and_check_length "
			"length of '%s' > PIN_NUM_CANON_MAX_LENGTH (%d)", 
			*str, PIN_NUM_CANON_MAX_LENGTH);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, msg);

		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_CANON_CONV, PIN_FLD_NUMBER, 0, 0);

	} /* else the size is ok, do nothing */

	return;
}
