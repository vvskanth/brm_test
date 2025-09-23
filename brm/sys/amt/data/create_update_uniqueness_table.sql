--
--	@(#)$Id: create_update_uniqueness_table.sql /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:21:31 nishahan Exp $
--     
-- Copyright (c) 1996, 2015, Oracle and/or its affiliates. All rights reserved.
--     
--	This material is the confidential property of Oracle Corporation or its
-- 	licensors and may be used, reproduced, stored or transmitted only in
--	accordance with a valid Oracle license or sublicense agreement.
--

CREATE OR REPLACE PROCEDURE update_uniqueness_table(job_id IN NUMBER,
                                                    batch_id IN NUMBER,
                                                    dest_db_id IN NUMBER)
IS
  v_cnt_au_uniq INTEGER;
BEGIN

  EXECUTE IMMEDIATE 'UPDATE uniqueness_t u '||
    ' SET u.account_obj_db = :1, u.service_obj_db = :2, u.POID_REV = u.POID_REV + 1 WHERE u.account_obj_id0 IN '||
    ' (SELECT b.account_obj_id0 FROM job_batch_accounts_t b WHERE b.job_id = :3  AND b.obj_id0 = :4)' USING dest_db_id,dest_db_id,job_id,batch_id;

  SELECT COUNT(1) INTO v_cnt_au_uniq FROM TABS WHERE TABLE_NAME = 'AU_UNIQUENESS_T';
  IF v_cnt_au_uniq>0 THEN
  
    EXECUTE IMMEDIATE 'UPDATE AU_UNIQUENESS_T u '||
      ' SET u.account_obj_db = :1, u.service_obj_db = :2 WHERE u.account_obj_id0 IN '||
      ' (SELECT b.account_obj_id0 FROM job_batch_accounts_t b WHERE b.job_id = :3 AND b.obj_id0 = :4)' USING dest_db_id,dest_db_id,job_id,batch_id;
  END IF;

  -- update unique_acct_no_t with target db number
  EXECUTE IMMEDIATE 'UPDATE unique_acct_no_t u SET u.account_obj_db = :1 WHERE u.account_obj_id0 IN '||
    ' (SELECT b.account_obj_id0 FROM job_batch_accounts_t b WHERE b.job_id = :2 AND b.obj_id0 = :3)' USING dest_db_id,job_id,batch_id;
	
END;
/
show errors;
