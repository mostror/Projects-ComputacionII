#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "tp2.h"

int analize (int channel, int parentfd, int lenght, u_char buff[])
{
	int i;
	int count[256];
	u_char str[257];
	
	for (i = 0 ; i < 256 ; i++) 
	{
		count[i] = 0;
	}
	
	str[0] = channel - 1;
	
	for(i = channel - 1; i < lenght; i += 3)
	{
		count[ buff[i] ]++;
	}

	for(i = 0 ; i < 256 ; i++)
	{
		str[i+1] = (u_char) count[i];
	}

	write(parentfd, str, 257);

	return 0;
}

