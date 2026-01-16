/* continuus file information --- %full_filespec: pin_bdfs.h~3:incl:1 % */
/*******************************************************************
 *
 *  @(#) %full_filespec: pin_bdfs.h~3:incl:1 %
 *
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 *
 *******************************************************************/

#ifndef _PIN_BDFS_H
#define _PIN_BDFS_H

/*******************************************************************
* Operation flags
*******************************************************************/

#define PIN_BDFS_GET_RESULTS		0x01
#define PIN_BDFS_FTP			0x02
	
/*******************************************************************
* Porvision results batch flags
*******************************************************************/

#define PIN_PROVISION_RES_COMPLETE	0x00	
#define PIN_PROVISION_RES_PENDING	0x01

/*******************************************************************
* Mandate status flags
*******************************************************************/

#define PIN_MANDATE_RECEIVED		0x00
#define PIN_MANDATE_PENDING_IN_FIAS	0x01
#define PIN_MANDATE_SENT_TO_FIAS	0x02
#define PIN_MANDATE_PENDING_PROVISION	0X03
#define PIN_MANDATE_NOT_REQUIRED        0x04

/******************************************************************
* FTP modes
******************************************************************/
typedef enum pin_ftp_cmd {
	PIN_FTP_CMD_DD_SYNC =             1001,
	PIN_FTP_CMD_DD_SEND_ALL =         1002,
	PIN_FTP_CMD_DD_SEND_REQ =         1003,
 	PIN_FTP_CMD_DD_SEND_ACK =         1004,
	PIN_FTP_CMD_DD_RECV_ALL =         1005,
 	PIN_FTP_CMD_DD_RECV_RESP =        1006,
 	PIN_FTP_CMD_DD_RECV_RESULT =      1007,
 	PIN_FTP_CMD_DD_CLEAN_ALL =        1008,
 	PIN_FTP_CMD_DD_CLEAN_LOCAL =      1009,
 	PIN_FTP_CMD_DD_CLEAN_REMOTE =     1010
} pin_ftp_cmd_t;

typedef enum pin_bdfs_cmd {
	PIN_BDFS_CMD_DD_PROVISION =     	1011,
	PIN_BDFS_CMD_DD_PROVISION_RFR = 	1012,
	PIN_BDFS_CMD_DD_PROVISION_RECOVER =     1013,
	PIN_BDFS_CMD_DD_PROVISION_COMPLETE=    1014,
	PIN_BDFS_CMD_DD_PROCESS_RESULT =       1015,
	PIN_BDFS_CMD_DD_ACK_RESULT=    	1016
} pin_bdfs_cmd_t;

#endif /* !_PIN_BDFS_H */
