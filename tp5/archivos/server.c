#include "mostronet.h"
#define REQUESTS 5
int main(int argc, char ** argv){
	signal (SIGCHLD,SIG_IGN);
	int sd, csd ,leido, opcion, confd=0, reqs = 0, flag4=0, flag6=0;
	char buffer[1000];
	struct sockaddr_in dir = {};
	serverData config;


	while ((opcion = getopt(argc, argv, "hf:46") ) >= 0)
	{
		switch(opcion)
		{
			case 'h':
				write(STDOUT_FILENO, "Usage: hist [OPTION...]\n  -h, 			     show this help and exit\n  -a <ppm-file>,	     use ppm-file file\n  -c <rojo/verde/azul>,	     show only the result for selected colour\n  -o <file-name>,	     create a file named file-name and stores the results\n", 249);
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
				if (flag6 == 1)
				{
					printf("Options 4 & 6 called. Please, call only one, or none\n");
					return -2;
				}
				else
				{
					flag4=1;
				}
				break;

			case '6':
				if (flag4 == 1)
				{
					printf("Options 4 & 6 called. Please, call only one, or none\n");
					return -2;
				}
				else
				{
					flag6=1;
				}
				break;

			case '?':
				write(STDERR_FILENO, "Missing file configuration path, default used\n", 46);
				break;
		}
	}






	if (confd == 0){
		if ((confd = open("tp3.conf", O_RDONLY)) == -1) 
		{
			perror("open");
			return -1;
		}
		if (setConfig(confd, &config) < 0 )
			return -1;
	}

	sd = socket(AF_INET,SOCK_STREAM,0);
	if (sd < 0){
		perror ("socket");
		return -1;
	}
	dir.sin_family = AF_INET;
	dir.sin_port = htons(atoi(config.port));
	printf("puerto: %d\nruta: %s", atoi(config.port), config.droot);
	inet_pton(AF_INET, "0.0.0.0", &dir.sin_addr);
	if (bind(sd , (struct sockaddr *)&dir , sizeof dir) < 0 ){
		perror ("bind");
		return -1;
	}
	listen(sd,10);
	printf("Server running, waiting for clients\n");
	while ((csd = accept(sd,NULL,NULL)) > 0) {
		switch (fork()){
			case 0:
				printf("New incoming client\n");
				while ((leido = read (csd,buffer,sizeof buffer)) > 0){
					buffer[leido]=0;
					printf ("leido %s\n",buffer);
					write(csd,buffer,leido);
					if (++reqs == REQUESTS) {
						write(csd, "Requests depleted\n", 5 );
						shutdown(csd, SHUT_RDWR);
					}
				}
				return 0; //del hijo    
		}
	}
	close (csd);
	return 0;
}

