/**************************************************************************************************
* Copyright (c) 1999-2022 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
 *	Module:	lsserver.h v1.35.0.0
 *
 *	Description: Local definitions for license manager servers
 *
 */

#ifndef _LS_SERVER_H_
#define _LS_SERVER_H_

#include <stdlib.h>
#include "lmclient.h"

#ifdef PC
#include <winsock2.h>
#else
#include <sys/time.h>
#endif

#ifdef getenv
#undef getenv
#define getenv(x) l_getenv(lm_job, x)
#endif /* getenv */
#include <stdio.h>
#include <sys/types.h>
#ifndef PC
#include <arpa/inet.h>
#include <netinet/in.h>
#endif
/*
 *	P3090:  BASE_EXIT was based on NSIG which is a number that 
 *		varies across platforms and across OS releases.  
 *		This has caused serious problems when the vendor daemon 
 *		is built with one BASE_EXIT, but lmgrd another.
 *		In particular, redundant servers fail.
 *		Therefore, we're now identifying existing platforms
 *		and hardcoding BASE_EXIT, and all new platforms should
 *		hard-code BASE_EXIT.
 */

/*
 *	BASE_EXIT must be a number higher than the signals available
 *	on the o/s, and low enough that BASE_SIG + <num> is < 255
 *	which is the highest possible exit value on unix.
 */
#ifdef SUNOS4
#define BASE_EXIT 32
#endif /* SUNOS4 */
#ifdef SUNOS5
#ifdef sparc
#define BASE_EXIT 34 /* sparc 32 bit and 64 bit */
#else
#define BASE_EXIT 44 /* intel 32 bit and 64 bit */
#endif /* sparc */
#endif /* SUNOS5 */
#ifdef UNIXWARE
#define BASE_EXIT 35
#endif /* UNIXWARE */
#ifdef DEC_UNIX_3
#define BASE_EXIT 32
#endif /* ALPHA_OSF */
#ifdef DGX86
#define BASE_EXIT 65
#endif /* DGX86 */
#ifdef SGI
#define BASE_EXIT 65
#endif /* SGI */
#ifdef HP700
#ifdef HP64
#define BASE_EXIT 45
#else
#define BASE_EXIT 31
#endif /* HP64 */
#endif /* HP700 */
#ifdef PMAX
#define BASE_EXIT 32
#endif /* PMAX -- ultrix mips */
#ifdef HP300
#define BASE_EXIT 33
#endif /* HP300 */
#ifdef LINUX
#define BASE_EXIT 32
#endif /* LINUX */
#ifdef MAC10
#define BASE_EXIT 32
#endif /* LINUX */
#ifdef NEC
#define BASE_EXIT 32
#endif /* NEC */
#ifdef RS6000 /* ppc also */
#define BASE_EXIT 64
#endif /* RS6000 */
#ifdef SINIX 
#define BASE_EXIT 32
#endif /* SINIX */
#ifdef SCO 
#define BASE_EXIT 32
#endif /* SCO */
#ifdef FREEBSD
#define BASE_EXIT 32
#endif /* FREEBSD */
#ifdef BSDI
#define BASE_EXIT 32
#endif /* BSDI */
#ifdef sony_news
#define BASE_EXIT 32
#endif /* sony_news */
#ifdef TANDEM
#define BASE_EXIT 35
#endif /* sony_news */
#ifdef HPINTEL_64
#define BASE_EXIT 45
#endif /* HPINTEL_64 */

#ifdef HPINTEL_32
#define BASE_EXIT 45
#endif /* HPINTEL_32 */

#ifdef PC
#define BASE_EXIT 23
#endif /* PC */

#if defined (LYNX) || defined (LYNX_PPC) 
#define BASE_EXIT 22
#endif /* LYNX */

#ifdef cray
#define BASE_EXIT 65
#endif /* cray */

#ifndef BASE_EXIT
	ERROR -- Must define BASE_EXIT
#endif /* BASE_EXIT */


/*
 *	Error exit codes - These are used by the master to tell
 *		what went wrong with the vendor daemon
 */

#define EXIT_BADCONFIG	    BASE_EXIT + 1	/* Bad configuration data */
#define EXIT_WRONGHOST	    BASE_EXIT + 2	/* Invalid host */
#define EXIT_PORT_IN_USE    BASE_EXIT + 3	/* Internet "ADDRESS ALREADY IN USE" */
#define EXIT_NOFEATURES	    BASE_EXIT + 4	/* No features to serve */
#define EXIT_COMM	    BASE_EXIT + 5	/* Communications error */
#define EXIT_DESC	    BASE_EXIT + 6	/* Not enough descriptors to */
							/* re-create pipes */
#define EXIT_NOMASTER	    BASE_EXIT + 7	/* Couldn't find a master */
#define EXIT_SIGNAL	    BASE_EXIT + 8	/* Exited due to some signal */
#define EXIT_SERVERRUNNING  BASE_EXIT + 9	/* Exited because another server */
							    /* was running */
#define EXIT_MALLOC	    BASE_EXIT + 10	/* malloc() failure */
#define EXIT_BICKER	    BASE_EXIT + 11	/* Servers can't agree on who is */
							     /* the master */
#define EXIT_BADDAEMON	    BASE_EXIT + 12	/* DAEMON name doesn't agree between */
						/* daemon and license file */
#define EXIT_CANTEXEC	    BASE_EXIT + 13	/* Child cannot exec requested server */
#define EXIT_REQUESTED	    BASE_EXIT + 14	/* lmgrd requested vendor daemon down */
#define EXIT_EXPIRED	    BASE_EXIT + 15	/* demo version has expired */
#define EXIT_BADCALL	    BASE_EXIT + 16	/* vendor daemon started incorrectly */
#define EXIT_INCONSISTENT   BASE_EXIT + 17	/* vendor daemon consistency error */
#define EXIT_FEATSET_ERROR  BASE_EXIT + 18	/* Feature-set inconsistent */
#define EXIT_BORROW_ERROR   BASE_EXIT + 19	/* Borrow database corrupted */
#define EXIT_NO_LICENSE_FILE BASE_EXIT + 20	/* No license file */
#define EXIT_NO_SERVERS     BASE_EXIT + 20	/* Vendor keys don't support */
#define EXIT_VM_NOT_ALLOW   BASE_EXIT + 21	/* daemon is not allowed to operate in virtual environment */
#define EXIT_VM_ONLY	    BASE_EXIT + 22	/* daemon is not allowed to operate in physical machine */
#define EXIT_3SVRTS_CONF_MISMATCH		BASE_EXIT + 23 /* 3ServerTS - Config data mismatch */
#define EXIT_3SVRTS_FAILOVER_ERROR		BASE_EXIT + 24 /* 3ServerTS - The failover to replicated TS file failed.*/
#define EXIT_3SVRTS_RECOVER_ERROR		BASE_EXIT + 25 /* 3ServerTS - The recovery to original TS file failed */
#define EXIT_3SVRTS_NODE_NOT_CONFIGURED	BASE_EXIT + 26 /* 3ServerTS - Node not configured */
#define EXIT_VMW_ONLY	   BASE_EXIT + 27	/* Vendor daemon can be operated in VMware environment only */
#define EXIT_HOSTID_ENV_MISMATCH		BASE_EXIT + 28 /* BMB support - SERVER line hostid and license server environment mismatch. */ 
/* BASE_EXIT + 29 this status is reserved but will never be generated */
/* BASE_EXIT + 30 this status is reserved but will never be generated */
/* BASE_EXIT + 31 this status is reserved but will never be generated */ 
#define EXIT_BAD_VM_KEY				BASE_EXIT + 32 /* FNP vendor keys do not support Virtualization feature. */
#define EXIT_HPV_ONLY	   				BASE_EXIT + 33	/* Vendor daemon can be operated in Hyper-V environment only */
#define EXIT_UNSUPPORTED_AMZN_HOSTID		BASE_EXIT + 34 /* The AMZN hostid on SERVER line doesn't supported by vendor daemon */
#define EXIT_3SVRTS_FAILOVER_PERIOD_END	BASE_EXIT + 36 /* 3ServerTS - The allowed failover period ended.*/
#define EXIT_3SVRTS_GET_FAILOVER_PERIOD_FAILED BASE_EXIT + 37 /* 3ServerTS - Failed to retrieve the start time for the failover period. */
#define EXIT_XEN_ONLY	   				BASE_EXIT + 38	/* Vendor daemon can be operated in XEN environment only */
#define EXIT_3SVRTS_INCOMPLETE_CONFIG	BASE_EXIT + 39  /* 3ServerTS - Incomplete configuration on the node. */
#define EXIT_3SVRTS_TRIAD_HOSTID_MISMATCH	BASE_EXIT + 40 /* 3ServerTS - Triad hostid mismtach. */
#define EXIT_3SVRTS_TS_NOT_FOR_THIS_HOST    BASE_EXIT + 41 /* 3ServerTS - TS file not for this host. */
#define EXIT_TS_BINDING_CHANGED				BASE_EXIT + 42 /* Trusted storage binding change detected. */
#define EXIT_VIRTUALBOX_ONLY	   				BASE_EXIT + 43	/* Vendor daemon can be operated in VirtualBox environment only */
#define EXIT_NO_LOCALHOST               BASE_EXIT + 44      /* Not able to resolve local host */
#define EXIT_QEMU_ONLY	   				BASE_EXIT + 45	/* Vendor daemon can be operated in Qemu environment only */
#define EXIT_PARALLELS_ONLY	   			BASE_EXIT + 46	/* Vendor daemon can be operated in Parallels environment only */
#define EXIT_AMAZONEC2_ONLY	   			BASE_EXIT + 47	/* Vendor daemon can be operated in Amazon Ec2 environment only */
#define EXIT_GOOGLE_COMPUTE_ONLY	   	BASE_EXIT + 48	/* Vendor daemon can be operated in Google Compute environment only */
#define EXIT_AZURE_ONLY	   				BASE_EXIT + 49	/* Vendor daemon can be operated in Azure environment only */
#define EXIT_HOSTID_DISCONTINUED		BASE_EXIT + 50	/* HostID support has been discontinued */
#define EXIT_OUT_OF_VALID_PORT_RANGE	BASE_EXIT + 51	/* The port mentioned in the license file is out of range (1 - 65535) */
#define EXIT_PORTNUM_NOTSUPP           	BASE_EXIT + 52	/* Port number is incompatible between lmgrd and VD. */
#define EXIT_EVERRUN_ONLY	   			BASE_EXIT + 53	/* Vendor daemon can be operated in Everrun environment only */

typedef unsigned char SERVERNUM;
#define MAX_SERVER_NUM 256		/* Maximum # of servers in a "chain" */

#ifdef MULTIPLE_VD_SPAWN
extern SERVERNUM snum;	/* Our server number */
#endif

#define TRUE 1
#define FALSE 0
#define INCLUDE_DUMP_CMD	    /* Define to include (debugging) dump command */
#define MASTER_TIMEOUT 10	    /* # of seconds to wait for master to respond */
#define TIMERSECS 60                /* Timer to go off to send data to another lmgrd */
#define FIFTEEN_MINUTES (15 * 60)
#define REPLOG_TIMESTAMP FIFTEEN_MINUTES
#define MASTER_ALIVE (2*TIMERSECS)  /* # seconds to wait for any data from */
				    /* another lmgrd - if we don't get it, 
				      declare the other lmgrd down */

#define CLIENT_FD   0
#define CLIENT_SOCK 1
typedef int HANDLE_T;

#ifdef SUPPORT_FIFO
typedef struct _ls_local_addr {
	int server_write; /* write fd */
	char *name;	/* there are actually two files: with
				 * "cr" or "cw" appended for client read
				 * or client write -- destroyer of
				 * CLIENT_ADDR is responsible for freeing
				 * malloc's memory.  This is the name
				 * that's sent in the original message from
				 * client.
				 */
} LS_LOCAL_DATA;
#endif

typedef union _addr_id  {
			LM_SOCKET	fd;		/* if is_fd */
			struct sockaddr_in sin; 	/* if LM_UDP */
		} ADDR_ID;
	
typedef struct _client_addr {
		ADDR_ID addr; 		/* enough to uniquely identify */
		int is_fd; 		/*boolean -- TRUE if fd address*/
		int transport;          /*LM_TCP, LM_LOCAL*/
#ifdef SUPPORT_FIFO
		LS_LOCAL_DATA local;
#endif
	} CLIENT_ADDR;

#define MAX_CLIENT_HANDLE sizeof(HANDLE_T) /*(32 bit int)*/

typedef struct _client_group {
	struct _client_group *next;	/* Forward link */
	char *	name;	/* Group name */
	char *	list;	/* List for INCLUDE, EXCLUDE */
} CLIENT_GROUP;

typedef struct _client_vendor_data {
        struct _client_vendor_data *next;     /* Forward link */
        char *  name;   /* Feature Name */
        long 	flags;  
} CLIENT_VENDOR_DATA;


typedef struct client_data {
		CLIENT_ADDR addr;		/* fd / sockaddr*/
		HANDLE_T handle;
		char name[MAX_LONGNAME_SIZE + 1];	/* Username */
		char node[MAX_LONGNAME_SIZE + 1];	/* Node */
		char display[MAX_LONGNAME_SIZE + 1];	/* Display */
		char vendor_def[MAX_VENDOR_CHECKOUT_DATA + 1]; /* Changes with
								each checkout */
		char client_vendor_def[MAX_VENDOR_CHECKOUT_DATA + 1]; /*Internal: same as vendor_def until LS_ATTR_CHECKOUT_DATA used*/
#define USE_VENDOR_DEF_INTERNAL    0x0         /* Internal: use vendor_def to match request*/ 
#define USE_CLIENT_VENDOR_DEF_INTERNAL 	0x1    /* Internal: use client_vendor_def to match request*/
		short use_client_vendor_def;
		short vendor_def_override;
		short use_vendor_def;		/* Use vendor_def data */
		short inet_addr[4];		/* Internet addr - deprecated */
		long time;	/* Time started */
		char platform[MAX_PLATFORM_NAME + 1];	/* e.g., sun4_u4 */
		int		nTimeZoneInMin;		/* current time zone of the client */
		time_t  tTimeInMin;			/* current time of the client */
		unsigned int vm_platform;	/* VM platform */
		unsigned int clientVMKeySupport;	/* Cleint VM key support/unsupport */
		time_t lastcomm;	/* Last communications 
					 * received (or heartbeat sent) 
					 * if -1, marked for later deletion
					 */
#define	LS_DELETE_THIS_CLIENT 0xffffffff
		struct _FlexCrypt *encryption; /* Comm encryption code */
		int comm_version; /* Client's communication version */
		int comm_revision; /* Client's communication revision */
		int tcp_timeout;	
		long pid;		/* client's pid */
		HOSTID *hostids;       /* null-terminated array of
					 * client's hostids */
		char *project;		/* $LM_PROJECT for reportlog */
		short capacity;		/* Multiplier for checkout cnt */
		unsigned short flexlm_ver; /* FLEXlm ver (>= v5) */
		unsigned short flexlm_rev; /* FLEXlm revision (>= v5) */
		int curr_cpu_usage[4];	/* Usage numbers */
		int last_logged_cpu_usage[4]; /* Last logged Usage numbers */
#define CPU_USAGE_DEFAULT_DELTA 3 		/* 10ths/sec -- .3 sec */
#define CPU_USAGE_CHECKIN_ONLY	0xffffff	/* no usage with heartbeats */
#define CPU_USAGE_DEFAULT_INTERVAL CPU_USAGE_CHECKIN_ONLY
		CLIENT_GROUP *groups;			/* groups this user */
		CLIENT_GROUP *hostgroups ;		/*   belongs to */
		CLIENT_GROUP *inetgroups ;		

		long flags; /*long is 32 bit by standard, 0b0000 0000 0000 0000 0000 0000 0000 0000*/

#define CLIENT_FLAG_BADDATE_DETECTED				0x00000001
#define CLIENT_FLAG_BORROWER						0x00000002
#define CLIENT_FLAG_NOMORE							0x00000004
#define CLIENT_FLAG_INTERNAL1						0x00000008

#define CLIENT_FLAG_INTERNAL2						0x00000010
#define CLIENT_FLAG_INTERNAL3						0x00000020
#define CLIENT_FLAG_INTERNAL4						0x00000040
#define CLIENT_FLAG_LINGER							0x00000080

#define CLIENT_FLAG_BORROW_INIT						0x00000100	/* Used to indicate that client was created for sole purpose
																	of restoring borrow/linger info */

#define CLIENT_FLAG_ACTIVATION_IN_PROGRESS			0x00000200	/* Set when a activation request is started, 
																	cleared when it is done */

#define CLIENT_FLAG_CLIENT_DISCONNECTED_ACTIVATION	0x00000400	/* Set when CLIENT_FLAG_ACTIVATION_IN_PROGRESS is set 
																	and the client has died.  This in conjunction with 
																	CLIENT_FLAG_ACTIVATION_IN_PROGRESS can be used to cleanup
																	this client at a later stage 
																	(maybe while checking for lingers). */

#define CLIENT_FLAG_ACTIVATION_CLIENT				0x00000800	/* Set such that on a checkup of a license the vendor daemon 
																	does not try to send status info to client 
																	(since it isn't a real client). */

#define CLIENT_FLAG_DQUEUE_REQUEST					0x00001000	/* Queue Flag */
#define CLIENT_FLAG_FOR_DEC_CONN					0x00002000
#define CLIENT_FLAG_IS_UTIL							0x00004000
#define CLIENT_FLAG_EXTENDED_SIZE_SUPPORT			0x00008000
#define CLIENT_FLAG_USING_ECOMMS 					0x00010000

		long internal1;		
		unsigned short ckout_sernum;
		unsigned long group_id;	
#define NO_GROUP_ID 0xffffffff
		char *sn;
		unsigned int borrow_seconds;
		struct _vendor_info *vendorInfo;
		struct _commBuffer *commBufRead;
		int err;
		int featureCount;
		/* private data for client data. size will change across
		 * releases */
		char internalData[MAX_IPADDRESS_SIZE+1];
		char internet_override[MAX_IPADDRESS_SIZE+1];
		unsigned short client_commrev_support;
#define FNP_CLIENT_SUPPORT_PRIOR_COMMREV4	0
#define FNP_CLIENT_SUPPORT_COMMREV4_AND_LATER	1
		char act_user_name[MAX_LONGNAME_SIZE + 1];	/* Activation username */
		int nCOAvailLicCount;	/* For an internal use only */
		unsigned short cb_req_hostid_type_index;		/* For an internal use only */
		unsigned short cb_req_hostid_populated; /* For an internal use only */
		struct timeval creation;
		int unique_count;       /* LM_A_MULTIPLE_CHECKOUT_DATA */
		const char* checkout_data;
		CLIENT_VENDOR_DATA	*client_vendor_data;
		int largest_checked_out_feature_timeout;
	} CLIENT_DATA;


typedef struct lm_quorum {
				int count;      /* How many active */
				int quorum;     /* How many required */
				int master;	/* Index into list of master */
				int n_order;	/* How many order messages we
						   need before we start */
				int alpha_order; /* Use alphabetical master
						   selection algorithm */
				LM_SERVER *list; /* Who they are */
				int debug;	/* Debug flag */
			  } LM_QUORUM;


typedef struct ls_pooled {
			struct ls_pooled *next;
			char date[DATE_LEN+1]; 
			char code[MAX_CRYPT_LEN+1];
			char *lc_vendor_def;
			LM_CHAR_PTR lc_dist_info;
			LM_CHAR_PTR lc_user_info;
			LM_CHAR_PTR lc_asset_info;      
			LM_CHAR_PTR lc_issuer;
			LM_CHAR_PTR lc_notice;  
			LM_CHAR_PTR parent_featname;
			LM_CHAR_PTR parent_featkey;
			char *lc_sign;
			int users; 	/* How many expire at that date */
			unsigned int trusted_storage_feat;
			char *entitlementId;/*EntitlementId is also added as pooling component*/
} LS_POOLED;
			
/*
 *	States involved in connecting to other servers (bitmap)
 */
#define C_NONE 0	/* Not started */
#define C_SOCK 1	/* Socket to other server connected */
#define C_SENT 2	/* (fd1) Socket connected, Msg sent to other master */
#define C_CONNECT1 4	/* (fd1) Connection to other server complete */
#define C_CONNECT2 8	/* (fd2) Other server has established connection */
#define C_CONNECTED 16	/* Completely connected */
#define C_MASTER_READY 32 /* Connection complete, master elected and ready */
#define C_TIMEDOUT 64	/* Connection timed out - don't retry */
#define C_NOHOST 128	/* This host doesn't exist */

/*
 *	Logging functions signatures
 */

void
#ifdef ANSI
  ls_obfusc_log_asc_printf(int pObfuscPosition, ...);
#else
  ls_obfusc_log_asc_printf(int pObfuscPosition,  va_alist);       
#endif


/*
 *	Output to error log file
 *
 *	The LOG_INFO macros are used to generate the documentation
 *	on all the LOG calls.  They are always ignored, as far as
 *	the code is concerned.
 */

#ifndef NO_MT_LOGGING
#define LOG_LOCK ls_log_lock()
#define LOG_UNLOCK ls_log_unlock()
#else
#define LOG_LOCK 
#define LOG_UNLOCK 
#endif

#define LOGERR(x) \
	{\
		LOG_LOCK ; \
		ls_log_prefix(LL_LOGTO_BOTH, LL_ERROR); \
		ls_log_error x;\
		LOG_UNLOCK ; \
	}
#define _LOGERR(x) { LOG_LOCK ; ls_log_error x ; LOG_UNLOCK ; }

#ifndef RELEASE_VERSION
#define LOG(x) \
	{ \
		LOG_LOCK ; \
		ls_log_prefix(LL_LOGTO_ASCII, 0); \
		(void) ls_log_asc_printf x; \
		LOG_UNLOCK ; \
	}
#else /* RELEASE_VERSION */
#define LOG(x) \
	{ \
		LOG_LOCK ; \
		ls_log_prefix(LL_LOGTO_ASCII, 0); \
		(void) ls_log_asc_printf x; \
		LOG_UNLOCK ; \
	}
#endif /* RELEASE_VERSION */
#define LOG_INFO(x)

#define OBF_LOG(x) \
	{ \
		LOG_LOCK ; \
		ls_log_prefix(LL_LOGTO_ASCII, 0); \
		(void) ls_obfusc_log_asc_printf x; \
		LOG_UNLOCK ; \
	}


/*
 *	If LOG_TIME_AT_START is defined, all daemon logging is
 *	done with timestamps at the beginning of the lines, rather
 *	than just certain (important) lines being timestamped at the
 *	end.  LOG_TIME_AT_START became the default in v1.3.
 */
#define LOG_TIME_AT_START	/* Log time @ start of log lines */

/* 
 *	Default directory for lock file and lmgrd info file
 */

#ifdef WINNT 
#define LS_LOCKDIR "C:\\flexlm"
#else
#if defined(BSDI) || defined (MONTAVISTA)
#define LS_LOCKDIR "/tmp/.flexlm"
#else
#ifdef MAC10
#define LS_LOCKDIR "/var/tmp/.flexlm"
#else
#define LS_LOCKDIR "/usr/tmp/.flexlm"
#endif /* MAC10 */
#endif /* BSDI */
#endif /* WINNT */

/*
 *	Output to log without header (for building strings)
 */
#define _LOG(x) { LOG_LOCK ; (void) ls_log_asc_printf x; LOG_UNLOCK ; }

#define OBF_ULOG(x) { LOG_LOCK ; (void) ls_obfusc_log_asc_printf x; LOG_UNLOCK ; }


extern int dlog_on;
extern int _ls_fifo_fd;		/* Global fifo fd for reading */
extern LM_HANDLE *lm_job;	/* Server's one (and only) job */

#ifndef RELEASE_VERSION
#define DLOG(x) { if (dlog_on) \
	  {LOG_LOCK ; ls_log_prefix(LL_LOGTO_ASCII, -1); ls_log_file_line(LL_LOGTO_ASCII,__FILE__,__LINE__); (void) ls_log_asc_printf x; LOG_UNLOCK ; } }
#else
#define DLOG(x) { if (dlog_on) \
	  { LOG_LOCK ; ls_log_prefix(LL_LOGTO_ASCII, -1); (void) ls_log_asc_printf x; LOG_UNLOCK ; } }
#endif 
/*
 *	Output to log without header (for building strings)
 */
#define _DLOG(x) { if (dlog_on) { LOG_LOCK ; (void) ls_log_asc_printf x; LOG_UNLOCK ; } }


/*
 *	Exchange descriptors so that parent-child communication is happy
 */

#define XCHANGE(_p) { int _t = _p[3]; _p[3] = _p[1]; _p[1] = _t; }

/*
 *	Server's malloc - simply logs an error if malloc fails, and exits
 */

extern char *ls_malloc();
#define LS_MALLOC(x)	ls_malloc((x), __LINE__, __FILE__)

/*
 *	listen() backlog - number of pending connections
 */
#define LISTEN_BACKLOG 500
/*
 *	Get the time 
 */
/* 
 *	Sun/Vax uses _TIME_ to detect <sys/time.h>, Apollo uses the 
 *	same include file for both <time.h> and <sys/time.h>
 */

#ifndef ITIMER_REAL	
#include <time.h>
#endif
#ifdef THREAD_SAFE_TIME
struct tm *ls_gettime(struct tm * ptst);
#else /* !THREAD_SAFE_TIME */
struct tm *ls_gettime();
#endif

/*************************************************************
 * More externs 
 **************************************************************/
extern CLIENT_GROUP *groups;
extern CLIENT_GROUP *hostgroups;


typedef void (LM_CALLBACK_TYPE * LS_CB_USERDOWN) (CLIENT_DATA *);
#define LS_CB_USERDOWN_TYPE (LS_CB_USERDOWN)

/*
 * write a error message to the debug log
 */
lm_extern void ls_log_message(char *message);
lm_extern int ls_set_log_vendor_enable(char * string, int enable);

/*
 * lc_vsend support
 */
extern int ls_vendor_msg_async;
extern char* (*ls_vendor_msg)(char*);
extern size_t ls_vendor_msg_list_size;
extern int ls_api_mutex_lock(int relTimeoutMs);
extern int ls_api_mutex_unlock();
extern int ls_vsend_async_mode();

/*
 * enable secure comms 
 */
extern int ls_secure_comms;

/*
 * secure comms setup thread-pool
*/
extern int ls_secure_comms_tp_setup;

/*
 * secure comms proxy thread-pool
*/
extern int ls_secure_comms_tp_proxy; 

/*
 * Diagnostics logging
 */
extern int ls_diagnostics_enabled;

/*
 * TS recovery support
 */
extern void (*ls_ts_recovery)();

#endif 	/* _LS_SERVER_H_ */
