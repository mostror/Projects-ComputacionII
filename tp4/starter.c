#include "imgpimping.h"
int main (int argc, char** argv)
{
	int args=0, mqd = 0, f = -1;
	char opcion;
	char * in, *o, *msg;

	while ((opcion = getopt(argc, argv, "i:o:f:hu") ) >= 0)
	{
		switch(opcion)
		{
			case 'h':
				write(STDOUT_FILENO, "Usage: hist [OPTION...]\n  -h, 			     show this help and exit\n  -a <ppm-file>,	     use ppm-file file\n  -c <rojo/verde/azul>,	     show only the result for selected colour\n  -o <file-name>,	     create a file named file-name and stores the results\n", 249);
				return 0;

				break;

			case 'i':
				args++;
				in=malloc(sizeof (char) * strlen(optarg));
				strcpy (in, optarg); 
				break;
			

			case 'o':
				args++;
				o=malloc(sizeof (char) * strlen(optarg));
				strcpy (o, optarg); 
				break;
			

			case 'f':
				args++;
				if(strcmp(optarg, "lowpass") == 0)
					f = 0;
				if(strcmp(optarg, "weigmean") == 0)
					f = 1;
				if(strcmp(optarg, "laplace") == 0)
					f = 2;
				if(f == -1)
				{
					write(STDERR_FILENO, "Bad filter option!\n", 20);
					return -1;
				}	
				break;
			case 'u':
				mq_unlink("/lequeu");
				write(STDOUT_FILENO,"Message queue unlinked\n",23);
				return 0;
				break;
	

			case '?':
				write(STDERR_FILENO, "Y U NOT PUT ARGUMENTS?! To learn the propper use of this tool, please use the option -h\n", 88);
				return -1;
				break;
		}
	}
	

	if(args < 3)
	{
		write(STDERR_FILENO, "Y U NO PUT OPTIONS?\nTo learn the propper use of this tool, please use the option -h\n",86 );
		return -1;
	}

	msg=malloc(sizeof(char)*(strlen(in) + strlen(o) + 4));
	sprintf(msg, "%s%c%s%c%d",in,'\0',o,'\0', f);

	printf("%s\n", msg+strlen(msg)+1);
	if((mqd = mq_open ("/lequeue", O_RDWR | O_CREAT, 0666, NULL)) < 0 )
	{
		perror("mq_open");
		return -1; 
	}

	if (mq_send(mqd, msg, strlen(in) + strlen(o) + 4, 1) < 0)
	{
		perror("mq_send");
		return -1;
	}
	printf("sent!\n");
	return 0;
}

