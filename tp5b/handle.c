#include "mostronet.h"


void *handle(void *arg)
{
	char *msg = (char *) arg;
	char buffer[1024];
	int csd = -1, leido, fd;
	char contentType[40], path[256];
	char * droot = safemalloc (sizeof (char) * 95);
	
	sscanf (msg, "%d %s", &csd, droot);
	while ((leido = read (csd,buffer,1024)) > 0)
	{
		buffer[leido]=0;
		memset(contentType, '\0', 40);
		parser(buffer, path, contentType);
		memset(buffer, '\0', 1024);
		fd = getFile(droot, path, contentType, buffer);
		
			
			
		write(csd, buffer, strlen(buffer));
		if (fd > 0 || fd == -99)
		{
			printf("FD: %d\n",fd);
			while ((leido = read(fd, buffer, 1024)) > 0)
			{
				write(csd, buffer, leido);
			}
		/*	if (++reqs == REQUESTS)
			{
				shutdown(csd, SHUT_RDWR);
				close(csd);
			}*/
			memset(buffer, '\0', 1024);
			memset(path, '\0', 256);
			memset(contentType, '\0', 40);
		}
		else 
		{
			printf("ERROR:\n%s\n", buffer);
			shutdown (csd, SHUT_RDWR);
		
		}
		if(fd != -1)
		{
			if (close(fd) < 0)
			{
				perror("closeFD");
				return NULL;
			}
		}
		if(close(csd) < 0)
		{
			perror("closeCSD");
			return NULL;
		}
	}
	free(droot);
	free(msg);
	printf("client served\n");
	return NULL;
}
