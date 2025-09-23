--
--	@(#)$Id: create_amt_rerate_pkb.sql /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:21:12 nishahan Exp $
--
-- Copyright (c) 1996, 2015, Oracle and/or its affiliates. All rights reserved.
--     
--	This material is the confidential property of Oracle Corporation or its
--	licensors and may be used, reproduced, stored or transmitted only in
--	accordance with a valid Oracle license or sublicense agreement.
--

--This package will help in eliminating the gap between AMT and rerate.
--All the accounts which are in an active rerate job will either be blocked or migrated along with their rerate job
--depending upon their rerate status.
-- A number of console output statement has been added and commented out for debugging purpose.
-- One can uncomment them during the time debugging.
CREATE OR REPLACE PACKAGE BODY amt_rerate
IS
PROCEDURE handle_gap_in_batch(amt_job_id IN number ,amt_batch_id IN number,batch_type IN number,dest_schema_name IN varchar,log_message OUT clob) ; 
PROCEDURE release_group(job_id IN number);
PROCEDURE migrate_rerate_batch(account_poid IN number,dest_schema_name IN varchar,src_poid_db number,dest_poid_db number,log_message out clob);
PROCEDURE account_cleanup(job_id IN number,batch_id IN number , account_poid number);
FUNCTION check_accnt_for_block(account_poid IN number) RETURN number;
FUNCTION check_accnt_for_trns_lst(account_poid IN number) RETURN number;
FUNCTION test_for_batch_block(job_id number,batch_id number) RETURN number;
PROCEDURE update_buffer(batch_id IN number,rec_id number,source_db IN number,dest_db IN number);
FUNCTION to_hex(dec_no number) RETURN varchar2;
--This Procedure will find each and every account of an AMT job in source
--Also this will be the entry point from java code to this package
PROCEDURE eliminate_rerate_gap
(amt_job_id  NUMBER,
dest_schema_name IN VARCHAR,
log_message OUT CLOB)
IS
  amt_batch_id NUMBER;
  temp clob;
  batch_type NUMBER;
  CURSOR list_amt_batches_cur
  IS
  SELECT poid_id0,batch_type
  FROM
  job_batch_t,job_batch_amt_t
  WHERE
  job_obj_id0 = amt_job_id AND
  job_batch_t.poid_id0=job_batch_amt_t.obj_id0
  ;
  BEGIN
     log_message := '';
	 temp :='';
  	 FOR   list_amt_batches_cur_rec IN list_amt_batches_cur
	 LOOP
	 	 
	 	 amt_batch_id := list_amt_batches_cur_rec.poid_id0;
		 batch_type :=   list_amt_batches_cur_rec.batch_type;
		 
		 handle_gap_in_batch (amt_job_id,amt_batch_id,batch_type,
		 					  dest_schema_name,temp);
		 log_message := log_message || temp;					  
		 
	 END LOOP;
	 COMMIT;
	 EXCEPTION
	 	WHEN OTHERS THEN
		raise_application_error('-20100', 'Exception raised from PIN_RERATE:' 
					  		  || SQLCODE||' '||SQLERRM);
	 	--log_message:=log_message || 'Exception raised from amt_rerate.eliminate_rerate_gap:' 
					  		  --|| SQLCODE||' '||SQLERRM;
 END eliminate_rerate_gap; 	   	  


PROCEDURE handle_gap_in_batch
(amt_job_id IN number ,
amt_batch_id IN number,
batch_type IN number,
dest_schema_name IN varchar,
log_message OUT clob) 
IS
	account_poid number;
	block_for_rerate number;
	block_for_line_transfer number;
	batch_flag number;
	block_flag number;
	batch_id number;
	src_poid_db number;
	dest_poid_db number;
	temp clob;	
	CURSOR all_accounts_cur 
	IS 
	   SELECT 
	   job_batch_accounts_t.account_obj_id0
	   FROM 
	   job_batch_t,job_batch_accounts_t 
	   WHERE  
	   job_batch_t.poid_id0=job_batch_accounts_t.obj_id0 AND
	   job_batch_t.poid_id0=amt_batch_id AND job_batch_t.poid_type='/job_batch/amt'
	   ;
	CURSOR all_batches_cur 
	IS 
	   SELECT job_batch_t.poid_id0 
	   FROM 
	   job_batch_t,job_batch_amt_t 
	   WHERE 
	   job_batch_t.job_obj_id0=amt_job_id AND job_batch_t.poid_type='/job_batch/amt' AND 
	   job_batch_t.poid_id0=job_batch_amt_t.obj_id0 AND job_batch_amt_t.group_id!=0
	   ;
	BEGIN				
		EXECUTE IMMEDIATE 'SELECT '||
		'to_number(substr(src_database,length(src_database),1)) , '||
		'to_number(substr(dest_database,length(dest_database),1)) '||
		'FROM job_amt_t '||
		'WHERE obj_id0=:1' INTO src_poid_db,dest_poid_db USING amt_job_id;
		
		
		--This flag will be on when this batch got removed for some reason
		--so that we will not proceed further in java code
		log_message:=log_message  
					 			  || 'AMT batch id:'|| amt_batch_id || ';'
					 			  || 'AMT Batch type:'|| batch_type || ';'
					 			  || chr(10);			     		
		--If it is a group type batch first check whether there
		--is any batch for which all the group batches in this job
		--need to be blocked
		IF(batch_type=2) THEN 
			FOR all_batches_cur_rec IN all_batches_cur
			LOOP				
				batch_id:=all_batches_cur_rec.poid_id0;
				--dbms_output.put_line('batch_id=' || batch_id);
				block_flag:=test_for_batch_block(amt_job_id,batch_id);
				IF (block_flag=1) THEN
					--dbms_output.put_line('Found account in rerate status 4 for batch_id '||
					--batch_id ||' of job id ' || amt_job_id);
					log_message:=log_message || 'Found one or more account in an active rerate job with ' || 
					' status either  PIN_RERATE_STATUS_WAITING_ACCOUNT_LOCK or 
					PIN_RERATE_STATUS_ACCOUNT_LOCKED  or
					PIN_RERATE_STATUS_RERATED  or
					PIN_RERATE_STATUS_READY_FOR_RECYCLE.' || chr(10);
					
					release_group(amt_job_id);
					log_message:=log_message || 'Blocked all the group batches of this job. ' || chr(10);	
					RETURN;
				END IF;
				IF(block_flag=2) THEN
					--dbms_output.put_line('Found account in transfer list for batch_id '||
					--batch_id ||' of job id ' || amt_job_id);
					log_message:=log_message || 'Found one more account in transfer list. ' || chr(10); 	
					release_group(amt_job_id);
					log_message:=log_message || 'Blocked all the group batches of this job. ' || chr(10);	
					RETURN;
				END IF;
			END LOOP;
		END IF;
		--Here we came to know that this is  either
		--1)single type batch or 
		--2)Group type batch which needn't be blocked.
		--Hence we may only need to either 
		--1)block an account or 
		--2)create a new rerate job for this account in destination
		FOR all_accounts_cur_rec IN all_accounts_cur 
		LOOP
			account_poid:=all_accounts_cur_rec.account_obj_id0; 
			--dbms_output.put_line('Account poid:' || account_poid);			
			--If found status 4 jobs for this account or
			--found this account in transfer list then 
			--block this account in source
			block_for_rerate:=check_accnt_for_block(account_poid);
			block_for_line_transfer:=check_accnt_for_trns_lst(account_poid);
			IF(block_for_rerate!=0) OR (block_for_line_transfer!=0)
			THEN
			--We have got some accounts which we need to block
				BEGIN
					IF (block_for_rerate!=0) THEN
						--dbms_output.put_line('Found this account with rerate status 4');
						log_message:=log_message || 'Found account poid('|| account_poid ||') in an active rerate job with' || 	
						' status either  PIN_RERATE_STATUS_WAITING_ACCOUNT_LOCK or 
						PIN_RERATE_STATUS_ACCOUNT_LOCKED  or
						PIN_RERATE_STATUS_RERATED  or
						PIN_RERATE_STATUS_READY_FOR_RECYCLE.' || chr(10);
					END IF;
					IF (block_for_line_transfer!=0) THEN
						--dbms_output.put_line('Found this account in transfer list');
						log_message:=log_message || 'Found account poid(' || account_poid || ') in'|| 
						'transfer '|| 'list' || chr(10);						
					END IF;	
					account_cleanup(amt_job_id,amt_batch_id,account_poid);
					log_message:=log_message || 'Blocked account poid(' || account_poid|| ') of this ' || 
					'batch' || chr(10);														
				END;
			--None of the account in this batch are need to be blocked
			--Lets check now whether any account is part of an active rerate job
			--with status other then 4 and -1.
			ELSE
				BEGIN
					--dbms_output.put_line('Neither status 4 nor in transfer list');
					migrate_rerate_batch(account_poid,dest_schema_name,src_poid_db,dest_poid_db,temp);
					log_message:=log_message || temp;					
				END;
			END IF;
		END LOOP;
		--COMMIT;
		/*EXCEPTION
			WHEN OTHERS THEN
			log_message:=log_message || 'Exception raised from handle_gap_in_batch:' 
				     || SQLCODE||' '||SQLERRM;*/
	END handle_gap_in_batch;
--This function will check each and every account of a batch to see
--Whether it is there either in an active rerate job with status 2,3,4,5 or 
--in a transfer list
FUNCTION test_for_batch_block
(job_id number,
batch_id number) 
RETURN number
IS
account_poid number;
CURSOR all_accounts_cur 
IS 
	SELECT 
	job_batch_accounts_t.account_obj_id0
	FROM 
	job_batch_t,job_batch_accounts_t 
	WHERE  
	job_batch_t.poid_id0=job_batch_accounts_t.obj_id0 AND
	job_batch_t.poid_id0=batch_id AND job_batch_t.poid_type='/job_batch/amt'
	;
BEGIN	
	FOR all_accounts_cur_rec IN all_accounts_cur 
	LOOP
		account_poid:=all_accounts_cur_rec.account_obj_id0;
		IF (check_accnt_for_block(account_poid)!=0) THEN
			RETURN 1;
		END IF;
		if (check_accnt_for_trns_lst(account_poid)!=0) then
			RETURN 2;
		END IF;		
	END LOOP;
	RETURN 0;
	
END test_for_batch_block;
--This procedure will release an account from a batch
PROCEDURE account_cleanup
(job_id IN number,
batch_id IN number , 
account_poid number
)
IS
batch_cnt number;
acct_cnt number;
BEGIN
	
	--dbms_output.put_line('Job Id:'||job_id);
	--dbms_output.put_line('Batch id:'||batch_id);
	--dbms_output.put_line('Account poid:'||account_poid);
	
	--Delete this account 
	--dbms_output.put_line('Deleting the account');
	EXECUTE IMMEDIATE 'DELETE FROM job_batch_accounts_t WHERE account_obj_id0= :1 AND job_id= :2 AND obj_id0= :3' 
	USING account_poid,job_id,batch_id;
	--If the account count for this batch is one then:
	--1)remove this batch
	--2)If the batch count for this job is one then
	--  remove this job else decrment the batch count by one
	--else decrement the account count by one
	EXECUTE IMMEDIATE 'SELECT account_cnt FROM job_batch_t WHERE poid_id0= :1 AND job_obj_id0= :2' INTO acct_cnt
	USING batch_id,job_id; 

	IF(acct_cnt=1) THEN
	BEGIN
		--dbms_output.put_line('Total Number of accounts is 1');
		--dbms_output.put_line('Deleting the batch from job_batch_t and job_batch_amt_t');
		EXECUTE IMMEDIATE 'DELETE FROM job_batch_t WHERE poid_id0= :1' USING batch_id;
		EXECUTE IMMEDIATE 'DELETE FROM job_batch_amt_t WHERE obj_id0= :1' USING batch_id;
		
		EXECUTE IMMEDIATE 'SELECT batch_cnt FROM job_t WHERE  poid_id0= :1' INTO batch_cnt USING job_id;
		IF(batch_cnt=1) THEN
		BEGIN   	 				 
			--dbms_output.put_line('Total Number of batches is 1');
			--dbms_output.put_line('Deleting the job from job_t,job_amt_t and amt_account_batch_table_t');
			EXECUTE IMMEDIATE 'DELETE FROM job_t WHERE poid_id0= :1' USING job_id;
			EXECUTE IMMEDIATE 'DELETE FROM job_amt_t WHERE obj_id0= :1' USING job_id;
			EXECUTE IMMEDIATE 'DELETE FROM amt_account_batch_table_t WHERE job_id= :1' USING job_id;		
			
		END;
		ELSE
		BEGIN
			--dbms_output.put_line('Total Number of batches is' ||  batch_cnt);
			--dbms_output.put_line('Decrmenting the batch count by one');
			EXECUTE IMMEDIATE 'UPDATE job_t SET batch_cnt=batch_cnt-1 WHERE poid_id0= :1' USING job_id;
		END;
	END IF;
	END;
	ELSE
	BEGIN
		--dbms_output.put_line('Total Number of accounts is' ||  acct_cnt);
		--dbms_output.put_line('Decrmenting the account count by one');
		EXECUTE IMMEDIATE 'UPDATE job_batch_t SET account_cnt=account_cnt-1 WHERE poid_id0= :1' USING batch_id;
	END;
	END IF;
END account_cleanup;
--This function will check whether an account from AMT batch is in rerate status 2,3,4,5
FUNCTION check_accnt_for_block
(account_poid IN number) 
RETURN number
AS
	cnt number;
	BEGIN
		--Find the number of rerate batches where this account has been populated with rerate job status 2,3,4,5
		EXECUTE IMMEDIATE 'SELECT  count(job_batch_accounts_t.obj_id0) FROM job_batch_accounts_t,job_batch_t,job_rerate_t WHERE '||
		'job_batch_accounts_t.account_obj_id0= :1 AND job_batch_accounts_t.obj_id0=job_batch_t.poid_id0 '||
		'AND job_batch_t.poid_type=''/job_batch/rerate'' AND job_batch_t.job_obj_id0=job_rerate_t.obj_id0 AND '||
		'(status=2 OR status =3 OR status=4 OR status=5)' INTO cnt USING account_poid;
		RETURN cnt;
	END check_accnt_for_block;
--This function will check whether an account from AMT batch is in transfer list
FUNCTION check_accnt_for_trns_lst
(account_poid IN number) RETURN number
IS
	cnt number;
	BEGIN
		--Check whether this account is there in transfer list
		EXECUTE IMMEDIATE 'SELECT count(*) FROM service_transfer_list_t '||
		'WHERE account_obj_id0= :1' INTO cnt USING account_poid;
		RETURN cnt;
	END check_accnt_for_trns_lst;	
--This procedure will clean all related rerate job:batches in source
--and create a new job:batch in destination to populate this account
--We will call this procedure when we definitely want to migrate the rerate
--jobs containg accounts which are also selected for migration
PROCEDURE migrate_rerate_batch
(account_poid IN number,
dest_schema_name IN varchar,
src_poid_db number,
dest_poid_db number,
log_message OUT clob
) 
IS
	v_partition_mode INTEGER;

	--Select the list of rerate batches in source where this account poid has been populated and the rerate 
	--status is pin_rerate_status_new(1)
	CURSOR all_accounts_cur 
	IS
	SELECT  
	job_batch_accounts_t.obj_id0 
	FROM 
	job_batch_accounts_t,job_batch_t,job_rerate_t 
	WHERE
	job_batch_accounts_t.account_obj_id0=account_poid AND job_batch_accounts_t.obj_id0=job_batch_t.poid_id0
	AND job_batch_t.poid_type='/job_batch/rerate' AND job_batch_t.job_obj_id0=job_rerate_t.obj_id0 AND
	status=1;
	old_batch_id number;
	old_job_id number;
	new_job_id number;
	new_batch_id number;
	rec_id number;
	cnt number;
	stmt_str varchar(255);
	batch_cnt number;
	rerate_request_size number;
	acct_obj_db number;
	acct_obj_id number;
	acct_obj_type varchar(255);
	acct_obj_rev number;
        v_start_id INTEGER;
        v_end_id INTEGER;
        v_inc_by INTEGER;
        v_cnt INTEGER;
	BEGIN
		cnt:=-1;
		batch_cnt:=1;

                EXECUTE IMMEDIATE 'SELECT count(*) FROM job_batch_accounts_t,job_batch_t,job_rerate_t WHERE '||
                  'job_batch_accounts_t.account_obj_id0=:1 AND job_batch_accounts_t.obj_id0=job_batch_t.poid_id0 '||
                  'AND job_batch_t.poid_type=''/job_batch/rerate'' AND job_batch_t.job_obj_id0=job_rerate_t.obj_id0 AND '||
                  'status=1' INTO v_cnt USING account_poid;

		IF v_cnt != 0 THEN
                	pin_sequence.get_new_poid_id_range((v_cnt * 2), v_start_id, v_end_id, v_inc_by, v_partition_mode);
		END IF;
		
		FOR all_accounts_cur_rec IN all_accounts_cur
		LOOP
			
			old_batch_id:=all_accounts_cur_rec.obj_id0;
			--dbms_output.put_line('Old Batch id is ' || old_batch_id);
			BEGIN --This block is for inserting a record into destination.job_t
				--Get the old job id for this batch id in source
				EXECUTE IMMEDIATE 'SELECT job_obj_id0 FROM job_batch_t '||
				' WHERE poid_id0= :old_batch_id' INTO old_job_id USING old_batch_id;
				--dbms_output.put_line('Old Job id is ' || old_job_id);
				--Generate a new job id in destination
                                new_job_id := v_start_id;
                                v_start_id := v_start_id + v_inc_by ;
				--dbms_output.put_line('New Job id is:' || new_job_id);			
				--dbms_output.put_line('Destination poid db is:' || dest_poid_db);
				--Copy the job record from source and paste it in destination
				stmt_str:='insert into '||dest_schema_name||'.job_t select * from job_t where 
				poid_id0=:old_job_id';
				EXECUTE IMMEDIATE stmt_str using old_job_id;
				--Change the poid db ,job id  and batch_cnt for this record
				stmt_str:='update '||dest_schema_name||'.job_t set 
				poid_id0=:new_job_id,poid_db=:dest_poid_db,batch_cnt=:batch_cnt
				where poid_id0=:old_job_id';
				EXECUTE IMMEDIATE  stmt_str using new_job_id,dest_poid_db,batch_cnt,old_job_id;
				--dbms_output.put_line('Inserted one record  in job_t with job id as  ' || new_job_id );
			END;
			BEGIN  --This block is for inserting a record into destination.job_batch_t
				--Genarete a new  batch id  in destination
                                new_batch_id := v_start_id;
                                v_start_id := v_start_id + v_inc_by ;
				--dbms_output.put_line('The new batch id :' || new_batch_id);
				--Copy the old record from source and paste it in destination
				stmt_str:='insert into '||dest_schema_name||'.job_batch_t select * from job_batch_t where 
				poid_id0=:old_batch_id';
				EXECUTE IMMEDIATE stmt_str using old_batch_id;				
				--Change the  job_obj_ido,poid_id0,account_cnt,batch_num and poid_db
				stmt_str:='update '||dest_schema_name||'.job_batch_t set 
				poid_id0=:new_batch_id,job_obj_id0=:new_job_id,
				poid_db=:dest_poid_db,batch_num=:batch_cnt,account_cnt=:batch_cnt where 
				poid_id0=:old_batch_id ';
				EXECUTE IMMEDIATE stmt_str using 
				new_batch_id,new_job_id,dest_poid_db,batch_cnt,batch_cnt,old_batch_id;
				--dbms_output.put_line('Inserted one record in job_batch_t');
			END;
			BEGIN --This block is for inserting a record into destination.job_batch_accounts_t
				  --Copy the old record from source and paste it in destination
				  stmt_str:='insert into '||dest_schema_name||'.job_batch_accounts_t' || 
				  ' select * from job_batch_accounts_t  where obj_id0=:old_batch_id';
				  EXECUTE IMMEDIATE stmt_str using old_batch_id;	 
				--Change the batch id,rec_id,account_num,batch_num,job_id
				stmt_str:='update '||dest_schema_name||'.job_batch_accounts_t set obj_id0=:new_batch_id ,
				rec_id=0,account_num=1,batch_num=1,job_id=:new_job_id where obj_id0=:old_batch_id';
				EXECUTE IMMEDIATE stmt_str using new_batch_id,new_job_id,old_batch_id; 			
				--dbms_output.put_line('Inserted one record in job_batch_accounts_t');
			END;
			BEGIN --This block is for inserting a record into destination.job_rerate_t
				--Copy the old record from source and paste it in destination
				stmt_str:='insert into '||dest_schema_name||'.job_rerate_t select * from job_rerate_t 
				where obj_id0=:old_job_id';
				EXECUTE IMMEDIATE stmt_str using old_job_id;				
				--Change the job_id and status
				stmt_str:='update '||dest_schema_name||'.job_rerate_t set obj_id0=:new_job_id ,status=1 
				where obj_id0=:old_job_id';
				EXECUTE IMMEDIATE stmt_str using new_job_id,old_job_id;
				--dbms_output.put_line('Inserted one record in job_rerate_t');
			END;
			BEGIN --This block is for inserting a record into destination.job_batch_rerate_accounts_t
				 --Store the old record
				EXECUTE IMMEDIATE 'SELECT '||
				'job_batch_rerate_accts_t.rec_id, job_batch_rerate_accts_t.account_obj_db, '||
				'job_batch_rerate_accts_t.account_obj_id0, job_batch_rerate_accts_t.account_obj_type, '||
				'job_batch_rerate_accts_t.account_obj_rev, job_batch_rerate_accts_t.rerate_request_size '||
				'FROM job_batch_rerate_accts_t, job_batch_accounts_t '||
				'WHERE job_batch_accounts_t.obj_id0 = :old_batch_id '||
				'AND  job_batch_rerate_accts_t.obj_id0=job_batch_accounts_t.obj_id0 ' ||
				'AND job_batch_accounts_t.rec_id=job_batch_rerate_accts_t.rec_id'
				INTO 
				rec_id, acct_obj_db, acct_obj_id, acct_obj_type, acct_obj_rev, rerate_request_size 
				USING old_batch_id ; 

				stmt_str:='insert into '||dest_schema_name||'.job_batch_rerate_accts_t values' ||
					'(:new_batch_id, 0, 1, :acct_obj_db, :acct_obj_id, :acct_obj_type, 
					:acct_obj_rev, :rerate_request_size)';
				EXECUTE IMMEDIATE stmt_str using new_batch_id, acct_obj_db, acct_obj_id, acct_obj_type, 
					acct_obj_rev, rerate_request_size;			
				--dbms_output.put_line('Inserted one record in job_batch_rerate_accts_t');								
			END;
			BEGIN --This block is for insertin one record into rerate_request_buf_t
				--Before transfering this record let us update the buffer
				update_buffer(old_batch_id,rec_id,src_poid_db,dest_poid_db);
				stmt_str:='insert into '||dest_schema_name||'.rerate_request_buf_t' || 
				' select * from rerate_request_buf_t where  obj_id0=:old_batch_id and rec_id=:rec_id';
				EXECUTE IMMEDIATE stmt_str using old_batch_id,rec_id;
				stmt_str:='update '||dest_schema_name||'.rerate_request_buf_t' ||
				' set obj_id0=:new_batch_id,rec_id=0 where obj_id0=:old_batch_id and rec_id=:rec_id';
				EXECUTE IMMEDIATE stmt_str using new_batch_id,old_batch_id,rec_id;								
			END;
			BEGIN --This block is for deleting unwanted records from source
				--Delete from rerate_request_buf_t@source
				EXECUTE IMMEDIATE 'DELETE FROM rerate_request_buf_t '||
				'WHERE obj_id0=:old_batch_id AND rec_id=:rec_id' USING old_batch_id,rec_id;
				--Delete from job_batch_rerate_accounts_t@source
				EXECUTE IMMEDIATE 'DELETE FROM job_batch_rerate_accts_t '||
				'WHERE job_batch_rerate_accts_t.obj_id0=:old_batch_id '||
				'AND job_batch_rerate_accts_t.rec_id=:rec_id' USING old_batch_id,rec_id;
				--dbms_output.put_line('Deleted one record from job_batch_rerate_accounts_t in source');
				--Delete from job_batch_accounts_t@source
				EXECUTE IMMEDIATE 'DELETE FROM job_batch_accounts_t '||
				'WHERE job_batch_accounts_t.obj_id0=:old_batch_id '||
				'AND job_batch_accounts_t.rec_id=:rec_id' USING old_batch_id,rec_id;
				--dbms_output.put_line('Deleted one record from job_batch_accounts_t in source');
				--Check whether any more accounts are there for this batch
				EXECUTE IMMEDIATE 'SELECT count(*) FROM job_batch_accounts_t '||
				'WHERE job_batch_accounts_t.obj_id0=:old_batch_id' INTO cnt USING old_batch_id;
				--dbms_output.put_line('Remaining ' || cnt || ' accounts for this batch in source');
				--Delete from job_batch_t@source if necessary
				IF(cnt=0) THEN
				BEGIN
					EXECUTE IMMEDIATE 'DELEE FROM job_batch_t '||
					'WHERE job_batch_t.poid_id0=:old_batch_id '||
					'AND job_batch_t.job_obj_id0=:old_job_id' USING old_batch_id,old_job_id;
					--dbms_output.put_line('Deleting this batch from job_batch_t as it has no more accounts 
					--in  source');
					--Check whether any more batches are there for this job
					EXECUTE IMMEDIATE 'SELECT count(*) FROM job_batch_t '||
					'WHERE job_obj_id0=:old_job_id' INTO cnt USING old_job_id;
					--dbms_output.put_line('Remaining ' || cnt || ' batches for this job in source');
				END;
				ELSE
				BEGIN
					EXECUTE IMMEDIATE 'UPDATE job_batch_t SET account_cnt=account_cnt-1 '||
					'WHERE job_batch_t.poid_id0=:old_batch_id AND '||
					'job_batch_t.job_obj_id0=:old_job_id' USING old_batch_id,old_job_id;
					--dbms_output.put_line('Updating the account count for this batch as it has still' || 
					--' some more accounts in source');
				END;	
				END IF;
				--Delete from job_t@source if necessary
				IF(cnt=0) THEN
				BEGIN
					EXECUTE IMMEDIATE 'DELETE FROM job_t '||
					'WHERE poid_id0=:old_job_id' USING old_job_id;
					--dbms_output.put_line('Deleting  this job from job_t as it has no more batches in 
					--source');
					--Delete from job_rerate_t@source
					EXECUTE IMMEDIATE 'DELETE FROM job_rerate_t '||
					'WHERE job_rerate_t.obj_id0=:old_job_id' USING old_job_id;
					--dbms_output.put_line('Deleting  this job from job_rerate_t as it has no more batches 
					--in  source');
				END;
				ELSE
				BEGIN
					EXECUTE IMMEDIATE 'UPDATE job_t SET batch_cnt=batch_cnt-1 '||
					'WHERE poid_id0=:old_job_id' USING old_job_id;
					--dbms_output.put_line('Updating the batch count for this job as it has still' || 
					--' some more batches in source');
				END;	
				END IF;  
			END;
			log_message:=log_message ||  'created new job id ' || new_job_id 
					||' ,new batch id '|| new_batch_id || ' for account poid ' 
					|| account_poid || ' in destination' || ', as it is found in '
					|| 'rerate status PIN_RERATE_STATUS_NEW.'  || chr(10);	
		END LOOP;
		/*IF(cnt=-1) THEN
		BEGIN 
			--dbms_output.put_line('No such rerate batch:job for account poid ' || account_poid || ' found which ' 	
			--||'need to be recreated in destination');
			log_message:=log_message ||  'No new rerate job need to be created in destination for account poid ' || account_poid || chr(10);
		END;
		END IF;	*/                  
	END migrate_rerate_batch;

--This procedure will delete all the group batches under a job
PROCEDURE release_group
(job_id IN number)
AS
batch_id number;
batch_cnt number;
job_batch_cnt number;
DELTA number;
CURSOR fetch_batches_cur 
IS 
SELECT 
job_batch_amt_t.obj_id0,job_batch_amt_t.group_id 
FROM 
job_batch_amt_t,job_batch_t 
WHERE 
job_batch_amt_t.obj_id0=job_batch_t.poid_id0 AND job_batch_t.job_obj_id0=job_id AND job_batch_amt_t.group_id!=0;
BEGIN
	--dbms_output.put_line('Job Id :'|| job_id);	
	batch_cnt:=0;
	--Fetch all the group batches for this job
	FOR fetch_batches_cur_rec in fetch_batches_cur
	LOOP		
		batch_id:=fetch_batches_cur_rec.obj_id0;
		batch_cnt:=batch_cnt+1;
		--dbms_output.put_line('Batch Id: ' || batch_id);
		--Delete all the accounts for this batch
		EXECUTE IMMEDIATE 'DELETE FROM job_batch_accounts_t WHERE obj_id0=:batch_id AND '||
		'job_batch_accounts_t.job_id=:job_id' USING batch_id,job_id;
		--dbms_output.put_line('Deleted ' || SQL%rowcount ||' accounts for this batch from' || 				

		--'job_batch_accounts_t');
		--Delete the batch detail from job_batch_t
		EXECUTE IMMEDIATE 'DELETE FROM job_batch_t WHERE poid_id0=:batch_id AND job_obj_id0=:job_id'
		USING batch_id,job_id;
		--dbms_output.put_line('Deleted ' || SQL%rowcount ||' batches for this job from job_batch_t');
		--Delete the batch detail from job_batch_amt_t
		EXECUTE IMMEDIATE 'DELETE FROM job_batch_amt_t WHERE obj_id0=:batch_id AND '||
		'job_batch_amt_t.group_id=:grp_id' USING batch_id,fetch_batches_cur_rec.group_id;
		--dbms_output.put_line('Deleted ' || SQL%rowcount ||' batches for this job from '|| 				

		--'job_batch_amt_t');
	END LOOP;
	--Proceed to next only when some batches has been deleted
	IF(batch_cnt!=0) THEN
	BEGIN
		--Check if this job should be deleted
		EXECUTE IMMEDIATE 'SELECT batch_cnt FROM job_t WHERE job_t.poid_id0=:job_id'
		INTO job_batch_cnt USING job_id;
		--dbms_output.put_line('Total no. of batches deleted for this group:' || batch_cnt);
		--dbms_output.put_line('Total batch count for this job is:'||job_batch_cnt);
		--dbms_output.put_line('The new batch count will be:' || (job_batch_cnt-batch_cnt));
		delta:=job_batch_cnt-batch_cnt;
		IF(batch_cnt=job_batch_cnt) THEN
		BEGIN
			--dbms_output.put_line('Deleting this job as all the batches has been deleted');
			EXECUTE IMMEDIATE 'DELETE FROM job_t WHERE poid_id0=:job_id' USING job_id;
			EXECUTE IMMEDIATE 'DELETE FROM job_amt_t WHERE obj_id0=:job_id' USING job_id;
			
		END;
		ELSE
		BEGIN
			--dbms_output.put_line('Delta is' || delta);
			EXECUTE IMMEDIATE 'UPDATE job_t SET job_t.batch_cnt=:delta WHERE poid_id0=:job_id'
			USING delta,job_id;
			--dbms_output.put_line('Updated the batch count for this job as it still has some batches');
		END;
		END IF;
	END;
	END IF;
	
	--COMMIT;
END release_group;

--This procedure will update the rerate buffer.
--The source poid_db need to be changed to destination poid_db
PROCEDURE update_buffer
(batch_id IN number,
rec_id number,
source_db IN number,
dest_db IN number)
IS
buff blob;
position integer;
search_str varchar2(14);
replace_str varchar2(14); 
BEGIN
   	--Set the serach pattern
	search_str:='302E302E302E' || to_hex(source_db);
	replace_str:='302E302E302E' || to_hex(dest_db);
	--get the buffer into a variable
	SELECT 
	rerate_request_buf 
	INTO buff 
	FROM 
	rerate_request_buf_t 
	WHERE 
	obj_id0=batch_id AND rerate_request_buf_t.rec_id=rec_id
	FOR UPDATE;
	LOOP
   		position:=dbms_lob.instr(buff,search_str,1,1);
		--If there is no more pattern found then exit
		IF (position=0) THEN
			EXIT;
		END IF;	
		dbms_lob.write(buff,7,position,replace_str);
	END LOOP;
END update_buffer;
--This procedure converts a decimal number to internal hex format
FUNCTION to_hex
(dec_no number) 
RETURN 
varchar2
AS
hex_str varchar2(10);
ret_str varchar2(10);
ascii_no decimal(5,0);
reminder number;
flag number;
BEGIN
	hex_str:='';
	ret_str:='';
	ascii_no:=ascii(dec_no);
	flag:=0;
	LOOP
		reminder:=mod(ascii_no,16);
		IF(reminder=10 ) THEN
			hex_str:=hex_str || 'A';
			flag:=1;
		END IF;
		IF(reminder=11 ) THEN
			hex_str:=hex_str || 'B';
			flag:=1;
		END IF;
		IF(reminder=12 ) THEN
			hex_str:=hex_str || 'C';
			flag:=1;
		END IF;
		IF(reminder=13 ) THEN
			hex_str:=hex_str || 'D';
			flag:=1;
		END IF;
		IF(reminder=14 ) THEN
			hex_str:=hex_str || 'E';
			flag:=1;
		END IF;
		IF(reminder=15 ) THEN
			hex_str:=hex_str || 'F';
			flag:=1;
		END IF;
		IF(flag=0) THEN
			hex_str:=hex_str || reminder;
		END IF;
		ascii_no:=ascii_no/16;
		IF(ascii_no =0) THEN
			EXIT;
		END IF;
	END LOOP;
	FOR I IN REVERSE 1..length(hex_str)
	LOOP
		ret_str:=ret_str || substr(hex_str,i,1);
	END LOOP;
	RETURN ret_str;
END to_hex;
END  amt_rerate;
/

