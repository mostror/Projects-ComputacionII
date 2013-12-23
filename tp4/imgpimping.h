#ifndef _IMGPIMPING_H_
	#define _IMGPIMPING_H_

#include "filter.h"
#include "error.h"
#include "ppm.h"
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>


int inic ();
int openImg(image_t *img, int fd);

#endif
