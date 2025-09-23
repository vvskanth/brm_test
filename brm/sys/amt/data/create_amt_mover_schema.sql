--
--      @(#)$Id: create_amt_mover_schema.sql /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:21:00 nishahan Exp $
--     
-- Copyright (c) 1996, 2014, Oracle and/or its affiliates. All rights reserved.
--     
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--

DROP TABLE amt_base_detail_t CASCADE CONSTRAINTS ; 

CREATE TABLE amt_base_detail_t ( 
  poid_id0         NUMBER, 
  poid_type        VARCHAR2 (255), 
  base_table_name  VARCHAR2 (32), 
  job_id           NUMBER, 
  batch_id         NUMBER)
	$PIN_CONF_TBLSPACE15
	$PIN_CONF_STORAGE_LARGE ;

CREATE INDEX i_amt_base_detail ON amt_base_detail_t (job_id,batch_id,poid_id0,poid_type) 
	$PIN_CONF_TBLSPACEX17
	$PIN_CONF_STORAGE_LARGE ;

DROP TABLE amt_poid_type_map_t CASCADE CONSTRAINTS; 

CREATE TABLE amt_poid_type_map_t ( 
  poid_type   VARCHAR2 (255), 
  table_name  VARCHAR2 (255))
	$PIN_CONF_TBLSPACE14
	$PIN_CONF_STORAGE_SMALL ; 

CREATE  INDEX i_amt_poid_type_map ON amt_poid_type_map_t (poid_type,table_name)
	$PIN_CONF_TBLSPACEX16 
	$PIN_CONF_STORAGE_SMALL ;

DROP TABLE amt_metadata_t CASCADE CONSTRAINTS;

CREATE TABLE amt_metadata_t (
  dd_obj0     NUMBER,
  field_type  NUMBER,
  srch_type   VARCHAR2(1),
  table_name  VARCHAR2(32),
  insert_column_list VARCHAR2(4000),
  select_column_list VARCHAR2(4000))
	$PIN_CONF_TBLSPACE14
	$PIN_CONF_STORAGE_SMALL ;

CREATE UNIQUE INDEX i_amt_metadata ON amt_metadata_t ( table_name )
	$PIN_CONF_TBLSPACEX16 
	$PIN_CONF_STORAGE_SMALL;

DROP TABLE migrated_objects_t CASCADE CONSTRAINTS;

CREATE TABLE migrated_objects_t (
  batch_id   NUMBER,
  old_poid   VARCHAR2(4000),
  new_poid   VARCHAR2(4000))
        $PIN_CONF_TBLSPACE14
        $PIN_CONF_STORAGE_LARGE ;

DROP FUNCTION convert_poid_list;

CREATE OR REPLACE FUNCTION convert_poid_list(poid_list       VARCHAR2,
				             instance_offset NUMBER) RETURN VARCHAR2
AS LANGUAGE JAVA
NAME 'com.portal.amt.MigrateBlob.convertPoidList(java.lang.String,long) return java.lang.String';
/
DROP FUNCTION convert_flist_string;

CREATE OR REPLACE FUNCTION convert_flist_string(poid_list       VARCHAR2,
				             	instance_offset NUMBER,
						poid_tag       VARCHAR2) RETURN VARCHAR2
AS LANGUAGE JAVA
NAME 'com.portal.amt.MigrateBlob.convertFlistString(java.lang.String,long,java.lang.String) return java.lang.String';
/
DROP FUNCTION update_balances_small;

CREATE OR REPLACE FUNCTION update_balances_small(balances VARCHAR2,
                                                   instance_offset NUMBER) RETURN VARCHAR2 
AS LANGUAGE JAVA
NAME 'com.portal.amt.MigrateBlob.updateBalancesSmall(java.lang.String,long) return java.lang.String';
/
DROP FUNCTION update_balances_large;

CREATE OR REPLACE FUNCTION update_balances_large(balances CLOB,
                                                   instance_offset NUMBER) RETURN CLOB
AS LANGUAGE JAVA
NAME 'com.portal.amt.MigrateBlob.updateBalancesLarge(oracle.sql.CLOB,long) return oracle.sql.CLOB';
/

DROP FUNCTION convert_account_lineage;

CREATE OR REPLACE FUNCTION convert_account_lineage(account_lineage VARCHAR2,
				                   instance_offset NUMBER) RETURN VARCHAR2
AS LANGUAGE JAVA
NAME 'com.portal.amt.MigrateBlob.convertAccountLineage(java.lang.String,long) return java.lang.String';
/

DROP FUNCTION convert_item_transfer_buffer;

CREATE OR REPLACE FUNCTION convert_item_transfer_buffer(item_transfer_buffer BLOB,
							instance_offset NUMBER) RETURN BLOB
AS LANGUAGE JAVA
NAME 'com.portal.amt.MigrateBlob.convertItemTransferBuffer(oracle.sql.BLOB,long) return oracle.sql.BLOB';
/

DROP FUNCTION convert_schedule_input_buffer;

CREATE OR REPLACE FUNCTION convert_schedule_input_buffer(item_transfer_buffer BLOB,
							 instance_offset NUMBER) RETURN BLOB
AS LANGUAGE JAVA
NAME 'com.portal.amt.MigrateBlob.convertScheduleInputBuffer(oracle.sql.BLOB,long) return oracle.sql.BLOB';
/

DROP FUNCTION format_millisec_time;

CREATE OR REPLACE FUNCTION format_millisec_time ( time_ms NUMBER) RETURN VARCHAR2
AS LANGUAGE JAVA
NAME 'com.portal.amt.MilSecTimeFormatter.formatTime(long) return java.lang.String';
/ 



