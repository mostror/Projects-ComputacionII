#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "tp2.h"

int main (int argc, char * const argv[])
{

	int red[2], green[2], blue[2], ret[2];
	int id, leido, opcion, channel = NOSET; 
	int inputfd = STDIN_FILENO, outfd = STDOUT_FILENO;
	u_char buff[768] = {};
	char * inputfile = NULL;
	char * outputfile = NULL;
	char * header = NULL;
	
	extern char * optarg;
	extern int optind, opterr, optopt;

	while ((opcion = getopt(argc, argv, "hc:a:o:") ) >= 0)
	{
		switch(opcion)
		{
			case 'h':
				write(STDOUT_FILENO, "Usage: hist [OPTION...]\n  -h, 			     show this help and exit\n  -a <ppm-file>,	     use ppm-file file\n  -c <rojo/verde/azul>,	     show only the result for selected colour\n  -o <file-name>,	     create a file named file-name and stores the results\n", 249);
				return 0;

				break;

			case 'o':
				if ((outfd = open(optarg, O_CREAT | O_RDWR | O_TRUNC, 0640)) == -1)
				{
					perror("open");
					return -3;
				}

				outputfile = malloc(sizeof (char) * strlen(optarg));
				strcpy(outputfile, optarg);

				break;

			case 'a':
				if ((inputfd = open(optarg, O_RDONLY)) == -1)
				{
					perror("open");
					return -3;
				}

				inputfd = setpointer(inputfd, optarg);
				inputfile = malloc(sizeof (char) * strlen(optarg));
				strcpy(inputfile, optarg);

				break;

			case 'c':
				if (!strcmp("rojo", optarg)) 
					channel = RED;
				else if (!strcmp("verde", optarg)) 
					channel = GREEN;
				else if (!strcmp("azul", optarg)) 
					channel = BLUE;
				else
				{
					write(STDERR_FILENO, "Wrong argument\n", 15);
					return -3;
				}
				break;

			case '?':
				write(STDERR_FILENO, "Missing argument\n", 17);
				return -2;
				
				break;
		}
	}



	if (inputfd == STDIN_FILENO)
	{
		write(STDERR_FILENO, "No input file\n", 14);
		return -4;
	}

	header = malloc(sizeof (char) * 256);

	sprintf(header, "File read: %s\n", inputfile);
	if (outputfile != NULL)
		sprintf(header, "%sOutput file: %s\n", header, outputfile);
	if (channel == NOSET)
		sprintf(header, "%sChannels analized: all\n\n", header);
	if (channel == RED)
		sprintf(header, "%sChannel analized: red\n\n", header);
	if (channel == GREEN)
		sprintf(header, "%sChannel analized: green\n\n", header);
	if (channel == BLUE)
		sprintf(header, "%sChannel analized: blue\n\n", header);

	write(outfd,header,strlen(header));
	id = inic(red, green, blue, ret, channel);

	switch(id)
	{
		case RED: 
			while ((leido = read(red[0], buff, 768)) > 0)
			{
				if (analize(RED, ret[1], leido, buff) != 0)
					return -1;
			}

			return 0;
			break;

		case GREEN: 
			while ((leido = read(green[0], buff, 768)) > 0)
			{
				if (analize(GREEN, ret[1], leido, buff) != 0)
					return -1;
			}

			return 0;
			break;

		case BLUE: 
			while ((leido = read(blue[0], buff, 768)) > 0)
			{
				if (analize(BLUE, ret[1], leido, buff) != 0)
					return -1;
			}

			return 0;
			break;

	}

	u_char r[257];
	char outTxt[50];
	int pix[3][256], i, j, status;


	for(i = 0 ; i < 3  ; i++  )
		for(j = 0; j < 256; j++ )
			pix[i][j] = 0;

	while ((leido = read(inputfd, buff, 768)) > 0)
	{
		if (leido == -1)
		{
			perror("read");
			return -9;
		}
		if (channel == RED || channel == NOSET) 
			write(red[1], buff, leido); 
		if (channel == GREEN || channel == NOSET) 
			write(green[1], buff, leido);
		if (channel == BLUE || channel == NOSET) 
			write(blue[1], buff, leido);

		for(j = 0 ; j < 3 ; j++)
		{
			if (channel != NOSET) j += 2;
			leido = read(ret[0],r,257);
			for (i=1; i<257; i++)
			{
				pix [ r[0] ] [i-1] += (int) r[i];
			}
		}
	}


	if (channel == RED || channel == NOSET) 
		close(red[1]);
	if (channel == GREEN || channel == NOSET) 
		close(green[1]);
	if (channel == BLUE || channel == NOSET) 
		close(blue[1]);

	for(i = 0 ; i < 2 ; i++  )
	{
		if (channel != NOSET) 
			i += 2;

		wait(&status);

		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) != 0)
				write(STDERR_FILENO, "Child ended with error\n", 23);
		}
		else
			write(STDERR_FILENO, "Child ended signalized\n", 17);
	}

	switch (channel)
	{
		case NOSET:
			for (i = 0 ; i < 256 ; i++) 
			{
				sprintf(outTxt, "red[%3d]=%d\t\tgreen[%3d]=%d\t\tblue[%3d]=%d\n", i, pix[0][i], i, pix[1][i], i, pix[2][i]);
				write(outfd, outTxt, strlen(outTxt));
			}
			break;
		case RED:
			for (i = 0 ; i < 256 ; i++) 
			{
				sprintf(outTxt, "red[%3d]=%d\n", i, pix[0][i]);
				write(outfd, outTxt, strlen(outTxt));
			}
			break;
		case GREEN:
			for (i = 0 ; i < 256 ; i++) 
			{
				sprintf(outTxt, "green[%3d]=%d\n", i, pix[1][i]);
				write(outfd, outTxt, strlen(outTxt));
			}
			break;
		case BLUE:
			for (i = 0 ; i < 256 ; i++) 
			{
				sprintf(outTxt, "blue[%3d]=%d\n", i, pix[2][i]);
				write(outfd, outTxt, strlen(outTxt));
			}
			break;
	
			
	}

	return 0;
}

