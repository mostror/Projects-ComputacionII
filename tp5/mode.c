#include <net/if.h>
#include <sys/ioctl.h>
#include "mostronet.h"

int mode(serverData *config, int _mode)
{
	if(*(config->device) == 0)
	{
		sprintf(config->device, "wlan0");
	}

	int fd, leido, sd, id = -1;
	int aux = NOTSET, port;
	struct ifreq ifr;
	char buffer[55];
	struct sockaddr_in dir = {};
	struct sockaddr_in6 dir6 = {};
	if (_mode != IPV6)
	{
		fd = socket(AF_INET, SOCK_DGRAM, 0);
		ifr.ifr_addr.sa_family = AF_INET;
	
		strncpy(ifr.ifr_name, config->device, IFNAMSIZ-1);
	
		if(!ioctl(fd, SIOCGIFADDR, &ifr))
			aux = IPV4;

		close(fd);	

	}
	if (_mode != IPV4)
	{
		if ((fd = open("/proc/net/if_inet6", O_RDONLY)) < 0)
		{
			perror("open");
			return -1;
		}
	
		while((leido = read (fd, buffer, 54)) > 0) 
		{
			buffer[leido] = 0;
			if(strstr(buffer, config->device))
			{
				if(aux == IPV4)
					aux = FULLSUPPORT;
				else
					aux =IPV6;
			}
		}
	}	
	if (aux == FULLSUPPORT && _mode == FULLSUPPORT)
		id = fork();

	if (id == 0)
		config->mode = 6;
	if (id > 0)
		config->mode = 4;


	if((_mode == IPV4 || id > 0) && (aux != IPV6))
	{
		printf ("IPV4\n");
		sd = socket(AF_INET,SOCK_STREAM,0);
		if (sd < 0)
		{
			perror ("socket");
			return -1;
		}
			int opt=1;
		setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof (opt));
		dir.sin_family = AF_INET;
		dir.sin_port = htons(atoi(config->port));

		inet_pton(AF_INET, "0.0.0.0", &dir.sin_addr);
		if (bind(sd , (struct sockaddr *)&dir , sizeof dir) < 0 )
		{
			perror ("bind");
			return -1;
		}
		if(listen(sd,10) < 0)
		{
			perror("listen");
			return -1;
		}
		return sd;
	}
	printf ("mode = %d\naux = %d\n", _mode, aux);

	if((_mode == IPV6 || id == 0) && (aux != IPV4))
	{
		printf ("IPV6\n");
		sd = socket(AF_INET6, SOCK_STREAM, 0);

		if (sd < 0)
		{
			printf("me\n");
			perror("socket");
			return -1;
		}

		int opt=1;
		setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof (opt));

		dir6.sin6_family = AF_INET6;
		port = (_mode != FULLSUPPORT ? atoi(config->port) : (atoi(config->port))+ 10);
		printf("port IPV6: %d\n", port);
		dir6.sin6_port = htons(port);
		inet_pton(AF_INET6, "::", &dir6.sin6_addr);
		if (bind(sd , (struct sockaddr *)&dir6 , sizeof dir6) < 0 )
		{
			printf("me\n");
			perror ("bind");
			return -1;
		}
		listen(sd,10);
		return sd;
	}
	write(STDERR_FILENO, "Can setup socket, check device setup\n",36);
	return -1;

}
