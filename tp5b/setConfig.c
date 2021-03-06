#include "mostronet.h"
#include <unistd.h>
#include <fcntl.h>	
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int setConfig(int fd, serverData *sd)
{
	if(fd < 0)
	{
		write(STDERR_FILENO, "Error: invalid file descriptor\n\n", 32);
		return -1;
	} 
	if (sd == NULL)
	{
		write(STDERR_FILENO, "Error: invalid reference to struct\n\n", 36);
		return -1;
	}
	sd->droot = (char *)safemalloc(sizeof(char) * 256);
	sd->port = (char *)safemalloc(sizeof(char) * 256);
	sd->device = (char *)safemalloc(sizeof(char) * 256);

	memset(sd->droot, 0, 256);
	memset(sd->device, 0, 256);
	memset(sd->port, 0, 256);
	int leido;
	char *buffer = malloc(sizeof(char) *256);
	char *aux;
	char *ptr;

	while ((leido = read(fd, buffer, 256)) > 0)

	{
		buffer[leido] = 0;
		if((ptr = strstr(buffer, "port=")))
		{
			aux = strchr(ptr, '\n');
			strncpy(sd->port, (ptr + 5),(int) (aux - ptr - 5));
			*(sd->port +(aux -ptr - 4)) = 0; 
		}
		if((ptr = strstr(buffer, "droot=")))
		{
			aux  = strchr(ptr, '\n');
			strncpy(sd->droot, (ptr + 6), (int) (aux - ptr - 6));
			*(sd->droot +(aux -ptr - 5)) = 0; 
		}
		if ((ptr = strstr(buffer, "device=")))
		{
			aux = strchr(ptr, '\n');
			strncpy(sd->device, (ptr + 7), (int) (aux - ptr - 7));
			*(sd->device + (aux - ptr - 7)) = 0;
		}
	
	}	

	if (*(sd->droot) == 0 || *(sd->port) == 0)
	{
		write(STDERR_FILENO, "Error parsing configuration file\n\n", 34);	
		free (buffer);
		return -1;
	}

	free (buffer);
	return 0;

}
