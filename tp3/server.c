#include "mostronet.h"
#define REQUESTS 1000
int main(int argc, char ** argv){
	signal (SIGCHLD,SIG_IGN);
	int fd, sd, csd ,leido, opcion, confd=0, reqs = 0;
	char buffer[1024];
	char * path = malloc(sizeof(char) * 400);
	char * contentType = malloc(sizeof (char) * 40);
	struct sockaddr_in dir = {};
	struct sockaddr_in6 dir6 = {};
	int _mode;
	serverData config;

	_mode = mode("wlan0");
	printf("%d\n\n", _mode);

	while ((opcion = getopt(argc, argv, "d:hf:46") ) >= 0)
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
			case 'd':
				_mode = mode(optarg);
				
			case '4':
				if (_mode == IPV6)
				{
					printf("Options 4 & 6 called or the selected device only supports IPv6\n");
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
					printf("Options 4 & 6 called or the selected device only supports IPv4\n");
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
		if ((confd = open("tp3.conf", O_RDONLY)) == -1) 
		{
			perror("open");
			return -1;
		}
		if (setConfig(confd, &config) < 0 ){
			return -1;
		}
	}

//	if (fork())
//	{
		sd = socket(AF_INET,SOCK_STREAM,0);
		if (sd < 0){
			perror ("socket");
			return -1;
		}

		int opt=1;
		setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof (opt));
//		setsockopt(sd, IPPROTO_TCP, TCP_NODELAY , (char *) &opt, sizeof (opt));

		dir.sin_family = AF_INET;
		dir.sin_port = htons(atoi(config.port));
		printf("puerto: %d\nruta: %s\n\n", atoi(config.port), config.droot);
		inet_pton(AF_INET, "0.0.0.0", &dir.sin_addr);
		if (bind(sd , (struct sockaddr *)&dir , sizeof dir) < 0 ){
			perror ("bind");
			return -1;
		}
		listen(sd,10);
//	}
/*	else
	{
		sd = socket(AF_INET6, SOCK_STREAM);
		if (sd < 0){
			perror("socket");
			return -1;
		}

		int opt=1;
		setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof (opt));
//		setsockopt(sd, IPPROTO_TCP, TCP_NODELAY , (char *) &opt, sizeof (opt));

		dir6.sin6_family = AF_INET6;
		dir6.sin6_port = htons(atoi(config.port));
		printf("puerto: %d\nruta: %s\n\n", atoi(config.port), config.droot);
		inet_pton(AF_INET6, "::", &dir6.sin6_addr);
		if (bind(sd , (struct sockaddr *)&dir6 , sizeof dir6) < 0 ){
			perror ("bind");
			return -1;
		}
		listen(sd,10);

	}*/
	printf("Server running, waiting for clients\n");
	while ((csd = accept(sd,NULL,NULL)) > 0) {
		switch (fork()){
			case 0:
				printf("New incoming client\n");
				while ((leido = read (csd,buffer,1024)) > 0){
					buffer[leido]=0;
					memset(contentType, '\0', 40);
					parser(buffer, path, contentType);
					memset(buffer, '\0', 1024);
					fd = getFile(config, path, contentType, buffer);
					write(csd, buffer, strlen(buffer));
					printf("FD: %d\n",fd);
					if (fd > 0){
						while ((leido = read(fd, buffer, 1024)) > 0){
							write(csd, buffer, leido);
						}
						if (++reqs == REQUESTS){
							shutdown(csd, SHUT_RDWR);
							close(csd);
						}
						memset(buffer, '\0', 1024);
						memset(path, '\0', 400);
						memset(contentType, '\0', 40);
					}
					else {
						printf("ERROR:\n%s\n", buffer);
						shutdown (csd, SHUT_RDWR);
						close(csd);
					}
				}
				return 0; //del hijo    
		}
	}
	close (csd);
	return 0;
}

