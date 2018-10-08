// PREPROCESADOR

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "estructuras.h"
#include "funciones.h"
#include "procedimientos.h"

// VARIABLES GLOBALES

pthread_mutex_t llave;
float pixelesNegros = 0.0;
float pixelesTotales = 0.0;
int hebrasConversion = 1;
int hebrasBinarizacion = 1;
int hebrasClasificacion = 1;
int inicio = 0;
int division = 0;


// Funcion main: Funcion que crea los nombres de las imagenes de entrada,
// y da paso a la creacion de hebras para la ejecucion de las funciones (lectura, conversion, binarizacion y clasificacion)
// Entrada: Cuatro variables de tipo entero llamadas c que representa la cantidad de imagenes, h que es la cantidad de hebras, u que es el umbral de binarizacion
// y n que es el umbral para la clasificacion como argumentos.
// Salida: Entero que representa fin de su ejecucion.

int main(int argc, char **argv)
{
	int i = 0;
	int c = 0;
	int h = 0;
	int u = 0;
	int n = 0;
	int bflag = 0;
	int o;
	printf("****Bienvenido al programa: Lab_3 - PROCESAMIENTO DE IMAGENES a través de uso de hebras****\n");
	while((o = getopt(argc, argv, "c:h:u:n:b"))!=-1)
	{
		switch(o)
		{
			case 'c':
				sscanf(optarg,"%d",&c);
				break;
			case 'h':
				sscanf(optarg,"%d",&h);
			case 'u':
				sscanf(optarg,"%d",&u);
				break;
			case 'n':
				sscanf(optarg,"%d",&n);
				break;
			case 'b':
				bflag = 1;
				break;
			case '?':
				if(optopt=='c')
					fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
				else if(isprint(optopt))
					fprintf(stderr, "Opcion desconocida '-%c'.\n", optopt);
				else if('c'==0)
				{
					fprintf(stderr, "-%c no puede ser 0\n", optopt);
				}
				else
					fprintf(stderr, "Opcion con caracter desconocido '\\x%x'\n", optopt);
				return 1;
			default:
				abort();
		}
		if(h<0)
		{
			printf("\nParametro de cantidad de hebras incorrecto, ejecute nuevamente ./Laboratorio3....\n");
			abort();
		}
		if(u>255 || u<0)
		{
			printf("\nParametro del umbral de binarizacion incorrecto, ejecute nuevamente ./Laboratorio3....\n");
			abort();
		}
		if(n>100 || n<0)
		{
			printf("\nParametro del umbral de clasificacion incorrecto, ejecute nuevamente ./Laboratorio3....\n");
			abort();
		}
	}
	printf("\n|     Imagen     |     Nearly Black     |\n");
	
	while(c>0)
    {
		// SE CREAN LAS VARIABLES/ARREGLOS DE TIPO HEBRA A UTILIZAR.
		pthread_t hiloLectura;
		pthread_t hilosConversion[h];
		pthread_t hilosBinarizacion[h];
		pthread_t hilosClasificacion[h];
		
		// SE INICIALIZA LA EXCLUSION MUTUA A UNA SECCION CRITICA.
		pthread_mutex_init(&llave,NULL);
		
		parametros datos;
		bmpFileHeader file; // VARIABLE file.
		bmpInfoHeader info; // VARIABLE info.

		char cantidadImg[10];
		char cantidadImg2[10];
		sprintf(cantidadImg,"%d",c);
		sprintf(cantidadImg2,"%d",c);
	
		char archivo[] = "imagen_";
		char texto[] = ".bmp";
	
		strcat(archivo,cantidadImg);
		strcat(archivo,texto); // VARIABLE nomImagen CORRESPONDIENTE A archivo.
		
		char archivo2[] = "imagen_salida_";
		char texto2[] = ".bmp";
		
		strcat(archivo2,cantidadImg2);
		strcat(archivo2,texto2);
		
		// SE RELLENA LA ESTRUCTURA parametros LLAMADA datos.
		datos.file = &file;
		datos.info = &info;
		datos.nomImagen = archivo;
		datos.nomImagenSalida = archivo2;
		datos.hebras = &h;
		datos.umbralBin = &u;
		datos.umbralClas = &n;
		
		//printf("nomImagenSalida: %s\n",datos.nomImagenSalida);
		
		// SE INICIALIZA LA HEBRA LECTURA.
		pthread_create(&hiloLectura,NULL,lectura,(void *)&datos);
		//printf("HEBRA LECTURA INICIALIZADA\n");
		pthread_join(hiloLectura,NULL); // LA HEBRA PRINCIPAL (main) ESPERA A QUE TERMINE LA HEBRA CREADA, EN ESTE CASO hiloLectura.
		//printf("HEBRA LECTURA FINALIZADA\n");
		
		division = (datos.info->imgsize)/h;
		//printf("DIVISION = %d\n",division);
		//printf("UMBRALBIN = %d\n", *(datos.umbralBin));
		//printf("UMBRALCLAS = %d\n", *(datos.umbralClas));
		//infoH(datos.info); // IMPRIME LA INFO.
		//printf("DATOS ORIGINALES:");
		//imprimirImagen(datos.imagen,datos.file,datos.info);
		
		while(i<h)
		{
			pthread_create(&hilosConversion[i],NULL,conversion,(void *)&datos);
			//printf("HEBRA %d CONVERSION INICIALIZADA\n",i+1);
			i++;
		}
		i = 0;
		
		while(i<h)
		{
			pthread_join(hilosConversion[i],NULL);
			//printf("HEBRA %d CONVERSION FINALIZADA\n",i+1);
			i++;
		}
		i = 0;
		inicio = 0;
		//printf("SE PROCEDE A BINARIZAR\n");
		//printf("DATOS CONVERSION:");
		//imprimirImagen(datos.imagen,datos.file,datos.info);
		
		while(i<h)
		{
			pthread_create(&hilosBinarizacion[i],NULL,binarizacion,(void *)&datos);
			//printf("HEBRA %d BINARIZACION INICIALIZADA\n",i+1);
			i++;
		}
		i = 0;
		
		while(i<h)
		{
			pthread_join(hilosBinarizacion[i],NULL);
			//printf("HEBRA %d BINARIZACION FINALIZADA\n",i+1);
			i++;
		}
		i = 0;
		inicio = 0;
		//printf("SE PROCEDE A CLASIFICAR\n");
		//printf("DATOS BINARIZACION:");
		//imprimirImagen(datos.imagen,datos.file,datos.info);
		
		while(i<h)
		{
			pthread_create(&hilosClasificacion[i],NULL,clasificacion,(void *)&datos);
			//printf("HEBRA %d CLASIFICACION INICIALIZADA\n",i+1);
			i++;
		}
		i = 0;
		
		while(i<h)
		{
			pthread_join(hilosClasificacion[i],NULL);
			//printf("HEBRA %d CLASIFICACION FINALIZADA\n",i+1);
			i++;
		}
		i = 0;
		inicio = 0;
		//printf("SE PROCEDE A IMPRIMIR\n");
		//printf("PIXELES NEGROS: %d\n",(int)pixelesNegros);
		//printf("PIXELES TOTALES: %d\n",(int)pixelesTotales);
		guardarBMP(datos.nomImagen,datos.nomImagenSalida,datos.imagen,datos.file,datos.info,*(datos.umbralClas));
		
		pixelesNegros = 0.0;
		pixelesTotales = 0.0;
		hebrasConversion = 1;
		hebrasBinarizacion = 1;
		hebrasClasificacion = 1;
		division = 0;
		c--;
	}
	return 0;
}
