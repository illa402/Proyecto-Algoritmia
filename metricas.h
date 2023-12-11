#ifndef METRICAS_H
#define METRICAS_H
#include <stdio.h> 
#include "datos.h"

void calcularMetricas(int verdaderosPositivos, int verdaderosNegativos, int falsosPositivos, int falsosNegativos, int k);
void calcularMetricasKnn(Dato *datosEntrenamiento, int cantidadFilasEntrenamiento, Dato *datosPrueba, int cantidadFilasPrueba, int cantidadColumnas, int k);
                      
#endif /* METRICAS_H */

