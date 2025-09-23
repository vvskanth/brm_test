--
--      @(#) $Id: create_grant_amt_permissions.sql /cgbubrm_7.5.0.portalbase/1 2015/11/27 04:21:29 nishahan Exp $
--
-- Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.
--
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--

create or replace procedure grant_amt_permissions(p_schema IN VARCHAR2)
        AS

        cursor proc_cur is
                SELECT unique OBJECT_NAME from USER_PROCEDURES WHERE OBJECT_TYPE in ('PACKAGE', 'PROCEDURE', 'FUNCTION');

        cursor tab_cur is
                SELECT distinct(substr(dof.sm_item_name,
                    decode(instr(dof.sm_item_name,'T='), 0, 1,instr(dof.sm_item_name,'T=')+2),
                    32)) a FROM dd_objects_t DO, dd_objects_fields_t DOF
                        WHERE DOF.obj_id0 = DO.obj_id0 AND DOF.field_type IN (6, 9, 10, 11, 16)
                UNION
                SELECT table_name a from user_tables where LOWER(table_name) in ('amt_account_batch_table_t',
                        'amt_base_detail_t','amt_poid_type_map_t','amt_metadata_t','migrated_objects_t','md_static_objects_t');

        proc_name       proc_cur%ROWTYPE;
        t_name          tab_cur%ROWTYPE;
        cid             int := 0;

        BEGIN
                FOR t_name in tab_cur
                LOOP
                        BEGIN
                                cid := DBMS_SQL.OPEN_CURSOR;
                                DBMS_SQL.PARSE (cid,
                                        'GRANT ALL ON ' ||
                                        t_name.a ||
                                        ' to ' ||
                                        p_schema ||
                                        ' with grant option ',
                                        dbms_sql.native);
                                DBMS_SQL.CLOSE_CURSOR(cid);
                        EXCEPTION
                                WHEN OTHERS THEN
                                        DBMS_SQL.CLOSE_CURSOR(cid);
                        END;
                END LOOP;

                FOR proc_name in proc_cur
                LOOP
                        BEGIN
                                cid := DBMS_SQL.OPEN_CURSOR;
                                DBMS_SQL.PARSE (cid,
                                        'GRANT EXECUTE ON ' ||
                                        proc_name.object_name ||
                                        ' to ' ||
                                        p_schema ||
                                        ' with grant option ',
                                        dbms_sql.native);
                                        DBMS_SQL.CLOSE_CURSOR(cid);
                        EXCEPTION
                        WHEN OTHERS THEN
                                DBMS_SQL.CLOSE_CURSOR(cid);
                        END;
                END LOOP;
        END;
/
