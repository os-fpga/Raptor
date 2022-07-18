/**************************************************************************************************
* Copyright (c) 1997-2018, 2021 Flexera. All Rights Reserved.
**************************************************************************************************/

/**
 * FlxActCommon include file
 *
 * The FlxActCommon include file contains the function definitions for those
 * functions which are common to both the client and the server activation
 * applications.
 */

#ifndef FlxActCommon_h

#define FlxActCommon_h  1

#include "FlxActTypes.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Setup the FlxActMode enumerated type. Used to specify the mode of operation when the
 * handle is created and initialized. 
 * An application that is prepped "server" (the <IsServer> parameter of the prep XML) can
 * operate in APP or SVR mode.  Otherwise is can only operate in APP mode.
 * FLX_ACT_PREP sets the mode to the prep mode.
 */
typedef enum flxActMode
{
	FLX_ACT_APP, FLX_ACT_SVR, FLX_ACT_PREP
} FlxActMode;


/**
 *	Reason for error response
 */
typedef enum flxErrorResponseReason
{
	flxErrorResponseReasonUnknown = 0,
	flxErrorResponseReasonError,
	flxErrorResponseReasonDenied
} FlxErrorResponseReason;

/**
 *	Unique Machine Number types
 */
typedef enum flxUMNType
{
	flxUMNTypeOne,
	flxUMNTypeTwo,
	flxUMNTypeThree,	/* Virtual environments only		*/
	flxUMNTypeFour,		/* Deprecated */
	flxUMNTypeFive		/* Generation ID - Virtual environments & currently Windows only */
} FlxUMNType;

/**
 * Setup the FlxActCountType enumerated type. Used to specify the different kinds of license
 * counts possible for each product
 */
typedef enum flxActCountType
{
	CONCURRENT,
	ACTIVATABLE,
	HYBRID,
	CONCURRENT_OD,
	ACTIVATABLE_OD,
	HYBRID_OD,
	REPAIRS
} FlxActCountType;

/**
 *	Enumeration for fulfillment types
 */
typedef enum flxFulfillmentType
{
	FULFILLMENT_TYPE_UNKNOWN = 0,
	FULFILLMENT_TYPE_TRIAL = 1,
	FULFILLMENT_TYPE_SHORTCODE,
	FULFILLMENT_TYPE_EMERGENCY,
	FULFILLMENT_TYPE_SERVED_ACTIVATION,				/*	From a vendor daemon */
	FULFILLMENT_TYPE_SERVED_OVERDRAFT_ACTIVATION,	/*	From a vendor daemon */
	FULFILLMENT_TYPE_PUBLISHER_ACTIVATION,
	FULFILLMENT_TYPE_PUBLISHER_OVERDRAFT_ACTIVATION
} FlxFulfillmentType;

/**
 *	Enumeration for deducion types
 */
typedef enum flxActDeductionType
{
	DEDUCTION_TYPE_UNKNOWN    = 0,
	DEDUCTION_TYPE_ACTIVATION = 1,
	DEDUCTION_TYPE_TRANSFER   = 2,
	DEDUCTION_TYPE_REPAIR     = 3
} FlxActDeductionType;

typedef enum flxActCommType
{
	flxCommsNone     = 0,
	flxCommsMvsnFlex = 1,
	flxCommsMvsnSoap = 4
} FlxActCommType;

/*
 * Status change callback definition
 * SSC - Should be defined in one place.  Currently it is defined in fnptypes.h and here
 */
typedef uint32_t(*FLX_STATUS_CALLBACK)(const void* pUserData, uint32_t statusOld, uint32_t statusNew);

/*
 * Status related defs.
 * SSC - They are also defined in CommsDefs.h.  Should reuse the same header
 */
typedef enum flxCommsStatusCodes {
	flxCommsStatusError = 0,
	flxCommsStatusSuccess,
	flxCommsStatusNotSet,
	flxCommsStatusCancelledByUser,
    flxCommsStatusCreatingRequest,
    flxCommsStatusRequestCreated,
	flxCommsStatusContextCreated,
	flxCommsStatusConnected,
	flxCommsStatusRequestSent,
	flxCommsStatusPolingForResponse,
	flxCommsStatusWaitingForResponse,
	flxCommsStatusDone
} flxCommsStatusCodes;

typedef enum flxActCallbackReturn {
	flxCallbackReturnContinue = 0,
	flxCallbackReturnCancelRequest
} flxActCallbackReturn;


/*
 *	Initialization/cleanup routine
 *
 *	See FlxInit.h for return values.
 */
int	flxActCommonLibraryInit(const char * runtimepath);
void flxActCommonLibraryCleanup();

/**
 * Define the FlxActHandle and related operations
 */
typedef struct flxActHandle * FlxActHandle;

	/* get protection mode (application or server) */
	FlxActMode flxActCommonGetProtectionMode();

	/* initialization */
	FlxActBool flxActCommonHandleOpen(FlxActHandle *handle, FlxActMode mode, FlxActError *err);

	/* cleanup */
	FlxActBool flxActCommonHandleClose(FlxActHandle handle);

	/* activation libraries version and build information*/
	FlxActBool flxActCommonHandleGetVersion(FlxActHandle handle, 
											uint32_t*	 pMajor, 
											uint32_t*	 pMinor, 
											uint32_t*	 pMaint, 
											uint32_t*	 pHotfix);

	FlxActBool flxActCommonHandleGetBuildInfo(	FlxActHandle handle, 
												uint32_t*	 pBuildNo,		
												uint32_t*	 pBuildYear, 
												uint32_t*	 pBuildMonth, 
												uint32_t*	 pBuildDay);

	/* licensing service version and build information*/
	FlxActBool flxActCommonHandleGetServiceVersion(FlxActHandle handle, 
												   uint32_t*	pMajor, 
												   uint32_t*	pMinor, 
												   uint32_t*	pMaint, 
												   uint32_t*	pHotfix);

	FlxActBool flxActCommonHandleGetServiceBuildInfo(	FlxActHandle handle, 
														uint32_t*	 pBuildNo,		
														uint32_t*	 pBuildYear, 
														uint32_t*	 pBuildMonth, 
														uint32_t*	 pBuildDay);

	/* get back error code for last operation performed using handle */
	void flxActCommonHandleGetError(FlxActHandle handle, FlxActError *err);

	/* get back additional error info from operations server */
	uint32_t flxActCommonHandleGetLastOpsError(FlxActHandle handle);
	const char * flxActCommonHandleGetLastOpsErrorString(FlxActHandle handle);

	/* get last error code from last processed error response */
	uint32_t flxActCommonHandleGetLastResponseError(FlxActHandle handle);
	const char *flxActCommonHandleGetLastResponseErrorString(FlxActHandle handle);
	FlxErrorResponseReason flxActCommonHandleGetLastResponseErrorReason(FlxActHandle handle);

	/* UMN */
	const char * flxActCommonHandleGetUniqueMachineNumber(FlxActHandle handle, FlxUMNType type);


	/* set the remote server to be used for subsequent operations */
	FlxActBool flxActCommonHandleSetRemoteServer(FlxActHandle handle, const char *targetServer);

	const char * flxActCommonHandleGetRemoteServer(FlxActHandle handle);

	/* which comm to use */
	FlxActBool flxActCommonHandleSetCommType(FlxActHandle handle, FlxActCommType commType);

	FlxActCommType flxActCommonHandleGetCommType(FlxActHandle handle);

	/* timeout */
	FlxActBool flxActCommonHandleSetTimeout(FlxActHandle handle, uint32_t timeout);

	/* poll interval */
	FlxActBool flxActCommonHandleSetPollInterval(FlxActHandle handle, uint32_t pollInterval);

	/* Soap specific target details */
	/* proxy details */
	FlxActBool flxActCommonHandleSetProxyDetails(FlxActHandle handle, uint32_t port, const char* pHost, const char* pUserid, const char* pPasswd);

	/* SSL details */
	FlxActBool flxActCommonHandleSetSSLDetails(FlxActHandle handle, const char* pCacert, const char* pCapath);

	/* status callback */
	FlxActBool flxActCommonHandleSetStatusCallback(FlxActHandle handle, FLX_STATUS_CALLBACK callback, const void* pCallerData);

	/* delete specified product from all of trusted storage */
	FlxActBool flxActCommonHandleDeleteProduct(FlxActHandle handle, const char * pszProductId);

	FlxActBool flxActCommonRepairLocalTrustedStorage(FlxActHandle handle);

/**
 * Define the FlxActLicSpc and related operations. This represents the Flex Activation
 * License specification and the operations related to the population and retrieval of
 * defined license counts for products
 */
typedef struct flxActLicSpc * FlxActLicSpc;
typedef struct flxActProdLicSpc * FlxActProdLicSpc;
typedef struct flxActDedSpc * FlxActDedSpc;

	/* initialization */
	FlxActBool flxActCommonLicSpcCreate(FlxActHandle handle, FlxActLicSpc *licSpc);

	/* cleanup */
	void flxActCommonLicSpcDelete(FlxActLicSpc licSpc);

	FlxActBool flxActCommonLicSpcPopulateFromTS(FlxActLicSpc licSpc);
	FlxActBool flxActCommonLicSpcPopulateAllFromTS(FlxActLicSpc licSpc);

	/* 
	 * Retrieves trusted and total fulfillment record count from server trusted storage. 
	 */
	FlxActBool flxActCommonGetFRCountFromServerTS(  FlxActHandle handle,
													const char *pszEntitlementID, 
													const char *pszProductID,
													const char *pszServer,
													uint32_t  *pnTrustedFRCount,
													uint32_t  *pnTotalFRCount);

	/* 
	 * Retrieves trusted fulfillment record(s) from server-trusted storage into the 
	 * license specification structure. The fulfillment records obtained with this API 
	 * will not have all fields populated. 
	 * Use flxActCommonProdLicSpcPopulateFRFromServerTS API to obtain the entire fulfillment record.
	 */
	FlxActBool flxActCommonLicSpcPopulateFromServerTS(FlxActLicSpc licSpc, 
														const char* pszEntitlementID, 
														const char* pszProductID, 
														const char* pszServer);

	/* 
	 * Retrieves all (e.g. trusted, expired, non-expired, untrusted) fulfillment records
	 * from server-trusted storage into the license specification structure. The fulfillment records 
	 * obtained with this API will not have all fields populated. 
	 * Use flxActCommonProdLicSpcPopulateFRFromServerTS API to obtain the entire fulfillment record.
	 */
	FlxActBool flxActCommonLicSpcPopulateAllFromServerTS(FlxActLicSpc licSpc, 
															const char* pszEntitlementID, 
															const char* pszProductID, 
															const char* pszServer);

	/* 
	 * Retrieve all details for the fulfillment record from server trusted storage. The fulfillmentId from the 
	 * supplied product license specification will be used in determining the fulfillment record that
	 * should get retrieved on the server trusted storage. The product license specification should get 
	 * populated by calling flxActCommonLicSpcPopulateFromServerTS or 
	 * flxActCommonLicSpcPopulateAllFromServerTS API.
	 */
	FlxActBool flxActCommonProdLicSpcPopulateFRFromServerTS(FlxActProdLicSpc prodSpcPtr, 
																const char *pszServer);

	/* 
	 * Local activation and reset from one or more Activation Specification Record (*.asr) files. 
	 *
	 * The recommended use is to call these functions specifying the path of the ASR file;
	 * if there is more than one ASR, call the function multiple times.
	 *
	 * The option of specifying a directory is supported, in which case the function attempts 
	 * to add all ASRs in the directory.  This use is not recommended because if one or more 
	 * ASRs fail to load there is no way to find out which and why.
	 * 
	 * Reset is provided for development testing - the ASR must have a non-zero 
	 * PUBLISHER_TRIAL_RESET value.
	 *
	 * Returns	FLX_ACT_TRUE  if one or more ASRs have been added/reset.
	 *			FLX_ACT_FALSE otherwise - call flxActCommonHandleGetError()
	 */
	FlxActBool flxActCommonLicSpcAddASRs(FlxActLicSpc licSpc, const char *fileOrDirPath);
	FlxActBool flxActCommonResetTrialASRs(FlxActLicSpc licSpc, const char *fileOrDirPath);

	/* 
	 * Local activation and reset from an Activation Specification Record in a buffer.
	 */
	FlxActBool flxActCommonLicSpcAddASRFromBuffer(FlxActLicSpc licSpc, const char * pszBuffer);
	FlxActBool flxActCommonResetTrialASRFromBuffer(FlxActLicSpc licSpc, const char * pszBuffer);

	/* 
	 * Check the status of a SINGLE Activation Specification Record in a file or buffer.
	 * These functions can be used to determine whether the ASR has already been added.
	 * If the trial anchor exists (*pTrialId != 0 && *pStartTime != 0) but no FlxProdLicSpc
	 * is added, the fullfillment record (or the trusted storage file) has been deleted.
	 */
	FlxActBool flxActCommonLicSpcCheckASR(	
		FlxActLicSpc	licSpc,			/* If the ASR's FR already exists in TS, a FlxProdLicSpc will
										   be added to this FlxActLicSpc */
		const char*		pszFilePath,	/* The path of the file containing the ASR */
		uint32_t*		pTrialId,		/* On success, the TrialId from the ASR is returned here
									       (0 if there is no TrialId) */
		time_t*			pStartTime);	/* If the TrialId != 0, the UTC time_t of midnight of the 
										   day on which the anchor was written is returned here. 
										   If the anchor is not present, 0 is returned. */ 

	FlxActBool flxActCommonLicSpcCheckASRFromBuffer(	
		FlxActLicSpc	licSpc,			/* If the ASR's FR already exists in TS, a FlxProdLicSpc will
										   be added to this FlxActLicSpc */
		const char*		pszAsrXml,		/* The ASR */
		uint32_t*		pTrialId,		/* On success, the TrialId from the ASR is returned here
										   (0 if there is no TrialId) */
		time_t*			pStartTime);	/* If the TrialId != 0, the UTC time_t of midnight of the 
										   day on which the anchor was written is returned here 
										   If the anchor is not present, 0 is returned. */ 

	uint32_t flxActCommonLicSpcGetNumberProducts(FlxActLicSpc licSpc);
	FlxActProdLicSpc flxActCommonLicSpcGet(FlxActLicSpc licSpc, uint32_t index);

	uint32_t flxActCommonLicSpcGetNumProdEntries(FlxActLicSpc licSpc, const char *productId);
	FlxActProdLicSpc flxActCommonLicSpcGetProd(FlxActLicSpc licSpc, const char *productId, uint32_t index);

	FlxActBool flxActCommonLicSpcProductDelete(FlxActLicSpc licSpc, FlxActProdLicSpc product);

    /* Get the FlxActProdLicSpc created or modified by the last transaction response, if any.
     * This function takes a FlxActHandle rather than transaction object such as FlxActAppActivation
     * because only the last response is available (not one per transaction type). 
     * For convenience, the function flxActAppActivationRespProdLicSpcGet is provided - it just calls
     * this function with the FlxActHandle that was used to create the FlxActAppActivation object.
     * Use the returned FlxActProdLicSpc for query only, changes will not be saved.
     */
    FlxActBool flxActCommonRespProdLicSpcGet(FlxActHandle handle, FlxActProdLicSpc *pProdSpc);

   /* 
    * Enable the FR behind a FlxActProdLicSpc.
    */

    FlxActBool flxActCommonProdLicSpcEnable(FlxActProdLicSpc prodSpc);

/**
 * Object to define license specification attributes for a particular product
 */
	const char *flxActCommonProdLicSpcProductIdGet(FlxActProdLicSpc prodSpc);
	uint32_t flxActCommonProdLicSpcCountGet(FlxActProdLicSpc prodSpc, FlxActCountType countType);
	const char *flxActCommonProdLicSpcEntitlementIdGet(FlxActProdLicSpc prodSpc);
	const char *flxActCommonProdLicSpcFulfillmentIdGet(FlxActProdLicSpc prodSpc);
	const char *flxActCommonProdLicSpcExpDateGet(FlxActProdLicSpc prodSpc);
	const char *flxActCommonProdLicSpcSuiteIdGet(FlxActProdLicSpc prodSpc);
	const char * flxActCommonProdLicSpcFeatureLineGet(FlxActProdLicSpc prodSpc);
	uint32_t flxActCommonProdLicSpcTrustFlagsGet(FlxActProdLicSpc prodSpc);
	const char *flxActCommonProdLicSpcActServerChainGet(FlxActProdLicSpc prodSpc);
	FlxFulfillmentType flxActCommonProdLicSpcFulfillmentTypeGet(FlxActProdLicSpc prodSpc);
	FlxActBool flxActCommonProdLicSpcIsDisabled(FlxActProdLicSpc prodSpc);

	FlxActBool flxActCommonProdLicSpcVendorDataGetCount(  FlxActProdLicSpc	prodSpc,
														  uint32_t*			pCount);
    FlxActBool flxActCommonProdLicSpcVendorDataGetByIndex(FlxActProdLicSpc	prodSpc, 
														  uint32_t			index,
														  const char**		ppKey,
														  const char**		ppValue);
    FlxActBool flxActCommonProdLicSpcVendorDataGetByKey(  FlxActProdLicSpc	prodSpc, 
														  const char*		pKey,
														  const char**		ppValue);

    /* This property of a license specification is guaranteed to be unique 
     * within a given trusted storage.
     */
    const char *flxActCommonProdLicSpcUniqueIdGet(FlxActProdLicSpc prodSpc);

	/* deductions */
	uint32_t		flxActCommonProdLicSpcNumberDedSpcGet(FlxActProdLicSpc prodSpc);
	FlxActDedSpc	flxActCommonProdLicSpcDedSpcGet(FlxActProdLicSpc prodSpc, uint32_t index);

	uint32_t		flxActCommonProdLicSpcNumberValidDedSpcGet(FlxActProdLicSpc prodSpc);

	const char *		flxActCommonDedSpcDestinationFulfillmentIdGet(FlxActDedSpc dedSpc);
	const char *		flxActCommonDedSpcDestinationSystemNameGet	(FlxActDedSpc dedSpc);
	const char *		flxActCommonDedSpcExpDateGet				(FlxActDedSpc dedSpc);
	FlxActDeductionType flxActCommonDedSpcTypeGet					(FlxActDedSpc dedSpc);
	uint32_t			flxActCommonDedSpcCountGet(FlxActDedSpc dedSpc, FlxActCountType countType);

	/* Virtualization API */
	FlxActBool flxActCommonVirtualStatusGet(FlxActHandle handle, FlxActBool* pbIsVirtual);
	FlxActBool flxActCommonVirtualFamilyGet(FlxActHandle handle, const char** pszFamily);
	FlxActBool flxActCommonVirtualNameGet(FlxActHandle handle, const char** pszName);
	FlxActBool flxActCommonVirtualUuidGet(FlxActHandle handle, const char** pszUuid);
	FlxActBool flxActCommonVirtualGenidGet(FlxActHandle handle, const char** pszGenid);

	/*
	 *	Trust flags
	 */
	#define	FLAGS_TRUST_RESTORE		0x1		/* The restore trust (1 = trusted, 0 = untrusted) */
	#define	FLAGS_TRUST_HOST		0x2		/* The host trust (1 = trusted, 0 = untrusted) */
	#define FLAGS_TRUST_TIME		0x4		/* The time trust (1 = trusted, 0 = untrusted) */
	#define FLAGS_FULLY_TRUSTED		(FLAGS_TRUST_RESTORE | FLAGS_TRUST_HOST | FLAGS_TRUST_TIME)

	/* Date validation */
	FlxActBool flxActCommonValidateDateString(const char* pszDate, FlxActBool* pbIsValid);

#ifdef __cplusplus
}
#endif

#endif


