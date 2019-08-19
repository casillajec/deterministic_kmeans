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

void* pcube_init(){
	void *pcube = malloc(256*256*256*3);
	if(!pcube){
		printf("Out of memmory\n");
		exit(0);
	}
	memset(pcube, -1, 256*256*256*3);

	return pcube;
}

void pcube_reset(void *pcube){
	memset(pcube, -1, 256*256*256*3);
}

int pcube_get(void* pcube, pixel_uint8 p){
	int ret = 0;
	unsigned int offset = (p.b + p.g*256 + p.r*65536)*3;
	memcpy(&ret, pcube + offset, 3);

	return ret;
}

void pcube_set(void* pcube, pixel_uint8 p, int idx){
	unsigned int offset = (p.b + p.g*256 + p.r*65536)*3;
	memcpy(pcube + offset, &idx, 3);
}

char bcube_get(char* found, unsigned char r, unsigned char g, unsigned char b){

	unsigned int offset = b + g*256 + r*65536;
	unsigned char mask = 1 << (offset%8);
	unsigned char res;
	memcpy(&res, found + (offset/8), 1);
	res = res & mask;
	res = res >> (offset%8);

	return res;
}

void bcube_set(char* found, unsigned char r, unsigned char g, unsigned char b){
	unsigned int offset = b + g*256 + r*65536;
	unsigned char mask = 1 << (offset%8);
	unsigned char res = *(found + (offset/8));
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
	unsigned int n_pixels, n_datap, i, k, width, height, n, uniquep_count, tmp;
	pixel *c_means;
	pixel_uint8 *unique_data;
	int *data_count, *clusters;
	float mse;
	pixel_uint8 p_tmp;
	char *img_path, *output_path;
	unsigned char* img;
	char flag_write = 0;

	// Read args
	img_path = argv[1];
	k = atoi(argv[2]);
	if(argc == 4){
		flag_write = 1;
	}

	// Read image
	img = stbi_load(img_path, &width, &height, &n, 3);
	if(img == NULL){
		printf("Could not load image\n");
		exit(0);
	}
	printf("width: %d, height: %d, n: %d\n", width, height, n);

	// Calculate amount of pixels
	n_pixels = width * height;

	// Count unique pixels
	void *pcube = pcube_init();
	printf("Allocated pcube\n");
	n_datap = 0;
	for(i = 0; i < n_pixels; i++){
		p_tmp = (pixel_uint8){img[i*3], img[(i*3)+1], img[(i*3)+2]};
		if(pcube_get(pcube, p_tmp) == 0x00FFFFFF && n_datap < 256*256*256){
			pcube_set(pcube, p_tmp, n_datap);
			n_datap++;
		}
	}
	printf("n_datap %d\n", n_datap);

	// Allocate algorithm data
	unique_data = malloc(sizeof(pixel_uint8)*n_datap);
	data_count = malloc(sizeof(int)*n_datap);
	clusters = (int*) malloc(sizeof(int)*n_datap);
	c_means = malloc(sizeof(pixel)*k);
	printf("Data allocated\n");

	// Build unique datap array
	pcube_reset(pcube);
	uniquep_count = 0;
	for(i = 0; i < n_pixels; i++){
		p_tmp = (pixel_uint8){img[i*3], img[(i*3)+1], img[(i*3)+2]};
		tmp = pcube_get(pcube, p_tmp);
		if(tmp == 0x00FFFFFF && uniquep_count < n_datap){
			pcube_set(pcube, p_tmp, uniquep_count);
			unique_data[uniquep_count] = p_tmp;
			data_count[uniquep_count] = 1;
			uniquep_count++;
		}else{
			data_count[tmp] += 1;
		}
	}
	printf("Uniques has been built\n");

	// Clusterize image
	printf("Clustering data...\n");
	mse = k_means(n_datap, unique_data, data_count, k, rgb_distance, random_init, clusters, c_means);

	printf("MSE: %f\n", mse);

	// Build new image
	if(flag_write){
		printf("Building new image\n");
		for(i = 0; i < n_pixels; i++){
			p_tmp = (pixel_uint8){img[i*3], img[(i*3)+1], img[(i*3)+2]};
			tmp = pcube_get(pcube, p_tmp);
			img[(i*3)  ] = (unsigned char) round(c_means[clusters[tmp]].r);
			img[(i*3)+1] = (unsigned char) round(c_means[clusters[tmp]].g);
			img[(i*3)+2] = (unsigned char) round(c_means[clusters[tmp]].b);
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
		free(data_count);
		free(pcube);
	}

	return 1;
}
