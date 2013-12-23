#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main (int argc, char **argv)
{
	int sd, leido, escrito;
	char buffer[128];
	struct sockaddr_in dir = {};
	
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0)
	{
		perror ("socket");
		return -1;
	}
	dir.sin_family = AF_INET;
	dir.sin_port = htons(2500);

	if (!inet_aton (argv[1] , &dir.sin_addr))
	{
		printf ("dirreccion invalida\n");
		return -1;
	}
	


	if (connect (sd, (struct sockaddr *)&dir,sizeof dir) < 0)
	{
		perror("connect");
		return -1;
	}
	if (fork())
	{
		while ((leido = read (STDIN_FILENO, buffer, sizeof (buffer))) > 0)
		{
			buffer[leido] = 0;
			if ((escrito = write(sd, buffer, sizeof(buffer))) <= 0)
			{
				perror("write");
				printf("ENTRE!!!!!!");
				break;
			}
		}
		if (leido < 0)
			perror("read");
		return 0;
	}
	while((leido = read(sd, buffer, sizeof(buffer))) > 0)
	{
		buffer[leido] = 0;
		printf("Server says: %s", buffer);
	}
	printf("salio");
	return 0;


}



