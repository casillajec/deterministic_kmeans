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

char b_cube_get(char* found, unsigned char r, unsigned char g, unsigned char b){

	unsigned int offset = b + g*256 + r*65536;
	unsigned char mask = 1 << (offset%8);
	unsigned char res;
	memcpy(&res, found + (offset/8), 1);
	res = res & mask;
	res = res >> (offset%8);

	return res;
}

void b_cube_set(char* found, unsigned char r, unsigned char g, unsigned char b){
	unsigned int offset = b + g*256 + r*65536;
	unsigned char mask = 1 << (offset%8);
	unsigned char res = *(found + (offset/8));
	//memcpy(&res, found + (offset/8), 1);
	res = res | mask;
	memcpy(found + (offset/8), &res, 1);
}

int main(int argc, char* argv[]){

	if (argc < 3){
		printf("You must specify image_path and k\n");
		exit(0);
	}

	// Declare all vars
	printf("Data initialization...\n");
	unsigned int n_pixels, n_datap, i, k, width, height, n;
	pixel *c_means;
	pixel_uint8 *data, *unique_data;
	int *data_count, *clusters;
	float mse;
	pixel_uint8 p_tmp;
	char *img_path, *output_path;
	unsigned char* img;

	// Read args
	img_path = argv[1];
	k = atoi(argv[2]);

	// Read image
	img = stbi_load(img_path, &width, &height, &n, 3);
	if(img == NULL){
		printf("Could not load image\n");
		exit(0);
	}
	printf("width: %d, height: %d, n: %d\n", width, height, n);

	// Calculate amount of pixels
	n_pixels = width * height;

	// Build data array
	data = malloc(sizeof(pixel_uint8)*n_pixels);
	for(i = 0; i < n_pixels; i++){
		data[i] = (pixel_uint8){img[i*3], img[(i*3)+1], img[(i*3)+2]};
	}

	// Count unique pixels
	char *found = malloc((256*256*256)/8);
	printf("Allocated found array\n");
	memset(found, 0, (256*256*256)/8);
	n_datap = 0;
	for(i = 0; i < n_pixels; i++){
		if(!b_cube_get(found, data[i].r, data[i].g, data[i].b)){
			b_cube_set(found, data[i].r, data[i].g, data[i].b);
			n_datap++;
		}
	}
	printf("n_datap %d\n", n_datap);

	// Allocate algorithm data
	unique_data = malloc(sizeof(pixel_uint8)*n_datap);
	clusters = (int*) malloc(sizeof(int)*n_datap);
	c_means = malloc(sizeof(pixel)*k);
	printf("Data allocated\n");

	// Build unique datap array
	memset(found, 0, (256*256*256)/8);
	unsigned int tmp = 0;
	for(i = 0; i < n_pixels; i++){
		if(!b_cube_get(found, data[i].r, data[i].g, data[i].b)){
			b_cube_set(found, data[i].r, data[i].g, data[i].b);
			unique_data[tmp] = data[i];
			tmp++;
		}
	}
	printf("Uniques has been built\n");

	/*
	// Create pixel -> index mapping, this also counts pixels
	printf("Mapping pixel to indices\n");
	for(i = 0; i < n_pixels; i++){
		p_tmp = (pixel_uint8){img[(i*3)], img[(i*3)+1], img[(i*3)+2]};
		mapping_update(&uniques_mapping, &p_tmp, i);

	}
	n_datap = HASH_COUNT(uniques_mapping);
	if(n_datap <= k){
		printf("Abort, the image has %d different pixels and you are trying to create %d clusters.", n_datap, k);
		exit(0);
	}
	printf("n_pixels: %d, uniques: %d\n", n_pixels, n_datap);*/

	// Clusterize image
	printf("Clustering data...\n");
	mse = k_means(n_datap, unique_data, k, rgb_distance, random_init, clusters, c_means);

	printf("MSE: %f\n", mse);

	// Build new image
	/*
	printf("Building new image\n");
	for(i = 0; i < n_pixels; i++){
		p_tmp = (pixel_uint8){img[(i*3)], img[(i*3)+1], img[(i*3)+2]};
		HASH_FIND(hh, uniques_mapping, &p_tmp, sizeof(pixel_uint8), walk);
		img[(i*3)  ] = (unsigned char) round(c_means[clusters[walk->unique_index]].r);
		img[(i*3)+1] = (unsigned char) round(c_means[clusters[walk->unique_index]].g);
		img[(i*3)+2] = (unsigned char) round(c_means[clusters[walk->unique_index]].b);
	}*/
	printf("Building new image\n");
	unsigned int j;
	for(i = 0; i < n_pixels; i++){

		// Find unique index
		for(j = 0; j < n_datap; j++){
			if(pixel_uint8_cmp(data[i], unique_data[j])){ break; }
		}
		img[(i*3)  ] = (unsigned char) round(c_means[clusters[j]].r);
		img[(i*3)+1] = (unsigned char) round(c_means[clusters[j]].g);
		img[(i*3)+2] = (unsigned char) round(c_means[clusters[j]].b);
	}


	// Write image to disk
	printf("Writing to disk\n\n");
	output_path = build_output_path(img_path, k);
	stbi_write_png(output_path, width, height, 3, img, 0);

	// Finally free structures
	stbi_image_free(img);
	free(output_path);
	free(clusters);
	free(c_means);
	free(unique_data);

	return 1;
}
