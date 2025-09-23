--
--	@(#)$Id: create_amt_mv_pkb.sql /cgbubrm_mainbrm.portalbase/1 2016/12/14 02:20:20 madbiswa Exp $
--
--      Copyright (c) 2022, Oracle and/or its affiliates.
--     
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--

-- Commenting the 'SET serveroutput', since it will not have any effect on logging from 'dbms_output.put_line'.
-- This parameter should be set explicitly in the test/application program, which would invoke this SP.
-- SET serveroutput ON SIZE 1000000

create or replace
PACKAGE BODY amt_mv
IS
   --
   CURSOR amt_base_table_c IS
     SELECT mt.table_name, mt.srch_type, mt.insert_column_list, mt.select_column_list
     FROM amt_metadata_t mt
     WHERE
     (
       mt.table_name NOT LIKE 'au%'
       OR mt.table_name = 'au_profile_t'
       OR mt.table_name = 'au_service_t'
       OR mt.table_name = 'au_account_t'
       OR mt.table_name = 'au_bal_grp_t'
       OR mt.table_name = 'au_payinfo_t'
       OR mt.table_name = 'au_dependency_t'
       OR mt.table_name = 'au_group_t'
       OR mt.table_name = 'au_ordered_balgroup_t'
       OR mt.table_name = 'au_transition_t'
       OR mt.table_name = 'au_device_t'
       OR mt.table_name = 'au_purchased_product_t'
       OR mt.table_name = 'au_purchased_product_cycle_f_t'
       OR mt.table_name = 'au_billinfo_t'
       OR mt.table_name = 'au_purchased_discount_t'
       OR mt.table_name = 'au_bal_grp_sub_bals_t'
       OR mt.table_name = 'au_bal_grp_bals_t'
     )
     AND mt.srch_type IN ('P', 'A', 'G')
     ORDER BY mt.srch_type;
   --
   CURSOR amt_chld_table_c(j_id IN NUMBER, b_id IN NUMBER) IS
     SELECT DISTINCT p.table_name, p.poid_type, mt.srch_type, mt.insert_column_list, mt.select_column_list
     FROM amt_base_detail_t b, amt_poid_type_map_t p, amt_metadata_t mt
     WHERE b.poid_type = p.poid_type
     AND p.table_name NOT LIKE b.base_table_name
     AND p.table_name = mt.table_name
     AND b.job_id = j_id
     AND b.batch_id = b_id;
   --
   -- open log file
   --
   PROCEDURE log_open (p_log_file_flag IN CHAR,
                       p_log_file_dir IN VARCHAR2,
                       p_log_file_name IN VARCHAR2,
                       p_log_file OUT utl_file.file_type) IS
   BEGIN
     IF (p_log_file_flag = 'Y') THEN
       p_log_file := utl_file.fopen(p_log_file_dir,
                                    p_log_file_name,
                                    'w',
                                    5000); -- indiv. message can be as long as 5000 bytes
     END IF;
   END;
   --
   -- close log file
   --
   PROCEDURE log_close (p_log_file_flag IN CHAR, p_log_file IN OUT utl_file.file_type) IS
   BEGIN
     IF (p_log_file_flag = 'Y') THEN
       IF (utl_file.is_open(p_log_file)) THEN
           utl_file.fflush(p_log_file);
	   utl_file.fclose(p_log_file);
       END IF;
     END IF;
   END;
   --
   -- write message to log file
   --
   PROCEDURE log_mesg(p_log_file_flag IN CHAR, p_log_file IN utl_file.file_type, p_mesg IN VARCHAR2) IS
   BEGIN
     IF (p_log_file_flag = 'Y') THEN
       utl_file.put_line(p_log_file, to_char(sysdate, 'MM/DD/YY HH24:MM:SS')||' '||p_mesg);
       utl_file.fflush(p_log_file);
     ELSE
       dbms_output.put_line(to_char(sysdate, 'MM/DD/YY HH24:MM:SS')||' '||p_mesg);
     END IF;
   END;
   --
   -- compare account batch data in source and destination instance
   --
   PROCEDURE compare_src_dest(job_id IN NUMBER,
			      batch_id IN NUMBER,
                              src_schema_name IN VARCHAR2,
			      p_status OUT NUMBER)
   IS
   -- define report layout
   table_name_column_head VARCHAR2(30) := 'Table Name                |';
   table_name_column_line VARCHAR2(30) := '---------------------------';
   table_type_column_head VARCHAR2(30) := 'Type|';
   table_type_column_line VARCHAR2(30) := '-----';
   src_rec_cnt_column_head VARCHAR2(20) := 'Src. Rec. Cnt.|';
   src_rec_cnt_column_line VARCHAR2(20) := '---------------';
   dest_rec_cnt_column_head VARCHAR2(20) := 'Dest. Rec. Cnt.|';
   dest_rec_cnt_column_line VARCHAR2(20) := '----------------';
   delta_column_head VARCHAR2(20) := 'Delta    ';
   delta_column_line VARCHAR2(20) := '---------';
   --
   src_sql_stmt    VARCHAR2(512);
   src_rec_cnt     NUMBER;
   dest_sql_stmt   VARCHAR2(512);
   dest_rec_cnt    NUMBER;
   delta_value     VARCHAR2(30);
   --
   PROCEDURE print_table_header IS
   BEGIN
      dbms_output.put_line(table_name_column_head||
	table_type_column_head||
	src_rec_cnt_column_head||
	dest_rec_cnt_column_head||
	delta_column_head);
      dbms_output.put_line(table_name_column_line||
	table_type_column_line||
	src_rec_cnt_column_line||
	dest_rec_cnt_column_line||
	delta_column_line);
   END;
   --
   PROCEDURE compare_rec_cnt(table_name IN VARCHAR2,
			     srch_type IN VARCHAR2,
			     poid_type IN VARCHAR2,
			     job_id IN NUMBER,
			     batch_id IN NUMBER,
			     src_schema_name IN VARCHAR2)
   IS
      dest_join_table_name        VARCHAR2(500);
      src_join_table_name         VARCHAR2(500);
      where_clause_completion     VARCHAR2(500);
   BEGIN
      IF ((INSTR(srch_type, 'P') != 0) OR (INSTR (srch_type, 'A') != 0)) THEN
	 -- base table
	 dest_join_table_name := ', job_batch_accounts_t d ';
	 src_join_table_name := ', '||src_schema_name||'.job_batch_accounts_t d ';
	 --
	 where_clause_completion := ' d.account_obj_id0 AND d.job_id = '||job_id||
	   ' AND d.obj_id0 = '||batch_id;
	 --
      ELSE
	 -- child table
	 dest_join_table_name := ', amt_base_detail_t d ';
	 src_join_table_name := ', '||src_schema_name||'.amt_base_detail_t d ';
	 --
	 where_clause_completion := ' d.poid_id0 AND d.job_id = '||job_id||
	   ' AND d.batch_id = '||batch_id||' AND d.poid_type = '''||poid_type||'''';
	 --
      END IF;
      --
      IF (INSTR(srch_type, 'P') != 0) THEN
	 -- account_t
	 dest_sql_stmt := 'SELECT count(*) FROM '||table_name||' t '||dest_join_table_name||
	   ' WHERE t.poid_id0 = '||where_clause_completion;
	 src_sql_stmt := 'SELECT count(*) FROM '||src_schema_name||'.'||table_name||' t '||src_join_table_name||
	   ' WHERE t.poid_id0 = '||where_clause_completion;
      ELSE IF (INSTR(srch_type, 'O') != 0) THEN
	     -- child table
	     dest_sql_stmt := 'SELECT count(*) FROM '||table_name||' t '||dest_join_table_name||
    	        ' WHERE t.obj_id0 = '||where_clause_completion;
	     src_sql_stmt := 'SELECT count(*) FROM '||src_schema_name||'.'||table_name||' t '||src_join_table_name||
	        ' WHERE t.obj_id0 = '||where_clause_completion;
           ELSE
             -- all other base tables (srch_type = 'A')
	     dest_sql_stmt := 'SELECT count(*) FROM '||table_name||' t '||dest_join_table_name||
	        ' WHERE t.account_obj_id0 = '||where_clause_completion;
   	     src_sql_stmt := 'SELECT count(*) FROM '||src_schema_name||'.'||table_name||' t '||src_join_table_name||
	       ' WHERE t.account_obj_id0 = '||where_clause_completion;
	   END IF;
      END IF;
      --
      -- dbms_output.put_line('dest_sql_stmt = '||dest_sql_stmt);
      -- dbms_output.put_line('src_sql_stmt = '||src_sql_stmt);
      --
      EXECUTE IMMEDIATE dest_sql_stmt INTO dest_rec_cnt;
      EXECUTE IMMEDIATE src_sql_stmt INTO src_rec_cnt;
      -- verfication step:
      -- 1) do record counts in source and destination table match ?
      IF (src_rec_cnt != dest_rec_cnt) THEN
        p_status := 1;
      END IF;
      -- decode can only be used in SELECT statement ...
      SELECT
	decode((src_rec_cnt - dest_rec_cnt),
	  0, '',
	  RPAD((src_rec_cnt-dest_rec_cnt), LENGTH(delta_column_head)))
	INTO delta_value
      FROM dual;
      --
--      IF (src_rec_cnt != 0) THEN
	 dbms_output.put_line(RPAD(table_name, LENGTH(table_name_column_head) - 1)||
	   '|'||RPAD(srch_type, LENGTH(table_type_column_head) - 1)||
	   '|'||RPAD(src_rec_cnt, LENGTH(src_rec_cnt_column_head) - 1)||
	   '|'||RPAD(dest_rec_cnt, LENGTH(dest_rec_cnt_column_head) -1 )||
	   '|'||delta_value);
--      END IF;
   END;
   BEGIN
      p_status := 0;
      --
      print_table_header();
      FOR amt_table_r IN amt_base_table_c LOOP
	 compare_rec_cnt(amt_table_r.table_name,
	                 amt_table_r.srch_type,
			 '/account',
	                 job_id,
	                 batch_id,
			 src_schema_name);
      END LOOP;
      FOR amt_table_r IN amt_chld_table_c(job_id, batch_id) LOOP
	 compare_rec_cnt(amt_table_r.table_name,
	                 amt_table_r.srch_type,
			 amt_table_r.poid_type,
	                 job_id,
              	         batch_id,
			 src_schema_name);
      END LOOP;
   END;
  --
  -- Lock base tables
  --
  PROCEDURE lock_base_tables(job_id IN NUMBER,
			    batch_id IN NUMBER,
			    src_schema_name IN VARCHAR2,
                            primary_schema_name IN VARCHAR2,
			    log_file_flag IN CHAR,
			    log_file_dir IN VARCHAR2)
  IS
     poid_column_name		 VARCHAR2(100);
     src_base_table_upd_stmt     VARCHAR2(5000);
     --
     cnt			 NUMBER;
     --
     log_file                    utl_file.file_type;
     --
     begin_time                  NUMBER;
     end_time                    NUMBER;
     --
     -- prevent processing of empty tables (following initial locking step)
     --
     TYPE BaseTableList IS TABLE OF amt_base_table_c%ROWTYPE;
     base_table_list		 BaseTableList := BaseTableList();
     base_table_index		 NUMBER := 1;
     --
     -- retrieves the destination database id
     --
     FUNCTION get_dest_database_id RETURN NUMBER IS
	db_id NUMBER;
     BEGIN
	-- all Infranet database instances have a root account
	-- with poid_id0 = 1
	SELECT t.poid_db INTO db_id FROM account_t t WHERE t.poid_id0 = 1;
	RETURN db_id;
     END;
     --
     -- retrieves the source database id
     --
     FUNCTION get_src_database_id(src_schema_name IN VARCHAR2) RETURN NUMBER IS
       db_id NUMBER;
     BEGIN
        -- all Infranet database instances have a root account
        -- with poid_id0 = 1
        EXECUTE IMMEDIATE 'SELECT t.poid_db FROM '||src_schema_name||'.account_t t WHERE t.poid_id0 = 1' INTO db_id;
        RETURN db_id;
     END;
     --
   BEGIN
    --
    log_open(log_file_flag, log_file_dir, 'amt_migrate_'||job_id||'_'||batch_id||'.log', log_file);
    --
    log_mesg(log_file_flag, log_file,
             'processing job_id = '||job_id||', batch_id = '||batch_id||', src_schema_name = '||src_schema_name
	     ||', log_file_flag = '||log_file_flag||', log_file_dir = '||log_file_dir);
    --
    FOR amt_table_r IN amt_base_table_c LOOP
      --
      -- lock base tables
      --
      cnt := 0;
      --
      begin_time := dbms_utility.get_time();
      --
      IF (INSTR(amt_table_r.srch_type, 'P') != 0) THEN
        IF(amt_table_r.table_name = 'au_account_t') THEN
          poid_column_name := 'au_parent_obj_id0';
        ELSE
	  poid_column_name := 'poid_id0';
        END IF;
      ELSE
        poid_column_name := 'account_obj_id0';
      END IF;
      --
      -- Checking for group_t table.
      -- For Hierarchy and Sponsorship, use parent_id0 to identify group owner.
      -- For Charge/Discount share, use account_obj_id0 to identify group owner.
      IF(amt_table_r.table_name = 'group_t') THEN
        src_base_table_upd_stmt :=
          'UPDATE '||src_schema_name||'.'||amt_table_r.table_name||' gt '||
	  ' SET POID_REV = POID_REV + 1 WHERE ( gt.parent_id0 IN '||
             '(SELECT a.account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t a '||
                'WHERE a.job_id = '||job_id||' AND a.obj_id0 = '||batch_id||') AND poid_type IN '||
                  ' (''/group/billing'', ''/group/sponsor'')) OR ( gt.account_obj_id0 IN ' ||
             '(SELECT b.account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t b '||
                'WHERE b.job_id = '||job_id||' AND b.obj_id0 = '||batch_id||') AND poid_type LIKE '||
                  ' ''/group/sharing%'')';
      ELSE IF(amt_table_r.table_name = 'device_t') THEN
        src_base_table_upd_stmt :=
          'UPDATE '||src_schema_name||'.'||amt_table_r.table_name||
          ' SET POID_REV = POID_REV + 1 WHERE poid_id0 IN '||
             '(SELECT ds.obj_id0 FROM '||src_schema_name||'.device_services_t ds WHERE ds.account_obj_id0 IN' ||
             '(SELECT account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t a '||
                'WHERE a.job_id = '||job_id||' AND a.obj_id0 = '||batch_id||'))';
      ELSE IF(amt_table_r.table_name = 'au_device_t') THEN
        src_base_table_upd_stmt :=
          'UPDATE '||src_schema_name||'.'||amt_table_r.table_name||
          ' SET POID_REV = POID_REV + 1 WHERE poid_id0 IN '||
             '(SELECT ds.obj_id0 FROM '||src_schema_name||'.au_device_services_t ds WHERE ds.account_obj_id0 IN' ||
             '(SELECT account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t a '||
                'WHERE a.job_id = '||job_id||' AND a.obj_id0 = '||batch_id||'))';
      ELSE
        src_base_table_upd_stmt :=
          'UPDATE '||src_schema_name||'.'||amt_table_r.table_name||
	  ' SET POID_REV = POID_REV + 1 WHERE '||poid_column_name||' IN '||
             '(SELECT account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t a '||
                'WHERE a.job_id = '||job_id||' AND a.obj_id0 = '||batch_id||')';
      END IF;
      END IF;
      END IF;
      --
      -- log_mesg(log_file_flag, log_file, 'src_base_table_upd_stmt='||src_base_table_upd_stmt);
      --
      EXECUTE IMMEDIATE src_base_table_upd_stmt;
      cnt := sql%ROWCOUNT;
      --
      end_time := dbms_utility.get_time();
      --
      log_mesg(log_file_flag, log_file,
               'lock base table: table_name = '||amt_table_r.table_name||
	       ', records = '||cnt||
   	       ', update time = '||(end_time - begin_time)/100||'[sec]');
      --
      IF (cnt != 0) THEN
        base_table_list.extend;
        base_table_list(base_table_index) := amt_table_r;
	base_table_index := base_table_index + 1;
	-- persist information about non-empty account traversal tables in amt_account_batch_table_t
	-- (for purge operation)
	EXECUTE IMMEDIATE 'INSERT INTO '||primary_schema_name||'.amt_account_batch_table_t(job_id, batch_id, srch_type, table_name, poid_type) '||
	'VALUES (:job_id, :batch_id, :srch_type, :table_name, null)' 
	USING job_id,batch_id,amt_table_r.srch_type,amt_table_r.table_name;
      END IF;
    END LOOP;
    --
    log_close(log_file_flag, log_file);
    --
  EXCEPTION
    WHEN OTHERS THEN
      dbms_output.put_line('exception');
      log_close(log_file_flag, log_file);
      RAISE;
  END;
  --
  -- migrate account batch
  --
  PROCEDURE migrate_account_batch(job_id IN NUMBER,
				  batch_id IN NUMBER,
				  src_schema_name IN VARCHAR2,
                                  primary_schema_name IN VARCHAR2,
				  log_file_flag IN CHAR,
				  log_file_dir IN VARCHAR2)
  IS
     src_join_table_name         VARCHAR2(100);
     dest_join_table_name        VARCHAR2(100);
     poid_column_name		 VARCHAR2(100);
     where_clause_completion     VARCHAR2(100);
     dest_base_table_sql_stmt    VARCHAR2(5000);
     dest_chld_table_sql_stmt    VARCHAR2(5000);
     dest_chld_table_upd_stmt    VARCHAR2(5000);
     src_base_table_upd_stmt     VARCHAR2(5000);
     src_metadata_sql_stmt       VARCHAR2(5000);
     --
     poid_db_delta		 VARCHAR2(10);
     cnt			 NUMBER;
     --
     log_file                    utl_file.file_type;
     --
     begin_time                  NUMBER;
     end_time                    NUMBER;
     sel_ins_record_cnt		 NUMBER;
     --
     update_begin_time           NUMBER;
     update_end_time             NUMBER;
     --
     -- prevent processing of empty tables (following initial locking step)
     --
     TYPE BaseTableList IS TABLE OF amt_base_table_c%ROWTYPE;
     base_table_list		 BaseTableList := BaseTableList();
     base_table_index		 NUMBER := 1;
     --
     -- retrieves the destination database id
     --
     FUNCTION get_dest_database_id RETURN NUMBER IS
	db_id NUMBER;
     BEGIN
	-- all Infranet database instances have a root account
	-- with poid_id0 = 1
	SELECT t.poid_db INTO db_id FROM account_t t WHERE t.poid_id0 = 1;
	RETURN db_id;
     END;
     --
     -- retrieves the source database id
     --
     FUNCTION get_src_database_id(src_schema_name IN VARCHAR2) RETURN NUMBER IS
       db_id NUMBER;
     BEGIN
        -- all Infranet database instances have a root account
        -- with poid_id0 = 1
        EXECUTE IMMEDIATE 'SELECT t.poid_db FROM '||src_schema_name||'.account_t t WHERE t.poid_id0 = 1' INTO db_id;
        RETURN db_id;
     END;
     --
   BEGIN
    --
    poid_db_delta := to_char(get_dest_database_id() - get_src_database_id(src_schema_name), 'S9999');
    --
    log_open(log_file_flag, log_file_dir, 'amt_migrate_'||job_id||'_'||batch_id||'.log', log_file);
    --
    log_mesg(log_file_flag, log_file, 'processing job_id = '||job_id||', batch_id = '||batch_id
	||', src_schema_name = '||src_schema_name||', log_file_flag = '||log_file_flag
	||', log_file_dir = '||log_file_dir);
    log_mesg(log_file_flag, log_file, 'poid_db_delta = '||poid_db_delta);
    --
    FOR amt_table_r IN amt_base_table_c LOOP
      base_table_list.extend;
      base_table_list(base_table_index) := amt_table_r;
      base_table_index := base_table_index + 1;
    END LOOP;
    --
    base_table_index := base_table_list.FIRST;
    WHILE base_table_index IS NOT NULL LOOP
      --
      -- migrate base tables
      --
      begin_time := dbms_utility.get_time(); -- 100 th of a second
      --
      src_join_table_name := ', '||src_schema_name||'.job_batch_accounts_t d ';
      dest_join_table_name := ', job_batch_accounts_t d ';
      where_clause_completion := ' d.account_obj_id0 AND d.job_id = '||job_id||' AND d.obj_id0 = '||batch_id;
      --
      IF (INSTR(base_table_list(base_table_index).srch_type, 'P') != 0) THEN
	 -- account_t
        IF(base_table_list(base_table_index).table_name = 'au_account_t') THEN
          poid_column_name := 'au_parent_obj_id0';
        ELSE
          poid_column_name := 'poid_id0';
        END IF;
      ELSE
      -- all other base tables
        poid_column_name := 'account_obj_id0';
      END IF;
      --

      -- Checking for group_t table.
      -- For Hierarchy and Sponsorship, use parent_id0 to identify group owner.
      -- For Charge/Discount share, use account_obj_id0 to identify group owner.
      IF (base_table_list(base_table_index).table_name = 'group_t') THEN
        dest_base_table_sql_stmt :=
          'INSERT INTO '||base_table_list(base_table_index).table_name||
          ' t ('||base_table_list(base_table_index).insert_column_list||') '||
          ' SELECT '||replace(base_table_list(base_table_index).select_column_list, '#', poid_db_delta)||' FROM '||
          src_schema_name||'.'||base_table_list(base_table_index).table_name||' t '||
          ' WHERE ( t.parent_id0 IN '||
             '(SELECT a.account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t a '||
                'WHERE a.job_id = '||job_id||' AND a.obj_id0 = '||batch_id||') AND poid_type IN '||
                  ' (''/group/billing'', ''/group/sponsor'')) OR ( t.account_obj_id0 IN ' ||
             '(SELECT b.account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t b '||
                'WHERE b.job_id = '||job_id||' AND b.obj_id0 = '||batch_id||') AND poid_type LIKE '||
                  ' ''/group/sharing%'')';
        --
        src_metadata_sql_stmt :=
          'INSERT INTO amt_base_detail_t '||
          ' SELECT t.poid_id0, t.poid_type, '''||base_table_list(base_table_index).table_name||''', '||
          job_id||', '||batch_id||' FROM '||
          base_table_list(base_table_index).table_name||' t '||
          ' WHERE ( t.parent_id0 IN '||
             '(SELECT a.account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t a '||
                'WHERE a.job_id = '||job_id||' AND a.obj_id0 = '||batch_id||') AND poid_type IN '||
                  ' (''/group/billing'', ''/group/sponsor'')) OR ( t.account_obj_id0 IN ' ||
             '(SELECT b.account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t b '||
                'WHERE b.job_id = '||job_id||' AND b.obj_id0 = '||batch_id||') AND poid_type LIKE '||
                  ' ''/group/sharing%'')';
      ELSE IF (base_table_list(base_table_index).table_name = 'device_t') THEN
        dest_base_table_sql_stmt :=
          'INSERT INTO '||base_table_list(base_table_index).table_name||
          ' t ('||base_table_list(base_table_index).insert_column_list||') '||
          ' SELECT '||replace(base_table_list(base_table_index).select_column_list, '#', poid_db_delta)||' FROM '||
          src_schema_name||'.'||base_table_list(base_table_index).table_name||' t '||src_join_table_name||
          ' WHERE t.poid_id0 IN (SELECT ds.obj_id0 FROM '||src_schema_name||'.device_services_t'||
          ' ds WHERE ds.account_obj_id0 = '||where_clause_completion||')';
        --
        src_metadata_sql_stmt :=
          'INSERT INTO amt_base_detail_t '||
          ' SELECT t.poid_id0, t.poid_type, '''||base_table_list(base_table_index).table_name||''', '||
          job_id||', '||batch_id||' FROM '||
          base_table_list(base_table_index).table_name||' t '||dest_join_table_name||
          ' WHERE t.poid_id0 IN (SELECT ds.obj_id0 FROM '||src_schema_name||'.device_services_t'||
          ' ds WHERE ds.account_obj_id0 = '||where_clause_completion||')';
      ELSE IF (base_table_list(base_table_index).table_name = 'au_device_t') THEN
        dest_base_table_sql_stmt :=
          'INSERT INTO '||base_table_list(base_table_index).table_name||
          ' t ('||base_table_list(base_table_index).insert_column_list||') '||
          ' SELECT '||replace(base_table_list(base_table_index).select_column_list, '#', poid_db_delta)||' FROM '||
          src_schema_name||'.'||base_table_list(base_table_index).table_name||' t '||src_join_table_name||
          ' WHERE t.poid_id0 IN (SELECT ds.obj_id0 FROM '||src_schema_name||'.au_device_services_t'||
          ' ds WHERE ds.account_obj_id0 = '||where_clause_completion||')';
        --
        src_metadata_sql_stmt :=
          'INSERT INTO amt_base_detail_t '||
          ' SELECT t.poid_id0, t.poid_type, '''||base_table_list(base_table_index).table_name||''', '||
          job_id||', '||batch_id||' FROM '||
          base_table_list(base_table_index).table_name||' t '||dest_join_table_name||
          ' WHERE t.poid_id0 IN (SELECT ds.obj_id0 FROM '||src_schema_name||'.au_device_services_t'||
          ' ds WHERE ds.account_obj_id0 = '||where_clause_completion||')';
      ELSE
        dest_base_table_sql_stmt :=
	  'INSERT INTO '||base_table_list(base_table_index).table_name||
	  ' t ('||base_table_list(base_table_index).insert_column_list||') '||
	  ' SELECT '||replace(base_table_list(base_table_index).select_column_list, '#', poid_db_delta)||' FROM '||
	  src_schema_name||'.'||base_table_list(base_table_index).table_name||' t '||src_join_table_name||
	  ' WHERE t.'||poid_column_name||' = '||where_clause_completion;
        --
        src_metadata_sql_stmt :=
	  'INSERT INTO amt_base_detail_t '||
	  ' SELECT t.poid_id0, t.poid_type, '''||base_table_list(base_table_index).table_name||''', '||
	  job_id||', '||batch_id||' FROM '||
	  base_table_list(base_table_index).table_name||' t '||dest_join_table_name||
	  ' WHERE t.'||poid_column_name||' = '||where_clause_completion;
      END IF;
      END IF;
      END IF;
      --
      --log_mesg(log_file_flag, log_file, 'dest_base_table_sql_stmt = '||dest_base_table_sql_stmt);
      --
      EXECUTE IMMEDIATE dest_base_table_sql_stmt;
      sel_ins_record_cnt := sql%ROWCOUNT;
      --
      --log_mesg(log_file_flag, log_file, 'src_metadata_sql_stmt = '||src_metadata_sql_stmt);
      --
      EXECUTE IMMEDIATE src_metadata_sql_stmt;
      --
      end_time := dbms_utility.get_time(); -- 100 th of a second
      --
      --
      -- nullify POID_DB in base tables
      --
      update_begin_time := dbms_utility.get_time();
      --
      IF (INSTR(base_table_list(base_table_index).srch_type, 'P') != 0) THEN
        IF(base_table_list(base_table_index).table_name = 'au_account_t') THEN
          poid_column_name := 'au_parent_obj_id0';
        ELSE
          poid_column_name := 'poid_id0';
        END IF;
      ELSE
        poid_column_name := 'account_obj_id0';
      END IF;
      --
      -- Checking for group_t table.
      -- For Hierarchy and Sponsorship, use parent_id0 to identify group owner.
      -- For Charge/Discount share, use account_obj_id0 to identify group owner.
      IF(base_table_list(base_table_index).table_name = 'group_t') THEN
        src_base_table_upd_stmt :=
          'UPDATE '||src_schema_name||'.'||base_table_list(base_table_index).table_name||' t SET poid_db = null '||
	  'WHERE ( t.parent_id0 IN '||
             '(SELECT a.account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t a '||
                'WHERE a.job_id = '||job_id||' AND a.obj_id0 = '||batch_id||') AND poid_type IN '||
                  ' (''/group/billing'', ''/group/sponsor'')) OR ( t.account_obj_id0 IN ' ||
             '(SELECT b.account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t b '||
                'WHERE b.job_id = '||job_id||' AND b.obj_id0 = '||batch_id||') AND poid_type LIKE '||
                  ' ''/group/sharing%'')';
      ELSE IF(base_table_list(base_table_index).table_name = 'device_t') THEN
        src_base_table_upd_stmt :=
          'UPDATE '||src_schema_name||'.'||base_table_list(base_table_index).table_name||' SET poid_db = null '||
          'WHERE poid_id0 IN '||
          '(SELECT ds.obj_id0 FROM '||src_schema_name||'.device_services_t ds WHERE ds.account_obj_id0 IN '||
          '(SELECT account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t a '||
          'WHERE a.job_id = '||job_id||' AND a.obj_id0 = '||batch_id||'))';
      ELSE IF(base_table_list(base_table_index).table_name = 'au_device_t') THEN
        src_base_table_upd_stmt :=
          'UPDATE '||src_schema_name||'.'||base_table_list(base_table_index).table_name||' SET poid_db = null '||
          'WHERE poid_id0 IN '||
          '(SELECT ds.obj_id0 FROM '||src_schema_name||'.au_device_services_t ds WHERE ds.account_obj_id0 IN '||
          '(SELECT account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t a '||
          'WHERE a.job_id = '||job_id||' AND a.obj_id0 = '||batch_id||'))';
      ELSE
        src_base_table_upd_stmt :=
          'UPDATE '||src_schema_name||'.'||base_table_list(base_table_index).table_name||' SET poid_db = null '||
	  'WHERE  '||poid_column_name||' IN '||
          '(SELECT account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t a '||
          'WHERE a.job_id = '||job_id||' AND a.obj_id0 = '||batch_id||')';
      END IF;
      END IF;
      END IF;
      --
      --log_mesg(log_file_flag, log_file, 'src_base_table_upd_stmt='||src_base_table_upd_stmt);
      --
      EXECUTE IMMEDIATE src_base_table_upd_stmt;
      --
      update_end_time := dbms_utility.get_time();
      --
      log_mesg(log_file_flag, log_file,
               'migrate/update base table: table_name = '||base_table_list(base_table_index).table_name||
	       ', records = '||sel_ins_record_cnt||
   	       ', sel/ins time = '||(end_time - begin_time)/100||'[sec]'||
               ', update time = '||(update_end_time - update_begin_time)/100||'[sec]');
      --
      base_table_index := base_table_list.NEXT(base_table_index);
    END LOOP;
    --
    -- update account object location in uniqueness table at the primary database
    -- (trigger replication of the uniqueness table?)
    --
    begin_time := dbms_utility.get_time(); -- 100 th of a second
    --
    EXECUTE IMMEDIATE 'begin '||primary_schema_name||'.update_uniqueness_table(:job_id, :batch_id, :db_id); end;' USING job_id,batch_id,get_dest_database_id();
    --
    end_time := dbms_utility.get_time(); -- 100 th of a second
    --
    log_mesg(log_file_flag, log_file,
             'update uniqueness table: ' ||
	     'time = '||(end_time - begin_time)/100||'[sec]');

    EXECUTE IMMEDIATE 'UPDATE '||primary_schema_name||'.device_services_t u SET u.account_obj_db = :1, u.service_obj_db = :2 WHERE  u.account_obj_id0 IN '||' (SELECT b.account_obj_id0 FROM job_batch_accounts_t b WHERE b.job_id = :job_id AND b.obj_id0 = :batch_id)' using get_dest_database_id(),get_dest_database_id(),job_id,batch_id;

    --
    -- replicate amt_base_detail_t table from destination to source database
    --
    begin_time := dbms_utility.get_time(); -- 100 th of a second
    --
    EXECUTE IMMEDIATE 'INSERT INTO '||src_schema_name||'.amt_base_detail_t'||
      ' SELECT * FROM amt_base_detail_t b WHERE b.job_id = :job_id AND b.batch_id = :batch_id' USING job_id,batch_id;
    --
    end_time := dbms_utility.get_time(); -- 100 th of a second
    --
    log_mesg(log_file_flag, log_file,
             'replicate: amt_base_detail_t to '||src_schema_name||'.amt_base_detail_t'||
             ', records = '||sql%ROWCOUNT||
	     ', sel/ins time = '||(end_time - begin_time)/100||'[sec]');

    FOR amt_table_r IN amt_chld_table_c(job_id, batch_id) LOOP
      --
      -- migrate child tables
      --
      begin_time := dbms_utility.get_time(); -- 100 th of a second
      --
      src_join_table_name := ', '||src_schema_name||'.amt_base_detail_t d ';
      where_clause_completion := ' d.poid_id0 AND d.job_id = '||job_id||' AND d.batch_id = '||batch_id;
      -- all child tables
      dest_chld_table_sql_stmt :=
	'INSERT INTO '||amt_table_r.table_name||' t ('||amt_table_r.insert_column_list||') '||
	' SELECT '||replace(amt_table_r.select_column_list, '#', poid_db_delta)||' FROM '||
	src_schema_name||'.'||amt_table_r.table_name||' t '||src_join_table_name||
	' WHERE d.poid_type = '''||amt_table_r.poid_type||''' AND t.obj_id0 = '||where_clause_completion;
      --
      --log_mesg(log_file_flag, log_file, 'dest_chld_table_sql_stmt = '||dest_chld_table_sql_stmt);
      --
      EXECUTE IMMEDIATE dest_chld_table_sql_stmt;
      cnt := sql%ROWCOUNT;
      --
      -- update of Infranet buffer fields information (FLIST in Oracle BLOB) in destination database
      --
      IF (amt_table_r.table_name = 'schedule_input_flist_buf') THEN
        dest_chld_table_upd_stmt :=
          'UPDATE schedule_input_flist_buf SET '||
          'input_flist_buf = convert_schedule_input_buffer(input_flist_buf, '||poid_db_delta||') '||
	  'WHERE obj_id0 IN ('||
	  '  SELECT poid_id0 FROM amt_base_detail_t d '||
	  '    WHERE d.poid_type = '''||amt_table_r.poid_type||''' AND d.job_id = '||job_id||
          '    AND d.batch_id = '||batch_id||')';
        --
        -- log_mesg(log_file_flag, log_file, 'dest_chld_table_upd_stmt = '||dest_chld_table_upd_stmt);
	--
	EXECUTE IMMEDIATE dest_chld_table_upd_stmt;
      ELSIF (amt_table_r.table_name = 'event_item_transfer_buf_t') THEN
        dest_chld_table_upd_stmt :=
	  'UPDATE event_item_transfer_buf_t SET '||
	  'buffer_buf = convert_item_transfer_buffer(buffer_buf, '||poid_db_delta||') '||
	  'WHERE obj_id0 IN ('||
	  '  SELECT poid_id0 FROM amt_base_detail_t d '||
	  '    WHERE d.poid_type = '''||amt_table_r.poid_type||''' AND d.job_id = '||job_id||
          '    AND d.batch_id = '||batch_id||')';
        --
        -- log_mesg(log_file_flag, log_file, 'dest_chld_table_upd_stmt = '||dest_chld_table_upd_stmt);
	--
        log_mesg(log_file_flag, log_file, 'dest_chld_table_upd_stmt = '||dest_chld_table_upd_stmt);
	--
	EXECUTE IMMEDIATE dest_chld_table_upd_stmt;
	ELSIF (amt_table_r.table_name = 'invoice_formats_buf_t') THEN
        dest_chld_table_upd_stmt :=
          'UPDATE invoice_formats_buf_t SET '||
          'buffer_buf = convert_item_transfer_buffer(buffer_buf, '||poid_db_delta||') '||
          'WHERE obj_id0 IN ('||
          '  SELECT poid_id0 FROM amt_base_detail_t d '||
          '    WHERE d.poid_type = '''||amt_table_r.poid_type||''' AND d.job_id = '||job_id||
          '    AND d.batch_id = '||batch_id||')';
        log_mesg(log_file_flag, log_file, 'dest_chld_table_upd_stmt = '||dest_chld_table_upd_stmt);
        --
        EXECUTE IMMEDIATE dest_chld_table_upd_stmt;
	--Update the balances_small and balances_large column of event_essentials_t table with correct poid db.
	--This buffer  uses ';' as the poid db delimeter(CO #PRSF00159264)
        ELSIF (amt_table_r.table_name = 'event_essentials_t') THEN
         dest_chld_table_upd_stmt :=
         'UPDATE event_essentials_t SET ' ||
         ' balances_small = update_balances_small(balances_small,'||poid_db_delta||')'  ||
         ',balances_large = update_balances_large(balances_large,'||poid_db_delta||')'   ||
	  ' WHERE obj_id0 IN ('||
	  '  SELECT poid_id0 FROM amt_base_detail_t d '||
	  '    WHERE d.poid_type = '''||amt_table_r.poid_type||''' AND d.job_id = '||job_id||
          '    AND d.batch_id = '||batch_id||')';

         log_mesg(log_file_flag, log_file, 'dest_chld_table_upd_stmt = '||dest_chld_table_upd_stmt);

          EXECUTE IMMEDIATE dest_chld_table_upd_stmt;
      END IF;
      --
      end_time := dbms_utility.get_time(); -- 100 th of a second
      --
      log_mesg(log_file_flag, log_file,
               'migrate child table: table_name = '||amt_table_r.table_name||
	       ', records = '||cnt||
	       ', sel/ins time = '||(end_time - begin_time)/100||'[sec]');
      --
      IF (cnt != 0) THEN
	-- persist information about non-empty account traversal tables in amt_account_batch_table_t
	-- (for purge operation)
	EXECUTE IMMEDIATE 'INSERT INTO '||primary_schema_name||'.amt_account_batch_table_t(job_id, batch_id, srch_type, table_name, poid_type) '||
          'VALUES (:1, :2, :3, :4, :5)'
	USING job_id, batch_id, amt_table_r.srch_type, amt_table_r.table_name, amt_table_r.poid_type;
      END IF;
    END LOOP;
    --
    log_close(log_file_flag, log_file);
    --
  EXCEPTION
    WHEN OTHERS THEN
      dbms_output.put_line('exception');
      log_close(log_file_flag, log_file);
      RAISE;
  END;
  --
  --
  --
  PROCEDURE delete_account_batch(job_id NUMBER,
				 batch_id NUMBER,
                                 primary_schema_name IN VARCHAR2,
				 log_file_flag IN CHAR,
				 log_file_dir IN VARCHAR2)
  IS
    --
    account_batch_table_c SYS_REFCURSOR;
    a_srch_type NVARCHAR2(1);
    a_table_name VARCHAR2(32);
    a_poid_type NVARCHAR2(255);
    --
    CURSOR base_detail_c(j_id IN NUMBER, b_id IN NUMBER, p_type IN VARCHAR2) IS
      SELECT b.poid_id0
      FROM amt_base_detail_t b
      WHERE b.job_id = j_id AND b.batch_id = b_id AND b.poid_type = p_type;
    --
    log_file                    utl_file.file_type;
    --
    begin_time                  NUMBER;
    end_time                    NUMBER;
    del_record_cnt	        NUMBER;
    --
  BEGIN
    --
    log_open(log_file_flag, log_file_dir,
             'amt_delete_'||job_id||'_'||batch_id||'.log', log_file);
    --
    log_mesg(log_file_flag, log_file, 'processing job_id = '||job_id||', batch_id = '||batch_id);
    -- delete data in base and child tables
    OPEN account_batch_table_c FOR 'SELECT srch_type,table_name,poid_type FROM '||primary_schema_name||'.amt_account_batch_table_t '||
                                   'WHERE job_id = '||job_id||'AND batch_id = '||batch_id || 'order by SRCH_TYPE';
    LOOP
      FETCH account_batch_table_c INTO a_srch_type,a_table_name,a_poid_type;
      EXIT WHEN account_batch_table_c%NOTFOUND;
      --
      begin_time := dbms_utility.get_time(); -- 100 th of a second
      del_record_cnt := 0;
      --
      IF (    (INSTR(a_srch_type, 'P') != 0)
           OR (INSTR(a_srch_type, 'A') != 0)
           OR (INSTR(a_srch_type, 'G') != 0)) THEN
        -- base table
	  IF (INSTR(a_srch_type, 'P') != 0) THEN
	   EXECUTE IMMEDIATE  'DELETE au_account_t b WHERE b.au_parent_obj_id0 IN ( SELECT b.account_obj_id0 FROM '||
			' job_batch_accounts_t b WHERE b.job_id = :jobid AND b.obj_id0 = :objid) AND '||
			' b.poid_db is null' USING job_id, batch_id ;
	   EXECUTE IMMEDIATE  'DELETE account_t b WHERE b.poid_id0 IN ( SELECT b.account_obj_id0 FROM '||
                        ' job_batch_accounts_t b WHERE b.job_id = :jobid AND b.obj_id0 = :objid) AND '||
                        ' b.poid_db is null' USING job_id, batch_id ;
          ELSE IF (INSTR(a_srch_type, 'G') != 0) THEN
          
            EXECUTE IMMEDIATE 'DELETE group_t WHERE (parent_id0 IN ( SELECT b.account_obj_id0 FROM '||
				' job_batch_accounts_t b WHERE b.job_id = :jobid AND b.obj_id0 = :objid) '||
				' AND poid_type IN (''/group/billing'', ''/group/sponsor'')) '||
				' OR (account_obj_id0 IN ( SELECT b.account_obj_id0 FROM '||
				' job_batch_accounts_t b WHERE b.job_id = :jid AND b.obj_id0 = :bid) '||  
				' AND poid_type LIKE ''/group/sharing%'')' USING job_id, batch_id, job_id, batch_id;
	  ELSE
            IF(a_table_name = 'device_t') THEN
	      EXECUTE IMMEDIATE 'DELETE '||a_table_name||' b WHERE '||
                ' b.poid_id0 IN (SELECT ds.obj_id0 FROM device_services_t ds WHERE '||
	        ' ds.account_obj_id0 IN ( SELECT b.account_obj_id0 FROM job_batch_accounts_t b WHERE '||
		' b.job_id = :jobid AND b.obj_id0 = :objid ) ) AND b.poid_db is null' USING job_id, batch_id;
	    ELSE IF(a_table_name = 'au_device_t') THEN
              EXECUTE IMMEDIATE 'DELETE '||a_table_name||' b WHERE '||
                ' b.poid_id0 IN (SELECT ds.obj_id0 FROM au_device_services_t ds WHERE '||
                ' ds.account_obj_id0 IN ( SELECT b.account_obj_id0 FROM job_batch_accounts_t b WHERE '||
		' b.job_id = :jobid AND b.obj_id0 = :objid ) ) AND b.poid_db is null' USING job_id, batch_id;
            ELSE
	      EXECUTE IMMEDIATE 'DELETE '||a_table_name||' b WHERE '||
	        ' b.account_obj_id0 IN ( SELECT b.account_obj_id0 FROM job_batch_accounts_t b WHERE '||
		' b.job_id = :jobid AND b.obj_id0 = :objid ) AND b.poid_db is null' USING job_id, batch_id;
            END IF;
            END IF;
	  END IF;
	  END IF;
          del_record_cnt := sql%ROWCOUNT;
	--
        end_time := dbms_utility.get_time(); -- 100 th of a second
	--
	log_mesg(log_file_flag, log_file,
                 'base table name = '||a_table_name||
	         ', deleted records = '||del_record_cnt||
	         ', deletion time = '||(end_time - begin_time)/100||'[sec]');
      ELSE
        -- child table, a_srch_type = 'O'
      
		 EXECUTE IMMEDIATE 'DELETE '||a_table_name||' b WHERE '|| 
		' b.obj_id0 in (SELECT poid_id0 FROM amt_base_detail_t WHERE '|| 
		' job_id = :1 AND batch_id = :2 AND poid_type = :3)' 
		USING job_id, batch_id, a_poid_type; 

        del_record_cnt := sql%ROWCOUNT;
        --
        end_time := dbms_utility.get_time(); -- 100 th of a second
        --
        log_mesg(log_file_flag, log_file,
                 'child table name = '||a_table_name||
	         ', deleted records = '||del_record_cnt||
	         ', deletion time = '||(end_time - begin_time)/100||'[sec]');
      END IF;
    END LOOP;
    --
    log_close(log_file_flag, log_file);
    --
  END;
  --
  --
  --
  PROCEDURE fetch_timos_objects(job_id IN NUMBER,
				batch_id IN NUMBER,
				group_id IN NUMBER,
				batch_type IN CHAR,
				src_schema_name IN VARCHAR2,
                                primary_schema_name IN VARCHAR2,
				log_file_flag IN CHAR,
				log_file_dir IN VARCHAR2,
				timos_objects OUT VAR_ARRAY)
  IS
  --
  v_log_file		utl_file.file_type;
  --
  v_begin_time		NUMBER;
  v_end_time		NUMBER;
  v_count		NUMBER;
  v_index		NUMBER := 1;
  v_table_sel_stmt	VARCHAR2(500);
  v_timos_obj_sel_stmt	VARCHAR2(500);
  v_acct_obj_sel_stmt	VARCHAR2(500);
  --
  v_temp 		VAR_ARRAY;
  v_acct_obj 		VAR_ARRAY;
  --
  CURSOR base_table_c IS
  	SELECT mt.table_name
  	FROM amt_metadata_t mt
  	WHERE
    	(
      	mt.table_name NOT LIKE 'au_%'
      	AND mt.table_name != 'reservation_t'
      	AND mt.table_name != 'reservation_list_t'
      	AND mt.table_name != 'active_session_t'
      	AND mt.table_name != 'block_t'
      	AND mt.table_name != 'bulkacct_t'
      	AND mt.table_name != 'data_t'
      	AND mt.table_name != 'admin_action_t'
      	AND mt.table_name != 'schedule_t'
      	AND mt.table_name != 'filter_set_t'
      	AND mt.table_name != 'message_t'
      	AND mt.table_name != 'note_t'
      	AND mt.table_name != 'batch_t'
    	)
    	AND mt.srch_type IN ('A', 'G')
    	ORDER BY mt.srch_type;
  --
  BEGIN
    	--
    	log_open(log_file_flag, log_file_dir, 'amt_fetch_timos_objects_'||job_id
		||'_'||batch_id||'.log', v_log_file);
    	log_mesg(log_file_flag, v_log_file, 'processing job_id = '||job_id
		||', batch_id = '||batch_id);
	--
    	-- Fetch all the accounts contained in the AMM job batch.
	--
      	v_begin_time := dbms_utility.get_time(); -- 100 th of a second
	IF (batch_type = 'S') THEN
		v_acct_obj_sel_stmt := 'SELECT b.account_obj_id0 FROM '||src_schema_name||'.job_batch_accounts_t'
			||' b WHERE b.job_id = '||job_id
			||'AND b.batch_num = '||batch_id;
	ELSE
		v_acct_obj_sel_stmt := 'SELECT a.account_obj_id0 FROM '||primary_schema_name||'.job_batch_accounts_t'
			||' a, '||primary_schema_name||'.job_batch_amt_t b, '||primary_schema_name||'.job_batch_t'
			||' c WHERE c.job_obj_id0 = a.job_id AND c.batch_num=a.batch_num '
			||'AND c.poid_id0 = b.obj_id0 AND b.group_id='||group_id
			||' AND c.job_obj_id0='||job_id;
	END IF;
	EXECUTE IMMEDIATE v_acct_obj_sel_stmt BULK COLLECT INTO v_acct_obj;
	--
	-- For each account contained in the AMM job batch, fetch all the objects
	-- residing in the TIMOS.
	--
	IF (v_acct_obj.COUNT > 0) THEN
		FOR itr IN v_acct_obj.FIRST .. v_acct_obj.LAST LOOP
		--
			v_acct_obj_sel_stmt := 'SELECT count(*) FROM '||src_schema_name||'.account_t'
				||' WHERE poid_id0 = '
				||v_acct_obj(itr)||' AND object_cache_type != 2';
			EXECUTE IMMEDIATE v_acct_obj_sel_stmt INTO v_count;
			IF (v_count > 0) THEN
				timos_objects(v_index) := '/account'||' '||v_acct_obj(itr);
				v_index := v_index + 1;
			END IF;
			--
			FOR base_table_r IN base_table_c LOOP
			--
				v_table_sel_stmt := 'SELECT count(*) FROM cols WHERE table_name '
					||'LIKE '''||upper(base_table_r.table_name)
					||''' AND column_name = ''OBJECT_CACHE_TYPE''';
				EXECUTE IMMEDIATE v_table_sel_stmt INTO v_count;
				--
				IF (v_count > 0) THEN
					v_timos_obj_sel_stmt := 'SELECT poid_type||'' ''||poid_id0 FROM '
						||src_schema_name||'.'||base_table_r.table_name
						||' WHERE account_obj_id0='||v_acct_obj(itr)
						||' AND poid_type IN (SELECT dt.NAME FROM dd_objects_t dt'
						||' WHERE dt.residency_type = 5 OR dt.residency_type = 7)'
						||' AND object_cache_type != 2';
					log_mesg(log_file_flag, v_log_file, 'v_timos_obj_sel_stmt = '
						||v_timos_obj_sel_stmt);
					EXECUTE IMMEDIATE v_timos_obj_sel_stmt BULK COLLECT INTO v_temp;
					--
					IF (v_temp.COUNT > 0) THEN
						FOR cnt IN v_temp.FIRST .. v_temp.LAST LOOP
							timos_objects(v_index) := v_temp(cnt);
							v_index := v_index + 1;
						END LOOP;
					END IF;
				END IF;
			--
			END LOOP;
			--
			v_timos_obj_sel_stmt := 'SELECT poid_type||'' ''||poid_id0 FROM '
				||primary_schema_name||'.uniqueness_t WHERE account_obj_id0='
				|| v_acct_obj(itr)||' AND object_cache_type != 2';
			EXECUTE IMMEDIATE v_timos_obj_sel_stmt BULK COLLECT INTO v_temp;
			--
			IF (v_temp.COUNT > 0) THEN
				FOR cnt IN v_temp.FIRST .. v_temp.LAST LOOP
					timos_objects(v_index) := v_temp(cnt);
					v_index := v_index + 1;
				END LOOP;
			END IF;
			--
		END LOOP;
	END IF;
      	v_end_time := dbms_utility.get_time(); -- 100 th of a second
	--
	log_mesg(log_file_flag, v_log_file, 'timos objects fetch time = '
		||(v_end_time - v_begin_time)/100||'[sec]');
	--
	log_close(log_file_flag, v_log_file);
	--
  EXCEPTION
  	WHEN OTHERS THEN
      		dbms_output.put_line('exception');
      		log_close(log_file_flag, v_log_file);
      		RAISE;
  END;
  --
  --
  --
  PROCEDURE update_xref_table(amt_job_id IN NUMBER,
			      amt_batch_id IN NUMBER,
			      class_list IN VARCHAR2,
			      src_schema_name IN VARCHAR2,
			      dest_schema_name IN VARCHAR2,
			      log_file_flag IN CHAR,
			      log_file_dir IN VARCHAR2)
  IS
     storable_class	VARCHAR2(4000);
     class_table	VARCHAR2(4000);

     old_poid_str	VARCHAR2(4000);
     new_poid_str	VARCHAR2(4000);
     where_clause	VARCHAR2(4000);
     sql_stmt		VARCHAR2(4000);
     errMesg            VARCHAR2(4000) := 'NULL';

     acct_obj_rev	NUMBER;
     object_id		NUMBER;

     src		NUMBER;
     dest		NUMBER;

     rec_cnt		NUMBER;

     comma_posn		NUMBER := 1;
     start_posn		NUMBER := 1;
     len		NUMBER := 0;

     log_file		utl_file.file_type;

     TYPE c1_type IS TABLE OF NUMBER;
     rec1 c1_type;
     rec2 c1_type;
     rec3 c1_type;

     TYPE c2_type IS TABLE OF VARCHAR2(4000);
     rec4 c2_type;

     --
     --
     -- retrieves the source database id
     --
     FUNCTION get_src_database_id(src_schema_name IN VARCHAR2) RETURN NUMBER IS
       db_id NUMBER;
     BEGIN
        -- all Infranet database instances have a root account
        -- with poid_id0 = 1
        EXECUTE IMMEDIATE 'SELECT t.poid_db FROM '||src_schema_name||'.account_t t WHERE t.poid_id0 = 1' INTO db_id;
        RETURN db_id;
     EXCEPTION
       WHEN OTHERS THEN
           dbms_output.put_line('Exception in get_src_database_id function');
           RAISE_APPLICATION_ERROR('-20100', 'Exception raised from get_src_database_id() :'
				   || SQLCODE||' '||SQLERRM);
     END;
     --
     -- retrieves the destination database id
     --
     FUNCTION get_dest_database_id(dest_schema_name IN VARCHAR2) RETURN NUMBER IS
        db_id NUMBER;
     BEGIN
        -- all Infranet database instances have a root account
        -- with poid_id0 = 1
	EXECUTE IMMEDIATE 'SELECT t.poid_db FROM '||dest_schema_name||'.account_t t WHERE t.poid_id0 = 1' INTO db_id;
        RETURN db_id;
     EXCEPTION
       WHEN OTHERS THEN
           dbms_output.put_line('Exception in get_dest_database_id function');
           RAISE_APPLICATION_ERROR('-20100', 'Exception raised from dest_src_database_id() :'
				   || SQLCODE||' '||SQLERRM);
     END;
     --

  BEGIN
     --
     log_open(log_file_flag, log_file_dir, 'amt_migrate_'||amt_job_id||'_'||amt_batch_id||'.log', log_file);
     --
     log_mesg(log_file_flag, log_file,
              'processing job_id = '||amt_job_id||', batch_id = '||amt_batch_id||', src_schema_name = '||src_schema_name
              ||', log_file_flag = '||log_file_flag||', log_file_dir = '||log_file_dir);
     --


     src := get_src_database_id(src_schema_name);
     dest := get_dest_database_id(dest_schema_name);


     BEGIN
         EXECUTE IMMEDIATE 'SELECT DISTINCT t.ACCOUNT_OBJ_ID0 
                            FROM '||src_schema_name||'.job_batch_accounts_t t 
                            WHERE t.job_id = :amt_job_id  AND t.obj_id0 = :amt_batch_id' 
         BULK COLLECT INTO rec1 USING amt_job_id,amt_batch_id;
     EXCEPTION
         WHEN NO_DATA_FOUND THEN
                errMesg := 'NO_DATA_FOUND Exception while fetching the accounts from job_batch_accounts_t ';
		dbms_output.put_line(errMesg);
	        RAISE_APPLICATION_ERROR('-20100', errMesg
                                        || SQLCODE||' '||SQLERRM);
         WHEN OTHERS THEN
                errMesg := 'Exception while fetching the accounts from job_batch_accounts_t ';
		dbms_output.put_line(errMesg);
	        RAISE_APPLICATION_ERROR('-20100', errMesg
                                        || SQLCODE||' '||SQLERRM);
     END;


     FOR itr IN rec1.FIRST .. rec1.LAST
     LOOP
        EXECUTE IMMEDIATE 'SELECT t.poid_rev
                           FROM '||src_schema_name||'.account_t t
                           WHERE t.poid_id0 = :1' 
        INTO acct_obj_rev USING rec1(itr);

	old_poid_str := '0.0.0.' || src || ' /account ' || rec1(itr) || ' ' || acct_obj_rev ;
	new_poid_str := '0.0.0.' || dest || ' /account ' || rec1(itr) || ' ' || acct_obj_rev ;

        EXECUTE IMMEDIATE 'INSERT INTO migrated_objects_t (batch_id, old_poid, new_poid)
	VALUES (:1, :2, :3)' USING amt_batch_id, old_poid_str, new_poid_str;

     END LOOP;

     len := length(class_list);

     WHILE (comma_posn != 0)
     LOOP
     --
     -- process subscriber objects
     --
	start_posn := comma_posn;
	comma_posn := INSTR(class_list, ',', comma_posn, 1);
	IF comma_posn != 0 THEN
		storable_class := SUBSTR(class_list, start_posn, comma_posn - start_posn);
		comma_posn := comma_posn+1;
	ELSE
		storable_class := SUBSTR(class_list, start_posn);
	END IF;

        BEGIN
            EXECUTE IMMEDIATE 'SELECT p.obj_id0 FROM dd_objects_t p WHERE p.NAME LIKE :storable_class'
            INTO object_id
            USING storable_class ;
        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                errMesg := 'NO_DATA_FOUND Exception while fetching the storable class ';
	        dbms_output.put_line(errMesg);
	        RAISE_APPLICATION_ERROR('-20100', errMesg
                                        || SQLCODE||' '||SQLERRM);
            WHEN OTHERS THEN
                errMesg := 'Exception while fetching the storable class ';
	        dbms_output.put_line(errMesg);
	        RAISE_APPLICATION_ERROR('-20100', errMesg
                                        || SQLCODE||' '||SQLERRM);
        END;

	EXECUTE IMMEDIATE 'SELECT t.SM_ITEM_NAME FROM dd_objects_fields_t t WHERE t.obj_id0 = :1 AND t.FIELD_NAME LIKE ''PIN_FLD_MAIN'''
        INTO class_table USING object_id;

        where_clause := '(SELECT t.account_obj_id0
                          FROM '||src_schema_name||'.job_batch_accounts_t  t
                          WHERE t.job_id = ' || amt_job_id || ' AND t.obj_id0 = '|| amt_batch_id || ') ';

        sql_stmt := 'SELECT count(*)
                     FROM ' || src_schema_name||'.'||class_table||
                    ' WHERE account_obj_id0 in ' || where_clause ;

        --
        -- dbms_output.put_line('sql_stmt = '||sql_stmt);
        --
        EXECUTE IMMEDIATE sql_stmt INTO rec_cnt;

	IF rec_cnt != 0 THEN
		sql_stmt := 'SELECT poid_id0 , poid_rev, poid_type 
			     FROM ' || src_schema_name||'.'||class_table ||
                    	    ' WHERE account_obj_id0 in ' || where_clause ;

		EXECUTE IMMEDIATE sql_stmt bulk collect INTO rec2, rec3, rec4;

		FOR itr IN rec2.FIRST .. rec2.LAST
		LOOP
			old_poid_str := '0.0.0.' || src || ' ' || rec4(itr) || ' ' || rec2(itr) || ' ' || rec3(itr) ;
			new_poid_str := '0.0.0.' || dest || ' ' || rec4(itr) || ' ' || rec2(itr) || ' ' || rec3(itr) ;
       			INSERT INTO migrated_objects_t
				(batch_id, old_poid, new_poid)
			VALUES
				(amt_batch_id, old_poid_str, new_poid_str);

		END LOOP;
	END IF;

     --
     -- End of process subscriber objects
     --
     END LOOP;

     --
     log_close(log_file_flag, log_file);
     --
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
	dbms_output.put_line('NO_DATA_FOUND Exception in update_xref_table procedure');
	log_close(log_file_flag, log_file);
	RAISE_APPLICATION_ERROR('-20100', 'NO_DATA_FOUND Exception raised from update_xref_table: '  || errMesg || ' : '
				|| SQLCODE||' '||SQLERRM);
    WHEN OTHERS THEN
	dbms_output.put_line('Exception in update_xref_table procedure');
	log_close(log_file_flag, log_file);
	RAISE_APPLICATION_ERROR('-20100', 'Exception raised from update_xref_table: '  || errMesg || ' : '
 				|| SQLCODE||' '||SQLERRM);
  END;
--
END;
/
show errors; 
