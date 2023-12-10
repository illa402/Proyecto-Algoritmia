#include "knn.h"
#include "enn.h"
#include "datos.h"
#include "metricas.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

int main() {
    Dato *datosEntrenamiento;
    Dato *datosEntrenamientoEnn = NULL;
    Dato *datosPrueba = NULL;
    int cantidadFilasEntrenamiento = 0;
    int cantidadFilasEntrenamientoEnn = 0;
    int cantidadFilasPrueba = 0;
    int cantidadColumnas = 0;
    int res;
    int prediccion;
    int k = 3;

    FILE *archivoEntrenamiento = fopen("entrenamiento50.csv", "r");

    if (!archivoEntrenamiento) {
        printf("Error al abrir el archivo de entrenamiento\n");
        return 1;
    }

    char **nombresAtributos;
    leerDatos(archivoEntrenamiento, &datosEntrenamiento, &cantidadFilasEntrenamiento, &cantidadColumnas, &nombresAtributos);

    double *min_atributos = (double *)malloc(cantidadColumnas * sizeof(double));
    double *max_atributos = (double *)malloc(cantidadColumnas * sizeof(double));

    for (int j = 0; j < cantidadColumnas; j++) {
        min_atributos[j] = DBL_MAX;
        max_atributos[j] = DBL_MIN;

        for (int i = 0; i < cantidadFilasEntrenamiento; i++) {
            if (datosEntrenamiento[i].v[j] < min_atributos[j]) {
                min_atributos[j] = datosEntrenamiento[i].v[j];
            }
            if (datosEntrenamiento[i].v[j] > max_atributos[j]) {
                max_atributos[j] = datosEntrenamiento[i].v[j];
            }
        }
    }

    normalizarDatosConMinMax(datosEntrenamiento, cantidadFilasEntrenamiento, cantidadColumnas, min_atributos, max_atributos);

    enn(datosEntrenamiento, &datosEntrenamientoEnn, cantidadColumnas, cantidadFilasEntrenamiento, &cantidadFilasEntrenamientoEnn, k);
    printf("Cantidad de filas del dataset entrenamiento: %d\nCantidad de filas entrenamiento ENN: %d\n", cantidadFilasEntrenamiento, cantidadFilasEntrenamientoEnn);
    printf("Introduce 0 si desea introducir un dato de prueba o 1 si desea "
           "probarlo con un dataset de prueba: ");

    printf("Introduce 0 si deseas introducir un dato de prueba o 1 si deseas probarlo con un dataset de prueba: ");
    scanf(" %d", &res);

    if (res) {
        FILE *archivoPrueba = fopen("prueba50.csv", "r");
        if (!archivoPrueba) {
            printf("Error al abrir el archivo de prueba\n");
            return 1;
        }
        leerDatos(archivoPrueba, &datosPrueba, &cantidadFilasPrueba, &cantidadColumnas, &nombresAtributos);

        normalizarDatosConMinMax(datosPrueba, cantidadFilasPrueba, cantidadColumnas, min_atributos, max_atributos);

        calcularMetricasKnn(datosEntrenamiento, cantidadFilasEntrenamiento, datosPrueba, cantidadFilasPrueba, cantidadColumnas, k);
        printf("\nMétricas con Enn ->\n");
        calcularMetricasKnn(datosEntrenamientoEnn, cantidadFilasEntrenamientoEnn, datosPrueba, cantidadFilasPrueba, cantidadColumnas, k);

        liberarMemoria(datosPrueba, cantidadFilasPrueba);
        fclose(archivoPrueba);
    } else {
        datosPrueba = (Dato *)malloc(sizeof(Dato));
        datosPrueba->v = (double *)malloc(cantidadColumnas * sizeof(double));

        printf("Ingrese los datos para el nuevo dato de prueba:\n");
        for (int i = 0; i < cantidadColumnas - 1; i++) {
            printf("Ingrese el valor para '%s': ", nombresAtributos[i]);
            scanf("%lf", &datosPrueba->v[i]);
        }
        datosPrueba->match = knn(datosEntrenamiento, cantidadFilasEntrenamiento,
                                 datosPrueba, k, cantidadColumnas, -1);
        printf("La prediccion es: %d\n", datosPrueba->match);
        prediccion = knn(datosEntrenamientoEnn, cantidadFilasEntrenamientoEnn,
                         datosPrueba, k, cantidadColumnas, -1);
        printf("La prediccion con Enn es: %d\n", prediccion);

        free(datosPrueba->v);
        free(datosPrueba);
    }



    liberarMemoria(datosEntrenamiento, cantidadFilasEntrenamiento);
    free(min_atributos);
    free(max_atributos);
    fclose(archivoEntrenamiento);

    return 0;
}
