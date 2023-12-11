#include "metricas.h"
#include <stdio.h>

void calcularMetricas(int verdaderosPositivos, int verdaderosNegativos,
                      int falsosPositivos, int falsosNegativos, int k) {
    double precision = (double)(verdaderosPositivos + verdaderosNegativos) /
                     (verdaderosPositivos + verdaderosNegativos +
                      falsosPositivos + falsosNegativos);
    double sensibilidad = 0.0;
    if (verdaderosPositivos + falsosNegativos != 0) {
        sensibilidad =
            (double)verdaderosPositivos / (verdaderosPositivos + falsosNegativos);
    } else {
        sensibilidad = 0.0;
    }
    double especificidad =
        (double)verdaderosNegativos / (verdaderosNegativos + falsosPositivos);
    double valorPredictivoPositivo =
        (double)verdaderosPositivos / (verdaderosPositivos + falsosPositivos);
    double valorPredictivoNegativo =
        (double)verdaderosNegativos / (verdaderosNegativos + falsosNegativos);

    printf("Precision: %.3f\n", precision);
    printf("Sensibilidad: %.3f\n", sensibilidad);
    printf("Especificidad: %.3f\n", especificidad);
    printf("Valor Predictivo Positivo: %.3f\n", valorPredictivoPositivo);
    printf("Valor Predictivo Negativo: %.3f\n", valorPredictivoNegativo);
}

void calcularMetricasKnn(Dato *datosEntrenamiento, int cantidadFilasEntrenamiento,
                         Dato *datosPrueba, int cantidadFilasPrueba,
                         int cantidadColumnas, int k) {
    int verdaderosPositivos = 0;
    int verdaderosNegativos = 0;
    int falsosPositivos = 0;
    int falsosNegativos = 0;

    for (int i = 0; i < cantidadFilasPrueba; i++) {
        int prediccion = knn(datosEntrenamiento, cantidadFilasEntrenamiento,
                             &datosPrueba[i], k, cantidadColumnas, -1);

        if (prediccion == 1 && datosPrueba[i].match == 1) {
            verdaderosPositivos++;
        } else if (prediccion == 0 && datosPrueba[i].match == 0) {
            verdaderosNegativos++;
        } else if (prediccion == 1 && datosPrueba[i].match == 0) {
            falsosPositivos++;
        } else if (prediccion == 0 && datosPrueba[i].match == 1) {
            falsosNegativos++;
        }
    }

    calcularMetricas(verdaderosPositivos, verdaderosNegativos, falsosPositivos,
                     falsosNegativos, k);
}


