#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define numeroAtributos 19
#define LONGMAX 4096

typedef struct {
    double *v;
    int match;
} Dato;

void liberarMemoria(Dato *datos, int cantidadFilas);
void leerDatos(FILE *archivo, Dato **datos, int *cantidadFilas,
               int *cantidadColumnas, char ***nombresAtributos);
void normalizarDatosConMinMax(Dato *datos, int cantidadFilas,
                              int cantidadColumnas, double *min_atributos,
                              double *max_atributos);
int knn(Dato *datosEntrenamiento, int tamEntrenamiento, Dato *datoPrueba, int k,
        int attributeCount, int j);
double calcular_dis(double *a, double *b, int n);
void calcularMetricas(int verdaderosPositivos, int verdaderosNegativos,
                      int falsosPositivos, int falsosNegativos);
void calcularMetricasKnn(Dato *datosEntrenamiento, int cantidadFilasEntrenamiento,
 Dato *datosPrueba, int cantidadFilasPrueba,
 int cantidadColumnas, int k);
void imprimirMatriz(Dato *datos, int cantidadFilas, int cantidadColumnas);
void enn(Dato *datosEntrenamiento, Dato **datosEntrenamientoEnn,
         int cantidadColumnas, int cantidadFilasEntrenamiento,
         int *cantidadFilasEntrenamientoEnn, int k);

int main() {

    Dato *datosEntrenamiento;
    Dato *datosEntrenamientoEnn = NULL; // Inicializado a NULL
    Dato *datosPrueba;
    int cantidadFilasEntrenamiento = 0;
    int cantidadFilasEntrenamientoEnn = 0;
    int cantidadFilasPrueba = 0;
    int cantidadColumnas = 0;
    int k = 1;
    int res;
    int prediccion;

    FILE *archivoEntrenamiento = fopen("entrenamiento50.csv", "r");

    if (!archivoEntrenamiento) {
        printf("Error al abrir el archivo\n");
        return 1;
    }

    char **nombresAtributos;
    leerDatos(archivoEntrenamiento, &datosEntrenamiento,
              &cantidadFilasEntrenamiento, &cantidadColumnas, &nombresAtributos);

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

    normalizarDatosConMinMax(datosEntrenamiento, cantidadFilasEntrenamiento,
                             cantidadColumnas, min_atributos, max_atributos);
    enn(datosEntrenamiento, &datosEntrenamientoEnn, cantidadColumnas, cantidadFilasEntrenamiento, &cantidadFilasEntrenamientoEnn, k);
    printf("Cantidad de filas del dataset entrenamiento: %d\nCantidad de filas entrenamiento ENN: %d\n", cantidadFilasEntrenamiento, cantidadFilasEntrenamientoEnn);
    printf("Introduce 0 si desea introducir un dato de prueba o 1 si desea "
           "probarlo con un dataset de prueba: ");
    scanf(" %d", &res);

    if (res) {
        FILE *archivoPrueba = fopen("prueba50.csv", "r");
        if (!archivoPrueba) {
            printf("Error al abrir el archivo\n");
            return 1;
        }
        leerDatos(archivoPrueba, &datosPrueba, &cantidadFilasPrueba,
                  &cantidadColumnas, &nombresAtributos);
        normalizarDatosConMinMax(datosPrueba, cantidadFilasPrueba, cantidadColumnas,
                                 min_atributos, max_atributos);
        
        printf("\nMétricas con DataSet ->\n");
        calcularMetricasKnn(datosEntrenamiento, cantidadFilasEntrenamiento, datosPrueba, cantidadFilasPrueba, cantidadColumnas, k);
        printf("\nMétricas con Enn ->\n");
        calcularMetricasKnn(datosEntrenamientoEnn, cantidadFilasEntrenamientoEnn, datosPrueba, cantidadFilasPrueba, cantidadColumnas, k);

        liberarMemoria(datosPrueba, cantidadFilasPrueba);
        free(min_atributos);
        free(max_atributos);
        fclose(archivoPrueba);
    } else {
        // Ingresar un nuevo dato
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
    fclose(archivoEntrenamiento);
    liberarMemoria(datosEntrenamientoEnn, cantidadFilasEntrenamientoEnn);

    return 0;
}

void liberarMemoria(Dato *datos, int cantidadFilas) {
    for (int i = 0; i < cantidadFilas; i++) {
        free(datos[i].v);
    }
    free(datos);
}

void leerDatos(FILE *archivo, Dato **datos, int *cantidadFilas,
               int *cantidadColumnas, char ***nombresAtributos) {
    char linea[LONGMAX];
    *cantidadColumnas = 0;
    *cantidadFilas = 0;
    *nombresAtributos = malloc(numeroAtributos * sizeof(char *));

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

int knn(Dato *datosEntrenamiento, int tamEntrenamiento, Dato *datoPrueba, int k,
        int attributeCount, int j) {
    int match_counts[2] = {0};
    double *distances = (double *)malloc(tamEntrenamiento * sizeof(double));
    //Para evitar sobreestimacion con el enn
    if (j == -1){ 
      for (int i = 0; i < tamEntrenamiento; i++) {
          distances[i] =
              calcular_dis(datoPrueba->v, datosEntrenamiento[i].v, attributeCount);
      }
    }
  else{
      for (int i = 0; i < j; i++) {
          distances[i] =
              calcular_dis(datoPrueba->v, datosEntrenamiento[i].v, attributeCount);
      }
      distances[j] = DBL_MAX;
      for (int i = j + 1; i < tamEntrenamiento; i++) {
          distances[i] =
              calcular_dis(datoPrueba->v, datosEntrenamiento[i].v, attributeCount);
      }
  }

    for (int i = 0; i < k; i++) {
        int min_index = 0;
        double min_distance = DBL_MAX;
        for (int j = 0; j < tamEntrenamiento; j++) {
            if (distances[j] < min_distance) {
                min_distance = distances[j];
                min_index = j;
            }
        }
        distances[min_index] = DBL_MAX;

        match_counts[datosEntrenamiento[min_index].match] += 1;
    }

    free(distances);
    if (match_counts[1] > match_counts[0]) {
        return 1;
    } else {
        return 0;
    }
}

double calcular_dis(double *a, double *b, int n) {
    double dist = 0;
    for (int i = 0; i < n; i++) {
        dist += pow((a[i] - b[i]), 2);
    }

    if (dist > 0) {
        return sqrt(dist);
    } else {
        return 0;
    }
}

void calcularMetricas(int verdaderosPositivos, int verdaderosNegativos,
                      int falsosPositivos, int falsosNegativos) {
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

void imprimirMatriz(Dato *datos, int cantidadFilas, int cantidadColumnas) {
    for (int i = 0; i < cantidadFilas; i++) {
        printf("Caso %d:\n", i + 1);
        for (int j = 0; j < cantidadColumnas; j++) {
            printf("Atributo %d: %lf\n", j + 1, datos[i].v[j]);
        }
        printf("\n");
    }
}

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

            // Copiar los valores de datosEntrenamiento[i] a datosEntrenamientoEnn
            memcpy((*datosEntrenamientoEnn)[*cantidadFilasEntrenamientoEnn - 1].v,
                   datosEntrenamiento[i].v, cantidadColumnas * sizeof(double));

            (*datosEntrenamientoEnn)[*cantidadFilasEntrenamientoEnn - 1].match =
                datosEntrenamiento[i].match;
        }
    }
  *cantidadFilasEntrenamientoEnn -= 1;
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
                     falsosNegativos);
}
