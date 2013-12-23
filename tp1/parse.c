//#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include <assert.h>
#include <string.h>

int parse (char *buf, char *address)
{
	//Checkeo los parametros
	if (buf == NULL)
		return -1;
	char *str = NULL; //Guarda las substrings de buff
	char *buffer= (char *) malloc(sizeof(char)*strlen(buf));
	regex_t pattern;
	//Asignacion de memoria e inicialiazacion
	str = (char *) malloc(sizeof(char) * strlen(buf));

	strncpy(buffer,buf,strlen(buf));
	
	if (regcomp(&pattern, "^DST=(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$", REG_NOSUB | REG_EXTENDED))
	{
		write (STDERR_FILENO, "Ocurrio un error al armar la expresion regular\n", 48);
		return -2;
	}
	//Loop dividiendo el string y buscando IPs
	for (str = strtok_r(buffer, " ", &buffer); str != NULL; str = strtok_r(NULL, " ", &buffer))
	{

		if (!regexec(&pattern, str, 0, NULL, 0))
		{

			if (address == NULL)
				address = (char *) malloc (sizeof(char)*17);
			strncpy (address,str+4,strlen(str)-4);
			*(address+strlen(str)-3)=0;
			//if (str != NULL) free(str);
			//if (buffer != NULL) free(buffer);
			return 1;
		}
	}
	
	//if (buffer != NULL) free(buffer);

	return 0;
}

