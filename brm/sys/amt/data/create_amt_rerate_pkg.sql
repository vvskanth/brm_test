CREATE OR REPLACE PACKAGE amt_rerate 
IS
	PROCEDURE eliminate_rerate_gap
	(amt_job_id  NUMBER,
	dest_schema_name IN VARCHAR,
	log_message OUT CLOB);
END  amt_rerate ;
/
show errors;
