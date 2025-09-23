--
-- @(#)%Portal Version: create_cmt_procedure.sql:OptMgrsVelocityInt:4:2006-Sep-04 13:39:17 %
--
-- Copyright (c) 2004, 2023, Oracle and/or its affiliates.
--
--     This material is the confidential property of Oracle Corporation
--     or its licensors and may be used, reproduced, stored or transmitted 
--     only in accordance with a valid Oracle license or sublicense agreement.
--
--
--

CREATE OR REPLACE TYPE num_tab is table of number(38);
/

CREATE OR REPLACE PROCEDURE CMT_DEPLOY_UPDATE(
			i_poid_db IN NUMBER,
			i_poid_type IN VARCHAR2,
			i_poid_id0 IN NUMBER,
			i_poid_rev IN NUMBER,
  			t_stage_id IN NUMBER,
                        p_stage_id IN NUMBER,
			p_dom IN NUMBER,
                        p_return_code OUT NUMBER,
                        p_return_mesg OUT VARCHAR2
                        )
IS
poids_list num_tab;

BEGIN

-- Build account POID list
IF i_poid_id0 <> 0
THEN
  SELECT  b.account_obj_id0 BULK COLLECT INTO poids_list
  FROM    billinfo_t b, cmt_reference_t cr
  WHERE   b.account_obj_id0 = cr.poid_id
  and     cr.stage_id = p_stage_id
  AND     ((cr.obj_type = '/account' and b.actg_cycle_dom = p_dom)or (cr.obj_type like '/service%'))
  AND     b.account_obj_id0 = i_poid_id0
  AND     cr.deploy_status = 0 -- set 1 for testing
  AND     b.actg_cycle_dom = p_dom;
ELSE       
  SELECT  b.account_obj_id0 BULK COLLECT INTO poids_list
  FROM    billinfo_t b, cmt_reference_t cr
  WHERE   b.account_obj_id0 = cr.poid_id
  and     cr.stage_id = p_stage_id
  AND     cr.obj_type = '/account'
  AND     cr.deploy_status = 0 -- set 1 for testing
  AND     b.actg_cycle_dom = p_dom;
END IF;


-- update tables using the POID list
UPDATE /*+RULE*/ account_t SET poid_db = t_stage_id WHERE 
poid_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ service_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ purchased_bundle_t SET poid_db = t_stage_id WHERE
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ purchased_product_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ purchased_discount_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ billinfo_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ bill_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ profile_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ item_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ event_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ journal_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ ordered_balgroup_t SET poid_db = 
t_stage_id WHERE account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list 
as num_tab)));

UPDATE /*+RULE*/ bal_grp_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ group_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ payinfo_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ device_t SET poid_db = t_stage_id WHERE poid_id0 IN
                (SELECT /*+RULE*/ obj_id0 FROM 
device_services_t WHERE account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list 
as num_tab))));

UPDATE /*+RULE*/ au_account_t SET poid_db = t_stage_id WHERE 
poid_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ au_service_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ au_purchased_product_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ au_purchased_discount_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ au_ordered_balgroup_t SET poid_db = 
t_stage_id WHERE account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ au_bal_grp_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ au_group_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ au_profile_t SET poid_db = t_stage_id WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

UPDATE /*+RULE*/ au_purchased_bundle_t SET poid_db = t_stage_id WHERE
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab)));

        /* Updating Auditing Tables */

 UPDATE /*+RULE*/ au_service_t SET au_parent_obj_db = 
t_stage_id,au_parent_obj_id0= poid_id0,au_parent_obj_type= poid_type,au_parent_obj_rev= 0,
poid_type=REPLACE(poid_type,'/service','/au_service') WHERE account_obj_id0 in(
SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab))) AND au_parent_obj_db IS NULL;

 UPDATE /*+RULE*/ au_purchased_product_t SET au_parent_obj_db = 
t_stage_id,au_parent_obj_id0= poid_id0,au_parent_obj_type='/purchased_product',au_parent_obj_rev= 0 
WHERE account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab))) AND au_parent_obj_db IS NULL;

 UPDATE /*+RULE*/ au_purchased_discount_t SET au_parent_obj_db = 
t_stage_id,au_parent_obj_id0= poid_id0,au_parent_obj_type='/purchased_discount',au_parent_obj_rev= 0 
WHERE account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab))) AND au_parent_obj_db IS NULL;

 UPDATE /*+RULE*/ au_bal_grp_t SET au_parent_obj_db = 
t_stage_id, au_parent_obj_id0= poid_id0,au_parent_obj_type='/balance_group',au_parent_obj_rev= 0 
WHERE account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab))) AND au_parent_obj_db IS NULL;

 UPDATE /*+RULE*/ au_group_t SET au_parent_obj_db = 
t_stage_id, au_parent_obj_id0= poid_id0,au_parent_obj_type= poid_type,au_parent_obj_rev= 0, 
poid_type=REPLACE(poid_type,'/group','/au_group') WHERE account_obj_id0 in(
SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab))) AND au_parent_obj_db IS NULL;

 UPDATE /*+RULE*/ au_ordered_balgroup_t SET au_parent_obj_db = 
t_stage_id, au_parent_obj_id0= poid_id0,au_parent_obj_type= '/ordered_balgrp',au_parent_obj_rev= 0
WHERE account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab))) AND au_parent_obj_db IS NULL;

 UPDATE /*+RULE*/ au_profile_t SET au_parent_obj_db = 
t_stage_id, au_parent_obj_id0= poid_id0,au_parent_obj_type= poid_type,au_parent_obj_rev= 0, 
poid_type=REPLACE(poid_type,'/profile','/au_profile') WHERE account_obj_id0 in(
SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab))) AND au_parent_obj_db IS NULL;

UPDATE /*+RULE*/ au_purchased_bundle_t SET au_parent_obj_db =
t_stage_id,au_parent_obj_id0= poid_id0,au_parent_obj_type='/purchased_bundle',au_parent_obj_rev= 0
WHERE account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab))) AND au_parent_obj_db IS NULL;

UPDATE /*+RULE*/ au_billinfo_t SET poid_db = t_stage_id, au_parent_obj_db = t_stage_id, au_parent_obj_id0= poid_id0,
au_parent_obj_type= poid_type, au_parent_obj_rev= 0, poid_type=REPLACE(poid_type,'/billinfo','/au_billinfo') WHERE 
account_obj_id0 in(SELECT COLUMN_VALUE FROM TABLE(CAST(poids_list as num_tab))) AND au_parent_obj_db IS NULL;

EXCEPTION
WHEN OTHERS THEN
        p_return_code := 1;
        p_return_mesg := 'IN OTHERS OF CONV_MAIN_TABLES_RECOVERY '|| SQLERRM;
END;
/

CREATE OR REPLACE PROCEDURE CMT_MODIFY_INDEXES(P_TABLE_NAME IN VARCHAR,
						P_RETURN_CODE OUT NUMBER,

						P_RETURN_MESSAGE OUT VARCHAR) AS

  v_tmp_dml		   	 VARCHAR2(3000);

  v_tablespace_name		 all_ind_partitions.tablespace_name%TYPE;
  v_pct_free			 all_ind_partitions.pct_free%TYPE;
  v_ini_trans			 all_ind_partitions.ini_trans%TYPE;
  v_max_trans			 all_ind_partitions.max_trans%TYPE;
  v_initial_extent		 all_ind_partitions.initial_extent%TYPE;
  v_next_extent		 	 all_ind_partitions.next_extent%TYPE;
  v_min_extent			 all_ind_partitions.min_extent%TYPE;
  v_max_extent			 all_ind_partitions.max_extent%TYPE;
  v_pct_increase		 all_ind_partitions.pct_increase%TYPE;
  v_freelists			 all_ind_partitions.freelists%TYPE;
  v_freelist_groups		 all_ind_partitions.freelist_groups%TYPE;
  v_logging			 all_ind_partitions.logging%TYPE;
  v_buffer_pool			 all_ind_partitions.buffer_pool%TYPE;

  v_index_name 			 VARCHAR2(30);
  v_column_name			 VARCHAR2(4000);
  v_row_count			 NUMBER(5);
  v_drop			 VARCHAR2(40);
  v_create			 VARCHAR2(3000);
  
  e_no_index_found		 EXCEPTION;
  e_no_column_found		 EXCEPTION;
  
  CURSOR c_indx(b_table_name VARCHAR2) IS
    SELECT index_name FROM all_indexes WHERE uniqueness = 'UNIQUE' 
    AND table_name = DBMS_ASSERT.SQL_OBJECT_NAME(b_table_name);
  
  CURSOR c_cols(b_ind_name VARCHAR2) IS
    SELECT COLUMN_NAME FROM ALL_IND_COLUMNS WHERE INDEX_NAME = b_ind_name;
  
  CURSOR c_cnt(b_ind_name VARCHAR2) IS
    SELECT COUNT(*) FROM ALL_IND_COLUMNS WHERE INDEX_NAME = b_ind_name;
  
  CURSOR c_dets(b_index_name VARCHAR2) IS
  SELECT TABLESPACE_NAME,PCT_FREE,INI_TRANS,MAX_TRANS,INITIAL_EXTENT,MIN_EXTENT,MAX_EXTENT,PCT_INCREASE,
	FREELISTS,FREELIST_GROUPS,LOGGING,BUFFER_POOL FROM ALL_IND_PARTITIONS WHERE INDEX_NAME = b_index_name;
 
BEGIN
	OPEN c_indx(p_table_name);
	LOOP
		v_tmp_dml := NULL;
	
		FETCH c_indx INTO v_index_name;

		IF(c_indx%NOTFOUND) THEN
		    CLOSE c_indx;
		    RAISE e_no_index_found;		 
		END IF; 

		v_tmp_dml := 'CREATE INDEX ' || v_index_name || ' ON '|| p_table_name || '(';	
		 
		OPEN c_cnt(v_index_name);
		FETCH c_cnt INTO v_row_count;		
		IF(v_row_count = 0) THEN
		     CLOSE c_cnt;
		     RAISE e_no_column_found;
		END IF;
		IF(c_cnt%ISOPEN) THEN
		     CLOSE c_cnt;
		END IF;     

		OPEN c_cols(v_index_name);		
		FOR i IN 1..(v_row_count-1) LOOP
			FETCH c_cols INTO v_column_name;			
			EXIT WHEN c_cols%NOTFOUND;
			v_tmp_dml := v_tmp_dml || v_column_name || ',';
			
		END LOOP;
		
		FETCH c_cols INTO v_column_name;
		v_tmp_dml := v_tmp_dml || v_column_name || ')';
		
		IF(c_cols%ISOPEN) THEN
		    CLOSE c_cols;		
		END IF;    
		
		OPEN c_dets(v_index_name);
		FETCH c_dets INTO v_tablespace_name,v_pct_free,v_ini_trans,v_max_trans,v_initial_extent,v_min_extent,v_max_extent,
			  v_pct_increase,v_freelists,v_freelist_groups,v_logging,v_buffer_pool;
			
		IF(c_dets%ROWCOUNT > 0) THEN
			v_tmp_dml := v_tmp_dml || '  LOGGING TABLESPACE ' || v_tablespace_name || ' PCTFREE ' || v_pct_free || ' INITRANS ' ||
			v_ini_trans || ' MAXTRANS ' || v_max_trans || ' STORAGE ( INITIAL ' || v_initial_extent || ' MINEXTENTS ' || v_min_extent ||
			' MAXEXTENTS' || v_max_extent || ' PCTINCREASE ' || v_pct_increase || ' FREELISTS ' || v_freelists || ' FREELIST GROUPS ' || v_freelist_groups ||
			' BUFFER_POOL ' || v_buffer_pool || ');';
		ELSE
			v_tmp_dml := v_tmp_dml;
		END IF;
		 
		IF(c_dets%ISOPEN) THEN 
		    CLOSE c_dets; 	 		 
		END IF;    
		 v_drop := 'DROP INDEX ' || v_index_name;
		 dbms_output.put_line('Dropping Index: ' || v_index_name);
		 EXECUTE IMMEDIATE v_drop;
		 --EXECUTE IMMEDIATE 'DROP INDEX :b_index_name' USING v_index_name;
		 dbms_output.put_line(v_index_name || ' dropped ');
		 v_create := v_tmp_dml;		 
		 EXECUTE IMMEDIATE v_create;
		 dbms_output.put_line('NonUnique Index: ' || v_index_name || ' created ');
	END LOOP;
	IF(c_indx%ISOPEN) THEN
	   CLOSE c_indx;
	END IF;   
	
EXCEPTION 
WHEN e_no_index_found THEN
	p_return_code := 1;
	p_return_message := 'No more Unique Key Index found for table: ' || p_table_name;

WHEN e_no_column_found THEN
	p_return_code := 2;
	p_return_message := 'No entry found of columns for Unique key of table :' || p_table_name;
WHEN OTHERS THEN
	p_return_code := 3;
	p_return_message := 'In OTHERS: ' || SQLERRM;

END;
/

CREATE OR REPLACE PROCEDURE CMT_COPY_GROUP_PLANLIST(
  			p_stage_dbno NUMBER,	   
                        p_return_code OUT NUMBER,
                        p_return_mesg OUT VARCHAR2
                        )
IS
BEGIN

INSERT INTO  GROUP_T
(POID_DB,
 POID_ID0,
 POID_TYPE,
 POID_REV,
 CREATED_T,
 MOD_T,
 READ_ACCESS,
 WRITE_ACCESS,
 ACCOUNT_OBJ_DB,
 ACCOUNT_OBJ_ID0,
 ACCOUNT_OBJ_TYPE,
 ACCOUNT_OBJ_REV,
 COUNT,
 DESCR,
 NAME,
 PARENT_DB,
 PARENT_ID0,
 PARENT_TYPE,
 PARENT_REV,
 BAL_GRP_OBJ_DB,
 BAL_GRP_OBJ_ID0,
 BAL_GRP_OBJ_TYPE,
 BAL_GRP_OBJ_REV,
 STATUS,
 TYPE_STR)
(SELECT p_stage_dbno,
 POID_ID0,
 POID_TYPE,
 POID_REV,
 CREATED_T,
 MOD_T,
 READ_ACCESS,
 WRITE_ACCESS,
 ACCOUNT_OBJ_DB,
 ACCOUNT_OBJ_ID0,
 ACCOUNT_OBJ_TYPE,
 ACCOUNT_OBJ_REV,
 COUNT,
 DESCR,
 NAME,
 PARENT_DB,
 PARENT_ID0,
 PARENT_TYPE,
 PARENT_REV,
 BAL_GRP_OBJ_DB,
 BAL_GRP_OBJ_ID0,
 BAL_GRP_OBJ_TYPE,
 BAL_GRP_OBJ_REV,
 STATUS,
 TYPE_STR from group_t where poid_type ='/group/plan_list');

  commit;

EXCEPTION
WHEN OTHERS THEN
        p_return_code := 1;
        p_return_mesg := 'IN OTHERS OF CONV_MAIN_TABLES_RECOVERY '|| SQLERRM;
END;
/
