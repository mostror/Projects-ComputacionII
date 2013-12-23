#ifndef _PPM_H_
#define _PPM_H_

#include <sys/types.h>

typedef struct image {
	int width;
	int height;
	int bpp;	
	u_char *data;
} image_t;

/**
 * Crea una estrucutra con la informacion de la imagen.
 *
 * @param int width
 * @param int height
 * @return image_t
 */
image_t *image_create(int width, int height);

/**
 * Lee la cabecera y los datos del archivo filename, almacena estos datos  
 * en la estructura image_t, y devuelve ls estructura image_t que contiene 
 * la informacion del archivo.
 *
 * @param const char *filename
 * @return image_t
 */
image_t *image_read(const char *filename);

/**
 * Vuelca a un archivo la informacion almacenada de una imagen en la 
 * estructura image_t.
 *
 * @param image_t *img
 * @param const char *img_filename
 */
void image_write(image_t *img, const char *img_filename);

/**
 * Rellena la imagen (la parte de datos de la imagen ppm) a un color
 * especificado por red, green y blue.
 *
 * @param image_t *img
 * @param u_char red
 * @param u_char green
 * @param u_char blue
 */
void image_bk_color(image_t *img, u_char red, u_char green, u_char blue);

/**
 * Pone un pixel en la parte de datos de la imagen. Luego debe utilizarse la
 * funcion imgae_write() para escribir la informacion de la imagen guardada
 * en la estructura image_t en un archivo.
 *
 * @param image_t *img
 * @param int x
 * @param int y
 * @param int chan (channel: red, green and blue)
 * @param u_char val
 */
inline void image_put_pixel(image_t *img, int x, int y, int chan, u_char val);

/**
 * Devuelve la informacion del pixel en x e y.
 *
 * @param image_t *img
 * @param int x
 * @param int y
 * @param int chan (channel: red, green and blue)
 * @return u_char pixel
 */
inline u_char image_get_pixel(image_t *img, int x, int y, int chan);

/**
 * Devuelve el ancho de la imagen.
 *
 * @param image_t *img
 * @return int width
 */
inline int image_get_width(image_t *img);

/**
 * Devuelve el alto de la imagen.
 *
 * @param image_t *img
 * @return int height
 */
inline int image_get_height(image_t *img);

/**
 * @param image_t *img
 * @return int bpp
 */
inline int image_get_bpp(image_t *img);

/**
 * Libera la memoria asignada para una imagen.
 *
 * @param image_t *img
 */
void image_destroy(image_t *img);

#endif
