-- @(#)$Id: device_framework_primary_schema_75ps4.sql /cgbubrm_7.5.0.portalbase/1 2015/11/27 06:18:09 nishahan Exp $
-- Copyright (c) 2011, 2013, Oracle and/or its affiliates. 
-- All rights reserved. 
--
-- This material is the confidential property of Oracle Corporation or its
-- licensors and may be used, reproduced, stored or transmitted only in
-- accordance with a valid Oracle license or sublicense agreement.
--
-- This file contins the permissions for /device and /device/* objects 
-- rename the SEC_SCHEMA_NAME to the actual schema name

-- if Any of the devices listed below are not being used by customer they need to comment 
-- those devices entries.

-- This file needs to be run only when user wants to support multi-schma
-- and all the /device objects recides in primary schema

-- Creation Date: May 23, 2011
--

grant all on DEVICE_T TO SEC_SCHEMA_NAME ; 
grant all on DEVICE_NUM_T TO SEC_SCHEMA_NAME ;
grant all on DEVICE_SIM_T TO SEC_SCHEMA_NAME ;
grant all on DEVICE_IP_T TO SEC_SCHEMA_NAME ;
grant all on DEVICE_VOUCHER_T TO SEC_SCHEMA_NAME;
grant all on DEVICE_SERVICES_T TO SEC_SCHEMA_NAME ;

grant all on AU_DEVICE_T TO SEC_SCHEMA_NAME ; 
grant all on AU_DEVICE_NUM_T TO SEC_SCHEMA_NAME ;
grant all on AU_DEVICE_SIM_T TO SEC_SCHEMA_NAME ;
grant all on AU_DEVICE_IP_T TO SEC_SCHEMA_NAME ;
grant all on AU_DEVICE_VOUCHER_T TO SEC_SCHEMA_NAME;
grant all on AU_DEVICE_SERVICES_T TO SEC_SCHEMA_NAME ;

grant all on PIN_SEQUENCE TO SEC_SCHEMA_NAME ;
grant all on POID_IDS2 TO SEC_SCHEMA_NAME ;

grant all on BLOCK_T TO SEC_SCHEMA_NAME ;
grant all on ORDER_T TO SEC_SCHEMA_NAME ;
grant all on ORDER_SIM_RESULTS_T TO SEC_SCHEMA_NAME ;
grant all on ORDER_SIM_ORDERS_T TO SEC_SCHEMA_NAME ;
grant all on ORDER_NAME_INFO_T TO SEC_SCHEMA_NAME ;
grant all on ORDER_ORDERS_T TO SEC_SCHEMA_NAME ;
grant all on ORDER_RESULTS_T TO SEC_SCHEMA_NAME ;
grant all on ORDER_PHONES_T TO SEC_SCHEMA_NAME ;
grant all on ORDER_FILES_T TO SEC_SCHEMA_NAME ;
grant all on ORDER_VOUCHER_T TO SEC_SCHEMA_NAME;

commit ;
