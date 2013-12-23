#ifndef _MOSTRONET_H_
#define _MOSTRONET_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <string.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <signal.h>
	#include <arpa/inet.h>

	typedef struct 
	{
		char *port;
		char *droot;
	} serverData;

	int setConfig(int confd, serverData * sd);

#endif
