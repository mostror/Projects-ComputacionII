/*
 * J. Federico Hernandez <federico.hernandez@um.edu.ar>
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "filter.h"
#include "ppm.h"

int lowpass_mask[9] = {1,1,1,1,1,1,1,1,1};
int weigmean_mask[9] = {1,2,1,2,4,2,1,2,1};
int laplace_mask[9] = {0,-1,0,-1,5,-1,0,-1,0};

/* 
 * Filtros pasa bajos (suavizantes). 
 */
//filter_t lowpass = {NULL, NULL, -1, NULL, lowpass_mask, 9, 9};
//filter_t weigmean = {NULL, NULL, -1, NULL, weigmean_mask, 9, 16};

/* 
 * Filtros pasa altos (realzantes) 
 */
//filter_t laplace = {NULL, NULL, -1, NULL, laplace_mask, 9, 1.2};


/**
 * Setea, segun el tamaño de la mascara, el valor de los extremos de la mascara que
 * se ira aplicando a la imagen. 
 * Por ej.: si la mascara es de 7x7 -> return 3 (los valores extremos son -3 y 3).
 *
 * @param int mask_size
 * @return int val_extrem
 */
static int set_mask_extrem_val(int mask_size) {
	switch (mask_size) {
	case 9: /* 3x3 */
		return 1;
	case 25: /* 5x5 */
		return 2;
	case 49: /* 7x7 */
		return 3;
	default:
		fprintf(stderr, "El tamaño de la mascara %d no esta soportado.\n", mask_size);
		break;
	}
	return 0;
}


int filter_exec(filter_t *arg) {
	int height, width;
	int x, y;
	int i, j, c, val;
	double result=0.0;
	u_char pixel, new_pixel;
	
	#if DEBUG
	char filename[20];
	memset(filename, 0, sizeof filename);
	#endif

	if (arg->mask == NULL)
		return 1;

	height = image_get_height(arg->img) - 1;
	width  = image_get_width(arg->img) - 1 ;

	switch (arg->type) {
	case LOWPASS:
		arg->mask = lowpass_mask;
		arg->mask_size = 9;
		arg->normal = 9;
		break;
	case WEIGMEAN:
		arg->mask = weigmean_mask;
		arg->mask_size = 9;
		arg->normal = 12;
		break;
	case LAPLACE:
		arg->mask = laplace_mask;
		arg->mask_size = 9;
		arg->normal = 1.2;
		break;
	}

	for (y = 1; y < height; y++) {
		for (x = 1; x < width; x++)	{

			if ((val = set_mask_extrem_val(arg->mask_size)) == 0) {
				return 2;
			}
			
			for (i = -(val), c = 0; i <= val; i++) {
				for (j = -(val); j <= val; j++, c++) {
					pixel = image_get_pixel(arg->img, x+i, y+j, arg->channel);
					result += pixel * arg->mask[c];

					new_pixel = abs((int)round(result / (double)arg->normal));
					image_put_pixel(arg->new_img, x, y, arg->channel, new_pixel);
				}
			}
			result=0.0;
		}
	}

	#if DEBUG
	snprintf(filename, sizeof filename, "images/%d.ppm", arg->channel);
	image_write(arg->new_img, filename);
	#endif

	return 0;
}
