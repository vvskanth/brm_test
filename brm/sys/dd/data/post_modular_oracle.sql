--
-- @(#) %full_filespec: post_modular_oracle.sql~1:sql:1 %
--
-- Copyright (c) 2000, 2009, Oracle and/or its affiliates. 
-- All rights reserved. 
-- 
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--
--
-- sql file for Modular Products
--


set serveroutput on size 1000000


-- Restore the obj_ids sequence.

DECLARE
        v_cid INTEGER;
        obj_ids_seq_value INT;

BEGIN
      
        /*
         * We have saved the obj_ids sequence in upg_temp_seq.
         * use it now.
         */

        SELECT upg_temp_seq.nextval INTO obj_ids_seq_value FROM sys.dual;

        v_cid := DBMS_SQL.OPEN_CURSOR;

        DBMS_SQL.PARSE(v_cid, 'DROP sequence obj_ids', dbms_sql.native);

        DBMS_SQL.PARSE(v_cid, 'CREATE sequence obj_ids START WITH ' || to_char(obj_ids_seq_value), dbms_sql.native);

        /*
         * upg_temp_seq is no longer needed.
         */

        DBMS_SQL.PARSE(v_cid, 'DROP sequence upg_temp_seq', dbms_sql.native);

        DBMS_SQL.CLOSE_CURSOR (v_cid);
EXCEPTION
        WHEN others THEN
                IF dbms_sql.is_open(v_cid) THEN
                        dbms_sql.close_cursor(v_cid);
                END IF;
                dbms_output.put_line(sqlerrm);
END;
/
