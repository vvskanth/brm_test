--
--      @(#) % %
--     
-- Copyright (c) 1996, 2015, Oracle and/or its affiliates. All rights reserved.
--     
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--

SET serveroutput ON SIZE 1000000
CREATE or REPLACE PROCEDURE generate_poid_type_map
IS

slash_posn PLS_INTEGER := 1;
str_len PLS_INTEGER := 0;
sub_event dd_objects_t.name%TYPE;
afctd_table_name dd_objects_fields_t.sm_item_name%TYPE;

/* Get all the event objects*/
CURSOR events_cur IS
SELECT name 
FROM dd_objects_t;
-- WHERE name like '/event%';

/* Query the data dictionary to get the related table information */
CURSOR afctd_table_cur(isub_event IN VARCHAR2) IS
SELECT sm_item_name , field_type
FROM dd_objects_t A, dd_objects_fields_t B
WHERE A.obj_id0 = B.obj_id0
AND A.name =isub_event
  AND B.field_type IN (6, 9, 10,11);
--
-- field_type's are defined in pcm.h as:
-- 6 = PIN_FLDT_BUF 
-- 9 = PIN_FLDT_ARRAY
-- 10 = PIN_FLDT_SUBSTRUCT
-- 11 = PIN_FLDT_OBJ 
--
i INT := 100;

BEGIN

    FOR event_rec IN events_cur LOOP

	IF  event_rec.name IS NOT NULL THEN
		str_len := LENGTH(event_rec.name);
dbms_output.put_line('Event type:: '||event_rec.name);
		slash_posn := 1;
		WHILE (slash_posn != 0) LOOP
			i := i + 1;
	    		slash_posn := INSTR(event_rec.name, '/', slash_posn+1, 1);
	    		IF slash_posn != 0 THEN
	    			sub_event := SUBSTR(event_rec.name, 1, slash_posn - 1);
	    		ELSE
				sub_event := event_rec.name;
	    		END IF;
			dbms_output.put_line('sub_event = '|| sub_event);
			FOR afctd_table_rec IN afctd_table_cur(sub_event) LOOP
			    afctd_table_name := afctd_table_rec.sm_item_name;
			    IF (afctd_table_rec.field_type = 6) THEN
				afctd_table_name := SUBSTR(afctd_table_rec.sm_item_name, INSTR(afctd_table_rec.sm_item_name, 'T=', 1,1) + 2);
			    END IF;

			    EXECUTE IMMEDIATE 'INSERT INTO amt_poid_type_map_t (poid_type, table_name) '||
			    'VALUES (:1, :2)' USING event_rec.name,afctd_table_name;

                            dbms_output.put_line(afctd_table_name);

			END LOOP;
		END LOOP;
	END IF;
    END LOOP;
END;
/
show errors;




