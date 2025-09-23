--
-- @(#)%Portal Version: update_realtime_batch_cntr.sql:OptMgrsVelocityInt:3:2006-Sep-15 21:29:13 %
--
-- Copyright (c) 2004, 2014, Oracle and/or its affiliates. All rights reserved.
--
--     This material is the confidential property of Oracle Corporation
--     or its licensors and may be used, reproduced, stored or transmitted 
--     only in accordance with a valid Oracle license or sublicense agreement.
--
--
--

CREATE OR REPLACE TYPE V_ARRAY AS VARRAY(100000) OF NUMBER
/

-- The procedure update_realtime_batch_cntr will update values of the column realtime_cntr
-- in bal_grp_t. max(recId) will be derived as maximum value of rec_id column in -
-- bal_grp_sub_bals_t. realtime_cntr will be derived as max(recId).


CREATE OR REPLACE PROCEDURE update_realtime_batch_cntr(
			    p_acct_poids  IN  V_ARRAY,
			    p_return_code OUT NUMBER,
			    p_return_mesg OUT VARCHAR2
			    )
AS
 
	TYPE v_num_arr_typ IS TABLE OF NUMBER INDEX BY BINARY_INTEGER;

	v_poid_id_arr 	   	v_num_arr_typ;
	v_realtime_cntr_arr 	v_num_arr_typ;

	v_rec_id NUMBER := 0;
BEGIN

 	-- Collect all the bg poids that belongs to this batch

	SELECT poid_id0,
		0
	BULK COLLECT INTO v_poid_id_arr,
			  v_realtime_cntr_arr
	FROM bal_grp_t
	WHERE account_obj_id0 IN ( SELECT * from  
 	table(p_acct_poids)) ;
 
	-- Loop through bg poids collection to update 
	-- realtime_cntr in bal_grp_t

	FOR i IN v_poid_id_arr.FIRST..v_poid_id_arr.LAST
        LOOP

		SELECT nvl(max(REC_ID), 0)
		INTO v_rec_id 
		FROM bal_grp_sub_bals_t
		WHERE obj_id0 = v_poid_id_arr(i) ;

		v_realtime_cntr_arr(i) := v_rec_id;
 
	END LOOP;
	   
        FORALL i IN v_poid_id_arr.first .. v_poid_id_arr.last
        UPDATE bal_grp_t
        SET realtime_cntr = v_realtime_cntr_arr(i)
	WHERE poid_id0 = v_poid_id_arr(i);

	   
	-- check for the exception
	-- If yes, then set the return code and return mesg

	EXCEPTION
	WHEN OTHERS THEN
        	p_return_code := 1;
		p_return_mesg := 'Exception has occured while updating '
	          		     ||'realtime_cntr, batch_cntr in bal_grp_t'
		                     || SQLERRM;

END update_realtime_batch_cntr;
/

