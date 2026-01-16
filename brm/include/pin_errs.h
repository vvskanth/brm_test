// Copyright (c) 1996, 2023, Oracle and/or its affiliates.

#ifndef _PIN_ERRS_H
#define _PIN_ERRS_H

#define PIN_ERR_NONE                         0
#define PIN_ERR_NO_MEM                       1
#define PIN_ERR_NO_MATCH                     2
#define PIN_ERR_NOT_FOUND                    3
#define PIN_ERR_BAD_ARG                      4
#define PIN_ERR_BAD_XDR                      5
#define PIN_ERR_BAD_FIRST_READ               6
#define PIN_ERR_BAD_READ                     7
#define PIN_ERR_NO_SOCKET                    8
#define PIN_ERR_BAD_TYPE                     9
#define PIN_ERR_DUPLICATE                   10
#define PIN_COMPARE_EQUAL                   11
#define PIN_COMPARE_NOT_EQUAL               12
#define PIN_ERR_MISSING_ARG                 13
#define PIN_ERR_BAD_POID_TYPE               14
#define PIN_ERR_BAD_CRYPT                   15
#define PIN_ERR_BAD_WRITE                   16
#define PIN_ERR_DUP_SUBSTRUCT               17
#define PIN_ERR_BAD_SEARCH_ARG              18
#define PIN_ERR_BAD_SEARCH_ARG_RECID        19
#define PIN_ERR_DUP_SEARCH_ARG              20
#define PIN_ERR_NONEXISTANT_POID            21
#define PIN_ERR_POID_DB_IS_ZERO             22
#define PIN_ERR_UNKNOWN_POID                23
#define PIN_ERR_NO_SOCKETS                  24
#define PIN_ERR_DM_ADDRESS_LOOKUP_FAILED    25
#define PIN_ERR_IM_ADDRESS_LOOKUP_FAILED    25  /* compat with old */
#define PIN_ERR_DM_CONNECT_FAILED           26
#define PIN_ERR_IM_CONNECT_FAILED           26  /* compat with old */
#define PIN_ERR_NAP_CONNECT_FAILED          27  /* connect to CM failed */
#define PIN_ERR_INVALID_RECORD_ID           28
#define PIN_ERR_STALE_CONF                  29
#define PIN_ERR_INVALID_CONF                30
#define PIN_ERR_WRONG_DATABASE              31
#define PIN_ERR_DUP_ARG                     32
#define PIN_ERR_BAD_SET                     33
#define PIN_ERR_BAD_CREATE                  34
#define PIN_ERR_BAD_FIELD_NAME              35
#define PIN_ERR_BAD_OPCODE                  36
#define PIN_ERR_TRANS_ALREADY_OPEN          37
#define PIN_ERR_TRANS_NOT_OPEN              38
#define PIN_ERR_NULL_PTR                    39
#define PIN_ERR_BAD_FREE                    40
#define PIN_ERR_FILE_IO                     41
#define PIN_ERR_NONEXISTANT_ELEMENT         42
#define PIN_ERR_STORAGE                     43  /* underlying DM storage */
#define PIN_ERR_TRANS_TOO_MANY_POIDS        44  /* transactions attempted */
                                                /* to too many DMs */
#define PIN_ERR_TRANS_LOST                  45  /* transaction lost, DM */
                                                /* went away during a trans. */
#define PIN_ERR_BAD_VALUE                   46  /* For when what we get from the DB just doesn't make any sense */
#define PIN_ERR_PARTIAL                     47  /* For batching, at least one post litle error occurred. */
#define PIN_ERR_NOT_YET_DONE                48  /* op, xdr, ... not done yet */
#define PIN_ERR_STREAM_IO                   49  /* IO error on data stream */
#define PIN_ERR_STREAM_EOF                  50  /* EOF on data stream */
#define PIN_ERR_OP_NOT_OUTSTANDING          51  /* no op in progress */
#define PIN_ERR_OP_ALREADY_BUSY             52  /* op already in progress */
#define PIN_ERR_OP_ALREADY_DONE             53  /* op already done, */
                                                 /* no callback... set */
#define PIN_ERR_NO_DATA_FIELDS              54  /* (DM err) in_flist with no */
                                                /* data fields on it */
#define PIN_ERR_PROHIBITED_ARG              55  /* "prohibited on create" */
#define PIN_ERR_BAD_LOGIN_RESULT            56  /* login attempt failed */
#define PIN_ERR_CM_ADDRESS_LOOKUP_FAILED    57  /* lookup of CM address bad */
#define PIN_ERR_BAD_LOGIN_REDIRECT_INFO     58  /* connect redirect info bad */
#define PIN_ERR_TOO_MANY_LOGIN_REDIRECTS    59  /* too many connect redirects */
                                                /* perhaps a loop in config? */
#define PIN_ERR_STEP_SEARCH                 60  /* bad op, expected */
                                                /* STEP_NEXT/STEP_END */
#define PIN_ERR_STORAGE_DISCONNECT          61  /* connection to underlying */
                                                /* DM storage was lost */
#define PIN_ERR_NOT_GROUP_ROOT              62  /* Not the root of a group*/
#define PIN_ERR_BAD_LOCKING                 63  /* OS locking error */
#define PIN_ERR_AUTHORIZATION_FAIL          64
#define PIN_ERR_NOT_WRITABLE                65  /* Not writable */
#define PIN_ERR_UNKNOWN_EXCEPTION           66  /* Unknown C++ exception */
#define PIN_ERR_START_FAILED                67  /* Failed to start servers */
#define PIN_ERR_STOP_FAILED                 68  /* Failed to stop servers */
#define PIN_ERR_INVALID_QUEUE               69  /* Invalid queue */
#define PIN_ERR_TOO_BIG                     70  /* something was too big */
#define PIN_ERR_BAD_LOCALE                  71  /* Invalid Locale passed */
#define PIN_ERR_CONV_MULTIBYTE              72  /* Error conversion */
#define PIN_ERR_CONV_UNICODE                73  /* Error conversion */
#define PIN_ERR_BAD_MBCS                    74  /* Error conversion */
#define PIN_ERR_BAD_UTF8                    75  /* Error conversion */
#define PIN_ERR_CANON_CONV                  76  /* Error canon conversion */
#define PIN_ERR_UNSUPPORTED_LOCALE          77  /* Unsupported locale */
#define PIN_ERR_CURRENCY_MISMATCH           78  /* Parent/child currency mismatch*/
#define PIN_ERR_DEADLOCK                    79  /* Transaction rolled back due to resource contention */
#define PIN_ERR_BACKDATE_NOT_ALLOWED        80  /* ledger report posted */
#define PIN_ERR_CREDIT_LIMIT_EXCEEDED       81  /* ledger report posted already */
#define PIN_ERR_IS_NULL                     82  /* value is "NULL" - not set */
#define PIN_ERR_DETAILED_ERR                83  /* detailed error - msg_id, uses enhanced errbuf */
#define PIN_ERR_AR_ACTION_EXISTS	    84 /* Open AR Item Exists */

/****************************************************************
* Error Message String Definition for Backdated Validation Failures
* These messages are used by Customer Center to display the
* error messages that are being sent by server instead of a generic error
* message.
*******************************************************************/
#define PIN_ERR_BACKDATE_BEFORE_ACCOUNT_EFFECTIVE_DATE	85	
#define PIN_ERR_BACKDATE_BEFORE_GL_POSTING_DATE		86      
#define PIN_ERR_BACKDATE_BEFORE_SERVICE_EFFECTIVE_DATE	87
#define PIN_ERR_BACKDATE_BEFORE_OFFERING_EFFECTIVE_DATE	88
#define PIN_ERR_BACKDATE_FEES_ALREADY_APPLIED		89         
#define PIN_ERR_BACKDATE_BEFORE_STATUS_CHANGE		90        
#define PIN_ERR_BACKDATE_BEFORE_PURCHASE_START		91      

#define PIN_ERR_STMT_CACHE                  92  /* statement cache problem */
#define PIN_ERR_CACHE_SIZE_ZERO             93  /* Tried to init cm_cache with zero size */

/* Error Flag for Movement of Balance Group from one billinfo to another
   billinfo if any AR actions exists */
#define PIN_ERR_DEF_BG_MOVEMENT		94
#define PIN_ERR_ACCT_BG_MOVEMENT	95

#define PIN_ERR_VALIDATE_PROFILE	96   /* Error Flag for failure of profile validation */

/****************************************************************
 * Error message string definition for Corrective Bill validation
 * failures. These messages will be used by Customer Center to
 * display error messages that are being sent by server instead of a
 * generic error message. Note that numbers for these messages are
 * same as in the errors.en_US file.
 *******************************************************************/
#define PIN_ERR_CORR_INV_DISABLED			97
#define PIN_ERR_BILL_IS_SUBORD				98
#define PIN_ERR_BILL_IS_NOT_FINALIZED			99
#define PIN_ERR_REASON_AND_DOMAIN_NOT_PRESENTED		100
#define PIN_ERR_TYPE_NOT_REPLACEMENT			101
#define PIN_ERR_NO_AR_CHARGES				102
#define PIN_ERR_BILL_IS_NOT_LAST			103
#define PIN_ERR_BILL_IS_FULLY_PAID			104
#define PIN_ERR_BILL_IS_PAID				105
#define PIN_ERR_AR_CHARGES_ARE_TOO_LOW			106
#define PIN_ERR_REASON_OR_DOMAIN_ARE_NOT_CORRECT	108
#define PIN_ERR_NO_INVOICE_FOR_BILL			109
#define PIN_ERR_PAYMENTS_EXIST				110

#define PIN_ERR_NOT_PRIMARY                107  /* Not the primary instance */
#define PIN_ERR_NOT_ACTIVE                 107  /* Instance is not active */

#define PIN_ERR_INVALID_STATE              111  /* Invalid State*/
#define PIN_ERR_NO_SECONDARY               112  /* No secondary instance*/
#define PIN_ERR_ACCT_CLOSED                113  /* Account is in Closed State */
#define PIN_ERR_EM_CONNECT_FAILED          114  /* CM-EM connection failure */
#define PIN_ERR_FAST_FAIL                  115  /* fast fail in dual timeout */
#define PIN_ERR_BAD_ENC_SCHEME             116  /* Bad encryption scheme */
#define PIN_ERR_MAX_BILL_WHEN              117  /* Error to be raised when bill_when crosses max value */
#define PIN_ERR_PERF_LIMIT_REACHED         118  /* Performance limit was reached */
#define PIN_ERR_ACTIVE_NOT_READY           119  /* TIMOS passive instance in transition to be active during switchover/failover but is not active ready yet */				
#define PIN_ERR_POID_ALREADY_LOCKED        120	/* Used to indicate already locked poids */
#define PIN_ERR_SERVICE_LOCKED             121  /* Used by SOX */

#define PIN_ERR_VALIDATE_PASSWORD	   122 /* Invalid password */
#define PIN_ERR_DISC_AMT_AND_PERCENT	   123 /* Either discount amount and percentage allowed. */              

/* 124 - 145 Reserved for Collections */

/* Error Flag related to SEPA */
#define PIN_ERR_CUST_SEPA_CURRENCY_NOT_EURO	146
#define PIN_ERR_CUST_INVALID_IBAN_LENGTH	147
#define PIN_ERR_CUST_INVALID_IBAN_FORMAT	148
#define PIN_ERR_CUST_NON_MOD97_IBAN		149	
#define PIN_ERR_CUST_INVALID_BIC_LENGTH		150
#define PIN_ERR_CUST_INVALID_BIC_FORMAT		151
#define PIN_ERR_CUST_UMR_ALREADY_EXISTS		152
#define PIN_ERR_CUST_INVALID_MANDATE_SIGNED_T	153
#define PIN_ERR_CUST_INVALID_CREDITOR_INFO	154
#define PIN_ERR_CUST_NON_EXISTANT_CREDITOR_ID	155
#define PIN_ERR_CUST_NON_EXISTANT_CREDITOR_INFO	156
#define PIN_ERR_CUST_PENDING_SEPA_REQUEST	157

/* Error codes returned by Oracle for  XA operations */
#define PIN_ERR_XAER_PROTO                  158  /* Routine was invoked in an improper context */
#define PIN_ERR_XAER_OUTSIDE                159  /* Resoure mg is doing work outside txn */
#define PIN_ERR_XAER_NOTA                   160  /* The XID is not valid */
#define PIN_ERR_XAER_INVAL                  161  /* Invalid argument */
#define PIN_ERR_XAER_DUPID                  162  /* XID already exists */
#define PIN_ERR_XAER_ASYNC                  163  /* Async operation is outstanding */
#define PIN_ERR_XA_RDONLY                   164  /* RDONLY TXN has been committed */
#define PIN_ERR_XA_RETRY                    165  /* Maybe retried */
#define PIN_ERR_XA_HEURHAZ                  166  /* TXN BRANCH may have been heuristically committed */
#define PIN_ERR_XA_HEURCOM                  167  /* TXN BRANCH has been heuristically committed */
#define PIN_ERR_XA_HEURRB                   168  /* TXN branch has been heuristically rolled back */
#define PIN_ERR_XA_HEURMIX                  169  /* TXN branch has been heuristically committed and rolled back */
#define PIN_ERR_XA_RBCOMMFAIL               170  /* rollback caused by comm failure */
#define PIN_ERR_XA_RBDEADLOCK               171  /* deadlock detected */
#define PIN_ERR_XA_RBINTEGRITY              172  /* resource integrity violation */
#define PIN_ERR_XA_RBOTHER                  173  /* rolled back for other reason */
#define PIN_ERR_XA_RBPROTO                  174  /* protocol error */
#define PIN_ERR_XA_RBROLLBACK               175  /* rollback due to unspecified reason */
#define PIN_ERR_XA_RBTIMEOUT                176  /* txn branch took too long */
#define PIN_ERR_XA_RBTRANSIENT              177  /* may retry trasaction branch */
#define PIN_ERR_XAER_RMFAIL                 178  /* Resource manager is unavailable */
#define PIN_ERR_XAER_RMERR                  179  /* Resource manager err in txn branch */

#define PIN_ERR_PURCHASE_FUTURE_DEAL	    180  /* Attempt to purchase a future deal */
#define PIN_ERR_PURCHASE_EXPIRED_DEAL	    181  /* Attempt to purchase expired deal */
#define PIN_ERR_PERMISSION_DENIED           182  /* Operation not allowed; data not viewable */
#define PIN_ERR_PDO_INTERNAL                183  /* The PDO data objects error */
#define PIN_ERR_IPT_DNIS                    184  /* The DNIS is not authorized. */
#define PIN_ERR_DB_MISMATCH                 185  /* Db. nos. did not match */ 
#define PIN_ERR_NO_CREDIT_BALANCE           186  /* No credit balance available for the account */
#define PIN_ERR_NOTHING_TO_BILL             187  /* No new items to bill */
#define PIN_ERR_MASTER_DOWN                 188  /* The main Infranet is down. */
#define PIN_ERR_OPCODE_HNDLR_INIT_FAIL      189  /* Initialization of opcode handler at JS failed */
#define PIN_ERR_INVALID_OBJECT              190  /* Object may have moved to another database - as in AMT/MultiDB*/
#define PIN_ERR_VALIDATE_ADJUSTMENT         191  /* Validate adjustment policy opcode returns failure */
#define PIN_ERR_SYSTEM_ERROR                192  /* Generic system error during running application */
#define PIN_ERR_BILLING_ERROR               193  /* Error during billing run */
#define PIN_ERR_AUDIT_COMMIT_FAILED         194  /* Commit for the audit tables failed */
#define PIN_ERR_NOT_SUPPORTED               195  /* Operation not supported */
#define PIN_ERR_INVALID_SER_FORMAT          196  /* Serialized format received from database is incorrect */
#define PIN_ERR_READ_ONLY_TXN               197  /* Cannot insert/update in a r/o txn */
#define PIN_ERR_VALIDATION_FAILED           198  /* Deals/plans validation failed*/
#define PIN_ERR_PROC_BIND                   199  /* Binding for Procedure arguments failed */
#define PIN_ERR_PROC_EXEC                   200  /* Procedure returned Application Specific error */
#define PIN_ERR_STORAGE_FAILOVER            201  /* RAC Failover Message */
#define PIN_ERR_RETRYABLE                   202  /* Error due to failover - operation is retryable if needed */
#define PIN_ERR_TIMEOUT                     203  /* Request timeout */
#define PIN_ERR_CONNECTION_LOST             204  /* Connection lost */
#define PIN_ERR_FEATURE_DISABLED            205  /* Feature is disabled */

/* Bill Unit conversion into subordinate not allowed in delay period as pervious cycle bill not yet finalized */
#define PIN_ERR_CONV_NOT_ALLOWED_IN_DELAY_PERIOD   206

/*Error messages for AR*/

#define PIN_ERR_AR_CREATE_ITEM_DISPUTE_WITH_ZERO_DUE    207  /* Billing item due is zero */
#define PIN_ERR_AR_TAX_INFORMATION_NOT_FOUND		208  /* Tax information is not found in the configuration*/
#define PIN_ERR_AR_ACCOUNT_WRITEOFF_FAILED		209  /* Account writeoff is failed */ 

#define PIN_ERR_PYMT_NOT_RECORDED_FOR_CORRECT_ACCT 210 /* Payment is not recorded for correct account */

/*Error messages used in 7.3.1*/
#define PIN_ERR_BACKDATE_PURCH_BEFORE_EFFECTIVE_DATE	85
#define PIN_ERR_BACKDATE_PURCH_BEFORE_GL_POSTING_DATE	86

/*
 * the class of error
 */
#define PIN_ERRCLASS_SYSTEM_DETERMINATE      1
#define PIN_ERRCLASS_SYSTEM_INDETERMINATE    2
#define PIN_ERRCLASS_SYSTEM_RETRYABLE        3
#define PIN_ERRCLASS_APPLICATION             4
#define PIN_ERRCLASS_SYSTEM_SUSPECT          5

/*
 * which subsystem gave the error
 */
#define PIN_ERRLOC_PCM                       1
#define PIN_ERRLOC_PCP                       2  /* internal error */
#define PIN_ERRLOC_CM                        3
#define PIN_ERRLOC_DM                        4
#define PIN_ERRLOC_IM                        4  /* compatibility with old */
#define PIN_ERRLOC_FM                        5
#define PIN_ERRLOC_FLIST                     6  /* flist manipulation problem */
#define PIN_ERRLOC_POID                      7  /* poid manipulation problem */
#define PIN_ERRLOC_APP                       8
#define PIN_ERRLOC_QM                        9  /* QM (qmflist) framework */
#define PIN_ERRLOC_PCMCPP                   10  /* PCM C++ wrappers */
#define PIN_ERRLOC_LDAP                     11  /* Pin LDAP libraries */
#define PIN_ERRLOC_NMGR                     12  /* Node Manager */
#define PIN_ERRLOC_INFMGR                   13  /* Infranet Manager */
#define PIN_ERRLOC_UTILS                    14  /* Portal utility */
#define PIN_ERRLOC_JS                       15  /* Java Server Framework error */
#define PIN_ERRLOC_JSAPP                    16  /* Java Server App/Opcode handler error */
#define PIN_ERRLOC_PDO                      17  /* PDO Data Objects error */
#define PIN_ERRLOC_EM                       18
#define PIN_ERRLOC_RTP                      19  /* Realtime Pipeline */
#define PIN_ERRLOC_TIMOS                    20  /* Timos */
#define PIN_ERRLOC_ADT                      21  /* Errors in ADT lib (iscript) */

/*Errors for Installment Plan*/
#define PIN_ERR_INSTL_DATE_NOT_VALID            211
#define PIN_ERR_INSTL_DUPLICATE                 212
#define PIN_ERR_INSTL_AMT_NOT_VALID             213
#define PIN_ERR_INSTL_TERM_NOT_VALID            214
#define PIN_ERR_INSTL_UNEQUAL_INSTLS_NOT_VALID  215
#define PIN_ERR_INSTL_UNEQUAL_INTVLS_NOT_VALID  216
#define PIN_ERR_INSTL_PERCENTAGE_NOT_VALID      217
#define PIN_ERR_INSTL_CRITERIA_NOT_VALID        218
#define PIN_ERR_INSTL_OPERATOR_NOT_VALID        219
#define PIN_ERR_INSTL_BAD_INPUT                 220
#define PIN_ERR_INSTL_INVALID_STATE_TRANSITION  221
#define PIN_ERR_INSTL_UPDATE_STATUS_FAILED      222
#define PIN_ERR_INSTL_BUFFER_NOT_FOUND          223
#define PIN_ERR_INSTL_NON_ZERO_ITEM_DUE         224
#define PIN_ERR_INSTL_UPDATE_FAILED             225
#define PIN_ERR_INSTL_OBJECT_BAD_RESULTS        226
#define PIN_ERR_INSTL_TOTAL_AMT_INVALID         227
#define PIN_ERR_INSTL_NOT_FOUND                 228
#define PIN_ERR_INSTL_ERROR_MANDATORY_FIELD_MISSING     229
#define PIN_ERR_INSTL_BAD_STATUS                230
#define PIN_ERR_INSTL_ALREADY_CREATED           231
#define PIN_ERR_INSTL_INVALID_PERIOD            232
#define PIN_ERR_INSTL_NO_OPEN_ITEMS             257

/*Errors for Notification Management*/
#define PIN_ERR_NOTIF_SPEC_DUPLICATE                  250
#define PIN_ERR_NOTIF_SPEC_CRITERIA_NOT_VALID         251
#define PIN_ERR_NOTIF_SPEC_MANDATORY_FIELD_MISSING    252
#define PIN_ERR_NOTIF_OPERATOR_NOT_VALID              253
#define PIN_ERR_NOTIF_TYPE_NOT_VALID                  254
#define PIN_ERR_NOTIF_INVALID_DELIVERY_METHOD         255
#define PIN_ERR_NOTIFICATION_SPEC_NOT_FOUND           256

/*Errors for Deposit Management*/
#define PIN_ERR_DEPOSIT_SPEC_PROFILE_DUPLICATE          331
#define PIN_ERR_DEPOSIT_SPEC_PROFILE_NOT_FOUND          332
#define PIN_ERR_DEPOSIT_SPEC_NOT_FOUND                  333
#define PIN_ERR_DEPOSIT_BAD_INPUT                       334
#define PIN_ERR_DEPOSIT_PURCHASED_DEPOSIT_NOT_FOUND     335
#define PIN_ERR_DEPOSIT_REFUND_REQUEST_NOT_FOUND        336
#define PIN_ERR_DEPOSIT_INVALID_AMOUNT                  337
#define PIN_ERR_DEPOSIT_INVALID_CHARGE_OFFER            338
#define PIN_ERR_DEPOSIT_BUFFER_NOT_FOUND                339
#define PIN_ERR_DEPOSIT_SPEC_STATUS_DRAFT               340
#define PIN_ERR_DEPOSIT_INVALID_SERVICE                 341
#define PIN_ERR_DEPOSIT_INVALID_ACTION                  342

#define PIN_ERR_RETRY_SET_TRANS_ERROR              261 /* Rollback and Retry a failed transaction */
#define PIN_ERR_SKIP_FLD_MISMATCH_ERROR            262 /* Skip the mismatch error between portal_op_flds.dat and db */
#define PIN_ERR_OPCODE_VALIDATE_ERROR              263 /* String field length validation */

#define PIN_ERR_DATA_OVERFLOW                      343 /* To indicate error when size limit exceeds during conversion */

#endif /*_PIN_ERRS_H*/
