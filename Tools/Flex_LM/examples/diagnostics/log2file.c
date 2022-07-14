/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *  Description:    This is an example program, to demonstrate the capture of
 *                  diagnostic logging information from the Vendor Daemon diagnostic port.
 *                  It is not intended to be a complete production quality tool, it exists 
 *                  purely to illustrate the basic interaction with the diagnostics port.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <afunix.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#endif

/* Definition of the diagnostics port address */
#define DIAGNOSTICS_PORT_ADDRESS "./.logger"

/* Macro for socket family */
#ifdef AF_LOCAL
#define DIAG_SOCKET_DOMAIN  AF_LOCAL
#else
#define DIAG_SOCKET_DOMAIN  AF_UNIX
#endif


int main(int argc, char** argv)
{
	struct sockaddr_un dp_sa;
    int dp_sock = -1;							/* Diagnostics port socket */

	FILE* output_stream = stdout;				/* Stream on which to write diagnostic output * defaults to stdout */
#ifdef WIN32_LEAN_AND_MEAN
	WORD wVersionRequested;
	WSADATA wsaData;
	int errorNum;
#endif
	
	do
	{
		/* Change the output destination if requested */
		if( argc > 1 )
		{
			output_stream = fopen(argv[1],"w");
			if( !output_stream )
			{
				perror(argv[1]);
				break;
			}
		}

#ifdef WIN32_LEAN_AND_MEAN
		/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
		wVersionRequested = MAKEWORD(2, 2);

		errorNum = WSAStartup(wVersionRequested, &wsaData);
		if (errorNum != 0) {
			/* Tell the user that we could not find a usable */
			/* Winsock DLL.                                  */
			printf("WSAStartup failed with error: %d\n", errorNum);
			break;
		}
#endif

		/* Create a local socket */
    	dp_sock = socket(DIAG_SOCKET_DOMAIN, SOCK_STREAM, 0);
		if( dp_sock == -1 )
		{
#ifdef WIN32_LEAN_AND_MEAN
			int err = WSAGetLastError();
			printf("socket() failed with error [%d]\n", err);
#else
			perror("socket");
#endif
			break;
		}

		/* Setup the diagnostics port address */
		memset(&dp_sa, 0, sizeof(dp_sa));
		dp_sa.sun_family = DIAG_SOCKET_DOMAIN;
		strncpy(dp_sa.sun_path, DIAGNOSTICS_PORT_ADDRESS, sizeof(dp_sa.sun_path)-1);

		/* Connect to the diagnostics port */
    	if( connect(dp_sock, (struct sockaddr*)&dp_sa, sizeof(dp_sa) ) == -1 )
		{
#ifdef WIN32_LEAN_AND_MEAN
			int err = WSAGetLastError();
			printf("connect() failed with error [%d]\n", err);
#else
			perror("connect");
#endif
			break;
		}

		/* Loop reading text from the port and writing to the output stream */
    	while(1)
		{
			int res;
			char text[1024];
			memset(text, 0, sizeof(text));

#ifdef WIN32_LEAN_AND_MEAN
			if ( (res=recv(dp_sock, text, sizeof(text), 0)) < 0 )
			{
				int err = WSAGetLastError();
				printf("recv() failed with error [%d]\n", err);
				break;
			}
#else
        	if ( (res=read(dp_sock, text, sizeof(text))) < 0 )
			{
				perror("read");
				break;
			}
#endif
			else if( res == 0 )
			{
				/* The diagnostics port was closed by the Vendor Daemon */
				break;
			}
			else
			{
				if( (fwrite(text, 1, res, output_stream) != res) || (fflush(output_stream) != 0) )
				{
					/* Couldn't write to output stream */;
					perror("output stream");
					break;
				}
			}
		}

    } while(0);

	if( output_stream != stdout )
		(void)fclose(output_stream);

	if( dp_sock != -1 )
#ifdef WIN32_LEAN_AND_MEAN
		(void)closesocket(dp_sock);
#else
    	(void)close(dp_sock);
#endif

#ifdef WIN32_LEAN_AND_MEAN
	(void)WSACleanup();
#endif

    return 0;
}
