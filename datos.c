#include "datos.h"
#include <stdlib.h>
#include <string.h>
#include "knn.h"

void leerDatos(FILE *archivo, Dato **datos, int *cantidadFilas,
               int *cantidadColumnas, char ***nombresAtributos) {
    char linea[LONGMAX];
    *cantidadColumnas = 0;
    *cantidadFilas = 0;
    *nombresAtributos = malloc(19 * sizeof(char *));

    fgets(linea, sizeof(linea), archivo);
    char *token = strtok(linea, ";");
    while (token != NULL) {
        (*nombresAtributos)[*cantidadColumnas] =
            (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy((*nombresAtributos)[*cantidadColumnas], token);
        (*cantidadColumnas)++;
        token = strtok(NULL, ";");
    }

    *datos = (Dato *)malloc(0);

    while (fgets(linea, sizeof(linea), archivo)) {
        char *token;
        int columnaActual = 0;

        *datos = (Dato *)realloc(*datos, (*cantidadFilas + 1) * sizeof(Dato));
        (*datos)[*cantidadFilas].v =
            (double *)malloc(*cantidadColumnas * sizeof(double));

        token = strtok(linea, ";");
        while (token != NULL && columnaActual < *cantidadColumnas) {
            if (columnaActual == *cantidadColumnas - 1) {
                (*datos)[*cantidadFilas].match =
                    (int)atof(token);
            } else {
                (*datos)[*cantidadFilas].v[columnaActual] = atof(token);
            }

            token = strtok(NULL, ";");
            columnaActual++;
        }
        (*cantidadFilas)++;
    }
}

void normalizarDatosConMinMax(Dato *datos, int cantidadFilas,
                              int cantidadColumnas, double *min_atributos,
                              double *max_atributos) {
    for (int j = 0; j < cantidadColumnas; j++) {
        double min_atributo = min_atributos[j];
        double max_atributo = max_atributos[j];

        if (min_atributo != max_atributo) {
            for (int i = 0; i < cantidadFilas; i++) {
                datos[i].v[j] =
                    (datos[i].v[j] - min_atributo) / (max_atributo - min_atributo);
            }
        } else {
            for (int i = 0; i < cantidadFilas; i++) {
                datos[i].v[j] = 0.5;
            }
        }
    }
}

void liberarMemoria(Dato *datos, int cantidadFilas) {
    for (int i = 0; i < cantidadFilas; i++) {
        free(datos[i].v);
    }
    free(datos);
}

void imprimirMatriz(Dato *datos, int cantidadFilas, int cantidadColumnas) {
    for (int i = 0; i < cantidadFilas; i++) {
        printf("Caso %d:\n", i + 1);
        for (int j = 0; j < cantidadColumnas; j++) {
            printf("Atributo %d: %lf\n", j + 1, datos[i].v[j]);
        }
        printf("\n");
    }
}
