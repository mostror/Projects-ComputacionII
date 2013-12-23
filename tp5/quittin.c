
#include "mostronet.h"


void *quittin(void *arg)
{
	char buff[5];
	while( (read(STDIN_FILENO, buff, 5)) > 0)
		if (strstr(buff,":wq") != NULL)
			system("killall server");
	return NULL;
	
}
