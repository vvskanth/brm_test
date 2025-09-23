SET serveroutput ON SIZE 1000000
CREATE OR REPLACE PROCEDURE account_profile(p_poid_id0_list IN VARCHAR2)
IS
   -- define report layout
   table_name_column_head VARCHAR2(30) := 'Table Name                |';
   table_name_column_line VARCHAR2(30) := '---------------------------';
   table_type_column_head VARCHAR2(30) := 'Type|';
   table_type_column_line VARCHAR2(30) := '-----';
   rec_cnt_column_head VARCHAR2(20) := 'Rec. Cnt.|';
   rec_cnt_column_line VARCHAR2(20) := '----------';
   avg_cnt_column_head VARCHAR2(20) := 'Avg. Rec. Cnt.|';
   avg_cnt_column_line VARCHAR2(20) := '---------------';
   --
   acct_num NUMBER;
   --
   CURSOR amt_base_table_c IS
     SELECT mt.table_name, mt.srch_type, mt.insert_column_list, mt.select_column_list
     FROM amt_metadata_t mt
     WHERE mt.table_name NOT LIKE 'au%'
     AND mt.srch_type IN ('P', 'A')  
     ORDER BY mt.srch_type;
   --	
   CURSOR amt_chld_table_c(j_id IN NUMBER, b_id IN NUMBER) IS  
     SELECT DISTINCT p.table_name, mt.srch_type, mt.insert_column_list, mt.select_column_list  
     FROM amt_base_detail_t b, amt_poid_type_map_t p, amt_metadata_t mt
     WHERE b.poid_type = p.poid_type 
     AND p.table_name NOT LIKE b.base_table_name 
     AND p.table_name = mt.table_name  
     AND b.job_id = j_id
     AND b.batch_id = b_id;       
   --
   PROCEDURE print_table_header IS
   BEGIN
      dbms_output.put_line(table_name_column_head||
	       	           table_type_column_head||
			   rec_cnt_column_head||
			   avg_cnt_column_head);
      dbms_output.put_line(table_name_column_line||
		           table_type_column_line||
			   rec_cnt_column_line||
			   avg_cnt_column_line);
   END;
   --
   PROCEDURE base_table_rec_cnt(table_name IN VARCHAR2, 
		                srch_type IN VARCHAR2,
   		                poid_id0_list IN VARCHAR2,
		                acct_num IN NUMBER)	     
   IS
     poid_column_name		 VARCHAR2(100);
     select_stmt                 VARCHAR2(512);
     insert_stmt                 VARCHAR2(512);
     table_rec_cnt		 NUMBER;
   BEGIN   
      --
      IF (INSTR(srch_type, 'P') != 0) THEN
	 -- account_t
	 poid_column_name := 'poid_id0';
      ELSE 
         -- all other base tables (srch_type = 'A')
	 poid_column_name := 'account_obj_id0';
      END IF;
      --
      select_stmt := 'SELECT count(*) FROM '||table_name||' t '||
	' WHERE t.'||poid_column_name||' IN ('||poid_id0_list||')';
      insert_stmt := 'INSERT INTO amt_base_detail_t '||
	'SELECT t.poid_id0, t.poid_type, '''||table_name||''', '||
	'0, 0 FROM '||table_name||' t '||
	' WHERE t.'||poid_column_name||' IN ('||poid_id0_list||')';
      --
      -- dbms_output.put_line('select_stmt = '||select_stmt);
      -- dbms_output.put_line('insert_stmt = '||insert_stmt);
      --
      EXECUTE IMMEDIATE select_stmt INTO table_rec_cnt;
      EXECUTE IMMEDIATE insert_stmt;

      IF table_rec_cnt != 0 THEN
        dbms_output.put_line(RPAD(table_name, LENGTH(table_name_column_head) - 1)||
   		             '|'||RPAD(srch_type, LENGTH(table_type_column_head) - 1)||
			     '|'||RPAD(table_rec_cnt, LENGTH(rec_cnt_column_head) - 1)||
			     '|'||RPAD(table_rec_cnt/acct_num, LENGTH(avg_cnt_column_head) -1));
      END IF;
   END;    
   --
   PROCEDURE child_table_rec_cnt(table_name IN VARCHAR2, 
   		                 srch_type IN VARCHAR2,
		                 acct_num IN NUMBER)	     
   IS
     select_stmt                 VARCHAR2(512);
     table_rec_cnt		 NUMBER;
   BEGIN   
      --
      select_stmt := 'SELECT count(*) FROM '||table_name||' t, amt_base_detail_t b '||
	' WHERE t.obj_id0 = b.poid_id0 AND b.job_id = 0 AND b.batch_id = 0';
      --
      -- dbms_output.put_line('select_stmt = '||select_stmt);
      --
      EXECUTE IMMEDIATE select_stmt INTO table_rec_cnt;

      IF table_rec_cnt != 0 THEN
        dbms_output.put_line(RPAD(table_name, LENGTH(table_name_column_head) - 1)||
     		             '|'||RPAD(srch_type, LENGTH(table_type_column_head) - 1)||
			     '|'||RPAD(table_rec_cnt, LENGTH(rec_cnt_column_head) - 1)||
			     '|'||RPAD(table_rec_cnt/acct_num, LENGTH(avg_cnt_column_head) -1));
      END IF;
   END;    
   --
BEGIN
   BEGIN
      --
      dbms_output.put_line('	');
      dbms_output.put_line('Account Profile Report');
      dbms_output.put_line('======================');
      --
      EXECUTE IMMEDIATE 'SELECT count(*) from account_t where poid_id0 IN ('||p_poid_id0_list||')' 
      INTO acct_num;
      --     
      dbms_output.put_line('	');
      dbms_output.put_line('total number of accounts = '||acct_num);
      dbms_output.put_line('	');
      --
      print_table_header();
      --
      FOR amt_table_r IN amt_base_table_c LOOP
	 base_table_rec_cnt(amt_table_r.table_name, 
	   	            amt_table_r.srch_type,
                            p_poid_id0_list,
		            acct_num);
      END LOOP;
      --
      FOR amt_table_r IN amt_chld_table_c(0, 0) LOOP
	 child_table_rec_cnt(amt_table_r.table_name, 
	                     amt_table_r.srch_type,
			     acct_num);
      END LOOP;	 
      -- delete all data in amt_base_detail_t with job_id = 0 and batch_id = 0	
      DELETE amt_base_detail_t WHERE job_id = 0 AND batch_id = 0;
      COMMIT;
      --
      dbms_output.put_line('	');
      dbms_output.put_line('Type codes:');
      dbms_output.put_line('-----------');
      dbms_output.put_line('P - Infranet account_t base-class table');
      dbms_output.put_line('A - Infranet base-class table');
      dbms_output.put_line('O - Infranet sub-class table');
   END;   
END; 
/
show errors;
  




