-- @(#)$Id: device_framework_secondary_schema_75ps4.sql /cgbubrm_7.5.0.portalbase/1 2015/11/27 06:18:11 nishahan Exp $
-- Copyright (c) 2011, 2013, Oracle and/or its affiliates. 
-- All rights reserved. 
--
-- This material is the confidential property of Oracle Corporation or its
-- licensors and may be used, reproduced, stored or transmitted only in
-- accordance with a valid Oracle license or sublicense agreement.
--
-- This file contins the RESIDENCY_TYPE for /device and /device/* objects
-- this file needs to be run only when user wants to support multi-schma 
-- and all the /device objects recides in primary schema

-- If Any of the devices listed below are not being used by customer 
--  they need to comment those devices entries.


-- Creation Date: May 23, 2011
--

update dd_objects_t set RESIDENCY_TYPE = '101' where name='/device';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/device/num';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/device/sim';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/device/ip';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/device/apn';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/device/voucher';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/au_device';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/au_device/num';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/au_device/sim';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/au_device/ip';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/au_device/apn';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/au_device/voucher';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/order' ;
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/order/sim' ;
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/order/voucher';
update dd_objects_t set RESIDENCY_TYPE = '101' where name='/block' ;


commit ;
