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
	sem_unlink("/semaph");
	int fd = shm_open("/archivo", O_CREAT | O_RDWR, 0666);
        //ftruncate(fd, sizeof (int));
        int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        *ptr = 0;
	if(fork() == 0)
	{
		system("./smless");
		return 0;
	}



	sem_t *sem = sem_open("/semaph", O_CREAT, 0666, 1);

	int i= 0;
	for(i=0; i < 2000000; i++)
	{
		sem_wait(sem);
		(*ptr)++;
		sem_post(sem);
	}
	wait (NULL);
	sem_close(sem);
	printf ("Final: %d\n", *ptr);
	return 0;
}

