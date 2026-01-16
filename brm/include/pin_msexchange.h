#ifndef _pin_msexchange_h_
#define _pin_msexchange_h_
/* Continuus file information --- %full_filespec: pin_msexchange.h~4:incl:1 % */
/*
 * @(#) %full_filespec: pin_msexchange.h~4:incl:1 %
 *
 *      
 *      Copyright (c) 1996 - 2006 Oracle. All rights reserved.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

/*
 * The provisioning attributes are initialized to some known values at the
 * time of creation of the service object. These values indicate that the
 * provisioning attributes have not yet been set. For most integer attributes,
 * a value of -1 is an illegal value so -1 is used to signal the 'initial'
 * state.
 * When a provisioning product is purchased, these attributes get set to legal
 * values (for most integer attributes, 0 or positive values) and provisioning
 * is performed in MS Exchange. Subsequently, when the product is cancelled,
 * these attributes are set to -2 to differentiate it from the 'initial' state. 
 */
#define PIN_MSEXCHANGE_INIT_STATE_INT_VALUE  (-1)
#define PIN_MSEXCHANGE_CANCELLED_STATE_INT_VALUE (-2)

/*
 * Service type strings in MS Exchange service poids
 */
#define PIN_MSEXCHANGE_SERVICE_TYPE_ANY "/service/msexchange"
#define PIN_MSEXCHANGE_SERVICE_TYPE_USER "/service/msexchange/user"
#define PIN_MSEXCHANGE_SERVICE_TYPE_FIRSTADMIN \
										"/service/msexchange/user/firstadmin"

/*
 * Indicates the disabled or enabled states of stuff such as User, Mailbox,
 * POP3 access to mail, etc.
 */
typedef enum {
	PIN_MSEXCHANGE_DISABLED_STATE = 0,
	PIN_MSEXCHANGE_ENABLED_STATE = 1
} pin_msexchange_state_t;

/*
 * Indicates the specific MS Exchange service type
 */
typedef enum {
	/* NOT one of the MS Exchange service types! */
	PIN_MSEXCHANGE_SERVICE_NONE = 0,

	/* /service/msexchange/user */
	PIN_MSEXCHANGE_SERVICE_USER = 1,

	/* /service/msexchange/user/firstadmin */
	PIN_MSEXCHANGE_SERVICE_FIRSTADMIN = 2
} pin_msexchange_service_type_t;

/*
 * Indicates the specific MS Exchange product type
 */
typedef enum {
	/* Product that provides organization provisioning attributes */
	PIN_MSEXCHANGE_PRODUCT_ORG = 1,

	/* Product that provides email provisioning attributes */
	PIN_MSEXCHANGE_PRODUCT_EMAIL = 2
} pin_msexchange_product_type_t;

/*
 * Tells the specific provisioning XML request to be sent
 */
typedef enum {
	PIN_MSEXCHANGE_REQ_CREATE_ORG,
	PIN_MSEXCHANGE_REQ_CREATE_USER,
	PIN_MSEXCHANGE_REQ_CREATE_RM_EX_USER,
	PIN_MSEXCHANGE_REQ_DELETE_ORG,
	PIN_MSEXCHANGE_REQ_UPDATE_ORG,
	PIN_MSEXCHANGE_REQ_UPDATE_MBOX,
	PIN_MSEXCHANGE_REQ_USER_STATUS,
	PIN_MSEXCHANGE_REQ_GET_COUNT
} pin_msexchange_prov_request_type_t;

#endif
