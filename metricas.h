#ifndef METRICAS_H
#define METRICAS_H

#include <stdio.h> 
void calcularMetricasKnn(Dato *datosEntrenamiento, int cantidadFilasEntrenamiento,
                         Dato *datosPrueba, int cantidadFilasPrueba,
                         int cantidadColumnas, int k);
void calcularMetricas(int verdaderosPositivos, int verdaderosNegativos,
                      int falsosPositivos, int falsosNegativos, int k);
                      
#endif /* METRICAS_H */

