#include "mostronet.h"
#include <errno.h>

int getFile(char * droot, char *path, char *contentType, char *header)
{
	/*if (strcmp(path, "stats/") == 0)
	{
		
		sprintf(header, "HTTP/1.1 200 OK\n\nNumber of request accepted: %d", config->requests);
		return -99;
	}*/
	int fd=0;
	struct stat buf;
	char * filepath = NULL;
	extern int errno;

	filepath = malloc(sizeof(char) * (strlen(path) + strlen(droot)));

	strcpy(filepath, droot);
	if (filepath[ strlen(filepath) - 1 ] == '/')
		filepath[ strlen(filepath) -1 ] = '\0';

	strcat(filepath,path);

	if(stat(filepath, &buf) < 0)
	{
		if (errno == ENOENT)
			strcpy(header, "HTTP/1.1 404 Not Found\n");
	}

	fd = open(filepath, O_RDONLY);

	if (fd < 0)
	{
		if(errno == EACCES)
			strcpy(header, "HTTP/1.1 500 Internal Server Error\n");
		return -1;
	}

	sprintf(header, "HTTP/1.1 200 OK\nDate: %sContent-Type: %s\nContent-Length: %d\n\n", ctime(&(buf.st_mtime)), contentType, (int) buf.st_size);

	free(filepath);
	
	return fd;
}
