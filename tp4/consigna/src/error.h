/*
 * J. Federico Hernandez <federico.hernandez@um.edu.ar>
 */
#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdlib.h>

#define EXIT_ERRSYS(call, cmp_val)	\
	do { 							\
		if ((call) == cmp_val) {	\
			perror(#call); 			\
			exit(EXIT_FAILURE);		\
		}							\
	} while (0)

#define EXIT_ERR(call)				\
	do {							\
		if (call) {					\
			perror(#call);			\
			exit(EXIT_FAILURE);		\
		}							\
	} while (0)

#define EXIT_ERRIMG(msg)			\
	do {							\
		fprintf(stderr, msg);		\
		exit(EXIT_FAILURE);			\
	} while (0)


#endif
