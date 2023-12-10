#include "enn.h"
#include "metricas.h"
#include "datos.h"
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include "knn.h"

void enn(Dato *datosEntrenamiento, Dato **datosEntrenamientoEnn,
         int cantidadColumnas, int cantidadFilasEntrenamiento,
         int *cantidadFilasEntrenamientoEnn, int k) {
    *cantidadFilasEntrenamientoEnn = 0;

    for (int i = 0; i < cantidadFilasEntrenamiento-1; i++) {
        if (knn(datosEntrenamiento, cantidadFilasEntrenamiento,
                &datosEntrenamiento[i], k, cantidadColumnas, i) == datosEntrenamiento[i].match) {
            (*cantidadFilasEntrenamientoEnn)++;
            *datosEntrenamientoEnn =
                realloc(*datosEntrenamientoEnn, *cantidadFilasEntrenamientoEnn * sizeof(Dato));
            (*datosEntrenamientoEnn)[*cantidadFilasEntrenamientoEnn - 1].v =
                malloc(cantidadColumnas * sizeof(double));

            memcpy((*datosEntrenamientoEnn)[*cantidadFilasEntrenamientoEnn - 1].v,
                   datosEntrenamiento[i].v, cantidadColumnas * sizeof(double));

            (*datosEntrenamientoEnn)[*cantidadFilasEntrenamientoEnn - 1].match =
                datosEntrenamiento[i].match;
        }
    }
  *cantidadFilasEntrenamientoEnn -= 1;
}

