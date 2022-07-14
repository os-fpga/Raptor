/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Vendor daemon attributes.
 *
 */

#ifndef _LS_ATTR_H
#define _LS_ATTR_H

#define LS_ATTR_FEATURE						1	/* (char *) */
#define LS_ATTR_NLIC						2	/* (int)    */
#define LS_ATTR_FLAG						3	/* (int)    */
#define LS_ATTR_VERSION						4	/* (char *) */
#define LS_ATTR_CLIENT						5	/* (CLIENT_DATA *) */
#define LS_ATTR_DUP_SEL						6	/* (int)    */
#define LS_ATTR_LINGER						7	/* (int)    */
#define LS_ATTR_CODE						8	/* (char *) */
#define LS_ATTR_SERVER						9   /* (int)    */
#define LS_ATTR_VENDOR_NAME					10	/* (char *) */
#define LS_ATTR_LAST_REPORT_LOG_ROTATION	20	/* (time_t) */
#define LS_ATTR_REPORT_LOG_FILENAME			21	/* (const char *) */
#define LS_ATTR_CHECKOUT_DATA				30  /* (char *) */
#define LS_ATTR_NLIC_OD						33  /* (int)    */
#define LS_ATTR_TOTAL_NLIC_OD_INUSE			34  /* (int)    */
#define LS_ATTR_OD_REQUESTS					35  /* (int)    */
#define LS_ATTR_CLIENT_TIMEZONE				36  /* (int)    */
#define LS_ATTR_CLIENT_TIME					37  /* (time_t) */
#define LS_ATTR_CLIENT_HOSTID_ETHER			38  /* (char *) - release memory after its use */
#define LS_ATTR_CLIENT_HOSTID_VENDOR		39  /* (char *) - release memory after its use */

int ls_get_attr(int key, char **val);
int ls_set_attr(int key, char *val);

/*
 * Determine if license server is a MASTER node in 3-Server configuration:
 * 
 * Return: 
 *         non-zero: vendor daemon is master
 *         zero: non-master
 *
 * The use of this API is limited to ls_daemon_periodic (vendor callback) only. 
 * This API cannot be called from ls_user_init* callbacks because this state is unknown at that point.
 */
int ls_get_status_is_master(void);

/*
 * Extracting client HostID from vendor daemon callbacks: 
 *    Use this API in ls_client_hostid_callback (vendor callback) to set required client hostid types.
 *
 *    Client hostid values could be obtained by calling ls_get_attr API with LS_ATTR_CLIENT_HOSTID_* attributes.
 *    LS_ATTR_CLIENT_HOSTID_* attributes require to release memory after its use.
 *    The use of this API is limited to ls_client_hostid_callback (vendor callback) only.
 *
 *    The checkout filter toolkit example (ls_co_ex.c) demonstrates extraction of both MAC address(es) and 
 *    vendor defined hostid of the client requesting feature checkout, via s_get_client_hostids sample function.
 */

void ls_attr_init_client_hostids(const int *cl_hostid_list, int cl_hostid_cnt);


/*
 * Get Trusted Platform Module (TPM) status of license server 
 * 
 * Parameters: void* - Pointer to FlexTPMStatus
 *
 * Return: int
 *
 * The toolkit example (ls_tpm_ex.c) demonstrates API usage.
 */
int ls_tpm_status_get(void* tpm_status_code);

#endif
