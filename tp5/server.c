#include "mostronet.h"


int main(int argc, char ** argv)
{
	signal (SIGCHLD,SIG_IGN);
	int sd, csd, opcion, confd=0, j = 0, i = 0;
	pthread_t pid[11];
	int _mode = FULLSUPPORT;
	serverData config;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	config.mutex = mutex;
	config.mutex2 = mutex;
	config.requests = 0;	
	while ((opcion = getopt(argc, argv, "d:hf:46") ) >= 0)
	{
		switch(opcion)
		{
			case 'h':
				printf("Programers: Y U NO HELP!\n");
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

	if((sd = mode(&config, _mode)) < 0)
	{
		return -1;
	}

	pthread_mutex_lock(&(config.mutex2));
	config.goOn = 1;

	for(i = 0; i < 10; i++)
	{	
		if(pthread_create(&pid[i], NULL, handle, (void *) &config) < 0)
		{
			perror("pthread_create");
			i--;
			j++;
			
			if(j == 3)
			{
				printf("thread_pool error");
				return -1;
			}
		}
	} 

	pthread_create(&pid[11], NULL, quittin, NULL);

	printf("Server running in IPv%d, waiting for clients\n", config.mode);

	while ((csd = accept(sd,NULL,NULL)) > 0) {
		pthread_mutex_lock(&(config.mutex));
		config.mqd = csd;
		pthread_mutex_unlock(&(config.mutex2));
	}
	return 0;
}

