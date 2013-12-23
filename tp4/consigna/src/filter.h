/* 
 * J. Federico Hernandez <federico.hernandez@um.edu.ar>
 */
#ifndef _FILTER_H_
#define _FILTER_H_

#include "ppm.h"

#define MASK_LOWPASS "lowpass"
#define MASK_WEIGMEAN "weigmean"
#define MASK_LAPLACE "laplace"

typedef enum mask_type {NOSET=-1, LOWPASS, WEIGMEAN, LAPLACE} mask_type_t;

typedef struct filter_arg {
	image_t	*img;
	image_t *new_img;
	int 	channel;
	mask_type_t type;
	int 	*mask;
	int		mask_size;
	float 	normal;
} filter_t;


/**
 * Ejecuta el filtro con los argumentos arg sobre la imagen arg->img, escribiendo el 
 * resultado en la nueva imagen arg->new.
 *
 * @param filter_arg_t *arg
 * @return 0 (se ejecuto el filtro) o 1 (no se pudo ejecutar el filtro)
 */
int filter_exec(filter_t *arg);

#endif
