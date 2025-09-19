/*******************************************************************
 *
 * Copyright (c) 1999, 2023, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle Corporation
 *      or its licensors and may be used, reproduced, stored or transmitted
 *      only in accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/ 

#ifndef lint
static  char Sccs_Id[] = "@#$Id: fm_cust_pol_prep_passwd.c /cgbubrm_mainbrm.portalbase/1 2023/07/17 12:44:19 visheora Exp $"; 
#endif

#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __SVR4
#include <sys/systeminfo.h>	/* no gethostid() in Solaris */
#endif /*__SVR4*/

#include "pcm.h"
#include "ops/cust.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pin_cust.h"
#include "pinlog.h"
#include <pin_os_random.h>

#define FM_PWD_USE_PASSWD	8
#define FM_PWD_OK_CHARS	"@#$%?0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUM_SPECIAL_CHARS 5
#define NUM_NUMBERS 10
#define NUM_SMALL_LETTERS 26
#define NUM_CAPITAL_LETTERS 26
#define RANDOM_BYTE_BUFFER_LEN 67

/*******************************************************************
 * Routines contained herein.
 *******************************************************************/
EXPORT_OP void
op_cust_pol_prep_passwd(
        cm_nap_connection_t	*connp,
	int32			opcode,
        int32			flags,
        pin_flist_t		*in_flistp,
        pin_flist_t		**ret_flistpp,
        pin_errbuf_t		*ebufp);

static void
fm_cust_pol_prep_passwd(
	pcm_context_t		*ctxp,
        int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp);

static void
fm_cust_pol_prep_pwd_acct(
	pcm_context_t		*ctxp,
	pin_flist_t		*r_flistp,
        pin_errbuf_t		*ebufp);

static void
fm_cust_pol_prep_pwd_srvc(
	pcm_context_t		*ctxp,
	int32                   flags,
	pin_flist_t		*r_flistp,
        pin_errbuf_t		*ebufp);

static void
fm_cust_pol_prep_pwd_gen(
	char		*passwd,
	int		pwd_length,
	pin_errbuf_t	*ebufp);

static void
fm_cust_pol_prep_pwd_gsm(
	pcm_context_t           *ctxp,
	int32                   flags,
	pin_flist_t             *r_flistp,
	pin_errbuf_t            *ebufp);

static void
fm_cust_pol_prep_pwd_tcf(
	pcm_context_t		*ctxp,
        int32		        flags,
	pin_flist_t		*r_flistp,
        pin_errbuf_t		*ebufp);

/*******************************************************************
 * Main routine for the PCM_OP_CUST_POL_PREP_PASSWD  command
 *******************************************************************/
void
op_cust_pol_prep_passwd(
        cm_nap_connection_t	*connp,
	int32			opcode,
        int32			flags,
        pin_flist_t		*in_flistp,
        pin_flist_t		**ret_flistpp,
        pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*r_flistp = NULL;

	/***********************************************************
	 * Null out results until we have some.
	 ***********************************************************/
	*ret_flistpp = NULL;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Insanity check.
	 ***********************************************************/
	if (opcode != PCM_OP_CUST_POL_PREP_PASSWD) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"bad opcode in op_cust_pol_prep_passwd", ebufp);
		return;
	}

	/***********************************************************
	 * We will not open any transactions with Policy FM
	 * since policies should NEVER modify the database.
	 ***********************************************************/

	/***********************************************************
	 * Call main function to do it
	 ***********************************************************/
	fm_cust_pol_prep_passwd(ctxp, flags, in_flistp, &r_flistp, ebufp);

	/***********************************************************
	 * Results.
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		*ret_flistpp = (pin_flist_t *)NULL;
		PIN_FLIST_DESTROY_EX(&r_flistp, NULL);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_cust_pol_prep_passwd error", ebufp);
	} else {
		*ret_flistpp = r_flistp;
		PIN_ERR_CLEAR_ERR(ebufp);
		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"op_cust_pol_prep_passwd return flist", r_flistp);
	}

	return;
}

/*******************************************************************
 * fm_cust_pol_prep_passwd()
 *
 *	Prep the passwd to be ready for on-line registration.
 *	Our action depends on the type of object using the passwd.
 *	Specifically, we care about service passwds but force the
 *	account level passwds to NULL.
 *
 *******************************************************************/
static void
fm_cust_pol_prep_passwd(
	pcm_context_t		*ctxp,
        int32			flags,
	pin_flist_t		*in_flistp,
	pin_flist_t		**out_flistpp,
        pin_errbuf_t		*ebufp)
{
	pin_flist_t		*r_flistp = NULL;

	poid_t			*o_pdp;
	const char		*o_type = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Create outgoing flist
	 ***********************************************************/
	r_flistp = PIN_FLIST_COPY(in_flistp, ebufp);
	*out_flistpp = r_flistp;

	/***********************************************************
	 * Get the object poid.
	 ***********************************************************/
	o_pdp = (poid_t *)PIN_FLIST_FLD_GET(in_flistp, PIN_FLD_POID, 0, ebufp);

	/***********************************************************
	 * What type of object?
	 ***********************************************************/
	o_type = PIN_POID_GET_TYPE(o_pdp);

	if (o_type && !strncmp(o_type, "/account", 8)) {

		/* Account level password */
		fm_cust_pol_prep_pwd_acct(ctxp, r_flistp, ebufp);

	} else if (o_type && !strncmp(o_type, "/service", 8)) {
		/***************************************************
		 * if telco service, we auto generate the 
		 * password, the firstime
		 ***************************************************/
		if (!strncmp(o_type, "/service/gsm", 12)) {
			fm_cust_pol_prep_pwd_gsm(ctxp, flags, r_flistp, ebufp);
		} 
		else if (!strncmp(o_type, "/service/telco",14)) {
			fm_cust_pol_prep_pwd_tcf(ctxp, flags, r_flistp, ebufp);
		}
		else if (flags & PCM_OPFLG_CUST_CREATE_ROLE){
                        fm_cust_pol_prep_pwd_srvc(ctxp, flags, r_flistp, ebufp);
                } else {
                        /* Service level password */
                        if ((strncmp(o_type, "/service/admin_client", 21)) &&
                        (strncmp(o_type, "/service/pcm_client", 19))) {
                                fm_cust_pol_prep_pwd_srvc(ctxp, flags,
                                        r_flistp, ebufp);
                        }
                }

	} else {

		/* Error - unknown/usupported type */
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_POID_TYPE, PIN_FLD_POID, 0, 0);

	}

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_cust_pol_prep_passwd error", ebufp);
	}

	return;
}

/*******************************************************************
 * fm_cust_pol_prep_pwd_acct():
 *
 *	Prep the account level passwd for registration.
 *
 *	Forces all account level passords to be NULL.
 *
 *******************************************************************/
static void
fm_cust_pol_prep_pwd_acct(
	pcm_context_t		*ctxp,
	pin_flist_t		*r_flistp,
        pin_errbuf_t		*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * Force account passwd to NULL.
	 ***********************************************************/
	PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_PASSWD_CLEAR, (void *)NULL, ebufp);

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_cust_pol_prep_pwd_acct error", ebufp);
	}

	return;
}

/*******************************************************************
 * fm_cust_pol_prep_pwd_srvc():
 *
 *	Prep the service level passwd for registration.
 *
 *	Basically, we just see if one is passed in or not. If
 *	so, we do nothing, but if not, we generate one. Note that
 *	we consider the passwd passed in if the PASSWD_CLEAR field
 *	is on the input flist, but don't care about the value it
 *	might contain (ie it could be NULL). The value itself is
 *	checked by the _VALID_PASSWD call.
 *
 *******************************************************************/
static void
fm_cust_pol_prep_pwd_srvc(
	pcm_context_t		*ctxp,
	int32                   flags,
	pin_flist_t		*r_flistp,
        pin_errbuf_t		*ebufp)
{
	pin_flist_t		*read_flistp = NULL; 
	pin_flist_t		*tmp_flistp = NULL; 
	char			pwd_gen[FM_PWD_USE_PASSWD + 1];
	char			*loginid = NULL;
	char			tmppwd[255] = "";
	char			tmpstr[255] = "";
	char			tmploginid[255] = "";
	int32			PASSWD_PART = 3;
	int32			flag = PIN_BOOLEAN_FALSE;
	int32			i = 0;
	void                    *vp = NULL;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/***********************************************************
	 * See if a password was passed in.
	 ***********************************************************/
	vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_PASSWD_CLEAR, 0, ebufp);

	/***************************************************
        * Generate the passwd
        * Add the clear text passwd to the input list.
        ***************************************************/
        if (!vp || (flags & PCM_OPFLG_CUST_CREATE_ROLE)) {

		read_flistp = PIN_FLIST_CREATE(ebufp);
		vp = PIN_FLIST_FLD_GET(r_flistp, PIN_FLD_POID, 0, ebufp);	
		PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_POID, vp, ebufp);
		PIN_FLIST_FLD_SET(read_flistp, PIN_FLD_LOGIN, NULL, ebufp);
	
		PCM_OP(ctxp, PCM_OP_READ_FLDS, 
			PCM_OPFLG_CACHEABLE, read_flistp, &tmp_flistp, ebufp);	
		PIN_FLIST_DESTROY_EX(&read_flistp, NULL);

		loginid = PIN_FLIST_FLD_GET(tmp_flistp, 
			PIN_FLD_LOGIN, 0, ebufp);	

		while (flag == PIN_BOOLEAN_FALSE) { 
			flag = PIN_BOOLEAN_TRUE;
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG,
				"Auto-generating password..");
			fm_cust_pol_prep_pwd_gen(pwd_gen,
				FM_PWD_USE_PASSWD, ebufp);
			
			if(loginid){
				for (i = 0; i < strlen(loginid); i++) {
					tmploginid[i] = tolower(loginid[i]);
				}
			}
			tmploginid[i] = '\0';

			for (i = 0; i < strlen(pwd_gen); i++) {
				tmppwd[i] = tolower(pwd_gen[i]);
			}
			tmppwd[i] = '\0';
			
			for (i = 0; i <= strlen(tmppwd) - PASSWD_PART; i++) {
				pin_strncpy(tmpstr, sizeof(tmpstr), &tmppwd[i], PASSWD_PART); 
				tmpstr[PASSWD_PART] = '\0';

				if (strstr(tmploginid, tmpstr) != NULL) {
					flag = PIN_BOOLEAN_FALSE;
					break;	
				}
			}
		}

                PIN_FLIST_FLD_SET(r_flistp, PIN_FLD_PASSWD_CLEAR,
                        (void *)pwd_gen, ebufp);

		PIN_FLIST_DESTROY_EX(&tmp_flistp, NULL);
        }

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_cust_pol_prep_pwd_srvc error", ebufp);
	}

	return;
}


/*******************************************************************
 * fm_cust_pol_prep_pwd_gen():
 *
 *	Generates a random string of the requested length to
 *	be used as a unix style passwd.
 *
 *******************************************************************/
static void
fm_cust_pol_prep_pwd_gen(
	char		*passwd,
	int		pwd_length,
	pin_errbuf_t	*ebufp)
{
	int		c = 0;
	int		c1 = 0;
	int		c2 = 0;
	int		c3 = 0;
	int		cr = 0;
	int		i = 0;
        char		*ok_chars = FM_PWD_OK_CHARS;
	int		indx=0;
	unsigned char 	*buf=pin_get_random_buf(RANDOM_BYTE_BUFFER_LEN,ebufp);

	if (PIN_ERR_IS_ERR(ebufp) || buf==NULL) {
		pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, "Failed to generate random buffer", PIN_FILE_SOURCE_ID, __LINE__, ebufp);
		return;
	}
	PIN_ERR_CLEAR_ERR(ebufp);
 
	/***********************************************************
         *  Generate a unique string.
	 *
	 * If we get a prohibited character, try again. Password
	 * strings might be passed into ed (or similar) as part of
	 * printing say welcome letters, so characters that are 
	 * special within ed patterns cannot be used.
	 ***********************************************************/
        i = 0;
        while (i < pwd_length) {
		c = ((pin_get_random_byte(buf,RANDOM_BYTE_BUFFER_LEN,indx,ebufp)) % (int)strlen(ok_chars));
		if (PIN_ERR_IS_ERR(ebufp)){
			pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, "Failed to get random byte from buffer", PIN_FILE_SOURCE_ID, __LINE__, ebufp);
			free(buf);
			return;
		}
		passwd[i] = ok_chars[c];
		indx++;
		i++;
        }

	passwd[pwd_length] = '\0';
	/***********************************************************
	* Make sure that generated password has atleast one
	* special character, one lowecase and one uppercase
	* character and one digit.
	***********************************************************/

	/* Insert special character at random position */
	c = ((pin_get_random_byte(buf,RANDOM_BYTE_BUFFER_LEN,indx,ebufp)) % (int)strlen(passwd));
	if (PIN_ERR_IS_ERR(ebufp)){
		pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, "Failed to get random byte from buffer", PIN_FILE_SOURCE_ID, __LINE__, ebufp);
		free(buf);
		return;
	}
	indx++;
	cr = ((pin_get_random_byte(buf,RANDOM_BYTE_BUFFER_LEN,indx,ebufp)) % NUM_SPECIAL_CHARS);
	
	if (PIN_ERR_IS_ERR(ebufp)){
		pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, "Failed to get random byte from buffer", PIN_FILE_SOURCE_ID, __LINE__, ebufp);
		free(buf);
		return;
	}
	indx++;
	passwd[c] = ok_chars[cr];

	/* Insert number at random position */
	do {
		c1 = ((pin_get_random_byte(buf,RANDOM_BYTE_BUFFER_LEN,indx,ebufp)) % (int)strlen(passwd));
		if (PIN_ERR_IS_ERR(ebufp)){
			pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, "Failed to get random byte from buffer", PIN_FILE_SOURCE_ID, __LINE__, ebufp);
			free(buf);
			return;
		}
		indx++;
	} while (c1 == c);
	cr = ((pin_get_random_byte(buf,RANDOM_BYTE_BUFFER_LEN,indx,ebufp)) % NUM_NUMBERS);

	if (PIN_ERR_IS_ERR(ebufp)){
		pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, "Failed to get random byte from buffer", PIN_FILE_SOURCE_ID, __LINE__, ebufp);
		free(buf);
		return;
	}
	passwd[c1] = ok_chars[NUM_SPECIAL_CHARS + cr];

	/* Insert lowercase character at random position */
	do {
		c2 = ((pin_get_random_byte(buf,RANDOM_BYTE_BUFFER_LEN,indx,ebufp)) % (int)strlen(passwd));
		if (PIN_ERR_IS_ERR(ebufp)){
			pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, "Failed to get random byte from buffer", PIN_FILE_SOURCE_ID, __LINE__, ebufp);
			free(buf);
			return;
		}
		indx++;
	} while ((c2 == c) || (c2 == c1));
	cr = ((pin_get_random_byte(buf,RANDOM_BYTE_BUFFER_LEN,indx,ebufp)) % NUM_SMALL_LETTERS);

	if (PIN_ERR_IS_ERR(ebufp)){
		pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, "Failed to get random byte from buffer", PIN_FILE_SOURCE_ID, __LINE__, ebufp);
		free(buf);
		return;
	}
	
	passwd[c2] = ok_chars[NUM_SPECIAL_CHARS + NUM_NUMBERS+cr];

	/* Insert uppercase character at random position */
	do {
		c3 = ((pin_get_random_byte(buf,RANDOM_BYTE_BUFFER_LEN,indx,ebufp)) % (int)strlen(passwd));
		if (PIN_ERR_IS_ERR(ebufp)){
			pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, "Failed to get random byte from buffer", PIN_FILE_SOURCE_ID, __LINE__, ebufp);
			free(buf);
			return;
		}
		indx++;
	} while ((c3 == c) || (c3 == c1) || (c3 == c2));
	cr = ((pin_get_random_byte(buf,RANDOM_BYTE_BUFFER_LEN,indx,ebufp)) % NUM_CAPITAL_LETTERS);

	if (PIN_ERR_IS_ERR(ebufp)){
		pin_err_log_ebuf(PIN_ERR_LEVEL_ERROR, "Failed to get random byte from buffer", PIN_FILE_SOURCE_ID, __LINE__, ebufp);
		free(buf);
		return;
	}

	passwd[c3] = ok_chars[NUM_SPECIAL_CHARS + NUM_NUMBERS + NUM_SMALL_LETTERS + cr];


	/***********************************************************
	 * No errors.
	 ***********************************************************/
	PIN_ERR_CLEAR_ERR(ebufp);
	free(buf);
	return;
}

/*******************************************************************
 * fm_cust_pol_prep_pwd_gsm():
 *
 *      Specific /service/gsm password during registration.
 *      We systematically set the password to the value returned by
 *      fm_cust_pol_prep_pwd_srvc during customer registration.
 *      This password can be changed later thru webkit for example.
 *
 *******************************************************************/
static void
fm_cust_pol_prep_pwd_gsm(
	pcm_context_t           *ctxp,
	int32                   flags,
	pin_flist_t             *r_flistp,
	pin_errbuf_t            *ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/* if it's the creation of the service */
	if (flags & PCM_OPFLG_CUST_REGISTRATION){
		/***************************************************
		 * Generate the passwd
		 ***************************************************/
		fm_cust_pol_prep_pwd_srvc(ctxp, flags, r_flistp, ebufp);
	}

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_cust_pol_prep_pwd_gsm error", ebufp);
	}

	return;
}

/*******************************************************************
 * fm_cust_pol_prep_pwd_tcf():
 *
 *	Specific /service/telco password during registration.
 *      We systematically set the password to the value returned by
 *      fm_cust_pol_prep_pwd_srvc during customer registration.
 *      This password can be changed later thru webkit for example.
 *
 *******************************************************************/
static void
fm_cust_pol_prep_pwd_tcf(
	pcm_context_t		*ctxp,
        int32			flags,
	pin_flist_t		*r_flistp,
        pin_errbuf_t		*ebufp)
{
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/* if it's the creation of the service */
	if (flags & PCM_OPFLG_CUST_REGISTRATION){
		/***************************************************
		 * Generate the passwd
		 ***************************************************/
		fm_cust_pol_prep_pwd_srvc(ctxp, flags, r_flistp, ebufp);
	}

	/***********************************************************
	 * Error?
	 ***********************************************************/
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_cust_pol_prep_pwd_tcf error", ebufp);
	}

	return;
}
