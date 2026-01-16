/**************************************************************************
* Copyright (c) 1996, 2022, Oracle and/or its affiliates. All rights reserved. 
*
*      This material is the confidential property of Oracle Corporation or its
*      licensors and may be used, reproduced, stored or transmitted only in
*      accordance with a valid Oracle license or sublicense agreement.
****************************************************************************/

#ifndef _PCM_COLLECTIONS_OPS_H
#define _PCM_COLLECTIONS_OPS_H

/*
   NAME: TOTAL RANGE; USED RANGE; RESERVED RANGE; ASSOCIATED FM (if any)
   =====================================================================
   PCM_COLLECTIONS_OPS:   3800..3879; 3800..3810; 3811..3819; fm_collections
                      :               3820..3826; 3827..3839; fm_collections
                      :               3840..3854; 3855..3879; fm_collections
   PCM_COLLECTIONS_POL_OPS:  3880..3899; 3880..3894; 3895..3899; fm_collections_pol
 */

#include "ops/base.h"

        /* opcodes for collections configuration center*/
#define PCM_OP_COLLECTIONS_CONFIG_GET_PROFILES          3800
#define PCM_OP_COLLECTIONS_CONFIG_GET_ACTIONS           3801
#define PCM_OP_COLLECTIONS_CONFIG_GET_SCENARIOS         3802
#define PCM_OP_COLLECTIONS_CONFIG_GET_TEMPLATES         3803
#define PCM_OP_COLLECTIONS_CONFIG_GET_SCENARIO_DETAIL   3804
#define PCM_OP_COLLECTIONS_CONFIG_DELETE_PROFILE        3805
#define PCM_OP_COLLECTIONS_CONFIG_DELETE_ACTION         3806
#define PCM_OP_COLLECTIONS_CONFIG_DELETE_SCENARIO       3807
#define PCM_OP_COLLECTIONS_CONFIG_SET_PROFILE           3808
#define PCM_OP_COLLECTIONS_CONFIG_SET_ACTION            3809
#define PCM_OP_COLLECTIONS_CONFIG_SET_SCENARIO          3810

        /* core opcodes */
#define PCM_OP_COLLECTIONS_PROCESS_BILLINFO             3820
#define PCM_OP_COLLECTIONS_PROCESS_ACCOUNT              3820	/*PCM_OBSOLETE*/ /*renamed to PCM_OP_COLLECTIONS_PROCESS_BILLINFO*/ 
#define PCM_OP_COLLECTIONS_TAKE_ACTION                  3821
#define PCM_OP_COLLECTIONS_SET_DUNNING_LETTER           3822
#define PCM_OP_COLLECTIONS_GET_DUNNING_LETTER           3823
#define PCM_OP_COLLECTIONS_SET_INVOICE_REMINDER         3824
#define PCM_OP_COLLECTIONS_PUBLISH_EVENT                3825
#define PCM_OP_COLLECTIONS_EXECUTE_ACTION_PROMISE_TO_PAY       3826
  /* opcodes for collection group */
#define PCM_OP_COLLECTIONS_GROUP_GET_BILLINFO           3827
#define PCM_OP_COLLECTIONS_GROUP_CREATE                 3828
#define PCM_OP_COLLECTIONS_GROUP_MODIFY                 3829
#define PCM_OP_COLLECTIONS_GROUP_DELETE                 3830
#define PCM_OP_COLLECTIONS_GROUP_ADD_MEMBER             3831
#define PCM_OP_COLLECTIONS_GROUP_DELETE_MEMBER          3832
#define PCM_OP_COLLECTIONS_GROUP_SET_PARENT             3833

	/* policy opcodes */
#define PCM_OP_COLLECTIONS_POL_SELECT_PROFILE           3880
#define PCM_OP_COLLECTIONS_POL_EXIT_SCENARIO            3881
#define PCM_OP_COLLECTIONS_POL_PREP_DUNNING_DATA        3882
#define PCM_OP_COLLECTIONS_POL_EXEC_POLICY_ACTION       3883
#define PCM_OP_COLLECTIONS_POL_APPLY_LATE_FEES 	        3884
#define PCM_OP_COLLECTIONS_POL_APPLY_FINANCE_CHARGES    3885
#define PCM_OP_COLLECTIONS_POL_ASSIGN_AGENT    	        3886
#define PCM_OP_COLLECTIONS_POL_CALC_DUE_DATE            3887
#define PCM_OP_COLLECTIONS_POL_PUBLISH_EVENT            3888
#define PCM_OP_COLLECTIONS_POL_PROCESS_BILLINFO         3889
#define PCM_OP_COLLECTIONS_POL_INITIATE_PAYMENT         3890
#define PCM_OP_COLLECTIONS_POL_HANDLE_BREACH_PROMISE_TO_PAY    3891
#define PCM_OP_COLLECTIONS_POL_INVOKE_PROMISE_TO_PAY    3892
#define PCM_OP_COLLECTIONS_POL_GET_VALID_SCENARIOS      3893
#define PCM_OP_COLLECTIONS_POL_ASSIGN_DCA               3894
#define PCM_OP_COLLECTIONS_POL_GET_GROUP_TARGET_ACTIONS 3895


        /* opcodes for collection center */
#define PCM_OP_COLLECTIONS_GET_BILLINFOS                3840
#define PCM_OP_COLLECTIONS_GET_ACCOUNTS                	3840	/*PCM_OBSOLETE*/ /*renamed to PCM_OP_COLLECTIONS_GET_BILLINFOS*/
#define PCM_OP_COLLECTIONS_ASSIGN_AGENT                 3841
#define PCM_OP_COLLECTIONS_EXEMPT_BILLINFO              3842
#define PCM_OP_COLLECTIONS_EXEMPT_ACCOUNT               3842	/*PCM_OBSOLETE*/ /*renamed to PCM_OP_COLLECTIONS_EXEMPT_BILLINFO*/
#define PCM_OP_COLLECTIONS_GET_AGENTS_ACTIONS           3843
#define PCM_OP_COLLECTIONS_ADD_ACTION                   3844
#define PCM_OP_COLLECTIONS_RESCHEDULE_ACTION            3845
#define PCM_OP_COLLECTIONS_SET_ACTION_STATUS            3846
#define PCM_OP_COLLECTIONS_GET_SCENARIO_DETAIL          3847
#define PCM_OP_COLLECTIONS_GET_ACTION_HISTORY           3848
#define PCM_OP_COLLECTIONS_CALC_AGING_BUCKETS           3849
#define PCM_OP_COLLECTIONS_INVOKE_PROMISE_TO_PAY        3850
#define PCM_OP_COLLECTIONS_REVOKE_PROMISE_TO_PAY        3851
#define PCM_OP_COLLECTIONS_UPDATE_ACTION_PAYMENT_DETAILS 3852
#define PCM_OP_COLLECTIONS_GET_VALID_SCENARIOS          3853
#define PCM_OP_COLLECTIONS_REPLACE_SCENARIO             3854
#define PCM_OP_COLLECTIONS_NOTIFY_COLLECTIONS_ACTION	3855

        /* reserved through 3899 */

#endif /* _PCM_COLLECTIONS_OPS_H_ */
