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
	#include <netinet/tcp.h>
	#include <signal.h>
	#include <arpa/inet.h>
	#include <time.h>
	#include <sys/stat.h>

	#define NOTSET 0
	#define IPV4 1
	#define IPV6 2
	#define FULLSUPPORT 3

	typedef struct 
	{
		char *port;
		char *droot;
	} serverData;

	int setConfig(int confd, serverData * sd);
	int parser(char *request, char *path, char *contentType);
	int getFile(serverData sd, char *path, char *contentType, char *header);
//	int mode(char *device);

#endif
