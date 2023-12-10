#include "knn.h"
#include "datos.h"
#include <float.h>
#include <stdlib.h>
#include <math.h>

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



