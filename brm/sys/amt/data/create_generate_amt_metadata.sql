--
--  @(#)$Id: create_generate_amt_metadata.sql /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:21:25 nishahan Exp $ 
--     
-- Copyright (c) 1996, 2015, Oracle and/or its affiliates. All rights reserved.
--     
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--
create or replace
PROCEDURE generate_amt_metadata
	(primary_schema_name IN VARCHAR2)
  IS
  --
  -- procedure: generate_amt_metadata
  --   Generate and populate AMT metadata table 'amt_metadata'.
  --   This table contains a list of account related tables. Account tables
  --   attributes in the AMT metadata table are:
  --   table_name, srch_type (base table, child table, etc.) and column_list
  --
  table_c SYS_REFCURSOR;
  table_name_rec NVARCHAR2(255);
  obj_id0_rec NUMBER;
  field_type_rec NUMBER;
  srch_type_rec NVARCHAR2(1);
  table_does_not_exist exception;
  pragma exception_init(table_does_not_exist, -00942);
  --
  select_column_list VARCHAR2(4000);
  insert_column_list VARCHAR2(4000);
  v_exists NUMBER;
  --
  /*CURSOR table_c IS --
      SELECT distinct(substr(dof.sm_item_name,
                    decode(instr(dof.sm_item_name,'T='), 0, 1,instr(dof.sm_item_name,'T=')+2),
                    32)) table_name,
             dof.obj_id0,
             dof.field_type,
             decode(dof.field_type, 11, decode(t.table_name, 'ACCOUNT_T', 'P', decode(t.table_name, 'RECYCLE_SUSPENDED_USAGE_T', 'P',decode(t.table_name, 'TRANSACTION_ID_T', 'P', decode(t.table_name, 'GROUP_T', 'G', decode(t.table_name, 'AU_ACCOUNT_T', 'P', 'A'))))), 'O') srch_type
      FROM user_tables t, dd_objects_fields_t dof
	WHERE
	-- Exclude snapshots related
	t.table_name NOT LIKE 'MLOG$%'
	-- Exclude objects in primary database
        AND t.table_name NOT LIKE 'RUPD$%'
        -- Exclude tables not related to user accounts
        AND t.table_name NOT IN ('OPCODE_T', 'POP_T', 'SEARCH_T')
        -- Exclude tables related to hierarchies
	AND t.table_name NOT IN ('AU_UNIQUENESS_T')
        -- Excluding AMT job management tables
        AND t.table_name NOT LIKE 'JOB_%'
	-- Excluding CMT related tables if CMT got installed prior to AMT installation
	AND t.table_name NOT LIKE '%CMT%'
        -- Excluding Suspense Manager tables
        AND t.table_name NOT LIKE 'SUSP_USAGE%'
        AND t.table_name NOT LIKE 'SUSPENDED_USAGE%'
        AND t.table_name NOT IN ('TMP_SUSPENDED_USAGE_T')
	-- Excluding uniqueness table
        AND t.table_name NOT LIKE 'UNIQUENESS%'
	-- Excluding channels
        AND t.table_name NOT LIKE 'CHANNEL%'
        -- Exclude snapshots
        AND t.table_name NOT IN
          (SELECT s.table_name
           FROM primary_schema_name.md_static_objects_t s)
	AND lower(substr(dof.sm_item_name,
	      decode(instr(dof.sm_item_name,'T='), 0, 1,instr(dof.sm_item_name,'T=')+2),
              32)) = lower(t.table_name)
        AND dof.field_type in (6,9,10,11);*/
        --
  PROCEDURE generate_column_list (table_name IN VARCHAR2,
				  insert_column_list OUT VARCHAR2,
				  select_column_list OUT VARCHAR2)
  IS
          -- indicator that a particular column ending in 'DB' is a POID component (POID_DB)
     is_poid_column NUMBER := 0;

     -- indicator to see virtual_flag column exists
     is_virtual_column_exists NUMBER :=0;

     CURSOR column_c(t_name VARCHAR2) IS
       SELECT tc.column_name column_name
       FROM user_tab_columns tc
       WHERE tc.table_name = upper(t_name)
       ORDER BY tc.column_id;
  BEGIN
      dbms_output.put_line('table_name = <'||table_name||'>');

   -- check whether virtual_flag column exists in DD_OBJECTS_T table...
   Select count(*) into is_virtual_column_exists from DD_OBJECTS_T where VIRTUAL_FLAG = 1;

  FOR column_r IN column_c(table_name) LOOP
	dbms_output.put_line('column_name = '||column_r.column_name);

   -- ignore %_TYPE columns only when virtual column feature is enabled and table is event or its subtables..
   IF (is_virtual_column_exists > 0) AND (column_r.column_name LIKE '%_TYPE') AND (table_name like 'event%') THEN
	   --
	   -- ignore these columns because these are virtual columns
	   --
  	   dbms_output.put_line('ignore column = '||column_r.column_name||' because this is a virtual column');
	   -- continue the loop without adding this column into amt_metadata_t
	   CONTINUE;
           --
    	   --
   END IF;

	insert_column_list := insert_column_list||', t.'||column_r.column_name;
	 --
	 IF (column_r.column_name LIKE '%DB') THEN
	    is_poid_column := 0;
	    --
	    SELECT 1 INTO is_poid_column
            FROM user_tab_columns t
            WHERE t.table_name = UPPER(table_name)
	    AND t.column_name = SUBSTR(column_r.column_name,
	                               1,
	                               LENGTH(column_r.column_name) -2)||'ID0' AND ROWNUM < 2;
	    --
	    IF is_poid_column = 1 THEN
	       -- insert placehold # for offset (to be replace at AMT runtime, see amt_mv.migrate_account_batch()
	       select_column_list := select_column_list||
	         ', decode(t.'||column_r.column_name||', 0, 0, t.'||column_r.column_name||' #)';
	       dbms_output.put_line('replace conventional reference with dest_db_id');
	    END IF;
	 ELSIF (column_r.column_name LIKE '%POID_LIST' OR  column_r.column_name LIKE '%OBJ_LIST') THEN
	   --
	   -- operation: rewrite the POID_DB reference in %_POID_LIST and %_OBJ_LIST fields 
	   -- while preserving the null value
	   --
	   -- assumption: POID_DB reference is starts at character position 1 and the format is 0.0.0.x
	   --
	   -- call custom Java stored procedure convert_poid_list()
	   --
  	   select_column_list := select_column_list||
              ', convert_poid_list('||column_r.column_name||', #)';
           dbms_output.put_line('replace list reference with dest_db_id');
	 ELSIF (column_r.column_name LIKE 'LINEAGE') AND (table_name LIKE 'account_t' OR table_name LIKE 'au_account_t') THEN
	   --
	   -- call custom Java stored procedure convert_account_lineage()
	   --
  	   select_column_list := select_column_list||
             ', convert_account_lineage('||column_r.column_name||', #)';
           dbms_output.put_line('replace lineage reference with dest_db_id');
           --
	 ELSIF (column_r.column_name LIKE 'INVOICE_DATA') AND (table_name LIKE 'event_t') THEN
	 --
	 -- call custom Java stored procedure convert_flist_string()
	 --
	 select_column_list := select_column_list||
	   ', decode(INVOICE_DATA,'''','''',convert_flist_string('||column_r.column_name||', #, ''7''))';
	 dbms_output.put_line('replace invoice_data having db reference with dest_db_id');
	 --
	 ELSE
	    select_column_list := select_column_list||', t.'||column_r.column_name;
	 END IF;
      END LOOP;
      -- remove first ','
      insert_column_list := ltrim(insert_column_list, ', ');
      select_column_list := ltrim(select_column_list, ', ');
  END;
  --
  BEGIN
  --
  OPEN table_c FOR 'SELECT distinct(substr(dof.sm_item_name,
                    decode(instr(dof.sm_item_name,''T=''), 0, 1,instr(dof.sm_item_name,''T='')+2),
                    32)) table_name,
             dof.obj_id0,
             dof.field_type,
             decode(dof.field_type, 11, decode(t.table_name, ''ACCOUNT_T'', ''P'', decode(t.table_name, ''RECYCLE_SUSPENDED_USAGE_T'', ''P'',decode(t.table_name, ''TRANSACTION_ID_T'', ''P'', decode(t.table_name, ''GROUP_T'', ''G'', decode(t.table_name, ''AU_ACCOUNT_T'', ''P'', ''A''))))), ''O'') srch_type
      FROM user_tables t, dd_objects_fields_t dof
        WHERE
        t.table_name NOT LIKE ''MLOG$%''
        AND t.table_name NOT LIKE ''RUPD$%''
        AND t.table_name NOT IN (''OPCODE_T'', ''POP_T'', ''SEARCH_T'')
        AND t.table_name NOT IN (''AU_UNIQUENESS_T'')
        AND t.table_name NOT LIKE ''JOB_%''
        AND t.table_name NOT LIKE ''%CMT%''
        AND t.table_name NOT LIKE ''SUSP_USAGE%''
        AND t.table_name NOT LIKE ''SUSPENDED_USAGE%''
        AND t.table_name NOT IN (''TMP_SUSPENDED_USAGE_T'')
        AND t.table_name NOT LIKE ''UNIQUENESS%''
        AND t.table_name NOT LIKE ''CHANNEL%''
        AND t.table_name NOT IN
          (SELECT s.table_name
           FROM '||primary_schema_name||'.md_static_objects_t s)
        AND lower(substr(dof.sm_item_name,
              decode(instr(dof.sm_item_name,''T=''), 0, 1,instr(dof.sm_item_name,''T='')+2),
              32)) = lower(t.table_name)
        AND dof.field_type in (6,9,10,11)';

  LOOP
    FETCH table_c INTO table_name_rec,obj_id0_rec,field_type_rec,srch_type_rec;
    EXIT WHEN table_c%NOTFOUND;
    dbms_output.put_line('table_name = '||table_name_rec||', srch_type = '||srch_type_rec);
    --
    generate_column_list(table_name_rec,
                         insert_column_list,
                         select_column_list);
    --
	BEGIN
		SELECT DISTINCT 1 INTO v_exists
			FROM amt_metadata_t
			WHERE table_name = table_name_rec;

		EXCEPTION
			WHEN NO_DATA_FOUND THEN
				INSERT INTO amt_metadata_t
					(dd_obj0,
					field_type,
					srch_type,
					table_name,
					insert_column_list,
					select_column_list)
				VALUES (obj_id0_rec,
					field_type_rec,
					srch_type_rec,
					table_name_rec,
					insert_column_list,
					select_column_list);
	END;

  END LOOP;
  --
  COMMIT;
END;
/
show errors;
