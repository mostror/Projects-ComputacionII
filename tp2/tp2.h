#ifndef _TP2_H_
	#define _TP2_H_

#define NOSET 0
#define RED 1
#define GREEN 2
#define BLUE 3

	
/**
 * inic
 *
 * Inicia los pipes y los procesos hijos, devuelvo el numero de proceso
 *
 * @param int red[]
 * @param int green[]
 * @param int blue[]
 * @param int ret[]
 * @param int channel
 *
 * @return int
 *
 * @access public
 */

int inic(int red[],int green[], int blue[], int ret[], int channel); 



/**
 * setpointer
 *
 * Devuelve un numero de file descriptor correspondiente al archivo con el puntero de inicio despues de la cabecera
 *
 * @param int fd
 * @param char *fileName
 *
 * @return int
 *
 * @access public
 */
int setpointer(int fd, char *fileName);

/**
 * analize
 *
 * Analiza la cadena y escribe en el parentfd una cadena con las intensidades de los pixeles y el canal 
 *
 * @param int channel
 * @param int parentfd
 * @param int lenght
 * @param u_char buff[]
 *
 * @return int
 *
 * @access public
 */

int analize(int channel, int parentfd, int lenght, u_char buff[]); // analiza una cadena de caracteres y devuelve

#endif
