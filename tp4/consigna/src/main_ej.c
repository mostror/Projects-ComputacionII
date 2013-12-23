/*
 * J. Federico Hernandez <federico.hernandez@um.edu.ar>
 *
 * Uso: 
 * 		./filter -i <input> -o <output> -f <filtro> 
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ppm.h"
#include "error.h"
#include "filter.h"
#include "cmd_line_parse.h"

int proc(filter_t *filter, int channel);

int main(int argc, char **argv) {
	filter_t filter;
	image_t *img_input=NULL, *img_output=NULL;
	cmd_line_arg_t cmd_arg;
	int height=0, width=0;
	int i;

	cmd_line_init(&cmd_arg);
	if (cmd_line_parse(&cmd_arg, argc, argv) != 0)
		return 1;

	/*
	img_input = image_read()

	height = image_get_height();
	width  = image_get_width();

	img_output = image_create();

	filter.type = cmd_arg.type;

	for (i = 0; i < 3; i++) {
		switch (fork()) {
		case 0: // hijo
			proc(&filter, i);
			return 0;
		case -1: // error
			break;
		}
	}

	while ( -- espero a todos los hijos -- )

	//image_write(img_output, cmd_arg.file_out);
	*/

	image_destroy(img_output);	
	image_destroy(img_input);

	return 0;
}

int proc(filter_t *filter, int channel) {
	filter->channel = channel;
	
	printf("channel: %d\n", filter->channel);
	printf("type: %d\n", filter->type);

	filter_exec(filter);

	return 0;
}
