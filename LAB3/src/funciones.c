// PREPROCESADOR

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "estructuras.h"
#include "procedimientos.h"

// VARIABLES GLOBALES

extern pthread_mutex_t llave;
extern float pixelesNegros;
extern float pixelesTotales;
extern int hebrasConversion;
extern int hebrasBinarizacion;
extern int hebrasClasificacion;
extern int inicio;
extern int division;

// Funcion lectura: Funcion que abre y lee un arhivo de entrada de tipo .bmp, guarda los datos en las estructuras bFileHeader, bInfoHeader y
// los datos correspondientes a los componentes BGR en un arreglo de tipo unsigned char que se encuentra en la estructura bInfoHeader llamada imagen.
// Entrada: Puntero de tipo void llamado args que corresponde a una estructura de tipo datos.
// Salida: Sin retorno (void).

void *lectura(void *args)
{
	//printf("HEBRA LECTURA EN EL INICIO DE LA FUNCION LECTURA\n");
	parametros *datos = (parametros *)args; // CORRESPONDE A UNA DIRECCION DE MEMORIA DE TIPO parametros QUE ES UNA ESTRUCTURA QUE ALMACENA LOS DATOS.
	
	uint16_t type;
	FILE *imagenBMP = fopen(datos->nomImagen, "r");
	if(imagenBMP==NULL)
    {
		printf("NO SE HA PODIDO ABRIR EL ARCHIVO.\n");
		exit(1);
	}
	fread(&type, sizeof(uint16_t), 1, imagenBMP);
	if(type!=0x4D42)
    {
		fclose(imagenBMP);
		return NULL;
	}
	fread(datos->file,sizeof(bmpFileHeader),1,imagenBMP);
	fread(datos->info,sizeof(bmpInfoHeader),1,imagenBMP);
	datos->imagen = (unsigned char*)malloc((datos->info->imgsize)*sizeof(unsigned char));
	fseek(imagenBMP,datos->file->offset,SEEK_SET);
	fread(datos->imagen,datos->info->imgsize,1,imagenBMP);
	fclose(imagenBMP);
	//printf("HEBRA LECTURA EN EL FINAL DE LA FUNCION LECTURA\n");
}

// Funcion conversion: Funcion que utiliza los datos almacenados en imagen correspondiente a los componentes para el calculo de la intensidad mediante
// una formula, reemplazando este valor en cada componente del pixel.
// Entrada: Puntero de tipo void llamado args que corresponde a una estructura de tipo datos.
// Salida: Sin retorno (void).

void *conversion(void *args)
{
	//printf("HEBRA CONVERSION NUMERO %d EN EL INICIO DE LA FUNCION CONVERSION\n", hebrasConversion);
	pthread_mutex_lock(&llave);
	//printf("Llave bloqueada por hebra conversion numero: %d\n", hebrasConversion);
	parametros *datos = (parametros *)args;
	int i;
	int limite = (hebrasConversion*division);
	float intensidad;
	int cantidadByte = (datos->info->bpp)/8;
	
	if(hebrasConversion==*(datos->hebras))
	{
		limite = (datos->info->imgsize);
	}
	//printf("INICIO: %d\n", inicio);
	//printf("LIMITE: %d\n", limite);
	
	if(cantidadByte==3)
	{
		for(i = inicio;i<limite;i = i+3)
		{
			intensidad = 0.3*(datos->imagen[i+2]) + 0.59*(datos->imagen[i+1]) + 0.11*(datos->imagen[i]);
			datos->imagen[i+2] = (int)intensidad;
			datos->imagen[i+1] = (int)intensidad;
			datos->imagen[i] = (int)intensidad;
		}
		//printf("Llave desbloqueada por hebra conversion numero: %d\n", hebrasConversion);
		inicio = i;
		hebrasConversion++;
		pthread_mutex_unlock(&llave);
		//printf("HEBRA CONVERSION NUMERO %d EN EL FINAL DE LA FUNCION CONVERSION\n", hebrasConversion-1);
	}
	
	if(cantidadByte==4)
	{
		for(i = inicio;i<limite;i = i+4)
		{
			intensidad = 0.3*(datos->imagen[i+2]) + 0.59*(datos->imagen[i+1]) + 0.11*(datos->imagen[i]);
			datos->imagen[i+3] = 255;
			datos->imagen[i+2] = (int)intensidad;
			datos->imagen[i+1] = (int)intensidad;
			datos->imagen[i] = (int)intensidad;
		}
		//printf("Llave desbloqueada por hebra conversion numero: %d\n", hebrasConversion);
		inicio = i;
		hebrasConversion++;
		pthread_mutex_unlock(&llave);
		//printf("HEBRA CONVERSION NUMERO %d EN EL FINAL DE LA FUNCION CONVERSION\n", hebrasConversion-1);
	}
	//printf("DATOS MODIFICADOS POR HEBRA NUMERO: %d\n",hebrasConversion-1);
	//imprimirImagen(datos->imagen,datos->file,datos->info);
}

// Funcion binarizacion: Funcion que utiliza los datos almacenados en imagen correspondiente a los componentes que conforman pixeles en
// escala de grises, dependiendo del umbral de binarizacion, se toma la decision sobre que pixel es blanco o negro, reemplazando estos valores en cada componente del pixel.
// Entrada: Puntero de tipo void llamado args que corresponde a una estructura de tipo datos.
// Salida: Sin retorno (void).

void *binarizacion(void *args)
{
	//printf("HEBRA BINARIZACION NUMERO %d EN EL INICIO DE LA FUNCION BINARIZACION\n", hebrasBinarizacion);
	pthread_mutex_lock(&llave);
	//printf("Llave bloqueada por hebra binarizacion numero: %d\n", hebrasBinarizacion);
	parametros *datos = (parametros *)args;
	int i;
	int limite = (hebrasBinarizacion*division);
	int cantidadByte = (datos->info->bpp)/8;
	
	if(hebrasBinarizacion==*(datos->hebras))
	{
		limite = (datos->info->imgsize);
	}
	//printf("INICIO: %d\n", inicio);
	//printf("LIMITE: %d\n", limite);
	
	if(cantidadByte==3)
	{
	    for(i = inicio;i<limite;i = i+3)
	    {
	        if((datos->imagen[i])>(*(datos->umbralBin)))
            {
                datos->imagen[i+2] = 255;
                datos->imagen[i+1] = 255;
                datos->imagen[i] = 255;
            }
            else
            {
                datos->imagen[i+2] = 0;
                datos->imagen[i+1] = 0;
                datos->imagen[i] = 0;
            }
	    }
		//printf("Llave desbloqueada por hebra binarizacion numero: %d\n", hebrasBinarizacion);
		inicio = i;
		hebrasBinarizacion++;
		pthread_mutex_unlock(&llave);
		//printf("HEBRA BINARIZACION NUMERO %d EN EL FINAL DE LA FUNCION BINARIZACION\n", hebrasBinarizacion-1);
	}
	
	if(cantidadByte==4)
    {
        for(i = inicio;i<limite;i = i+4)
        {
            if((datos->imagen[i])>(*(datos->umbralBin)))
            {
                datos->imagen[i+3] = 255;
                datos->imagen[i+2] = 255;
                datos->imagen[i+1] = 255;
                datos->imagen[i] = 255;
            }
            else
            {
                datos->imagen[i+3] = 255;
                datos->imagen[i+2] = 0;
                datos->imagen[i+1] = 0;
                datos->imagen[i] = 0;
            }
        }
		//printf("Llave desbloqueada por hebra binarizacion numero: %d\n", hebrasBinarizacion);
		inicio = i;
		hebrasBinarizacion++;
		pthread_mutex_unlock(&llave);
		//printf("HEBRA BINARIZACION NUMERO %d EN EL FINAL DE LA FUNCION BINARIZACION\n", hebrasBinarizacion-1);
    }
	//printf("DATOS MODIFICADOS POR HEBRA NUMERO: %d\n",hebrasBinarizacion-1);
	//imprimirImagen(datos->imagen,datos->file,datos->info);
}


// Funcion clasificacion: Funcion que calcula la cantidad de pixeles negros y pixeles totales que conforman la imagen binarizada, obteniendo un porcentaje
// de pixeles negros y comparandolo con el umbral de clasificacion para clasificar si la imagen es nearly black o no.
// Entrada: Puntero de tipo void llamado args que corresponde a una estructura de tipo datos.
// Salida: Sin retorno (void).

void *clasificacion(void *args)
{
	//printf("HEBRA CLASIFICACION NUMERO %d EN EL INICIO DE LA FUNCION CLASIFICACION\n", hebrasClasificacion);
	pthread_mutex_lock(&llave);
	//printf("Llave bloqueada por hebra clasificacion numero: %d\n", hebrasClasificacion);
	parametros *datos = (parametros *)args;
	int i;
	int limite = (hebrasClasificacion*division);
	int cantidadByte = (datos->info->bpp)/8;
	float pNegros = 0.0;
	float pTotales = 0.0;
	
	if(hebrasClasificacion==*(datos->hebras))
	{
		limite = (datos->info->imgsize);
	}
	//printf("INICIO: %d\n", inicio);
	//printf("LIMITE: %d\n", limite);
	
	if(cantidadByte==3)
    {
        for(i = inicio;i<limite;i = i+3)
        {
            if((datos->imagen[i])==0)
            {
                pNegros++;
                pTotales++;
            }
            else
            {
                pTotales++;
            }
        }
		//printf("Llave desbloqueada por hebra clasificacion numero: %d\n", hebrasClasificacion);
		inicio = i;
		hebrasClasificacion++;
		pthread_mutex_unlock(&llave);
		//printf("HEBRA CLASIFICACION NUMERO %d EN EL FINAL DE LA FUNCION CLASIFICACION\n", hebrasClasificacion-1);
    }
	
    if(cantidadByte==4)
    {
        for(i = inicio;i<limite;i = i+4)
        {
            if((datos->imagen[i])==0)
            {
                pNegros++;
                pTotales++;
            }
            else
            {
                pTotales++;
            }
        }
		//printf("Llave desbloqueada por hebra clasificacion numero: %d\n", hebrasClasificacion);
		inicio = i;
		hebrasClasificacion++;
		pthread_mutex_unlock(&llave);
		//printf("HEBRA CLASIFICACION NUMERO %d EN EL FINAL DE LA FUNCION CLASIFICACION\n", hebrasClasificacion-1);
    }
	pixelesNegros+= pNegros;
	pixelesTotales+= pTotales;
}