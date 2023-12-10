#ifndef DATOS_H
#define DATOS_H
#include "knn.h"
#include <stdio.h> 

#define LONGMAX 4096


void liberarMemoria(Dato *datos, int cantidadFilas);
void leerDatos(FILE *archivo, Dato **datos, int *cantidadFilas, int *cantidadColumnas, char ***nombresAtributos);
void normalizarDatosConMinMax(Dato *datos, int cantidadFilas, int cantidadColumnas, double *min_atributos, double *max_atributos);

#endif /* DATOS_H */
