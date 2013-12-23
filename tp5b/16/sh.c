#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

//SI ponemos valores muy grandes el valor no termina en 0, es un problema de sincornizacion. 


int main (int argc, char const *argv[])
{
	int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
	*ptr = 0;

	if(fork() == 0)
	{
		int i= 0;
		for(i=0; i < 100; i++)
		{
			(*ptr)++;
		}
		printf("hijo %d\n", *ptr);
		return 0;
	}
	int i = 0;
	for(i=0; i<100; i++)
	{
		(*ptr)--;
	}
	wait(NULL);//Serializacion
	printf ("%d\n", *ptr);
	return 0;



}
