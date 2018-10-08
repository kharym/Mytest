#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

typedef struct bmpFileHeader
{
    uint32_t size;/* Tamaño del archivo */
    uint16_t resv1;/* Reservado */
    uint16_t resv2;/* Reservado */
    uint32_t offset;/* Offset hasta los datos de imagen */
}bmpFileHeader;

typedef struct bmpInfoHeader
{
    uint32_t headersize;/* Tamaño de la cabecera */
    uint32_t width;/* Ancho */
    uint32_t height;/* Alto */
    uint16_t planes;/* Planos de color (Siempre 1) */
    uint16_t bpp;/* Bits por pixel */
    uint32_t compress;/* Compresión */
    uint32_t imgsize;/* Tamaño de los datos de imagen */
    uint32_t bpmx;/* Resolución X en bits por metro */
    uint32_t bpmy;/* Resolución Y en bits por metro */
    uint32_t colors;/* Colores usados en la paleta */
    uint32_t imxtcolors;/* Colores importantes. 0 si son todos */
}bmpInfoHeader;

typedef struct parametros
{
	bmpFileHeader *file;
	bmpInfoHeader *info;
	char *nomImagen;
	char *nomImagenSalida;
	unsigned char *imagen;
	int *hebras;
	int *umbralBin;
	int *umbralClas;
}parametros;

#endif // ESTRUCTURAS_H_INCLUDED
