#include "imgpimping.h"

int openImg(image_t *img, int fd){
	int i,j, band=0;
	char * buffer = malloc (sizeof(char) * 50);
	if(read(fd, buffer, 50) < 0)
	{
		perror("read");
		return -1;
	}
	
	for(i = 0, j = 0 ; j < 4 && i < 40; i++)
	{
		if (buffer[i] == '\n')
		{
			j++;
			if (j==2 && !band){
				sscanf(&buffer[i+1],"%d %d", &img->width, &img->height);
				band++;
			}
		}
	}
	
if (i < 40) return i;
else return -1;
}
