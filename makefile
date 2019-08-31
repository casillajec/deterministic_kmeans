default: cluster_image clean

cluster_image: cluster_image.c kmeans.o pimap.o pixel.o int_list.o distances.o initializers.o
	gcc cluster_image.c -o cluster_image -lm kmeans.o pimap.o pixel.o int_list.o distances.o initializers.o

kmeans.o: kmeans.c
	gcc -c kmeans.c -lm

pixel.o: pixel.c
	gcc -c pixel.c

pimap.o: pimap.c
	gcc -c pimap.c -lm

int_list.o: int_list.c
	gcc -c int_list.c

distances.o: distances.c
	gcc -c distances.c

initializers.o: initializers.c
	gcc -c initializers.c

test: test.c
	gcc test.c -o test -lm

clean:
	rm *.o

