/**************************************************************************************************
* Copyright (c) 1997-2021 Flexera. All Rights Reserved.
**************************************************************************************************/

/**	@file 	appActUtil.c
 *	@brief	Test activation application

 *
 *	This is the source code for a simple client/server activation application whose
 *	primary purpose is for internal testing.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "FlxActCommon.h"
#include "FlxActError.h"
#include "FlxActApp.h"
#include "FlxActBorrow.h"
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

typedef enum
{
	OP_TS_UNKNOWN = 0,
	OP_TS_VIEW = 1,
	OP_TS_LOCALACTIVATION,
	OP_TS_LOCALACTIVATION_CHECK,
	OP_TS_SERVEDACTIVATION,
	OP_TS_RETURN,
	OP_TS_PROCESS,
	OP_TS_REPAIR,
	OP_TS_SHORT_CODE_ACTIVATION,
	OP_TS_SHORT_CODE_REPAIR,
	OP_TS_SHORT_CODE_RETURN,
	OP_TS_DELETE,
	OP_TS_DELETE_PRODUCT,
	OP_TS_LOCAL_REPAIR,
	OP_TS_UNIQUE_MACHINE_NUMBER,
	OP_TS_TRIAL_RESET,
	OP_TS_BORROW,
	OP_TS_RETURN_BORROW,
	OP_TS_VIRT,
	OP_TS_SERVER_VIEW

} OPERATION;

/*
 *	Application globals
 */
FlxActBool	g_bGenerateOnly = FLX_ACT_FALSE;
FlxActBool	g_bValidOnly = FLX_ACT_FALSE;
FlxActBool	g_bBrokenOnly = FLX_ACT_FALSE;
FlxActBool	g_bLongView = FLX_ACT_FALSE;
FlxActBool	g_bCancel = FLX_ACT_FALSE;
FlxActBool	g_bShortCodePending = FLX_ACT_FALSE;
FlxActBool	g_bShortCode = FLX_ACT_FALSE;
FlxActBool	g_bFromBuffer = FLX_ACT_FALSE;
char *		g_pszASRFile = NULL;
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
char *		g_pszFRId = NULL;
void *		g_pMyData = 0;
uint32_t	g_dwDuration = 0;
OPERATION	g_Operation = OP_TS_VIEW;

/* proxy details */
char		g_pszProxyHost[128];
uint32_t	g_pszProxyPort;
char		g_pszProxyUserid[128];
char		g_pszProxyPassword[128];

/*
 *	Application defines
 */
#define OPTION_VIEW					"-view"
#define OPTION_SERVER_VIEW          "-serverview"
#define OPTION_LOCAL				"-local"
#define OPTION_LOCAL_CHECK			"-localcheck"
#define OPTION_SERVED				"-served"
#define OPTION_COMM					"-comm"
#define OPTION_COMM_SERVER			"-commServer"
#define OPTION_PROXY_DETAILS		"-proxyDetails"
#define OPTION_RETURN				"-return"
#define OPTION_PROCESS				"-process"
#define OPTION_GENERATE				"-gen"
#define OPTION_REPAIR				"-repair"
#define OPTION_ENTITLEMENT_ID		"-entitlementID"
#define OPTION_REASON				"-reason"
#define OPTION_PRODUCT_ID			"-productID"
#define OPTION_EXPIRATION			"-expiration"
#define OPTION_DURATION				"-duration"
#define OPTION_VALID_ONLY			"-valid"
#define OPTION_BROKEN_ONLY			"-broken"
#define OPTION_LONG_VIEW			"-long"
#define OPTION_SHORT_CODE			"-shortcode"
#define OPTION_SHORT_CODE_PENDING	"-pending"
#define OPTION_CANCEL				"-cancel"
#define OPTION_DELETE				"-delete"
#define OPTION_VENDOR_DATA			"-vendordata"
#define OPTION_DELETE_PRODUCT		"-delproduct"
#define OPTION_LOCAL_FROM_BUFFER	"-buffer"
#define OPTION_LOCAL_REPAIR			"-localrepair"
#define OPTION_UNIQUE_MACHINE_NUMBER	"-unique"
#define OPTION_TRIAL_RESET			"-reset"
#define DEFAULT_EXPIRATION			"31-dec-2030"
#define DEFAULT_ENTITLEMENT_ID		"served-123"
#define OPTION_BORROW				"-borrow"
#define OPTION_RETURN_BORROW		"-rborrow"
#define OPTION_VIRT                 "-virtual"

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

/* temporary define here */
#define DEBUG printf

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
	printf("Usage:\tappactutil -view [-valid | -broken] [-long]\n");
	printf("\tappactutil -serverview -commServer <port@host> [-valid | -broken] [-long] [-entitlementID <entitlement_ID>] [-productID <product_ID>]\n");
	printf("\tappactutil -localcheck <asr_file> [-buffer]\n");
	printf("\tappactutil -local <asr_directory|asr_file> [-buffer]\n");
	printf("\tappactutil -reset <asr_directory|asr_file> [-buffer]\n");
	printf("\tappactutil -served [-comm <flex|soap>]\n");
	printf("\t                   [-commServer <comm server>]\n");
	printf("\t                   [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                   [-entitlementID <entitlement_ID>]\n");
	printf("\t                   [-reason <reason_number>]\n");
	printf("\t                   [-productID <product_ID>]\n");
	printf("\t                   [-expiration <expiration_date>]\n");
	printf("\t                   [-duration <duration_in_seconds>]\n");
	printf("\t                   [-vendordata <key> <value>]\n");
	printf("\t                   [-gen [<output_filename>]]\n");
	printf("\tappactutil -return <fulfillmentID>\n");
	printf("\t                   [-reason <reason_number>]\n");
	printf("\t                   [-comm <flex|soap>]\n");
	printf("\t                   [-commServer <comm server>]\n");
	printf("\t                   [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                   [-gen [<output_filename>]]\n");
	printf("\t                   [-vendordata <key> <value>]\n");
	printf("\tappactutil -borrow [-commServer <comm server>]\n");
	printf("\t                   [-entitlementID <entitlement_ID>]\n");
	printf("\t                   [-productID <product_ID>]\n");
	printf("\t                   [-expiration <expiration_date>]\n");
	printf("\tappactutil -rborrow <fulfillmentID>\n");
	printf("\t                   [-commServer <comm server>]\n");
	printf("\tappactutil -return <fulfillmentID> -cancel\n");
	printf("\tappactutil -process <input_file>\n");
	printf("\tappactutil -repair <fulfillmentID> [-gen [<output_filename>]]\n");
	printf("\t                   [-comm <flex|soap>]\n");
	printf("\t                   [-commServer <comm server>]\n");
	printf("\t                   [-proxyDetails \"<host> <port> [<user id>] [<password>]\"]\n");
	printf("\t                   [-vendordata <key> <value>]\n");
	printf("\tappactutil -delete <fulfillmentID>\n");
	printf("\tappactutil -delproduct <productId>\n");
	printf("\tappactutil -localrepair\n");
	printf("\tappactutil -shortcode <ASR_file> [-repair | -return] [fulfillmentID]\n");
	printf("\tappactutil -shortcode <ASR_file> -pending\n");
	printf("\tappactutil -shortcode <ASR_file> -cancel\n");
	printf("\tappactutil -unique\n");
	printf("\tappactutil -virtual [-long]\n");
}



/****************************************************************************/
/**	@brief	Print Additional Error Information
 *
 * This can be extended as required
 ****************************************************************************/
void sPrintAdditionalErrorInformation(const FlxActError* pError)
{
	switch (pError->majorErrorNo)
	{
	case LM_TS_NO_MATCHING_FULFILLMENT:
		printf( "The server does not have a fulfillment record that matches the\n"
				"entitlementID specified OR the expiration date or duration specified\n"
				"is later than the expiration of the fulfillment record OR server\n"
                "Trusted Storage has become untrusted.\n");
	}
}


/****************************************************************************/
/**	@brief	Status call back
 *
 ****************************************************************************/
uint32_t statusCallback(const void* pUserData, uint32_t statusOld, uint32_t statusNew)
{
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
	int	i = 0;
#ifdef PC	
	char *pszUTF8AppactUtilArg = NULL;
	char *pszUTF8AppactUtilArg1 =  NULL;
#endif

	if(argc && argv )
	{
		for(i = 0; i < argc; i++)
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
			else if(0 == strcmp(argument, OPTION_RETURN))
			{
				if(FLX_ACT_TRUE == g_bShortCode)
					op = OP_TS_SHORT_CODE_RETURN;
				else
					op = OP_TS_RETURN;
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_pszFRId = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_REPAIR))
			{
				if(FLX_ACT_TRUE == g_bShortCode)
					op = OP_TS_SHORT_CODE_REPAIR;
				else
					op = OP_TS_REPAIR;
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;				
					g_pszFRId = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_SHORT_CODE))
			{
				g_bShortCode = FLX_ACT_TRUE;
				/*
				 *	Determine what type of short code operation
				 */
				if(op == OP_TS_REPAIR)
				{
					op = OP_TS_SHORT_CODE_REPAIR;
				}
				else if(op == OP_TS_RETURN)
				{
					op = OP_TS_SHORT_CODE_RETURN;
				}
				else
				{
					/*
					 *	Default is activation
					 */
					op = OP_TS_SHORT_CODE_ACTIVATION;
				}
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;					
					g_pszASRFile = argument1;
				}
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
			else if(0 == strcmp(argument, OPTION_DURATION))
			{
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;					
					g_dwDuration = atoi(argument1);
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
			else if(0 == strcmp(argument, OPTION_LONG_VIEW))
			{
				g_bLongView = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argument, OPTION_SHORT_CODE_PENDING))
			{
				if(FLX_ACT_TRUE == g_bCancel)
				{
					sUsage();
					exit(1);
				}
				g_bShortCodePending = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argument, OPTION_CANCEL))
			{
				if(FLX_ACT_TRUE == g_bShortCodePending)
				{
					sUsage();
					exit(1);
				}
				g_bCancel = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argument, OPTION_DELETE))
			{
				op = OP_TS_DELETE;
			}
			else if(0 == strcmp(argument, OPTION_DELETE_PRODUCT))
			{
				op = OP_TS_DELETE_PRODUCT;
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
			else if(0 == strcmp(argument, OPTION_LOCAL_FROM_BUFFER))
			{
				g_bFromBuffer = FLX_ACT_TRUE;
			}
			else if(0 == strcmp(argument,  OPTION_LOCAL_REPAIR))
			{
				op = OP_TS_LOCAL_REPAIR;
			}
			else if(0 == strcmp(argument, OPTION_UNIQUE_MACHINE_NUMBER))
			{
				op = OP_TS_UNIQUE_MACHINE_NUMBER;
			}
			else if(0 == strcmp(argument, OPTION_BORROW))
			{
				op = OP_TS_BORROW;
			}
			else if(0 == strcmp(argument, OPTION_RETURN_BORROW))
			{
				op = OP_TS_RETURN_BORROW;
				if((i + 1) < argc)
				{
					CONVERT_ARGUMENT;					
					g_pszFRId = argument1;
				}
			}
			else if(0 == strcmp(argument, OPTION_VIRT))
			{
				op = OP_TS_VIRT;
            }
			else if(0 == strcmp(argument, OPTION_SERVER_VIEW))
			{
				op = OP_TS_SERVER_VIEW;
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
/**	@brief	Borrow Activation
 *
 *	@param	handle	FlxActHandle to use to access trusted storage
 *
 ****************************************************************************/
static
void
sDoBorrow(FlxActHandle handle)
{

	FlxActError error;
	char pszFulfillmentId[256] = {'\0'};
	int ret = LM_BADPARAM;
	
	/*
	*	If input parameters are NULL, check to see if environment variable set,
	*	If not, then use defaults
	*/
	if(NULL == g_pszEntitlementID)
	{
		g_pszEntitlementID = getenv("ENTITLEMENT_ID");
	}
	if(NULL == g_pszExpiration)
	{
		g_pszExpiration = getenv("EXP_DATE");
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
	
	ret = flxActBorrowActivate(g_pszEntitlementID,
					g_pszProductID,
					g_pszExpiration?g_pszExpiration:DEFAULT_EXPIRATION,
					g_pszCommServer,
					pszFulfillmentId,256,
					&error);
	if (ret == LM_TS_BADEXP_DATE)
	{
		DEBUG("ERROR: flxActAppActivationExpDateSet (%d,%d,%d)\n", error.majorErrorNo,
																   error.minorErrorNo,
																   error.sysErrorNo);
		printf("Invalid expiration date passed: %s\n", g_pszExpiration);
		return;
	}

	printf("Borrow Activate\n");
	printf(" Expiration: %s \n CommServer: %s\n",
				g_pszExpiration?g_pszExpiration:DEFAULT_EXPIRATION,
				g_pszCommServer);
	if(g_pszProductID)
	{
		printf(" ProductID: %s \n", g_pszProductID);
	}
	if(g_pszEntitlementID)
	{
		printf(" EntitlementID: %s \n", g_pszEntitlementID);
	}

	if(ret != LM_NOERROR)
	{
		if(ret == LM_BADPARAM)
		{
			DEBUG("ERROR: Invalid parameters \n");
		}
		flxActCommonHandleGetError(handle, &error);
		DEBUG("ERROR: flxActBorrowActivate (%d,%d,%d)\n", error.majorErrorNo,
								error.minorErrorNo,
								error.sysErrorNo);
	}
	else
	{
		printf("FulfillmentID %s\n",pszFulfillmentId);
	}
}

/****************************************************************************/
/**	@brief	Borrow Activation Return
 *
 *	@param	handle	FlxActHandle to use to access trusted storage
 *
 ****************************************************************************/
static
void
sReturnBorrow(FlxActHandle handle)
{
	FlxActError			error;
	int ret = LM_BADPARAM;
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

	if(g_pszFRId == NULL)
	{
		DEBUG("ERROR: No FulfillmentID \n");
		return;
	}

	printf("Borrow Return\n");
	printf(" CommServer: %s\n FulfillmentID: %s\n",g_pszCommServer,g_pszFRId);
	ret = flxActBorrowReturn(g_pszFRId,g_pszCommServer,&error);
	if(ret != LM_NOERROR)
	{
		if(ret == LM_BADPARAM)
		{
		    DEBUG("ERROR: Invalid parameters \n");
		}
		flxActCommonHandleGetError(handle, &error);
		DEBUG("ERROR: flxActBorrowReturn (%d,%d,%d)\n", error.majorErrorNo,
								error.minorErrorNo,
								error.sysErrorNo);
	}
	else
	{
		printf("Done Borrow Return\n");
	}
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
/**	@brief	Format and output the deduction record entries of a specific FlxActProdLicSpc.
 *
 *	@param	dedSpc of type FlxActDedSpc.
 *
 *	@return	None
 ****************************************************************************/

static
void
sOutputDedSpc(FlxActDedSpc dedSpc)
{
	unsigned int		concurrent = 0, concurrentO = 0;
	unsigned int		hybrid = 0, hybridO = 0;
	unsigned int		activatable = 0, activatableO = 0;
	unsigned int		repairs = 0;

	const char *		pszDestinationFulfillmentId	= NULL;
	const char *		pszDestinationSystemName	= NULL;
	const char *		pszExpDate					= NULL;

	FlxActDeductionType	type = (FlxActDeductionType) FULFILLMENT_TYPE_UNKNOWN;


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
			printf("Hybrid: %u\n", hybrid);
			printf("Activatable: %u\n",	activatable);
			break;


		case DEDUCTION_TYPE_TRANSFER:
			printf("Deduction Type: TRANSFER\n");
			printf("Destination Fulfillment ID: %s\n",
					pszDestinationFulfillmentId ? pszDestinationFulfillmentId : "NONE");
			printf("Destination System Name: %s\n",
					pszDestinationSystemName ? pszDestinationSystemName : "NONE");
			printf("Expiration Date: %s\n", pszExpDate ? pszExpDate : "NONE");
			printf("Concurrent: %u\n",				concurrent);
			printf("Concurrent overdraft: %u\n",	concurrentO);
			printf("Hybrid: %u\n",					hybrid);
			printf("Hybrid overdraft: %u\n",		hybridO);
			printf("Activatable: %u\n",				activatable);
			printf("Activatable overdraft: %u\n",	activatableO);
			printf("Repairs: %u\n",					repairs);
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



	if(product)
	{
		trustflags = flxActCommonProdLicSpcTrustFlagsGet(product);
		if(g_bBrokenOnly)
		{
			if(FLAGS_FULLY_TRUSTED == trustflags)
				return;
		}
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

		/* Display the count info for serverview option. */
		if (g_Operation == OP_TS_SERVER_VIEW)
		{
			unsigned int		concurrent = 0, concurrentO = 0;
			unsigned int		hybrid = 0, hybridO = 0;
			unsigned int		activatable = 0, activatableO = 0;
			unsigned int		repairs = 0;

			concurrent = flxActCommonProdLicSpcCountGet(product, CONCURRENT);
			concurrentO = flxActCommonProdLicSpcCountGet(product, CONCURRENT_OD);

			hybrid = flxActCommonProdLicSpcCountGet(product, HYBRID);
			hybridO = flxActCommonProdLicSpcCountGet(product, HYBRID_OD);

			activatable = flxActCommonProdLicSpcCountGet(product, ACTIVATABLE);
			activatableO = flxActCommonProdLicSpcCountGet(product, ACTIVATABLE_OD);

			repairs = flxActCommonProdLicSpcCountGet(product, REPAIRS);

			printf("Concurrent: %u\n", concurrent);
			printf("Concurrent overdraft:%u\n", concurrentO);
			printf("Hybrid: %u\n", hybrid);
			printf("Hybrid overdraft: %u\n", hybridO);
			printf("Activatable: %u\n", activatable);
			printf("Activatable overdraft: %u\n", activatableO);
			printf("Repairs: %u\n", repairs);
		}

		printf("Feature line(s):\n%s\n", pszFeatureLine ? pszFeatureLine : "NONE");
		sOutputProdLicVendorDictionary(product);

		if ( g_bLongView == FLX_ACT_TRUE )
		{
			pszActServerChain = flxActCommonProdLicSpcActServerChainGet(product);
			printf("Activation Server Chain: %s\n", pszActServerChain ? pszActServerChain : "NONE");

			/* Retrieve and display the deduction records for serverview option. */
			if (g_Operation == OP_TS_SERVER_VIEW)
			{
				uint32_t			dedCount = 0;
				uint32_t			validDedCount = 0;
				uint32_t			nIndex = 0;

				dedCount = flxActCommonProdLicSpcNumberDedSpcGet(product);
				validDedCount = flxActCommonProdLicSpcNumberValidDedSpcGet(product);

				printf("Deduction Count: %u\n", dedCount);
				printf("Valid Deduction Count: %u\n\n", validDedCount);

				for ( nIndex = 0; nIndex < dedCount; nIndex++ )
				{
					sOutputDedSpc(flxActCommonProdLicSpcDedSpcGet(product, nIndex));
				}
			}
		}

		printf("--------------------------------------------------------------------\n");
	}
	return;
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
							sOutputProdLic(prodSpec);
						}
					}
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

static
FlxActBool
sPrintVmAttr(FlxActHandle handle, const char* name, FlxActBool(*vmAccessor)(FlxActHandle hndle, const char **pszStr))
{
	FlxActBool bRV = FLX_ACT_FALSE;
	FlxActError error;
	const char* vmAttrStr = NULL;

	bRV = vmAccessor( handle, &vmAttrStr );

	if( bRV )
	{
		printf("%s=%s\n", name, vmAttrStr ? vmAttrStr : "");
		free((void*)vmAttrStr);
	}
	else
	{
		const char* errorMsg;

		flxActCommonHandleGetError(handle, &error);

		errorMsg = 	(error.majorErrorNo == LM_TS_VM_ATTRIBUTE_PRIVILEGE) ?	" Insufficient privilege." :
					(error.majorErrorNo == LM_TS_VM_ATTRIBUTE_PROTECTED) ?	" Protected resource encountered." :
																			"";

		DEBUG("%s: ERROR - (%d,%d,%d)%s\n", name, error.majorErrorNo, error.minorErrorNo, error.sysErrorNo, errorMsg);
	}

	return bRV;
}

static
FlxActBool
sVirtualizationExample(FlxActHandle handle)
{
	FlxActBool			bRV = FLX_ACT_FALSE;

	if (handle)
	{
#if defined(__linux__) || defined(OS_LINUX) || defined(WINDOWS) || defined(OS_WINDOWS)

		FlxActBool	bVirtPlat=FLX_ACT_FALSE;
		bRV = flxActCommonVirtualStatusGet( handle, &bVirtPlat );

		if( !bRV )
		{
			FlxActError error;
			flxActCommonHandleGetError(handle, &error);

			if( error.majorErrorNo == LM_TS_VM_ATTRIBUTE_PRIVILEGE )
			{
				printf(	"Insufficient privilege to determine Virtual Platform Status (%d,%d,%d)\n"
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
			bRV = FLX_ACT_TRUE;							/* Optimistic */

			if( g_bLongView )
			{
				bRV = sPrintVmAttr(handle,"FAMILY",flxActCommonVirtualFamilyGet) && bRV;
				bRV = sPrintVmAttr(handle,"NAME",flxActCommonVirtualNameGet) && bRV;
				bRV = sPrintVmAttr(handle,"UUID",flxActCommonVirtualUuidGet) && bRV;
				bRV = sPrintVmAttr(handle,"GENID",flxActCommonVirtualGenidGet) && bRV;
			}
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
			/* set proxy server */
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
/**	@brief	Show the result of response processing.
*
* Print the product and fulfillment IDs of the FlxActProdLicSpc created
* when an activation response was processed successfully.
*
*	@param	handle	FlxActHandle used to access trusted storage
*
*	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
*	@note	Primary use if for manual transactions
****************************************************************************/
void sShowResponseFulfillment(FlxActHandle handle)
{
	FlxActProdLicSpc product;
	const char *	 pszProductID;
	const char *	 pszFulfillmentID;

	if (flxActCommonRespProdLicSpcGet(handle, &product))
	{
		pszProductID     = flxActCommonProdLicSpcProductIdGet(product);
		pszFulfillmentID = flxActCommonProdLicSpcFulfillmentIdGet(product);

		printf("ProductID %s, FulfillmentID %s\n",pszProductID ,pszFulfillmentID);
	}
	else
	{
		/* Response was not a successful activation. */
	}
}

/****************************************************************************/
/**	@brief	Set expiration date using flxActAppActivationExpDateSet()
 *
 *	@param	handle	FlxActHandle used to access trusted storage
 *	@param	act		FlxActAppActivation object
 *	@param	expDate	Expiration Date
 *	@param	err		Pointer to FlxActError structure, populated on failure
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sSetExpirationDate(FlxActHandle handle, FlxActAppActivation act, const char* expDate, FlxActError *err)
{
	const char* pszStoredDate = NULL;	
	flxActAppActivationExpDateSet(act, expDate);
	flxActCommonHandleGetError(handle, err);

	pszStoredDate = flxActAppActivationExpDateGet(act);

    return ( !pszStoredDate || strcmp(expDate, pszStoredDate) ) ? FLX_ACT_FALSE : FLX_ACT_TRUE;
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
	FlxActAppActivation	client = 0;
	FlxActError			error;
	const char *		pszOutput = NULL;
	const char *		pszOpsError = NULL;
    char                szExpiration[DATE_LEN+1] = {'\0'};

	if(handle)
	{
		/*
		 *	Create a client handle
		 */
		if(FLX_ACT_FALSE == flxActAppActivationCreate(handle, &client))
		{
			DEBUG("ERROR: flxActAppActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		/*
		 *	If input parameters are NULL, check to see if environment variable set,
		 *	If not, then use defaults
		 */
		if(NULL == g_pszEntitlementID)
		{
			g_pszEntitlementID = getenv("ENTITLEMENT_ID");
		}
		if(NULL == g_pszExpiration)
		{
			g_pszExpiration = getenv("EXP_DATE");
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

		setCommDetails(handle);

		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the activation request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActAppActivationVendorDataSet(client, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		flxActAppActivationEntitlementIdSet(client, g_pszEntitlementID ? g_pszEntitlementID : DEFAULT_ENTITLEMENT_ID);
		if(g_pszProductID)
		{
			flxActAppActivationProductIdSet(client, g_pszProductID);
		}

		/*
		 *	If an activation reason has been supplied, set it
		*/
		if (NULL != g_pszReason)
		{
			if (FLX_ACT_FALSE == flxActAppActivationReasonSet(client, g_pszReason))
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: flxActAppActivationReasonSet (%d,%d,%d)\n", error.majorErrorNo,
																		  error.minorErrorNo,
																		  error.sysErrorNo);
				printf("Reason must be numeric.\n");
				flxActAppActivationDelete(client);
				return FLX_ACT_FALSE;
			}
		}

		if((NULL == g_pszExpiration) && (0 == g_dwDuration))
		{
			/*
			 *	No expiration or duration to set; the license server will default to the
			 *  expiration of the server fulfillment.
			 */
		}
		else
		{
			if(g_pszExpiration)
			{
			    if( FLX_ACT_FALSE == sSetExpirationDate(handle, client, g_pszExpiration, &error) )
				{
					flxActCommonHandleGetError(handle, &error);
					DEBUG("ERROR: flxActAppActivationExpDateSet (%d,%d,%d)\n", error.majorErrorNo,
																		  error.minorErrorNo,
																		  error.sysErrorNo);
					printf("Invalid expiration date: %s\n", g_pszExpiration);
					flxActAppActivationDelete(client);
					return FLX_ACT_FALSE;
				}
			}
			else
			{
				flxActAppActivationDurationSet(client, g_dwDuration);
			}
		}

		/*
		 *	Specify what we're requesting...
		 */
		printf("Generating activation request using:\n\tEntitlement ID = %s\n\tExpiration = %s\n",
			g_pszEntitlementID ? g_pszEntitlementID : DEFAULT_ENTITLEMENT_ID,
			g_pszExpiration ? g_pszExpiration : "default (server fulfillment expiration)");
		if(g_pszProductID)
		{
			printf("\tProduct ID = %s\n", g_pszProductID);
		}

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed activation request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActAppActivationReqSet(client, &pszOutput))
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
				DEBUG("ERROR: flxActAppActivationReqSet (%d,%d,%d)\n", error.majorErrorNo,
																		  error.minorErrorNo,
																		  error.sysErrorNo);
			}
		}
		else
		{
			/*
			 *	Generate and send
			 */
			if(flxActAppActivationSend(client, &error))
			{
				DEBUG("ACTIVATION REQUEST SUCCESSFULLY PROCESSED\n");

				sShowResponseFulfillment(handle);
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
					DEBUG("ERROR: flxActAppActivationSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
                    sPrintAdditionalErrorInformation(&error);
				}
			}
		}
		/*
		 *	Cleanup
		 */
		flxActAppActivationDelete(client);
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
	FlxActAppReturn		ret = 0;
	FlxActError			error;
	FlxActLicSpc		licSpec = 0;
	FlxActProdLicSpc	prodSpec = 0;
	unsigned int		count = 0;
	unsigned int		i = 0;
	const char *		pszFulfillId = NULL;
	const char *		pszOpsError = NULL;
	const char *		pszOutput = NULL;

	if(handle && fulfillId)
	{
		/*
		 *	Create a return
		 */
		if(FLX_ACT_FALSE == flxActAppReturnCreate(handle, &ret))
		{
			DEBUG("ERROR: flxActAppReturnCreate\n");
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
			flxActAppReturnDelete(ret);
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
			flxActAppReturnDelete(ret);
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
					flxActAppReturnDelete(ret);
					return FLX_ACT_FALSE;
				}
				/*
				 *	Check to see if this is the one we want
				 */
				pszFulfillId = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
				if(pszFulfillId && (0 == strcmp(pszFulfillId, fulfillId)))
				{
					flxActAppReturnProdLicSpcSet(ret, prodSpec);
					bFound = FLX_ACT_TRUE;
					break;
				}
			}
		}
		else
		{
			DEBUG("WARNING: No licenses in Trusted Storage\n");
			flxActCommonLicSpcDelete(licSpec);
			flxActAppReturnDelete(ret);
			return bRV;
		}

		if(FLX_ACT_FALSE == bFound)
		{
			DEBUG("ERROR: Unable to find fulfillment id %s\n", fulfillId);
			flxActCommonLicSpcDelete(licSpec);
			flxActAppReturnDelete(ret);
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
			bRV = flxActAppReturnCancel(ret);

			flxActCommonLicSpcDelete(licSpec);
			flxActAppReturnDelete(ret);
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
			flxActAppReturnVendorDataSet(ret, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		/*
		*	If a return reason has been supplied, set it
		*/
		if (NULL != g_pszReason)
		{
			if (FLX_ACT_FALSE == flxActAppReturnReasonSet(ret, g_pszReason))
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: flxActAppActivationReasonSet (%d,%d,%d)\n", error.majorErrorNo,
																		  error.minorErrorNo,
																		  error.sysErrorNo);
				printf("Reason must be numeric.\n");
				flxActCommonLicSpcDelete(licSpec);
				flxActAppReturnDelete(ret);
				return FLX_ACT_FALSE;
			}
		}

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed return request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActAppReturnReqSet(ret, &pszOutput))
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
				DEBUG("ERROR: flxActAppReturnReqSet - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
		}
		else
		{
			if(flxActAppReturnSend(ret, &error))
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
					DEBUG("ERROR: flxActAppActivationSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
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
		flxActAppReturnDelete(ret);

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



static
FlxActBool
sHandleDeleteProductFromTS(
	FlxActHandle	handle,
	const char *	productId)
{
	if(handle)
	{
		return flxActCommonHandleDeleteProduct(handle, productId);
	}
	else
		return FLX_ACT_FALSE;

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
	FlxActAppRepair		repair = 0;
	FlxActError			error;
	FlxActLicSpc		licSpec = 0;
	FlxActProdLicSpc	prodSpec = 0;
	unsigned int		count = 0;
	unsigned int		i = 0;
	const char *		pszFulfillId = NULL;
	const char *		pszOpsError = NULL;
	const char *		pszOutput = NULL;

	if(handle && fulfillId)
	{
		/*
		 *	Create a return
		 */
		if(FLX_ACT_FALSE == flxActAppRepairCreate(handle, &repair))
		{
			DEBUG("ERROR: flxActAppRepairCreate\n");
			return FLX_ACT_FALSE;
		}

		/*
		 *	Find out which one we want to return
		 */
		bRV = flxActCommonLicSpcCreate(handle, &licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			DEBUG("ERROR: flxActCommonLicSpcCreate\n");
			flxActAppRepairDelete(repair);
			return bRV;
		}
		/*
		 *	Populate from TS
		 */
		bRV = flxActCommonLicSpcPopulateAllFromTS(licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonLicSpcDelete(licSpec);
			flxActAppRepairDelete(repair);
			return bRV;
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
					flxActAppRepairDelete(repair);
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
					flxActAppRepairProdLicSpcSet(repair, prodSpec);
					bFound = FLX_ACT_TRUE;
					break;
				}
			}
		}
		else
		{
			DEBUG("WARNING: No licenses in Trusted Storage\n");
			flxActCommonLicSpcDelete(licSpec);
			flxActAppRepairDelete(repair);
			return bRV;
		}

		if(FLX_ACT_FALSE == bFound)
		{
			DEBUG("ERROR: Unable to find fulfillment id %s\n", fulfillId);
			flxActCommonLicSpcDelete(licSpec);
			flxActAppRepairDelete(repair);
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
			flxActAppRepairVendorDataSet(repair, g_pszVendorDataKey, g_pszVendorDataValue);
		}

		if(g_bGenerateOnly)
		{
			/*
			 *	Generate only
			 */
			printf("\nWriting signed return request to %s\n", g_pszOutputFile ? g_pszOutputFile : "stdout");
			if(flxActAppRepairReqSet(repair, &pszOutput))
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
				DEBUG("ERROR: flxActAppRepairReqSet - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}

		}
		else
		{
			if(flxActAppRepairSend(repair, &error))
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
					DEBUG("ERROR: flxActAppActivationSend - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
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
		flxActAppRepairDelete(repair);

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
FlxActBool 
sAllocReadFile(const char* filePath, char** ppFileContent)
{
	FlxActBool	bRV = FLX_ACT_FALSE;
	struct stat	statBuf;

	/* stat to check that it's a file and get its size */
	if (0 != stat(filePath, &statBuf))
	{
		printf("%s is not a file\n", filePath);
	}
	else if (0 != (statBuf.st_mode & S_IFDIR))
	{
		printf("%s is a directory; must specify a file\n", filePath);
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
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	filePath	Path where the ASR file can be found
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
			case LM_TSSE_ASR_WRONG_SERVER_MODE:		printf("ASR has wrong server mode (is server).\n");
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
					/*	Only the day of creation is recorded so the time returned by 
						API function flxActCommonLicSpcCheckASR is 0:00 (12am) on that day.
						We do not know explicitly what the local date of creation is.
						We could assume that the offset from UTC time now is the same as it was then, 
						but if it is not (e.g. because of daylight saving) then the date may be 1 day out.
						Rather than risk this we display it in UTC. 
					*/
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
 *	@param	filePath	Path where the ASR files can be found
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
						 *	Only support single file to now versus a directory
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
					(void)fread(pszData, sizeof(char), buf.st_size, fp);
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
					(void)fread(pszData, sizeof(char), buf.st_size, fp);
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
/**	@brief	Convert short code request type
 *
 *	@param	requestType		the member of FlxShortCodeType enumeration
 *
 *	@return	a string mapping to the enumeration.
 ****************************************************************************/
static char const * sRequestTypeAsString( uint32_t requestType )
{
	switch( requestType )
	{
	case flxShortCodeTypeNotSet: /* Request generated by an earlier version of FNP that did not store the request type. */
		return "not set";
	case flxShortCodeTypeActivation:
		return "activation";
	case flxShortCodeTypeReturn:
		return "return";
	case flxShortCodeTypeRepair:
		return "repair";
	default:
		break;
	}

	return "unknown";
}

/****************************************************************************/
/**	@brief	Get pending short code if there is one pending
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	pszASRFile	ASR file to use
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sPendingShortCode(
	FlxActHandle	handle,
	const char *	pszASRFile)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActError			error;
	const char *		pszPendingShortCode = NULL;
	uint32_t			requestType = flxShortCodeTypeNotSet;

	if(handle && pszASRFile)
	{
		/*
		 *	Get pending short code and request type. All the necessary info is in
		 *	the ASR file and you can only have one pending short code per a ASR.
		 *	If we had a buffer, we could instead call flxActAppGetPendingShortCodeFromBuffer.
		 */
		bRV = flxActAppGetPendingShortCode(handle, pszASRFile, &pszPendingShortCode, &requestType );
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			printf("Error: (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			return FLX_ACT_FALSE;
		}
		/*
		 *	Check to see if there's anything pending
		 */
		if(NULL == pszPendingShortCode)
		{
			printf("No short code pending for ASR %s\n", pszASRFile);
			return FLX_ACT_FALSE;
		}
		printf("Pending short code for %s is %s, of type %s\n", pszASRFile, pszPendingShortCode, sRequestTypeAsString( requestType ) );
		}

	return bRV;
}

/****************************************************************************/
/**	@brief	Cancel pending short code
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	pszASRFile	ASR file to use
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sCancelShortCode(
	FlxActHandle	handle,
	const char *	pszASRFile)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActError			error;
	FlxActAppActivation	client = 0;

	if(handle && pszASRFile)
	{
		/*
		 *	Create an activation handle, doesn't matter if it's a
		 *	activation, return, or repair handle in the case
		 *	of a pending short code, all the necessary info is in
		 *	the ASR file and you can only have one pending short
		 *	code per a ASR.
		 */
		if(FLX_ACT_FALSE == flxActAppActivationCreate(handle, &client))
		{
			DEBUG("ERROR: flxActAppActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		/*
		 *	Cancel request
		 *	If we had a buffer, we could instead call flxActAppActivationShortCodeCancelFromBuffer
		 */
		bRV = 	flxActAppActivationShortCodeCancel(client, pszASRFile);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			printf("Error: (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		}
		else
		{
			printf("Successfully cancelled pending short code for %s\n", pszASRFile);
		}
		if(client)
		{
			flxActAppActivationDelete(client);
		}
	}

	return bRV;
}

/****************************************************************************/
/**	@brief	Generate short code return request
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	pszASRFile	ASR file to use
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sGenShortCodeReturn(
	FlxActHandle	handle,
	const char *	pszASRFile)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActError		error;
	FlxActAppReturn	ret = 0;
	const char *	pszShortCode = NULL;

	if(handle && pszASRFile)
	{
		/*
		 *	Create a return
		 */
		bRV = flxActAppReturnCreate(handle, &ret);
		if(FLX_ACT_FALSE == ret)
		{
			DEBUG("ERROR: flxActAppReturnCreate\n");
			return FLX_ACT_FALSE;
		}

		if(g_pszFRId)
		{
			/*
			 *	Set the FR in TS to use to overwrite the one found in the ASR file
			 */
			flxActAppReturnFRIdSet(ret, g_pszFRId);
		}
		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the shortcode return request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActAppReturnVendorDataSet(ret, g_pszVendorDataKey, g_pszVendorDataValue);
		}
		/*
		 *	If we had a buffer, we could instead call flxActAppReturnShortCodeGenerateFromBuffer
		 */
		bRV = flxActAppReturnShortCodeGenerate(ret, pszASRFile, &pszShortCode);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			if(LM_TS_SHORT_CODE_PENDING == error.majorErrorNo)
			{
				printf("Error: Pending short code for %s\n", pszASRFile);
			}
			else if(LM_TS_SHORT_CODE_RETURN_CREATE == error.majorErrorNo)
			{
				printf("Error creating short code return %d %d %d\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			else
			{
				printf("Error: (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			flxActAppReturnDelete(ret);
			return FLX_ACT_FALSE;
		}
		printf("Return short code: %s\n", pszShortCode);
		/*
		 *	Cleanup
		 */
		if(ret)
		{
			flxActAppReturnDelete(ret);
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Generate short code repair request
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	pszASRFile	ASR file to use
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sGenShortCodeRepair(
	FlxActHandle	handle,
	const char *	pszASRFile)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActError		error;
	FlxActAppRepair	repair = 0;
	const char *	pszShortCode = NULL;

	if(handle && pszASRFile)
	{
		/*
		 *	Create a repair
		 */
		bRV = flxActAppRepairCreate(handle, &repair);
		if(FLX_ACT_FALSE == repair)
		{
			DEBUG("ERROR: flxActAppRepairCreate\n");
			return FLX_ACT_FALSE;
		}
		if(g_pszFRId)
		{
			/*
			 *	Set the FR in TS to use to overwrite the one found in the ASR file
			 */
			flxActAppRepairFRIdSet(repair, g_pszFRId);
		}
		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the shortcode repair request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActAppRepairVendorDataSet(repair, g_pszVendorDataKey, g_pszVendorDataValue);
		}
		/*
		 *	If we had a buffer, we could instead call flxActAppRepairShortCodeGenerateFromBuffer
		 */
		bRV = flxActAppRepairShortCodeGenerate(repair, pszASRFile, &pszShortCode);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			if(LM_TS_SHORT_CODE_PENDING == error.majorErrorNo)
			{
				printf("Error: Pending short code for %s\n", pszASRFile);
			}
			else if(LM_TS_SHORT_CODE_REPAIR_CREATE == error.majorErrorNo)
			{
				printf("Error creating short code repair %d %d %d\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			else
			{
				printf("Error: (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			flxActAppRepairDelete(repair);
			return FLX_ACT_FALSE;
		}
		printf("Repair short code: %s\n", pszShortCode);
		/*
		 *	Cleanup
		 */
		if(repair)
		{
			flxActAppRepairDelete(repair);
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Generate short code activation request
 *
 *	@param	handle		FlxActHandle to use to access trusted storage
 *	@param	pszASRFile	ASR file to use
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sGenShortCodeActivation(
	FlxActHandle	handle,
	const char *	pszASRFile)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActError			error;
	FlxActAppActivation	act = 0;
	const char *		pszShortCode = NULL;

	if(handle && pszASRFile)
	{
		/*
		 *	Create a repair
		 */
		bRV = flxActAppActivationCreate(handle, &act);
		if(FLX_ACT_FALSE == act)
		{
			DEBUG("ERROR: flxActAppActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		/*
		 *	Check if vendor data specified and add it to the PublisherDictionary
		 *	of the shortcode activation request as a key/value pair.  To add mutiple key/value
		 *	pairs, repeat this block of code.
		 */
		if(g_pszVendorDataKey && g_pszVendorDataValue)
		{
			flxActAppActivationVendorDataSet(act, g_pszVendorDataKey, g_pszVendorDataValue);
		}
		/*
		 *	If we had a buffer, we could instead call flxActAppActivationShortCodeGenerateFromBuffer
		 */
		bRV = flxActAppActivationShortCodeGenerate(act, pszASRFile, &pszShortCode);
		if(FLX_ACT_FALSE == bRV)
		{
			/*
			 *	Get error
			 */
			flxActCommonHandleGetError(handle, &error);
			if(LM_TS_SHORT_CODE_PENDING == error.majorErrorNo)
			{
				printf("Error: Pending short code for %s\n", pszASRFile);
			}
			else if(LM_TS_SHORT_CODE_ACT_CREATE == error.majorErrorNo)
			{
				printf("Error creating short code activation %d %d %d\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			else
			{
				printf("Error: (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			flxActAppActivationDelete(act);
			return FLX_ACT_FALSE;
		}
		printf("Activation short code: %s\n", pszShortCode);
		/*
		 *	Cleanup
		 */
		if(act)
		{
			flxActAppActivationDelete(act);
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Handle short codes
 *
 *	@param	handle	FlxActHandle to use to access trusted storage
 *	@param	op		Operation being requested
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE.
 ****************************************************************************/
static
FlxActBool
sHandleShortCode(
	FlxActHandle	handle,
	OPERATION		op)
{
	FlxActBool	bRV = FLX_ACT_FALSE;

	if(NULL == g_pszASRFile)
	{
		sUsage();
		return FLX_ACT_FALSE;
	}
	if(handle)
	{
		/*
		 *	Determine what operation to do
		 */
		if(g_bShortCodePending)
		{
			bRV = sPendingShortCode(handle, g_pszASRFile);
		}
		else if(g_bCancel)
		{
			bRV = sCancelShortCode(handle, g_pszASRFile);
		}
		else
		{
			if(OP_TS_SHORT_CODE_REPAIR == op)
			{
				bRV = sGenShortCodeRepair(handle, g_pszASRFile);
			}
			else if(OP_TS_SHORT_CODE_RETURN == op)
			{
				bRV = sGenShortCodeReturn(handle, g_pszASRFile);
			}
			else if(OP_TS_SHORT_CODE_ACTIVATION == op)
			{
				bRV = sGenShortCodeActivation(handle, g_pszASRFile);
			}
			else
			{
				printf("ERROR: Unsupported short code request\n");
			}
		}
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	Repair local trusted storage
 *
 *	@param	handle	FlxActHandle used to access trusted storage
 *
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static
FlxActBool
sHandleLocalRepair(FlxActHandle handle)
{
	FlxActBool		bRV = FLX_ACT_FALSE;
	FlxActError		error;

	if(handle)
	{
		bRV = flxActCommonRepairLocalTrustedStorage(handle);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: Local repair - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		}
	}
	return bRV;
}

static
FlxActBool
sUniqueMachineNumber(FlxActHandle handle)
{
	FlxActBool		bRV = FLX_ACT_TRUE;
	FlxActError		error;
	const char *	pszUMN = NULL;
	FlxActBool		bIsVirtual = FLX_ACT_FALSE;

	if(handle)
	{
		pszUMN = flxActCommonHandleGetUniqueMachineNumber(handle, flxUMNTypeOne);
		if(pszUMN)
		{
			printf("Unique Machine Number type one is   %s\n", pszUMN);
		}
		else
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: Unique Machine Number type one - (%d,%d,%d)%s\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo,
						(error.majorErrorNo == LM_TS_CANT_FIND)? ". Not available on this machine." : "");
			bRV = FLX_ACT_FALSE;
		}

		pszUMN = flxActCommonHandleGetUniqueMachineNumber(handle, flxUMNTypeTwo);
		if(pszUMN)
		{
			printf("Unique Machine Number type two is   %s\n", pszUMN);
		}
		else
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: Unique Machine Number type two - (%d,%d,%d)%s\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo,
						(error.majorErrorNo == LM_TS_CANT_FIND)? ". Not available on this machine." : "");
			bRV = FLX_ACT_FALSE;
		}

#if defined(__linux__) || defined(OS_LINUX) || defined(WINDOWS) || defined(OS_WINDOWS)
		/* UMN3 & UMN5 are only available from virtual environments. */
		if (flxActCommonVirtualStatusGet(handle, &bIsVirtual))
		{
			if (bIsVirtual)
			{	
				pszUMN = flxActCommonHandleGetUniqueMachineNumber(handle, flxUMNTypeThree);
				if(pszUMN)
				{
					printf("Unique Machine Number type three is %s\n", pszUMN);
				}
				else
				{
					flxActCommonHandleGetError(handle, &error);
					DEBUG("ERROR: Unique Machine Number type three - (%d,%d,%d)%s\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo,
								(error.majorErrorNo == LM_TS_CANT_FIND)? ". Not available on this machine." : "");
					bRV = FLX_ACT_FALSE;
				}

				pszUMN = flxActCommonHandleGetUniqueMachineNumber(handle, flxUMNTypeFive);
				if(pszUMN)
				{
					printf("Unique Machine Number type five is %s\n", pszUMN);
				}
				else
				{
					flxActCommonHandleGetError(handle, &error);
					DEBUG("ERROR: Unique Machine Number type five - (%d,%d,%d)%s\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo,
								(error.majorErrorNo == LM_TS_CANT_FIND)? ". Not available on this machine." : "");
					bRV = FLX_ACT_FALSE;
				}

			}
		}
		else
		{
			flxActCommonHandleGetError(handle, &error);
			DEBUG("ERROR: flxActCommonVirtualStatusGet - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			bRV = FLX_ACT_FALSE;
		}
#endif
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
	FlxActAppActivation	client = 0;
	FlxActError			error;
	char *				pszInput = NULL;
	char				buffer[1024] = {'\0'};
	long				size = 0;
	long				bytesread = 0;

	if(handle)
	{
		/*
		 *	Create a client handle
		 */
		if(FLX_ACT_FALSE == flxActAppActivationCreate(handle, &client))
		{
			DEBUG("ERROR: flxActAppActivationCreate\n");
			return FLX_ACT_FALSE;
		}
		printf("\nReading response from %s\n", g_pszInputFile ? g_pszInputFile : "stdin");
		if(g_pszInputFile)
		{
			FILE *	fp = NULL;

			fp = fopen(g_pszInputFile, "rb");
			if(fp)
			{
				(void)fseek(fp, 0L, SEEK_END);
				size = ftell(fp);
                if ( size == -1L )
                {
					DEBUG("ERROR: ftell\n");
					flxActAppActivationDelete(client);
					fclose(fp);
					return FLX_ACT_FALSE;
                }                    
				(void)fseek(fp, 0L, SEEK_SET);
				pszInput = malloc(sizeof(char) * (size + 1));
				if(pszInput)
				{
					(void)fread(pszInput, sizeof(char), size, fp);
					pszInput[size] = '\0';	/* NULL terminate */
					fclose(fp);
					fp = NULL;
				}
				else
				{
					DEBUG("ERROR: malloc\n");
					flxActAppActivationDelete(client);
					fclose(fp);
					return FLX_ACT_FALSE;
				}
			}
			else
			{
				DEBUG("ERROR: fopen\n");
				flxActAppActivationDelete(client);
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
					/* NULL terminate */
					pszInput[size] = '\0';
				}
				else
				{
					DEBUG("ERROR: malloc\n");
					flxActAppActivationDelete(client);
					return FLX_ACT_FALSE;
				}
				bytesread = read(0, &buffer, sizeof(buffer));
			}
		}
		if(pszInput)
		{
			if(flxActAppActivationRespProcess(client, (const char *)pszInput, &bIsTrustedConfig))
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

                sShowResponseFulfillment(handle);
			}
			else
			{
				flxActCommonHandleGetError(handle, &error);
				DEBUG("ERROR: Processing response - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
			}
			free(pszInput);
			pszInput = NULL;
		}
		else
		{
			DEBUG("ERROR: NO INPUT\n");
		}
		flxActAppActivationDelete(client);
	}
	return bRV;

}

/****************************************************************************/
/**	@brief			Display the Fulfillment count summary from server trusted storage.
 *
 *	@param	handle	FlxActHandle used to access local trusted storage.
 *
 *	@return			None.
 ****************************************************************************/

static void sDumpServerFRCntSummary(FlxActHandle handle)
{
	uint32_t  nTrustedFRCount = 0, nTotalFRCount = 0;
	FlxActError		error;
	FlxActBool bReturn = FLX_ACT_FALSE;

	memset(&error, 0, sizeof(FlxActError));

	/* 
	 * Retrieve the trusted and total fulfillment record count from server trusted storage. 
	 */

	bReturn = flxActCommonGetFRCountFromServerTS(handle, g_pszEntitlementID, g_pszProductID, g_pszCommServer, &nTrustedFRCount, &nTotalFRCount);

	if (FLX_ACT_FALSE == bReturn)
	{
		flxActCommonHandleGetError(handle, &error);
		DEBUG("ERROR: flxActCommonGetFRCountFromServerTS (%d,%d,%d)\n", error.majorErrorNo,
																		  error.minorErrorNo,
																			error.sysErrorNo);
		return;
	}

	printf("\n--------------------------------------------------------------------");
	printf("\nServer Trusted Storage Fulfillment Count Summary");
	printf("\n--------------------------------------------------------------------");
	printf("\nLicense Server           : %s", g_pszCommServer);
	printf("\nEntitlement Id           : %s", g_pszEntitlementID ? g_pszEntitlementID : "NONE");
	printf("\nProduct Id               : %s", g_pszProductID ? g_pszProductID : "NONE");
	printf("\nTrusted Fulfillment Count: %u", nTrustedFRCount );
	printf("\nTotal Fulfillment Count  : %u", nTotalFRCount );
	printf("\n--------------------------------------------------------------------\n");

	return;
}

/****************************************************************************/
/**	@brief	Display the contents of server trusted storage
 *
 *	@param	handle	FlxActHandle to use to access trusted storage
 *
 *	@return	FLX_ACT_TRUE  on success. 
 *          FLX_ACT_FALSE on failure.
 ****************************************************************************/

static
FlxActBool
sDumpServerTS(FlxActHandle handle)
{
	FlxActBool		bReturn = FLX_ACT_FALSE;
	FlxActError		error;
	FlxActLicSpc		licSpc = 0;
	FlxActProdLicSpc	prodSpec = 0;

	if (!handle)
		return bReturn;

	if (NULL == g_pszCommServer)
	{
		printf("ERROR: License server address cannot be NULL.\n");
		return bReturn;
	}

	/* Display the fulfillment count summary of server trusted storage. */
	sDumpServerFRCntSummary(handle);

	/* Create a license specification. */
	bReturn = flxActCommonLicSpcCreate(handle, &licSpc);

	if (!bReturn)
	{
		flxActCommonHandleGetError(handle, &error);
		DEBUG("ERROR: flxActCommonLicSpcCreate (%d,%d,%d)\n", error.majorErrorNo,
															  error.minorErrorNo,
															  error.sysErrorNo);
	}

	if(g_bValidOnly)
	{
		/* Loads trusted fulfillment records from server trusted storage into the license specification. */
        bReturn = flxActCommonLicSpcPopulateFromServerTS(licSpc, g_pszEntitlementID, g_pszProductID, g_pszCommServer);
	}
	else
	{
		/* Loads all (trusted, untrusted, expired, etc.) fulfillment records from server trusted storage into the license specification. */
        bReturn = flxActCommonLicSpcPopulateAllFromServerTS(licSpc, g_pszEntitlementID, g_pszProductID, g_pszCommServer);
	}

	if(bReturn)
	{
		uint32_t nFRCount = 0, nIndex = 0;

		/* Obtain the number of fulfillment records in the license specification. */
		nFRCount = flxActCommonLicSpcGetNumberProducts(licSpc);

		if (0 == nFRCount)
		{
			printf("No fulfillment records in server trusted storage. License Server = %s\n", g_pszCommServer);
		}
		else
		{
			for (nIndex = 0; nIndex < nFRCount; nIndex++)
			{
				/* Get the fulfillment record at the specified index from the license specification. */
				prodSpec = flxActCommonLicSpcGet(licSpc, nIndex);

				if(prodSpec)
				{
					/* Retrieves the fulfillment record from server trusted storage. The fulfillmentID
					   from the supplied product license specification would be used in determining the 
					   fulfillment record that should get retrieved. */
					bReturn = flxActCommonProdLicSpcPopulateFRFromServerTS(prodSpec, g_pszCommServer);

					if (bReturn)
					{
						/* Display the fulfillment record. */
						sOutputProdLic(prodSpec);
					}
					else
					{
						flxActCommonHandleGetError(handle, &error);
						DEBUG("ERROR: flxActCommonProdLicSpcPopulateFRFromServerTS (%d,%d,%d)\n", error.majorErrorNo,
																									error.minorErrorNo,
																									error.sysErrorNo);
					}
				}
			}
		}
	}
	else
	{
		flxActCommonHandleGetError(handle, &error);
		DEBUG("ERROR: %s (%d,%d,%d)\n", (g_bValidOnly)? "flxActCommonLicSpcPopulateFromServerTS" 
													  : "flxActCommonLicSpcPopulateAllFromServerTS",
										error.majorErrorNo,
										error.minorErrorNo,
										error.sysErrorNo);
	}

	/* Delete the specified license specification. */
	flxActCommonLicSpcDelete(licSpc);

	return bReturn;
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
	FlxActHandle			handle = 0;
	FlxActMode				mode = FLX_ACT_APP;
	FlxActError				error;
	int						initErr;
	OPERATION				operation = OP_TS_VIEW;
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
	 *	Figure out what operation we're performing and initialize necessary resources
	 */
	g_Operation = operation = sWhichOperation(argc, argv);

	/*
	 *	Initialize
	 */

	if((initErr=flxActCommonLibraryInit(NULL)) != flxInitSuccess)
	{
		DEBUG("ERROR: Activation library initialization failed: status %d\n", initErr);
		return -1;
	}
	else
	{
#ifdef PC
		pszUTF8AppactUtilArg = sConvertWCToUTF8(argv[2]);
#endif		
		/*
		 *	Create handle and process.
		 */
		if(flxActCommonHandleOpen(&handle, mode, &error))
		{
			switch(operation)
			{
				case OP_TS_LOCALACTIVATION_CHECK:
					if (argc < 3)
					{
						sUsage();
						exit(1);
					}
					sCheckLocalActivation(handle, argument2);
					break;

				case OP_TS_LOCALACTIVATION:
					if (argc < 3)
					{
						sUsage();
						exit(1);
					}
					sHandleLocalActivation(handle, argument2);
					break;

				case OP_TS_TRIAL_RESET:
  					if (argc < 3)
  					{
  						sUsage();
  						exit(1);
  					}
  					sHandleTrialReset(handle, argument2);
  					break;

				case OP_TS_SERVEDACTIVATION:
					sHandleActivation(handle);
					break;

				case OP_TS_RETURN:
					if(argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleReturn(handle, g_pszFRId);
					break;

				case OP_TS_REPAIR:
					if(argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleRepair(handle, g_pszFRId);
					break;

				case OP_TS_DELETE:
					if(argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleDelete(handle, argument2);
					break;

				case OP_TS_DELETE_PRODUCT:
					if(argc < 3)
					{
						sUsage();
						exit(1);
					}
					(void)sHandleDeleteProductFromTS(handle, argument2);
					break;

				case OP_TS_LOCAL_REPAIR:
					(void)sHandleLocalRepair(handle);
					break;

				case OP_TS_PROCESS:
					(void)sHandleManualProcessing(handle);
					break;

				case OP_TS_SHORT_CODE_ACTIVATION:
				case OP_TS_SHORT_CODE_REPAIR:
				case OP_TS_SHORT_CODE_RETURN:
					(void)sHandleShortCode(handle, operation);
					break;

				case OP_TS_UNIQUE_MACHINE_NUMBER:
					(void)sUniqueMachineNumber(handle);
					break;

				case OP_TS_BORROW:
					sDoBorrow(handle);
					break;

				case OP_TS_RETURN_BORROW:
					sReturnBorrow(handle);
					break;

				case OP_TS_VIRT:
					sVirtualizationExample(handle);
					break;

				case OP_TS_SERVER_VIEW:
					sDumpServerTS(handle);
					break;

				case OP_TS_VIEW:
				case OP_TS_UNKNOWN:
				default:
					sDumpTS(handle);
					break;
			}

			/*
			 *	Cleanup
			 */
			flxActCommonHandleClose(handle);
		}
		else
		{
			DEBUG("ERROR: flxActCommonHandleOpen - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		}
		flxActCommonLibraryCleanup();
	}
#ifdef PC
	if(pszUTF8AppactUtilArg)
		free(pszUTF8AppactUtilArg);
#endif	
	return 0;
}

