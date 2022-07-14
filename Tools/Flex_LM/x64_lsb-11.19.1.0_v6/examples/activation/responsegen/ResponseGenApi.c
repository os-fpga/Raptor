/**************************************************************************************************
* Copyright (c) 2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This sample utility demonstrates the use of the response generation API for XML based
	transactions (but not shortcode transactions).

	It is not intended as a response generation tool - for this use the kit utilities
	responsegen or responsegenUI.

	API
	===
	See header "responsegen.h" in kit folder machind/activation/include/responsegen

	Build and run
	=============

		For windows:
			Define:			OS_WINDOWS
			Include path:	<kit>\machind\activation\include\responsegen
							<kit>\machind\activation\include\windows
			Import library:	<kit>\<architecture-eg-i86_n3>\activation\lib\responsegen\responsegen.lib

			e.g. 
			set KIT=<path to kit>
			cl ResponseGenApi.c /D OS_WINDOWS 
				/I %KIT%\machind\activation\include\responsegen
				/I %KIT%\machind\activation\include\windows	
				/link %KIT%\i86_n3\activation\lib\responsegen\responsegen.lib					
			
			Ensure the shared object responsegen.dll (kit "publisher" folder) is in the 
			same folder as ResponseGenApi.exe or on the PATH.
		
		For Linux:
			Include path:	<kit>/machind/activation/include/responsegen
			Link with shared object

			e.g.
			export KIT=<path to kit>
			gcc -I$(KIT)/machind/activation/include/responsegen -o responsegenapi ResponseGenApi.c \
                -L$(KIT)/x64_lsb/publisher -lresponsegen  -Wl,-rpath,$(KIT)/x64_lsb/publisher


	Usage
	=====
	See sShowUsage() below.
*/

#include "responsegen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#if OS_WINDOWS
/* Disable deprecated function warnings */
#pragma warning( disable:4996 )
#endif

void sShowUsage()
{
	printf(	"\n"
			"This sample shows how to use the response generation API.\n"
			"It is not intended as a response generation tool - for this use\n"
			"responsegen or responsegenUI\n"
			"\n"
			"Usage: responsegenapi <requestXmlFile> [<responseParameterFile> <keyFile>...]\n"
			"\n"
			"  The request is verified.\n"
			"\n"
			"  If a <responseParameterFile> is specified, a response is generated and printed.\n"
			"    For composite requests the parameters must conform to the W7 XML schema.\n"
			"    For single action requests the parameters must conform to W1 (activation)\n"
			"    W2 (return) W3 (repair) W4S/C (config server/non-server) W6 (failure)\n"
			"    (see kit publisher/XML/Schemas folders)\n"
			"    For the out-of-the-box kit the keyFile is publisher/xml/DemoKeyout.xml\n");
}

void sPrintApiError(HMARGE_CONTEXTC ctx)
{
	enum fnpMargeErrorCode lastError	= fnpMargeGetLastErrorC(ctx);
	const char* errorText				= fnpMargeGetErrorDescriptionC(lastError);
	const char* errorDetails			= fnpMargeGetLastErrorDetailC(ctx);

	printf("ERROR: API error %d: %s\n", (int)lastError, errorText);
	printf("Details: %s\n", errorDetails);
}

/****************************************************************************/
/* Read the contents of a file into allocated memory.  Caller to free.
 *
 * return	1 on success, else 0
 ****************************************************************************/
static int /* bool */ sbAllocReadFile(const char* pszFilePath, char** ppFileContent)
{
	int bRV = 0;			/* Default to failure */
	struct stat	statBuf;

	if (pszFilePath == NULL || ppFileContent == NULL)
		return 0;

	/* stat to check that it's a file and get its size */
	if (0 != stat(pszFilePath, &statBuf))
	{
		printf("ERROR: %s does not exist or is not a file\n", pszFilePath);
	}
	else if (0 != (statBuf.st_mode & S_IFDIR))
	{
		printf("ERROR: %s is a directory; must specify a file\n", pszFilePath);
	}
	else
	{
		/* Open the file */
		FILE* fp = fopen(pszFilePath, "r");
		if(NULL == fp)
		{
			printf("ERROR: Can\'t open %s (errno %d)\n", pszFilePath, errno);
		}
		else
		{
			/* Allocate a buffer for the content */
			*ppFileContent = calloc(sizeof(char) * (statBuf.st_size + 1), 1);
			if(NULL == ppFileContent)
			{
				printf("ERROR: No memory\n");
			}
			else
			{
				/* Read in the file content */
				(void)fread(*ppFileContent, sizeof(char), statBuf.st_size, fp); 
				if (ferror(fp))
				{
					printf("ERROR: Can't read %s (errno %d)\n", pszFilePath, errno);
					free(*ppFileContent);
					*ppFileContent = NULL;
				}
				else
					bRV = 1;	/* Success */
			}
			fclose(fp);
		}
	}
	return bRV;
}

/****************************************************************************/
/* Read the contents of the key file into allocated memory.  Caller to free.
 *
 * This sample currently only supports single keys, that is it excludes
 * composite responses that have actions for more than one trusted section.
 *
 * You can add this by combining the multiple single key ("P2" schema) files
 * into a single XML document ("P3" schema) either here or manually externally
 * (passing a "P3" file as argv[3]).
 *
 * Returns 1 on success, else 0
 ****************************************************************************/
static int /* bool */ sbAllocReadKeyFiles(int argc, char* argv[], char** ppszKeyData)
{
	const char*		pszKeyFile = argv[3];
	char*			pszKeyData;
	int /* bool */	bKeyValid = 0;

	if (argc > 4)
	{
		printf("\nERROR: Multiple key files not yet supported.\n");
		sShowUsage();
		return 0;
	}

	/* Read in the file */
	if (!sbAllocReadFile(pszKeyFile, &pszKeyData))
		return 0;

	/* If it's "P2" format then reduce it to just the "Long_1"  key data */
	if (strstr(pszKeyData, "<SecureKeyInformation") != NULL)
	{
		/* Note we're assuming the XML is valid here, checks are just to prevent exceptions */
		const char* pszTag	= "<Type>Long_1</Type>";
		const char* pszStart	= strstr(pszKeyData, pszTag);
		if (pszStart != NULL)
		{
			pszTag	 = "<KeyData>";
			pszStart = strstr(pszStart, pszTag);
			if (pszStart != NULL)
			{
				const char* pszEnd;
				pszStart += strlen(pszTag);
				pszEnd = strstr(pszStart, "</KeyData>");
				if (pszEnd != NULL)
				{
					size_t keySize = pszEnd - pszStart;
					if ((size_t)(pszStart - pszKeyData) > keySize)
					{
						strncpy(pszKeyData, pszStart, keySize);
						pszKeyData[keySize] = '\0';
						bKeyValid = 1;
					}
				}
			}
		}
	}
	else if (strstr(pszKeyData, "<MultipleKeyData") != NULL)
	{
		/* "P3" format - use as-is */
		bKeyValid = 1;
	}

	if ( !bKeyValid)
	{
		printf("ERROR: Invalid key file %s\n", pszKeyFile);
		free(pszKeyData);
		return 0;
	}
	*ppszKeyData = pszKeyData;
	return 1;		
}

int main(int argc, char* argv[])
{
	int	returnCode = 1;		/* Default to failure				*/
	HMARGE_CONTEXTC ctx;	/* Context (handle) for API calls	*/
	char* pszRequestXml;
	char* pszResponseParameterXml;
	char* pszKeyData;

	if (argc != 2 && argc < 4)
	{
		sShowUsage();
		return 1;
	}

	/* Show the API version number */
	{
	    uint32_t apiMajor = 0, apiMinor = 0, apiMaint = 0, apiHotfix = 0;
		fnpMargeGetVersionC(&apiMajor, &apiMinor, &apiMaint, &apiHotfix);
		printf("Responsegen API version number: %u.%u.%u.%u\n", 
			apiMajor, apiMinor, apiMaint, apiHotfix);
	}
	/* Get a context for use in subsequent calls */ 
	ctx = fnpMargeOpenHandleC();
	if (ctx == 0)
	{
		printf("ERROR: fnpMargeOpenHandleC returned 0\n");
		sPrintApiError(ctx);
	}

	/* If got context, read in the request XML */
	else if (sbAllocReadFile(argv[1], &pszRequestXml))
	{
		/*	Validate the request XML.
			Validation consists of checking the root element name, that
			required sub-elements are present and that the hash verifies.
		*/
		if ( !fnpMargeValidateRequestC(ctx, pszRequestXml))
		{
			printf("ERROR: fnpMargeValidateRequestC failed\n");
			sPrintApiError(ctx);
		}
		else 
		{
			if (argc < 4)
			{
				printf("Request is valid.\n");
			}
			else
			{
				/* Read in the response parameter file */
				if (sbAllocReadFile(argv[2], &pszResponseParameterXml))
				{
					/* Read in the key file(s) */
					if (sbAllocReadKeyFiles(argc, argv, &pszKeyData))
					{
						/*	Generate the response.
							The content is constructed from the information in the request and response parameter XML
							and signed using the public key from the key file.
						*/
						const char* pszResponseXml;
						if (fnpMargeGenerateResponseC(ctx, pszRequestXml, pszResponseParameterXml, pszKeyData, &pszResponseXml))
						{
							printf("%s",pszResponseXml);
							returnCode = 0;
						}
						else
						{
							printf("ERROR: fnpMargeGenerateResponseC failed\n");
							sPrintApiError(ctx);
						}
						free(pszKeyData);
					}
					free(pszResponseParameterXml);
				}
			}
			free(pszRequestXml);
		}
	}
	fnpMargeCloseHandleC(ctx);
	
	return returnCode;
}

