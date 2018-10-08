// PREPROCESADOR

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "estructuras.h"
#include "funciones.h"

// VARIABLES GLOBALES

extern float pixelesNegros;
extern float pixelesTotales;

void infoH(bmpInfoHeader *bInfoHeader)
{
	printf("Tamano de la cabecera: %d\n", bInfoHeader->headersize);
	printf("Ancho: %d\n", bInfoHeader->width);
	printf("Alto: %d\n", bInfoHeader->height);
	printf("Planos: %d\n", bInfoHeader->planes);
	printf("Bits por pixel: %d\n", bInfoHeader->bpp);
	printf("Compresion: %d\n", bInfoHeader->compress);
	printf("Tamano de los datos de imagen: %d\n", bInfoHeader->imgsize);
	printf("Resolucion horizontal (X): %d\n", bInfoHeader->bpmx);
	printf("Resolucion vertical (Y): %d\n", bInfoHeader->bpmy);
	printf("Colores usados en la paleta: %d\n", bInfoHeader->colors);
	printf("Colores importantes: %d\n", bInfoHeader->imxtcolors);
}

void guardarBMP(char *nombre, char *nombreS, unsigned char *datosImagen, bmpFileHeader *bFileHeader, bmpInfoHeader *bInfoHeader, int clas)
{
	float porcentaje;
	uint16_t type = 0x4D42;
    FILE *imagen;
    imagen = fopen(nombreS,"wb");
	fwrite(&type, sizeof(uint16_t), 1, imagen);
	fwrite(&bFileHeader->size, 4, 1, imagen);
	fwrite(&bFileHeader->resv1, 2, 1, imagen);
	fwrite(&bFileHeader->resv2, 2, 1, imagen);
	fwrite(&bFileHeader->offset, 4, 1, imagen);
	fwrite(&bInfoHeader->headersize, 4, 1, imagen);
	fwrite(&bInfoHeader->width, 4, 1, imagen);
	fwrite(&bInfoHeader->height, 4, 1, imagen);
	fwrite(&bInfoHeader->planes, 2, 1, imagen);
	fwrite(&bInfoHeader->bpp, 2, 1, imagen);
	fwrite(&bInfoHeader->compress, 4, 1, imagen);
	fwrite(&bInfoHeader->imgsize, 4, 1, imagen);
	fwrite(&bInfoHeader->bpmx, 4, 1, imagen);
	fwrite(&bInfoHeader->bpmy, 4, 1, imagen);
	fwrite(&bInfoHeader->colors, 4, 1, imagen);
	fwrite(&bInfoHeader->imxtcolors, 4, 1, imagen);
	fseek(imagen, bFileHeader->offset, SEEK_SET);
	fwrite(datosImagen, bInfoHeader->imgsize, 1, imagen);
    fclose(imagen);
	porcentaje = ((pixelesNegros*100)/pixelesTotales);
	//printf("PIXELES NEGROS EN guardarBMP: %f\n", pixelesNegros);
	//printf("PIXELES TOTALES EN guardarBMP: %f\n", pixelesTotales);
	//printf("PORCENTAJE EN guardarBMP: %f\n", porcentaje);
	
	if(porcentaje>=clas)
    {
		printf("|   %s   |     Yes     |\n", nombre);
	}
	else
	{
		printf("|   %s   |     No      |\n", nombre);
	}
}

void imprimirImagen(unsigned char *imagen, bmpFileHeader *bFileHeader, bmpInfoHeader *bInfoHeader)
{
	// SE IMPRIME COMO BGR.
    int i;
    int cantidadByte = (bInfoHeader->bpp)/8;
    printf("\n");
    if(cantidadByte==3)
	{
	    for(i = 0;i<bInfoHeader->imgsize;i = i+3)
	    {
	        printf("(%d %d %d) ",imagen[i],imagen[i+1],imagen[i+2]);
	    }
	}
	if(cantidadByte==4)
    {
        for(i = 0;i<bInfoHeader->imgsize;i = i+4)
        {
            printf("(%d %d %d %d) ", imagen[i],imagen[i+1],imagen[i+2],imagen[i+3]);
        }
    }
    printf("\n");
}