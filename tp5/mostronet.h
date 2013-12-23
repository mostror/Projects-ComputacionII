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
	#include <mqueue.h>
	#include <pthread.h>

	#define NOTSET 0
	#define IPV4 1
	#define IPV6 2
	#define FULLSUPPORT 3
	typedef struct 
	{
		char *port;
		char *droot;
		char *device;
		int mqd;
		int requests;
		int goOn;
		int mode;
		pthread_mutex_t mutex;
		pthread_mutex_t mutex2;
	} serverData;

	int setConfig(int confd, serverData * sd);
	int parser(char *request, char *path, char *contentType);
	int getFile(serverData *config, char *path, char *contentType, char *header);
	int mode(serverData *config, int _mode);
	void * handle(void *);
	void * safemalloc(size_t size);
	void * quittin(void *);
	int mq_safereceive(int mqd, char *msg, size_t size, unsigned *prio, serverData *config);

#endif
