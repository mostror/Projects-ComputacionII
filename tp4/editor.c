#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ppm.h"
#include "filter.h"
#include "imgpimping.h"

int main (int argc, char * argv[]){
	sem_unlink("/lesem");
	int mqd, ch, *var;
	struct mq_attr atrr;
	char* msg, *aux, *aux2;
	u_char *ptr, *ptr2;
	image_t *img = malloc(sizeof (image_t));
	image_t *img2 = malloc(sizeof (image_t));
	sem_t *sem, *sem2;
	int imgdesc, offset, des;
	struct stat st;
	filter_t filt;

	
	if ((mqd = mq_open ("/lequeue", O_RDWR | O_CREAT, 0666, NULL)) < 0)
	{
		perror("mq_open");
		return -1;
	}

	
	if ((sem = sem_open("/lesem", O_CREAT, 0666, 0)) == SEM_FAILED)
	{
		perror("sem_open");
		return -1;
	}
	if ((sem2 = sem_open("/lesem2", O_CREAT, 0666, 1)) == SEM_FAILED)
	{
		perror("sem_open");
		return -1;
	}
	des = shm_open("/var", O_RDWR | O_CREAT, 0666);
	ftruncate(des, sizeof(int));
	if ((var = mmap (NULL, sizeof (int), PROT_READ | PROT_WRITE, MAP_SHARED, des, 0 )) == (void* ) -1)
	{
		perror("mmap");
		return -1;
	}
	*var = 0;

	mq_getattr(mqd, &atrr);
	msg = malloc (atrr.mq_msgsize);

	for (; atrr.mq_curmsgs > 0; mq_getattr(mqd, &atrr))
	{
		if (mq_receive(mqd, msg, atrr.mq_msgsize, 0) < 0){
			perror("mq_receive");
			return 0;
		}
		imgdesc = open(msg, O_RDONLY );
		if (imgdesc < 1)
		{
			sprintf(msg, "Couldn't open file %s\n",msg);
			write(STDERR_FILENO, msg, strlen(msg));
			break;
		}

		if(fstat(imgdesc, &st))
		{
			perror("stat");
			return -1;
		}
		ptr2 =  malloc(sizeof(u_char) * st.st_size);
		if(read(imgdesc, ptr2, st.st_size) < 0)
		{
			perror("read");
			return -1;
		}
		close(imgdesc);

		aux=msg+strlen(msg)+1;
		aux2=malloc(sizeof(char)*(strlen(aux)+strlen(msg)+4));
		sprintf(aux2,"cp %s %s", msg, aux);
		system(aux2);
		if((imgdesc = open(aux, O_RDWR)) < 0)
		{
			perror("open");
			return -1;
		}
		free(aux2);
		aux+=strlen(aux)+1;
		filt.type = (mask_type_t) atoi(aux);
		
		if((offset = openImg(img, imgdesc)) < 0)
			return -1;
		
		if((ptr = mmap(NULL, st.st_size, PROT_EXEC | PROT_WRITE | PROT_READ, MAP_SHARED, imgdesc, 0)) == (void *) -1)
		{
			perror("mmap");
			return -1;
		}
		
		filt.new_img = img;
		filt.img = img2; //arreglar y poner malloc
		filt.img->height = filt.new_img->height;
		filt.img->width = filt.new_img->width;
		filt.img->data = (ptr2 + offset);
		filt.new_img->data = (ptr + offset);
		
		switch(ch=inic())
		{
		case 1: 
		case 2:
		case 3: 
			filt.channel=ch-1;
				if(filter_exec(&filt))
				{
					write(STDERR_FILENO, "Error in child\n", 15);
					return -1;
				}
			if (*var==2){
				sem_post(sem);
			}
			else
			{
				sem_wait(sem2);
				(*var)++;
				sem_post(sem2);
			}
			return 0;
		case 0:
			sem_wait(sem);
			write(STDOUT_FILENO, "Image done!\n",12);
			munmap(ptr, st.st_size);
			break;
		}
	break;//se saca cuando armamos el for
	}
	//trabar hasta que reciba
	sem_close(sem);
	return 0;

}

