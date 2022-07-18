/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This file is part of an FNP sample utility that demonstrates Composite Transactions.

	It defines helper functions and macros that abstract memory allocation, string handling etc.
*/
#include <stdlib.h>
#include <stdio.h>
#ifdef PC
#include <malloc.h>
#endif
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "lm_redir_std.h"
#include "compTranUtilHelper.h"


static FlxActBool sbIsVerbose = FLX_ACT_FALSE;
static FlxActBool sbIsBrief   = FLX_ACT_FALSE;
static FlxActBool sbIsTimePrint    = FLX_ACT_FALSE;

/*
	A free that accepts a const pointer.
*/
void  flxCtuFree(const void *pMemory)
{
	if (pMemory != NULL)
		free((void*)pMemory);
}

/*
	A malloc that exits if memory allocation fails, to avoid cluttering the sample with checks. 
*/
void* flxCtuMalloc(size_t len)
{
	void* pMemory = malloc(len);
	if (pMemory == NULL)
	{
		puts("No memory\n");
		flxCtuOnNoMemory();
	}
	return pMemory;
}

/*
	A malloc that sets the memory to zero.
*/
void* flxCtuMallocAndZeroise(size_t len)
{
	void* pMemory = flxCtuMalloc(len);
	memset(pMemory, 0, len);
	return pMemory;
}

/*
	Print formatted message to stream.
*/
void flxCtuPrint(FILE* fh, const char *pszFormatStr, va_list argList)
{
	int charCount = vfprintf(fh, pszFormatStr, argList);
	if (charCount <= 0)
		FLX_CTU_ASSERT(1);
}

/*
	Print "ERROR: " message.
*/
void flxCtuPrintError(const char* pszFormatStr, ...)
{
	va_list argList;
	va_start(argList, pszFormatStr);
	flxCtuPrintErrorV(pszFormatStr, argList);
	va_end(argList);
}

/*
	Print "ERROR: " message (variable arguments).
*/
void flxCtuPrintErrorV(const char* pszFormatStr, va_list argList)
{
	fputs("ERROR: ", lm_flex_stderr());
	flxCtuPrint(lm_flex_stderr(), pszFormatStr, argList);
}

/*
	Print a message to stdout.
*/
void flxCtuPrintMessage(const char* pszFormatStr, ...)
{
	if ( !sbIsBrief)
	{
		va_list argList;
		va_start(argList, pszFormatStr);
		flxCtuPrintMessageV(pszFormatStr, argList);
		va_end(argList);
	}
}

/*
	Print a message to stdout.
	Variable arguments.
*/
void flxCtuPrintMessageV(const char *pszFormatStr, va_list argList)
{
	if ( !sbIsBrief)
		flxCtuPrint(lm_flex_stdout(), pszFormatStr, argList);
}

/*
	Print to stdout.
*/
void flxCtuPrintOutput(const char* pszFormatStr, ...)
{
	va_list argList;
	va_start(argList, pszFormatStr);
	flxCtuPrintOutputV(pszFormatStr, argList);
	va_end(argList);
}

/*
	Print to stdout (variable arguments).
*/
void flxCtuPrintOutputV(const char* pszFormatStr, va_list argList)
{
	flxCtuPrint(lm_flex_stdout(), pszFormatStr, argList);
}

/*
	Print verbose message.
*/
void flxCtuPrintVerbose(const char* pszFormatStr, ...)
{
	if (sbIsVerbose)
	{
		va_list argList;
		va_start(argList, pszFormatStr);
		flxCtuPrintVerboseV(pszFormatStr, argList);
		va_end(argList);
	}
}

/*
	Print verbose message (variable arguments).
*/
void flxCtuPrintVerboseV(const char* pszFormatStr, va_list argList)
{
	if (sbIsVerbose)
	{
		flxCtuPrintMessage("VERBOSE: ");
		flxCtuPrintMessageV(pszFormatStr, argList);
	}
}

/*
	A realloc that exits if memory allocation fails.
*/
void* flxCtuRealloc(const void *pMemory, size_t len)
{
	void* pNewMemory;
	
	if (pMemory == NULL)
		pNewMemory = malloc(len);
	else
		pNewMemory = realloc((void*)pMemory, len);

	if (pNewMemory == NULL)
	{
		puts("No memory\n");
		flxCtuOnNoMemory();
	}
	return pNewMemory;
}

/*
	Set brief (output only, suppress messages).
*/
void flxCtuSetBrief(FlxActBool bIsBrief)
{
	sbIsBrief = bIsBrief;
}

/*
	Set verbose.
*/
void flxCtuSetVerbose(FlxActBool bIsVerbose)
{
	sbIsVerbose = bIsVerbose;
}

/*
	String validation.
*/
FlxActBool flxCtuStrIsNumeric(const char* pSource)
{
	if (strspn(pSource, "0123456789") < strlen(pSource))
		return FLX_ACT_FALSE;
	else
		return FLX_ACT_TRUE;
}

/*
	String compare ignoring case.
	Lower case duplicates of the strings are compared with strcmp.
*/
int flxCtuStrCmpLwr(const char* pszString1, const char* pszString2)
{
	const char* pszString1Lwr = flxCtuStrDupLwr(pszString1);
	const char* pszString2Lwr = flxCtuStrDupLwr(pszString2);
	int result				  = strcmp(pszString1Lwr, pszString2Lwr);

	flxCtuFree(pszString1Lwr);
	flxCtuFree(pszString2Lwr);
	return result;
}

/*
	String erase characters belonging to set supplied.
*/
char* flxCtuStrEraseChars(char* pszString, const char* pszEraseSet)
{
	size_t get = 0;
	size_t put = 0;
	
	while ( pszString[get] != '\0') 
	{
		/* If the source character is not in the erase set... */
		if (NULL == strchr(pszEraseSet, pszString[get]))
			pszString[put++] = pszString[get]; /* ... copy it */
		get++;
	} 
	pszString[put] = '\0';
	return pszString;	/* As a convenience */
}


/*
	String duplication.
*/
char* flxCtuStrDup(const char* pszString)
{
	return flxCtuStrNDup(pszString, strlen(pszString));
}

char* flxCtuStrDupDelim(const char** ppszString, char delimiter)
{
	/* Skip leading whitespace. */
	const char* pSource  = *ppszString + strspn(*ppszString, " \t");
	/* Get a pointer to the first delimiter character. */
	const char* pDelimiter = strchr(pSource, delimiter);

	if (pDelimiter != NULL)
	{	/* Delimiter found, move ppszString past it. */
		*ppszString = pDelimiter + 1;
	}
	else
	{	/* Delimiter not found, use end of string. */
		pDelimiter  = pSource + strlen(pSource);
		*ppszString = pDelimiter;
	}
	
	/* Duplicate and return the string. */
	return flxCtuStrNDup(pSource, (pDelimiter - pSource));
}

/*
	String duplication with conversion to lower case.
*/
char* flxCtuStrDupLwr(const char* pString)
{
	return flxCtuStrNDupLwr(pString, strlen(pString) + 1);
}

/*
	Extract and return a duplicate of the filename from a path.
*/
char* flxCtuStrDupNameFromPath(const char* pszPath)	
{
	const char* pszExt;
	/*
		Point pszName to the character following the last path separator.
	*/
	const char* pszName = pszPath;
	size_t		len		= 0;
	do 
	{
		pszName += len;
		len = strcspn(pszName, "\\/");
	} while (pszName[len++] != '\0');

	/*
		Calculate the length excluding any extension.
	*/
	len    = strlen(pszName);
	pszExt = strrchr(pszName, '.');
	if ( pszExt != NULL)
		len -= strlen(pszExt); 
	/*
		Duplicate and return the name.
	*/
	return flxCtuStrNDup(pszName, len);
}

/*
	Duplicate and return the sub-strings before and after the separator.
	If separator missing, before string will be a copy, after string will be empty.
*/
void flxCtuStrDupSeparate(const char*	pszString, 
						  char			separator, 
						  const char**	ppDupBefore,
						  const char**	ppDupAfter)
{
	const char* pSeparator = strchr(pszString, separator);
	if (pSeparator == NULL)
		pSeparator = pszString + strlen(pszString);

	*ppDupBefore = flxCtuStrNDup(pszString, (pSeparator - pszString));
	*ppDupAfter  = flxCtuStrNDup(pSeparator + 1, strlen(pSeparator));
}

char* flxCtuStrLwr(char* pszString)
{
	return flxCtuStrNLwr(pszString, strlen(pszString) + 1);
}

char* flxCtuStrNDup(const char* pString, size_t len)
{
	char* pDuplicate = (char*)flxCtuMalloc(len + 1);
	memcpy(pDuplicate, pString, len);
	pDuplicate[len] = '\0';
	return pDuplicate;
}

/*
	String replace - destination string replaced with duplicate of source string;
	NUL terminator appended.
*/
void flxCtuStrNDupReplace(const char** ppDestination, const char* pSource, size_t len)
{
	char* pReplacement = (char*)flxCtuRealloc((void *)*ppDestination, len + 1);
	memcpy(pReplacement, pSource, len);
	pReplacement[len] = '\0';
	*ppDestination = pReplacement;
}

char* flxCtuStrNDupLwr(const char* pString, size_t len)
{
	char* pDuplicate = flxCtuStrNDup(pString, len);
	return flxCtuStrNLwr(pDuplicate, len);
}
/*
	String convert to lower case.
*/
char* flxCtuStrNLwr(char* pString, size_t len)
{
	size_t index;

	for (index = 0; index < len; index++)
		pString[index] = tolower(pString[index]);

	return pString;
}

/*
	Timing - total elapsed and interval times in decimal seconds; enabled by "-time" command.
	Resolution is platform dependent.
*/
static double sTimeStart = 0;
static double sTimeIntervalStart = 0;
#if defined OS_WINDOWS || defined PC
#include <windows.h>
static double sGetSecondsCounter()
{
	return (double)GetTickCount() / 1e3f;
}
#elif defined OS_MACOSX || defined MAC10
#include <sys/time.h>
static double sGetSecondsCounter()
{
	struct timeval tv;
	double res = 0e0f;

	if( gettimeofday(&tv, NULL) == 0 )
	{
		res = (double)tv.tv_sec + ((double)tv.tv_usec/1e6f);
	}

	return res;
}
#else
#include <time.h>
static double sGetSecondsCounter()
{ 
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME,&ts);

    return (double)ts.tv_sec + ((double)ts.tv_nsec / 1e9f);
}
#endif

void	flxCtuTimeStart(FlxActBool bEnableTimePrint)
{
	sbIsTimePrint		= bEnableTimePrint;
	sTimeStart			= sGetSecondsCounter();
	sTimeIntervalStart  = sTimeStart;
}

void flxCtuTimeResetInterval()
{
	(void)flxCtuTimeGetInterval();
}

float	flxCtuTimeGetInterval()		/* Secs since flxCtuTimeStart() or flxCtuTimeGetInterval()	*/
{
	double now			= sGetSecondsCounter();
	double interval		= now - sTimeIntervalStart;
	sTimeIntervalStart	= now;
	return (float)interval;
}

float	flxCtuTimeGetElapsed()	/* Secs since flxCtuTimeStart()							*/
{
	return (float)(sGetSecondsCounter() - sTimeStart);
}

void	flxCtuTimePrintInterval(const char* pszMessage)
{
	if (sbIsTimePrint)
		flxCtuPrintMessage("Time interval %6.3f secs - %s\n", flxCtuTimeGetInterval(), pszMessage);
}

void	flxCtuTimePrintElapsed(const char* pszMessage)
{
	if (sbIsTimePrint)
		flxCtuPrintMessage("Time elapsed %7.3f secs - %s\n", flxCtuTimeGetElapsed(), pszMessage);
}

