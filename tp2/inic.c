#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "tp2.h"
int inic (int red[], int green[], int blue[], int ret[], int channel){

	int id;
	switch(channel)
	{
		case 0:
		case RED:
			if (pipe(red)){
				perror("pipe");
				return -1;
			}
			if (channel == RED) break;
		case GREEN:
			if (pipe(green)){
				perror("pipe");
				return -1;
			}
			if (channel == GREEN) break;
		case BLUE:
			if (pipe(blue)){
				perror("pipe");
				return -1;
			}
			if (channel == BLUE) break;
	}

	if (pipe(ret))
	{
		perror("pipe");
		return -1;
	}
	

	if (channel == RED || channel == NOSET)
	{
		id = fork();
		if (id == 0) 
		{
			if (channel == NOSET)
			{
				close(red[1]);
				close(green[0]);
				close(green[1]);
				close(blue[0]);
				close(blue[1]);
				close(ret[0]);
			}
			else
			{
				close(red[1]);
				close(ret[0]);
			}
			return RED;
		}
		else 
		{
			if (id == -1)
			{
				perror("fork");
				return -1;
			}
		}
	}


	if (channel == GREEN || channel == NOSET)
	{
		id = fork();
		if (id == 0) 
		{
			if (channel == NOSET)
			{
				close(red[0]); 
				close(red[1]); 
				close(green[1]); 
				close(blue[0]); 
				close(blue[1]); 
				close(ret[0]);
			}
			else
			{
				close (green[1]); 
				close (ret[0]);
			}
			return GREEN;
		}
		else 
			if (id == -1)
			{
				perror("fork");
				return -1;
			}
	}

	if (channel == BLUE || channel == NOSET)
	{
		id = fork();
		if (id == 0) 
		{
			if (channel == NOSET)
			{
				close(red[0]); 
				close(red[1]); 
				close(green[0]); 
				close(green[1]); 
				close(blue[1]); 
				close(ret[0]);
			}
			else
			{
				close (blue[1]); 
				close (ret[0]);
			}
			return BLUE;
		}
		else
		{ 
			if (id == -1)
			{
				perror("fork");
				return -1;
			}
		}
	}
	switch(channel){
		case NOSET: 
			close(red[0]); 
			close(green[0]); 
			close(blue[0]); 
			break;

		case RED: 
			close(red[0]); 
			break;

		case GREEN: 
			close(green[0]); 
			break;
		case BLUE: 
			close(blue[0]); 
			break;
	}

	close(ret[1]);
	return 0;
}
