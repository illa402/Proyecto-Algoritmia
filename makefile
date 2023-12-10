run: mi_programa
	./mi_programa
mi_programa: knn.o enn.o datos.o main.o
	gcc -g knn.o enn.o datos.o main.o -o mi_programa -lm

main.o: main.c knn.o enn.o datos.o
	gcc -g -c main.c

enn.o: enn.c datos.c knn.c
	gcc -g -c enn.c

knn.o: knn.c datos.c
	gcc -g -c knn.c

datos.o: datos.c
	gcc -g -c datos.c

clean:
	rm -f mi_programa *.o
	
