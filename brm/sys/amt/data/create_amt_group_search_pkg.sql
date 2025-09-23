--
--      @(#) % %
--     
-- Copyright (c) 1996, 2013, Oracle and/or its affiliates. All rights reserved.
--     
--      This material is the confidential property of Oracle Corporation or its
--      licensors and may be used, reproduced, stored or transmitted only in
--      accordance with a valid Oracle license or sublicense agreement.
--

CREATE OR REPLACE TYPE GROUPACCOUNTLIST IS TABLE OF NUMBER(38);
/

create or replace
PACKAGE amt_group_search
IS

-- function: is_account_member_included
--   To check if the member is already included in list of members.
-- parameters:
--   IN v_poid_id0                 Account which is being checked in the list.
--   IN v_account_group_members    List of members that are already included.
--   IN log_file_flag              For turning logs TRUE/FALSE.
--   IN log_file                   Log media where logs are generated.
  FUNCTION is_account_member_included(
       v_poid_id0                 IN INTEGER,
       v_account_group_members    IN OUT GROUPACCOUNTLIST,
       log_file_flag              IN CHAR,
       log_file                   IN utl_file.file_type)
  RETURN BOOLEAN;


-- function: traverse_account_group
--   Implements the account group traversal algorithm.
--   Account group is traversed recursively. If total number of members
--   included in the group exceed the maximum limit, the traversal
--   terminates and an empty list is returned.
-- parameters:
--   IN v_poid_id0                     Account for which group traversal is executed.
--   IN v_max_group_size               Max limit to number of members.
--   IN OUT v_account_group_members    List of account group members.
--   IN OUT v_number_of_members        Total number of members already included.
--   IN log_file_flag              For turning logs TRUE/FALSE.
--   IN log_file                   Log media where logs are generated.
  FUNCTION traverse_account_group(
       v_poid_id0                 IN INTEGER, -- POID OF THE MEMBER ACCOUNT.
       v_max_group_size           IN INTEGER, -- ACCOUNT GROUP SIZE GREATER THAN
                                              -- THIS WILL NOT BE INCLUDED IN THE JOB.
       v_billinfo_group_members    IN OUT GROUPACCOUNTLIST,
       log_file_flag              IN CHAR,
       log_file                   IN utl_file.file_type)
  RETURN BOOLEAN;
  
      FUNCTION traverse_other_account_group(
       v_poid_id0                 IN INTEGER, -- POID OF THE MEMBER ACCOUNT.
       v_max_group_size           IN INTEGER, -- ACCOUNT GROUP SIZE GREATER THAN
                                              -- THIS WILL NOT BE INCLUDED IN THE JOB.
       v_account_group_members    IN OUT GROUPACCOUNTLIST,
       v_billinfo_group_members  IN OUT GROUPACCOUNTLIST,
       v_number_of_members        IN OUT INTEGER,
       log_file_flag              IN CHAR,
       log_file                   IN utl_file.file_type)
  RETURN BOOLEAN;


-- procedure: find_account_group_members
--   Find all account group members for the specified account.
-- parameters:
--   IN v_poid_id0                 Account for which all other members
--                                 are to be found out.
--   IN v_max_group_size           Maximum number of members allowed.
--   OUT v_account_group_members   Account members for the group, if the
--                                 traversal was successful.
--   OUT v_number_of_members       Number of account group members found.
--   IN log_file_flag              For turning logs TRUE/FALSE.
--   IN log_file                   Log media where logs are generated.
  PROCEDURE find_account_group_members(
       v_poid_id0                 IN INTEGER,
       v_max_group_size           IN INTEGER,
       v_account_group_members    OUT GROUPACCOUNTLIST,
       v_number_of_members        OUT INTEGER,
       log_file_flag              IN CHAR,
       log_file_dir               IN VARCHAR2);

END;
/
show errors;
