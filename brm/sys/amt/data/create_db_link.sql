--
--      @(#) % %
--     
--      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
--     
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--

SET serveroutput ON SIZE 1000000
CREATE OR REPLACE PROCEDURE create_db_link(p_username IN VARCHAR2,
					   p_password IN VARCHAR2,
					   p_host IN VARCHAR2,
					   p_db_link IN VARCHAR2) 
IS
   f_exists NUMBER;
BEGIN
   f_exists := 0;
   -- dbms_output.put_line('before test: f_exists = '||f_exists);
   BEGIN
     SELECT 1 INTO f_exists FROM user_db_links 
       WHERE db_link LIKE upper(p_db_link||'%') AND username = upper(p_username) AND host LIKE p_host||'%';
   EXCEPTION
     WHEN NO_DATA_FOUND THEN
       dbms_output.put_line('database link = '||p_db_link||' does not exist');
       --
       -- problem: 
       --
       -- creation of database link from within stored procedure fails with message:
       -- ORA-01031: insufficient privileges
       -- ORA-06512: at "PIN.CREATE_DB_LINK", line 20
       --
       -- solution:
       --
       -- creation of database link from within a stored procedure requires
       -- direct privilege assignment and not via a role :
       -- connect system/xyz
       -- GRANT CREATE DATABASE LINK TO pin
       --
       -- reference:
       -- Oracle Metalink Note:1048327.6 
       -- Subject:  ORA-1031 WHILE EXECUTING A STORED PROCEDURE 
       --
       f_exists := 0;
       --
       EXECUTE IMMEDIATE 'CREATE DATABASE LINK '||
	 p_db_link||' CONNECT TO '||p_username||
	 ' IDENTIFIED BY '||p_password||' USING '''||p_host||'''';
       --
       dbms_output.put_line('created database link = '||p_db_link);
   END;    
   IF(f_exists = 1) THEN
     dbms_output.put_line('database link = '||p_db_link||' already exists');
   END IF;   
END;
/
show errors;

