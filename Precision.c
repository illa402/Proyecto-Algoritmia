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

void liberarMemoria(Dato *datos, int cantidadFilas);
void leerDatos(FILE *archivo, Dato **datos, int *cantidadFilas, *cantidadColumnas);
void normalizarDatosConMinMax(Dato *datos, int cantidadFilas, cantidadColumnas, double *min_atributos, *max_atributos);
int knn(Dato *datosEntrenamiento, int tamEntrenamiento, Dato *datoPrueba, int k, attributeCount);
double calcular_dis(double *a, double *b, int n);
void calcularMetricas(int verdaderosPositivos, int verdaderosNegativos, falsosPositivos, int falsosNegativos);
void imprimirMatriz(Dato *datos, int cantidadFilas, int cantidadColumnas);

void contarMatch(Dato *datos, int cantidadFilas, int *matchCount, *noMatchCount) {
  *matchCount = 0;
  *noMatchCount = 0;

  for (int i = 0; i < cantidadFilas; i++) {
    if (datos[i].match == 1) {
      (*matchCount)++;
    } else {
      (*noMatchCount)++;
    }
  }
}

int main() {
  Dato *datosEntrenamiento;
  Dato *datosPrueba;
  int cantidadFilasEntrenamiento = 0;
  int cantidadFilasPrueba = 0;
  int cantidadColumnas = 0;
  int k = 9;

  FILE *archivoEntrenamiento = fopen("entrenamiento50.csv", "r");
  FILE *archivoPrueba = fopen("prueba50.csv", "r");

  if (!archivoEntrenamiento || !archivoPrueba) {
    printf("Error al abrir el archivo\n");
    return 1;
  }

  // Leer datos de entrenamiento
  leerDatos(archivoEntrenamiento, &datosEntrenamiento,&cantidadFilasEntrenamiento, &cantidadColumnas);

  // Calcular los valores máximos y mínimos de cada columna para normalizarlos
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

  // imprimirMatriz(datosEntrenamiento, cantidadFilasEntrenamiento,
  // cantidadColumnas);

  // Normalizar datos de entrenamiento
  normalizarDatosConMinMax(datosEntrenamiento, cantidadFilasEntrenamiento, cantidadColumnas, min_atributos, max_atributos);

  // imprimirMatriz(datosEntrenamiento, cantidadFilasEntrenamiento,
  // cantidadColumnas);

  // Leer datos de prueba
  leerDatos(archivoPrueba, &datosPrueba, &cantidadFilasPrueba,&cantidadColumnas);

  // Normalizar datos de prueba usando los mismos min y max que en el
  // entrenamiento
  normalizarDatosConMinMax(datosPrueba, cantidadFilasPrueba, cantidadColumnas,min_atributos, max_atributos);

  int verdaderosPositivos = 0;
  int verdaderosNegativos = 0;
  int falsosPositivos = 0;
  int falsosNegativos = 0;

  // Realizar predicciones y comparar con las etiquetas reales
  for (int i = 0; i < cantidadFilasPrueba; i++) {
    int prediccion = knn(datosEntrenamiento, cantidadFilasEntrenamiento,&datosPrueba[i], k, cantidadColumnas);

    // Comparar con la etiqueta real
    if (prediccion == 1 && datosPrueba[i].match == 1) {
      verdaderosPositivos++;
    } 
    else if (prediccion == 0 && datosPrueba[i].match == 0) {
      verdaderosNegativos++;
    }
    else if (prediccion == 1 && datosPrueba[i].match == 0) {
      falsosPositivos++;
    }
    else if (prediccion == 0 && datosPrueba[i].match == 1) {
      falsosNegativos++;
    }
  }

  calcularMetricas(verdaderosPositivos, verdaderosNegativos, falsosPositivos,falsosNegativos);

  // Liberar memoria
  liberarMemoria(datosEntrenamiento, cantidadFilasEntrenamiento);
  liberarMemoria(datosPrueba, cantidadFilasPrueba);
  free(min_atributos);
  free(max_atributos);

  fclose(archivoEntrenamiento);
  fclose(archivoPrueba);

  return 0;
}

void liberarMemoria(Dato *datos, int cantidadFilas) {
  for (int i = 0; i < cantidadFilas; i++) {
    free(datos[i].v);
  }
  free(datos);
}

void leerDatos(FILE *archivo, Dato **datos, int *cantidadFilas,int *cantidadColumnas) {
  char linea[LONGMAX];
  *cantidadColumnas = 0;
  *cantidadFilas = 0;

  // Obtener la primera línea del archivo para saber el número de columnas
  // La primera linea de la base de datos son los nombres de los atributos
  // SINO NO VA BIEN
  fgets(linea, sizeof(linea), archivo);
  char *token = strtok(linea, ";");
  while (token != NULL) {
    (*cantidadColumnas)++;
    token = strtok(NULL, ";");
  }

  *datos = (Dato *)malloc(0); // Inicializar con tamaño cero

  while (fgets(linea, sizeof(linea), archivo)) {
    char *token;
    int columnaActual = 0;

    *datos = (Dato *)realloc(*datos, (*cantidadFilas + 1) * sizeof(Dato));
    (*datos)[*cantidadFilas].v =(double *)malloc(*cantidadColumnas * sizeof(double));

    token = strtok(linea, ";");
    while (token != NULL && columnaActual < *cantidadColumnas) {
      if (columnaActual == *cantidadColumnas - 1) {
        // Si es la última columna, se guarda en el campo match
        (*datos)[*cantidadFilas].match =(int)atof(token); // Convertimos el token a entero
      } else {
        // No es la última columna, se guarda en el vector v
        (*datos)[*cantidadFilas].v[columnaActual] = atof(token);
      }

      token = strtok(NULL, ";");
      columnaActual++;
    }

    // Moverse al siguiente caso
    (*cantidadFilas)++;
  }
}

void normalizarDatosConMinMax(Dato *datos, int cantidadFilas,int cantidadColumnas, double *min_atributos,double *max_atributos) {
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

int knn(Dato *datosEntrenamiento, int tamEntrenamiento, Dato *datoPrueba, int k,int attributeCount) {
  int match_counts[2];
  match_counts[0] = match_counts[1] = 0; // inicializar la tabla de match/no match a 0
  double *distances = (double *)malloc(tamEntrenamiento * sizeof(double));

  for (int i = 0; i < tamEntrenamiento; i++) {
    distances[i] =
        calcular_dis(datoPrueba->v, datosEntrenamiento[i].v, attributeCount);
  }

  // Obtener los índices de los k vecinos más cercanos
  for (int i = 0; i < k; i++) {
    int min_index = 0;
    double min_distance = DBL_MAX;
    for (int j = 0; j < tamEntrenamiento; j++) {
      if (distances[j] < min_distance) {
        min_distance = distances[j];
        min_index = j;
      }
    }
    distances[min_index] = DBL_MAX; // Para no considerar este valor de nuevo

    match_counts[datosEntrenamiento[min_index].match]++;
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

void calcularMetricas(int verdaderosPositivos, int verdaderosNegativos,int falsosPositivos, int falsosNegativos) {
  double precision = (double)(verdaderosPositivos + verdaderosNegativos) /(verdaderosPositivos + verdaderosNegativos + falsosPositivos + falsosNegativos);
  double sensibilidad = 0.0;
  if (verdaderosPositivos + falsosNegativos != 0) {
    sensibilidad =(double)verdaderosPositivos / (verdaderosPositivos + falsosNegativos);
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
