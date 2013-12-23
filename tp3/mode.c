#include <net/if.h>
#include <sys/ioctl.h>
#include "mostronet.h"

int mode(char *device)
{
	int fd, leido;
	int _mode = NOTSET;
	struct ifreq ifr;
	char buffer[55];
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;

	strncpy(ifr.ifr_name, device, IFNAMSIZ-1);

	if(!ioctl(fd, SIOCGIFADDR, &ifr))
		_mode = IPV4;

	close(fd);
	
	if ((fd = open("/proc/net/if_inet6", O_RDONLY)) < 0)
	{
		perror("open");
		return -1;
	}
	
	while((leido = read (fd, buffer, 54)) > 0) 
	{
		buffer[leido] = 0;
		if(strstr(buffer, device))
		{
			if(_mode == IPV4)
				return FULLSUPPORT;
			
			return IPV6;
		}
	}
	return _mode;
}
