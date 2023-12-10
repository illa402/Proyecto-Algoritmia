#ifndef ENN_H
#define ENN_H
#include "knn.h"

void enn(Dato *datosEntrenamiento, Dato **datosEntrenamientoEnn,
         int cantidadColumnas, int cantidadFilasEntrenamiento,
         int *cantidadFilasEntrenamientoEnn, int k);
#endif /* ENN_H */
