default: cluster_image clean

cluster_image: cluster_image.c kmeans.o mapping.o pixel.o
	gcc cluster_image.c -o cluster_image -lm kmeans.o mapping.o pixel.o

kmeans.o: kmeans.c
	gcc -c kmeans.c -lm

pixel.o: pixel.c
	gcc -c pixel.c

mapping.o: mapping.c
	gcc -c mapping.c

test: test.c mapping.o
	gcc test.c -o test mapping.o -lm

clean:
	rm *.o

