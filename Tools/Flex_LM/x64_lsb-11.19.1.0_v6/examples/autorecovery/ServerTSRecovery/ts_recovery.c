/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Description: This is a sample application program, to illustrate
 *			     how to achieve automatic recovery of a broken Server TS using V2 transaction.
 *
 *  Points to note:
 *               - Automatic repair method will repair only the broken FRs those activated from FNO/ Back Office.
 *               - It will not repair other type of FRs (i,e. FRs activated from local ASR or from other enterprise server, etc. ) 
 *               - The repaired FRs will reflect in license server after next TS polling interval. 
 *				 - Back Office Address(FNO) need to be pass as second argument to child process. For details see ts_recovery()
*/
 
#include <stdio.h>
#ifdef PC
	// For Window platform
	#include <windows.h>
	#include <tchar.h>
	#include <conio.h>	
#else
	// For Non Windows platform
	#include <sys/types.h>
	#include <unistd.h>
	#include <sys/wait.h>	
#endif /* PC */	

#include "ts_recovery.h"

#define TRUE 1
#define FALSE 0

#ifdef PC
	PROCESS_INFORMATION wProcessInfo;
	STARTUPINFO wStartupInfo;
#else
	pid_t lProcessId;
#endif

typedef unsigned int M_BOOL;
M_BOOL repairState = FALSE;


void resetRepairState()
{	
#ifdef PC
	DWORD exit_code;
	
	/* Check if any repair activity is in progress */
	GetExitCodeProcess( wProcessInfo.hProcess, &exit_code );
	if (exit_code == STILL_ACTIVE)
	{	
		if ( WaitForSingleObject( wProcessInfo.hProcess, 0 ) == WAIT_TIMEOUT )
		{
			/* set current RepairState */
			repairState = TRUE;
		}
	}
	else
	{
		if ( wProcessInfo.hProcess && wProcessInfo.hThread )
		{
			CloseHandle( wProcessInfo.hProcess );
			CloseHandle( wProcessInfo.hThread );
		}
		
		/* Reset memory */
		ZeroMemory( &wStartupInfo, sizeof(wStartupInfo) );
		wStartupInfo.cb = sizeof(wStartupInfo);
		ZeroMemory( &wProcessInfo, sizeof(wProcessInfo) );
		
		/* set current RepairState */
		repairState = FALSE;
	}

#else
        /* waitpid() will return 0 if any repair activity is in progress */
        if( lProcessId && lProcessId != -1 && !waitpid(lProcessId, NULL, WNOHANG) )
            repairState = TRUE;
        else
            repairState = FALSE;		
#endif
}

/****************************************************************************/
/**	@brief	tsRecovery, will create a new process for TS Recovery 
 *	@param	void
 *	@return	void
 ****************************************************************************/
void tsRecovery(void)
{

#ifdef PC
	/*
	 * Command line arguments to child process as a string with space separated, 
	 * i,e: args="servercomptranutil.exe ServRecovery BackOfficeAddress";
     */
	char *args ="servercomptranutil.exe ServRecovery https://BackOfficeAddress.com/flexnet/services/ActivationService";
#else
	/*
	 * Command line arguments to child process as an arrary of strings, i,e: 
	 * argv[]={"./servercomptranutil", "autoServRec", "BackOfficeAddress",NULL};
     */
	char *argv[]={"./servercomptranutil", "ServRecovery", "https://BackOfficeAddress.com/flexnet/services/ActivationService", NULL};
#endif
	
	resetRepairState();
	
	if (repairState)
	{
		printf("\t One repair request is in progress..., ignore the new request! \n");
		return;
	}

#ifdef PC
    if( !CreateProcess( NULL,   // No module name (use command line)
        args,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &wStartupInfo,            // Pointer to STARTUPINFO structure
        &wProcessInfo )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "\t TSRecovery: CreateProcess failed (%d).\n", GetLastError() );
        return;
    }
	
	printf("\n\t TSRecovery (PID-%d): Child process started to repair untrusted licenses from Back Office! \n\n",wProcessInfo.dwProcessId);

#else
    lProcessId = fork();
	
    if (lProcessId == -1)
    {
        printf("\t Can't fork new process, error occured\n");
        return;
    }
    else if (lProcessId == 0)
    {
        printf("\t TSRecovery (PID-%d): Child process started to repair untrusted licenses from Back Office! \n\n",getpid());
        execv(argv[0], argv);
    }

#endif
    return;
}

