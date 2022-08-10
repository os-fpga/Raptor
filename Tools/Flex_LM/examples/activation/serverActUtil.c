/**************************************************************************************************
* Copyright (c) 1997-2021 Flexera. All Rights Reserved.
**************************************************************************************************/


/**	@file 	serverActUtil.c
 *	@brief	Test server activation application

 *
 *	This is the source code for a simple server activation application whose
 *	primary purpose is for internal testing. This will probably become the sample
 *	server activation application to be shipped with the product.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifndef PC
#include <unistd.h>
#endif
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "FlxActCommon.h"
#include "FlxActError.h"
#include "FlxActSvr.h"
#include "FlxInit.h"
#include "lmclient.h"


#ifdef WINDOWS
  #include "wininstaller.h"
  #include <io.h>
  #if ( _MSC_VER>=1300 )
    /* Disable deprecated function warnings */
    #pragma warning( disable:4996 )
  #endif
#endif /* WINDOWS */
/*
 *	Application globals
 */
FlxActBool	g_bGenerateOnly = FLX_ACT_FALSE;
FlxActBool	g_bValidOnly = FLX_ACT_FALSE;
FlxActBool	g_bBrokenOnly = FLX_ACT_FALSE;
FlxActBool	g_bLongView = FLX_ACT_FALSE;
FlxActBool	g_bCancel = FLX_ACT_FALSE;
FlxActBool	g_bFromBuffer = FLX_ACT_FALSE;
char *		g_pszOutputFile = NULL;
char *		g_pszInputFile = NULL;
char *		g_pszEntitlementID = NULL;
char *		g_pszReason = NULL;
char *		g_pszProductID = NULL;
char *		g_pszExpiration = NULL;
char *		g_pszComm = NULL;
char *		g_pszCommServer = NULL;
char *		g_pszProxyDetails = NULL;
char *		g_pszVendorDataKey = NULL;
char *		g_pszVendorDataValue = NULL;
void *		g_pMyData = 0;

/* proxy details */
char		g_pszProxyHost[128];
uint32_t	g_pszProxyPort;
char		g_pszProxyUserid[128];
char		g_pszProxyPassword[128];

#define 	INIT_COUNT_VAL	((uint32_t)-1)

uint32_t	g_dwActCount = INIT_COUNT_VAL;
uint32_t	g_dwActOCount = INIT_COUNT_VAL;
uint32_t	g_dwConCount = INIT_COUNT_VAL;
uint32_t	g_dwConOCount = INIT_COUNT_VAL;
uint32_t	g_dwHybCount = INIT_COUNT_VAL;
uint32_t	g_dwHybOCount = INIT_COUNT_VAL;
uint32_t	g_dwRepairCount = INIT_COUNT_VAL;

/* to display status information */
const char* const COMMS_STATUS_STRINGS[] = {
	"Error",
	"Success",
	"Not set",
	"Cancelled by caller",
	"Creating request",
	"Request created",
	"Context created",
	"Connected to remote server",
	"Request Sent",
	"Polling for response",
	"Waiting for response",
	"Done"
};

/*
 *	Application defines
 */
#define OPTION_VIEW				"-view"
#define OPTION_LOCAL			"-local"
#define OPTION_LOCAL_CHECK		"-localcheck"
#define OPTION_TRIAL_RESET		"-reset"
#define OPTION_SERVED			"-served"
#define OPTION_COMM				"-comm"
#define OPTION_COMM_SERVER		"-commServer"
#define OPTION_PROXY_DETAILS	"-proxyDetails"
#define OPTION_RETURN			"-return"
#define OPTION_PROCESS			"-process"
#define OPTION_REPAIR			"-repair"
#define OPTION_GENERATE			"-gen"
#define OPTION_ENTITLEMENT_ID	"-entitlementID"
#define OPTION_REASON			"-reason"
#define OPTION_PRODUCT_ID		"-productID"
#define OPTION_EXPIRATION		"-expiration"
#define OPTION_ACT_COUNT		"-activatable"
#define OPTION_ACT_O_COUNT		"-activatableO"
#define OPTION_CON_COUNT		"-concurrent"
#define OPTION_CON_O_COUNT		"-concurrentO"
#define OPTION_HYB_COUNT		"-hybrid"
#define OPTION_HYB_O_COUNT		"-hybridO"
#define OPTION_REPAIRS			"-repairs"
#define OPTION_VALID_ONLY		"-valid"
#define OPTION_BROKEN_ONLY		"-broken"
#define OPTION_VENDOR_DATA		"-vendordata"
#define OPTION_LONG_VIEW		"-long"
#define OPTION_DELETE			"-delete"
#define OPTION_CANCEL			"-cancel"
#define OPTION_VIRT				"-virtual"
#define DEFAULT_EXPIRATION		"31-dec-2030"
#define DEFAULT_ENTITLEMENT_ID	"served-123"
#define DEFAULT_ACT_COUNT		0
#define DEFAULT_ACT_O_COUNT		0
#define DEFAULT_CON_COUNT		0
#define DEFAULT_CON_O_COUNT		0
#define DEFAULT_HYB_COUNT		0
#define DEFAULT_HYB_O_COUNT		0
#define DEFAULT_REPAIR_COUNT	0

#ifdef PC
	#define argument pszUTF8AppactUtilArg
	#define argument1 pszUTF8AppactUtilArg1
	#define argument2 argument
	#define CONVERT_ARGUMENT (pszUTF8AppactUtilArg1 = sConvertWCToUTF8(argv[++i]))
#else	  
	#define argument argv[i]
	#define argument1 argv[++i]
	#define argument2 argv[2]
	#define CONVERT_ARGUMENT 
#endif

/* temporary define here */
#define DEBUG printf

typedef enum
{
	OP_TS_UNKNOWN = 0,
	OP_TS_VIEW = 1,
	OP_TS_SERVEDACTIVATION,
	OP_TS_RETURN,
	OP_TS_PROCESS,
	OP_TS_REPAIR,
	OP_TS_DELETE,
	OP_TS_VIRT,
	OP_TS_LOCALACTIVATION,
	OP_TS_LOCALACTIVATION_CHECK,
	OP_TS_TRIAL_RESET
} OPERATION;

/****************************************************************************/
/**	@brief	Prints usage message
 *
 *	@param	None
 *
 *	@return	None
 ****************************************************************************/
static
void
sUsage()
{
	printf("Usage:\tserveractutil -view [-valid | -broken] [-long]\n");
	printf("\tserveractutil -served [-comm <flex|soap>]\n");
	printf("\t                      [-commServer <comm server>]\n");
	printf("\t                      [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                      [-entitlementID <entitlement_ID>]\n");
	printf("\t                      [-reason <reason_number>]\n");
	printf("\t                      [-productID <product_ID>]\n");
	printf("\t                      [-expiration <expiration_date>]\n");
	printf("\t                      [-activatable <count>] [-activatableO <count>]\n");
	printf("\t                      [-concurrent <count>] [-concurrentO <count>]\n");
	printf("\t                      [-hybrid <count>] [-hybridO <count>]\n");
	printf("\t                      [-repairs <count>]\n");
	printf("\t                      [-gen [<output_filename>]]\n");
	printf("\t                      [-vendordata <key> <value>]\n");
	printf("\tserveractutil -return <fulfillmentID>\n");
	printf("\t                      [-reason <reason_number>]\n");
	printf("\t                      [-comm <flex|soap>]\n");
	printf("\t                      [-commServer <comm server>]\n");
	printf("\t                      [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                      [-gen [<output_filename>]]\n");
	printf("\t                      [-vendordata <key> <value>]\n");
	printf("\tserveractutil -return <fulfillmentID> -cancel\n");
	printf("\tserveractutil -process <input_file>\n");
	printf("\tserveractutil -repair <fulfillmentID>\n");
	printf("\t                      [-comm <flex|soap>]\n");
	printf("\t                      [-commServer <comm server>]\n");
	printf("\t                      [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                      [-gen [<output_filename>]]\n");
	printf("\t                      [-vendordata <key> <value>]\n");
	printf("\tserveractutil -delete <fulfillmentID>\n");
	printf("\tserveractutil -virtual\n");
	printf("\tserveractutil -local <asr_file> [-buffer]\n");
	printf("\tserveractutil -localcheck <asr_file> [-buffer]\n");
	printf("\tserveractutil -reset <asr_file> [-buffer]\n");
}

/****************************************************************************/
/**	@brief	Status call back
 *
 ****************************************************************************/
uint32_t statusCallback(const void* pUserData, uint32_t statusOld, uint32_t statusNew) {
	printf("Status: %d, %s\n", statusNew, COMMS_STATUS_STRINGS[statusNew]);
	return flxCallbackReturnContinue;
}

#ifdef PC
/* Conversion from Wide Char to UTF8 */
static
int
sConvertToUTF8(
        const wchar_t * pwszInput,
        char *          pszOutput,
        int             iSize)
{
        if(!pwszInput)
                return 0;
        /*
         *      Do conversion IF iSize != 0, else return number of wchar_t's needed.
         */
        return WideCharToMultiByte(CP_UTF8,                         /* code page */
													0,              /* required to be zero for CP_UTF8 */
													pwszInput,      /* name to convert */
													-1,             /* string is NULL terminated */
													pszOutput,      /* ignored if iSize = 0 */
													iSize,          /* if iSize is 0, calc */
													NULL,           /* use system default for unknown chars */
													NULL);          /* don't care if defaut char used */

}

static
char *
sConvertWCToUTF8( const wchar_t * pwszInput )
{
	char *pszOutput = NULL;
	int piSize = 0;
	int status = 0;

	piSize = sConvertToUTF8(pwszInput,NULL,0);
	if(piSize)
	{
		pszOutput = (char *) malloc(sizeof(char) * ((piSize) + 1));
		if(pszOutput)
		{
			status = sConvertToUTF8(pwszInput, pszOutput, piSize + 1);
			if(status)
				return pszOutput;
		}
	}
	
    return NULL;
}
/* Conversion from UTF8 to Wide Char */
static
int
sConvertToWC(
        const char *    pszInput,
        wchar_t *       pwszOutput,
        int             iSize)
{
        if(!pszInput)
                return 0;
        /*
         *      Do conversion IF iSize != 0, else return number of wchar_t's needed.
         */
		return MultiByteToWideChar(CP_UTF8,                         /* code page */
													0,              /* flag */
													pszInput,          /* name to convert */
													-1,             /* string is NULL terminated */
													pwszOutput,     /* ignored if iSize = 0 */
													iSize);         /* if iSize is 0, calc */

}
/* Conversion from Wide Char to Multi Byte*/
static
int
sConvertToMB(
        const wchar_t *         pwszInput,
        char *                  pszOutput,
        int                     iSize)
{
        if(!pwszInput)
                return 0;
        /*
         *      Do conversion IF iSize != 0, else return number of wchar_t's needed.
         */
        return WideCharToMultiByte(CP_ACP,                          /* code page */
													0,              /* do the best we can */
													pwszInput,      /* name to convert */
													-1,             /* string is NULL terminated */
													pszOutput,      /* ignored if iSize = 0 */
													iSize,          /* if iSize is 0, calc */
													NULL,           /* use system default for unknown chars */
													NULL);          /* don't care if defaut char used */

}

/* Conversion from UTF8 to Multi Byte*/
static
char *
sConvertUTF8ToMB( const  char* pszInput )
{
	char *          pszOutput = NULL;
	wchar_t *       pwszTemp = NULL;
	int             piSize = 0;
	int             status = 0;

	piSize = sConvertToWC(pszInput,NULL,0);
	if(piSize)
	{
		pwszTemp = (wchar_t *) malloc(sizeof(wchar_t) * ((piSize) + 1));
		if(pwszTemp)
		{
			status = sConvertToWC(pszInput, pwszTemp, piSize + 1);
			if(status)
			{
				piSize = sConvertToMB(pwszTemp, NULL, 0);
				if(piSize)
				{
					pszOutput = (char *) malloc(sizeof(char) * ((piSize) + 1));
					if(pszOutput)
					{
						status = sConvertToMB(pwszTemp, pszOutput, piSize + 1);
					}
					if(status)
						return pszOutput;
				}
			}
		}
	}	
    return NULL;
}

#endif 

/****************************************************************************/
/**	@brief	Determine which operation is being requested
 *
 *	@param	argc	Number of command line arguments
 *	@param	argv	Array of command line arguments
 *
 *	@return	Operation requested
 *	@note	No error handling is done in this routine, this is a sample only.
 ****************************************************************************/
#ifdef PC
 static
OPERATION
sWhichOperation(
	int		  argc,
	wchar_t * argv[])
#else
 static
OPERATION
sWhichOperation(
	int		argc,
	char *	argv[])
#endif
{
	OPERATION	op = OP_TS_VIEW;	/* default */
	int			i = 0;
#ifdef PC	
	char *pszUTF8AppactUtilArg = NULL;
	char *pszUTF8AppactUtilArg1 =  NULL;
#endif	

	if(argc && argv)
	{
		for(i = 1; i < argc; i++)
		{
#ifdef PC		
			pszUTF8AppactUtilArg = sConvertWCToUTF8(argv[i]);
#endif		
			if(0 == strcmp(argument, OPTION_LOCAL_CHECK))
			{
				op = OP_TS_LOCALACTIVATION_CHECK;
			}
			else if(0 == strcmp(argument, OPTION_LOCAL))
			{
				op = OP_TS_LOCALACTIVATION;
			}
			else if(0 == strcmp(argument, OPTION_TRIAL_RESET))
			{
				op = OP_TS_TRIAL_RESET;
			}
			else if(0 == strcmp(argument, OPTION_SERVED))
			{
				op = OP_TS_SERVEDACTIVATION;
			}
			else if(0 == strcmp(argument, OPTION_COMM))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;			
					g_pszComm = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_COMM_SERVER))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;			
					g_pszCommServer = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_PROXY_DETAILS))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_pszProxyDetails = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_RETURN))
			{
				op = OP_TS_RETURN;
			}
			else if(0 == strcmp(argument, OPTION_CANCEL))
			{
				g_bCancel = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argument, OPTION_REPAIR))
			{
				op = OP_TS_REPAIR;
			}
			else if(0 == strcmp(argument, OPTION_PROCESS))
			{
				op = OP_TS_PROCESS;
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_pszInputFile = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_GENERATE))
			{
				g_bGenerateOnly = FLX_ACT_TRUE;
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_pszOutputFile = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_ENTITLEMENT_ID))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_pszEntitlementID = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_REASON))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_pszReason = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_PRODUCT_ID))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_pszProductID = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_EXPIRATION))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_pszExpiration = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_ACT_COUNT))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_dwActCount = atoi(argument1);
				}
			}
			else if(0 == strcmp(argument, OPTION_ACT_O_COUNT))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_dwActOCount = atoi(argument1);
				}
			}
			else if(0 == strcmp(argument, OPTION_CON_COUNT))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_dwConCount = atoi(argument1);
				}
			}
			else if(0 == strcmp(argument, OPTION_CON_O_COUNT))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_dwConOCount = atoi(argument1);
				}
			}
			else if(0 == strcmp(argument, OPTION_HYB_COUNT))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_dwHybCount = atoi(argument1);
				}
			}
			else if(0 == strcmp(argument, OPTION_HYB_O_COUNT))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_dwHybOCount = atoi(argument1);
				}
			}
			else if(0 == strcmp(argument, OPTION_REPAIRS))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_dwRepairCount = atoi(argument1);
				}
			}
			else if(0 == strcmp(argument, OPTION_VALID_ONLY))
			{
				if(g_bBrokenOnly)
				{
					sUsage();
					exit(1);
				}
				g_bValidOnly = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argument, OPTION_BROKEN_ONLY))
			{
				if(g_bValidOnly)
				{
					sUsage();
					exit(1);
				}
				g_bBrokenOnly = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argument, OPTION_VENDOR_DATA))
			{
				if((i + 2) < argc)
				{
					CONVERT_ARGUMENT;				
					g_pszVendorDataKey = argument1;
					CONVERT_ARGUMENT;
                    g_pszVendorDataValue = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_LONG_VIEW))
			{
				g_bLongView = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argument, OPTION_DELETE))
			{
				op = OP_TS_DELETE;
			}
			else if(0 == strcmp(argument, OPTION_VIRT))
			{
				op = OP_TS_VIRT;
			}
#ifdef PC
			if(pszUTF8AppactUtilArg)
				free(pszUTF8AppactUtilArg);
#endif				
		}
	}
	return op;
}

/****************************************************************************/
/**	@brief	If this is a 3-Server (as opposed to standalone) node,
 *			1. Only allow the primary server to perform transactions
 *			   (not during failover).
 *			2. Only allow concurrent licenses to be requested.
 *
 *	@param	prod	The FlxActProdLicSpc to output
 *
 *	@return	None
 ****************************************************************************/
static
FlxActBool 
sIsOperationAllowed(OPERATION op)
{
	FlxActSvrType	serverType;
	FlxActBool		bIsMaster;
	FlxActError		error;

	/* Get server Type. */
	if ( !flxActSvrGetType(&serverType, &bIsMaster, &error))
	{
		DEBUG("ERROR: flxActSvrGetType (%d,%d,%d)\n",
			  error.majorErrorNo,
			  error.minorErrorNo,
			  error.sysErrorNo);
		return FLX_ACT_FALSE;
	}

	/* If this is a 3-Server node... */
	if ((serverType != flxActSvrTypeStandalone))
	{
		/* Rule: only the 3STS primary node is allowed to do transactions (and not in failover). */
		/* If there's a transaction operation...
		   Note that the transaction and request action commands don't need to be checked as they 
		   can only be specified in addition to one or more of these commands. */
		if (	(op == OP_TS_SERVEDACTIVATION)
			 || (op == OP_TS_RETURN)
			 || (op == OP_TS_REPAIR))
		{
			if ((serverType != flxActSvrTypePrimary) || !bIsMaster)
			{
				printf("Command is not allowed for this 3-Server role.\n"); 
				return FLX_ACT_FALSE;
			}
		}

		/* Rule: 3STS activation requests can only specify concurrent or hybrid counts. */
		/* The Enterprise license server prevents hybrid licenses being used for activation. */
		if (op == OP_TS_SERVEDACTIVATION)
		{
			/* Assert that no activatable counts are present. */
			if (	( g_dwActCount  && (g_dwActCount  != INIT_COUNT_VAL) )
				 ||	( g_dwActOCount && (g_dwActOCount != INIT_COUNT_VAL) )
			   )
			{
				printf("3-Server requests may only specify concurrent or hybrid counts.\n"); 
				return FLX_ACT_FALSE;
			}
		}
	}
	return FLX_ACT_TRUE;
}

/****************************************************************************/
/**	@brief	Format and output deduction specs of a specific FlxActProdLicSpc.
 *
 *	@param	prod	The FlxActProdLicSpc to output
 *
 *	@return	None
 ****************************************************************************/
static
void
sOutputDedSpc(FlxActDedSpc dedSpc)
{
	unsigned int		concurrent		= 0;
	unsigned int		concurrentO		= 0;
	unsigned int		hybrid			= 0;
	unsigned int		hybridO			= 0;
	unsigned int		activatable		= 0;
	unsigned int		activatableO	= 0;
	unsigned int		repairs			= 0;

	const char *		pszDestinationFulfillmentId	= NULL;
	const char *		pszDestinationSystemName	= NULL;
	const char *		pszExpDate					= NULL;

	FlxActDeductionType	type = DEDUCTION_TYPE_UNKNOWN;


	if ( dedSpc == NULL )
	{
		return;
	}

	type = flxActCommonDedSpcTypeGet(dedSpc);

	pszDestinationFulfillmentId	= flxActCommonDedSpcDestinationFulfillmentIdGet(dedSpc);
#ifdef PC
	pszDestinationFulfillmentId = sConvertUTF8ToMB(pszDestinationFulfillmentId);
#endif
	pszDestinationSystemName	= flxActCommonDedSpcDestinationSystemNameGet(dedSpc);
	pszExpDate					= flxActCommonDedSpcExpDateGet(dedSpc);
	concurrent		= flxActCommonDedSpcCountGet(dedSpc, CONCURRENT);
	concurrentO		= flxActCommonDedSpcCountGet(dedSpc, CONCURRENT_OD);
	hybrid			= flxActCommonDedSpcCountGet(dedSpc, HYBRID);
	hybridO			= flxActCommonDedSpcCountGet(dedSpc, HYBRID_OD);
	activatable		= flxActCommonDedSpcCountGet(dedSpc, ACTIVATABLE);
	activatableO	= flxActCommonDedSpcCountGet(dedSpc, ACTIVATABLE_OD);
	repairs			= flxActCommonDedSpcCountGet(dedSpc, REPAIRS);

	printf("-------------------------------\n");

	switch ( type )
	{
		case DEDUCTION_TYPE_ACTIVATION:
			printf("Deduction Type: ACTIVATION\n");
			printf("Destination Fulfillment ID: %s\n",
					pszDestinationFulfillmentId ? pszDestinationFulfillmentId : "NONE");
			printf("Destination System Name: %s\n",
					pszDestinationSystemName ? pszDestinationSystemName : "NONE");
			printf("Expiration Date: %s\n", pszExpDate ? pszExpDate : "NONE");
			printf("Hybrid: %d\n", hybrid);
			printf("Activatable: %d\n",	activatable);
			break;


		case DEDUCTION_TYPE_TRANSFER:
			printf("Deduction Type: TRANSFER\n");
			printf("Destination Fulfillment ID: %s\n",
					pszDestinationFulfillmentId ? pszDestinationFulfillmentId : "NONE");
			printf("Destination System Name: %s\n",
					pszDestinationSystemName ? pszDestinationSystemName : "NONE");
			printf("Expiration Date: %s\n", pszExpDate ? pszExpDate : "NONE");
			printf("Concurrent: %d\n",				concurrent);
			printf("Concurrent overdraft: %d\n",	concurrentO);
			printf("Hybrid: %d\n",					hybrid);
			printf("Hybrid overdraft: %d\n",		hybridO);
			printf("Activatable: %d\n",				activatable);
			printf("Activatable overdraft: %d\n",	activatableO);
			printf("Repairs: %d\n",					repairs);
			break;


		case DEDUCTION_TYPE_REPAIR:
			printf("Deduction Type: REPAIR\n");
			printf("Destination Fulfillment ID: %s\n",
					pszDestinationFulfillmentId ? pszDestinationFulfillmentId : "NONE");
			printf("Destination System Name: %s\n",
					pszDestinationSystemName ? pszDestinationSystemName : "NONE");
			break;


		case DEDUCTION_TYPE_UNKNOWN:
		default:
			printf("Deduction Type: UNKNOWN\n");
	}

    printf("\n");
	return;
}

/****************************************************************************/
/**	@brief	Format and output the vendor dictionary entries of a specific FlxActProdLicSpc.
*
*	@param	product	The FlxActProdLicSpc
*
*	@return	None
****************************************************************************/
static
void
sOutputProdLicVendorDictionary(FlxActProdLicSpc product)
{
    uint32_t    count;
    uint32_t    index;
    const char* pKey;
    const char* pValue;
    const char* pValueByKey;

    /*
    * Get the number of entries in the vendor dictionary.
    */
    if (!flxActCommonProdLicSpcVendorDataGetCount(product, &count))
    {
        DEBUG("ERROR - product object invalid\n");
        return;
    }

    /*
    * Print the entries.
    */
    for (index = 0; index < count; index++)
    {
        if (flxActCommonProdLicSpcVendorDataGetByIndex(product, index, &pKey, &pValue))
            printf("Vendor dictionary entry: %s = %s\n", pKey, pValue);
        else
            DEBUG("ERROR - unable to get vendor dictionary entry\n");
    }

    if (count > 0)
    {
        /*
        * Test get-by-key by re-getting the last entry returned using its key.
        */
        if	(	!flxActCommonProdLicSpcVendorDataGetByKey(product, pKey, &pValueByKey)
            ||	(0 != strcmp(pValueByKey, pValue))
            )
        {
            DEBUG("ERROR - unable to get vendor dictionary entry by key\n");
        }
    }
}

/****************************************************************************/
/**	@brief	Format and output contents of a specific FlxActProdLicSpc.
 *
 *	@param	prod	The FlxActProdLicSpc to output
 *
 *	@return	None
 ****************************************************************************/
static
void
sOutputProdLic(FlxActProdLicSpc product)
{
	unsigned int		concurrent = 0;
	unsigned int		concurrentO = 0;
	unsigned int		hybrid = 0;
	unsigned int		hybridO = 0;
	unsigned int		activatable = 0;
	unsigned int		activatableO = 0;
	unsigned int		repairs = 0;
	const char *		pszProductID = NULL;
	const char *		pszFulfillmentID = NULL;
	const char *		pszEntitlementID = NULL;
	const char *		pszSuiteID = NULL;
	const char *		pszExpDate = NULL;
	const char *		pszFeatureLine = NULL;
	const char *		pszActServerChain = NULL;
	uint32_t			trustflags = 0;
	char				szTrustFlags[256] = {'\0'};
	static char			oneshot = 0;
	FlxActBool			bIsDisabled = FLX_ACT_TRUE;
	FlxFulfillmentType	type = FULFILLMENT_TYPE_UNKNOWN;
	char *				szType[] = {"UNKNOWN", "TRIAL", "SHORTCODE",  "EMERGENCY", \
									"SERVED ACTIVATION", "SERVED OVERDRAFT ACTIVATION", \
									"PUBLISHER ACTIVATION", "PUBLISHER OVERDRAFT ACTIVATION"};
	FlxActDedSpc		dedSpc = NULL;
	uint32_t			dedCount = 0;
	uint32_t			validDedCount = 0;
	uint32_t			ii = 0;


	if(product)
	{
		trustflags = flxActCommonProdLicSpcTrustFlagsGet(product);
		if(g_bBrokenOnly)
		{
			if(FLAGS_FULLY_TRUSTED == trustflags)
			{
				return;
			}
		}
		concurrent = flxActCommonProdLicSpcCountGet(product, CONCURRENT);
		concurrentO = flxActCommonProdLicSpcCountGet(product, CONCURRENT_OD);
		hybrid = flxActCommonProdLicSpcCountGet(product, HYBRID);
		hybridO = flxActCommonProdLicSpcCountGet(product, HYBRID_OD);
		activatable = flxActCommonProdLicSpcCountGet(product, ACTIVATABLE);
		activatableO = flxActCommonProdLicSpcCountGet(product, ACTIVATABLE_OD);
		repairs = flxActCommonProdLicSpcCountGet(product, REPAIRS);	
		pszProductID = flxActCommonProdLicSpcProductIdGet(product);
		pszFulfillmentID = flxActCommonProdLicSpcFulfillmentIdGet(product);
		pszEntitlementID = flxActCommonProdLicSpcEntitlementIdGet(product);
#ifdef PC	
		pszProductID = sConvertUTF8ToMB(pszProductID);
		pszFulfillmentID = sConvertUTF8ToMB(pszFulfillmentID);
		pszEntitlementID = sConvertUTF8ToMB(pszEntitlementID);
#endif
		pszSuiteID = flxActCommonProdLicSpcSuiteIdGet(product);
		pszExpDate = flxActCommonProdLicSpcExpDateGet(product);
		pszFeatureLine = flxActCommonProdLicSpcFeatureLineGet(product);
		bIsDisabled = flxActCommonProdLicSpcIsDisabled(product);
		type = flxActCommonProdLicSpcFulfillmentTypeGet(product);

		if(0 == oneshot)
		{
			printf("\n--------------------------------------------------------------------\n");
			oneshot = 1;
		}
		if(trustflags == FLAGS_FULLY_TRUSTED)
		{
			printf("Trust Flags: FULLY TRUSTED\n");
		}
		else
		{
			if(!(FLAGS_TRUST_RESTORE & trustflags))
			{
				strcpy(szTrustFlags, "RESTORE");
			}
			if(!(FLAGS_TRUST_HOST & trustflags))
			{
				if(szTrustFlags[0] == '\0')
				{
					strcpy(szTrustFlags, "HOST");
				}
				else
				{
					strcat(szTrustFlags, ",HOST");
				}
			}
			if(!(FLAGS_TRUST_TIME & trustflags))
			{
				if(szTrustFlags[0] == '\0')
				{
					strcpy(szTrustFlags, "TIME");
				}
				else
				{
					strcat(szTrustFlags, ",TIME");
				}
			}
			printf("Trust Flags: **BROKEN** %s\n", szTrustFlags);
		}
		printf("Fulfillment Type: %s\n",
			(type <= FULFILLMENT_TYPE_PUBLISHER_OVERDRAFT_ACTIVATION) ? szType[type] : szType[0]);
		printf("Status: %s\n", bIsDisabled ? "**DISABLED**" : "ENABLED");
		printf("Fulfillment ID: %s\n", pszFulfillmentID ? pszFulfillmentID : "NONE");
		printf("Entitlement ID: %s\n", pszEntitlementID ? pszEntitlementID : "NONE");
		printf("Product ID: %s\n", pszProductID ? pszProductID : "NONE");
		printf("Suite ID: %s\n", pszSuiteID ? pszSuiteID : "NONE");
		printf("Expiration date: %s\n", pszExpDate ? pszExpDate : "NONE");
		printf("Concurrent: %d\n", concurrent);
		printf("Concurrent overdraft:%d\n", concurrentO);
		printf("Hybrid: %d\n", hybrid);
		printf("Hybrid overdraft: %d\n", hybridO);
		printf("Activatable: %d\n", activatable);
		printf("Activatable overdraft: %d\n", activatableO);
		printf("Repairs: %d\n", repairs);
		printf("Feature line(s):\n%s\n", pszFeatureLine ? pszFeatureLine : "NONE");
        sOutputProdLicVendorDictionary(product);

		if ( g_bLongView == FLX_ACT_TRUE )
		{
			pszActServerChain = flxActCommonProdLicSpcActServerChainGet(product);
			dedCount = flxActCommonProdLicSpcNumberDedSpcGet(product);
			validDedCount = flxActCommonProdLicSpcNumberValidDedSpcGet(product);

			printf("Activation Server Chain: %s\n", pszActServerChain ? pszActServerChain : "NONE");
			printf("Deduction Count: %d\n", dedCount);
			printf("Valid Deduction Count: %d\n\n", validDedCount);

			for ( ii = 0; ii < dedCount; ii++ )
			{
				sOutputDedSpc(flxActCommonProdLicSpcDedSpcGet(product, ii));
			}
		}
		printf("--------------------------------------------------------------------\n");
	}
	return;
}

/****************************************************************************/
/**	@brief	Return a text string for the type.
 *
 *	@param	serverType	FlxActSvrType specifying server type
 *
 *	@return	Pointer to string representation of server type.
 ****************************************************************************/
const char* sServerTypeToText(FlxActSvrType serverType)
{
    /* Translate to text. */
    switch (serverType)
    {
    case flxActSvrTypeStandalone:   return "Standalone";
    case flxActSvrTypePrimary:      return "Primary";
    case flxActSvrTypeSecondary:    return "Secondary";
    case flxActSvrTypeTertiary:     return "Tertiary";
    default:                        return "Unknown";
    }
}

/****************************************************************************/
/**	@brief	Display the contents of trusted storage
 *
 *	@param	handle	FlxActHandle to use to access trusted storage
 *
 *	@return	FLX_ACT_TRUE if anything is found else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sDumpTS(FlxActHandle handle)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActLicSpc		licSpc = 0;
	FlxActProdLicSpc	prodSpec = 0;
	uint32_t			count = 0;
	uint32_t			i = 0;
	FlxActError			error;

	if(handle)
	{
		/*
		 *	Dump contents of trusted storage
		 */
		bRV = flxActCommonLicSpcCreate(handle, &licSpc);
		if(bRV)
		{
			if(g_bValidOnly)
				bRV = flxActCommonLicSpcPopulateFromTS(licSpc);
			else
                bRV = flxActCommonLicSpcPopulateAllFromTS(licSpc);
			if(bRV)
			{
       			/* Get the server type */
        		FlxActSvrType           serverType;
        		FlxActBool              bIsMaster;

				bRV = flxActSvrGetType(&serverType, &bIsMaster, &error);

				if (bRV)
				{
					if (serverType != flxActSvrTypeStandalone)
        			{
            			/* 3-Server, print the node type and whether master. */
						printf("3-Server Node Type: %s %s\n", sServerTypeToText(serverType),
																bIsMaster? " currently MASTER" : "");
					}

					count = flxActCommonLicSpcGetNumberProducts(licSpc);
					if(0 == count)
					{
						printf("No fulfillment records in trusted storage\n");
					}
					else
					{
						bRV = FLX_ACT_TRUE;
						for(i = 0; i < count; i++)
						{
							prodSpec = flxActCommonLicSpcGet(licSpc, i);
							if(prodSpec)
							{
								sOutputProdLic(prodSpec);
							}
						}
					}
				}
				else
				{
					DEBUG("ERROR: %s (%d,%d,%d)\n", "flxActSvrGetType",
													error.majorErrorNo,
													error.minorErrorNo,
													error.sysErrorNo);
        		}
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: %s (%d,%d,%d)\n", (g_bValidOnly)? "flxActCommonLicSpcPopulateFromTS" 
															  : "flxActCommonLicSpcPopulateAllFromTS",
												error.majorErrorNo,
												error.minorErrorNo,
												error.sysErrorNo);
			}

			/*
			 *	Cleanup
			 */
			flxActCommonLicSpcDelete(licSpc);
		}
		else
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: flxActCommonLicSpcCreate (%d,%d,%d)\n", error.majorErrorNo,
																  error.minorErrorNo,
																  error.sysErrorNo);
		}
	}

	return bRV;
}

/****************************************************************************/
/**	@brief	Set parameters to use in activation (transfer) request
 *
 *	@param	server		Server activation object
 *
 *	@return	None
 *	@note	If not specified on commandline, check to see if any environment
 *			variables are set to override the default.
 ****************************************************************************/
static
void
sSetActivatonRequestParameters(FlxActHandle handle, FlxActSvrActivation server)
{
	char *	pszActivate = NULL;
	char *	pszOActivate = NULL;
	char *	pszCon = NULL;
	char *	pszOCon = NULL;
	char *	pszHyb = NULL;
	char *	pszOHyb = NULL;
	char *	pszRepairCount = NULL;

	FlxActError error;

	if(NULL == g_pszEntitlementID)
		g_pszEntitlementID = getenv("ENTITLEMENT_ID");
	if(NULL == g_pszExpiration)
		g_pszExpiration = getenv("EXP_DATE");

	if (NULL == g_pszCommServer)
	{
		g_pszCommServer = getenv("COMM_SERVER");
		if (NULL == g_pszCommServer)
		{
			g_pszCommServer = getenv("LM_LICENSE_FILE");
			if(NULL == g_pszCommServer)
			{
				g_pszCommServer = "@localhost";
			}
		}
	}

	if(INIT_COUNT_VAL == g_dwActCount)
	{
		pszActivate = getenv("ACTIVATABLE");
		g_dwActCount = pszActivate ? atoi(pszActivate) : DEFAULT_ACT_COUNT;
	}
	if(INIT_COUNT_VAL == g_dwActOCount)
	{
		pszOActivate = getenv("OVERDRAFT_ACTIVATABLE");
		g_dwActOCount = pszOActivate ? atoi(pszOActivate) : DEFAULT_ACT_O_COUNT;
	}
	if(INIT_COUNT_VAL == g_dwConCount)
	{
		pszCon = getenv("CONCURRENT");
		g_dwConCount = pszCon ? atoi(pszCon) : DEFAULT_CON_COUNT;
	}
	if(INIT_COUNT_VAL == g_dwConOCount)
	{
		pszOCon = getenv("OVERDRAFT_CONCURRENT");
		g_dwConOCount = pszOCon ? atoi(pszOCon) : DEFAULT_CON_O_COUNT;
	}
	if(INIT_COUNT_VAL == g_dwHybCount)
	{
		pszHyb = getenv("HYBRID");
		g_dwHybCount = pszHyb ? atoi(pszHyb) : DEFAULT_HYB_COUNT;
	}
	if(INIT_COUNT_VAL == g_dwHybOCount)
	{
		pszOHyb = getenv("OVERDRAFT_HYBRID");
		g_dwHybOCount = pszOHyb ? atoi(pszOHyb) : DEFAULT_HYB_O_COUNT;
	}
	if(INIT_COUNT_VAL == g_dwRepairCount)
	{
		pszRepairCount = getenv("REPAIRS");
		g_dwRepairCount = pszRepairCount ? atoi(pszRepairCount) : DEFAULT_REPAIR_COUNT;
	}

	flxActSvrActivationEntitlementIdSet(server, g_pszEntitlementID ? g_pszEntitlementID : DEFAULT_ENTITLEMENT_ID);
	if(g_pszProductID)
	{
		flxActSvrActivationProductIdSet(server, g_pszProductID);
	}
	/*
	*	If an activation reason has been supplied, set it
	*/
	if (NULL != g_pszReason)
	{
		if (FLX_ACT_FALSE == flxActSvrActivationReasonSet(server, g_pszReason))
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: flxActAppActivationReasonSet (%d,%d,%d)\n", error.majorErrorNo, 
																	  error.minorErrorNo, 
																	  error.sysErrorNo);
			printf("Reason must be numeric.  Default of \"0\" will be used.\n");
		}
	}

	flxActSvrActivationExpDateSet(server, g_pszExpiration ? g_pszExpiration : DEFAULT_EXPIRATION);
	flxActSvrActivationCountSet(server, ACTIVATABLE, g_dwActCount);
	flxActSvrActivationCountSet(server, ACTIVATABLE_OD, g_dwActOCount);
	flxActSvrActivationCountSet(server, CONCURRENT, g_dwConCount);
	flxActSvrActivationCountSet(server, CONCURRENT_OD, g_dwConOCount);
	flxActSvrActivationCountSet(server, HYBRID, g_dwHybCount);
	flxActSvrActivationCountSet(server, HYBRID_OD, g_dwHybOCount);
	flxActSvrActivationCountSet(server, REPAIRS, g_dwRepairCount);

	printf("Generating transfer request using:\n");
	printf("\tEntitlement ID = %s\n", g_pszEntitlementID ? g_pszEntitlementID : DEFAULT_ENTITLEMENT_ID);
	if(g_pszProductID)
	{
		printf("\tProduct ID = %s\n", g_pszProductID);
	}
	printf("\tExpiration = %s\n", g_pszExpiration ? g_pszExpiration : DEFAULT_EXPIRATION);
	printf("\tActivatable Count = %d\n", g_dwActCount);
	printf("\tActivatable Overdraft Count = %d\n", g_dwActOCount);
	printf("\tConcurrent Count = %d\n", g_dwConCount);
	printf("\tConcurrent Overdraft Count = %d\n", g_dwConOCount);
	printf("\tHybrid Count = %d\n", g_dwHybCount);
	printf("\tHybrid Overdraft Count = %d\n", g_dwHybOCount);
	printf("\tRepair Count = %d\n", g_dwRepairCount);
}

static
FlxActBool
sVirtualizationExample(FlxActHandle handle)
{
	FlxActBool          bRV = FLX_ACT_FALSE;

	if (handle)
	{
#if defined(__linux__) || defined(OS_LINUX) || defined(WINDOWS) || defined(OS_WINDOWS)

		FlxActBool  bVirtPlat;

		bRV = flxActCommonVirtualStatusGet( handle, &bVirtPlat );

		if( !bRV )
		{
			FlxActError error;
			flxActCommonHandleGetError(handle, &error);

			if( error.majorErrorNo == LM_TS_VM_ATTRIBUTE_PRIVILEGE )
			{
				printf( "Insufficient privilege to determine Virtual Platform Status (%d,%d,%d)\n"
						"Is the FlexNet Licensing Service installed?\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			else
			{
				printf("Unexpected Error: Could not get Virtual Platform Status (%d,%d,%d)\n",
																		error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
            }

			return bRV;
		}

		if( bVirtPlat )
		{
			printf("Running on Virtual Platform\n");
			bRV = FLX_ACT_TRUE;
		}
		else
#endif
		{
			printf("Running on Physical Platform\n");
			bRV = FLX_ACT_TRUE;
		}
	}

	return bRV;
}

/****************************************************************************/
/**	@brief	To set comm type and comm server
 *
 *	@param	handle		FlxActHandle used to access trusted storage
 *
 *	@return	void
 ****************************************************************************/
static
void
setCommDetails(FlxActHandle	handle)
{
	flxActCommonHandleSetRemoteServer(handle, g_pszCommServer);

	if (g_pszComm != NULL && !strcmp(g_pszComm, "soap")) {
		flxActCommonHandleSetCommType(handle, flxCommsMvsnSoap);
		flxActCommonHandleSetStatusCallback(handle, &statusCallback, g_pMyData);

		if (g_pszProxyDetails) {
			/*set proxy server */
			sscanf(g_pszProxyDetails, "%s %d %s %s", g_pszProxyHost,
													 &g_pszProxyPort,
													 g_pszProxyUserid,
													 g_pszProxyPassword);
			flxActCommonHandleSetProxyDetails(handle,
											g_pszProxyPort,   /* (uint32_t) proxy_port, */
											g_pszProxyHost,   /* proxy_host, */
											g_pszProxyUserid,           /* proxy_userName, */
											g_pszProxyPassword);           /* proxy_password); */
		}

	}
	else {
		flxActCommonHandleSetCommType(handle, flxCommsMvsnFlex);
	}
}


/****************************************************************************/
/**	@brief	Handle end to end server transfer
 *
 *	@param	handle		FlxActHandle used to access trusted storage
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sHandleActivation(FlxActHandle	handle)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActSvrActivation	server = 0;
	FlxActError			error;
	char *				pszOutput = NULL;
	const char *		pszOpsError = NULL;
    FlxActSvrType   	serverType = flxActSvrTypeStandalone;
    FlxActBool      	bIsMaster = FLX_ACT_FALSE;

	if(handle)
	{
		/*
		 *	Create a client handle
		 */
		if(FLX_ACT_FALSE == flxActSvrActivationCreate(handle, &server))
		{
			DEBUG("ERROR: flxActSvrActivationCreate\n");
			return FLX_ACT_FALSE;
		}

		sSetActivatonRequestParameters(handle, server);

		setCommDetails(handle);

		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the activation request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActSvrActivationVendorDataSet(server, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		/*
		 *	Add entry to indicate whether we are in 3-Server mode
		 */
		if ( flxActSvrGetType(&serverType, &bIsMaster, &error))
		{
			flxActSvrActivationVendorDataSet(server,"3STS_MODE", serverType==flxActSvrTypeStandalone ? "false" : "true");

			if(g_bGenerateOnly)
			{
				/*
			 	*	Generate only
			 	*/
				printf("\nWriting signed activation request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
				if(flxActSvrActivationReqSet(server, (const char**)&pszOutput))
				{
					bRV = FLX_ACT_TRUE;
					if(g_pszOutputFile)
					{
						FILE *	fp = NULL;

						fp = fopen(g_pszOutputFile, "w+");
						if(fp)
						{
							fwrite(pszOutput, strlen(pszOutput), 1, fp);
							fclose(fp);
						}
						else
						{
							DEBUG("ERROR: Unable to open file %s\n", g_pszOutputFile);
						}
					}
					else
					{
						/*
					 	*	Output to stdout
					 	*/
						printf("%s\n", pszOutput);
					}
					if(pszOutput)
					{
						pszOutput = NULL;
					}
				}
				else
				{
					flxActCommonHandleGetError(handle, &error);
					DEBUG("ERROR: flxActSvrActivationReqSet - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
				}
			}
			else
			{
				/*
			 	*	Generate and send
			 	*/
				if(flxActSvrActivationSend(server, &error))
				{
					bRV = FLX_ACT_TRUE;
					DEBUG("TRANSFER REQUEST SUCCESSFULLY PROCESSED\n");
				}
				else
				{
					flxActCommonHandleGetError(handle, &error);
					if(LM_TS_OPERATIONS == error.majorErrorNo)
					{
						pszOpsError = flxActCommonHandleGetLastOpsErrorString(handle);
						if(pszOpsError)
						{
							printf("Operations error: %d %s\n", flxActCommonHandleGetLastOpsError(handle), pszOpsError);
						}
					}
					else
					{
						DEBUG("ERROR: flxActSvrActivationSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
					}
				}
			}
		}
		else
		{
			DEBUG("ERROR: flxActSvrActivationCreate: Could not determine 3-Server status (%d,%d,%d)\n", error.majorErrorNo, 
																										error.minorErrorNo,
																										error.sysErrorNo);
		}

		/*
		 *	Cleanup
		 */
		flxActSvrActivationDelete(server);
	}

	return bRV;
}

/****************************************************************************/
/**	@brief	Handle return of a fulfillment.
 *
 *	@param	handle		FlxActHandle used to access trusted storage
 *	@param	fulfillID	FulfillmentID to return
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static
FlxActBool
sHandleReturn(
	FlxActHandle	handle,
	const char *	fulfillId)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActBool			bFound = FLX_ACT_FALSE;
	FlxActSvrReturn		ret = 0;
	FlxActError			error;
	FlxActLicSpc		licSpec = 0;
	FlxActProdLicSpc	prodSpec = 0;
	unsigned int		count = 0;
	unsigned int		i = 0;
	const char *		pszEntitlementId = NULL;
	const char *		pszProdId = NULL;
	const char *		pszFulfillId = NULL;
	const char *		pszSuiteId = NULL;
	const char *		pszFRUniqueId = NULL;
	const char *		pszOpsError = NULL;
	char *				pszOutput = NULL;
    FlxActSvrType   	serverType = flxActSvrTypeStandalone;
    FlxActBool      	bIsMaster = FLX_ACT_FALSE;

	if(handle && fulfillId)
	{
		/*
		 *	Create a return
		 */
		if(FLX_ACT_FALSE == flxActSvrReturnCreate(handle, &ret))
		{
			DEBUG("ERROR: flxActSvrReturnCreate\n");
			return FLX_ACT_FALSE;
		}

		if (NULL == g_pszCommServer)
		{
			g_pszCommServer = getenv("COMM_SERVER");
			if (NULL == g_pszCommServer)
			{
				g_pszCommServer = getenv("LM_LICENSE_FILE");
				if(NULL == g_pszCommServer)
				{
					g_pszCommServer = "@localhost";
				}
			}
		}

		/*
		 *	Find out which one we want to return
		 */
		bRV = flxActCommonLicSpcCreate(handle, &licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			DEBUG("ERROR: flxActCommonLicSpcCreate\n");
			flxActSvrReturnDelete(ret);
			return bRV;
		}
		/*
		 *	Populate from TS
		 */
		bRV = flxActCommonLicSpcPopulateFromTS(licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			DEBUG("ERROR: flxActCommonLicSpcPopulateFromTS\n");
			flxActCommonLicSpcDelete(licSpec);
			flxActSvrReturnDelete(ret);
			return bRV;
		}

		/*
		 *	Figure out how many there are and which one we want
		 */
		count = flxActCommonLicSpcGetNumberProducts(licSpec);
		if(count)
		{
			for(i = 0; i < count; i++)
			{
				prodSpec = flxActCommonLicSpcGet(licSpec, i);
				if(0 == prodSpec)
				{
					DEBUG("ERROR: flxActCommonLicSpcGet - Invalid index\n");
					flxActCommonLicSpcDelete(licSpec);
					flxActSvrReturnDelete(ret);
					return FLX_ACT_FALSE;
				}
				/*
				 *	Check to see if this is the one we want
				 */
				pszFulfillId = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
				if(pszFulfillId && (0 == strcmp(pszFulfillId, fulfillId)))
				{
					flxActSvrReturnProdLicSpcSet(ret, prodSpec);
					bFound = FLX_ACT_TRUE;
					break;
				}
			}
		}
		else
		{
			DEBUG("WARNING: No licenses in Trusted Storage\n");
			flxActCommonLicSpcDelete(licSpec);
			flxActSvrReturnDelete(ret);
			return bRV;
		}

		if(FLX_ACT_FALSE == bFound)
		{
			DEBUG("ERROR: Unable to find fulfillment id %s\n", fulfillId);
			flxActCommonLicSpcDelete(licSpec);
			flxActSvrReturnDelete(ret);
			return bRV;
		}

		/*
		 * Check for action cancel.
		 */
		if (g_bCancel)
		{
			/*
			 * Cancel any outstanding transaction for this fulfillment id.
			 */
			bRV = flxActSvrReturnCancel(ret);

			flxActCommonLicSpcDelete(licSpec);
			flxActSvrReturnDelete(ret);
			return bRV;
		}

		setCommDetails(handle);

		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the return request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActSvrReturnVendorDataSet(ret, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		/*
		 *	Add entry to indicate whether we are in 3-Server mode
		 */
		if ( !flxActSvrGetType(&serverType, &bIsMaster, &error))
		{
			DEBUG("ERROR: Could not determine 3-Server status (%d,%d,%d)\n", error.majorErrorNo, 
																			 error.minorErrorNo,
																			 error.sysErrorNo);
			
			flxActCommonLicSpcDelete(licSpec);
			flxActSvrReturnDelete(ret);
			return bRV;
    	}
		flxActSvrReturnVendorDataSet(ret,"3STS_MODE", serverType==flxActSvrTypeStandalone ? "false" : "true");

		/*
		*	If a return reason has been supplied, set it
		*/
		if (NULL != g_pszReason)
		{
			if (FLX_ACT_FALSE == flxActSvrReturnReasonSet(ret, g_pszReason))
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: flxActAppActivationReasonSet (%d,%d,%d)\n", error.majorErrorNo, 
																		  error.minorErrorNo, 
																		  error.sysErrorNo);
				printf("Reason must be numeric.\n");
				flxActCommonLicSpcDelete(licSpec);
				flxActSvrReturnDelete(ret);
				return FLX_ACT_FALSE;
			}
		}

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed return request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActSvrReturnReqSet(ret, (const char**)&pszOutput))
			{
				bRV = FLX_ACT_TRUE;
				if(g_pszOutputFile)
				{
					FILE *	fp = NULL;

					fp = fopen(g_pszOutputFile, "w+");
					if(fp)
					{
						fwrite(pszOutput, strlen(pszOutput), 1, fp);
						fclose(fp);
					}
					else
					{
						DEBUG("ERROR: fopen\n");
					}
				}
				else
				{
					/*
					 *	Output to stdout
					 */
					printf("%s\n", pszOutput);
				}
				if(pszOutput)
				{
					pszOutput = NULL;
				}
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: flxActSvrReturnReqSet - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}

		}
		else
		{
			if(flxActSvrReturnSend(ret, &error))
			{
				DEBUG("SUCCESSFULLY SENT RETURN REQUEST\n");
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				if(LM_TS_OPERATIONS == error.majorErrorNo)
				{
					pszOpsError = flxActCommonHandleGetLastOpsErrorString(handle);
					if(pszOpsError)
					{
						printf("Operations error: %d %s\n", flxActCommonHandleGetLastOpsError(handle), pszOpsError);
					}
				}
				else
				{
					DEBUG("ERROR: flxActSvrReturnSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
				}
			}
		}

		/*
		 *	Cleanup
		 */
		if(licSpec)
		{
			flxActCommonLicSpcDelete(licSpec);
		}
		flxActSvrReturnDelete(ret);

	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Process XML
 *
 *	@param	handle	FlxActHandle used to access trusted storage
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 *	@note	Primary use if for manual transactions
 ****************************************************************************/
static
FlxActBool
sHandleManualProcessing(FlxActHandle handle)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActBool			bIsTrustedConfig = FLX_ACT_FALSE;
	FlxActSvrActivation	server = 0;
	FlxActError			error;
	char *				pszInput = NULL;
	char				buffer[1024] = {'\0'};
	long				size = 0;
	long				bytesread = 0;

	if(handle)
	{
		/*
		 *	Create a server handle
		 */
		if(FLX_ACT_FALSE == flxActSvrActivationCreate(handle, &server))
		{
			DEBUG("ERROR: flxActSvrActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		printf("\nReading request from %s\n", g_pszInputFile ? g_pszInputFile : "stdin");
		if(g_pszInputFile)
		{
			FILE *	fp = NULL;

			fp = fopen(g_pszInputFile, "rb");
			if(fp)
			{
				if (0 == fseek(fp, 0L, SEEK_END))
				{
					size = ftell(fp);
					if (0 == fseek(fp, 0L, SEEK_SET))
						pszInput = malloc(sizeof(char) * (size + 1));
				}
				if(pszInput)
				{
					size_t bytesRead = fread(pszInput, sizeof(char), size, fp);
					pszInput[bytesRead] = '\0';
					fclose(fp);
					fp = NULL;
				}
				else
				{
					DEBUG("ERROR: fseek or malloc\n");
					fclose(fp);
					flxActSvrActivationDelete(server);
					return FLX_ACT_FALSE;
				}
			}
			else
			{
				DEBUG("ERROR: fopen\n");
				flxActSvrActivationDelete(server);
				return FLX_ACT_FALSE;
			}
		}
		else
		{
			/*
			 *	Read input from stdin
			 */
			bytesread = read(0, &buffer, sizeof(buffer));
            while(bytesread > 0)
            {
				size += bytesread;
				pszInput = realloc(pszInput, size + 1);
				if(pszInput)
				{
					memcpy(&(pszInput[size - bytesread]), buffer, bytesread);
					pszInput[size] = '\0';
				}
				else
				{
					DEBUG("ERROR: malloc\n");
					flxActSvrActivationDelete(server);
					return FLX_ACT_FALSE;
				}
				bytesread = read(0, &buffer, sizeof(buffer));
			}
		}
		if(pszInput)
		{
			if(flxActSvrActivationRespProcess(server, (const char *)pszInput, &bIsTrustedConfig))
			{
				if(FLX_ACT_TRUE == bIsTrustedConfig)
				{
					DEBUG("SUCCESSFULLY PROCESSED TRUSTED CONFIG RESPONSE, RESUBMIT REQUEST\n");
				}
				else
				{
					DEBUG("SUCCESSFULLY PROCESSED RESPONSE\n");
				}
				bRV = FLX_ACT_TRUE;
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: flxActSvrActivationRespProcess - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			free(pszInput);
			pszInput = NULL;
		}
		else
		{
			DEBUG("ERROR: NO INPUT\n");
		}
		flxActSvrActivationDelete(server);
	}

	return bRV;

}

/****************************************************************************/
/**	@brief	Delete fulfillment whose ID matches
 *
 *	@param	handle		FlxActHandle used to access trusted storage
 *	@param	fulfillID	FulfillmentID to delete
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static
FlxActBool
sHandleDelete(
	FlxActHandle	handle,
	const char *	fulfillId)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActLicSpc		licSpc = 0;
	FlxActProdLicSpc	prodSpec = 0;
	FlxActError			error;
	uint32_t			count = 0;
	uint32_t			i = 0;
	const char *		pszFRID = NULL;

	if(handle)
	{
		/*
		 *	Create a license spec and populate it with all FR's
		 */
		bRV = flxActCommonLicSpcCreate(handle, &licSpc);
		if(bRV)
		{
			if(g_bValidOnly)
                bRV = flxActCommonLicSpcPopulateFromTS(licSpc);
			else
                bRV = flxActCommonLicSpcPopulateAllFromTS(licSpc);
			if(bRV)
			{
				count = flxActCommonLicSpcGetNumberProducts(licSpc);
				if(0 == count)
				{
					printf("No fulfillment records in trusted storage\n");
				}
				else
				{
					for(i = 0; i < count; i++)
					{
						prodSpec = flxActCommonLicSpcGet(licSpc, i);
						if(prodSpec)
						{
							/*
							 *	Check to see if this is the one we're interested in
							 */
							pszFRID = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
							if(pszFRID && (0 == strcmp(pszFRID, fulfillId)))
							{
								/*
								 *	Delete this FR
								 */
								bRV = flxActCommonLicSpcProductDelete(licSpc, prodSpec);
								if(FLX_ACT_FALSE == bRV)
								{
									flxActCommonHandleGetError(handle, &error);
									DEBUG("ERROR: flxActCommonLicSpcProductDelete - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
								}
								else
								{
									printf("Successfully deleted fulfillment %s\n", fulfillId);
								}
								break;
							}
						}
					}
				}
			}

			/*
			 *	Cleanup
			 */
			flxActCommonLicSpcDelete(licSpc);
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Handle repair of a fulfillment.
 *
 *	@param	handle		FlxActHandle used to access trusted storage
 *	@param	fulfillID	FulfillmentID to repair
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static
FlxActBool
sHandleRepair(
	FlxActHandle	handle,
	const char *	fulfillId)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActBool			bFound = FLX_ACT_FALSE;
	FlxActSvrRepair		repair = 0;
	FlxActError			error;
	FlxActLicSpc		licSpec = 0;
	FlxActProdLicSpc	prodSpec = 0;
	uint32_t			trustflags = 0;
	unsigned int		count = 0;
	unsigned int		i = 0;
	const char *		pszEntitlementId = NULL;
	const char *		pszProdId = NULL;
	const char *		pszFulfillId = NULL;
	const char *		pszSuiteId = NULL;
	const char *		pszFrUniqueId = NULL;
	const char *		pszOpsError = NULL;
	char *				pszOutput = NULL;
	uint32_t			tid = 0;
    FlxActSvrType   	serverType = flxActSvrTypeStandalone;
    FlxActBool      	bIsMaster = FLX_ACT_FALSE;

	if(handle && fulfillId)
	{
		/*
		 *	Create a return
		 */
		if(FLX_ACT_FALSE == flxActSvrRepairCreate(handle, &repair))
		{
			DEBUG("ERROR: flxActSvrRepairCreate\n");
			return FLX_ACT_FALSE;
		}

		if (NULL == g_pszCommServer)
		{
			g_pszCommServer = getenv("COMM_SERVER");
			if (NULL == g_pszCommServer)
			{
				g_pszCommServer = getenv("LM_LICENSE_FILE");
				if(NULL == g_pszCommServer)
				{
					g_pszCommServer = "@localhost";
				}
			}
		}
		/*
		 *	Find out which one we want to return
		 */
		bRV = flxActCommonLicSpcCreate(handle, &licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			DEBUG("ERROR: flxActCommonLicSpcCreate\n");
			flxActSvrRepairDelete(repair);
			return bRV;
		}
		/*
		 *	Populate from TS
		 */
		bRV = flxActCommonLicSpcPopulateAllFromTS(licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonLicSpcDelete(licSpec);
			flxActSvrRepairDelete(repair);
			return bRV;
		}

		/*
		 *	Figure out how many there are and which one we want
		 */
		count = flxActCommonLicSpcGetNumberProducts(licSpec);
		if(count)
		{
			for(i = 0; i < count; i++)
			{
				prodSpec = flxActCommonLicSpcGet(licSpec, i);
				if(0 == prodSpec)
				{
					DEBUG("ERROR: flxActCommonLicSpcGet - Invalid index\n");
					flxActCommonLicSpcDelete(licSpec);
					flxActSvrRepairDelete(repair);
					return FLX_ACT_FALSE;
				}
				/*
				 *	Check to see if this is trusted or not
				 */
				if(FLAGS_FULLY_TRUSTED == flxActCommonProdLicSpcTrustFlagsGet(prodSpec))
				{
					/*
					 *	Fully trusted, no need to do repair
					 */
					continue;
				}
				/*
				 *	Check to see if this is the one we want
				 */
				pszFulfillId = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
				if(pszFulfillId && (0 == strcmp(pszFulfillId, fulfillId)))
				{
					flxActSvrRepairProdLicSpcSet(repair, prodSpec);
					bFound = FLX_ACT_TRUE;
					break;
				}
			}
		}
		else
		{
			DEBUG("WARNING: No licenses in Trusted Storage\n");
			flxActCommonLicSpcDelete(licSpec);
			flxActSvrRepairDelete(repair);
			return bRV;
		}

		if(FLX_ACT_FALSE == bFound)
		{
			DEBUG("ERROR: Unable to find fulfillment id %s\n", fulfillId);
			flxActCommonLicSpcDelete(licSpec);
			flxActSvrRepairDelete(repair);
			return bRV;
		}

		setCommDetails(handle);

		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the repair request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActSvrRepairVendorDataSet(repair, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		/*
		 *	Add entry to indicate whether we are in 3-Server mode
		 */
		if ( !flxActSvrGetType(&serverType, &bIsMaster, &error))
		{
			DEBUG("ERROR: Could not determine 3-Server status (%d,%d,%d)\n", error.majorErrorNo, 
																			 error.minorErrorNo,
																			 error.sysErrorNo);
			
			flxActCommonLicSpcDelete(licSpec);
			flxActSvrRepairDelete(repair);
			return bRV;
    	}
		flxActSvrRepairVendorDataSet(repair,"3STS_MODE", serverType==flxActSvrTypeStandalone ? "false" : "true");

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed return request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActSvrRepairReqSet(repair, (const char**)&pszOutput))
			{
				bRV = FLX_ACT_TRUE;
				if(g_pszOutputFile)
				{
					FILE *	fp = NULL;

					fp = fopen(g_pszOutputFile, "w+");
					if(fp)
					{
						fwrite(pszOutput, strlen(pszOutput), 1, fp);
						fclose(fp);
					}
					else
					{
						DEBUG("ERROR: fopen\n");
					}
				}
				else
				{
					/*
					 *	Output to stdout
					 */
					printf("%s\n", pszOutput);
				}
				if(pszOutput)
				{
					pszOutput = NULL;
				}
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: flxActSvrRepairReqSet - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}

		}
		else
		{
			if(flxActSvrRepairSend(repair, &error))
			{
				DEBUG("SUCCESSFULLY SENT REPAIR REQUEST\n");
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				if(LM_TS_OPERATIONS == error.majorErrorNo)
				{
					pszOpsError = flxActCommonHandleGetLastOpsErrorString(handle);
					if(pszOpsError)
					{
						printf("Operations error: %d %s\n", flxActCommonHandleGetLastOpsError(handle), pszOpsError);
					}
				}
				else
				{
					DEBUG("ERROR: flxActSvrRepairSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
				}
			}
		}

		/*
		 *	Cleanup
		 */
		if(licSpec)
		{
			flxActCommonLicSpcDelete(licSpec);
		}
		flxActSvrRepairDelete(repair);

	}
	return bRV;
}


/****************************************************************************/
/**	@brief	Determine if path is a file or directory
 *
 *	@param	filePath	Path to determine if file or directory
 *
 *	@return	FLX_ACT_TRUE if directory (default) or FLX_ACT_FALSE if file.
 ****************************************************************************/
static
FlxActBool
sIsADirectory(const char * filePath, struct stat * pBuf)
{
	FlxActBool		bRV = FLX_ACT_TRUE;	/* default to true */

	if(filePath && pBuf)
	{
		if(0 == stat(filePath, pBuf))
		{
			if(!(pBuf->st_mode & S_IFDIR))
			{
				bRV = FLX_ACT_FALSE;
			}
		}
	}
	return bRV;
}

/****************************************************************************/
/* Read the contents of a file into allocated memory.  Caller to free.
 *
 * return	FLX_ACT_TRUE one success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool sAllocReadFile(const char* filePath, char** ppFileContent)
{
	FlxActBool	bRV = FLX_ACT_FALSE;
	struct stat	statBuf;

	/* stat to check that it's a file and get its size */
	if (sIsADirectory(filePath, &statBuf))
	{
		printf("%s is not a file\n", filePath);
	}
	else
	{
		/* Open the file */
		FILE* fp = fopen(filePath, "r");
		if(NULL == fp)
		{
			printf("Can\'t open %s (errno %d)\n", filePath, errno);
		}
		else
		{
			/* Allocate a buffer for the ASR */
			*ppFileContent = calloc(sizeof(char) * (statBuf.st_size + 1), 1);
			if(NULL == ppFileContent)
			{
				printf("No memory\n");
			}
			else
			{
				/* Read in the file content */
				(void)fread(*ppFileContent, sizeof(char), statBuf.st_size, fp); 
				bRV = FLX_ACT_TRUE;
			}
			fclose(fp);
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Check local activation
 *
 * Check whether an ASR has already been addded to tusted storage
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	filePath	Path of the ASR file
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sCheckLocalActivation(
	FlxActHandle	handle,
	const char *	filePath)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FILE*			fp = NULL;
	FlxActLicSpc	licSpc = 0;
	FlxActError		error;

	if(handle == NULL || filePath == NULL)
		return FLX_ACT_FALSE;

	printf("Checking ASR %s.\n", filePath);

	/*
	 *	Repair broken segments (if any) without repairing broken records to
	 *	ensure that we can check the local activation.
	 */
	bRV = flxActCommonRepairLocalTrustedStorage(handle);
	if(FLX_ACT_FALSE == bRV)
	{
		flxActCommonHandleGetError(handle, &error);
		DEBUG("ERROR: flxActCommonRepairLocalTrustedStorage - (%d,%d,%d)\n",
			error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		return bRV;
	}

	if ( !flxActCommonLicSpcCreate(handle, &licSpc))
	{
		printf("Error: no memory\n");
		return FLX_ACT_FALSE;
	}
	else
	{
		uint32_t trialId;	/* Will be set to the ASR's TrialId, or zero if none */
		time_t	 startTime;	/* If the ASR has a TrialId, this will be set to the time 
							   that the ASR was added, or zero if it was never added */
		if(g_bFromBuffer)
		{
			/* Read in the file */
			char *pszAsrXml;
			if (sAllocReadFile(filePath, &pszAsrXml))
			{
				/* Check the ASR contents read in */
				bRV = flxActCommonLicSpcCheckASRFromBuffer(licSpc, pszAsrXml, &trialId, &startTime);
				free(pszAsrXml);
			}
			else
			{
				flxActCommonLicSpcDelete(licSpc);
				return FLX_ACT_FALSE;
			}
		}
		else
		{
			/* Check the ASR file */
			bRV = flxActCommonLicSpcCheckASR(licSpc, filePath, &trialId, &startTime);
		}
		if (!bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: %s - (%d,%d,%d)\n",
					g_bFromBuffer? "flxActCommonLicSpcCheckASRFromBuffer" : "flxActCommonLicSpcCheckASR",
					error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			/* Give more information on likely errors */
			switch (error.sysErrorNo)
			{
			case LM_TSSE_ASR_CHECK_IS_DIR:	printf("%s is a directory; must specify a file\n", filePath);
											break;
			case LM_TSSE_ASR_PATH:			printf("Can\'t open %s\n", filePath);
											break;
			case LM_TSSE_ASR_NOT_XML:		printf("Not an ASR (not valid XML)\n");
											break;
			case LM_TSSE_ASR_NOT_ASR:		printf("Not an ASR (wrong XML root name)\n");
											break;
			case LM_TSSE_ASR_XML_ELEMENT_MISSING:	printf("Not a valid ASR (element missing)\n");
													break;
			case LM_TSSE_ASR_SIGNATURE:		printf("ASR signature is invalid\n");
											break;
			case LM_TSSE_ASR_WRONG_SERVER_MODE:		printf("ASR has wrong server mode (is not server).\n");
													break;
			}
		}
		else
		{
			if (trialId != 0)
			{
				if (startTime == 0)
				{
					printf("The ASR's trial (Id %u) has not been started.\n", trialId);
				}
				else
				{
					struct tm* startTm = gmtime(&startTime);
					printf("The ASR's trial (Id %u) was started on (ymd) %04d/%02d/%02d UTC (Coordinated Universal Time).\n",
						trialId, startTm->tm_year + 1900, startTm->tm_mon + 1, startTm->tm_mday  );
				}
			}
			/*	If the ASR's fullfillment record exists in trusted storage, it will have been 
				added to licSpc
			*/
			if (flxActCommonLicSpcGetNumberProducts(licSpc) == 0)
			{
				printf("No fulfillment record for this ASR exists in Trusted Storage.\n");
			}
			else
			{
				/* The fulfillment created when the ASR was loaded exists in TS */
				printf("The fulfillment record created when the ASR was loaded exists.\n");
			}
		}
		flxActCommonLicSpcDelete(licSpc);
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Process local activations
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	filePath	Path to the ASR file
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sHandleLocalActivation(
	FlxActHandle	handle,
	const char *	filePath)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActLicSpc	licSpc = 0;
	FlxActError		error;
	FILE *			fp = NULL;
	char *			pszData = NULL;
	struct stat		buf;

	DEBUG("Loading ASR %s\n", filePath);
	if(handle)
	{
		/*
		 *	Repair broken segments (if any) without repairing broken records to
		 *	ensure that we can do the local activation.
		 */
		bRV = flxActCommonRepairLocalTrustedStorage(handle);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: flxActCommonRepairLocalTrustedStorage - (%d,%d,%d)\n",
				error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			return bRV;
		}

		bRV = flxActCommonLicSpcCreate(handle, &licSpc);
		if(bRV)
		{
			bRV = flxActCommonLicSpcPopulateFromTS(licSpc);
			if(bRV)
			{
				if(g_bFromBuffer)
				{
					/*
					 *	Determine if a file or directory, for buffer only support file
					 */
					if(sIsADirectory(filePath, &buf))
					{
						/*
						 *	Only support single file now versus a directory
						 */
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonLicSpcAddASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						flxActCommonLicSpcDelete(licSpc);
						return FLX_ACT_FALSE;
					}
					fp = fopen(filePath, "r");
					if(NULL == fp)
					{
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonLicSpcAddASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						flxActCommonLicSpcDelete(licSpc);
						return FLX_ACT_FALSE;
					}
					pszData = malloc(sizeof(char) * (buf.st_size + 1));
					if(NULL == pszData)
					{
						fclose(fp);
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonLicSpcAddASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						flxActCommonLicSpcDelete(licSpc);
						return FLX_ACT_FALSE;
					}
					{
						size_t bytesRead = fread(pszData, sizeof(char), buf.st_size, fp);
						pszData[bytesRead] = '\0';
					}
					fclose(fp);
					fp = NULL;

					bRV = flxActCommonLicSpcAddASRFromBuffer(licSpc, pszData);
					/*
					 *	Clean up buffer
					 */
					free(pszData);
					pszData = NULL;
					if(FLX_ACT_FALSE == bRV)
					{
						flxActCommonHandleGetError(handle, &error);
						DEBUG("ERROR: flxActCommonLicSpcAddASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						flxActCommonLicSpcDelete(licSpc);
						return bRV;
					}
				}
				else if(FLX_ACT_FALSE == flxActCommonLicSpcAddASRs(licSpc, filePath))
				{
					flxActCommonHandleGetError(handle, &error);
					DEBUG("ERROR: flxActCommonLicSpcAddASRs - (%d,%d,%d)\n",
							error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
					flxActCommonLicSpcDelete(licSpc);
					return bRV;
				}
				DEBUG("Loaded AvailableASRs\n");
			}
			/*
			 *	Cleanup
			 */
			flxActCommonLicSpcDelete(licSpc);
		}
	}

	return bRV;
}

/****************************************************************************/
/**	@brief	Process trial reset asr(s)
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	filePath	Path where the ASR files can be found
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sHandleTrialReset(
	FlxActHandle	handle,
	const char *	filePath)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActLicSpc	licSpc = 0;
	FlxActError		error;
	FILE *			fp = NULL;
	char *			pszData = NULL;
	struct stat		buf;

	DEBUG("Resetting trial(s) with %s\n", filePath);
	if(handle)
	{
		/*
		 *	Repair broken segments (if any) without repairing broken records to
		 *	ensure that we can do the local activation.
		 */
		bRV = flxActCommonRepairLocalTrustedStorage(handle);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: flxActCommonRepairLocalTrustedStorage - (%d,%d,%d)\n",
				error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			return bRV;
		}

		bRV = flxActCommonLicSpcCreate(handle, &licSpc);
		if(bRV)
		{
			bRV = flxActCommonLicSpcPopulateFromTS(licSpc);
			if(bRV)
			{
				if(g_bFromBuffer)
				{
					/*
					 *	Determine if a file or directory, for buffer only support file
					 */
					if(sIsADirectory(filePath, &buf))
					{
						/*
						 *	Only support single file to now versus a directory
						 */
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonResetTrialASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						flxActCommonLicSpcDelete(licSpc);
						return FLX_ACT_FALSE;
					}
					fp = fopen(filePath, "r");
					if(NULL == fp)
					{
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonResetTrialASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						flxActCommonLicSpcDelete(licSpc);
						return FLX_ACT_FALSE;
					}
					pszData = malloc(sizeof(char) * (buf.st_size + 1));
					if(NULL == pszData)
					{
						fclose(fp);
						error.majorErrorNo = LM_TS_BADPARAM;
						error.minorErrorNo = 0;
						error.sysErrorNo = 0;
						DEBUG("ERROR: flxActCommonResetTrialASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						flxActCommonLicSpcDelete(licSpc);
						return FLX_ACT_FALSE;
					}
					{
						size_t bytesRead = fread(pszData, sizeof(char), buf.st_size, fp);
						pszData[bytesRead] = '\0';
					}
					fclose(fp);
					fp = NULL;

					bRV = flxActCommonResetTrialASRFromBuffer(licSpc, pszData);
					/*
					 *	Clean up buffer
					 */
					free(pszData);
					pszData = NULL;
					if(FLX_ACT_FALSE == bRV)
					{
						flxActCommonHandleGetError(handle, &error);
						DEBUG("ERROR: flxActCommonResetTrialASRs - (%d,%d,%d)\n",
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						flxActCommonLicSpcDelete(licSpc);
						return bRV;
					}
				}
				else if(FLX_ACT_FALSE == flxActCommonResetTrialASRs(licSpc, filePath))
				{
					flxActCommonHandleGetError(handle, &error);
					DEBUG("ERROR: flxActCommonResetTrialASRs - (%d,%d,%d)\n",
							error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
					flxActCommonLicSpcDelete(licSpc);
					return bRV;
				}
				DEBUG("Reset trials with available ASRs\n");
			}
			/*
			 *	Cleanup
			 */
			flxActCommonLicSpcDelete(licSpc);
		}
	}

	return bRV;
}


/****************************************************************************/
/**	@brief	Main entry point
 *
 *	@param	argc	Number of command line arguments
 *	@param	argv	Array of command line arguments
 *
 ****************************************************************************/
#ifdef PC 
int
wmain(int argc, wchar_t * argv[])
#else
int
main(int argc, char * argv[])
#endif
{
	FlxActHandle				handle = 0;
	FlxActMode					mode = FLX_ACT_PREP;
	FlxActError					error;
	int							initErr;
	void *						pData = NULL;
	OPERATION					operation = OP_TS_VIEW;
#ifdef PC	
    char *                  pszUTF8AppactUtilArg = NULL; 
#endif	

	if(argc < 2)
	{
		sUsage();
		exit(1);
	}

	memset(&error, 0, sizeof(FlxActError));

	/*
	 *	Handle processing command line arguments, whatever they are.
	 *	Default mode is for this to function as a client activation application.
	 */

	/*
	 *	Figure out what operation we're performing and initialize necessary resources
	 */
	operation = sWhichOperation(argc, argv);

	if( (initErr=flxActCommonLibraryInit(NULL)) != flxInitSuccess)
	{
		DEBUG("ERROR: Activation library initialization failed: status %d\n", initErr);
		return -1;
	}

	/*
	 *	Create handle
	 */

	if(flxActCommonHandleOpen(&handle, mode, &error))
	{
		/*
		 * Check that the operation is valid for this server.
		 */
		if (!sIsOperationAllowed(operation))
		{
			DEBUG("ERROR: Operation not allowed.\n");
		}
		else
		{
#ifdef PC
		pszUTF8AppactUtilArg = sConvertWCToUTF8(argv[2]);
#endif			
			switch(operation)
			{
				case OP_TS_SERVEDACTIVATION:
					sHandleActivation(handle);
					break;

				case OP_TS_LOCALACTIVATION_CHECK:
					if (argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sCheckLocalActivation(handle, argument2);
					break;

				case OP_TS_LOCALACTIVATION:
					if (argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleLocalActivation(handle, argument2);
					break;

				case OP_TS_TRIAL_RESET:
					if (argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleTrialReset(handle, argument2);
					break;

				case OP_TS_RETURN:
					if (argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleReturn(handle, argument2);
					break;

				case OP_TS_REPAIR:
					if(argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleRepair(handle, argument2);
					break;

				case OP_TS_DELETE:
					if(argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleDelete(handle, argument2);
					break;

				case OP_TS_PROCESS:
					(void)sHandleManualProcessing(handle);
					break;

				case OP_TS_VIRT:
					sVirtualizationExample(handle);
					break;

				case OP_TS_VIEW:
				case OP_TS_UNKNOWN:

				default:
					sDumpTS(handle);
					break;
			}
		}
#ifdef PC
		if(pszUTF8AppactUtilArg)
			free(pszUTF8AppactUtilArg);
#endif			
		flxActCommonHandleClose(handle);
	}
	else
	{
			DEBUG("ERROR: flxActCommonHandleOpen - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
	}
	/*
	 *	Cleanup
	 */
	flxActCommonLibraryCleanup();
	return 0;
}




