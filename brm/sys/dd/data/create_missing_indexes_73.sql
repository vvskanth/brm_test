--
-- @(#)%Oracle Version: create_missing_indexes_73.sql:RWSmod7.3.1Int:1:2008-Dec-31 23:05:04 %
--
-- Copyright (c) 2007, 2009, Oracle and/or its affiliates.All rights reserved. 
--
-- This material is the confidential property of Oracle Corporation or its
-- licensors and may be used, reproduced, stored or transmitted only in
-- accordance with a valid Oracle license or sublicense agreement.
--
--
-- SQL script to create the missing index on Active session & Reservation tables.
--

DECLARE
        ddl_stmt VARCHAR2(4000);
        v_errmsg VARCHAR2(2000);
        v_index_creation_error CONSTANT NUMBER := -20001;
BEGIN

        -- Create unique index on ACTIVE_SESSION_RESV_LIST_T (obj_id0, rec_id).
        BEGIN

                IF (NOT pin_upg_common.index_exists('I_ACTSN_RESV_LIST__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('ACTIVE_SESSION_RESV_LIST_T', v_errmsg)) THEN

                                ddl_stmt := 'create unique index I_ACTSN_RESV_LIST__ID
                                        on ACTIVE_SESSION_RESV_LIST_T ( obj_id0, rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;

                        END IF;
                END IF;

        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_ACTSN_RESV_LIST__ID '||sqlerrm, TRUE);
        END;


        -- Create unique index on AS_TELCO_SVC_CODES_T (obj_id0, rec_id).
        BEGIN

                IF (NOT pin_upg_common.index_exists('I_AS_TELCO_SVC_CODES__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('AS_TELCO_SVC_CODES_T', v_errmsg)) THEN

                                ddl_stmt := 'create unique index I_AS_TELCO_SVC_CODES__ID
                                        on AS_TELCO_SVC_CODES_T ( obj_id0, rec_id)
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_AS_TELCO_SVC_CODES__ID '||sqlerrm, TRUE);
        END;

        -- Create unique index on ACTIVE_SESSION_TELCO_GPRS_T (obj_id0).

        BEGIN
                IF (NOT pin_upg_common.index_exists('I_ACTIVE_SESSION_TLCO_GPRS__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('ACTIVE_SESSION_TELCO_GPRS_T', v_errmsg)) THEN

                                ddl_stmt := 'create unique index I_ACTIVE_SESSION_TLCO_GPRS__ID
                                        on ACTIVE_SESSION_TELCO_GPRS_T ( obj_id0)
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_ACTIVE_SESSION_TLCO_GPRS__ID '||sqlerrm, TRUE);
        END;

        -- Create unique index on RESERVATION_RUM_MAP_T (obj_id0, rec_id).

        BEGIN
                IF (NOT pin_upg_common.index_exists('I_RESERVATION_RUM_MAP__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('RESERVATION_RUM_MAP_T', v_errmsg)) THEN

                                ddl_stmt := 'create unique index I_RESERVATION_RUM_MAP__ID
                                        on RESERVATION_RUM_MAP_T ( obj_id0, rec_id)
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_RESERVATION_RUM_MAP__ID '||sqlerrm, TRUE);
        END;

        -- Create unique index on PROC_AUD_T (created_t , process_name , version_id)
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_PROC_AUD_CREATED_TIME__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('PROC_AUD_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_PROC_AUD_CREATED_TIME__ID 
                    on PROC_AUD_T ( created_t , process_name , version_id)
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_PROC_AUD_CREATED_TIME__ID '||sqlerrm, TRUE);
        END;

        -- Create unique index on PROC_AUD_T (poid_id0)
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_PROC_AUD_POID__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('PROC_AUD_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_PROC_AUD_POID__ID 
                    on PROC_AUD_T ( poid_id0 )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_PROC_AUD_POID__ID '||sqlerrm, TRUE);
        END;

        -- Create unique index on PROC_AUD_T (Process_Start, Process_End)
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_PROC_AUD__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('PROC_AUD_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_PROC_AUD__ID 
                    on PROC_AUD_T ( PROCESS_START, PROCESS_END )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_PROC_AUD__ID '||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_T (obj_id0, suspended_from_batch_id, original_batch_id).
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_BATCH__ID 
                    on GDTLS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_BATCH__ID '||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_T (obj_id0, rec_id).
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS__ID 
                    on GDTLS_T ( obj_id0 , rec_id)
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS__ID '||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_STS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_STS_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_STS_BATCH__ID 
                    on GDTLS_STS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_STS_BATCH__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_STS_T ( obj_id0 , rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_STS__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_STS__ID 
                    on GDTLS_STS_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_STS__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_STYPE_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_STYPE_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STYPE_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_STYPE_BATCH__ID 
                    on GDTLS_STYPE_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_STYPE_BATCH__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_STYPE_T ( obj_id0 , rec_id)
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_STYPE__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STYPE_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_STYPE__ID 
                    on GDTLS_STYPE_T ( obj_id0 , rec_id)
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_STYPE__ID'||sqlerrm, TRUE);
        END;

    -- Create unique index on GDTLS_STYPE_STS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_STYPE_STS_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STYPE_STS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_STYPE_STS_BATCH__ID 
                    on GDTLS_STYPE_STS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_STYPE_STS_BATCH__ID'||sqlerrm, TRUE);
        END;

    -- Create unique index on GDTLS_STYPE_STS_T ( obj_id0 , rec_id)
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_STYPE_STS__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STYPE_STS_T', v_errmsg)) THEN
                                ddl_stmt := 'create index I_GDTLS_STYPE_STS__ID
                    on GDTLS_STYPE_STS_T ( obj_id0, rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_STYPE_STS__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_RSTRM_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_RSTRM_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_RSTRM_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_RSTRM_BATCH__ID 
                    on GDTLS_RSTRM_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_RSTRM_BATCH__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_RSTRM_T ( obj_id0 , rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_RSTRM__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_RSTRM_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_RSTRM__ID 
                    on GDTLS_RSTRM_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_RSTRM__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_RSTRM_STS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_RSTRM_STS_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_RSTRM_STS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_RSTRM_STS_BATCH__ID 
                on GDTLS_RSTRM_STS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_RSTRM_STS_BATCH__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_RSTRM_STS_T ( obj_id0 , rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_RSTRM_STS__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_RSTRM_STS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_RSTRM_STS__ID 
                on GDTLS_RSTRM_STS_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_RSTRM_STS__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_RSTRM_STYPE_STS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_RM_STYPE_STS_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_RSTRM_STYPE_STS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_RM_STYPE_STS_BATCH__ID 
                    on GDTLS_RSTRM_STYPE_STS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_RM_STYPE_STS_BATCH__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_RSTRM_STYPE_STS_T ( obj_id0 , rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_RSTRM_STYPE_STS__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_RSTRM_STYPE_STS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_RSTRM_STYPE_STS__ID 
                    on GDTLS_RSTRM_STYPE_STS_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_RSTRM_STYPE_STS__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_STYPE_RSTRM_STS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_STYPE_RM_STS_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STYPE_RSTRM_STS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_STYPE_RM_STS_BATCH__ID 
                    on GDTLS_STYPE_RSTRM_STS_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_STYPE_RM_STS_BATCH__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_STYPE_RSTRM_STS_T ( obj_id0 , rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_STYPE_RSTRM_STS__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STYPE_RSTRM_STS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_STYPE_RSTRM_STS__ID 
                    on GDTLS_STYPE_RSTRM_STS_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_STYPE_RSTRM_STS__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_OSTRM_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_OSTRM_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_OSTRM_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_OSTRM_BATCH__ID 
                    on GDTLS_OSTRM_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_OSTRM_BATCH__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_OSTRM_T ( obj_id0 , rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_OSTRM__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_OSTRM_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_OSTRM__ID 
                    on GDTLS_OSTRM_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_OSTRM__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_STYPE_RSTRM_OSTRM_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_ST_RM_OM_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STYPE_RSTRM_OSTRM_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_ST_RM_OM_BATCH__ID 
                    on GDTLS_STYPE_RSTRM_OSTRM_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_ST_RM_OM_BATCH__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_STYPE_RSTRM_OSTRM_T ( obj_id0 , rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_STYPE_RSTRM_OSTRM__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STYPE_RSTRM_OSTRM_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_STYPE_RSTRM_OSTRM__ID 
                    on GDTLS_STYPE_RSTRM_OSTRM_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_STYPE_RSTRM_OSTRM__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_SUSPENSE_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_SUSPENSE_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_SUSPENSE_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_SUSPENSE_BATCH__ID 
                    on GDTLS_SUSPENSE_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_SUSPENSE_BATCH__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_SUSPENSE_T ( obj_id0 , rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_SUSPENSE__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_SUSPENSE_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_SUSPENSE__ID 
                    on GDTLS_SUSPENSE_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_SUSPENSE__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_STYPE_RSTRM_STS_OSTRM_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_ST_RM_STS_OM_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STYPE_RSTRM_STS_OSTRM_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_ST_RM_STS_OM_BATCH__ID
                    on GDTLS_STYPE_RSTRM_STS_OSTRM_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_ST_RM_STS_OM_BATCH__ID'||sqlerrm,
 TRUE);
        END;

        -- Create unique index on GDTLS_STYPE_RSTRM_STS_OSTRM_T ( obj_id0 , rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_ST_RM_STS_OM__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_STYPE_RSTRM_STS_OSTRM_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_ST_RM_STS_OM__ID 
                    on GDTLS_STYPE_RSTRM_STS_OSTRM_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_ST_RM_STS_OM__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_RSTRM_STYPE_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_RSTRM_STYPE_BATCH__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_RSTRM_STYPE_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_RSTRM_STYPE_BATCH__ID 
                    on GDTLS_RSTRM_STYPE_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_RSTRM_STYPE_BATCH__ID'||sqlerrm, TRUE);
        END;

        -- Create unique index on GDTLS_RSTRM_STYPE_T ( obj_id0 , rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_GDTLS_RSTRM_STYPE__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('GDTLS_RSTRM_STYPE_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_GDTLS_RSTRM_STYPE__ID 
                    on GDTLS_RSTRM_STYPE_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'INDEX CREATION FAILED FOR I_GDTLS_RSTRM_STYPE__ID'||sqlerrm, TRUE);
        END;

        -- create unique index on GDTLS_STYPE_RSTRM_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
        begin
                if (not pin_upg_common.index_exists('I_GDTLS_STYPE_RSTRM_BATCH__ID', v_errmsg)) then
                        if (pin_upg_common.table_exists('GDTLS_STYPE_RSTRM_T', v_errmsg)) then

                                ddl_stmt := 'create index I_GDTLS_STYPE_RSTRM_BATCH__ID 
                    on GDTLS_STYPE_RSTRM_T ( obj_id0 , suspended_from_batch_id , original_batch_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                execute immediate ddl_stmt;
                        end if;
                end if;
        exception
                when others then
                        raise_application_error(v_index_creation_error, 'index creation failed for I_GDTLS_STYPE_RSTRM_BATCH__ID'||sqlerrm, true);
        end;

        -- create unique index on GDTLS_STYPE_RSTRM_T ( obj_id0 , rec_id )
        begin
                if (not pin_upg_common.index_exists('I_GDTLS_STYPE_RSTRM__ID', v_errmsg)) then
                        if (pin_upg_common.table_exists('GDTLS_STYPE_RSTRM_T', v_errmsg)) then

                                ddl_stmt := 'create index I_GDTLS_STYPE_RSTRM__ID 
                    on GDTLS_STYPE_RSTRM_T ( obj_id0 , rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                execute immediate ddl_stmt;
                        end if;
                end if;
        exception
                when others then
                        raise_application_error(v_index_creation_error, 'index creation failed for I_GDTLS_STYPE_RSTRM__ID'||sqlerrm, true);
        end;

        -- create unique index on PROC_AUD_BILL_SUPPRESS_T ( obj_id0 , rec_id2 )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_PROC_AUD_BILL_SUPPRESS__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('PROC_AUD_BILL_SUPPRESS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_PROC_AUD_BILL_SUPPRESS__ID 
                    on PROC_AUD_BILL_SUPPRESS_T ( obj_id0 , rec_id2 )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'index creation failed for I_PROC_AUD_BILL_SUPPRESS__ID'||sqlerrm, true);
        END;

        -- create unique index on PROC_AUD_BSTAT_T ( obj_id0 )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_PROC_AUD_BSTAT__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('PROC_AUD_BSTAT_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_PROC_AUD_BSTAT__ID 
                    on PROC_AUD_BSTAT_T ( obj_id0 )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'index creation failed for I_PROC_AUD_BSTAT__ID'||sqlerrm, true);
        END;

        -- create unique index on PROC_AUD_BILL_T ( obj_id0 )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_PROC_AUD_BILL__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('PROC_AUD_BILL_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_PROC_AUD_BILL__ID 
                    on PROC_AUD_BILL_T ( obj_id0 )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'index creation failed for I_PROC_AUD_BILL__ID'||sqlerrm, true);
        END;

        -- create unique index on PROC_AUD_BILL_SEGMENTS_T ( obj_id0, rec_id )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_PROC_AUD_BILL_SEGMENTS__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('PROC_AUD_BILL_SEGMENTS_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_PROC_AUD_BILL_SEGMENTS__ID 
                    on PROC_AUD_BILL_SEGMENTS_T ( obj_id0, rec_id )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'index creation failed for I_PROC_AUD_BILL_SEGMENTS__ID'||sqlerrm, true);
        END;

        -- create unique index on PROC_AUD_BILL_REVENUES_T ( obj_id0, rec_id2 )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_PROC_AUD_BILL_REVENUES__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('PROC_AUD_BILL_REVENUES_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_PROC_AUD_BILL_REVENUES__ID 
                    on PROC_AUD_BILL_REVENUES_T ( obj_id0, rec_id2 )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'index creation failed for I_PROC_AUD_BILL_REVENUES__ID'||sqlerrm, true);
        END;

        -- create unique index on PROC_AUD_BILL_ERR_ACCT_T ( obj_id0, rec_id2 )
        BEGIN
                IF (NOT pin_upg_common.index_exists('I_PROC_AUD_BILL_ERR_ACCT__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('PROC_AUD_BILL_ERR_ACCT_T', v_errmsg)) THEN

                                ddl_stmt := 'create index I_PROC_AUD_BILL_ERR_ACCT__ID 
                    on PROC_AUD_BILL_ERR_ACCT_T ( obj_id0, rec_id2 )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'index creation failed for I_PROC_AUD_BILL_ERR_ACCT__ID'||sqlerrm, true);
        END;

        -- Create index on AU_PURCHASED_PRODUCT_T (AU_PARENT_OBJ_ID0).
        BEGIN
                IF (NOT pin_upg_common.index_exists ('I_AU_PURCHASED_PROD_PARENT__ID', v_errmsg)) THEN
                        IF (pin_upg_common.table_exists('AU_PURCHASED_PRODUCT_T', v_errmsg)) THEN

                                ddl_stmt := 'CREATE INDEX I_AU_PURCHASED_PROD_PARENT__ID
                            ON AU_PURCHASED_PRODUCT_T ( AU_PARENT_OBJ_ID0 )
                                        $PIN_CONF_TBLSPACEX1
                                        $PIN_CONF_STORAGE_MED';
                                EXECUTE IMMEDIATE ddl_stmt;
                        END IF;
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                        RAISE_APPLICATION_ERROR(v_index_creation_error, 'index creation failed for I_AU_PURCHASED_PROD_PARENT__ID'||sqlerrm, true);
        END;

EXCEPTION
        WHEN others THEN
                RAISE_APPLICATION_ERROR(v_index_creation_error, 'ERROR ENCOUNTERED WHILE CREATING INDEX ' ||sqlerrm, TRUE);


END;

/

exit;
