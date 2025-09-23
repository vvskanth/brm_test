--
-- Copyright (c) 2005, 2009, Oracle and/or its affiliates. 
-- All rights reserved. 
--
--      This material is the confidential property of Oracle Corporation or
--      its licensors and may be used, reproduced, stored or transmitted only
--      in accordance with a valid Oracle license or sublicense agreement.
--
--      SQL  file to update the /config/collections/action object for 
-- 	writeoff billinfo action's opcode number field for Momentum and
--	above releases


UPDATE config_collections_action_t  SET opcode=4512   WHERE  obj_id0=821; 

COMMIT;
