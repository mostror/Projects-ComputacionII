#include "mostronet.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int parser(char *request, char *path, char *contentType)
{
	if (request == NULL)
	{
		write(STDERR_FILENO, "Error: invalid pointer reference\n\n", 33);
		return -1;
	}

	char *ptr = (char *)malloc(sizeof(char) *1024);
	char *aux = (char *)malloc(sizeof(char) *1024);
		
	if((ptr = strstr(request, "GET ")))
	{
		if((aux = strchr(ptr + 4, ' ')))
		{
			path = (char *) malloc(sizeof(char) * (aux - ptr - 3));
			strncpy(path, ptr + 4, aux - ptr - 4);
			*(path + (aux - path - 3)) = 0;
		}
	}
	printf ("%s\n", path);

	if(path == NULL || (aux = strchr(path, '.')))
	{
		contentType = (char *) malloc(sizeof(char) * 32);

		switch(*(aux + 1))
		{
		case 'h':
			snprintf(contentType, 10 ,"text/html");
			break;
	
		case 'j':
			snprintf(contentType, 9,"image/jpg");
			break;

		case 't':
			snprintf(contentType, 11, "text/plain");

		case 'p':
			snprintf(contentType, 16, "application/pdf");
			break;
		default:
			free (aux);
			free (ptr);
			free (contentType);
			contentType = NULL;
				
			return 0;
		}

		free (aux);
		free (ptr);

		return 0;
	}

	free(aux);
	free(ptr);

	return -1;
	
}



