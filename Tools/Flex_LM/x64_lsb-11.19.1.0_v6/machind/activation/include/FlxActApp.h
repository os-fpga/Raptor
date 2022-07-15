/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/**
 * FlxActApp include file
 *
 * The FlxActApp include file contains the object and function definitions
 * to be used as part of a client activation application.
 */

#ifndef FlxActApp_h

#define FlxActApp_h  1

#include "FlxActCommon.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * FlxActAppActivation related operations
 */
typedef struct flxActAppActivation *	FlxActAppActivation;

	/* initialization and cleanup functions */
	FlxActBool flxActAppActivationCreate(FlxActHandle handle, FlxActAppActivation *appAct);
	void flxActAppActivationDelete(FlxActAppActivation appAct);

    /* key method to send request to remote server and get back response */
	FlxActBool flxActAppActivationSend(FlxActAppActivation appAct, FlxActError *err);

	FlxActBool flxActAppActivationReqCreate(FlxActAppActivation appAct, char **xmlString);
	FlxActBool flxActAppActivationReqSet(FlxActAppActivation appAct, const char **xmlString);

	FlxActBool flxActAppActivationRespProcess(FlxActAppActivation appAct, const char *xmlString, FlxActBool * pbIsConfig);

    /* Get the FlxActProdLicSpc created or modified by the last activation response, if any.
     * Use the returned FlxActProdLicSpc for query only, changes will not be saved.
     */
	FlxActBool flxActAppActivationRespProdLicSpcGet(FlxActAppActivation act, FlxActProdLicSpc *pProdSpc);

	/* standard accessors */
	const char *flxActAppActivationEntitlementIdGet(FlxActAppActivation appAct);
	void flxActAppActivationEntitlementIdSet(FlxActAppActivation appAct, const char *entitlementId);

	const char *flxActAppActivationReasonGet(FlxActAppActivation appAct);
	FlxActBool flxActAppActivationReasonSet(FlxActAppActivation appAct, const char *reason);

	const char *flxActAppActivationProductIdGet(FlxActAppActivation appAct);
	void flxActAppActivationProductIdSet(FlxActAppActivation appAct, const char *productId);

	uint32_t flxActAppActivationDurationGet(FlxActAppActivation appAct);
	void flxActAppActivationDurationSet(FlxActAppActivation appAct, uint32_t duration);

	const char *flxActAppActivationExpDateGet(FlxActAppActivation appAct);
	void flxActAppActivationExpDateSet(FlxActAppActivation appAct, const char *expDate);

	void flxActAppActivationVendorDataSet(FlxActAppActivation appAct, const char * key, const char * value);
	const char * flxActAppActivationVendorDataGet(FlxActAppActivation appAct, const char * pszKey);

	FlxActBool
	flxActAppActivationShortCodeGenerate(
		FlxActAppActivation	appAct,
		const char *		pszASRFile,
		const char **		ppszShortCode);

	FlxActBool
	flxActAppActivationShortCodeGenerateFromBuffer(
		FlxActAppActivation	appAct,
		const char *		pszASRBuffer,
		const char **		pszShortCode);

	FlxActBool
	flxActAppActivationShortCodeCancel(
		FlxActAppActivation appAct,
		const char *		pszASRFile);

	FlxActBool
	flxActAppActivationShortCodeCancelFromBuffer(
		FlxActAppActivation appAct,
		const char *		pszASRBuffer);

	FlxActBool
	flxActAppActivationShortCodePending(
		FlxActAppActivation appAct,
        const char *		pszASRFile,
        const char **		ppszPendingShortCode);

	FlxActBool
	flxActAppActivationShortCodePendingFromBuffer(
		FlxActAppActivation appAct,
        const char *		pszASRBuffer,
        const char **		ppszPendingShortCode);

	FlxActBool
	flxActAppGetPendingShortCode(
		FlxActHandle handle,
		const char *	pszASRFile,
		const char **	ppszPendingShortCode,
		uint32_t * pRequestType);

	FlxActBool
	flxActAppGetPendingShortCodeFromBuffer(
		FlxActHandle handle,
		const char *	pszASRBuffer,
		const char **	ppszPendingShortCode,
		uint32_t * pRequestType);

typedef struct flxActAppReturn *	FlxActAppReturn;


	FlxActBool flxActAppReturnCreate(FlxActHandle handle, FlxActAppReturn * appRet);
	void flxActAppReturnDelete(FlxActAppReturn appRet);
	FlxActBool flxActAppReturnReqCreate(FlxActAppReturn	appRet, char ** xmlString);
	FlxActBool flxActAppReturnReqSet(FlxActAppReturn	appRet, const char ** xmlString);
	FlxActBool flxActAppReturnRespProcess(FlxActAppReturn appRet,const char * xmlString, FlxActBool * pbIsConfig);
	FlxActBool flxActAppReturnSend(FlxActAppReturn appRet, FlxActError * err);
	FlxActBool flxActAppReturnCancel(FlxActAppReturn appRet);
	const char *flxActAppReturnProductIdGet(FlxActAppReturn appRet);
	const char *flxActAppReturnSuiteIdGet(FlxActAppReturn appRet);
	const char *flxActAppReturnEntitlementIdGet(FlxActAppReturn appRet);

	const char *flxActAppReturnReasonGet(FlxActAppReturn appRet);
	FlxActBool flxActAppReturnReasonSet(FlxActAppReturn appRet, const char *reason);

	const char *flxActAppReturnFRIdGet(FlxActAppReturn appRet);
	void flxActAppReturnFRIdSet(FlxActAppReturn appRet, const char * pszID);

	void flxActAppReturnProdLicSpcSet(FlxActAppReturn appRet, FlxActProdLicSpc product);

	void flxActAppReturnVendorDataSet(
		FlxActAppReturn		ret,
		const char *		key,
		const char *		value);

	const char *flxActAppReturnVendorDataGet(FlxActAppReturn ret, const char * key);



	FlxActBool
	flxActAppReturnShortCodeGenerate(
		FlxActAppReturn		ret,
		const char *		pszASRFile,
		const char **		ppszShortCode);

	FlxActBool
	flxActAppReturnShortCodeGenerateFromBuffer(
		FlxActAppReturn		ret,
		const char *		pszASRBuffer,
		const char **		ppszShortCode);

	FlxActBool
	flxActAppReturnShortCodeCancel(
		FlxActAppReturn appRet,
        const char *	pszASRFile);

	FlxActBool
	flxActAppReturnShortCodeCancelFromBuffer(
		FlxActAppReturn appRet,
        const char *	pszASRBuffer);

	FlxActBool
	flxActAppReturnShortCodePending(
		FlxActAppReturn appRet,
		const char *	pszASRFile,
		const char **	ppszPendingShortCode);

	FlxActBool
	flxActAppReturnShortCodePendingFromBuffer(
		FlxActAppReturn appRet,
		const char *	pszASRBuffer,
		const char **	ppszPendingShortCode);



typedef struct flxActAppRepair *	FlxActAppRepair;


	FlxActBool flxActAppRepairCreate(FlxActHandle handle, FlxActAppRepair * appRep);
	void flxActAppRepairDelete(FlxActAppRepair appRep);
	FlxActBool flxActAppRepairSend(FlxActAppRepair	appRep, FlxActError * err);
	FlxActBool flxActAppRepairReqCreate(FlxActAppRepair	appRep, char **	xmlString);
	FlxActBool flxActAppRepairReqSet(FlxActAppRepair	appRep, const char **	xmlString);
	FlxActBool flxActAppRepairRespProcess(FlxActAppRepair appRep, const char *	xmlString, FlxActBool * pbIsConfig);
	const char *flxActAppRepairProductIdGet(FlxActAppRepair appRep);
	const char *flxActAppRepairSuiteIdGet(FlxActAppRepair appRep);
	const char *flxActAppRepairEntitlementIdGet(FlxActAppRepair appRep);

	const char *flxActAppRepairFRIdGet(FlxActAppRepair appRep);
	void flxActAppRepairFRIdSet(FlxActAppRepair appRep, const char * pszID);

	void flxActAppRepairProdLicSpcSet(FlxActAppRepair appRep, FlxActProdLicSpc product);

	void
	flxActAppRepairVendorDataSet(
		FlxActAppRepair		repair,
		const char *		key,
		const char *		value);

	const char *flxActAppRepairVendorDataGet(FlxActAppRepair repair, const char * key);

	FlxActBool
	flxActAppRepairShortCodeGenerate(
		FlxActAppRepair		repair,
		const char *		pszASRFile,
		const char **		ppszShortCode);

	FlxActBool
	flxActAppRepairShortCodeGenerateFromBuffer(
		FlxActAppRepair		repair,
		const char *		pszASRBuffer,
		const char **		ppszShortCode);

	FlxActBool
	flxActAppRepairShortCodeCancel(
		FlxActAppRepair appRep,
		const char *	pszASRFile);

	FlxActBool
	flxActAppRepairShortCodeCancelFromBuffer(
		FlxActAppRepair appRep,
		const char *	pszASRBuffer);

	FlxActBool
	flxActAppRepairShortCodePending(
		FlxActAppRepair appRep,
		const char *	pszASRFile,
		const char **	ppszPendingShortCode);

	FlxActBool
	flxActAppRepairShortCodePendingFromBuffer(
		FlxActAppRepair appRep,
		const char *	pszASRBuffer,
		const char **	ppszPendingShortCode);


#ifdef __cplusplus
}
#endif

#endif



