#ifndef PROCEDIMIENTOS_H_INCLUDED
#define PROCEDIMIENTOS_H_INCLUDED

void infoH(bmpInfoHeader *bInfoHeader);
void guardarBMP(char *nombre, char *nombreS, unsigned char *datosImagen, bmpFileHeader *bFileHeader, bmpInfoHeader *bInfoHeader, int clas);
void imprimirImagen(unsigned char *imagen, bmpFileHeader *bFileHeader, bmpInfoHeader *bInfoHeader);

#endif // PROCEDIMIENTOS_H_INCLUDED