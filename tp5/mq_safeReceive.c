#include "mostronet.h"

int mq_safereceive(int mqd, char *msg, size_t size,  unsigned *prio, serverData * config)
{
	int res;
	pthread_mutex_lock(&(config->mutex));
	res = mq_receive(mqd, msg, size, prio);
	config->requests++;
	pthread_mutex_unlock(&(config->mutex));
	return res;
}

