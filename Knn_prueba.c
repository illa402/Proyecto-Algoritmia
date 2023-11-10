#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 4
#define M 4

typedef int valores[M];
typedef struct dato{
    valores v;
    int match;
}dato;
typedef dato todos[N];

int Resultado (valores, todos);
int calcular_dis(valores a, valores b);
int abs(int a);

int main(void){
    todos tabla;
    valores d;

	tabla[0].v [0] = 0;
	tabla[0].v [1] = 2;
	tabla[0].v [2] = 13;
	tabla[0].v [3] = 4;
	tabla[0].match = 1;


	tabla[1].v [0] = 5;
	tabla[1].v [1] = 8;
	tabla[1].v [2] = 3;
	tabla[1].v [3] = 9;
	tabla[1].match = 1;

	tabla[2].v [0] = 9;
	tabla[2].v [1] = 5;
	tabla[2].v [2] = 11;
	tabla[2].v [3] = 2;
	tabla[2].match = 1;

	tabla[3].v [0] = 4;
	tabla[3].v [1] = 5;
	tabla[3].v [2] = 6;
	tabla[3].v [3] = 2;
	tabla[3].match = 0;


    printf("Cuentame que tal tu cita y te digo a ver si ha habido suerte ;)\n");
    for(int i = 0; i < M; i++){
        printf("dato %d: ", i);
        scanf(" %d", &d[i]);
    }
    int a = Resultado(d, tabla);
    if(a)
        printf("Ole, ole, ole, parece q has tenido suerte, ¡es un match!\n");

    else
        printf("Siento ser yo el que te lo diga pero no ha habido match. No te preocupes, hay más peces en el mar\n");

    printf("\nNo nos hacemos responsables por falsas ilusiones o lagrimas a causa de una equivocación de esta IA\n");

}
int Resultado (valores d, todos t){
    int match = t[0].match;
    float dis;
    float dis_min = calcular_dis(d,t[0].v);
    for(int i = 1; i < N; i++){
        dis = calcular_dis(d, t[i].v);
        if(dis < dis_min){
            dis_min = dis;
            match = t[i].match;
        }
        dis = 0;
    }
    return match;
}

int calcular_dis(valores a, valores b){
    int dis;
    float dist = 0;
    for (int i = 0; i < M; i++){
        dis = pow((a[i] - b[i]),2);
        dist += dis;
    }
    return(sqrt(dist));
}

