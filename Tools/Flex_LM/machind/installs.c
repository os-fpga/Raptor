/**************************************************************************************************
* Copyright (c) 1999-2022 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *      Module: Installs.c
 *
 *      Function:       main() - main routine for installing FLEXlm Service
 *

 *
 *      Description:    Example program for installing FLEXlm Service.
 *
 *      Parameters:     None
 *
 *      Return:         None.
 *
 *
 *
 *
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "lm_redir_std.h"

SC_HANDLE   schService;
SC_HANDLE   schSCManager;

#if defined(_WIN64)
#define CROSS_ACCESS KEY_WOW64_32KEY
#else
#define CROSS_ACCESS KEY_WOW64_64KEY
#endif

char service_name[256]; 
#define FLEX_MAX_CMD_LENGTH 33  
#define FLEX_MAX_LICENSE_FILES 30
char license_path[MAX_PATH*FLEX_MAX_LICENSE_FILES]; 
char log_file_path[MAX_PATH];
char lmgrd_path[MAX_PATH];
char CmdLine_Params[MAX_PATH];
char * token;
HKEY hcpl;
DWORD version_number;
int OsType;
#define NT_OS 1
#define FLEXLM_SERVICE_NAME     "FlexNet License Manager"
void CheckLmgrdSecurity (char * lpszFullName);
#if (_MSC_VER == 1400)

#define SERVICE_CONFIG_DELAYED_AUTO_START_INFO 3

	typedef struct _SERVICE_DELAYED_AUTO_START_INFO {
	BOOL fDelayedAutostart;
	}SERVICE_DELAYED_AUTO_START_INFO;
#endif

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Gets the TCP/IP Connect Retry Number Setting from Registry            //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
void
l_get_registry(int *value)
{
	HKEY hcpl;
	unsigned long length=4,type=REG_DWORD;
	char * RegistryAddress = NULL;
	char * FieldName = NULL;
	long error = 0;
	DWORD   version_number= GetVersion();

	if (version_number < 0x80000000)
	{
		RegistryAddress= "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters";
		FieldName="TcpMaxConnectRetransmissions";
	}
	else
	{
		RegistryAddress="System\\CurrentControlSet\\Services\\VxD\\MSTCP";
		FieldName="MaxConnectRetries";
	}

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				 RegistryAddress,
				 0,
				 KEY_READ,
				 &hcpl) == ERROR_SUCCESS)
	{
		// Set the value
		error = RegQueryValueEx(hcpl,
					FieldName,
					0,&type,
					(unsigned char *)value,
					&length);

		if (ERROR_SUCCESS != error)
			*value=-1;

		// Finished with keys
		RegCloseKey(hcpl);
		return ;
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Sets the TCP/IP Connect Retry Number Setting in  Registry             //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

void
l_set_registry(int value)
{
	HKEY hcpl;
	long length=4;
	char * RegistryAddress;
	char * FieldName;
	DWORD   version_number= GetVersion();

	if (version_number < 0x80000000)
	{
		RegistryAddress= "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters";
		FieldName="TcpMaxConnectRetransmissions";
	}
	else
	{
		RegistryAddress="System\\CurrentControlSet\\Services\\VxD\\MSTCP";
		FieldName="MaxConnectRetries";
	}

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					RegistryAddress,
					0,
					KEY_WRITE,
					&hcpl) == ERROR_SUCCESS)
	{


		RegSetValueEx(hcpl,
				FieldName,
				0,
				REG_DWORD,
				(unsigned char *) &value,
				length);


		// Finished with keys
		RegCloseKey(hcpl);
		return ;
	}
}

void
l_write_registry(
	LPCTSTR	Service_Name,
	LPCTSTR	License_Path,
	LPCTSTR	Log_File_Path,
	LPCTSTR	Lmgrd_Path,
	LPCTSTR CmdLine_Params,
	REGSAM rsSamViewFlag
	)
{
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, KEY_WRITE|rsSamViewFlag, &hcpl) == ERROR_SUCCESS)
	{
		HKEY happ, hdefaultapp;
		DWORD dwDisp;
		char new_name[120];
		sprintf(new_name,"FLEXlm License Manager\\%s", Service_Name);
		if (RegCreateKeyEx(hcpl, new_name, 0, "", REG_OPTION_NON_VOLATILE, KEY_WRITE|rsSamViewFlag, NULL, &happ, &dwDisp) == ERROR_SUCCESS)
		{
			RegSetValueEx(happ, "Lmgrd", 0, REG_SZ, Lmgrd_Path, (DWORD)strlen(Lmgrd_Path)+1);
			RegSetValueEx(happ, "LMGRD_LOG_FILE", 0, REG_SZ,Log_File_Path, (DWORD)strlen(Log_File_Path)+1);
			RegSetValueEx(happ, "License", 0, REG_SZ, License_Path, (DWORD)strlen(License_Path)+1);
			RegSetValueEx(happ, "cmdlineparams", 0, REG_SZ, CmdLine_Params, (DWORD)strlen(CmdLine_Params)+1);
			RegSetValueEx(happ, "Service", 0, REG_SZ, Service_Name, (DWORD)strlen(Service_Name)+1);

			/* update default service */
			sprintf(new_name,"FLEXlm License Manager");
			if (RegCreateKeyEx(hcpl, new_name, 0, "", REG_OPTION_NON_VOLATILE ,KEY_WRITE|rsSamViewFlag ,NULL, &hdefaultapp, &dwDisp) == ERROR_SUCCESS)
			{
				RegSetValueEx(hdefaultapp,"Service",0,REG_SZ,Service_Name,(DWORD)strlen(Service_Name)+1);
			}
			RegCloseKey(hdefaultapp);
			// Finished with keys
			RegCloseKey(happ);
		}
		RegCloseKey(hcpl);
	}
}


////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
// Installs Service on Nt platform                                        //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

int
InstallServiceNT(
	LPCTSTR	Service_Name,
	LPCTSTR	License_Path,
	LPCTSTR	Log_File_Path,
	LPCTSTR Lmgrd_Path)
{
	LPTSTR lpszQuotedPath = NULL;
	OSVERSIONINFO osvi;
	BOOL bOsVersionInfoEx;
	LPCTSTR dependentServices = "+NetworkProvider\0WinMgmt\0";
	
	memset(&osvi, 0, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);
	lpszQuotedPath = (LPTSTR) malloc(strlen(Lmgrd_Path) + 3);
	
	if (lpszQuotedPath)
	{
		memset(lpszQuotedPath, 0, strlen(Lmgrd_Path) + 3);
		sprintf(lpszQuotedPath, "\"%s\"", Lmgrd_Path);
		
		schService = CreateService(
					schSCManager,               // SCManager database
					Service_Name,               // name of service
					Service_Name,               // name to display
					SERVICE_ALL_ACCESS,         // desired access
					SERVICE_WIN32_OWN_PROCESS,  // service type
					SERVICE_AUTO_START,         // start type:
												//   SERVICE_DEMAND_START = Manual start of service
												//   SERVICE_AUTO_START   = Starts service automatically at start of OS
					SERVICE_ERROR_NORMAL,       // error control type
					lpszQuotedPath,             // service's binary
					NULL,                       // no load ordering group
					NULL,                       // no tag identifier
					"+NetworkProvider",         // dependent on Network Provider
					"NT AUTHORITY\\LocalService", // LocalService account
					NULL);                      // no password


		free(lpszQuotedPath);
	}

	if (schService == NULL)
	{
		printf("Failed to install FlexNet License Manager:\n");
		switch (GetLastError())
		{
			case ERROR_ACCESS_DENIED:
					printf( "Access to Windows NT Service Control "
							"Manager Database denied.\n" );
					break;

			case ERROR_DUP_NAME:
			case ERROR_SERVICE_EXISTS:
					printf("FlexNet License Manager is already installed\n");
					break;

			default:
					printf( "Error code: 0X%X\n", GetLastError());
					break;
		}
		return FALSE;
	}
	else
	{
		ChangeServiceConfig( 
						schService,        // handle of service 
				        SERVICE_NO_CHANGE, // service type: no change 
				        SERVICE_AUTO_START,  // service start type 
				        SERVICE_NO_CHANGE, // error control: no change 
				        NULL,              // binary path: no change 
				        NULL,              // load order group: no change 
				        NULL,              // tag ID: no change 
				        dependentServices,              // dependencies: no change 
				        NULL,              // account name: no change 
				        NULL,              // password: no change 
				        NULL);
		if  ((bOsVersionInfoEx) &&  (osvi.dwMajorVersion >=6) )
		{
			SERVICE_DELAYED_AUTO_START_INFO  srvc_delayed_start = { 1 } ;
			ChangeServiceConfig2(schService,SERVICE_CONFIG_DELAYED_AUTO_START_INFO,&srvc_delayed_start);
		}
		CloseServiceHandle(schService);
		return TRUE;
	}

}


////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Installs the service, figures out which os, writes Registry settings  //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


VOID
InstallService(
	LPCTSTR	Service_Name,
	LPCTSTR	License_Path,
	LPCTSTR	Log_File_Path,
	LPCTSTR	Lmgrd_Path,
	LPCTSTR CmdLine_Params)
{
	LPCTSTR lpszBinaryPathName = Lmgrd_Path;
	int RetOk = 0;
	if (NT_OS==OsType)
	{
		RetOk=InstallServiceNT(Service_Name,License_Path,Log_File_Path,Lmgrd_Path);
	}
	


	if (RetOk)
	{

		// next write registry entries
		// Update the registry
		// Try creating/opening the registry key

		l_write_registry(Service_Name,License_Path,Log_File_Path,Lmgrd_Path,CmdLine_Params,CROSS_ACCESS);
		l_write_registry(Service_Name,License_Path,Log_File_Path,Lmgrd_Path,CmdLine_Params,0);

		printf("\nFlexNet License Manager is successfully installed \n"
               "as one of your Windows Services.  Some handy tips: \n\n"
               "\n"
               "\t* The FlexNet License Manager will be automatically started\n"
               "\t  every time your system is booted.\n"
               "\n"
               "\t* The FlexNet service log file is lmgrd.log in your NT system\n"
               "\t  directory.\n"
               "\n"
               "\t* To remove FlexNet License Manager, type 'installs -r'\n");
	}
	else
		printf("The FlexNet License Manager was not successfully \n"
			"installed as a service on your system.\n");
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Removes service from NT                                               //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


RemoveServiceNT(LPCTSTR Service_Name)
{
	int ret;
	HKEY hcpl;
	char szServiceEntry[120];
	SERVICE_STATUS ssStatus;
	schService = OpenService(schSCManager, Service_Name, SERVICE_ALL_ACCESS);
	if (schService == NULL)
	{
		switch ( GetLastError() )
		{
		case ERROR_ACCESS_DENIED:
			printf( "Access to Windows NT Service Control "
                                        "Manager Database denied.\n" );
			break;
		case ERROR_INVALID_NAME:
		case ERROR_SERVICE_DOES_NOT_EXIST:
			printf( "%s service is already removed.\n",Service_Name );
			break;
		default:
			printf( "OpenService error(0x%02x)\n", GetLastError());
			break;
		}
		return FALSE;
	}
	QueryServiceStatus(schService, &ssStatus);
	if(ssStatus.dwCurrentState == SERVICE_RUNNING)
	{
		printf("%s service is still running. The service has to be stopped before it can be removed.\n", Service_Name);
		return FALSE;
	}
    ret = DeleteService(schService);

	if (ret)
	{
		printf("The %s service has been removed.\n",Service_Name);
					/* remove reg entries */
		wsprintf(szServiceEntry, "SOFTWARE\\FLEXlm License Manager");
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
									szServiceEntry,
									0,
									KEY_SET_VALUE|CROSS_ACCESS,
									&hcpl) == ERROR_SUCCESS)

			RegDeleteKeyEx(hcpl,Service_Name,CROSS_ACCESS,0);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
									szServiceEntry,
									0,
									KEY_SET_VALUE|0,
									&hcpl) == ERROR_SUCCESS)

			RegDeleteKeyEx(hcpl,Service_Name,0,0);
		RegCloseKey(hcpl);
		return TRUE;
	}
	else
	{
		switch ( GetLastError() )
		{
			case ERROR_ACCESS_DENIED:
			printf( "Access to Windows NT Service Control "
                                        "Manager Database denied.\n" );
			break;

			case ERROR_SERVICE_MARKED_FOR_DELETE:
			case ERROR_INVALID_NAME:
			case ERROR_SERVICE_DOES_NOT_EXIST:
				printf( "The FlexNet License Manager service is already removed.\n" );
				break;

			default:
				printf("failure: DeleteService (0x%02x)\n", GetLastError());
				break;
		}
		return FALSE;
	}
}


////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                                                        //
//  Removes service, figures out which os                                 //
//                                                                        //
//                                                                        //
//                                                                        //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

VOID
RemoveService(LPCTSTR Service_Name)
{
    Sleep(2000);
    switch (OsType)
    {
	case NT_OS:
        {
            RemoveServiceNT( Service_Name);
        }
	break;
        
    }

}
int
main(int argc, char *argv[])
{
BOOL remove=FALSE;
int slashPos = 0;
     
wchar_t dirname[MAX_PATH] = {0}, *tempdir = NULL, *pch = NULL, *ch = NULL, chr = 0;
ch = L"\\";
chr = '\\';

    if (argc == 1 || *argv[1] != '-') {
		printf("installs - Copyright (c) 1989-2022 Flexera.\nAll Rights Reserved.\n");
        printf("\nUsage: \n");
        printf("        To install the license manager as a service:\n");
        printf("          installs -c <license file path.	Windows Proposed path <systemdrive>\\ProgramFiles \\\n");
        printf("                   -e <path to lmgrd.exe.	Windows Proposed path <systemdrive>\\ProgramFiles \\\n");
        printf("                   -l <log file path.		Windows Proposed path Preferred Path <systemdrive>\\ProgramData> \\\n");
        printf("                   -n <service name> \\\n");
        printf("                   [-k <lmgrd parameters>] \n");
        printf("\n");
        printf("        To remove the license manager as a service:\n");
        printf("          installs -r -n <service name>\n");
        printf("\n");
        printf("If -n is not specified, \"FlexNet License Manager\" is used as the service name.\n");
        printf("\n");
		printf("The -k switch is optional and is used to pass one or more startup command-line\n");
		printf("parameters (-local, -x lmdown, and -x lmremove) to lmgrd.\n");
        printf("\n");
        printf("Refer to the FlexNet Licensing documentation for more details on the \"installs\" command.\n\n");
		printf("To continue the execution without prompting, redirect the output to a log file\n");
		printf("Press any key to continue...\n");
	
		/* To halt the Admin command prompt, so as to display the console log */
			getchar();
			
        exit(1);
    }

   strcpy(service_name,FLEXLM_SERVICE_NAME); //use the default name

   *license_path=0;
   *log_file_path=0;
   *lmgrd_path=0;
   *CmdLine_Params=0;

   while (argc > 1)
        {
                if (*argv[1] == '-')
                {
                  char *p = argv[1]+1;

                        switch(*p)
                        {


                          case 'n':
                          case 'N':
                                /*
                                 *
                                 *      specify the name that the service will use
                                 *
                                 * .
                                 */
                                strncpy(service_name,argv[2],sizeof(service_name)-1);
                                argc--; argv++;
                                break;

                          case 'c':
                          case 'C':
                             
                                if(strlen(argv[2])>=(MAX_PATH*FLEX_MAX_LICENSE_FILES))
                                {

                                  
                                   printf("\n\n************************************************************\n"
								   	       "The number of license files entered exceeds the maximum limit\n"
								   	       "***********************************************************\n");
								  
                                    return 0; 
								}
								
							    else
                                strncpy(license_path,argv[2],(sizeof(license_path) - 1));
                                argc--; argv++;
                                break;


                          case 'e':
                          case 'E':
                                strncpy(lmgrd_path,argv[2],(sizeof(lmgrd_path) - 1));
                                argc--; argv++;
                                break;


                          case 'l':
                          case 'L':
                              {
                                int ret = 0;
                                wchar_t* buffer = 0;

                                MultiByteToWideChar(CP_UTF8, 0, argv[2], -1, dirname, sizeof(dirname)/sizeof(wchar_t));
                                 tempdir = (wchar_t *) malloc (MAX_PATH * sizeof(wchar_t));
                                 
                                 if (tempdir)
                                 {
                                    slashPos = (int)(wcsrchr(dirname, chr) - dirname);
                                    if (slashPos > 0)
                                    {
                                       dirname[slashPos] = L'\0';

                                       if (_waccess(dirname, 0) == -1)
                                       {
#if (defined(_MSC_VER) && _MSC_VER < 1900)
                                    	   pch = wcstok(dirname, ch);
#else
                                    	   pch = wcstok(dirname, ch,&buffer);
#endif
                                          if (pch)
                                          {
                                             /* If user enters the path from root directory, after wcstok / will be removed, so prefixing the path with '/' */
                                             if (*(*(argv + 2)) == chr)
                                             {
                                                wcsncpy(tempdir, ch, wcslen(ch) + 1);
                                                wcscat(tempdir, pch);
                                             }
                                             else
                                                wcsncpy(tempdir, pch, wcslen(pch) + 1);
                                                      
                                             while (pch != NULL) 
                                             {
                                                ret = _wmkdir(tempdir);
                                                if (ret == -1)
                                                {
                                                    free (tempdir);
                                                    tempdir = NULL;
                                                    fprintf(lm_flex_stdout(), "%s\n", "Unable to create Debug Log File");
                                                    break;
                                                }
#if (defined(_MSC_VER) && _MSC_VER < 1900)
                                    	   pch = wcstok(dirname, ch);
#else
                                    	   pch = wcstok(dirname, ch,&buffer);
#endif
                                                if (pch)
                                                {
                                                   wcscat(tempdir, ch);
                                                   wcscat(tempdir, pch);
                                                }
                                             }
                                          }
                                       }
                                    }
                                    free (tempdir);
                                    tempdir = NULL;
                                 } 

                                strncpy(log_file_path,argv[2],(sizeof(log_file_path) - 1));
                                argc--; argv++;
                                break;

                              }
                          case 'r':
                          case 'R':
                                remove=TRUE;
                                break;

                          case 'k':
                          case 'K':
                                strncpy(CmdLine_Params, argv[2],(sizeof(CmdLine_Params) - 1));
                                argc--; argv++;
                                break;

                          default:
                                printf("Unrecognized command-line switch ");
                                return 0;
                        }
                }
                argc--; argv++;
        }

        version_number= GetVersion();

        if (  version_number < 0x80000000 )
                  OsType=NT_OS; 

        if (NT_OS == OsType )
                schSCManager = OpenSCManager(
                        NULL,                   // machine (NULL == local)
                        NULL,                   // database (NULL == default)
                        SC_MANAGER_ALL_ACCESS   // access required
                        );

        if (remove)
		{
                RemoveService(service_name);
		}
        else
        {

                if (lmgrd_path[0]=='\0')
                {
                        printf (" Need to specify path to LMGRD.EXE, Service not installed \n");
                        return 0;
                }
                InstallService(service_name, license_path, log_file_path,lmgrd_path, CmdLine_Params);
		if (NT_OS == OsType )	CheckLmgrdSecurity( lmgrd_path );
        }
        if (NT_OS == OsType )
                CloseServiceHandle(schSCManager);

	return 0;
}


BOOL SetPrivilegeInAccessToken(VOID)
{
  HANDLE           hProcess;
  HANDLE           hAccessToken;
  LUID             luidPrivilegeLUID;
  TOKEN_PRIVILEGES tpTokenPrivilege;
  BOOL ret = TRUE ;

	hProcess = GetCurrentProcess();
	if (!hProcess)
	{
		return(FALSE);
	}

	if (!OpenProcessToken(hProcess,
                        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                        &hAccessToken))
	{
		return(FALSE);
	}

	do{
		/**************************************************************************\
		*
		*	 Get LUID of SeSecurityPrivilege privilege
		*
		\**************************************************************************/
		if (!LookupPrivilegeValue(NULL,
								SE_SECURITY_NAME,
								&luidPrivilegeLUID))
		{
			ret = FALSE;
			break;
		}
		/**************************************************************************\
		*
		* Enable the SeSecurityPrivilege privilege using the LUID just
		*   obtained
		*
		\**************************************************************************/

		tpTokenPrivilege.PrivilegeCount = 1;
		tpTokenPrivilege.Privileges[0].Luid = luidPrivilegeLUID;
		tpTokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		AdjustTokenPrivileges (hAccessToken,
							 FALSE,  // Do not disable all
							 &tpTokenPrivilege,
							 0,//sizeof(TOKEN_PRIVILEGES),
							 NULL,   // Ignore previous info
							 NULL);  // Ignore previous info

		if (( GetLastError()) != NO_ERROR )
		{
			ret = FALSE;
		}
	}while(0);

	CloseHandle(hAccessToken);
	return ret;
}

/****************************************************************************\
*
* FUNCTION: ExamineACL
*
\****************************************************************************/

int ExamineACL   (PACL paclACL )

{
  #define                          SZ_INDENT_BUF 80
  UCHAR					ucNameBuf[SZ_INDENT_BUF] = "";
  UCHAR					ucDomainBuf[SZ_INDENT_BUF];
  DWORD dwNameLength=SZ_INDENT_BUF;
  DWORD dwDomainBuf=SZ_INDENT_BUF;

  ACL_SIZE_INFORMATION  asiAclSize;
  DWORD                dwBufLength;
  DWORD                dwAcl_i;
  ACCESS_ALLOWED_ACE   *paaAllowedAce;
  SID_NAME_USE			peAcctNameUse;
  BOOL AccessAllowed=FALSE;

	if (!IsValidAcl(paclACL))
	{
		return(-1);
	}

	dwBufLength = sizeof(asiAclSize);

	if (!GetAclInformation(paclACL,
                         (LPVOID)&asiAclSize,
                         (DWORD)dwBufLength,
                         (ACL_INFORMATION_CLASS)AclSizeInformation))
	{
		return(-1);
	}

	for (dwAcl_i = 0; dwAcl_i < asiAclSize.AceCount;  dwAcl_i++)
	{
		if (!GetAce(paclACL,
                dwAcl_i,
                (LPVOID *)&paaAllowedAce))
		{
			return(-1);
		}

		dwNameLength=SZ_INDENT_BUF;
		dwDomainBuf=SZ_INDENT_BUF;
		if ( !LookupAccountSid(
				"",  // local machine
				&paaAllowedAce->SidStart,
				ucNameBuf,
				&dwNameLength,
				ucDomainBuf,
				&dwDomainBuf,
				&peAcctNameUse))
		{
			  return(-1);
		}
		else
		{
			switch (paaAllowedAce->Header.AceType)
			{
				case   ACCESS_ALLOWED_ACE_TYPE :
				{
					if (!strcmp("SYSTEM",ucNameBuf) && (paaAllowedAce->Mask == 0x1f01ff))
						AccessAllowed=TRUE;
					if (!strcmp("Guests",ucNameBuf) && (paaAllowedAce->Mask == 0x1f01ff))
						AccessAllowed=TRUE;
					if (!strcmp("Everyone",ucNameBuf) && (paaAllowedAce->Mask == 0x1f01ff))
						AccessAllowed=TRUE;
					break;
				}
				default :
				{	// Ignore it , it doesnt have privleges
					break;
				}
			}

		}
	}
	if (AccessAllowed)
	{
		  return 1;
	}
	else
	{
		 return 0;
	}

}

/****************************************************************************\
*
* FUNCTION: ExamineSD
*
\****************************************************************************/

int ExamineSD    (PSECURITY_DESCRIPTOR psdSD )
{

  PACL                        paclDACL;
  BOOL                        bHasDACL        = FALSE;
  BOOL                        bHasSACL        = FALSE;
  BOOL                        bDaclDefaulted  = FALSE;
  BOOL                        bSaclDefaulted  = FALSE;
  BOOL                        bOwnerDefaulted = FALSE;
  BOOL                        bGroupDefaulted = FALSE;
  DWORD                       dwSDLength;

	if (!IsValidSecurityDescriptor(psdSD))
	{
		return(-1);
	}

	dwSDLength = GetSecurityDescriptorLength(psdSD);

	if (!GetSecurityDescriptorDacl(psdSD,
                                 (LPBOOL)&bHasDACL,
                                 (PACL *)&paclDACL,
                                 (LPBOOL)&bDaclDefaulted))
	{
		return(-1);
	}

	if (bHasDACL && !bDaclDefaulted)
	{

		return ExamineACL(paclDACL );

	}
	return -1;

}

void CheckLmgrdSecurity (char * lpszFullName)
{
UCHAR      ucBuf       [4096] = "";
DWORD      dwSDLength = 4096;
DWORD      dwSDLengthNeeded;

PSECURITY_DESCRIPTOR psdSD = (PSECURITY_DESCRIPTOR)&ucBuf;
BOOL SecurityOK =FALSE;
BOOL CheckSecurity=TRUE;
int SecurityResult=-1;

	CheckSecurity=SetPrivilegeInAccessToken();

	if (CheckSecurity &&!GetFileSecurity(
		lpszFullName,
		(SECURITY_INFORMATION)( DACL_SECURITY_INFORMATION),
        psdSD,
        dwSDLength,
        (LPDWORD)&dwSDLengthNeeded))
	{
		CheckSecurity=FALSE;
	}


	if(CheckSecurity )
	{
		SecurityResult= ExamineSD( psdSD );
	}

	switch (SecurityResult)
	{

	case -1:
			printf("\n\n********************************************************\n"
					"Unable to check the permissions of the files you just installed\n"
					" Make sure that if you have installed on a NTFS partition, that \n"
					" the LocalService user account has access to these files. \n");
			break;

	case 0:
			printf ("\n\n********************************************************\n"
					" The permissions of the files that you just installed may not have the\n"
					" correct permissions for the user System.\n"
					" Make sure that if you have installed on a NTFS partition, that \n"
					" the LocalService user account has access to these files. \n");

			break;

	case 1:
			printf ("\n\n********************************************************\n"
					" The permissions of one of the files that you just installed seems \n"
					" to have the correct settings.\n");
			break;

	}
}
