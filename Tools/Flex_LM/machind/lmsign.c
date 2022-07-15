/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *	Description:	Wrapper for lc_cryptstr to be used by ASRgen
 */

#include <string.h>
#include "lmprikey.h"
#include "lmclient.h"
#include "lm_code.h"
#include "lmseeds.h"

#define FUDGE_FACTOR	40	/* extra space for newline and spaces on SERVER and VENDOR line */

LM_CODE(site_code, ENCRYPTION_SEED1, ENCRYPTION_SEED2, VENDOR_KEY1,
			VENDOR_KEY2, VENDOR_KEY3, VENDOR_KEY4, VENDOR_KEY5);

/****************************************************************************/
/**	@brief	Sign specified license using specified hostid.
 *
 *	@param	pszHostId	Hostid to sign feature/increment lines with
 *	@param	pszInput	FEATURE/INCREMENT line(s) to sign
 *	@param	pszOutput	Pointer to buffer that will receive signed output
 *	@param	pOutSize	Size of output buffer, if 0 will return size needed
 *						in bytes (including null) to hold output.
 *	@param	pszError	Pointer to buffer that will receive error message if an
 *						an error occurs.
 *	@param	pErrorSize	Size of error buffer, if 0 and an error occurs will
 *						return size needed in bytes (including null) to hold
 *						error message.
 *
 *	@return	LM_NOERROR on success else a FLEX error code, same as lc_cryptstr()
 *	@note	If size of output buffer is smaller than generated output, data
 *			will be truncated (and null terminated).
 ****************************************************************************/
int
l_flexSignLicense(
	char *			pszHostId,
	char *			pszInput,
	char *			pszOutput,
	size_t *		pOutSize,
	char *			pszError,
	size_t *		pErrorSize)
{
	FLEX_ERROR		err = LM_NOERROR;
	LM_HANDLE *		pJob = NULL;
	char *			buffer = NULL;
	char *			pszOut = NULL;
	char *			pszBegin = NULL;
	char			szSearchStr[10] = {'\0'};
	char *			pszSignError = NULL;
	size_t			bufferSize = 0;
	LM_CODE_GEN_INIT(&site_code);

	if(pszHostId && pszInput)
	{
		if(0 == lc_init((LM_HANDLE *)0, VENDOR_NAME, &site_code, &pJob))
		{
			/*
			 *	Generate input for crypt routine
			 */
			bufferSize = strlen(pszHostId) + strlen(pszInput) +
				strlen("VENDOR") + strlen(VENDOR_NAME) + FUDGE_FACTOR;
			buffer = malloc(sizeof(char) * bufferSize);
			if(buffer)
			{
				sprintf(buffer, "SERVER this_host %s\nVENDOR %s\n%s",
					pszHostId, VENDOR_NAME, pszInput);
				err = lc_cryptstr(pJob, buffer, &pszOut, &site_code,
					LM_CRYPT_FORCE, NULL, &pszSignError);
				if(LM_NOERROR == err)
				{
					/*
					 *	Move to begining of real output
					 */
					memcpy(szSearchStr, pszInput,
						(strlen(pszInput) > (sizeof(szSearchStr) - 1) ? (sizeof(szSearchStr) - 1) : strlen(pszInput)));
					pszBegin = strstr(pszOut, szSearchStr);
					if(NULL == pszBegin)
					{
						pszBegin = pszOut;
					}
					if(0 == (*pOutSize))
					{
						/*
						 *	Just calculate the size of buffer
						 *	needed plus 1 for NULL and return
						 */
						*pOutSize = strlen(pszBegin) + 1;
					}
					else
					{
						/*
						 *	They want the data
						 */
						if(NULL == pszOutput)
						{
							err = LM_BADPARAM;
						}
						else
						{
							if(strlen(pszBegin) >= (*pOutSize))
							{
								/*
								 *	Truncate
								 */
								memcpy(pszOutput, pszBegin, *pOutSize);
								/*
								 *	NULL terminate
								 */
								(pszOutput)[*pOutSize - 1] = '\0';
							}
							else
							{
								memcpy(pszOutput, pszBegin, strlen(pszBegin));
								(pszOutput)[strlen(pszBegin)] = '\0';

							}
						}
					}
				}
				else
				{
					/*
					 *	lc_cryptstr error
					 */
					if(pszSignError && pErrorSize)
					{
						if(0 == (*pErrorSize))
						{
							/*
							 *	Just calculate the size of buffer to hold error message
							 *	plus 1 for NULL and return
							 */
							*pErrorSize = strlen(pszSignError) + 1;
						}
						else
						{
							if(pszError)
							{
								if(strlen(pszSignError) >= (*pErrorSize))
								{
									/*
									 *	Truncate
									 */
									memcpy(pszError, pszSignError, *pErrorSize);
									/*
									 *	NULL terminate
									 */
									(pszError)[*pErrorSize - 1] = '\0';
								}
								else
								{
									memcpy(pszError, pszSignError, strlen(pszSignError));
									(pszError)[strlen(pszSignError)] = '\0';
								}
							}
						}
					}
					if(pszSignError)
					{
						/* Free up memory allocated by lc_cryptstr */
						lc_free_mem(pJob, pszSignError);
						pszSignError = NULL;
					}
				}
			}
			else
			{
				err = LM_CANTMALLOC;
			}
		}
	}
	else
	{
		err = LM_BADPARAM;
	}
	if(pszOut)
	{
		lc_free_mem(pJob, pszOut);
		pszOut = NULL;
	}
	if(buffer)
	{
		free(buffer);
		buffer = NULL;
	}
	if(pJob)
	{
		lc_free_job(pJob);
		pJob = NULL;
	}
	return err;
}
