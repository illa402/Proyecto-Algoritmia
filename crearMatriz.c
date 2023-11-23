#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LONGMAX 4096

typedef struct {
  double *v;
  int match;
} Dato;

void imprimirMatriz(Dato *datos, char **nombresAtributos, int cantidadFilas,int cantidadColumnas);
void leerDatos(FILE *archivo, Dato **datos, char ***nombresAtributos,int *cantidadFilas, int *cantidadColumnas, int *matchCount,int *noMatchCount);
void normalizarMatriz(Dato *matriz, int filas, int columnas);
void liberarMemoria(Dato *datos, char **nombresAtributos, int cantidadFilas);

int main() {
  char **nombresAtributos;
  Dato *datos;
  int cantidadFilas = 0;
  int cantidadColumnas = 0;
  int matchCount = 0;
  int noMatchCount = 0;

  FILE *archivo = fopen("sdDESORDEN.csv", "r");
  if (!archivo) {
    printf("Error al abrir el archivo\n");
    return 1;
  }

  leerDatos(archivo, &datos, &nombresAtributos, &cantidadFilas, &cantidadColumnas, &matchCount, &noMatchCount);
  printf("Cantidad de match: %d\n", matchCount);
  printf("Cantidad de no match: %d\n", noMatchCount);

  // imprimirMatriz(datos, nombresAtributos, cantidadFilas, cantidadColumnas);

  liberarMemoria(datos, nombresAtributos, cantidadFilas);
  fclose(archivo);

  return 0;
}

void liberarMemoria(Dato *datos, char **nombresAtributos, int cantidadFilas) {
  for (int i = 0; i < cantidadFilas; i++) {
    free(datos[i].v);
  }
  free(datos);

  for (int i = 0; i < cantidadFilas; i++) {
    free(nombresAtributos[i]);
  }
  free(nombresAtributos);
}

void normalizarMatriz(Dato *matriz, int filas, int columnas) {
  for (int j = 0; j < columnas; j++) {
    double min_atributo = DBL_MAX;
    double max_atributo = DBL_MIN;

    if (filas > 0) {
      min_atributo = matriz[0].v[j];
    }

    for (int i = 0; i < filas; i++) {
      if (matriz[i].v[j] < min_atributo) {
        min_atributo = matriz[i].v[j];
      }
      if (matriz[i].v[j] > max_atributo) {
        max_atributo = matriz[i].v[j];
      }
    }

    if (min_atributo == max_atributo) {
      for (int i = 0; i < filas; i++) {
        matriz[i].v[j] = 0.5;
      }
    } else {
      for (int i = 0; i < filas; i++) {
        matriz[i].v[j] =
            (matriz[i].v[j] - min_atributo) / (max_atributo - min_atributo);
      }
    }
  }
}

void imprimirMatriz(Dato *datos, char **nombresAtributos, int cantidadFilas,
                    int cantidadColumnas) {
  for (int i = 0; i < cantidadFilas; i++) {
    printf("Caso %d:\n", i + 1);
    for (int j = 0; j < cantidadColumnas; j++) {
      printf("%s: %lf\n", nombresAtributos[j], datos[i].v[j]);
    }
    printf("\n");
  }
}

void leerDatos(FILE *archivo, Dato **datos, char ***nombresAtributos,int *cantidadFilas, int *cantidadColumnas, int *matchCount, int *noMatchCount) {
  char linea[LONGMAX];

  if (fgets(linea, sizeof(linea), archivo)) {
    char **atributos = NULL;
    char *token = strtok(linea, ";");
    int cantidadAtributos = 0;

    while (token != NULL) {
      atributos =(char **)realloc(atributos, (cantidadAtributos + 1) * sizeof(char *));
      atributos[cantidadAtributos] =(char *)malloc((strlen(token) + 1) * sizeof(char));
      strcpy(atributos[cantidadAtributos], token);
      token = strtok(NULL, ";");
      cantidadAtributos++;
    }

    *nombresAtributos = atributos;
    *cantidadColumnas = cantidadAtributos;

    *datos = (Dato *)malloc(0);
#nada
    int filaActual = 0;
    while (fgets(linea, sizeof(linea), archivo)) {
      token = strtok(linea, ";");
      int columnaActual = 0;

      *datos = (Dato *)realloc(*datos, (*cantidadFilas + 1) * sizeof(Dato));
      (*datos)[*cantidadFilas].v =(double *)malloc(cantidadAtributos * sizeof(double));

      while (token != NULL) {
        if (columnaActual == cantidadAtributos - 1) {
          int matchValue = atoi(token);
          (*datos)[*cantidadFilas].match = matchValue;

          if (matchValue == 1) {
            (*matchCount)++;
          } 
          else {
            (*noMatchCount)++;
          }
        } 
          else {
          (*datos)[*cantidadFilas].v[columnaActual] = atof(token);
        }

        token = strtok(NULL, ";");
        columnaActual++;
      }

      (*cantidadFilas)++;
    }
  } else {
    printf("Error al leer la primera línea del archivo\n");
  }
}
