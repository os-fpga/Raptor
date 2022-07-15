/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This source file is part of a sample utility that demonstrates Composite Transactions.

	It defines the functions use to load, list and cancel outstanding requests.

	Only composite requests are managed; single action requests are stored separaely.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FlxActCommon.h"
#include "FlxActError.h"

#include "compTranUtilManageRequests.h"
#include "compTranUtilCreateRequest.h"
#include "compTranUtilResponse.h"

typedef struct FlxCtuRequestFilter_struct
{
	FlxActBool	bIsAllSeqNos;
	const char*	pszSeqNoFilter;
} FlxCtuRequestFilter;

typedef enum flxCtuListType_enum
{
	flxCtuListTypeShort						= 0,
	flxCtuListTypeLong						= 1,
	flxCtuListTypeXml						= 2,
} FlxCtuListType;

static FlxActBool sDeleteCurrentRequest(FlxCtuContext* pContext);

static FlxCtuListType sGetListType(const FlxCtuCommand* pCommand);

static const char* sGetRequestStatusString(FlxActTranRequest hTranRequest);
static const char* sGetSequenceNoOfCurrentRequest(FlxCtuContext* pContext);
static const char* sGetSequenceNoOfRequest(FlxActTranRequest hTranRequest);

static FlxActBool sHasThisSequenceNo(FlxActTranRequest hTranRequest, const char* pszSequenceNo);

static FlxActBool sIsListFilterPass(FlxActTranRequest hTranRequest, 
									const FlxCtuRequestFilter* pFilter);

static void sListRequestLong(FlxActTranRequest hTranRequest);
static void sListRequestXml(FlxActTranRequest hTranRequest);
static void sListNameAndValue(const char*pszName, const char*pszValue);

static void sSetRequestFilter(const FlxCtuCommand* pCommand, FlxCtuRequestFilter* pFilter);

/***************************************************************************************************
*	flxCtuDoCommandStored
*
* Process a "stored" command - load an exiting request from trusted storage and output for the user.
***************************************************************************************************/
FlxActBool flxCtuDoCommandStored(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	/* Load the request, making it the current request for the transaction. */
	if ( !flxCtuLoadStoredRequest(pContext, pCommand) )
		return FLX_ACT_FALSE;

	/* Write the request XML to file / stdout. */
	if ( !flxCtuRequestOutputXml(pContext, pCommand) )
		return FLX_ACT_FALSE;

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuDoCommandCancelAll
*
* Cancel all stored composite requests.
***************************************************************************************************/
FlxActBool flxCtuDoCommandCancelAll(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	uint32_t	initialRequestCount	= 0;
	uint32_t	currentRequestCount	= 0;
	uint32_t	requestIndex		= 0;
	uint32_t	deletedRequestCount	= 0;

	FlxActTranRequest   hTranRequest;

	/* Get the number of stored composite requests. */
	if ( !flxActTransactionGetStoredRequestCount(pContext->hTransaction, &initialRequestCount) )
	{
		flxCtuContextPrintLastApiError(pContext);
		return FLX_ACT_FALSE;
	}

	/* Iterate over the requests, deleting each one. */
	for (requestIndex = 0, currentRequestCount = initialRequestCount; 
		 requestIndex < currentRequestCount; )
	{
		/* Load request and make current for transaction - should not fail if 
		   hTransaction and requestIndex are valid. */
		if ( !flxActTransactionLoadStoredRequest(pContext->hTransaction, 
												 requestIndex,
												 &hTranRequest) )
		{
			FLX_CTU_ASSERT(FLX_ACT_FALSE);
		}

		/* This could fail if another process has just deleted it. */
		if ( sDeleteCurrentRequest(pContext) )
		{
			/* Request deleted so adjust count (next request now has same index as this one) */
			currentRequestCount--;
		}
		else
		{
			/* Request could not be deleted so continue with the next. */
			requestIndex++;
		}
	}

	/* Show the result. */
	deletedRequestCount = initialRequestCount - currentRequestCount;
	flxCtuPrintMessage("Cancelled %u of %u stored composite requests.\n", 
					   deletedRequestCount, 
					   initialRequestCount);
	return (deletedRequestCount == initialRequestCount)? FLX_ACT_TRUE : FLX_ACT_FALSE;
}

/***************************************************************************************************
*	flxCtuDoCommandCancelRequest
*
* Delete the stored composite request with the sequence number specified by the command value.
***************************************************************************************************/
FlxActBool flxCtuDoCommandCancelRequest(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	const char*			pszSequenceNo	= NULL;
	uint32_t			requestCount	= 0;
	uint32_t			requestIndex	= 0;
	FlxActTranRequest   hTranRequest;

	/* Get the sequence number from the command value. */
	if ( !flxCtuCommandHasValue(pCommand) )
	{
		flxCtuCommandPrintError(pCommand, "expecting a composite request sequence number.\n");
		return FLX_ACT_FALSE;
	}
	pszSequenceNo = flxCtuCommandGetValue(pCommand);

	/* Get the number of stored composite requests. */
	if ( !flxActTransactionGetStoredRequestCount(pContext->hTransaction, &requestCount) )
	{
		flxCtuContextPrintLastApiError(pContext);
		return FLX_ACT_FALSE;
	}

	/* Iterate over the requests - if one with the specified sequence number is found, delete it. */
	for (requestIndex = 0; requestIndex < requestCount; requestIndex++)
	{
		/* Load the request. */
		if ( !flxActTransactionLoadStoredRequest(pContext->hTransaction, 
												 requestIndex,
												 &hTranRequest) )
		{
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
		}

		/* If it is the right request, delete it. */
		if (sHasThisSequenceNo(hTranRequest, pszSequenceNo))
		{
			if ( sDeleteCurrentRequest(pContext) )
				break; /* OK */
			else
				return FLX_ACT_FALSE;
		}
	}

	/* Error if the request was not found. */
	if (requestIndex == requestCount)
	{
		flxCtuPrintError("no stored composite request with sequence number %s\n", pszSequenceNo);
		return FLX_ACT_FALSE;
	}
	return FLX_ACT_TRUE;		
}

/***************************************************************************************************
*	flxCtuDoCommandListRequests
*
* Process a list command - list stored composite requests.
* For command line usage see above.
***************************************************************************************************/
FlxActBool flxCtuDoCommandListRequests(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	uint32_t			requestCount	= 0;
	uint32_t			requestIndex	= 0;
	uint32_t			listedCount		= 0;
	FlxActBool			bPrintHeading	= FLX_ACT_TRUE;
	FlxCtuListType		listType		= sGetListType(pCommand);
	FlxCtuRequestFilter	filter;
	FlxActTranRequest   hTranRequest;

	/* Set the filtering choices and list type from the command value and attributes. */
	sSetRequestFilter(pCommand, &filter);
	listType = sGetListType(pCommand);
	
	/* Get the number of stored composite requests. */
	if ( !flxActTransactionGetStoredRequestCount(pContext->hTransaction, &requestCount) )
	{
		flxCtuContextPrintLastApiError(pContext);
		return FLX_ACT_FALSE;
	}

	/* Iterate over the requests, listing those that the filters allow. */
	for (requestIndex = 0; requestIndex < requestCount; requestIndex++)
	{
		if ( !flxActTransactionLoadStoredRequest(pContext->hTransaction, 
												 requestIndex, 
												 &hTranRequest) )
		{
			FLX_CTU_ASSERT(FLX_ACT_FALSE);
		}

		if (sIsListFilterPass(hTranRequest, &filter))
		{
			if (listType == flxCtuListTypeLong)
			{
				FlxActTranResponse hTranResponse;

				/* List the request details. */
				sListRequestLong(hTranRequest);

				/* If there is a stored response for this request, list that too. */
				if (flxActTransactionGetResponse(pContext->hTransaction, &hTranResponse))
				{
					flxCtuPrintMessage("   Response:\n");
					flxCtuResponsePrint(pContext, hTranResponse);
				}
				else
				{	/* Check the the error is expected. */
					if ( FLX_ACT_TRAN_ERR_RESPONSE_NOT_FOUND != flxActTransactionGetResult())
						FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
				}
			}
			else if (listType == flxCtuListTypeXml)
			{
				sListRequestXml(hTranRequest);
			}
			else
			{
				flxCtuListRequestShort(hTranRequest, bPrintHeading);
			}

			bPrintHeading = FLX_ACT_FALSE;	/* First time only. */
			listedCount++;
		}
	}

	if (requestCount == 0)
		flxCtuPrintMessage("\nNo stored composite requests.\n");
	else
		flxCtuPrintMessage("\nListed %d of %d composite requests.\n", listedCount, requestCount);
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuGetRequestBySequenceNo
*
* Returns the stored request with the specified sequence number (by iterating through all stored
* requests).
***************************************************************************************************/
FlxActBool flxCtuGetRequestBySequenceNo(FlxCtuContext*		pContext, 
										const char*			pszSequenceNo, 
										FlxActTranRequest*	phTranRequest)
{
	uint32_t			requestCount	= 0;
	uint32_t			requestIndex	= 0;
	FlxCtuRequestFilter	filter;
	FlxActTranRequest   hTranRequest;

	/* Initialise the filter for the requested sequence number. */
	filter.bIsAllSeqNos		= FLX_ACT_FALSE;
	filter.pszSeqNoFilter	= pszSequenceNo;

	/* Get the number of stored composite requests. */
	if ( !flxActTransactionGetStoredRequestCount(pContext->hTransaction, &requestCount) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	/* Iterate over the requests, checking for the requested sequence number. */
	for (requestIndex = 0; requestIndex < requestCount; requestIndex++)
	{
		if ( !flxActTransactionLoadStoredRequest(pContext->hTransaction, 
												 requestIndex, 
												 &hTranRequest) )
		{
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
		}

		if (sIsListFilterPass(hTranRequest, &filter))
			break;	/* Found it. */
	}

	/* If request found, return it; else error. */
	if (requestIndex < requestCount)
	{
		*phTranRequest = hTranRequest;
		return FLX_ACT_TRUE;
	}
	return FLX_ACT_FALSE;
}

/***************************************************************************************************
*	flxCtuListRequestShort
*
* Print one-line details of a single request.
***************************************************************************************************/
void flxCtuListRequestShort(FlxActTranRequest hTranRequest, FlxActBool bPrintHeading)
{
	const char* pszSequenceNo    = NULL;
	const char* pszTimeGenerated = NULL;
	const char* pszReference	 = NULL;
	char	    pszDate[12];
	char	    pszTime[12];

	if ( !flxActTranRequestGetAttribute(hTranRequest, FLX_ACT_TRAN_REQ_SEQUENCE_NO, &pszSequenceNo) )
		FLX_CTU_ASSERT(FLX_ACT_FALSE);
	if ( !flxActTranRequestGetAttribute(hTranRequest, FLX_ACT_TRAN_REQ_TIME_GENERATED, &pszTimeGenerated) )
		FLX_CTU_ASSERT(FLX_ACT_FALSE);
	if ( !flxActTranRequestGetAttribute(hTranRequest, FLX_ACT_TRAN_REQ_REFERENCE, &pszReference) )
		FLX_CTU_ASSERT(FLX_ACT_FALSE);
	
	sscanf(pszTimeGenerated, "%10sT%5s", pszDate, pszTime);

	if (bPrintHeading)
	{
		flxCtuPrintMessage(" SeqNo Status     Date       Time  Reference\n"); 
	}

	flxCtuPrintMessage("%6.6s %-10.10s %10.10s %5.5s \"%s\"\n",
					  pszSequenceNo,
					  sGetRequestStatusString(hTranRequest),
					  pszDate,
					  pszTime,
					  pszReference
				     );
}

/***************************************************************************************************
*	flxCtuLoadStoredRequest
*
* Loads the request specified by the CMD_ATTR_REQUEST attribute of the command, or the most recent 
* request if the attribute is not present. 
***************************************************************************************************/
FlxActBool flxCtuLoadStoredRequest(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	const char*				pszRequestSeqNo;
	FlxActTranRequest		hTranRequest;
	FlxActTranRequestStatus status;


	if ( flxCtuCommandHasAttribute(pCommand, CMD_ATTR_REQUEST) )
	{	
		/* Use request with specified sequence number, */
		pszRequestSeqNo = flxCtuCommandGetAttribute(pCommand, CMD_ATTR_REQUEST);
		if ( !flxCtuGetRequestBySequenceNo(pContext, pszRequestSeqNo, &hTranRequest) )
		{
			flxCtuPrintError("Request with sequence number %s does not exist.\n", pszRequestSeqNo);
			return FLX_ACT_FALSE;
		}
		flxCtuPrintMessage("Loaded stored request %s\n", pszRequestSeqNo);
	}
	else
	{	/* Use most recent request. */
		uint32_t requestCount = 0;
		if ( !flxActTransactionGetStoredRequestCount(pContext->hTransaction, &requestCount) )
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		if (requestCount == 0)
		{
			flxCtuPrintError("No stored requests.\n");
			return FLX_ACT_FALSE;
		}

		/* The last request is the most recent - it has index (requestCount - 1). */
		if ( !flxActTransactionLoadStoredRequest(pContext->hTransaction, 
			(requestCount - 1), 
			&hTranRequest) )
		{
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
		}
		pszRequestSeqNo = sGetSequenceNoOfRequest(hTranRequest);
		flxCtuPrintMessage("Loaded stored request %s (latest)\n", pszRequestSeqNo);
	}

	/* The request has been loaded and is now the current request for the transaction.
	   Check its status to see if it us usable. */
	if ( !flxActTranRequestGetStatus(hTranRequest, &status) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (status != FLX_ACT_TRAN_REQ_STATUS_PENDING)
	{
		flxCtuPrintError("Stored request %s cannot be used - status is not PENDING.\n", 
						 sGetSequenceNoOfCurrentRequest(pContext));
		return FLX_ACT_FALSE;
	}

	pContext->bUsingStoredRequest = FLX_ACT_TRUE;

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sDeleteCurrentRequest
*
* Delete the current request for the transaction.
***************************************************************************************************/
static FlxActBool sDeleteCurrentRequest(FlxCtuContext* pContext)
{
	const char*	pszSequenceNo = sGetSequenceNoOfCurrentRequest(pContext);
	flxCtuPrintMessage("Deleting composite request with sequence number %s...", pszSequenceNo);

	/* This could fail if another process has deleted it (very) recently. */
	if ( flxActTransactionDeleteStoredRequest(pContext->hTransaction) )
	{
		flxCtuPrintMessage(" OK\n");
		return FLX_ACT_TRUE;
	}
	else
	{
		flxCtuPrintMessage(" ** FAILED ** \n");
		flxCtuContextPrintLastApiError(pContext);
		return FLX_ACT_FALSE;
	}
}

/***************************************************************************************************
*	sGetListType
*
* Return the list format specified by the command attributes.
***************************************************************************************************/
static FlxCtuListType sGetListType(const FlxCtuCommand* pCommand)
{
	FlxCtuListType viewType = flxCtuListTypeShort;
	if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_FORMAT))
	{
		if (flxCtuCommandAttributeIs(pCommand, CMD_ATTR_FORMAT, "long"))
			viewType = flxCtuListTypeLong;
		else if (flxCtuCommandAttributeIs(pCommand, CMD_ATTR_FORMAT, "xml"))
			viewType = flxCtuListTypeXml;
	}
	return viewType;
}

/***************************************************************************************************
 *	sGetRequestStatusString
 *
 * Return a string describing the request status.
 ***************************************************************************************************/
static const char* sGetRequestStatusString(FlxActTranRequest hTranRequest)
{
	FlxActTranRequestStatus status;

	if ( !flxActTranRequestGetStatus(hTranRequest, &status) )
	{	/* Should not fail if hTransRequest is valid and there is a current request. */
		FLX_CTU_ASSERT(FLX_ACT_FALSE);
	}

	switch (status)
	{
	case FLX_ACT_TRAN_REQ_STATUS_PENDING:		/* Waiting for a response.						*/
												return "Pending";
	case FLX_ACT_TRAN_REQ_STATUS_UNTRUSTED:		/* Trusted storage break since request stored.	*/
												return "Untrusted";
	case FLX_ACT_TRAN_REQ_STATUS_COMPLETE:		/* Response processed successfully.				*/
												return "Complete";

	/* This status should never be set for saved requests/ */
	case FLX_ACT_TRAN_REQ_STATUS_CREATING:		/* Created but not yet saved to trusted storage */
												/* ... should never be set for saved requests.  */
												return "Creating???";
	default:									/* Unknown status.								*/
												return "????????";
	}
}

/***************************************************************************************************
*	sGetSequenceNoOfCurrentRequest
*
* Return the sequence number of the current request for the transaction.
***************************************************************************************************/
static const char* sGetSequenceNoOfCurrentRequest(FlxCtuContext* pContext)
{
	FlxActTranRequest	hTranRequest;
	const char*			pszRequestSequenceNo = NULL;

	if ( !flxActTransactionGetRequest(pContext->hTransaction, &hTranRequest) )
	{
		/* Will not fail if hTransaction is valid and there is a current request. */ 
		FLX_CTU_ASSERT(FLX_ACT_FALSE);
	}

	if ( !flxActTranRequestGetAttribute(hTranRequest, 
										FLX_ACT_TRAN_REQ_SEQUENCE_NO, 
										&pszRequestSequenceNo) )
	{
		/* Will not fail if hTranRequest is valid. */
		FLX_CTU_ASSERT(FLX_ACT_FALSE);
	}
	return pszRequestSequenceNo;
}

/***************************************************************************************************
*	sGetSequenceNoOfRequest
*
* Return the sequence number of the supplied request.
***************************************************************************************************/
static const char* sGetSequenceNoOfRequest(FlxActTranRequest hTranRequest)
{
	const char* pszRequestSequenceNo    = NULL;

	if ( !flxActTranRequestGetAttribute(hTranRequest, 
		FLX_ACT_TRAN_REQ_SEQUENCE_NO, 
		&pszRequestSequenceNo) )
	{
		FLX_CTU_ASSERT(FLX_ACT_FALSE);
	}
	return pszRequestSequenceNo;
}

/***************************************************************************************************
*	sHasThisSequenceNo
*
* Return FLX_ACT_TRUE if the sequence number matches that of the supplied current request.
***************************************************************************************************/
static FlxActBool sHasThisSequenceNo(FlxActTranRequest hTranRequest, const char* pszSequenceNo)
{
	const char* pszRequestSequenceNo = sGetSequenceNoOfRequest(hTranRequest);

	return (strcmp(pszSequenceNo, pszRequestSequenceNo) == 0)? FLX_ACT_TRUE : FLX_ACT_FALSE;
}

/***************************************************************************************************
*	sIsListFilterPass
*
* Return FLX_ACT_TRUE if the request passes the filter criteria.
***************************************************************************************************/
static FlxActBool sIsListFilterPass(FlxActTranRequest hTranRequest, const FlxCtuRequestFilter* pFilter)
{
	FlxActBool bIsPass = FLX_ACT_FALSE;

	if (pFilter->bIsAllSeqNos || sHasThisSequenceNo(hTranRequest, pFilter->pszSeqNoFilter))
	{
		bIsPass = FLX_ACT_TRUE;
	}
	return bIsPass;
}

/***************************************************************************************************
*	sListNameAndValue
*
* Output name:  value
***************************************************************************************************/
static void sListNameAndValue(const char* pszName, const char* pszValue)
{
	flxCtuPrintMessage("%20s : %s\n", 
		pszName,
		(pszValue == NULL)? "" : pszValue
		);
}

/***************************************************************************************************
*	sListRequestLong
*
* Print request details in long (multi-line) format.
***************************************************************************************************/
void sListRequestLong(FlxActTranRequest hTranRequest)
{
	const char* pszSequenceNo    = NULL;
	const char* pszTimeGenerated = NULL;
	const char* pszReference	 = NULL;
	
	if ( !flxActTranRequestGetAttribute(hTranRequest, FLX_ACT_TRAN_REQ_SEQUENCE_NO, &pszSequenceNo) )
		FLX_CTU_ASSERT(FLX_ACT_FALSE);
	if ( !flxActTranRequestGetAttribute(hTranRequest, FLX_ACT_TRAN_REQ_TIME_GENERATED, &pszTimeGenerated) )
		FLX_CTU_ASSERT(FLX_ACT_FALSE);
	if ( !flxActTranRequestGetAttribute(hTranRequest, FLX_ACT_TRAN_REQ_REFERENCE, &pszReference) )
		FLX_CTU_ASSERT(FLX_ACT_FALSE);

	flxCtuPrintMessage("---------------------------------------------------------------------\n");

	sListNameAndValue("Sequence number", pszSequenceNo);
	sListNameAndValue("Status",			 sGetRequestStatusString(hTranRequest));
	sListNameAndValue("Generated",		 pszTimeGenerated);
	sListNameAndValue("Reference",		 pszReference);
	flxCtuPrintMessage("\n");
}

/***************************************************************************************************
 *	sListRequestXml
 *
 * Output the raw XML for the request.
 *
 * No separators are output, so that the XML request can be piped if required.
 ***************************************************************************************************/
void sListRequestXml(FlxActTranRequest hTranRequest)
{
	const char* pszXml = NULL;

	if ( !flxActTranRequestGetXML(hTranRequest, &pszXml) )
		FLX_CTU_ASSERT(FLX_ACT_FALSE);

	flxCtuPrintMessage("%s\n", pszXml);
}

/***************************************************************************************************
*	sSetRequestFilter
*
* Set the filter criteria from the command value.
***************************************************************************************************/
static void sSetRequestFilter(const FlxCtuCommand* pCommand, FlxCtuRequestFilter* pFilter)
{
	pFilter->bIsAllSeqNos		= !flxCtuCommandHasValue(pCommand);

	pFilter->pszSeqNoFilter		= pFilter->bIsAllSeqNos? "" : flxCtuCommandGetValue(pCommand);
}
