--
--      @(#) % %
--     
-- Copyright (c) 1996, 2015, Oracle and/or its affiliates. All rights reserved.
--     
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--

SET serveroutput ON SIZE 1000000;

create or replace
PACKAGE BODY amt_group_search
IS

  --
  --
  -- open log file
  --
  PROCEDURE log_open (p_log_file_flag IN CHAR,
                      p_log_file_dir IN VARCHAR2,
                      p_log_file_name IN VARCHAR2,
                      p_log_file OUT utl_file.file_type)
  IS
  BEGIN
    IF (p_log_file_flag = 'Y') THEN
      p_log_file := utl_file.fopen(p_log_file_dir,
                                   p_log_file_name,
                                   'w',
                                   5000); 
                       -- indiv. message can be as long as 5000 bytes
    END IF;
  END;

  --
  -- close log file
  --
  PROCEDURE log_close (p_log_file_flag IN CHAR,
                       p_log_file      IN OUT utl_file.file_type)
  IS
  BEGIN
    IF (p_log_file_flag = 'Y') THEN
      IF (utl_file.is_open(p_log_file)) THEN
          utl_file.fflush(p_log_file);
          utl_file.fclose(p_log_file);
      END IF;
    END IF;
  END;
  --
  -- write message to log file
  --
  PROCEDURE log_mesg(p_log_file_flag IN CHAR,
                     p_log_file IN utl_file.file_type,
                     p_mesg IN VARCHAR2)
  IS
  BEGIN
    IF (p_log_file_flag = 'Y') THEN
      utl_file.put_line(p_log_file, 
                        to_char(sysdate, 'MM/DD/YY HH24:MM:SS')||' '||p_mesg);
      utl_file.fflush(p_log_file);
    ELSE
      dbms_output.put_line(to_char(sysdate, 'MM/DD/YY HH24:MM:SS')||' '||p_mesg);
    END IF;
  END;

  -- 
  -- is the member already included in the account groups
  -- 
  FUNCTION is_account_member_included(
     v_poid_id0                IN INTEGER,
     v_account_group_members   IN OUT GROUPACCOUNTLIST,
      log_file_flag            IN CHAR,
      log_file                 IN utl_file.file_type)
  RETURN BOOLEAN
  AS
      v_result   BOOLEAN;
      v_index    INTEGER;
  BEGIN
      v_index := v_account_group_members.FIRST;
      WHILE v_index IS NOT NULL
      LOOP
          IF (v_account_group_members(v_index) = v_poid_id0) THEN
              log_mesg(log_file_flag,
                       log_file,
                       '/account ' || v_poid_id0 || 
                       ' already included as member');
              RETURN TRUE;
          ELSE
              v_index := v_account_group_members.NEXT(v_index);
          END IF;
      END LOOP;
      RETURN FALSE;
  END;


  -- 
  -- traverse the account group for the specified account.
  -- 
  FUNCTION traverse_account_group(
      v_poid_id0                IN INTEGER,
      v_max_group_size          IN INTEGER,
      v_billinfo_group_members   IN OUT GROUPACCOUNTLIST,
      log_file_flag             IN CHAR,
      log_file                  IN utl_file.file_type)
  RETURN BOOLEAN
  IS
  
  	CURSOR billinfosearch ( v_account_poid IN billinfo_t.account_obj_id0%TYPE) is 
		select account_obj_id0,poid_id0, ar_billinfo_obj_id0, parent_billinfo_obj_id0 from billinfo_t where account_obj_id0 = v_account_poid AND poid_db is not null;
	CURSOR acctlistcurr ( v_billinfo_poid IN billinfo_t.poid_id0%TYPE, v_ar_billinfo_poid IN billinfo_t.poid_id0%TYPE, v_parent_billinfo_poid IN billinfo_t.poid_id0%TYPE) is 
		select account_obj_id0 from billinfo_t where poid_id0 in ( v_ar_billinfo_poid, v_parent_billinfo_poid) or ar_billinfo_obj_id0 in ( v_billinfo_poid, v_parent_billinfo_poid) or parent_billinfo_obj_id0 in ( v_ar_billinfo_poid, v_billinfo_poid) AND poid_db is not null;
		
 	ACCT_COUNT INTEGER :=1;
	ACCT_INDEX INTEGER :=0;
	x INTEGER :=1;
	acct_poid NUMBER(38); 
	accountflag INTEGER := 1;

	BEGIN
    ACCT_INDEX := v_billinfo_group_members.count;
		v_billinfo_group_members.extend(1);
		v_billinfo_group_members(v_billinfo_group_members.count) := v_poid_id0;
    -- ACC_COUNT shall be 
    ACCT_COUNT := ACCT_INDEX + 1;
	
 			while (ACCT_INDEX < ACCT_COUNT) LOOP
  			ACCT_INDEX := ACCT_INDEX+1;
       
  			FOR j in billinfosearch(v_billinfo_group_members(ACCT_INDEX)) loop
	
      			For k in acctlistcurr(j.poid_id0, j.ar_billinfo_obj_id0,j.parent_billinfo_obj_id0) loop
        			x:=1;
        			accountflag := 1;
      			-- Add condition to add account poid
     				acct_poid := k.account_obj_id0;
      				WHILE (x <= v_billinfo_group_members.count) loop
        				if(v_billinfo_group_members(x)=acct_poid) then
          				accountflag :=0;
        				end IF;
          				x :=x+1;
      				end LOOP;
      				if(accountflag != 0) then 
        				ACCT_COUNT := ACCT_COUNT + 1;
        				v_billinfo_group_members.extend(1);
        				v_billinfo_group_members(ACCT_COUNT) := acct_poid;
       				end if; 
      		END Loop;
 		 END Loop;
  END Loop;

    
    IF(billinfosearch%ISOPEN = TRUE) THEN
      CLOSE billinfosearch;
    END IF;
    IF(acctlistcurr%ISOPEN = TRUE) THEN
      CLOSE acctlistcurr;
    END IF;
    RETURN TRUE;
END;

  -- traverse the account group for the specified account.
  -- 
  -- traverse the account group for the specified account.
  -- 
  FUNCTION traverse_other_account_group(
      v_poid_id0                IN INTEGER,
      v_max_group_size          IN INTEGER,
      v_account_group_members   IN OUT GROUPACCOUNTLIST,
      v_billinfo_group_members  IN OUT GROUPACCOUNTLIST,
      v_number_of_members       IN OUT INTEGER,
      log_file_flag             IN CHAR,
      log_file                  IN utl_file.file_type)
  RETURN BOOLEAN
  IS

  CURSOR cur_Sponsoree(v_sponsorer_poid IN INTEGER)
  IS
  SELECT gsmt.object_id0 FROM group_sponsor_members_t gsmt 
         WHERE gsmt.obj_id0 =
                       (SELECT gt.poid_id0 FROM group_t gt 
                        WHERE gt.parent_id0 = v_sponsorer_poid
                        AND poid_type='/group/sponsor');

  CURSOR cur_Sponsorer(v_sponsoree_poid IN INTEGER)
  IS
  SELECT gt.parent_id0 FROM group_t gt WHERE gt.poid_id0 IN
         (SELECT gsmt.obj_id0 FROM group_sponsor_members_t gsmt 
          WHERE gsmt.object_id0 = v_sponsoree_poid);

  CURSOR cur_GroupShOwnerGroup(v_owner_poid IN INTEGER)
  IS
  SELECT t.poid_id0 FROM group_t t
                  WHERE t.account_obj_id0 = v_owner_poid 
                  AND t.poid_type LIKE '/group/sharing%';

  CURSOR cur_GroupShMembers(v_group_poid IN INTEGER)
  IS
  SELECT account_obj_id0 FROM group_sharing_members_t 
                         WHERE obj_id0 = v_group_poid;

  CURSOR cur_GroupShMemberGroup(v_member_poid IN INTEGER)
  IS
  SELECT obj_id0 FROM group_sharing_members_t
                 WHERE account_obj_id0 = v_member_poid;

  v_result       BOOLEAN;
  v_parent_poid  INTEGER;
  v_child_poid   INTEGER;
  v_tmp_poid     INTEGER;
  v_group_poid   INTEGER;
  i INTEGER;

  
  rec_Sponsoree            cur_Sponsoree%ROWTYPE;
  rec_Sponsorer            cur_Sponsorer%ROWTYPE;
  rec_GroupShOwnerGroup    cur_GroupShOwnerGroup%ROWTYPE;
  rec_GroupShMembers       cur_GroupShMembers%ROWTYPE;
  rec_GroupShMemberGroup   cur_GroupShMemberGroup%ROWTYPE;

  BEGIN
    -- THE ACCOUNT GROUP SIZE EXCEED MAXIMUM LIMIT,
    -- ACCOUNT GROUP MEMBERS SHOULD NOT BE RETURNED.
    IF (v_number_of_members > v_max_group_size) THEN
        log_mesg(log_file_flag, log_file, 
                 'traverse_account_group: WARNING: TRAVERSAL TERMINATED. Group size '
                 ||v_number_of_members|| ' found greater than max_group_size '
                 || v_max_group_size);
      -- EMPTY THE ACCOUNT GROUP MEMBER LIST AND RETURN FALSE.
      v_account_group_members.DELETE();
      v_number_of_members := 0;
      RETURN FALSE;
    END IF;
    -- IF THE ACCOUNT IS NOT ALREADY IN ACCOUNT GROUP MEMBER LIST, ADD THE ACCOUNT.
    IF NOT (is_account_member_included(v_poid_id0,
                                       v_account_group_members,
                                       log_file_flag,
                                       log_file)) THEN
      v_account_group_members.EXTEND(1);
      v_number_of_members := v_number_of_members + 1;
      v_account_group_members(v_number_of_members) := v_poid_id0;
    ELSE
      RETURN TRUE;
    END IF;

    -- PARENT/CHILD BILLINFO MEMBERS
	BEGIN
	    -- SKIP IF ALREADY IN BILLINFO_GROUP_MEMBER
		IF NOT (is_account_member_included(v_poid_id0,
									v_billinfo_group_members,
									log_file_flag,
									log_file)) THEN
      i := v_billinfo_group_members.count;
			v_result := traverse_account_group(v_poid_id0,
											v_max_group_size,
											v_billinfo_group_members,
											log_file_flag,
											log_file);
			-- COPY BILLINFO
      dbms_output.put_line('this is a test');
      dbms_output.put_line(' value of count right now is' || v_billinfo_group_members.count);
      i := i+1;
  			FOR j in i..v_billinfo_group_members.count LOOP
        dbms_output.put_line(' Value of J is ' || j);
				v_result := traverse_other_account_group(v_billinfo_group_members(j),
											v_max_group_size,
											v_account_group_members,
											v_billinfo_group_members,
											v_number_of_members,
											log_file_flag,
											log_file);
			END LOOP;
		END IF;
    END;	
		
    -- IF THE ACCOUNT IS A SPONSORER.
    BEGIN
      EXECUTE IMMEDIATE 'SELECT parent_id0 FROM group_t WHERE parent_id0 = :v_poid_id0 AND poid_type=''/group/sponsor'''
	INTO v_parent_poid USING v_poid_id0;
      log_mesg(log_file_flag, log_file, 
               'traverse_account_group: /account '
               ||v_poid_id0||' found as Sponsorer.');
      OPEN cur_Sponsoree(v_parent_poid);
      LOOP
        FETCH cur_Sponsoree INTO rec_Sponsoree;
        EXIT WHEN cur_Sponsoree%NOTFOUND;
        v_result := traverse_other_account_group(rec_Sponsoree.object_id0,
                                           v_max_group_size,
                                           v_account_group_members,
										   v_billinfo_group_members,
                                           v_number_of_members,
                                           log_file_flag,
                                           log_file);
        IF (v_result = FALSE) THEN
          -- THE LIST OF ACCOUNT MEMBERS IS ALREADY CLEANED. JUST RETURN FALSE.
          CLOSE cur_Sponsoree;
          RETURN FALSE;
        END IF;
      END LOOP;
      CLOSE cur_Sponsoree;
    EXCEPTION
      WHEN NO_DATA_FOUND THEN
        IF(cur_Sponsoree%ISOPEN = TRUE) THEN
          CLOSE cur_Sponsoree;
        END IF;
      WHEN TOO_MANY_ROWS THEN
        log_mesg(log_file_flag, log_file, 
                 'traverse_account_group: ERROR: TRAVERSAL TERMINATED. Multiple rows not expected for Sponsorer /account ' || v_poid_id0);
        IF(cur_Sponsoree%ISOPEN = TRUE) THEN
          CLOSE cur_Sponsoree;
        END IF;
        v_account_group_members.DELETE();
        v_number_of_members := 0;
        RETURN FALSE;
    END;

    -- IF THE ACCOUNT IS A SPONSOREE.
    BEGIN
      EXECUTE IMMEDIATE 'SELECT DISTINCT(object_id0) FROM group_sponsor_members_t WHERE object_id0 = :v_poid_id0' 
	INTO v_child_poid USING v_poid_id0;
      log_mesg(log_file_flag, log_file, 
               'traverse_account_group: /account '
               ||v_poid_id0||' found as Sponsoree.');
      OPEN cur_Sponsorer(v_child_poid);
      LOOP
        FETCH cur_Sponsorer INTO rec_Sponsorer;
        EXIT WHEN cur_Sponsorer%NOTFOUND;
        v_result := traverse_other_account_group(rec_Sponsorer.parent_id0,
                                           v_max_group_size,
                                           v_account_group_members,
										   v_billinfo_group_members,
                                           v_number_of_members,
                                           log_file_flag,
                                           log_file);
        IF (v_result = FALSE) THEN
          -- THE LIST OF ACCOUNT MEMBERS IS ALREADY CLEANED. JUST RETURN FALSE.
          CLOSE cur_Sponsorer;
          RETURN FALSE;
        END IF;
      END LOOP;
      CLOSE cur_Sponsorer;
    EXCEPTION
      WHEN NO_DATA_FOUND THEN
        IF (cur_Sponsorer%ISOPEN = TRUE) THEN
          CLOSE cur_Sponsorer;
        END IF;
      WHEN TOO_MANY_ROWS THEN
        log_mesg(log_file_flag, log_file, 
                 'traverse_account_group: ERROR: TRAVERSAL TERMINATED. Multiple rows not expected for Sponsoree /account ' || v_poid_id0);
        IF(cur_Sponsorer%ISOPEN = TRUE) THEN
          CLOSE cur_Sponsorer;
        END IF;
        v_account_group_members.DELETE();
        v_number_of_members := 0;
        RETURN FALSE;
    END;

    -- IF THE ACCOUNT IS OWNER OF CHARGE OR DISCOUNT SHARING.
    BEGIN
      OPEN cur_GroupShOwnerGroup(v_poid_id0);
      LOOP
        FETCH cur_GroupShOwnerGroup INTO rec_GroupShOwnerGroup;
        EXIT WHEN cur_GroupShOwnerGroup%NOTFOUND;
        log_mesg(log_file_flag, log_file, 
                 'traverse_account_group: /account '||v_poid_id0||
                 ' found as Group Sharing Owner for Group id '
                 || rec_GroupShOwnerGroup.poid_id0);
        -- FIND OUT ALL MEMBER OF CHARGE SHARING GROUP.
        OPEN cur_GroupShMembers(rec_GroupShOwnerGroup.poid_id0);
        LOOP
          FETCH cur_GroupShMembers INTO rec_GroupShMembers;
          EXIT WHEN cur_GroupShMembers%NOTFOUND;
          v_result := traverse_other_account_group(rec_GroupShMembers.account_obj_id0,
                                             v_max_group_size,
                                             v_account_group_members,
											 v_billinfo_group_members,
                                             v_number_of_members,
                                             log_file_flag,
                                             log_file);
          IF (v_result = FALSE) THEN
            -- THE LIST OF ACCOUNT MEMBERS IS ALREADY CLEANED. JUST RETURN FALSE.
            IF(cur_GroupShOwnerGroup%ISOPEN = TRUE) THEN
              CLOSE cur_GroupShOwnerGroup;
            END IF;
            IF (cur_GroupShMembers%ISOPEN = TRUE) THEN
              CLOSE cur_GroupShMembers;
            END IF;
            RETURN FALSE;
          END IF;
        END LOOP; -- END OF INNER LOOP FOR MEMBERS.
        IF (cur_GroupShMembers%ISOPEN = TRUE) THEN
          CLOSE cur_GroupShMembers;
        END IF;
      END LOOP; -- END OF OUTER LOOP FOR GROUP OWNER.

      EXCEPTION
        WHEN NO_DATA_FOUND THEN
          IF(cur_GroupShOwnerGroup%ISOPEN = TRUE) THEN
            CLOSE cur_GroupShOwnerGroup;
          END IF;
          IF (cur_GroupShMembers%ISOPEN = TRUE) THEN
            CLOSE cur_GroupShMembers;
          END IF;
        WHEN TOO_MANY_ROWS THEN
          log_mesg(log_file_flag, log_file, 
                   'traverse_account_group: WARNING: Multiple rows not expected for charge sharing group owner /account ' || v_poid_id0);
          IF(cur_GroupShOwnerGroup%ISOPEN = TRUE) THEN
            CLOSE cur_GroupShOwnerGroup;
          END IF;
          IF (cur_GroupShMembers%ISOPEN = TRUE) THEN
            CLOSE cur_GroupShMembers;
          END IF;
      END;

    -- IF THE ACCOUNT IS MEMBER OF CHARGE / DISCOUNT SHARING GROUP.
    BEGIN
      OPEN cur_GroupShMemberGroup(v_poid_id0);
      LOOP
        FETCH cur_GroupShMemberGroup INTO rec_GroupShMemberGroup;
        EXIT WHEN cur_GroupShMemberGroup%NOTFOUND;
        log_mesg(log_file_flag, log_file, 
                 'traverse_account_group: /account '
                 ||v_poid_id0||' found as Member of Group Sharing Id '
                 || rec_GroupShMemberGroup.obj_id0);
         -- TRAVERSE THE OWNER OF THE CHARGE SHARING GROUP.
         SELECT account_obj_id0 INTO v_tmp_poid FROM group_t 
                           WHERE poid_id0 = rec_GroupShMemberGroup.obj_id0;
         v_result := traverse_other_account_group(v_tmp_poid,
                                            v_max_group_size,
                                            v_account_group_members,
											v_billinfo_group_members,
                                            v_number_of_members,
                                            log_file_flag,
                                            log_file);
         IF (v_result = FALSE) THEN
         -- THE LIST OF ACCOUNT MEMBERS IS ALREADY CLEANED. JUST RETURN FALSE.
           IF(cur_GroupShMemberGroup%ISOPEN = TRUE) THEN
             CLOSE cur_GroupShMemberGroup;
           END IF;
           RETURN FALSE;
         END IF;
       END LOOP;
     EXCEPTION
       WHEN NO_DATA_FOUND THEN
       IF(cur_GroupShMemberGroup%ISOPEN = TRUE) THEN
         CLOSE cur_GroupShMemberGroup;
       END IF;
       WHEN TOO_MANY_ROWS THEN
         log_mesg(log_file_flag, log_file, 
                  'traverse_account_group: WARNING: Multiple rows not expected for charge sharing group member /account '|| v_poid_id0);
       IF(cur_GroupShMemberGroup%ISOPEN = TRUE) THEN
         CLOSE cur_GroupShMemberGroup;
       END IF;
     END;

    RETURN TRUE;
    END;

    --
    -- find account group members for the specified account
    --
    PROCEDURE find_account_group_members(
        v_poid_id0                 IN INTEGER,
        v_max_group_size           IN INTEGER,
        v_account_group_members    OUT GROUPACCOUNTLIST,
        v_number_of_members        OUT INTEGER,
        log_file_flag              IN CHAR,
        log_file_dir               IN VARCHAR2)
    AS
        v_counter    NUMBER;
        v_result     BOOLEAN;
        log_file     utl_file.file_type;
        v_billinfo_group_members GROUPACCOUNTLIST;
    BEGIN
      v_billinfo_group_members := GROUPACCOUNTLIST();
      log_open(log_file_flag, 
               log_file_dir, 
               'amt_find_group_members.log', log_file);

      log_mesg(log_file_flag,
               log_file,
               '=========================================================');
      log_mesg(log_file_flag,
               log_file,
               'find_account_group_members: BEGIN for poid ' || v_poid_id0);
      v_account_group_members := GROUPACCOUNTLIST();
      -- INITIALIZE THE ACCOUNT LIST TO EMPTY.
      v_account_group_members.DELETE();
      v_number_of_members := 0;
      -- IF poid_id0 IS ALREADY SELECTED FOR MIGRATION EARLIER,
      -- THE ENTRY FOR THIS ACCOUNT WOULE BE FOUND IN amt_account_detail_t
	EXECUTE IMMEDIATE 'SELECT COUNT(*) FROM job_batch_accounts_t,job_batch_t WHERE
             job_batch_accounts_t.account_obj_id0 = :v_poid_id0 
             AND job_batch_accounts_t.obj_id0=job_batch_t.poid_id0
             AND job_batch_t.poid_type=''/job_batch/amt''' INTO v_counter USING v_poid_id0;
      IF(v_counter > 0) THEN
          log_mesg(log_file_flag,
          log_file, 
          'find_account_group_members: Account poid '
          || v_poid_id0 ||'already selected for migration.');
          log_close(log_file_flag, log_file);
          RETURN;
      END IF;
      log_mesg(log_file_flag,
               log_file,
               'find_account_group_members: Performing account group traversal for '
               || v_poid_id0);
      -- TRAVERSE THE ACCOUNT GROUP TO FIND OUT ALL ACCOUNTS IN THE ACCOUNT GROUP.
      v_result := traverse_other_account_group (v_poid_id0,
                                          v_max_group_size,
                                          v_account_group_members,
                                          v_billinfo_group_members,
                                          v_number_of_members,
                                          log_file_flag,
                                          log_file);
      log_close(log_file_flag, log_file);
      RETURN;
    END;
END;
/
show errors;
