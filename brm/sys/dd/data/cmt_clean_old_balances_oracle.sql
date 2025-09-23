--
-- @(#) cmt_clean_old_balances.sql 1
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

-- The procedure cmt_clean_old_balances will delete the old balances of all the accounts
-- present in the balances XML. It takes the collection of account poids existing in the 
-- balances XML as input.

CREATE OR REPLACE PROCEDURE cmt_clean_old_balances (
                                p_bg_poids  IN  V_ARRAY,
                                p_max_rec_id  IN  V_ARRAY,
                                p_delete_bal IN VARCHAR2,
                                p_return_code OUT NUMBER,
                                p_return_mesg OUT VARCHAR2
                                )
AS
	v_start_recid NUMBER;
	v_index NUMBER;
BEGIN

        -- check whether the given bg poids list is empty or not
        -- If it is not empty, then delete all the rows from bal_grp_bals_t matching the
        -- given balance group poids

IF p_delete_bal = 'true'
  THEN
        IF p_bg_poids.COUNT > 0
        THEN
          FORALL cnt IN p_bg_poids.FIRST..p_bg_poids.LAST
            DELETE FROM bal_grp_bals_t bg WHERE bg.obj_id0 = p_bg_poids(cnt);
        END IF;
END IF;

        -- delete all the rows from bal_grp_sub_bals_t matching
        -- every bal_grp poid_id existing in the list

IF p_delete_bal = 'true'
  THEN
        IF p_bg_poids.COUNT > 0
        THEN
          FORALL cnt IN p_bg_poids.FIRST..p_bg_poids.LAST
            DELETE FROM bal_grp_sub_bals_t bgsb WHERE bgsb.obj_id0 = p_bg_poids(cnt);
        END IF;
ELSE
        IF p_bg_poids.COUNT > 0
        THEN
          v_index := p_bg_poids.FIRST ;
          WHILE v_index IS NOT NULL LOOP
            SELECT MIN(rec_id) INTO v_start_recid from bal_grp_sub_bals_t bgsb WHERE bgsb.obj_id0 = p_bg_poids(v_index);
            IF v_start_recid = 0 THEN
              update bal_grp_sub_bals_t bgsb set bgsb.rec_id = bgsb.rec_id + p_max_rec_id(v_index) + 2 WHERE bgsb.obj_id0 = p_bg_poids(v_index);
            ELSE
              update bal_grp_sub_bals_t bgsb set bgsb.rec_id = bgsb.rec_id + p_max_rec_id(v_index) WHERE bgsb.obj_id0 = p_bg_poids(v_index);
            END IF;
            v_index := p_bg_poids.NEXT(v_index);
          END LOOP;
        END IF;
END IF;

        -- check for the exception
        -- If yes, then set the return code and return mesg

        EXCEPTION
        WHEN OTHERS THEN
                p_return_code := 1;
                p_return_mesg := 'Exception has occured while cleaning the old balances'|| SQLERRM;

END cmt_clean_old_balances;
/
