#include "mostronet.h"
int getFile(serverData sd, char *path, char *contentType, char *header){
	int fd=0;
	struct stat buf;
	char * filepath = NULL;

	filepath = malloc(sizeof(char) * (strlen(path) + strlen(sd.droot)));
	if (header == NULL) header = malloc(sizeof (char) * 1024);

	strcpy(filepath, sd.droot);
	if (filepath[ strlen(filepath) - 1 ] == '/')
		filepath[ strlen(filepath) -1 ] = '\0';
	strcat(filepath,path);

	fd = open(filepath, O_RDONLY);

	printf("FD: %d\n",fd);
	if (fd < 0){
		strcpy(header, "HTTP/1.1 404 Not Found\n");
		return -1;
		}
	else if (contentType == NULL){
		strcpy(header, "HTTP/1.1 500 Internal Server Error\n");
		return -2;
		}

	stat(filepath, &buf);
	sprintf(header, "HTTP/1.1 200 OOK\nDate: %sContent-Type: %s\nContent-Length: %d\n\n", ctime(&(buf.st_mtime)), contentType, (int) buf.st_size);

	
	return fd;
}
