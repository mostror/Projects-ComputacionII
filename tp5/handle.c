
#include "mostronet.h"


void *handle(void *arg)
{
	serverData *config = (serverData *)arg;
	char buffer[2048];
	int csd = -1, leido, fd;
	char contentType[40], path[256];
	
	while(config->goOn == 1)
	{
		pthread_mutex_lock(&(config->mutex2));
		csd = config->mqd;
		(config->requests)++;
		printf("number of requests in the IPv%d family= %d\n", config->mode, config->requests);
		pthread_mutex_unlock(&(config->mutex));
		
		if((leido = read (csd,buffer,2048)) > 0)
		{
			buffer[leido]=0;
			memset(contentType, '\0', 40);
			parser(buffer, path, contentType);
			memset(buffer, '\0', 2048);
			fd = getFile(config, path, contentType, buffer);
			
			write(csd, buffer, strlen(buffer));
			if (fd > 0)
			{
				while ((leido = read(fd, buffer, 2048)) > 0)
				{
					write(csd, buffer, leido);
				}

				memset(buffer, '\0', 2048);
				memset(path, '\0', 256);
				memset(contentType, '\0', 40);
			}
			else 
				if(fd != -99)
				{
					printf("ERROR:\n%s\n", buffer);
					shutdown (csd, SHUT_RDWR);
				}
			if(fd != -1 && fd != -99)
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
	}
	
	return NULL;
}
