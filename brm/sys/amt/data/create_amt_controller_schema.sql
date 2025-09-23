--
--      @(#) % %
--     
--      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
--     
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--

DROP TABLE amt_account_batch_table_t CASCADE CONSTRAINTS; 

CREATE TABLE amt_account_batch_table_t ( 
  job_id             NUMBER        NOT NULL,
  batch_id           NUMBER        NOT NULL,
  srch_type	     VARCHAR2(1),
  table_name	     VARCHAR2(32),
  poid_type          VARCHAR2 (255))
	$PIN_CONF_TBLSPACE14 
	$PIN_CONF_STORAGE_MED ;  

CREATE INDEX i_amt_account_batch_table ON amt_account_batch_table_t (batch_id,job_id) 
	$PIN_CONF_TBLSPACEX16
	$PIN_CONF_STORAGE_MED ;
 



