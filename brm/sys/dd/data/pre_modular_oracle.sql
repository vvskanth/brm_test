--
-- @(#) %full_filespec: pre_modular_oracle.sql~2:sql:1 %
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
-- We will be creating some portal objects.
-- So bring down the obj_ids sequence to portal range.
--


DECLARE
        max_portal_obj_id0 INT;
	max_obj_id0_objs INT;
        max_obj_id0_flds INT;
        v_cid INTEGER;

        obj_ids_seq_value INT;

BEGIN
        /*
         * First let us create temporary sequence to store the current value of obj_ids sequence.
         */

        SELECT obj_ids.nextval INTO obj_ids_seq_value FROM sys.dual;

        v_cid := DBMS_SQL.OPEN_CURSOR;
        DBMS_SQL.PARSE(v_cid, 'CREATE sequence upg_temp_seq START WITH ' || to_char(obj_ids_seq_value),
                                dbms_sql.native);


        /*
         * What is the maximum obj_id0 we have used?
         */

        SELECT max(obj_id0) into max_obj_id0_objs FROM dd_objects_t WHERE obj_id0 < 100000;
        SELECT max(obj_id0) into max_obj_id0_flds FROM dd_objects_fields_t WHERE obj_id0 < 100000;


        IF (max_obj_id0_objs > max_obj_id0_flds) THEN
                max_portal_obj_id0 := max_obj_id0_objs;
        ELSE
                max_portal_obj_id0 := max_obj_id0_flds;
        END IF;

        /*
         * Additional error check..
         * If this value is less than the known 601 upper range (1480), use the 601 upper range.
         */

        IF (max_portal_obj_id0 < 1480) THEN
                max_portal_obj_id0 := 1480;
        END IF;

        v_cid := DBMS_SQL.OPEN_CURSOR;

        DBMS_SQL.PARSE(v_cid, 'DROP sequence obj_ids', dbms_sql.native);
        DBMS_SQL.PARSE(v_cid, 'CREATE sequence obj_ids START WITH ' || to_char(max_portal_obj_id0 + 1), dbms_sql.native);
        DBMS_SQL.CLOSE_CURSOR (v_cid);
EXCEPTION
        WHEN others THEN
                IF dbms_sql.is_open(v_cid) THEN
                        dbms_sql.close_cursor(v_cid);
                END IF;
                dbms_output.put_line(sqlerrm);
END;
/
