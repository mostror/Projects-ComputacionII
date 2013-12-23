#include <stdlib.h>
#include <unistd.h>

void * safemalloc(size_t size){
	void * var;
	while ((var = malloc(size) ) == NULL){
		write(STDERR_FILENO, "malloc failed, reattempting in half second...\n", 46);
		usleep(500);
	}
	return var;
}
