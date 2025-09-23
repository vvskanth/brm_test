--
-- @(#) update_due_amount.sql 4
--
--     Copyright (c) 2004-2007 Oracle. All rights reserved.
--
--     This material is the confidential property of Oracle Corporation
--     or its licensors and may be used, reproduced, stored or transmitted 
--     only in accordance with a valid Oracle license or sublicense agreement.
--
--
--

CREATE OR REPLACE TYPE VAR_ARRAY AS VARRAY(1000000) OF VARCHAR2(500)
/

-- The procedure update_due_amount will update the due amd item_total columns in item_t 
-- with the given due amount for all the currency resources present in the balances XML.

CREATE OR REPLACE PROCEDURE update_due_amount(
				p_due IN VAR_ARRAY,
                                p_return_code OUT NUMBER,
                                p_return_mesg OUT VARCHAR2
                                )
AS
	v_first_occurence NUMBER;
	v_second_occurence NUMBER;
	v_diff NUMBER;
	v_cnt NUMBER;
	v_item_obj_id INTEGER;
	v_entry	VARCHAR2(500);
	v_due VARCHAR2(500);
	v_bal_grp_obj_id VARCHAR2(500);
	v_service_obj_id VARCHAR2(500);
	v_sql_stmt VARCHAR2(500);
BEGIN

	FOR cnt IN p_due.FIRST..p_due.LAST
	LOOP	
		v_entry := p_due(cnt);
		v_first_occurence := instr(v_entry, '#', 1, 1);
		v_second_occurence := instr(v_entry, '#', 1, 2);
		v_diff := v_second_occurence - v_first_occurence;

		v_due := substr(v_entry, 1, v_first_occurence - 1);
                v_bal_grp_obj_id := substr(v_entry, v_first_occurence + 1, v_diff - 1);
                v_service_obj_id := substr(v_entry, v_second_occurence + 1);

		SELECT count(*) INTO v_cnt
		FROM item_t
		WHERE bal_grp_obj_id0=v_bal_grp_obj_id 
		AND service_obj_id0=v_service_obj_id;

		IF (v_cnt > 0) THEN
			SELECT min(poid_id0) INTO v_item_obj_id
			FROM item_t
			WHERE bal_grp_obj_id0=v_bal_grp_obj_id
			AND service_obj_id0=v_service_obj_id;

			v_sql_stmt := 'UPDATE item_t SET due='||v_due||', item_total='||v_due
				||' WHERE bal_grp_obj_id0='||v_bal_grp_obj_id
				||' AND service_obj_id0='||v_service_obj_id
				||' AND poid_id0='||v_item_obj_id;
			EXECUTE IMMEDIATE v_sql_stmt;
		END IF;
	END LOOP;

        -- check for the exception
        -- If yes, then set the return code and return mesg

        EXCEPTION
        WHEN OTHERS THEN
                p_return_code := 1;
                p_return_mesg := 'Exception has occured while updating the item '|| SQLERRM;

END update_due_amount;
/
