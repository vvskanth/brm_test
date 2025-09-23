CREATE OR REPLACE VIEW job_history_vw
("POID_DB", 
 "POID_TYPE", 
 "POID_ID0", 
 "POID_REV", 
 "TRACKING_ID", 
 "PROCESS_START_T", 
 "PROCESS_END_T", 
 "PROCESSING_TIME",
 "JOB_STATUS", 
 "STATUS", 
 "REASON_ID",
 "REASON_DOMAIN_ID",
 "TEMPLATE_OBJ_DB", 
 "TEMPLATE_OBJ_TYPE", 
 "TEMPLATE_OBJ_ID0", 
 "TEMPLATE_OBJ_REV", 
 "USER_TAG", 
 "TEMPLATE_STATUS",
 "TEMPLATE_NAME",
 "CAT_TYPE",
 "JOB_TYPE",
 "CREATED_BY",
 "PARENT_TEMPLATE_OBJ_DB", 
 "PARENT_TEMPLATE_OBJ_TYPE", 
 "PARENT_TEMPLATE_OBJ_ID0", 
 "PARENT_TEMPLATE_OBJ_REV", 
 "JOB_OBJ_DB", 
 "JOB_OBJ_TYPE", 
 "JOB_OBJ_ID0", 
 "JOB_OBJ_REV",
 "WORKFLOW_OBJ_DB",
 "WORKFLOW_OBJ_TYPE",
 "WORKFLOW_OBJ_ID0",
 "WORKFLOW_OBJ_REV"
 ) AS 
  ((select 
  a.poid_db ,
  a.poid_type,
  a.poid_id0,
  a.poid_rev,
  a.tracking_id,
  a.process_start_t,
  a.process_end_t,
  a.processing_time,
  decode(b.status, 11,6,12,7,13,8,b.status),
  b.status,
  b.reason_id,
  b.reason_domain_id,
  c.poid_db,
  c.poid_type,
  c.poid_id0,
  c.poid_rev,
  c.user_tag,
  c.status,
  c.name,
  c.cat_type,
  c.job_type,
  c.created_by,
  c.parent_template_obj_db,
  c.parent_template_obj_type,
  c.parent_template_obj_id0,
  c.parent_template_obj_rev,
  c.job_obj_db, 
  c.job_obj_type,
  c.job_obj_id0,
  c.job_obj_rev,
  b.workflow_obj_db, 
  b.workflow_obj_type,
  b.workflow_obj_id0,
  b.workflow_obj_rev
  from job_t a, 
      job_boc_t b,
      job_template_t c
  where a.poid_id0 = b.obj_id0
  and b.template_obj_id0 = c.poid_id0
  and (b.status >= 5 or (b.status=1 and c.cat_type=612) ))
  UNION ALL
  (select 
  a.job_obj_db ,
  a.job_obj_type,
  a.job_obj_id0,
  a.job_obj_rev,
  to_char(a.job_obj_id0),
  a.process_start,
  a.process_end,
  (a.process_end - a.process_start),
  decode(a.failrecords, 0,5,6) ,
  decode(a.failrecords, 0,5,6),
  0,
  0,
  a.poid_db,
  decode(process_name,'pin_export_price','/job_template/price_sync',
                      'fm_price','/job_template/price_sync',
                      'pin_bill_accts', '/job_template/billling',
                      'Bill-Now','/job_template/billling',
                      'Bill-ON-DEMAND','/job_template/billling',
                      'Auto-Trigger','/job_template/billling',
                      'pin_trial_bill_accts','/job_template/billling',
                      'pin_collect', '/job_template/collect',
                      'pin_inv_accts', '/job_template/invoice',
                      'pin_ledger_report', '/job_template/ledger_report',
                      '/job_template'),
  -1,
   0,
   NULL,
   10100,
   'System Job',
   decode(process_name,'pin_export_price',605,
                      'fm_price',605,
                      'pin_bill_accts', 601,
                      'Bill-Now',601,
                      'Bill-ON-DEMAND',601,
                      'Auto-Trigger',601,
                      'pin_trial_bill_accts',601,
                      'pin_collect', 602,
                      'pin_inv_accts', 603,
                      'pin_ledger_report', 604,
                      600),
  1,
  'System',
  a.poid_db,
  decode(process_name,'pin_export_price','/job_template/price_sync',
                      'fm_price','/job_template/price_sync',
                      'pin_bill_accts', '/job_template/billling',
                      'Bill-Now','/job_template/billling',
                      'Bill-ON-DEMAND','/job_template/billling',
                      'Auto-Trigger','/job_template/billling',
                      'pin_trial_bill_accts','/job_template/billling',
                      'pin_collect', '/job_template/collect',
                      'pin_inv_accts', '/job_template/invoice',
                      'pin_ledger_report', '/job_template/ledger_report',
                      '/job_template'),
  -1,
   0,
   a.poid_db,
  '/job/boc',
  -1,
   0,
   a.poid_db,
  '/job/boc',
  -1,
   0
  from proc_aud_t a
  where a.job_obj_type like '%system%')
);
/
show errors;
/
CREATE OR REPLACE VIEW rev_assurance_vw
 ("POID_ID0", 
  "FAILRECORDS", 
  "SUCCESSRECORDS", 
  "INPUTRECORDS", 
  "NUMBER_OF_BILLS", 
  "NUM_DEALS", 
  "NUM_DISCOUNTS", 
  "NUM_PRODUCTS", 
  "NUM_PLAN_LISTS", 
  "NUM_PLANS", 
  "NUM_SPONSORSHIPS") AS 
  ((select 
   a.job_obj_id0,
   sum(a.failrecords),
   sum(a.successrecords),
   sum(a.inputrecords),
   sum(b.number_of_bills),
   0,
   0, 
   0, 
   0, 
   0,
   0  
   from proc_aud_t a, proc_aud_bill_t b 
   where a.poid_id0 = b.obj_id0
   and a.job_obj_id0 != 0
   group by a.job_obj_id0)
   union All
   (select 
   a.job_obj_id0,
   sum(a.failrecords),
   sum(a.successrecords),
   sum(a.inputrecords),
   0, 
   0, 
   0,
   0, 
   0, 
   0,
   0  
   from proc_aud_t a
   where a.process_name = 'pin_ledger_report'
   and a.job_obj_id0 != 0
   group by a.job_obj_id0)
   union All
   (select 
   a.job_obj_id0,
   a.failrecords,
   a.successrecords,
   a.inputrecords,
   0,
   b.num_deals,
   b.num_discounts,
   b.num_products,
   b.num_plan_lists,
   b.num_plans, 
   b.num_sponsorships
   from proc_aud_t a, proc_aud_price_sync_t b
   where a.poid_id0 = b.obj_id0
   and a.job_obj_id0 != 0)
   union All
   (select 
   a.obj_id0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0, 
   0
   from job_boc_t a
   where a.status >= 5
   and NOT EXISTS (SELECT * FROM proc_aud_t b
                    WHERE a.obj_id0 = b.job_obj_id0))
   UNION ALL
   (select
   a.obj_id0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0
   from job_boc_t a
   where a.status =1 and a.template_obj_type='/job_template/workflow'
   )
);
/
show errors;
/
CREATE OR REPLACE VIEW TRANS_TYPE1_LEVEL0_VW
(
        "CREATED_T", "TRANS_ID", "ACCOUNT_NO", "AMOUNT", "COMMAND", "FIRST_NAME", "LAST_NAME", "TOTAL_ROWS"
) AS
(
        SELECT
                TEMP_T.CREATED_T, TEMP_T.TRANS_ID, TEMP_T.ACCOUNT_NO, TEMP_T.AMOUNT, TEMP_T.COMMAND, ANIT.FIRST_NAME, ANIT.LAST_NAME,
                count(*) over () TOTAL_ROWS
    FROM
                (
                        (SELECT
                                ET.ACCOUNT_OBJ_ID0, ET.CREATED_T, EBVT.TRANS_ID, ACT.ACCOUNT_NO, EBVT.AMOUNT, EBVT.COMMAND
                        FROM
                                EVENT_T ET JOIN ACCOUNT_T ACT on ACT.POID_ID0 = ET.ACCOUNT_OBJ_ID0
                                JOIN EVENT_BILLING_VALID_T EBVT on ET.POID_ID0 = EBVT.OBJ_ID0
                        WHERE
                                EBVT.COMMAND = 1 and EBVT.RESULT >= 999)
                        UNION ALL
                        (SELECT
                                ET.ACCOUNT_OBJ_ID0, ET.CREATED_T, EBCT.TRANS_ID, ACT.ACCOUNT_NO, EBCT.AMOUNT, EBCT.COMMAND
                        FROM
                                EVENT_T ET JOIN ACCOUNT_T ACT on ACT.POID_ID0 = ET.ACCOUNT_OBJ_ID0
                                JOIN EVENT_BILLING_CHARGE_T EBCT on ET.POID_ID0 = EBCT.OBJ_ID0
                        WHERE
                                EBCT.COMMAND = 2 and EBCT.RESULT >= 888)
                ) TEMP_T
                JOIN ACCOUNT_NAMEINFO_T ANIT ON ANIT.OBJ_ID0 = TEMP_T.ACCOUNT_OBJ_ID0
);
/
show errors;
/
CREATE OR REPLACE VIEW TRANS_TYPE2_LEVEL0_VW
(
        "POID_DB", "POID_ID0", "POID_TYPE", "POID_REV", "BATCH_ID", "CREATED_T", "PAY_TYPE", "ACH", "NO_OF_PAYMENTS", "BATCH_TYPE"
) AS
(
        (
        SELECT
                TEMP1.POID_DB, TEMP1.POID_ID0, TEMP1.POID_TYPE, TEMP1.POID_REV, TEMP1.BATCH_ID, TEMP1.CREATED_T,
                TEMP2.PAY_TYPE, TEMP2.ACH, TEMP2.NO_OF_PAYMENTS, 1 "BATCH_TYPE"
        FROM
                (
                SELECT
                        ET.POID_DB, ET.POID_ID0, ET.POID_TYPE, ET.POID_REV, EPBT.BATCH_ID, ET.CREATED_T
                FROM
                        EVENT_T ET, EVENT_PAYMENT_BATCH_T EPBT
                WHERE
                        ET.POID_TYPE = '/event/billing/batch/payment' AND
                        ET.POID_ID0    = EPBT.OBJ_ID0
                ) TEMP1,
                (
                SELECT
                        UNIQUE EBCT.PAY_TYPE, EBCT.ACH, ET.SESSION_OBJ_ID0, COUNT(ET.SESSION_OBJ_ID0) NO_OF_PAYMENTS
                FROM
                        EVENT_T ET, EVENT_BILLING_CHARGE_T EBCT
                WHERE
                        ET.POID_ID0 = EBCT.OBJ_ID0 AND
                        (EBCT.COMMAND = 4 OR EBCT.COMMAND   = 5) AND
                        EBCT.RESULT   = 999
                GROUP BY ET.SESSION_OBJ_ID0, EBCT.PAY_TYPE, EBCT.ACH
                ) TEMP2
        WHERE
                TEMP1.POID_ID0 = TEMP2.SESSION_OBJ_ID0
        )
        UNION ALL
        (
        SELECT
                TEMP1.POID_DB, TEMP1.POID_ID0, TEMP1.POID_TYPE, TEMP1.POID_REV, TEMP1.BATCH_ID, TEMP1.CREATED_T,
                TEMP2.PAY_TYPE, TEMP2.ACH, TEMP2.NO_OF_PAYMENTS, 2 "BATCH_TYPE"
        FROM
                (
                SELECT
                        ET.POID_DB, ET.POID_ID0, ET.POID_TYPE, ET.POID_REV, EPBT.BATCH_ID, ET.CREATED_T
                FROM
                        EVENT_T ET, EVENT_PAYMENT_BATCH_T EPBT
                WHERE
                        ET.POID_TYPE = '/event/billing/batch/refund' AND
                        ET.POID_ID0    = EPBT.OBJ_ID0
                ) TEMP1,
                (
                SELECT
                        UNIQUE EBCT.PAY_TYPE, EBCT.ACH, ET.SESSION_OBJ_ID0, COUNT(ET.SESSION_OBJ_ID0) NO_OF_PAYMENTS
                FROM
                        EVENT_T ET, EVENT_BILLING_CHARGE_T EBCT
                WHERE
                        ET.POID_ID0 = EBCT.OBJ_ID0 AND
                        EBCT.COMMAND = 6 AND
                        EBCT.RESULT  = 999
                        GROUP BY ET.SESSION_OBJ_ID0, EBCT.PAY_TYPE, EBCT.ACH
                ) TEMP2
        WHERE
                TEMP1.POID_ID0 = TEMP2.SESSION_OBJ_ID0
        )
);
/
show errors;
/
CREATE OR REPLACE VIEW TRANS_TYPE2_LEVEL1_VW
(
        "CREATED_T", "TRANS_ID", "ACCOUNT_NO", "FIRST_NAME", "LAST_NAME", "AMOUNT", "SESSION_OBJ_ID0", "TRANSACTION_TYPE",
        "PAY_TYPE", "ACH", "TOTAL_ROWS"
) AS
(
        SELECT
                ET.CREATED_T, EBCT.TRANS_ID, AT.ACCOUNT_NO, ANT.FIRST_NAME, ANT.LAST_NAME, EBCT.AMOUNT, ET.SESSION_OBJ_ID0,
                DECODE(ET.SESSION_OBJ_TYPE, '/event/billing/batch/refund',2,'/event/billing/batch/payment', 1, 0) "TRANSACTION_TYPE",
                EBCT.PAY_TYPE, EBCT.ACH, COUNT(*) OVER () total_rows
        FROM
                EVENT_T ET, EVENT_BILLING_CHARGE_T EBCT, ACCOUNT_T AT, ACCOUNT_NAMEINFO_T ANT
        WHERE
                ET.POID_ID0 = EBCT.OBJ_ID0 AND
                ET.ACCOUNT_OBJ_ID0   = AT.POID_ID0 AND
                ET.ACCOUNT_OBJ_ID0   = ANT.OBJ_ID0
);
/
show errors;
/
CREATE OR REPLACE VIEW TRANS_TYPE3_LEVEL0_VW
(
        CREATED_T, TRANS_ID, ACCOUNT_NO, AMOUNT, COMMAND, FIRST_NAME, LAST_NAME, RESULT, TOTAL_ROWS
) AS
(
        SELECT
                TEMP_T.CREATED_T, TEMP_T.TRANS_ID, TEMP_T.ACCOUNT_NO, TEMP_T.AMOUNT, TEMP_T.COMMAND, TEMP_T1.FIRST_NAME, TEMP_T1.LAST_NAME,
                TEMP_T.RESULT, COUNT(*) OVER () TOTAL_ROWS
        FROM
        (
                SELECT
                        ET.ACCOUNT_OBJ_ID0, ET.CREATED_T, EBCT.TRANS_ID, act.ACCOUNT_NO, EBCT.AMOUNT, EBCT.COMMAND, EBCT.RESULT
                from
                        EVENT_T ET JOIN ACCOUNT_T act on act.POID_ID0 = ET.ACCOUNT_OBJ_ID0
                JOIN
                        EVENT_BILLING_CHARGE_T EBCT ON ET.POID_ID0 = EBCT.OBJ_ID0
                WHERE
                        (EBCT.COMMAND = 4 OR EBCT.COMMAND = 5 OR EBCT.COMMAND = 6) AND
                        EBCT.RESULT >= 1000
        ) TEMP_T
        JOIN
        (
                SELECT ant.OBJ_ID0,ant.FIRST_NAME, ant.LAST_NAME from ACCOUNT_NAMEINFO_T ant
                JOIN (
                        SELECT
                                OBJ_ID0, MIN(REC_ID) as REC_ID
                        FROM
                                ACCOUNT_NAMEINFO_T
                        GROUP BY OBJ_ID0) temp on temp.OBJ_ID0 = ant.OBJ_ID0
        ) TEMP_T1
        ON
                TEMP_T1.OBJ_ID0 = TEMP_T.ACCOUNT_OBJ_ID0
);
/
show errors;
/
CREATE OR REPLACE VIEW UNIFIED_JOB_HISTORY_VW AS SELECT * FROM JOB_HISTORY_VW;
CREATE OR REPLACE VIEW UNIFIED_REV_ASSURANCE_VW AS SELECT * FROM REV_ASSURANCE_VW;
CREATE OR REPLACE VIEW UNIFIED_JOB_TEMPLATE_COLL_VW  AS SELECT * FROM JOB_TEMPLATE_COLL_AC_INFO_T;
CREATE OR REPLACE VIEW UNIFIED_TTYPE1_LEVEL0_VW  AS SELECT * FROM TRANS_TYPE1_LEVEL0_VW;
CREATE OR REPLACE VIEW UNIFIED_TTYPE2_LEVEL0_VW  AS SELECT * FROM TRANS_TYPE2_LEVEL0_VW;
CREATE OR REPLACE VIEW UNIFIED_TTYPE2_LEVEL1_VW  AS SELECT * FROM TRANS_TYPE2_LEVEL1_VW;
CREATE OR REPLACE VIEW UNIFIED_TTYPE3_LEVEL0_VW  AS SELECT * FROM TRANS_TYPE3_LEVEL0_VW;
create or replace PACKAGE job_history_pkg wrapped 
a000000
367
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
9
554 223
UgaQqbikdtObzAQ8NeB8yHGf7SYwg9fDAK5qfC8BeaqV8OngqUhN62y+FOj0pDrWFrdsb7xA
ZnDumgvaLHnKuZ7MZq0/u/+8F532fFULNaXAMEuucixPB/6W+DOSU8knso+HtSniqFO2ofqJ
b8IM8MPm4V7KPgf5IMERlxQ3+ibtgFinBFdquJkTzc3XgUNIR7GevnLVs0UWnHungmmIsRZs
nxw8dcM2ZltI/EbhF7RDtfmSsCwutX8GITkRGlo+gvEOynmQCZ9XRBNrYHKbckAIqa0IWtOu
7cm3m+rvmTlvxuzKl1rMBTUzAawlTfCbuKo5glAW8SsLLXarUEvP9ceGJZm+xyveH0/183pG
5jaOZdof1ekeyRB5CU+wL3JmRi9+KgoLRFNQx0vyDOWzwH+cPw2rGvoWxPBwWAFYe2OpHVDD
g0PY5SzkhjjptIaSFUYeKlXRJYXFQ7Cqk5pYMHMXYOz94IpmPA33vIu26tK85xjXX1J+acab
hHNOQSHrcCZMraTkAVcqAQOWnx+aSrFmJQ==

/
show errors;
/
create or replace PACKAGE BODY job_history_pkg wrapped 
a000000
367
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
b
29a5 c02
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
nR3Sgbdm

/
show errors;
/
create or replace PACKAGE failed_transactions_pkg wrapped 
a000000
367
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
9
255 158
lWAHRUxC1nTARXZtYVWItfmkUEEwg433rvZqfHSVv/+wpVICkJ/XoS09R3cD8NiR0hQV+czf
RbMz+F5Jsjj1LjH1wkOa64Ij89KeJ8MgVYtOgenLmEiYu0EoE2TNh7wkGZkTGOY+h4DDGgK9
z9NVaZ4afglGDDJFjVgYjiHsQGk9hcI2HlBf0x10WAQen3ujJOHW1oGXeNCUJVUg3ThtSECF
rkFsOfbbw7cMKCgyTH9XTz6jynOm9SSwLYtPG0bND47REbhqHNH5ysg18ScAAzmerr7V9DfO
e6wZceHmamVjfsjHrCYgQYCdFm0/FXJGNQOyiOMKRTSt4fDtyLE=

/
show errors;
create or replace PACKAGE BODY failed_transactions_pkg wrapped 
a000000
367
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
abcd
b
1466 66e
/rN6nQEzfWtjlkZLNluureHbz2Uwg+1UuyCM3wQZsUNkMJ33w3Rszc0amLpJrXCj4qmCExoe
SdsqoG08GwAOJtz8LJ2xZ+EUZ1qY1wnV4IckP6p6P0sAqJKUvg58ydSyaERtB7i5ScxLX9YF
kS2j1Pnrmv4ADNKmupIgFVjc8D6wuDUAmhSnLrKahtuUvDV6Ja7wkQNoNAWYWFBRtt2QewuX
PRCEE/9NVUjjb+LBrnUBztRTjDginSFu8+IP0z+DkSeIVcqtkA0Eg1lJM9+RERMTUtbAqs9e
KjuvXYqrEhDu87kyfxris0CF6EFygS5VTQcBjsK2YZCSiZnUWynjNPnC188IB7KNYb3YtTjO
KYamIk/WUc8+soXgd3lllKB7j/VX2C2dvBpuPIPQ8jMpqX/tS2gZn4r51gHmnW6LxwSy3Zga
SGTTNT2ArR3bcq44XUmjp7IU1HpfIAw/CbqK1k8cvdz1kC73w9EeNqqfFDQuNuC2B5/mtWnW
QbqNb2U5vWfITNrZYSIDkMIM9XLgSDmdhhG5VgCewxdK7wDYavFKE1vTCB4tXPWR8/bAWdqE
fW7Trp4pHPK5uO8N1RW4y2ouefJ7qe0N81xvo/z/pof5nowvUUNO7+RKcM0IduFXB3JtAaAj
ryw/bjbM4a/AQe34O9LF2//cqB8o8ClNfDJ33XeS9Vo4tzXb7oQwE929aOkKgH27zGQRvkPo
40knb2mrONlLjh+j9wqC9XxHg6dgxX71DALmkaWtY+WeOtPMr6Aw4vPFfkPmvIAD+fqmtF+b
+abjptBC26eg06Ro96ifMyP8o+hs7acB/QFcq5IDHmqm1i65Ey7qs3pfjwUoaYcPv5xwg14b
UqQpnvR9Gqy7wSv9Cuy2BUcbNXnlElu3WCDlw4W/5mrkodQTJAZhgAJ/CNLZiVREBcvWtxN2
432cxVNHF+jQkMtIJts5XqshZlhure2MdfUjqaHW37l+3l5kUBYytURCYDtd+p70Xfe+V1Sl
0zcOR8qtzkvS9RQuF+7eUFjwHM25HcN9c1YFyyAZSQyQFndgY/ca8fXVYhKLaw7mWmQxzgpx
IVSiPiESxhix6mNS/Eii5JyBHoq165a0lEv+bT2j1X9IO5RuuvYBPDsNsq88gzO3xDyXkXEf
mF1qZclpkIignTxXlF79OAiirHsmFv9mSh/ZEjCeP3pA2TZRpW+ZW21sX76MFsXb9G9J8+wa
GumaRMdcV1fuBeeqNCtBlKO9HtRr3yoAzpelZz56SL4fWht0dHW6RsGWi1pRLqwUlXtkU9PW
07JqjdnyJwYxBcx0iy8MrBaeKNYwDUYFlJf+lrc0WclLMDDUzSczZSuJxK6NLJJ5HhjJLFtE
/ZEJyJ3Vex4NDhB4xxovIbno8C3vz3r8ruhBQKq8eb6OkvLkc/tqMFlw82rAgeHs6EOaV3H4
HRb7NP6Prxt3O52pfIhyi/atMvUEsl43XWjtKswK/w2R4/km6/TkjnkSfA3iaKCF/dGa2yqT
yJGWNEDkRdV1W6GmCpjk7LKXsX5YVlErGmCarFeJOpzeZFPduN/RSkETYLPFrPh8oRpgY0ho
OMazP4VZPRmSOJtdmMCqM38f+V38Btmutabvnmtl

/
show errors;
