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
	char *aux;

	if(*(request + 5) == ' ')
	{
		sprintf(path, "/index.html");
		sprintf(contentType, "text/html");
		return 0;
	}
	if((ptr = strstr(request, "GET ")))
	{
		if((aux = strchr(ptr + 4, ' ')))
		{
			strncpy(path, ptr + 4, aux - ptr - 4);
			*(path + (aux - path - 3)) = 0;
		}
	}

	if(path == NULL || (aux = strchr(path, '.')))
	{
		aux++;

		if(strstr(aux, "html"))
			snprintf(contentType, 10 ,"text/html");
		
		else if(strstr(aux, "jpg"))
			snprintf(contentType, 11,"image/jpeg");
		
		else if(strstr(aux, "txt"))
			snprintf(contentType, 11, "text/plain");

		else if(strstr(aux, "pdf"))
			snprintf(contentType, 16, "application/pdf");
		
		else
		{		
			free (contentType);
			contentType = NULL;
			return 0;
		}

		return 0;
	}

	return -1;
	
}



