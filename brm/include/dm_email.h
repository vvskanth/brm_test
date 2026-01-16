/* continuus file information --- %full_filespec: dm_email.h~7:incl:1 % */
/*******************************************************************
 *
 *  	(#)$Id: dm_email.h st_cgbubrm_gsivakum_bug-8497454/2 2023/08/07 23:04:06 visheora Exp $
 *
 * 	Copyright (c) 2000, 2023, Oracle and/or its affiliates.
 *
 *      This material is the confidential property of Oracle
 *      Corporation or its licensors and may be used,reproduced,
 * 	stored or transmitted only in accordance with a valid 
 *	Oracle license or sublicense agreement.
 *
 *******************************************************************/

#define PIN_MAIL_TO    		1
#define PIN_MAIL_CC    		2
#define PIN_MAIL_BCC   		3
#define PIN_MAIL_REPLY_TO 	4

#define DM_EMAIL_UNIX_MAIL_COMMAND         "${PIN_HOME}/bin/brm_mail.py"

extern int dm_email_init(void);

extern void dm_email_ux_send_mail(char 	*sender,
				char	*from,
				char    *subject,
				char	*content_type,
                                char    **recipient,
                                int   *recipient_type,
                                char    *attachment,
                                char    *body,
                                int32 length,
				char    **attachment_data,
				char    **attachment_fnam,
				int32   **attachment_size,
				int32   attachment_count,
                                pin_errbuf_t *ebufp);

extern void dm_email_ux_print_mail(char *emailBody,
			  	int32	bodyLen,
			  	pin_errbuf_t	*ebufp);
