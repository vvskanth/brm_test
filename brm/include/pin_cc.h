/* continuus file information --- %full_filespec: pin_cc.h~5:incl:2 % */
/*******************************************************************
 *
 *  @(#) %full_filespec: pin_cc.h~5:incl:2 %
 *
 *      Copyright (c) 1994 - 2023 Oracle.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/
#ifndef PIN_CC_H
#define PIN_CC_H

/* Setup the external declaration stuff correctly */
#define PIN_CC_EXTERN		extern

/*******************************************************************
 * Length Constants:
 *******************************************************************/
#define PIN_CARD_LENGTH		30
#define PIN_TRANSID_LENGTH	22
#define CC_CVV2_LENGTH		3
#define CC_CID_LENGTH		4

/*******************************************************************
 * Descriptions
 *******************************************************************/
#define PIN_PAYMENT_CC_DESCR	"Credit Card Payment"
#define PIN_PAYMENT_DD_DESCR	"Direct Debit Payment"

/*******************************************************************
 * CreditCard types that we understand.
 *******************************************************************/
typedef enum pin_cc_types {
	PIN_CC_TYPE_UNDEFINED		= 0,
	PIN_CC_TYPE_VISA		= 1,	/* Visa */
	PIN_CC_TYPE_MCARD		= 2,	/* MasterCard */
	PIN_CC_TYPE_AMEX		= 3,	/* American Express */
	PIN_CC_TYPE_OPTIMA		= 4,	/* Optima */
	PIN_CC_TYPE_DISCOVER		= 5,	/* Discover */
	PIN_CC_TYPE_CARTE_BLANCHE	= 6,	/* Carte Blanche */
	PIN_CC_TYPE_DINERS_CLUB		= 7,	/* Diners Club */
	PIN_CC_TYPE_JCB			= 8,	/* JCB */
	PIN_CC_TYPE_CUP                 = 9     /* China Union Pay */
} pin_cc_types_t;

/*******************************************************************
 * Main API Calls:
 *******************************************************************/
/*******************************************************************
 * Sanity Check a CreditCard Number:
 *
 * 	pin_cc_sanity(card_number, card_type, error_code)
 *		char				*card_number;
 *		pcm_fld_acct_bill_type_t	card_type;
 *		u_int				*error_code;
 * 
 *	Possible return codes:
 *		PIN_ERR_NONE
 *		PIN_ERR_BAD_ARG
 *
 *******************************************************************/
PIN_CC_EXTERN void	pin_cc_sanity();

/*******************************************************************
 * Sanity Check an Expiration Date:
 *
 * 	pin_cc_check_exp(exp_date, error_code)
 *		char		*exp_date;
 *		u_int		*error_code;
 * 
 *	Possible return codes:
 *		PIN_ERR_NONE
 *		PIN_ERR_BAD_ARG
 *
 *******************************************************************/
PIN_CC_EXTERN void	pin_cc_check_exp();

/*******************************************************************
 * Retrieve the Specific CreditCard Type:
 *
 *	pin_cc_type(card_number)
 *		char	*card_number;
 *
 *	NOTE: This is a function. It returns a value
 *		in the pin_cc_types_t enumeration.
 *
 *******************************************************************/
PIN_CC_EXTERN pin_cc_types_t	pin_cc_type();


#endif /* PIN_CC_H */
