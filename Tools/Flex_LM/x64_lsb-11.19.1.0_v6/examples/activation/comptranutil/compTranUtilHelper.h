/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of an FNP sample utility that demonstrates Composite Transactions.

	It declares helper functions and macros for printing, memory allocation, string handling etc.
*/
#ifndef COMP_TRAN_UTIL_HELPER_H
#define COMP_TRAN_UTIL_HELPER_H

#include <stdlib.h>
#include <stdarg.h>

#include "FlxActTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
	Printing.
	Output, e.g. response XML, is written to stdout.
	Informative and error messages are written to stderr.
*/
void flxCtuPrintOutput(const char* pszFormatStr, ...);
void flxCtuPrintOutputV(const char* pszFormatStr, va_list argList);
void flxCtuSetBrief(FlxActBool bIsBrief); /* Brief = don't print messages. */
void flxCtuPrintMessage(const char* pszFormatStr, ...);
void flxCtuPrintMessageV(const char* pszFormatStr, va_list argList);
void flxCtuPrintError(const char* pszFormatStr, ...);
void flxCtuPrintErrorV(const char* pszFormatStr, va_list argList);

void flxCtuSetVerbose(FlxActBool bIsVerbose);
void flxCtuPrintVerbose(const char *pszFormatStr, ...);
void flxCtuPrintVerboseV(const char *pszFormatStr, va_list argList);

/* 
	Memory allocation - these functions exit on no memory rather than fail, to avoid cluttering 
	the sample with checks.  Realloc and free also do the right thing if passed NULL.
*/
void* flxCtuMalloc(size_t len);
void* flxCtuMallocAndZeroise(size_t len);
void* flxCtuRealloc(const void *pMemory, size_t len);
void  flxCtuFree(const void *pMemory);

/* 
	Function called if no memory is available - it must be implemented by users of this module.
*/
void flxCtuOnNoMemory();

/*
	String validation.
*/
FlxActBool flxCtuStrIsNumeric(const char* pSource);

/*
	String replace - destination string freed and replaced with duplicate of source string.
*/
void flxCtuStrNDupReplace(const char** ppszDestination, const char* pSource, size_t len);
void flxCtuStrDupReplace( const char** ppszDestination, const char* pszSource);

/*
	String duplicate.
*/
char* flxCtuStrNDup(const char* pString, size_t len);
char* flxCtuStrDup( const char* pszString);
char* flxCtuStrDupDelim(const char** ppszString, char delimiter);

/*
	String convert to lower case.
*/
char* flxCtuStrNLwr(char* pString, size_t len);
char* flxCtuStrLwr (char* pszString);

/*
	String duplicate and convert to lower case.
*/
char* flxCtuStrNDupLwr(const char* pString, size_t len);
char* flxCtuStrDupLwr (const char* pszString);

/*
	String compare ignoring case.
	Lower case duplicates of the strings are compared with strcmp.
*/
int flxCtuStrCmpLwr(const char* pszString1, const char* pszString2);

/*
	String erase characters belonging to set supplied.
*/
char* flxCtuStrEraseChars(char* pszString, const char* pszEraseSet);

/*
	Extract and return a duplicate of the filename from a path.
*/
char* flxCtuStrDupNameFromPath(const char* pszPath);

/*
	Duplicate and return the sub-strings before and after the separator.
	If separator missing, before string will be a copy, after string will be empty.
*/
void flxCtuStrDupSeparate(const char*	pszString, 
						  char			separator, 
						  const char**	ppDupBefore,
						  const char**	ppDupAfter);

/*
	Assert.
	The target function flxCtuOnAssert must be implemented by users of this module.
*/
void flxCtuOnAssert(int line, const char* pszFilename);
#define FLX_CTU_ASSERT(condition) if (!(condition)) flxCtuOnAssert(__LINE__, __FILE__);

/*
	Timing
*/
void	flxCtuTimeStart(FlxActBool bEnableTimePrint);
void	flxCtuTimeResetInterval();
float	flxCtuTimeGetInterval();	/* Secs since start, reset or get interval	*/
float	flxCtuTimeGetElapsed();		/* Secs since start							*/

void	flxCtuTimePrintInterval(const char* pszMessage);
void	flxCtuTimePrintElapsed(const char* pszMessage);

#ifdef __cplusplus 
}
#endif

#endif	/* Include guard */
