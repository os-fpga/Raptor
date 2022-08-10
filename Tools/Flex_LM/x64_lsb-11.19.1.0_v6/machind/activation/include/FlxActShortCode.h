/**************************************************************************************************
* Copyright (c) 2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
 * FlxActShortCode include file
 *
 * API functions for Short Code Transactions
 * These are common to both the application and the server activation applications.
 */

#ifndef FlxActShortCode_h
#define FlxActShortCode_h 1

/*	Usage.

	Pre-requisite - an API handle obtained from flxActCommonOpen

	For request code creation and management:

	1. Obtain a short code context handle using one of
			flxActShortCodeCreate				ASR is read from a file
			flxActShortCodeCreateFromBuffer		ASR is in memory

	2a. To create a request code use one of 
			flxActShortCodeGetActivationRequest
			flxActShortCodeGetReturnRequest
			flxActShortCodeGetRepairRequest

	2b. To obtain a pending request code 
			flxActShortCodeGetPendingRequest
	   A request is pending from the time it is created until a valid response is processed or it is cancelled.
	   Only one request may be pending for a given ASR; to create a different one, cancel the pending one.

	2c. To cancel a pending request
			flxActShortCodeCancelRequest
		Note that cancelling a return request re-enables the fulfillment making it usable again.
		You need to trust that the user has not completed the return on the server, otherwise it's an exploit.
		The alternative is to delete the fulfillment as well as the request
		(see function flxActCommonLicSpcProductDelete).

	3. Destroy the short code context 
			flxActShortCodeDestroy
	
	When the response is available,  process it using 
			flxActShortCodeProcessResponse
	No context is neeeded as the ASR is cached in trusted storage when the request is created and it can be 
	identified from the content of the response code.
	These functions can be called to get more information about the response processing
			flxActShortCodeGetDenyReason	For Deny response types
			flxActShortCodeGetWarning		On success - see flxShortCodeWarning below  
			flxActShortCodeGetErrorDetail	On failure - 


	Vendor data can be included in requests and responses - see flxActShortCodeSetVendorData() below.
*/
	
	
#include "FlxActCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Context for short code operations.
 */
typedef struct flxActShortCode*	FlxActShortCode;

/*
 * Create and destroy a short code context.
 */
FlxActBool flxActShortCodeCreate(
		FlxActHandle		handle,			/* As returned by flxActCommonHandleOpen	*/
		const char*			pszAsrFile,		/* Path of short code ASR XML				*/
		FlxActShortCode*	phShortCode);	/* For return of the short code handle		*/

FlxActBool flxActShortCodeCreateFromBuffer(
		FlxActHandle		handle,			/* As returned by flxActCommonHandleOpen	*/
		const char*			pszAsrBuffer,	/* Buffer containing short code ASR XML		*/
		FlxActShortCode*	phShortCode);	/* For return of the short code handle		*/

void flxActShortCodeDestroy(FlxActShortCode hShortCode);

FlxActBool flxActShortCodeGetActivationRequest(
		FlxActShortCode		hShortCode,
		const char**		ppszRequest);		/* For return of a pointer to the short code request */

FlxActBool flxActShortCodeGetReturnRequest(
		FlxActShortCode		hShortCode,
		const char*			pszFulfillmentId,	/* The fulfillment to be returned						*/
		const char**		ppszRequest);		/* For return of a pointer to the short code request	*/

FlxActBool flxActShortCodeGetRepairRequest(
		FlxActShortCode		hShortCode,
		const char*			pszFulfillmentId,	/* The fulfillment to be returned						*/
		const char**		ppszRequest);		/* For return of a pointer to the short code request */

FlxActBool flxActShortCodeGetPendingRequest(
		FlxActShortCode		hShortCode,
		const char**		ppszRequest,	/* For return of a pointer to the short code request */
		FlxShortCodeType*	pRequestType);	/* For return of the request type */

FlxActBool flxActShortCodeGetPendingRequestProdSpc(
		FlxActShortCode		hShortCode,
		FlxActLicSpc		licSpc);		/* For RETURN and REPAIR, FlxActProdSpc of target fulfillment is added */

FlxActBool flxActShortCodeCancelRequest(FlxActShortCode hShortCode);

/*
 * Process a short code response.
 * On success, if pResponseType is flxShortCodeTypeError then
 *			   call flxActShortCodeGetDenyReason() to get the deny reason given by the server,
 *             otherwise call flxActCommonRespProdLicSpcGet() to get details of the fulfillment just 
 *             activated, returned or repaired.
 *             For activations, optionally call flxActShortCodeGetWarning to find out whether 
 *             an existing fulfillment was overwritten.
 * On failure, call flxActCommonHandleGetError() to get the error tuple.
 *			   See also flxActShortCodeGetErrorDetail() below.
 */
FlxActBool flxActShortCodeProcessResponse(
		FlxActHandle		handle,			/* As returned by flxActCommonHandleOpen	*/
		const char*			pszResponse,	/* The response code from the server		*/
		FlxShortCodeType*	pResponseType);	/* For return of the response type			*/

/*
 * Get the deny reason.
 * If the request type returned by flxActShortCodeProcessResponse() is flxShortCodeTypeError
 * then this function will return the reason given by the server for denying the request.
 * If the response was of a different type or the server did not set a reason, it will return zero.
 * Note that the reason values are defined by the server, not FNP.
 */
uint32_t flxActShortCodeGetDenyReason(FlxActHandle handle);

/*
 * Response processing warnings - set when processing succeeds but there is something to note
 */
typedef enum 
{
	flxShortCodeWarningNone						= 0,
	flxShortCodeWarningFulfillmentOverwitten	= 1,


	/* Returned by flxActShortCodeGetWarning if handle is NULL */
	flxShortCodeWarningInvalidHandle			= -1
} flxShortCodeWarning;

flxShortCodeWarning flxActShortCodeGetWarning(FlxActHandle handle);

/*
 *	Additional error details.
 *
 * For system errors LM_TSSE_SHORTCODE_INVALID_CHAR and LM_TSSE_SHORTCODE_INVALID_CHECK the details
 * contain a copy of the short code with potentially invalid characters replaced by a '?' 
 */
const char* flxActShortCodeGetErrorDetail(FlxActHandle handle);

/*
 * Vendor Data
 *
 * Any vendor data items must be pre-defined in the ASR - name, type and length.
 * Request item values are set by calling flxActShortCodeSetVendorData() before creating the request.
 * Response items are written to the fulfillment record and can be queried using 
 * flxActCommonProdLicSpcVendorDataGetCount() and flxActCommonProdLicSpcVendorDataGetByIndex()
 */
FlxActBool flxActShortCodeSetVendorData(
		FlxActShortCode	hShortCode,
		const char*		pszItemName,		/* The name as defined in the short code ASR					*/
		const char*		pszItemValue);		/* A value valid for the type specified in the ASR,
											   e.g. "123" for DEC, "F12A" for HEX, "MyValue" for ASCII96	*/

#ifdef __cplusplus
}
#endif

#endif


