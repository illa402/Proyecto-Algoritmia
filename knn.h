#ifndef KNN_H
#define KNN_H
#include <stdio.h> 


typedef struct {
    double *v;
    int match;
} Dato;

int knn(Dato *datosEntrenamiento, int tamEntrenamiento, Dato *datoPrueba, int k,
        int attributeCount, int j);
double calcular_dis(double *a, double *b, int n);

#endif /* KNN_H */
