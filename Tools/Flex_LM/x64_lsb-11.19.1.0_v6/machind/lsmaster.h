#ifndef _LM_MASTER_H_
#define _LM_MASTER_H_

/**************************************************************************************************
* Copyright (c) 1997-2016, 2018-2020, 2022 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *	Description: Local definitions for master daemon
 *
 */

/*
 *	DAEMONS that we have started
 */


typedef struct _daemon {
			char name[MAX_DAEMON_NAME + 1];
			char *path;						/* Path to this DAEMON executable */
			int tcp_port;					/* TCP Internet Port number (local)*/
			int ecomms_port;				/* TCP Ecomms Port number (on master) */
			int m_tcp_port;					/* TCP Port number (on master) */
			int pid;						/* pid of this daemon */
			int print_pid;					/* this is different on PC */
			int recommended_transport;		/* LM_TCP */
			int transport_reason;			/*LM_RESET_BY_USER/APPL*/
			time_t time_started;			/* last time started */
			int num_recent_restarts;
			int nTotalRestarts;
			struct _daemon *next;
			int file_tcp_port;				/* port on DAEMON line */
			int file_ecomms_port;			/* ecomms on DAEMON line */
			char *options_path;				/* path to end-user options file */
			int flex_ver;
			int flex_rev;
			void *daemon_data;

		  } DAEMON;

/*
 * if (imaster) then tcp_port == m_tcp_port
 * If another node is master, then they will differ.
 * It takes an LM_DAEMON message from another lmgrd to tell lmgrd that
 *    about these (master) ports.
 */
void 		l_print_daemon	lm_args(( DAEMON *, char *));
struct _daemon * API_ENTRY l_cur_dlist lm_args((LM_HANDLE *job));
lm_extern DAEMON * API_ENTRY l_get_dlist lm_args((LM_HANDLE *job));
lm_extern void API_ENTRY l_free_daemon_list lm_args((LM_HANDLE_PTR job,
                                                      DAEMON *sp));

lm_extern
void API_ENTRY
l_daemonCopyNonAllocatedMembers(
	LM_HANDLE *	job,
	DAEMON *	pTarget,
	DAEMON *	pSource);

void
API_ENTRY
l_daemonCopy(
	LM_HANDLE *	job,
	DAEMON *	pTarget,
	DAEMON *	pSource);

DAEMON * API_ENTRY l_get_dlist_from_server lm_args((LM_HANDLE *));
#endif /*  _LM_MASTER_H_  */
