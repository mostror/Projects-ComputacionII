#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>


int main (int argc, char const *argv[])
{
	int *ptr = mmap(NULL, sizeof(int) + sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
	*ptr = 0;

	sem_t *sem = (sem_t *)ptr + 1;


	if(sem_init(sem, -1, 1) == -1)
	{
		perror("sem_init");
		return -1;
	}

	if(fork() == 0)
	{
		int i= 0;
		for(i=0; i < 2000000; i++)
		{
			sem_wait(sem);
			(*ptr)++;
			sem_post(sem);
		}
		return 0;
	}
	int i = 0;
	for(i=0; i<2000000; i++)
	{
		sem_wait(sem);
		(*ptr)--;
		sem_post(sem);
	}
	wait(NULL);//Serializacion
	printf ("%d\n", *ptr);
	return 0;



}
