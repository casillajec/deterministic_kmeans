#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "kmeans.h"
#include "pimap.h"
#include "pixel.h"
#include "distances.h"
#include "initializers.h"
#include "image_cmp.h"

char* build_output_path(char* img_path, int k){
	char *output_path = NULL, *img_name = NULL;
	int i = (int) strlen(img_path) - 1;
	int dot_idx = -1, slash_idx = -1;
	int name_length = 0, buffer_size = 0;

	// Find index of last / and last .
	while(i >= 0){
		if(img_path[i] == '.' && dot_idx == -1) dot_idx = i;
		if(img_path[i] == '/' && slash_idx == -1) slash_idx = i;
		i -= 1;
	}
	// Build string
	name_length = (dot_idx - (slash_idx+1));
	buffer_size = name_length + 1 + 10 + 4 + 1 + 7;
	img_name = malloc(name_length);
	output_path = malloc(buffer_size);
	memset(output_path, 0, buffer_size);

	strncpy(img_name, &(img_path[slash_idx+1]), name_length);
	sprintf(output_path, "images/%s_%d.png", img_name, k);

	free(img_name);

	return output_path;
}

int main(int argc, char* argv[]){
	// Parse arguments
	unsigned int k = 0, flag_write = 0;
	char *img_path = NULL;
	int c, index;

	while ((c = getopt(argc, argv, "i:k:w")) != -1){
		switch(c){
			case 'i':
				img_path = optarg;
				break;
			case 'k':
				k = atoi(optarg);
				break;
			case 'w':
				flag_write = 1;
				break;
			case '?':
				if (optopt == 'k' || optopt == 'i')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);

				return 1;
			default:
				abort();
		}
	}

	// Validate input
	if (img_path == NULL){
		printf("You need to provide an image path\n");
		return 1;
	}

	if(k <= 0){
		printf("You need to provide a value for K greater than 0\n");
		return 1;
	}
	printf("Clustering %s in %d clusters...\n", img_path, k);

	// Declare all vars
	printf("Data initialization...\n");
	unsigned int n_pixels, n_datap, i, width, height, n, uniquep_count, tmp;
	pixel *c_means;
	pixel_uint8 *unique_data;
	int *data_count, *clusters;
	float mse;
	pixel_uint8 p_tmp;
	char *output_path;
	unsigned char *img, *new_img;
	pimap pi_map;

	// Read image
	img = stbi_load(img_path, &width, &height, &n, 3);
	if(img == NULL){
		printf("Could not load image\n");
		return 1;
	}
	printf("width: %d, height: %d, n: %d\n", width, height, n);

	// Calculate amount of pixels
	n_pixels = width * height;

	// Count unique pixels
	n_datap = 0;
	pi_map = pimap_init();
	for(i = 0; i < n_pixels; i++){
		p_tmp = (pixel_uint8){img[i*3], img[(i*3)+1], img[(i*3)+2]};
		if(pimap_get(pi_map, p_tmp) == 0x00FFFFFF && n_datap < 256*256*256){
			pimap_set(pi_map, p_tmp, n_datap);
			n_datap++;
		}
	}

	// Check if it would make sense to perform the clustering
	if(n_datap < k){
		printf("K is too big, you are trying to cluster %i datapoints into %i clusters\n", n_datap, k);
		return 1;
	}
	printf("n_datap %d\n", n_datap);

	// Allocate algorithm data
	unique_data = malloc(sizeof(pixel_uint8)*n_datap);
	data_count = malloc(sizeof(int)*n_datap);
	clusters = (int*) malloc(sizeof(int)*n_datap);
	c_means = malloc(sizeof(pixel)*k);
	new_img = malloc(3*n_pixels);
	printf("Data allocated\n");

	// Build unique datap array
	pimap_reset(pi_map);
	uniquep_count = 0;
	for(i = 0; i < n_pixels; i++){
		p_tmp = (pixel_uint8){img[i*3], img[(i*3)+1], img[(i*3)+2]};
		tmp = pimap_get(pi_map, p_tmp);
		if(tmp == 0x00FFFFFF && uniquep_count < n_datap){
			pimap_set(pi_map, p_tmp, uniquep_count);
			unique_data[uniquep_count] = p_tmp;
			data_count[uniquep_count] = 1;
			uniquep_count++;
		}else{
			data_count[tmp] += 1;
		}
	}

	// Clusterize image
	printf("Clustering data...\n");
	mse = k_means(n_datap, unique_data, data_count, k, rgb_distance, random_init, clusters, c_means);

	printf("MSE: %f\n", mse);

	// Build new image
	if(flag_write){
		printf("Building new image\n");
		for(i = 0; i < n_pixels; i++){
			p_tmp = (pixel_uint8){img[i*3], img[(i*3)+1], img[(i*3)+2]};
			tmp = pimap_get(pi_map, p_tmp);
			new_img[(i*3)  ] = (unsigned char) round(c_means[clusters[tmp]].r);
			new_img[(i*3)+1] = (unsigned char) round(c_means[clusters[tmp]].g);
			new_img[(i*3)+2] = (unsigned char) round(c_means[clusters[tmp]].b);
		}

		// Write image to disk
		printf("Writing to disk\n\n");
		output_path = build_output_path(img_path, k);
		stbi_write_png(output_path, width, height, 3, img, 0);

		float lin_cmp = linear_cmp(img, new_img, n_pixels);
		float avg_dis = avg_distance(n_datap, unique_data, c_means, clusters, data_count, n_pixels);
		printf("lin_cmp: %f\n", lin_cmp);
		printf("avg_dis: %f\n", avg_dis);
		printf("rel error: %f\n", fabs(lin_cmp - avg_dis)/lin_cmp);

		// Finally free structures
		stbi_image_free(img);
		stbi_image_free(new_img);
		free(output_path);
		free(clusters);
		free(c_means);
		free(unique_data);
		free(data_count);
		free(pi_map);
	}

	return 0;
}
