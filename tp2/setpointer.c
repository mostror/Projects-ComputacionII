#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "tp2.h"
int setpointer (int fd, char* file)
{
	char buff[40];
	int leido, i, j;

	leido = read(fd, buff, 40);
	for(i = 0 ; j < 4 && i < 40; i++)
	{
		if (buff[i] == '\n') 
			j++;
	}
	if (j == 4)
	{
		close(fd);
		if ((fd=open(file, O_RDONLY)) == -1)
		{
			perror("open");
			return -1;
		}

		leido = read(fd, buff, i);
		return fd;
	}
	else 
		return -1;
}
