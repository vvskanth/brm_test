--
--      @(#) % %
--     
-- Copyright (c) 1996, 2015, Oracle and/or its affiliates. All rights reserved.
--     
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--

SET serveroutput ON SIZE 1000000
CREATE OR REPLACE FUNCTION cleanup_instance(src_instance IN VARCHAR2,
     					     f_primary IN CHAR,
                                             src_schema_name IN VARCHAR2,
                                             primary_schema_name IN VARCHAR2,
					     log_file_flag IN CHAR,
					     log_file_dir IN VARCHAR2) RETURN NUMBER

IS
  CURSOR job_c(p_src_database IN VARCHAR2) IS
    SELECT obj_id0 
    FROM job_amt_t j
    WHERE j.status = 3 -- state FINISHED
    AND j.src_database = p_src_database;
  CURSOR batch_c(p_job_id IN NUMBER) IS
    SELECT poid_id0
    FROM job_batch_t b, job_batch_amt_t c
    WHERE c.status = 3 -- state FINISHED
    AND c.obj_id0 = b.poid_id0 
    AND b.job_obj_id0 = p_job_id;
  --
  sql_stmt    VARCHAR2(5000);
  --
  batch_cnt   NUMBER;
  --
BEGIN
   batch_cnt := 0;
   FOR job_r IN job_c(src_instance) LOOP
     FOR batch_r IN batch_c(job_r.obj_id0) LOOP
       dbms_output.put_line('cleanup_instance: deletion of job_id = '||
                            job_r.obj_id0||' , batch_id = '||batch_r.poid_id0);
       IF (f_primary = 'Y') THEN
         sql_stmt := 'BEGIN amt_mv.delete_account_batch('||
                        job_r.obj_id0||', '||batch_r.poid_id0||', '''||primary_schema_name||''', '''||
                        log_file_flag||''', '''||log_file_dir||'''); END; ';
       ELSE
         sql_stmt := 'BEGIN '||src_schema_name||'.amt_mv.delete_account_batch('||
                        job_r.obj_id0||', '||batch_r.poid_id0||', '''||primary_schema_name||''', '''||
                        log_file_flag||''', '''||log_file_dir||'''); END; ';
       END IF;
       dbms_output.put_line('cleanup_instance: '||sql_stmt);
       EXECUTE IMMEDIATE sql_stmt;      
       -- update job_batch_amt_t status ...
       UPDATE job_batch_amt_t 
         SET status = 5 -- state SRC_DELETED
         WHERE obj_id0 in 
	 ( SELECT poid_id0 FROM job_batch_t
	   WHERE job_obj_id0 = job_r.obj_id0
           AND poid_id0 = batch_r.poid_id0);
       --
       batch_cnt := batch_cnt + 1;
       --
       dbms_output.put_line('cleanup_instance: commit account batch number = '||batch_cnt);	
       COMMIT;
       --
     END LOOP;
   END LOOP;
   -- return total number of account batches 
   RETURN batch_cnt;
   --
   EXCEPTION
    WHEN OTHERS THEN
      --
      dbms_output.put_line('cleanup_instance: rollback');	
      ROLLBACK;
      --
      RAISE;
END;
/
show errors;


