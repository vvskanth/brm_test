/**********************************************************************
*
*	C Source:		fm_zonemap_pol_storage.c
*	Instance:		1
*	Description:	
*	%created_by:	bertm %
*	%date_created:	Wed Oct 04 13:26:20 2000 %
*
* Copyright (c) 1999, 2024, Oracle and/or its affiliates.
*
*       This material is the confidential property of Oracle Corporation
*       or its subsidiaries or licensors and may be used, reproduced, stored
*       or transmitted only in accordance with a valid Oracle license or
*       sublicense agreement. 
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#)$Id: fm_zonemap_pol_storage.c /cgbubrm_mainbrm.portalbase/1 2023/07/18 03:13:59 visheora Exp $";
#endif

#include <sys/types.h>
#include <netinet/in.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "pcm.h"
#include "ops/zonemap.h"
#include "cm_fm.h"
#include "pin_errs.h"
#include "pinlog.h"
#include "fm_utils.h"
#include "pin_zonemap.h"
#include "fm_zonemap_pol.h"

/********************************************************************
 * Helper functions
 ********************************************************************/
void
fm_zonemap_pol_convert_zonemap(
	pcm_context_t           *ctxp,
	pin_flist_t             *i_flp,
	pin_flist_t             **o_flpp,
	pin_errbuf_t            *ebufp);

/********************************************************************
 * Opcodes
 ********************************************************************/

EXPORT_OP void
op_zonemap_pol_set_zonemap(
	 cm_nap_connection_t     *connp,
	 u_int			opcode,
      	 u_int			flags,
      	 pin_flist_t		*i_flistp,
       	 pin_flist_t		**r_flistpp,
       	 pin_errbuf_t		*ebufp);

EXPORT_OP void
op_zonemap_pol_get_zonemap(
       cm_nap_connection_t     *connp,
       u_int			opcode,
       u_int			flags,
       pin_flist_t		*i_flistp,
       pin_flist_t		**r_flistpp,
       pin_errbuf_t		*ebufp);

/*
 * fm_zonemap_pol_convert_zonemap - converts a zonemap from one version to another, if necessary.
 * If the RAW data is provided, use it, otherwise we'll need to read it from the database based on
 * the provided POID.
 */
void
fm_zonemap_pol_convert_zonemap(
	pcm_context_t           *ctxp,
	pin_flist_t             *i_flp,
	pin_flist_t             **o_flpp,
	pin_errbuf_t            *ebufp)
{
	poid_t 		*pdp = NULL;
	pin_buf_t 	*bufp = NULL;
	pin_flist_t 	*cvti_flp = NULL;
	pin_flist_t 	*cvto_flp = NULL;
	pin_flist_t *ri_flp = NULL;
	pin_flist_t *ro_flp = NULL;
	pin_flist_t *tmp_flp = NULL;
	pin_flist_t *sub_flp = NULL;
	void *vp = NULL;

	/*
	 * If the raw data is passed then use it, otherwise we need to read it.
	 */
	pdp = (poid_t *)PIN_FLIST_FLD_GET(i_flp, PIN_FLD_POID, 0, ebufp);
	bufp = (pin_buf_t *)PIN_FLIST_FLD_GET(i_flp, PIN_FLD_ZONEMAP_DATA_RAW, 1, ebufp);
	if (bufp == NULL) {
		ri_flp = PIN_FLIST_CREATE(ebufp);
		PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_POID, pdp, ebufp);
		PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_ZONEMAP_DATA_RAW, NULL, ebufp);

		PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
			"fm_zonemap_pol_convert_zonemap : read raw zonemap data", ri_flp);

		PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, ri_flp, &ro_flp, ebufp);

		bufp = (pin_buf_t *)PIN_FLIST_FLD_GET(ro_flp, PIN_FLD_ZONEMAP_DATA_RAW, 0, ebufp);
	}
	if (bufp == NULL) {
		/*
		 * If the PIN_FLD_ZONEMAP_DATA_RAW is not present, I can't do anything.
		 * Get out with ebufp set to an error.
		 */
		goto cleanup;
	}

	/*
	 * Build the flist for PCM_OP_ZONEMAP_COMMIT_ZONEMAP to convert to the new version
	 */
	cvti_flp = PIN_FLIST_CREATE(ebufp);
	pin_str_to_flist(bufp->data, PIN_POID_GET_DB(pdp), &tmp_flp, ebufp);
	if (tmp_flp == NULL) {
		goto cleanup;
	}
	PIN_FLIST_FLD_SET(cvti_flp, PIN_FLD_POID, pdp, ebufp);
	sub_flp = PIN_FLIST_ELEM_ADD(cvti_flp, PIN_FLD_ZONEMAPS, 0, ebufp);
	PIN_FLIST_SUBSTR_PUT(sub_flp, tmp_flp, PIN_FLD_ZONEMAP_INFO, ebufp);
	tmp_flp = NULL;

	vp = PIN_FLIST_FLD_GET(i_flp, PIN_FLD_DESCR, 1, ebufp);
	if (vp != NULL) {
		PIN_FLIST_FLD_SET(sub_flp, PIN_FLD_DESCR, vp, ebufp);
	}
	vp = PIN_FLIST_FLD_GET(i_flp, PIN_FLD_NAME, 1, ebufp);
	if (vp != NULL) {
		PIN_FLIST_FLD_SET(sub_flp, PIN_FLD_NAME, vp, ebufp);
	}

	/*
	 * Call zonemap commit opcode
	 */
	PCM_OP(ctxp, PCM_OP_ZONEMAP_COMMIT_ZONEMAP, 0, cvti_flp, &cvto_flp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		goto cleanup;
	}

	/*
	 * Read the ZONEMAP's PIN_FLD_ZONEMAP_DATA_DERIVED
	 */
	PIN_FLIST_DESTROY_EX(&ri_flp, NULL);
	PIN_FLIST_DESTROY_EX(&ro_flp, NULL);

	ri_flp = PIN_FLIST_CREATE(ebufp);
	pdp = (poid_t *)PIN_FLIST_FLD_GET(cvto_flp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_POID, pdp, ebufp);
	PIN_FLIST_FLD_SET(ri_flp, PIN_FLD_ZONEMAP_DATA_DERIVED, 0, ebufp);

	PCM_OP(ctxp, PCM_OP_READ_FLDS, 0, ri_flp, &ro_flp, ebufp);

	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_zonemap_pol_convert_zonemap : failed converting zonemap", ebufp);
	} else {
		*o_flpp = ro_flp;
		ro_flp = NULL;
	}

cleanup:
	PIN_FLIST_DESTROY_EX(&cvti_flp, NULL);
	PIN_FLIST_DESTROY_EX(&cvto_flp, NULL);
	PIN_FLIST_DESTROY_EX(&ri_flp, NULL);
	PIN_FLIST_DESTROY_EX(&ro_flp, NULL);
	PIN_FLIST_DESTROY_EX(&tmp_flp, NULL);
	return;
}

/*******************************************************************
 * PCM_OP_ZONEMAP_POL_SET_ZONEMAP
 * converts the zonemap flist into a trie flist
 *******************************************************************/

void
op_zonemap_pol_set_zonemap (
	cm_nap_connection_t 	*connp,
	u_int			opcode,
	u_int			flags,
	pin_flist_t		*i_flistp,
	pin_flist_t		**r_flistpp,
	pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*zonemap_flistp = NULL;
	pin_flist_t		*return_flistp = NULL;
	int32			len = 0;
	pin_buf_t		*bufp = NULL;
	char			*strp = NULL;
	poid_t			*poidp = NULL;
	pin_zonemap_data_type_t	*data_typep = NULL;
	pin_zonemap_data_type_t	default_type = PIN_ZONEMAP_DATA_FLIST;
 	pin_zonemap_err_t	*errp = NULL;	
	int32 			zonemap_cnt = 0;
	char 			zmap_cnt_buf[100] = "";
	
	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_ZONEMAP_POL_SET_ZONEMAP) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_zonemap_pol_set_zonemap bad opcode error", ebufp);
		return;
	}

	/*
	 * Debug: What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_zonemap_pol_set_zonemap input flist", i_flistp);

	return_flistp = PIN_FLIST_CREATE(ebufp);
	
	/* Grab the poid of the zonemap */
	poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(return_flistp, PIN_FLD_POID, (void *)poidp, ebufp); 
	*r_flistpp = return_flistp;
		
	/* Check to see if the zone map is in flist format */
        data_typep = (pin_zonemap_data_type_t *)
			PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ZONEMAP_DATA_TYPE, 1, ebufp);

	if (data_typep == NULL) {
		/* default is flist */
		PIN_FLIST_FLD_SET(i_flistp, PIN_FLD_ZONEMAP_DATA_TYPE, (void *)(&default_type), ebufp);
	}
	else if (*data_typep == PIN_ZONEMAP_DATA_XML) {
		/* XML format, don't deal with it now */
		pin_set_err(ebufp, PIN_ERRLOC_FM,PIN_ERRCLASS_APPLICATION, 
				PIN_ERR_BAD_CRYPT, PIN_FLD_ZONEMAP_DATA_RAW, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_zonemap_pol_set_zonemap: unhandled XML data format", ebufp);
		PIN_FLIST_DESTROY_EX(&return_flistp, NULL);
		*r_flistpp = NULL;
		return;
	}

	/* Snag the zonemap flist from the flist */
	zonemap_flistp = (pin_flist_t *)PIN_FLIST_SUBSTR_GET(i_flistp,
			PIN_FLD_ZONEMAP_INFO, 0, ebufp);

	if(zonemap_flistp != NULL){
		zonemap_cnt = PIN_FLIST_COUNT(zonemap_flistp,ebufp);
		pin_snprintf(zmap_cnt_buf,sizeof(zmap_cnt_buf),"Number of fields in zonemap_flistp:[%d]",zonemap_cnt);
		PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, zmap_cnt_buf);
		if(zonemap_cnt <= 0){
			PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, "No fields under ZONEMAP_INFO,so NULLify zonemap_flistp");
			zonemap_flistp = NULL;
		}
	}

	/* If there are some data, construct the blob. */
	fm_zonemap_pol_construct_blob(zonemap_flistp, return_flistp, ebufp);

	/* Turn the flist into a (compact) string */
	len = 0;
	if (zonemap_flistp == NULL) {
		strp = (char *)NULL;
	}
	else {
		PIN_FLIST_TO_STR_COMPACT(zonemap_flistp, &strp, &len, ebufp);
	}

	bufp = (pin_buf_t *)calloc(1, sizeof(pin_buf_t));
	if(bufp != (pin_buf_t *)NULL) {
		if (strp != NULL) {	
			bufp->data = (caddr_t)strp;
			bufp->size = len + 1;		/* Include NULL termination */
		} else {
			bufp->data = (char *)NULL;
			bufp->size = 0;
		}	
		bufp->flag = 0;
		bufp->offset = 0;
		bufp->xbuf_file = (char *)NULL;

		/* Plob buf into flist */
		PIN_FLIST_FLD_PUT(return_flistp, PIN_FLD_ZONEMAP_DATA_RAW, 
			bufp, ebufp);
	} else {
		/* Memory not being able to allocated. Error. */ 
		pin_set_err(ebufp, PIN_ERRLOC_FM, PIN_ERRCLASS_APPLICATION,
			PIN_ERR_NO_MEM, PIN_FLD_ZONEMAP_DATA_RAW, 0, 0); 

		if (strp) {
			free(strp);
		}
	}
		
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"Error in op_zonemap_pol_set_zonemap", ebufp);
	}
	
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_zonemap_pol_set_zonemap output flist", *r_flistpp);
	
	return;
}

/******************************************************************
 * PCM_OP_ZONEMAP_POL_GET_ZONEMAP 
 * returns the trie flist if called by 
 * PCM_OP_ZONEMAP_POL_SPEC_IMPACT_CATEGORY
 * returns the zonemap flist otherwise
 *******************************************************************/
void
op_zonemap_pol_get_zonemap(
       cm_nap_connection_t     *connp,
       u_int			opcode,
       u_int			flags,
       pin_flist_t		*i_flistp,
       pin_flist_t		**r_flistpp,
       pin_errbuf_t		*ebufp)
{
	pcm_context_t		*ctxp = connp->dm_ctx;
	pin_flist_t		*zonemap_flistp = NULL;
	pin_flist_t		*tmp_flistp = NULL;
	pin_flist_t		*sub_flistp = NULL;
	pin_flist_t		*ret_flistp = NULL;
	pin_buf_t		*bufp = NULL;
	poid_t			*poidp = NULL;
	poid_t			*route_poidp = NULL;
	pin_zonemap_data_type_t		*data_typep = NULL;
	void			*vp = NULL;
	Blob_t			*blobp = NULL;
	u_int32			blob_ver = 0;

	if (PIN_ERR_IS_ERR(ebufp))
		return;
	PIN_ERR_CLEAR_ERR(ebufp);

	/*
	 * Insanity check.
	 */
	if (opcode != PCM_OP_ZONEMAP_POL_GET_ZONEMAP) {
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_SYSTEM_DETERMINATE,
			PIN_ERR_BAD_OPCODE, 0, 0, opcode);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"op_zonemap_pol_get_zonemap bad opcode error", ebufp);
		return;
	}

	/*
	 * Debug: What we got.
	 */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_zonemap_pol_get_zonemap input flist", i_flistp);

	/* Check to see if the zone map is in flist format */
	data_typep = (pin_zonemap_data_type_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ZONEMAP_DATA_TYPE, 1, ebufp);
	if (data_typep != NULL && *data_typep == PIN_ZONEMAP_DATA_XML) {
		/* XML format, don't deal with it now */
		pin_set_err(ebufp, PIN_ERRLOC_FM,
			PIN_ERRCLASS_APPLICATION, PIN_ERR_BAD_CRYPT, PIN_FLD_ZONEMAP_DATA_RAW, 0, 0);
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"fm_zonemap_pol_set_zonemap: unhandled XML data format", ebufp);
		*r_flistpp = NULL;
		return;
	}

	zonemap_flistp = PIN_FLIST_CREATE(ebufp);
	/* Grab the routing poid */
	poidp = (poid_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_POID, 0, ebufp);
	PIN_FLIST_FLD_SET(zonemap_flistp, PIN_FLD_POID, (void *)poidp, ebufp);

	/* Construct the data field and put it onto return flist */
	if (data_typep == NULL || *data_typep == PIN_ZONEMAP_DATA_FLIST) {
		/* If the data type is flist */
		bufp = (pin_buf_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ZONEMAP_DATA_RAW, 1, ebufp);
		if (bufp != NULL && bufp->data != NULL) {
			/* converts the buffer into flist */
			pin_str_to_flist(bufp->data, PIN_POID_GET_DB(poidp), &tmp_flistp, ebufp);

			if (tmp_flistp != NULL) {
				PIN_FLIST_SUBSTR_PUT(zonemap_flistp, tmp_flistp, PIN_FLD_ZONEMAP_INFO, ebufp);
			}
		}
	}
	else if (*data_typep == PIN_ZONEMAP_DATA_BINARY) {
		bufp = (pin_buf_t *)PIN_FLIST_FLD_GET(i_flistp, PIN_FLD_ZONEMAP_DATA_DERIVED, 1, ebufp);
		if (bufp != NULL && bufp->data != NULL) {
			/*
			 * Check if it is the right version number, and if it is add it to the output and
			 * convert that buffer.  If not, commit it back to convert to the new format.
			 */
			blobp = (Blob_t *)bufp->data;
			blob_ver = (u_int32)ntohl((u_int32)blobp->version);
			if (blob_ver == ZONEMAP_CURRENT_VERSION) {
				PIN_FLIST_FLD_SET(zonemap_flistp, PIN_FLD_ZONEMAP_DATA_DERIVED, (void *)bufp, ebufp);
				bufp = PIN_FLIST_FLD_GET(zonemap_flistp, PIN_FLD_ZONEMAP_DATA_DERIVED, 0, ebufp);
				if (bufp != NULL && bufp->data != NULL) {
					blobp = (Blob_t *)bufp->data;
					fm_zonemap_pol_pre_process_blob(blobp);
				}
				goto Done;
			}

			/* 
			 * Display this as an ERROR because the "Wrong Version" message was displayed as an
			 * ERROR and we want to tell the customer, who is running loglevel 1, that we are trying to
			 * convert this automatically.
			 */
			PIN_ERR_LOG_POID(PIN_ERR_LEVEL_DEBUG, 
					"op_zonemap_pol_get_zonemap converting zonemap to new version", poidp);

			fm_zonemap_pol_convert_zonemap(ctxp, i_flistp, &ret_flistp, ebufp);



			/* Now retry ... it should be the correct version now!
			 */
			bufp = (pin_buf_t *)PIN_FLIST_FLD_TAKE(ret_flistp, PIN_FLD_ZONEMAP_DATA_DERIVED, 0, ebufp);
			if (bufp != NULL && bufp->data != NULL) {
				blobp = (Blob_t *)bufp->data;
				blob_ver = (u_int32)ntohl((u_int32)blobp->version);

				/*
				 * Check if it is the right version number. If not,then something is very wrong!.
				 * Convert from network to host byte order and PUT on the output
				 */
				if (blob_ver == ZONEMAP_CURRENT_VERSION) {
					fm_zonemap_pol_pre_process_blob(blobp);
					PIN_FLIST_FLD_PUT(zonemap_flistp, PIN_FLD_ZONEMAP_DATA_DERIVED, bufp, ebufp);
				
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_DEBUG, 
						"op_zonemap_pol_get_zonemap successfully converted "
						"this zonemap to the new version");
					goto Done;
				} else {
					pin_free(bufp->data);
					pin_free(bufp);
					PIN_ERR_LOG_MSG(PIN_ERR_LEVEL_ERROR, 
						"op_zonemap_pol_get_zonemap unable to convert this "
						"zonemap to the new version");
					pin_set_err(ebufp, PIN_ERRLOC_FM,
						PIN_ERRCLASS_SYSTEM_DETERMINATE,
						PIN_ERR_BAD_VALUE,
						PIN_FLD_ZONEMAP_DATA_DERIVED, 0,
						0);
				}
			}
		}
	}
Done:			
	PIN_FLIST_DESTROY_EX(&ret_flistp, NULL);
	
	if (PIN_ERR_IS_ERR(ebufp)) {
		PIN_ERR_LOG_EBUF(PIN_ERR_LEVEL_ERROR,
			"Error in op_zonemap_pol_get_zonemap", ebufp);
		if (zonemap_flistp) {
			PIN_FLIST_DESTROY_EX(&zonemap_flistp, NULL);
		}
		*r_flistpp = NULL;
	} else {
		*r_flistpp = zonemap_flistp;
	}

	/* Send data back... */
	PIN_ERR_LOG_FLIST(PIN_ERR_LEVEL_DEBUG,
		"op_zonemap_pol_get_zonemap return flist", *r_flistpp);
	
	return;
}

