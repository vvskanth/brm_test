CREATE OR REPLACE PROCEDURE compare_record(p_table_name IN VARCHAR2,
			                  p_poid_id0 IN NUMBER,
				          p_src_schema_name IN VARCHAR2,
					  p_status OUT NUMBER) 
IS
--
-- procedure: compare_record
--   Comparision of base table records identified by poid_id0.
--   Script needs to be executed from destination database.
--   Differences between the source and destination record are marked by '*'
--   in the 'Diff' report column. 
-- 
-- return values:
--   The following p_status codes are returned:
--  
--   0: no error
--   1: POID_DB field in source database instance is not set to NULL
--   2: _DB field value in destination database instance does not match database id
--   3: other field value differs between source and destination database
--   4: no data found in source table
--   5: _POID_LIST POID_DB component value in destination database instance does not match database id
--   6: ACCOUNT_T.LINEAGE field has not been converted
--   
--   The p_status status code only reflects the last error.
--
-- parameters:
--   IN  p_table_name		 - Infranet base table name (i.e. account_t)
--   IN  p_poid_id0		 - poid_id0 value
--   IN  p_src_schema_name       - MultiSchema source schema name
--   OUT p_status		 - status code
--
  column_name_column_head VARCHAR(50) := 'Column Name                |';
  column_name_column_line VARCHAR(50) := '----------------------------';
  src_value_column_head VARCHAR(50) := 'Src. Value            |';
  src_value_column_line VARCHAR(50) := '-----------------------';
  dest_value_column_head VARCHAR(50) := 'Dest. Value           |';
  dest_value_column_line VARCHAR(50) := '-----------------------';
  diff_column_head VARCHAR(50) := 'D|';
  diff_column_line VARCHAR(50) := '--';
  status_column_head VARCHAR(50) := 'S ';
  status_column_line VARCHAR(50) := '--';

  --
  src_result VARCHAR(5000);
  dest_result VARCHAR(5000);
  -- indicator that a particular column ending in 'DB' is a POID component (POID_DB)
  is_poid_column NUMBER := 0;
  -- POID_DB component of destination %POID_LIST structure
  poid_list_poid_db VARCHAR2(32);
  -- converted source ACCOUNT_T.LINEAGE value
  convert_lineage VARCHAR2(1023);
  -- 
  poid_db_delta		 VARCHAR2(10);
  --
  diff_flag VARCHAR2(1);
  --
  CURSOR column_c(t_name VARCHAR2) IS 
    SELECT tc.column_name column_name
    FROM user_tab_columns tc
    WHERE tc.table_name = upper(t_name);
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
  p_status := 0;
  poid_db_delta := to_char(get_dest_database_id() - get_src_database_id(p_src_schema_name), 'S9999');
  -- print report header
  dbms_output.put_line(column_name_column_head||
                       src_value_column_head||dest_value_column_head||
                       diff_column_head||
                       status_column_head);
  dbms_output.put_line(column_name_column_line||
                       src_value_column_line||dest_value_column_line||
                       diff_column_line||
                       status_column_line);
  --
  FOR column_r IN column_c(p_table_name) LOOP
    -- src select
    BEGIN 
      EXECUTE IMMEDIATE 'SELECT '||column_r.column_name||' FROM '||p_src_schema_name||'.'||p_table_name||
                        ' WHERE poid_id0 = '||p_poid_id0 INTO src_result ;
    EXCEPTION
      WHEN NO_DATA_FOUND THEN
        dbms_output.put_line('no data found in source table');
	p_status := 4;
        EXIT;
    END;
    -- dest select
    BEGIN
      EXECUTE IMMEDIATE 'SELECT '||column_r.column_name||' FROM '||p_table_name||
                        ' WHERE poid_id0 = '||p_poid_id0 INTO dest_result ;
    EXCEPTION
      WHEN NO_DATA_FOUND THEN
        dest_result := NULL;
    END;
    -- verification steps:
    -- 1) has POID_DB in source database been set to NULL ?
    IF (column_r.column_name = 'POID_DB') THEN
      IF (nvl(src_result,'NULL') != 'NULL') THEN
        p_status := 1;
      END IF;
    END IF;
    -- 2) have all object reference _DB fields in destination database be set to the correct database id ?
    IF (column_r.column_name LIKE '%DB') THEN
      is_poid_column := 0;
      --
      SELECT 1 INTO is_poid_column
      FROM user_tab_columns t
      WHERE t.table_name = UPPER(table_name)
      AND t.column_name = SUBSTR(column_r.column_name, 
	                         1,
	                         LENGTH(column_r.column_name) -2)||'ID0' AND ROWNUM < 2;
      IF is_poid_column = 1 THEN	
        IF (dest_result != get_dest_database_id() AND src_result != 0) THEN
          p_status := 2;
        END IF;        
      END IF;
    ELSIF (column_r.column_name LIKE '%POID_LIST') THEN
      -- 3) has the POID_DB reference been updated
      --
      -- note: decode can only be used inside SELECT statement
      -- isolate POID_DB component from dest_result %POID_LIST structure
      IF (nvl(src_result,'NULL') != 'NULL') THEN
        SELECT substr(dest_result,1,instr(dest_result,'|',1,1)-1) INTO poid_list_poid_db FROM dual;
        -- dbms_output.put_line('poid_list_poid_db='||poid_list_poid_db);					       
        IF (poid_list_poid_db != '0.0.0.'||get_dest_database_id()) THEN
          p_status := 5;
        END IF;
      END IF;
    ELSIF (column_r.column_name LIKE 'LINEAGE') AND (p_table_name LIKE 'account_t') THEN
      -- 
      -- 4) has account_t.lineage field been converted ?
      --
      -- call custom Java stored procedure convert_account_lineage() 
      -- 
      IF (nvl(src_result,'NULL') != 'NULL') THEN
        SELECT convert_account_lineage(src_result, poid_db_delta) INTO convert_lineage FROM dual;
	-- dbms_output.put_line('convert_lineage='||substr(convert_lineage,1,254));
        IF (convert_lineage != dest_result) THEN
          p_status := 6;
        END IF;
      END IF;
    ELSIF (nvl(src_result,'NULL') != nvl(dest_result,'NULL')) THEN
      -- 5) have all other field be unchanged ?
        p_status := 3;
    END IF;
    -- report generation:  
    IF (nvl(src_result,'NULL') != nvl(dest_result,'NULL')) THEN
      diff_flag := '*';
    ELSE  
      diff_flag := ' ';
    END IF;
    --
    dbms_output.put_line(RPAD(column_r.column_name, LENGTH(column_name_column_head) -1)||
      '|'||RPAD(nvl(src_result,'NULL'), LENGTH(src_value_column_head) -1)||
      '|'||RPAD(nvl(dest_result,'NULL'), LENGTH(src_value_column_head) -1)||
      '|'||RPAD(diff_flag, LENGTH(diff_column_head) -1)||
      '|'||RPAD(p_status, LENGTH(status_column_head)));
  END LOOP;
  -- exception
END;    
/
show errors;
  







