/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *	Function: validdat date
 *
 *	Description: Verifies that the date specified is valid.
 *
 *	Parameters:	(char *) date - a trial date
 *
 *	Return:		(int) - 0 - OK, date if valid
 *				BADDATE - date format invalid
 *				DATE_TOOBIG - Year too large for binary format
 *				LONGGONE - Date has expired
 *				-1 - no date specified
 *
 *	M. Christiano
 *	2/18/88
 *
 *	Last changed:  2/10/96
 *
 */

#include "stdio.h"
#include "lmclient.h"

int l_validdate();
void exit();

int
main(int argc, char * argv[])
{
	LM_HANDLE job;

	memset (&job, 0, sizeof(LM_HANDLE));
	if (argc < 2)
	{
		(void) printf("Usage: %s trial-date\n", argv[0]);
		exit(-1);
	}
	return(l_validdate(&job, argv[1]));
}
