--
--      Copyright (c) 2004-2007  Oracle Software, Inc. All rights reserved.
--      This material is the confidential property of Oracle Software, Inc.
--      or its subsidiaries or licensors and may be used, reproduced, stored
--      or transmitted only in accordance with a valid Oracle license
--      or sublicense agreement.
--
--
--      SQL  file to update the /config/collections/action object from 
-- 	account to billinfo architechture
--      and also this will migrate collections related data from 6.5 to 6.7



UPDATE collections_action_t ca
   SET (billinfo_obj_id0
       ,billinfo_obj_db
       ,billinfo_obj_type
       ,billinfo_obj_rev) =
       (SELECT poid_id0
              ,poid_db
              ,poid_type
              ,poid_rev
          FROM billinfo_t bi
         WHERE bi.account_obj_id0 = ca.account_obj_id0
              and bi.account_obj_db = ca.account_obj_db and bi.account_obj_type = ca.account_obj_type);

COMMIT;

UPDATE collections_scenario_t cs
   SET (billinfo_obj_id0
       ,billinfo_obj_db
       ,billinfo_obj_type
       ,billinfo_obj_rev) =
       (SELECT poid_id0
              ,poid_db
              ,poid_type
              ,poid_rev
          FROM billinfo_t bi
         WHERE bi.account_obj_id0 = cs.account_obj_id0
              and bi.account_obj_db = cs.account_obj_db and bi.account_obj_type = cs.account_obj_type);

COMMIT;

UPDATE config_collections_action_t  SET action_descr='Writeoff Billinfo' ,action_type='writeoff_billinfo' ,action_name='Writeoff Billinfo' WHERE  obj_id0=821; 

COMMIT;

UPDATE config_collections_action_t  SET action_descr='close services of billinfo' ,action_type='close_billinfo' ,action_name='Close services of billinfo' WHERE  obj_id0=822; 

COMMIT;

UPDATE config_collections_action_t  SET action_descr='inactivate services of billinfo' ,action_type='inactivate_billinfo' ,action_name='Inactivate services of billinfo' WHERE  obj_id0=823; 

UPDATE collections_action_t  SET action_mode=0;

UPDATE config_collections_actions_t SET action_mode=0;

COMMIT;
