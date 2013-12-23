#include "mostronet.h"


int main(int argc, char ** argv)
{
	signal (SIGCHLD,SIG_IGN);
	int sd, csd, opcion, confd=0;
	pthread_t pid;
	int _mode = FULLSUPPORT;
	char * msg;
	serverData config;
	int lereqs=0;
	while ((opcion = getopt(argc, argv, "d:hf:46") ) >= 0)
	{
		switch(opcion)
		{
			case 'h':
				write(STDOUT_FILENO, "Usage: hist [OPTION...]\n  -h, 			     show this help and exit\n  -f <configFile>,	     uses configFile to set socket parameters, if not used default config is used\n  -4 - 6	     	Force IPv4 or IPv6 functionality\n", 249);
				return 0;

				break;

			case 'f':
				if ((confd = open(optarg, O_RDONLY)) == -1)
				{
					perror("open");
					return -3;
				}
				if (setConfig(confd, &config) < 0)
					return -1;

				break;
				
			case '4':
				if (_mode == IPV6)
				{
					printf("Options 4 & 6 called, select only one\n");
					return -2;
				}
				else
				{
					_mode = IPV4;
				}
				break;

			case '6':
				if (_mode == IPV4)
				{
					printf("Options 4 & 6 called, select only one\n");
					return -2;
				}
				else
				{
					_mode = IPV6;
				}
				break;
			
			case '?':
				write(STDERR_FILENO, "Missing file configuration path, default used\n", 46);
				break;
		}
	}

	if (confd == 0){
		if ((confd = open("tp5.conf", O_RDONLY)) == -1) 
		{
			perror("open");
			printf("Conf FAIL!\n");
			return -1;
		}
		if (setConfig(confd, &config) < 0 ){
			return -1;
		}
	}

	if((sd = mode(config, _mode)) < 0)
	{
		return -1;
	}





	printf("Server running, waiting for clients\n");
	while ((csd = accept(sd,NULL,NULL)) > 0) {
		msg = safemalloc(sizeof (char) * 100);
		sprintf(msg, "%d %s", csd, config.droot);
		pthread_create(&pid, NULL, handle, (void *) msg);
		lereqs++;
		printf("new Client, request number:%d\n", lereqs);
	}
	return 0;
}


