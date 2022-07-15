/**************************************************************************************************
* Copyright (c) 1997-2022 Flexera. All Rights Reserved.
**************************************************************************************************/

/*

 *
 *	Function:  None
 *
 *	Description: Vendor customization data for server.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef PC
#include <shlobj.h>
#endif
#include "lm_code.h"
#include "lmclient.h"
#include "lm_attr.h"

#ifdef PC
#include <io.h>
typedef int (WINAPI* L_SHCREATEDIRECTORYEX)( HWND hwnd,
    LPCTSTR pszPath,
    const SECURITY_ATTRIBUTES *psa
 );

#define PREV_FLEX_DIR "Macrovision\\FLEXlm"
#endif

#define CURR_FLEX_DIR "FNP\\FLEXlm"

/*
 *	Prototypes for persistent borrow/linger
 */
static void sBorrowInit(char **	ppBuffer, int *	piSize);
static void sBorrowOut(FNP_BORROW_CACHE_ENTRY *borrow,int iSize);
static void sBorrowIn(FNP_BORROW_CACHE_ENTRY *borrow);
static int sGetBorrowInfo(char *szBorrowFilename, char **	ppBuffer, int *	piSize);

extern LM_HANDLE* lm_job;

/* Vendor encryption routine */

char *(*ls_user_crypt)() = 0;

/* Vendor initialization routines */

/* Initalize any vendor specific attributes by calling lc_set_attr_vendor() 
 * from this routine or lc_set_attr(). Internally this callback is called after the 
 * vendor daemon is initalized but before any license files are read. */
void (*ls_user_init_attributes)() = 0; 
/* the user_init0() callback should be used to setup for common 
 * vendor daemon usage. Internally this callback is called at the start of the 
 * vendor daemon intialization before lm_job is initalized. */
void (*ls_user_init0)() = 0;
void (*ls_user_init1)() = 0;
void (*ls_user_init2)() = 0;
void (*ls_user_init3)() = 0;

/*
 *  The ls_ts_recovery() callback should be used if user want to implement 
 *  Automatic Recovery of Server Trusted Storage.
 */
void (*ls_ts_recovery)() = 0;

/* 
 * Extracting client HostIDs: 
 *
 *    In order to get hostid values of licensing client, its mandatory to enable 
 *	  ls_client_hostid_callback and set required hostid types via ls_attr_init_client_hostids API. 
 *    Client hostids could then be extracted with the use of server attributes LS_ATTR_CLIENT_HOSTID_*. 
 *
 *    Note, enabling ls_client_hostid_callback would have an additional communication 
 *    between client and vendor daemon at checkout so enable this callback only when needed. 
 */
void (*ls_client_hostid_callback)() = 0;

/* Checkout filter, Overdraft callback, checkin filter, checkin callback */
int (*ls_outfilter)() = 0;
void (*ls_outod_callback)() = 0;
void (*ls_inod_callback)() = 0;
int (*ls_infilter)() = 0;
int (*ls_incallback)() = 0;

/* vendor message */
char *(*ls_vendor_msg)() = 0;
int ls_vendor_msg_async = 0;
size_t ls_vendor_msg_list_size = 64;

/* secure comms */
int ls_secure_comms = 0;				/* 0=disabled, 1=enabled */

/* secure comms setup thread-pool */
int ls_secure_comms_tp_setup = 0;		/* 0=Use default, >0 defines number of threads in setup thread-pool */

/* secure comms proxy thread-pool */
int ls_secure_comms_tp_proxy = 0;		/* 0=Use default, >0 defines number of threads in proxy thread-pool */


/* callback for client shutdown p8031 */
void (*ls_user_down) () = 0;

/* Vendor daemon shutdown callback */
void (*ls_vd_shutdown)() = 0;	

VENDORCODE vendorkeys[] = {		/* Possible keys for vendor daemons */
		{ VENDORCODE_7,
		ENCRYPTION_SEED1^VENDOR_KEY5, ENCRYPTION_SEED2^VENDOR_KEY5,
		  VENDOR_KEY1, VENDOR_KEY2, VENDOR_KEY3, VENDOR_KEY4,
		  FLEXLM_VERSION, FLEXLM_REVISION, FLEXLM_PATCH,
		  LM_BEHAVIOR_CURRENT, {TRL_KEY1, TRL_KEY2}, 0,
		  LM_STRENGTH, LM_SIGN_LEVEL
		},
		    	   };	/* End of vendor codes*/

int keysize = sizeof(vendorkeys)/sizeof(vendorkeys[0]);

int ls_crypt_case_sensitive = 0; /* Is license key case-sensitive? -
					Only used for vendor-supplied
					encryption routines. */
/*
 *      ls_user_lockfile:  We no longer recommend changing this
 *          here, since the text string for the lockfile will
 *          be visible in the binary, and could be therefore
 *          changed.
 */

char *ls_user_lockfile = (char *)NULL;
int ls_read_wait = 10;		/* How long to wait for solicited reads */
int ls_dump_send_data = 0;	/* Set to non-zero value for debug output */
int ls_normal_hostid = 1;	/* <> 0 -> normal hostid call, 0 -> extended */
int ls_conn_timeout = MASTER_WAIT;  /* How long to wait for a connection */
int ls_enforce_startdate = 1;	/* Enforce start date in features */
int ls_tell_startdate = 1;	/* Tell the user if it is earlier than start
								date */
int ls_minimum_user_timeout = 900; /* Minimum user inactivity timeout (seconds)
					<= 0 -> activity timeout disabled */
int ls_server_override_client_tcp_timeout = 0; /* TCP keepalive timeout/LM_A_TCP_TIMEOUT (seconds) set/overridden by the SERVER for all clients irrespective of LM_A_TCP_TIMEOUT value set at client end.
					< 300(Min value = TCP default Idle timeout value) or > 15300(Max value of LM_A_TCP_TIMEOUT) -> Disabled. LM_A_TCP_TIMEOUT value set by the CLIENT is considered. */
int ls_min_lmremove = 120;	/* Minimum amount of time (seconds) that a
				   client must be connected to the daemon before
				   an "lmremove" command will work. */
int ls_use_featset = 0;		/* Use the FEATURESET line from the license
									file */
int ls_use_all_feature_lines = 0; /* Use ALL copies of feature lines that are
				     unique if non-zero (ADDITIVE licenses) */
int ls_do_checkroot = 0;	/* Perform check that we are running on the
				   real root of filesystem.  NOTE: this check
				   will fail on diskless systems, but they
				   shouldn't be used as server nodes, anyway */
int ls_show_vendor_def = 0;	/* If non-zero, the vendor daemon will send
				   the vendor-defined checkout data back in
				   lm_userlist() calls */
int ls_allow_borrow = 0;	/* Allow "Borrowing" of licenses by another
				   server */


/* Hostid redirection verification routine */

int (*ls_hostid_redirect_verify)() = 0;
				/* Hostid Redirection verification */

/* Vendor-defined periodic call */

void (*ls_daemon_periodic)() = 0;
				/* Vendor-defined periodic call in daemon */
void (*ls_child_exited)() = 0;
				/* Vendor-defined callback -- called with
				   CLIENT_DATA * argument */

int ls_compare_vendor_on_increment = 0;	/* Compare vendor-defined fields to
					 declare matches for INCREMENT lines? */
int ls_compare_vendor_on_upgrade = 0;	/* Compare vendor-defined fields to
					   declare matches for UPGRADE lines? */
/* EntitlementId a license pool component:

             Set to 1 (Default): include entitlement id in license pool

             Set to 0 : entitlement id not included in license pool

*/
int ls_entitlement_based_pooling = 1;
/*
 *	ls_a_behavior_ver can also be set in lm_code.h.
 *	lm_code.h takes priority.  If set there, the value here
 *	will not be used.
 */
char *ls_a_behavior_ver = 0;	/* like LM_A_BEHAVIOR_VER */

int ls_a_check_baddate = 0;		/* like LM_A_CHECK_BADDATE */
int ls_a_lkey_start_date = 0;		/* like LM_A_KEY_START_DATE */
int ls_a_lkey_long = 0;			/* like LM_A_KEY_LONG */
int ls_a_license_case_sensitive = 0;	/* like LM_A_LICENSE_CASE_SENSITIVE */

int ls_hud_hostid_case_sensitive = 0; 	/* Forces hostid checks for hostname,     */
					/* user, or display to be case sensitive. */

void (*ls_a_user_crypt_filter)() = 0;
void (*ls_a_phase2_app)() = 0;
#define TWELVE_HOURS (12 * 60 * 60)
int ls_user_based_reread_delay = TWELVE_HOURS;	/* Affects USER_BASED and HOST_BASED:
					 * a reread changes the  INCLUDE
					 * after this many hours.  Default
					 * is 12 hours.  Value is in seconds.
					 * -1 means that the INCLUDE list
					 * for these features cannot be changed
					 * via lmreread.
					 */

int ls_client_removal_on_reread = 1;		/* Affects removal of unauthenticated clients post Server reread. */

/* callback routine for semaphore name change 
 * use lc_set_attr_vendor(  LM_A_USER_LOCK_CALLBACK ) to set this for 
 * a vendor other than the primary vendor*/
char *(*ls_user_lock)() = NULL;         

int ls_borrow_return_early = 0;	/* Set to 1 to allow users to return borrowed licenses early */

int ls_no_ipaddress_in_server_cache = 0;		/* Server borrow cache will not save the client IP address while borrow checkout */
/*
 *	Forces vendor daemon to send oldest license if a client's version doesn't match a supported signature
 */
int ls_send_oldest_signature = 0;

/*
 *  When set, forces vendor daemon to use only one (the oldest) of multiple candidate cross-version signatures
 *  listed in the feature line for a legacy client's request. Legacy client is any client with version < 11.12.0.0
 */
int ls_single_xver_signature = 0;

/*
 *  When set, forces vendor daemon to communicate on commRev4 message with others.
 */
int ls_use_exclusive_commRev4 = 0;

/*
* When set, prefer f->timeout values over client->tcp_timeout values
*              0 - disabled (default)
*              1 - enabled
*/
unsigned int ls_prefer_feature_timeout_over_tcp = 0;

/*
 *  Allows access to trusted storage.
 */
int ls_use_trusted_storage = 1;

int ls_allow_physical_ethernetid_only = 0; /* Set to 1 to Authenticate only physical adapter hostid  - */

/*
 * Virtualization support for vendor daemon.
 * VM_ONLY  - Vendor Daemon can be operated in Virtual machine only
 * PHYSICAL - Vendor Daemon can be operated in Physical machine only
 */
FLEX_VM_TYPE ls_allow_vm = VM_ALL;

/* Hostid validation interval for AMZN Cloud server hostid.
   Specify in minutes; 0 means no heartbeat */
unsigned int ls_ba_heartbeat_interval = 30; 
/* The retry count for AMZN Cloud server hostid validation */
unsigned int ls_ba_heartbeat_retry_count = 3; 
/* Hostid validation retry interval for AMZN Cloud server hostid.
   Specify in minutes; 0 means no heartbeat */
unsigned int ls_ba_heartbeat_retry_interval = 5; 
unsigned int ls_allow_tz_override = 0; /* Set to 1 to allow server time zone and time to be overridden */

unsigned int ls_daemon_heartbeat_timeout = 300;	/* vendor daemon to lmgrd timeout value
										default is 300 seconds */

void (*ls_borrow_out)(FNP_BORROW_CACHE_ENTRY *borrow,int iSize)  = sBorrowOut;
void (*ls_borrow_in)(FNP_BORROW_CACHE_ENTRY *borrow) = sBorrowIn;
void (*ls_borrow_init)(char ** pszBorrowBuffer, int * piSize) = sBorrowInit;

/*
 *	When 1, Disallows the queuing with MAX keyword, when checkout request exceeds MAX value 
 */
int ls_disable_queue_on_max = 0 ; 

/* 
 * Set to 1 to allow vendor daemon select time out customization. 
 */
unsigned int ls_support_custom_daemon_select_timeout = 0; 

/*
 *	Activation borrow reclaim:
 *		When this vendor variable is set to 0, activation borrow reclaim operation is not allowed.
 *			By default, the borrow reclaim percentage is set to 0.
 *		When this vendor variable is set to X (X>0), X% of total deduction records is allowed to be reclaimed.		
 */
unsigned int ls_ts_borrow_reclaim_percentage = 0;

/*
 *	Poll interval to detect server-side trusted storage license rights update:
 *		The allowed values for this vendor variable is 30 <= X <= 86400, 30 seconds minimum and 24 hours maximum.
 *		By default, the poll interval is set to 600 seconds.
 *
 *		When this vendor variable is set to 0, poll is disabled and the server-side trusted storage update is detected by midnight reread only.
 *		When this vendor variable is set to X, poll is done at every X seconds. If server-side trusted storage update is detected at poll, 
 *		the vendor daemon reread is triggered.
 */
unsigned int ls_ts_update_poll_interval = 600; /* in seconds */

/*
 *  HASP4 dongle support on server:
 *              0 - not supported (default)
 *              1 - supported
 */
unsigned int ls_flexid9_hasp4_support = 0;

/*
 * Diagnostics output port
 *				0 - disabled
 *				1 - enabled
 *
 */
int ls_diagnostics_enabled = 1;

/*
 *  MAX_CONNECTIONS option file functionality:
 *              0 - disabled
 *              1 - enabled (default)
 */
unsigned int ls_max_connections_enable = 1;

/*
 *  To allow updated feature to validate server borrow cache on restart:
 *              0 - disabled (default)
 *              1 - enabled
 */
unsigned int ls_allow_updated_feature_borrow = 0;

/*
 *  To disable/enable the DUP_GROUP functionality with checkout filter:
 *              0 - disabled (default)
 *              1 - enabled 
 */
unsigned int ls_checkout_filter_dup_enable = 0;

/*
 *	Code to determine location of "borrowing" file
 */

#ifdef PC /* WINNT */

/***********************************************************************/
/* @Brief Description:  It creates the borrow cache file folder path
 *
 * @Parameters:  pFlexPath, pBorrowFolder, createFolder
 * 
 * @Retun: void
 ************************************************************************/
static
void
sGetFlexPath(
	char *	pFlexPath,
	char *  pBorrowFolder,
	int 	createFolder)
{
	L_SHGETFOLDERPATHA		GSISHGetFolderPath = NULL;
	HMODULE 				hSHFolder = NULL;
	L_SHCREATEDIRECTORYEX	GSISHCreateDir = NULL;
	HMODULE 				hShell = NULL;
	char					fullBorrowPath[MAX_PATH] = {'\0'};
	char 					systemPath[MAX_PATH] = {'\0'};
	char 					flexLMDir[MAX_PATH] = {'\0'};
	char *					commonfilebuffer = NULL;
	int						gotValidPath = 0;

	if( !pFlexPath )
	{
		printf("Internal Error: Invalid borrow cache return buffer\n");
		return;
	}

	strncpy(flexLMDir, pBorrowFolder, MAX_PATH - 1);
	flexLMDir[MAX_PATH - 1] = '\0';

	hSHFolder = LoadLibrary("shfolder.dll");
		
	if ( hSHFolder != NULL )
	{	
		GSISHGetFolderPath = (L_SHGETFOLDERPATHA)
			GetProcAddress(hSHFolder, "SHGetFolderPathA");
		if (GSISHGetFolderPath &&
			(*GSISHGetFolderPath)(NULL, CSIDL_COMMON_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, systemPath) == S_OK)
		{
			if((strlen(systemPath) + strlen(flexLMDir)) < MAX_PATH)
			{
				sprintf(fullBorrowPath,"%s\\%s", systemPath, flexLMDir);
				gotValidPath = 1;
			}
		}
		FreeLibrary(hSHFolder);
		hSHFolder = NULL;
	}
	else 
	{
		commonfilebuffer = getenv("ALLUSERSPROFILE");
		
		if ( commonfilebuffer && ((strlen(commonfilebuffer) + strlen("\\Application Data\\") + strlen(flexLMDir)) < MAX_PATH)) 
		{
			sprintf(fullBorrowPath,"%s\\Application Data\\%s",commonfilebuffer, flexLMDir);
			gotValidPath = 1;
		}
	}

	if( createFolder == 1 ) 
	{
		hShell = LoadLibrary("shell32.dll");
		if(hShell != NULL)
		{
			GSISHCreateDir = (L_SHCREATEDIRECTORYEX)GetProcAddress(hShell, "SHCreateDirectoryExA");
		}
		else
		{
			printf("Internal Error: Loading the library failed\n");
		}
	}

	if( gotValidPath && ( _access(fullBorrowPath, 0 /*existence*/) == -1 ) )
	{
		gotValidPath = 0 ;
		/* Create FNP\Flexlm folder  */
		if( createFolder == 1 ) 
		{
			if(GSISHCreateDir && (*GSISHCreateDir)(NULL, fullBorrowPath, NULL) ==  ERROR_SUCCESS )
			{
				gotValidPath = 1;
			}
		}
	}

	if( !gotValidPath ) 
	{
		/*    Use default     */
		strcpy(fullBorrowPath, DEFAULT_FLEXLM_DIR);
		gotValidPath = 1;
		
		if( createFolder == 1 )
		{
			if(GSISHCreateDir && (*GSISHCreateDir)(NULL, fullBorrowPath, NULL) !=  ERROR_SUCCESS )
			{
				printf("Unable to create a borrow cache directory %s\n",fullBorrowPath);
			}
		}
	}

	if ( hShell )
	{
		FreeLibrary(hShell);
		hShell = NULL;
	}
		
	/*    Copy data over    */
	
	strcpy(pFlexPath, fullBorrowPath);
	
}

#else /* !PC WINNT */

/***********************************************************************/
/* @Brief Description: Checks the borrow cache file folder Path in case of non-windows
 *
 * @Parameters: pFlexPath, pBorrowFolder, createFolder
 *				createFolder is not used
 * 
 * @Return:  void
 ************************************************************************/
static
void
sGetFlexPath(
	char *	pFlexPath,
	char *  pBorrowFolder,
	int 	createFolder)
{
	strcpy(pFlexPath, "/var/tmp");
}
#endif /* WINNT */

static
void
sBorrowOut(
	FNP_BORROW_CACHE_ENTRY *borrow,
	int		iSize)
{
	FILE *	fp = NULL;
	char	buffer[MAX_PATH] = {'\0'};
	char	szBorrowFilename[MAX_PATH] = {'\0'};

	if( borrow == NULL )
		return;

	sGetFlexPath(buffer, CURR_FLEX_DIR, 1);
	sprintf(szBorrowFilename, "%s%s%sborrow", buffer,
		DIRECTORY_SEPARATOR, VENDOR_NAME);
	/*
	 *	Open file and position at EOF
	 */
	fp = fopen(szBorrowFilename, "ab");
	if(fp)
	{
		/*
		 *	Write entry
		 */
		(void)fwrite(borrow->buffer, iSize, 1, fp);
		(void)fclose(fp);
	}
	return;
}

static
void
sBorrowIn(
	FNP_BORROW_CACHE_ENTRY *borrow)
{
	FILE *	fp = NULL;
	char	entry[FNP_MAX_BORROW_ENTRY_SIZE] = {'\0'};
	char *	pPosition = NULL;
	char *	pBuffer = NULL;
	char *	pCurr = NULL;
	int		length = 0;
	long	filesize = 0;
	long	remaining = 0;
	char	buffer[MAX_PATH] = {'\0'};
	char	szBorrowFilename[MAX_PATH] = {'\0'};

	if( borrow == NULL )
		return;
		
	sGetFlexPath(buffer, CURR_FLEX_DIR, 1);
	sprintf(szBorrowFilename, "%s%s%sborrow", buffer,
		DIRECTORY_SEPARATOR, VENDOR_NAME);

	/*
	 *	Open borrow file
	 */	
	fp = fopen(szBorrowFilename, "rb");
	if(fp)
	{
		/*
		 *	Allocate buffer to hold contents of new file
		 */
		(void)fseek(fp, 0, SEEK_END);
		remaining = filesize = ftell(fp);
		if(filesize > 0)
		{
			pBuffer = (char *)malloc(sizeof(char) * ( unsigned long )filesize);
			if(!pBuffer)
			{
				(void)fclose(fp);
				return;
			}
			(void)fseek(fp, 0, SEEK_SET);
			/*
			 *	Read entire contents of file into memory
			 */
			(void)fread(pBuffer, filesize, 1, fp);	/* overrun checked */
			(void)fclose(fp);
			fp = NULL;
			pCurr = pBuffer;
		}
		else
		{
			/*
			 *	File has no data
			 */	    	
			(void)fclose(fp);
			return;
		}

		while( borrow != NULL ) 
		{
			pCurr = pBuffer;
			remaining = filesize; 

			while( remaining )
			{
				/*
				 *	Read length of entry
				 */
				memset(entry, 0, sizeof(entry));
				memcpy((void *)&length, pCurr, sizeof(length));
				if( length > remaining )
				{
					if( pBuffer )
					{
						free(pBuffer);
						pBuffer = NULL;
					}
					return;
				}
				memcpy(entry, pCurr, length);
				remaining -= length;
				pPosition = pCurr;
				pCurr += length;
			
				/*
				*	Compare with borrow/linger coming back in
				*/
				if( memcmp(entry, borrow->buffer, length) == 0 )
				{
					/*
					 *	Found the match
					 */
					if( remaining )
					{
						/*
						*	Append the remaining portion of the file, starting
						*	at the location for this current entry
						*/
                        memmove(pPosition, pCurr, remaining);
					}
					filesize -= length;
					break;
				}
			}
			borrow = borrow->next;
		
		}

		(void)remove(szBorrowFilename);
		
		fp = fopen(szBorrowFilename, "ab");
		if( fp )
		{
			(void)fwrite(pBuffer, filesize, 1, fp);
			(void)fclose(fp);
			fp = NULL;
		}
			
	}
	
	if( pBuffer )
	{
		free(pBuffer);
		pBuffer = NULL;
	}
	
	return;
}


static
void
sBorrowInit(
	char **	ppBuffer,
	int *	piSize)
{
	char	buffer[MAX_PATH] = {'\0'};
	char	szBorrowFilename[MAX_PATH] = {'\0'};

	sGetFlexPath(buffer, CURR_FLEX_DIR, 1); /* Check whether the FNP folder exists, if it exists then don't create, otherwise create it */
	sprintf(szBorrowFilename, "%s%s%sborrow", buffer,
		DIRECTORY_SEPARATOR, VENDOR_NAME);
	if(sGetBorrowInfo(szBorrowFilename, ppBuffer, piSize ))	/* Read from FNP folder path if the borrow cache file exist */
	{
#ifdef PC 
		/* Check whether Macrovision folder exists,
		 * if it exists then read borrow cache from there,
		 * otherwise read from c:\flexlm folder  
		 */
		buffer[0]='\0';
		sGetFlexPath(buffer, PREV_FLEX_DIR, 0);
		sprintf(szBorrowFilename, "%s%s%sborrow", buffer,
			DIRECTORY_SEPARATOR, VENDOR_NAME);
		sGetBorrowInfo(szBorrowFilename, ppBuffer, piSize);
#endif
	}	
}

/***********************************************************************/
/* @Brief Description: Returns the status whether the borrow cache file is read or not
 *
 * @Parameters: szBorrowFilename , ppBuffer and piSize
 * 
 * @Return: The status 0-success, -1 on failure 
 ************************************************************************/
static
int 
sGetBorrowInfo(
	char *szBorrowFilename, 
	char **	ppBuffer,
	int *	piSize)
{
	FILE *	fp = NULL;
	long	filesize = 0;

	if(szBorrowFilename)
	{
		fp = fopen(szBorrowFilename, "rb");
		
		if(fp)
		{
			(void)fseek(fp, 0, SEEK_END);
			filesize = ftell(fp);
			(void)fseek(fp, 0, SEEK_SET);
			*piSize = 0;
			if (filesize > 0)
			{
				*ppBuffer = (char *)malloc(sizeof(char) * ( unsigned long )filesize);
				if(*ppBuffer)
				{
					(void)fread(*ppBuffer, filesize, 1, fp);	/* overrun checked */
					*piSize = (int)filesize;
				}
			}
			(void)fclose(fp);
			fp = NULL;
			remove(szBorrowFilename);
			return 0;
		}
	}
	return -1;
}
