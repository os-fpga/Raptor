/**************************************************************************************************
* Copyright (c) 2017-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

#include "lmclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lm_attr.h"
#include "ls_attr.h"
#include "lsserver.h"
#include "ls_tpm_ex.h"

static void s_tpm_status_get_example();
static void s_log_tpm_error(const FlexTPMStatus *tpm_status_code,int tpm_errno);

/*-----------------------------------------------------------------------------------------
 *	An example to get Trusted Platform Module (TPM) status of license server
 *  void (*ls_user_init1)() = ls_user_init1_callback_example()
 -----------------------------------------------------------------------------------------*/
void ls_user_init1_callback_example()
{
	ls_log_message("TPM-STATUS: In User Init1 Callback.\n");
	s_tpm_status_get_example();
}

/*----------------------------------------------------------------------------------------- 
 * Get Trusted Platform Module (TPM) status of license server
 *-----------------------------------------------------------------------------------------*/
static void s_tpm_status_get_example()
{
	int tpm_errno = LM_TPM_PROPS_NOT_AVAILABLE;
	FlexTPMStatus tpm_status = FLEX_TPM_STATUS_NOT_DETERMINED;

	/* Get Trusted Platform Module (TPM) status of license server */
	if((tpm_errno = ls_tpm_status_get(&tpm_status)) == LM_NOERROR)
		ls_log_message("TPM-STATUS: Trusted Platform Module (TPM) detected. \n");
	else
		s_log_tpm_error(&tpm_status,tpm_errno);
}

/*-----------------------------------------------------------------------------------------
 * Print Trusted Platform Module (TPM) status info 
 *-----------------------------------------------------------------------------------------*/
static void s_log_tpm_error(const FlexTPMStatus *tpm_status_code,int tpm_errno)
{
	char buffer[128] = {'\0'};

	if (!tpm_status_code)
	{
		sprintf(buffer, "TPM-STATUS:NONE  %d",tpm_errno);
		ls_log_message((char*)buffer);
		return;
	}

	switch (*tpm_status_code)  
	{
		case FLEX_TPM_STATUS_DISABLED:
		{
			sprintf(buffer, "TPM-STATUS: Trusted Platform Module (TPM) inactive.  %d\n",tpm_errno);
			break;
		}
		case FLEX_TPM_STATUS_TPMVER_UNSUPPORTED:
		{
			sprintf(buffer, "TPM-STATUS: Trusted Platform Module (TPM) version not supported.  %d\n",tpm_errno);
			break;
		}
		case FLEX_TPM_STATUS_NOT_DETERMINED:       
		default:
			sprintf(buffer, "TPM-STATUS: Trusted Platform Module (TPM) status cannot be determined. %d\n",tpm_errno);
			 break;
	}

	ls_log_message((char*)buffer);
}
