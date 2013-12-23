#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "error.h"
#include "ppm.h"

/**
 * Verifica que la dimension de la imagen sea de un tamaño razonable.
 *
 * @param int dim
 */
static void check_dimension(int dim) {
	if (dim < 1 || dim > 4000) 
		EXIT_ERRIMG("El archivo no tiene un width o height razonable.\r\n");
}

/**
 * Lee la cabecera, verifica el formato y obtiene el ancho y alto de la imagen.
 *
 * @param FILE *fp
 * @param int *width
 * @param int *height
 * @param int *bpp
 *
 * @return int width
 * @return int height
 * @return int bpp
 */
static void read_ppm_header(FILE *fp, int *width, int *height, int *bpp) {
	char ch, line[80];

	fseek(fp, 0, SEEK_SET);

	fgets(line, 3, fp);
	if (line[1] != '6')
		EXIT_ERRIMG("El archivo no esta en formato ppm.\nNo se puede leer.\n");

	fseek(fp, 3, SEEK_SET);
	ch = getc(fp);
	if (ch == '#') {
		do {
			fgets(line, sizeof line, fp);
		} while (line[0] == '#');
	} else {
		fseek(fp, 2, SEEK_SET);
	}

	fscanf(fp, "%d%d%d\n", width, height, bpp);
	if ( *width == 0 || *height == 0 || *bpp == 0)
		EXIT_ERRIMG("No se pudo leer la cabecera ppm del archivo.\n");
	if ( *bpp != 255)
		EXIT_ERRIMG("La imagen no es true-color (24 bits).\n");

	check_dimension(*width);
	check_dimension(*height);
}


image_t *image_create(int width, int height) {
	image_t *img;

	EXIT_ERRSYS(img=(image_t *)malloc(sizeof (image_t)), NULL);
		
	img->width  = width;
	img->height = height;

	EXIT_ERRSYS(img->data=(u_char *)malloc(width * height * 3), NULL);

	return img;
}

image_t *image_read(const char *filename) {
	int width=0, height=0, size=0, bpp=0;
	int num=0;
	FILE *fp=NULL;
	image_t *img=NULL;

	EXIT_ERRSYS(img=(image_t *)malloc(sizeof (image_t)), NULL);
	EXIT_ERRSYS(fp=fopen(filename, "r"), NULL);

	read_ppm_header(fp, &width, &height, &bpp);

	size = width * height * 3;
	img->width  = width;
	img->height = height;
	img->bpp    = bpp;

	EXIT_ERRSYS( img->data=(u_char *)malloc(size), NULL);

	if ( (num=fread((void *)img->data, 1, (size_t) size, fp)) != size)
		EXIT_ERRIMG("No se puede leer los datos de la imagen.\r\n");

	fclose(fp);
	return img;
}


void image_write(image_t *img, const char *img_filename) {
	int nchars=0;
	int size = img->width * img->height * 3;
	FILE *fp;
	
	EXIT_ERRSYS(fp = fopen(img_filename, "w"), NULL);

	fprintf(fp, "P6\n#size %s\n%d %d\n%d\n", " Filtros Digitales", img->width, img->height, 255);

	if ( (nchars=fwrite((void *) img->data, 1, (size_t) size, fp)) != size)
		EXIT_ERRIMG("No se pueden escribir datos de la imagen en el archivo.\r\n");

	fclose(fp);
}  

void image_bk_color(image_t *img, u_char red, u_char green, u_char blue) {
	int i;
	int size = img->width * img->height;
	u_char *data = img->data;

	for (i=0; i<size; i++) {
		*data++ = red;
		*data++ = green;
		*data++ = blue;
	}
}

inline void image_put_pixel(image_t *img, int x, int y, int chan, u_char val) {
	int offset = (y * img->width + x) * 3 + chan;
	img->data[offset] = val;
}

inline u_char image_get_pixel(image_t *img, int x, int y, int chan) {
	int offset = (y * img->width + x) * 3 + chan;
	return img->data[offset];
}

inline int image_get_width(image_t *img) {
	 return img->width;
}

inline int image_get_height(image_t *img) {
	return img->height;
}

inline int image_get_bpp(image_t *img) {
	return img->bpp;
}

void image_destroy(image_t *img) {
	free(img);
}
