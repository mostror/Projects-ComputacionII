#include "imgpimping.h"

int openImg(image_t *img, int fd){
	int i,j, w, h, band=0;
	char * buffer = malloc (sizeof(char) * 50);
	if(read(fd, buffer, 50) < 0)
	{
		perror("read");
		return -1;
	}
	printf("buffer:\n\n%s\n\nfinbuffer\n",buffer);
	
		printf("Y U NO WORK1!\n");	
	for(i = 0, j = 0 ; j < 4 && i < 40; i++)
	{
		if (buffer[i] == '\n')
		{
			j++;
			if (j==2)
				w=i+1;
		}
		if (j==2)
		{
			if (buffer[i] ==' '){
				h = i+1;
				buffer[i] = '\0';
				img->width=atoi(buffer +w );
			}
			if (buffer[i] =='\n'){
				buffer[i] = '\0';
				img->height=atoi(buffer + h);
			}

		}
		printf("Y U NO WORK3!\n");	
	}
	
printf("width->%d\nheight->%d\n",img->width, img->height);
if (i < 40) return i;
else return -1;
}
