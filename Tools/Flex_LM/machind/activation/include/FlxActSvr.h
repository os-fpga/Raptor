/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/**
 * FlxActServer include file
 *
 * The FlxActServer include file contains the object and function definitions
 * to be used as part of a client activation application.
 */

#ifndef FlxActSvr_h

#define FlxActSvr_h  1

#include "FlxActCommon.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * FlxActSvrActivation related operations
 */
typedef struct flxActSvrActivation * FlxActSvrActivation;

	/* initialization and cleanup functions */
FlxActBool
flxActSvrActivationCreate(
	FlxActHandle			handle,
	FlxActSvrActivation *	serverAct);

void
flxActSvrActivationDelete(FlxActSvrActivation serverAct);

/* key method to send request to remote server and get back response */
FlxActBool
flxActSvrActivationSend(
	FlxActSvrActivation serverAct,
	FlxActError *			err);

FlxActBool
flxActSvrActivationReqCreate(
	FlxActSvrActivation serverAct,
	char **				xmlString);

FlxActBool
flxActSvrActivationReqSet(
	FlxActSvrActivation serverAct,
	const char **				xmlString);

FlxActBool
flxActSvrActivationRespProcess(
	FlxActSvrActivation serverAct,
	const char *		xmlString,
	FlxActBool *		pbIsConfig);

void
flxActSvrActivationCountSet(
	FlxActSvrActivation	serverAct,
	FlxActCountType		type,
	uint32_t			count);

uint32_t
flxActSvrActivationCountGet(
	FlxActSvrActivation	serverAct,
	FlxActCountType		type);

const char *
flxActSvrActivationEntitlementIdGet(FlxActSvrActivation serverAct);

void
flxActSvrActivationEntitlementIdSet(
	FlxActSvrActivation	serverAct,
	const char *		entitlementId);

const char *
flxActSvrActivationReasonGet(FlxActSvrActivation serverAct);

FlxActBool  
flxActSvrActivationReasonSet(
	FlxActSvrActivation serverAct, 
	const char *		reason);

const char *
flxActSvrActivationProductIdGet(FlxActSvrActivation serverAct);

void
flxActSvrActivationProductIdSet(
	FlxActSvrActivation	serverAct,
	const char *		productId);

const char *
flxActSvrActivationExpDateGet(FlxActSvrActivation serverAct);

void
flxActSvrActivationExpDateSet(
	FlxActSvrActivation	serverAct,
	const char *		expDate);

const char *
flxActSvrActivationUsernameGet(FlxActSvrActivation serverAct);

void
flxActSvrActivationUsernameSet(
	FlxActSvrActivation serverAct,
	const char *		username);

const char *
flxActSvrActivationHostnameGet(FlxActSvrActivation serverAct);

void
flxActSvrActivationHostnameSet(
	FlxActSvrActivation	serverAct,
	const char *		hostname);

FlxActBool
flxActSvrActivationIncludeEnterpriseData(
	FlxActSvrActivation	act,
	FlxActBool			bInclude);

void
flxActSvrActivationVendorDataSet(
	FlxActSvrActivation svr,
	const char *		key,
	const char *		value);

const char *
flxActSvrActivationVendorDataGet(
	FlxActSvrActivation	svr,
	const char *		key);


typedef struct flxActSvrReturn *	FlxActSvrReturn;


FlxActBool
flxActSvrReturnCreate(
	FlxActHandle			handle,
	FlxActSvrReturn *		serverRet);

void
flxActSvrReturnDelete(FlxActSvrReturn serverRet);

FlxActBool
flxActSvrReturnSend(
	FlxActSvrReturn	serverRet,
	FlxActError *		err);

FlxActBool 
flxActSvrReturnCancel(
	FlxActSvrReturn serverRet);

FlxActBool
flxActSvrReturnReqCreate(
	FlxActSvrReturn	serverRet,
	char **			xmlString);

FlxActBool
flxActSvrReturnReqSet(
	FlxActSvrReturn	serverRet,
	const char **			xmlString);

FlxActBool
flxActSvrReturnRespProcess(
	FlxActSvrReturn	serverRet,
	const char *	xmlString,
	FlxActBool *	pbIsConfig);

const char *
flxActSvrReturnProductIdGet(FlxActSvrReturn serverRet);

const char *
flxActSvrReturnSuiteIdGet(FlxActSvrReturn serverRet);

const char *
flxActSvrReturnEntitlementIdGet(FlxActSvrReturn serverRet);

const char *
flxActSvrReturnFRIdGet(FlxActSvrReturn serverRet);

uint32_t
flxActSvrReturnNumberValidDedSpcGet(FlxActSvrReturn serverRet);

const char *
flxActSvrReturnReasonGet(FlxActSvrReturn ret);

FlxActBool  
flxActSvrReturnReasonSet(
	FlxActSvrReturn		ret, 
	const char *		reason);

void
flxActSvrReturnProdLicSpcSet(
	FlxActSvrReturn		ret,
	FlxActProdLicSpc	product);

FlxActBool
flxActSvrReturnForceIncompleteSet(
	FlxActSvrReturn		serverRet,
	FlxActBool			bForceIncomplete);

FlxActBool
flxActSvrReturnForceIncompleteGet(FlxActSvrReturn serverRet);

void flxActSvrReturnVendorDataSet(
	FlxActSvrReturn		ret,
	const char *		key,
	const char *		value);

const char *flxActSvrReturnVendorDataGet(FlxActSvrReturn ret, const char * key);

typedef struct flxActSvrRepair *	FlxActSvrRepair;

FlxActBool
flxActSvrRepairCreate(
	FlxActHandle		handle,
	FlxActSvrRepair *	serverRep);


void
flxActSvrRepairDelete(FlxActSvrRepair serverRep);

FlxActBool
flxActSvrRepairSend(
	FlxActSvrRepair		serverRep,
	FlxActError *		err);

FlxActBool
flxActSvrRepairReqCreate(
	FlxActSvrRepair	serverRep,
	char **			xmlString);

FlxActBool
flxActSvrRepairReqSet(
	FlxActSvrRepair	serverRep,
	const char **			xmlString);

FlxActBool
flxActSvrRepairRespProcess(
	FlxActSvrRepair	serverRep,
	const char *	xmlString,
	FlxActBool *	pbIsConfig);

const char *flxActSvrRepairFRIdGet(FlxActSvrRepair serverRep);
const char *flxActSvrRepairProductIdGet(FlxActSvrRepair serverRep);
const char *flxActSvrRepairSuiteIdGet(FlxActSvrRepair serverRep);
const char *flxActSvrRepairEntitlementIdGet(FlxActSvrRepair serverRep);
void flxActSvrRepairProdLicSpcSet(FlxActSvrRepair serverRep, FlxActProdLicSpc product);

void
flxActSvrRepairVendorDataSet(
	FlxActSvrRepair		repair,
	const char *		key,
	const char *		value);

const char *flxActSvrRepairVendorDataGet(FlxActSvrRepair repair, const char * key);

/* 3-Server TS */

/* Server Type - can be standalone or 3-Server TS node configuration (Primary, Secondary, Tertiary).   */
typedef enum flxActSvrType 
{
	flxActSvrTypeStandalone = 0,  /* Standalone server - master n/a and always true.    */
	flxActSvrTypePrimary    = 1,  /* 3-Server Primary - master during normal operation. */
	flxActSvrTypeSecondary  = 2,  /* 3-Server Secondary - master during failover.       */
	flxActSvrTypeTertiary   = 3   /* 3-Server Tertiary - never master.                  */
} FlxActSvrType;

/*
 * Define roles a 3-server node can be configured as
 */
typedef enum flxAct3ServerRole {
    flxAct3ServerPrimary = 1,
    flxAct3ServerSecondary = 2,
    flxAct3ServerTertiary = 3
}FlxAct3ServerRole;


FlxActBool 
flxActSvrGetType(FlxActSvrType* pType, FlxActBool* pbIsMaster, FlxActError* pErr);

/* Get node name - only call in 3-Server (non-standalone) mode.
    
   Set size=sizeof(nameBuffer).
   To obtain size for buffer allocation, call with nameBuffer=NULL and size=0;
   function will return FNP_FALSE but size will be set to required buffer size. */
FlxActBool 
flxAct3SvrGetNodeName(FlxActSvrType serverType, char* pName, size_t* size, FlxActError* pError);


#ifdef __cplusplus
}
#endif

#endif


