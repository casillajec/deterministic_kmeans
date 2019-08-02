#include <stdio.h>
#include <math.h>
#include <limits.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "kmeans.h"
#include "mapping.h"
#include "pixel.h"

char* build_output_path(char* img_path, int k){
	char* output_path = NULL;
	int i = (int) strlen(img_path) - 1;
	int dot_idx = -1, slash_idx = -1;
	int name_length = 0, buffer = 0;
	
	// Find index of last / and last .
	while(i >= 0){
		if(img_path[i] == '.') dot_idx = i;
		if(img_path[i] == '/') slash_idx = i;
		i -= 1;
	}
	// Build string
	name_length = (dot_idx - (slash_idx+1));
	buffer = name_length + 1 + 10 + 4 + 1;
	output_path = malloc(sizeof(char)*(buffer));
	memset(output_path, 0, buffer);
	
	strncpy(output_path, &(img_path[slash_idx+1]), name_length);
	sprintf(output_path, "%s_%d.png", output_path, k);
	return output_path;
}

int main(int argc, char* argv[]){
	
	if (argc < 3){
		printf("You must specify image_path and k\n");
		exit(0);
	}
	// Read args
	char* img_path = argv[1];
	int k = atoi(argv[2]);
	
	// Read image
	int width, height, n;
	unsigned char* img = stbi_load(img_path, &width, &height, &n, 3);
	if(img == NULL){
		printf("Could not load image\n");
		exit(0);
	}
	printf("width: %d, height: %d, n: %d\n", width, height, n);
	
	// Initialize data
	printf("Data initialization...\n");
	int n_pixels = width * height;
	int i, n_datap;
	mapping* uniques_mapping = NULL;
	pixel *c_means;
	pixel_uint8 *data;
	int *data_count, *clusters;
	float mse;
	unsigned char* clustered_img;
	
	// Create pixel -> index mapping
	printf("Mapping pixel to indices\n");
	for(i = 0; i < n_pixels; i++){
		mapping_update(&uniques_mapping, (pixel_uint8){img[(i*3)], img[(i*3)+1], img[(i*3)+2]}, i);
	}
	n_datap = HASH_COUNT(uniques_mapping);
	printf("n_pixels: %d, uniques: %d\n", n_pixels, n_datap);
	
	// Allocate algorithm data
	data = malloc(sizeof(pixel_uint8)*n_datap);
	data_count = malloc(sizeof(int)*n_datap);
	clusters = (int*) malloc(sizeof(int)*n_datap);
	c_means = malloc(sizeof(pixel)*k);
	
	// Count datap
	mapping* walk = uniques_mapping;
	for(i = 0; i < n_datap; i++){
		data[i] = walk->p;
		data_count[i] = int_list_length(walk->indices);
		walk = walk->hh.next;
	}
	
	// Clusterize image
	printf("Clustering data...\n");
	mse = k_means(n_datap, data, data_count, k, rgb_distance, random_init, clusters, c_means);
	
	printf("MSE: %f\n", mse);
	
	// Write image
	printf("Building new image\n");
	clustered_img = malloc(sizeof(unsigned char)*n_pixels*3);
	walk = uniques_mapping;
	int_list* list_walk;
	for(i = 0; i < n_datap; i++){
		list_walk = walk->indices;
		while(list_walk != NULL){
			clustered_img[(list_walk->el*3)  ] = (unsigned char) round(c_means[clusters[i]].r);
			clustered_img[(list_walk->el*3)+1] = (unsigned char) round(c_means[clusters[i]].g);
			clustered_img[(list_walk->el*3)+2] = (unsigned char) round(c_means[clusters[i]].b);
			list_walk = list_walk->next;
			list_walk != NULL;
		}
		walk = walk->hh.next;
	}
	
	free(data);
	free(data_count);
	free(clusters);
	free(c_means);
	mapping_free(&uniques_mapping);
	
	// Build output string
	char* output_path = build_output_path(img_path, k);
	printf("%s\n", output_path);
	
	// Write image to disk
	printf("Writing to disk\n");
	stbi_write_png(output_path, width, height, 3, clustered_img, 0);
	
	// Finally free structures
	stbi_image_free(img);
	free(clustered_img);
	free(output_path);
	
	return 1;
}
