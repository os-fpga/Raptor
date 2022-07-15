/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/**
 *	FlexAct error codes
 */


#ifndef FlxActError_h
#define FlxActError_h	1

#include "FlxActSystemError.h"	/* API System Error Codes (10000-19999) */

/* 
 * API Major Error Codes (50000-59999)
 *
 * These errors are returned in the majorErrorNo member of the FlxActError tuple.
 * More detail may be available from the sysErrorNo member (see FlxActSystemError.h) 
*
 * Errors in the main range 50000-50999 are defined here
*/
#define	LM_TS_NO_ERROR					50000 /* Success */
#define LM_TS_BADPARAM					50001 /* An invalid parameter was passed to an API function */
#define LM_TS_CANTMALLOC				50002 /* No memory available */
#define LM_TS_OPEN_ERROR				50003 /* Unable to open Trusted Storage (see sysErrorNo) */
#define LM_TS_CLOSE_ERROR				50004 /* Error when closing Trusted Storage */
#define LM_TS_INIT						50005 /* No previous successful call to flxActCommonLibraryInit OR mode is Certifcate SDT */
#define LM_TS_ACT_GEN_REQ				50006 /* Unable to create activation transaction request */
#define LM_TS_RETURN_GEN_REQ			50007 /* Unable to create return transaction request */
#define LM_TS_REPAIR_GEN_REQ			50008 /* Unable to create repair transaction request */
#define LM_TS_RESERVED_9				50009 /* Not used */
#define LM_TS_DELETE					50010 /* Unable to delete fulfillment record or stored transaction request */
#define LM_TS_CANT_FIND					50011 /* Can't find a fulfillment record or a vendor dictionary entry */
#define LM_TS_INTERNAL_ERROR			50012 /* Unexpected error (see sysErrorNo) */
#define LM_TS_RESERVED_13				50013 /* Not used */
#define LM_TS_INVALID_INDEX				50014 /* Invalid index specified when getting item from collection, 
												 e.g. FlxActProdLicSpc from FlxActLicSpc */
#define LM_TS_RESERVED_15				50015 /* Not used */
#define LM_TS_NO_ASR_FOUND				50016 /* ASR does not exist or is not suitable for the action requested */
#define LM_TS_UPDATING_TS				50017 /* Error when saving changes to Trusted Storage */
#define LM_TS_SEND_RECEIVE				50018 /* Error communicating with Enterprise or Operations License Server */
#define LM_TS_PROCESS_RESP				50019 /* Error processing a transaction response */
#define LM_TS_UNKNOWN_RESP				50020 /* The transaction response is invalid or of an unknown type */
#define LM_TS_ASR_LOAD_ONCE				50021 /* The ASR has already been used on this machine */
#define LM_TS_RESERVED_22				50022 /* Not used */
#define LM_TS_SHORT_CODE_PENDING		50023 /* A new short transaction request code cannot be created because 
												 one is already pending for the same ASR */
#define LM_TS_SHORT_CODE_ACT_CREATE		50024 /* Error creating activation shortcode */
#define LM_TS_SHORT_CODE_REPAIR_CREATE	50025 /* Error creating repair shortcode */
#define LM_TS_SHORT_CODE_RETURN_CREATE	50026 /* Error creating return shortcode */
#define LM_TS_SHORT_CODE_CANCEL			50027 /* Unable to cancel outstanding shortcode request */
#define LM_TS_SHORT_CODE_PROCESS		50028 /* The server denied the request OR there was an unexpected error 
												 post-processing the response (see sysErrorNo) */
#define LM_TS_SHORT_CODE_UNSUPPORTED	50029 /* The ASR supplied is not a shortcode ASR (it's for local activations) */
#define LM_TS_LOAD						50030 /* Unable to load shortcode ASR from path specified OR unexpected error 
												 loading an item from trsuted storage (see sysErrorNo) */
#define LM_TS_FR_DISABLE				50031 /* Unable to disable fulfillment record during return transaction */
#define LM_TS_TIMEDOUT					50032 /* The server did not respond to a request within the timeout period */
#define LM_TS_INSUFFICIENT_RESOURCE		50033 /* The server has refused the request because it did not have enough
												 licenses. Note that the license server will only provide licenses 
												 that can be served from a single fulfillment record */
#define LM_TS_INVALID_REQUEST_TYPE		50034 /* The server has refused the request because it does not support 
												 the request type */
#define LM_TS_NO_MATCHING_FULFILLMENT	50035 /* The server has refused the request because it does not have a 
												 fulfillment record that matches the entitlementID specified OR 
												 the expiration date or duration specified is later than the 
												 expiration of the fulfillment record OR server	Trusted Storage 
												 has become untrusted */
#define LM_TS_INVALID_REQUEST			50036 /* The server has refused the request because it sees the request as invalid */
#define LM_TS_RETURN_OUT_OF_CHAIN		50037 /* The server has refused the return request because it did not 
												 serve the fulfillment record */
#define LM_TS_MAX_COUNT_EXCEEDED		50038 /* The server has refused the return request because of an 
												 unexpected error when removing the deduction */
#define LM_TS_INSUFFICIENT_REPAIR_COUNT	50039 /* The server has refused the repair request because the specified number
												 of repairs have already been completed for this fulfillment record*/
#define LM_TS_OPERATIONS				50040 /* The Operations server has returned an error. 
												 Call flxActCommonHandleGetLastOpsError 
												 and flxActCommonHandleGetLastOpsErrorString 
												 to retrieve details of the error.*/
#define LM_TS_CONNECTION_FAILED			50041 /* Failed to connect to the license server or Operations server */
#define LM_TS_SSL_ERROR					50042 /* Error with SSL certificate provided. See flxActCommonHandleSetSSLDetails*/
#define LM_TS_RETURN_INCOMPLETE			50043 /* Return of license server fulfillment not allowed because there are 
												 outstanding deductions. Either return all licenses served from this
												 fulfillment or use flxActSvrReturnForceIncompleteSet */
#define LM_TS_LOCAL_REPAIR				50044 /* Error occurred during call to flxActCommonRepairLocalTrustedStorage */
#define LM_TS_UNSUPPORTED_REQUEST_VERSION	50045 /* The license server does not support the fulfillment version
													 specified in the request */
#define LM_TS_UNSUPPORTED				50046 /* The response contains a fulfillment record of a version that is 
												 not supported */
#define LM_TS_CONFIGURATION				50047 /* The ASR being used for local activation specifies a Trial Id but
												 Trusted Storage anchoring is disabled */
#define LM_TS_NO_PRODUCT_SET			50048 /* Error cancelling return request - the fulfillment has not been specified,
												 see flxActAppReturnProdLicSpcSet */
#define LM_TS_TRIALPACKS_ASR_ENTRY		50049 /* The ASR being used for local activation contains 
												 invalid Trial Pack values. */
#define LM_TS_VIRTUALIZATION_POLICY_MISMATCH	50050 /* The ASR being used for local activation has a virtualization
												 policy that excludes the current environment */
#define LM_TS_VIRTUAL_INTERFACE			50051 /* The attempt to retrieve Virtual Machine attributes failed */
#define LM_TS_SERVERQUERY_GEN_REQ		50052 /* The request to query remote server-side trusted storage could not
												 be generated*/	
#define LM_TS_SERVERQUERY_RESPONSE_FAILED	50053 /* The response to a server-side trusted storage query 
													 could not be processed */
#define LM_TS_SERVERQUERY_FR_NOT_FOUND	50054 /* The fulfillment record specified in a server-side trusted storage 
												 query does not exist on the server */
#define LM_TS_VM_ATTRIBUTE_PRIVILEGE	50055	/* The requested attribute could not be retrieved due to insufficient user
												   privilege */

#define LM_TS_VM_ATTRIBUTE_PROTECTED	50056	/* The requested attribute could not be retrieved because a protected resource
												   was encountered */

#define LM_TS_SHORTCODE					50058	/* flxActShortCodeXXX functions - short code action failed, see system error code */
#define LM_TS_BADEXP_DATE				50059	/* Invalid expiration date passed */

#define LM_TS_TRANSFER_TO_SELF			50060	/* Transfer requests to a license server on the same machine are not allowed */
#define LM_TS_SERVICE_INFO				50061	/* Unable to get information from the service; see system error code */
#define LM_TS_NOT_THIS_PLATFORM			50062	/* The action or information requested is not supported on this platform */
/* 
* Errors in the range 51000-51999 are returned only by Composite Transaction functions ( flxActTranXXX() )
* and are defined in FlxActTransaction.h
*/
#define LM_TS_CT_BASE					51000
#define LM_TS_CT_LAST					51999

/* 
 * Errors in the range 53000-53999 are returned only by 3Server Trusted Storage related functions
*/
#define LM_TS_3SERVER_NOT_PERMITTED				53000 /* Not used */
#define LM_TS_3SERVER_CONFIGURED				53001 /* Not used */
#define LM_TS_3SERVER_NOT_CONFIGURED			53002 /* Action not allowed because this machine is not confugured for 3ServerTS */
#define LM_TS_3SERVER_VALIDATION				53003 /* The configuration data for this 3ServerTS node is invalid or incomplete */
#define LM_TS_3SERVER_TWO_NEW_NODES_NOT_ALLOWED 53004 /* Two 3ServerTS nodes cannot be replaced at the same time */
#define LM_TS_3SERVER_BUFFER_SIZE				53005 /* The buffer supplied to a 3ServerTS API function is too small */
#define LM_TS_3SERVER_NOT_PRIMARY				53006 /* Action not allowed on this 3ServerTS node because it is not the Primary node */
#define LM_TS_3SERVER_NOT_CONCURRENT			53007 /* Non-concurrent licenses may not be requested or activated on 3ServerTS */

/*
* Defines for the Major Error Code range.
*/
#define LM_TS_BASE_ERROR				LM_TS_NO_ERROR
#define LM_TS_LAST_ERROR				LM_TS_BASE_ERROR + 9999

#endif /* FlxActError_h */
