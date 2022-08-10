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
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

/* Definition of the diagnostics port address */
#define DIAGNOSTICS_PORT_ADDRESS "./.logger"

/* Macro for socket family */
#ifdef AF_LOCAL
#define DIAG_SOCKET_DOMAIN  AF_LOCAL
#else
#define DIAG_SOCKET_DOMAIN  AF_UNIX
#endif

/* Definition of the network port to serve */
#define NETWORK_PORT_ADDRESS 27010

static int s_install_network_socket(int port)
{
	int enable = 1;
	int res = -1;										/* Pessimistic */
	int sock;
	struct sockaddr_in sa;

	do
	{
		/* Create the network port */
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0)
		{
			perror("network socket create");
			break;
		}
		
		/* Initialize network socket address structure */
		memset(&sa,0,sizeof(sa));
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = INADDR_ANY;
		sa.sin_port = htons(port);

		/* Set the reuse flag on the network socket */
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		{
			perror("network socket setsockopt(SO_REUSEADDR)");
			break;
		}

		/* Bind the socket */
		if( bind(sock, (struct sockaddr *) &sa, sizeof(sa)) < 0 ) 
		{
			perror("network socket bind");
			break;
		}

		/* Return the network socket fd */
		res = sock;

	} while(0);

	/* Tidy-up any errors */
	if( res < 0 && sock >= 0 )
	{
		close(sock);
	}

	return res;
}

int main(int argc, char** argv)
{
	/* Diagnostics port */
	struct sockaddr_un dp_sa;
	int dp_sock = -1;									/* Diagnostics port socket */

	/* Network port */
	int net_port = NETWORK_PORT_ADDRESS;
	int net_sock = -1;
	int client_sock = -1;
	int client_sa_len;
	struct sockaddr_in client_sa;

	do
	{
		/* Change the network port if requested */
		if( argc > 1 )
		{
			net_port = atoi(argv[1]);
			if( net_port <= 0 )
			{
				perror(argv[1]);
				break;
			}
		}

		if( (net_sock = s_install_network_socket(net_port)) < 0 )
			break;

		/* Now start listening for the clients */
		if( listen(net_sock,5) < 0 )
		{
			perror("network socket listen");
			break;
		}
		client_sa_len = sizeof(client_sa);	

		/* Wait for a network client to connect */
		while( (client_sock = accept(net_sock, (struct sockaddr *)&client_sa, &client_sa_len)) < 0 )
			;

		/* We have a client, so now we can connect to the VD to collect diagnostics */

		/* Create a local socket */
		dp_sock = socket(DIAG_SOCKET_DOMAIN, SOCK_STREAM, 0);
		if( dp_sock == -1 )
		{
			perror("diagnostics socket create");
			break;
		}

        /* Setup the diagnostics port address */
		memset(&dp_sa, 0, sizeof(dp_sa));
		dp_sa.sun_family = DIAG_SOCKET_DOMAIN;
		strncpy(dp_sa.sun_path, DIAGNOSTICS_PORT_ADDRESS, sizeof(dp_sa.sun_path)-1);

        /* Connect to the diagnostics port */
        if( connect(dp_sock, (struct sockaddr*)&dp_sa, sizeof(dp_sa) ) == -1 )
        {
            perror("diagnostics socket connect");
            break;
        }

		/* Loop reading text from the port and writing to the network socket */
    	while(1)
		{
			int res;
			char text[1024];
			memset(text, 0, sizeof(text));

        	if ( (res=read(dp_sock, text, sizeof(text))) < 0 )
			{
				perror("diagnostics socket read");
				break;
			}
			else if( res == 0 )
			{
				/* The diagnostics port was closed by the Vendor Daemon */
				break;
			}
			else
			{
				if( (res=write(client_sock, text, res)) < 0 )
				{
					perror("diagnostics socket write");
					break;
				}
				else if( res == 0 )
				{
					/* The network port was closed by the client */
					break;
				}
			}
    	}

	} while(0);

	/* Tidy-up */
	if( dp_sock != -1 )
		(void)close(dp_sock);

	if( net_sock != -1 )
		(void)close(net_sock);

	if( client_sock != -1 )
		(void)close(client_sock);

	return 0;
}
