--
--      @(#)$Id: create_amt_mv_pkg.sql /cgbubrm_mainbrm.portalbase/1 2016/12/14 02:20:20 madbiswa Exp $
--
--      Copyright (c) 2022, Oracle and/or its affiliates.
--     
--	This material is the confidential property of Oracle Corporation or its
--	licensors and may be used, reproduced, stored or transmitted only in
--	accordance with a valid Oracle license or sublicense agreement.
--

CREATE OR REPLACE PACKAGE amt_mv
IS
   -- procedure: lock_base_tables 
   --   Lock the base table records in the source database for all the accounts contained 
   --   in the AMM job batch. Accounts contained in AMM job batch are defined in table 
   --   AMT_ACCOUNT_DETAIL_T.
   -- parameters: 
   --   IN job_id                - AMT job id
   --   IN batch_num             - AMT batch number
   --   IN src_schema_name       - MultiSchema source schema qualifier
   --   IN log_file_flag         - 'Y': write to log file, 
   --                              'N': write stdout
   --   IN log_file_dir          - absolute path of log file directory; only used in combination
   --                              with log_file_flag set to 'Y'
   -- note: 
   --   log_file_dir needs to be enabled in init.ora (parameter: utl_file_dir).
   --
   PROCEDURE lock_base_tables(job_id IN NUMBER,
			      batch_id IN NUMBER,
			      src_schema_name IN VARCHAR2,
                              primary_schema_name IN VARCHAR2,
			      log_file_flag IN CHAR,
			      log_file_dir IN VARCHAR2);
   --
   -- procedure: migrate_account_batch
   --   Migrate batch of Infranet accounts from source database
   --   to target database. Accounts contained in account batch
   --   are defined in table AMT_ACCOUNT_DETAIL_T.
   -- parameters: 
   --   IN job_id                - AMT job id
   --   IN batch_num             - AMT batch number
   --   IN src_schema_name       - MultiSchema source schema qualifier
   --   IN log_file_flag         - 'Y': write to log file, 
   --                              'N': write stdout
   --   IN log_file_dir          - absolute path of log file directory; only used in combination
   --                              with log_file_flag set to 'Y'
   -- note: 
   --   log_file_dir needs to be enabled in init.ora (parameter: utl_file_dir).
   --
   PROCEDURE migrate_account_batch(job_id IN NUMBER,
		                   batch_id IN NUMBER,
				   src_schema_name IN VARCHAR2,
                                   primary_schema_name IN VARCHAR2,
				   log_file_flag IN CHAR,
				   log_file_dir IN VARCHAR2);
   --
   -- procedure: compare_src_dest
   --   Generate comparison report of data in source and destination
   --   database instances. Comparison reports can be produced for 
   --   individual accounts or all the accounts included in a particular
   --   job_id. 
   --
   -- return values:
   --   The following p_status codes are returned:
   -- 
   --   0: no error
   --   1: mismatch between record counts in source and destination table
   --
   -- parameters:
   --   IN  job_id                - AMT job id 
   --   IN  batch_id              - AMT batch_id
   --   IN  src_schema_name       - MultiSchema source schema qualifier
   --   OUT p_status		  - status code
   --
   PROCEDURE compare_src_dest(job_id IN NUMBER,
			      batch_id IN NUMBER,
   		              src_schema_name IN VARCHAR2,
			      p_status OUT NUMBER);
   --
   -- procedure: delete_account_batch
   --   Delete data from source MultiDB instance after accounts have 
   --   been migrated and POID_DB has been set to NULL.
   -- parameters:
   --   IN job_id                - AMT job id 
   --   IN batch_id              - AMT batch_id
   --   IN log_file_flag         - 'Y': write to log file, 
   --                              'N': write stdout
   --   IN log_file_dir          - absolute path of log file directory; only used in combination
   --                              with log_file_flag set to 'Y'
   PROCEDURE delete_account_batch(job_id NUMBER,
	 			  batch_id NUMBER,
				  primary_schema_name IN VARCHAR2,
				  log_file_flag IN CHAR,
				  log_file_dir IN VARCHAR2);

   --
   -- A PL/SQL TYPE which acts as Hashtable.
   --
   TYPE VAR_ARRAY IS TABLE OF VARCHAR2(500) INDEX BY BINARY_INTEGER;

   -- procedure: fetch_timos_objects
   --   Fetch all the objects from the source database that are residing in TIMOS and belongs to
   --   the accounts contained in the AMM job batch. Accounts contained in the AMM job batch are 
   --   defined in table AMT_ACCOUNT_DETAIL_T.
   -- parameters: 
   --   IN job_id                - AMT job id
   --   IN batch_num             - AMT batch number
   --   IN group_id		 - AMT group id
   --   IN batch_type		 - 'S': stand-alone batch, 
   --                              'G': group batch
   --   IN src_schema_name       - MultiSchema source schema qualifier
   --   IN log_file_flag         - 'Y': write to log file, 
   --                              'N': write stdout
   --   IN log_file_dir          - absolute path of log file directory; only used in combination
   --                              with log_file_flag set to 'Y'
   --   OUT timos_objects 	 - Collection of all the objects that are residing in TIMOS and belongs
   --				   to the accounts contained in the AMM job batch
   -- note: 
   --   log_file_dir needs to be enabled in init.ora (parameter: utl_file_dir).
   --
   PROCEDURE fetch_timos_objects(job_id IN NUMBER,
				 batch_id IN NUMBER,
				 group_id IN NUMBER,
				 batch_type IN CHAR,
   		              	 src_schema_name IN VARCHAR2,
                                 primary_schema_name IN VARCHAR2,
				 log_file_flag IN CHAR,
				 log_file_dir IN VARCHAR2,
				 timos_objects OUT VAR_ARRAY);

   --
   -- procedure: update_xref_table 
   --   Update the cross reference table "migrated_objects_t" for AIA Comms used case.
   --   The table will be populated with the old and new poid values for the migrated objects.
   --   This table resides in the primary schema.
   -- parameters: 
   --   IN amt_job_id            - AMT job id
   --   IN amt_batch_id          - AMT batch number
   --   IN class_list            - List of clases from the Infranet.properties file
   --   IN src_schema_name       - MultiSchema source schema qualifier
   --   IN dest_schema_name      - MultiSchema source schema qualifier
   --   IN log_file_flag         - 'Y': write to log file, 
   --                              'N': write stdout
   --   IN log_file_dir          - absolute path of log file directory; only used in combination
   --                              with log_file_flag set to 'Y'
   -- note: 
   --   log_file_dir needs to be enabled in init.ora (parameter: utl_file_dir).
   --
   PROCEDURE update_xref_table(amt_job_id IN NUMBER,
			       amt_batch_id IN NUMBER,
			       class_list IN VARCHAR2,
			       src_schema_name IN VARCHAR2,
			       dest_schema_name IN VARCHAR2,
			       log_file_flag IN CHAR,
			       log_file_dir IN VARCHAR2);

END;
/
show errors;

