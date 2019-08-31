#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#include "kmeans.h"
#include "pixel.h"

#define EPS 1.0E-3

float get_mse(int n_datap, pixel_uint8* data, int* data_count, pixel* c_means, int* clusters, float (*distance_f)(pixel, pixel)){
	float mse = 0;
	unsigned int acc = 0;
	pixel aux;

	for(int i = 0; i < n_datap; i++){
		aux = (pixel){data[i].r, data[i].g, data[i].b};
		mse += pow(distance_f(aux, c_means[clusters[i]]), 2)*data_count[i];
		acc += data_count[i];
	}

	mse /= acc;
	return mse;
}

int means_equal(pixel* c_means, pixel* old_means, int k){
	int equal = 1;
	for(int i = 0; i < k; i++){
		equal = equal && (
			fabs(c_means[i].r - old_means[i].r) < EPS &&
			fabs(c_means[i].g - old_means[i].g) < EPS &&
			fabs(c_means[i].b - old_means[i].b) < EPS);
	}

	return equal;
}

float k_means(int n_datap, pixel_uint8* data, int* data_count, int k, float (*distance_f)(pixel, pixel), void (*init_f)(int, pixel_uint8*, int, pixel*), int* clusters, pixel* c_means){

	// Structure initialization
	pixel* old_means = malloc(sizeof(pixel)*k);
	int* mean_count = malloc(sizeof(int)*k);
	int i, it_count = 0, min_idx, j, cluster_idx;
	float mse = 0, dis, min_dis;
	pixel* data_walk;
	pixel aux;

	// Select starting cluster centers
	init_f(n_datap, data, k, c_means);

	while(1){
		// Asign datap to corresponding clusters
		for(i = 0; i < n_datap; i++){
			min_dis = (int)INT_MAX;
			min_idx = -1;
			for(j = 0; j < k; j++){
				aux = (pixel){data[i].r, data[i].g, data[i].b};
				dis = distance_f(aux, c_means[j]);
				if( dis < min_dis){
					min_dis = dis;
					min_idx = j;
				}
			}
			clusters[i] = min_idx;
		}

		// Copy c_means to old_means
		for(i = 0; i < k; i++){
			old_means[i].r = c_means[i].r;
			old_means[i].g = c_means[i].g;
			old_means[i].b = c_means[i].b;
		}

		// Update centroids
		for(i = 0; i < k; i++){
			mean_count[i] = 1;
		}

		for(i = 0; i < n_datap; i++){
			cluster_idx = clusters[i];
			c_means[cluster_idx].r += data[i].r*data_count[i];
			c_means[cluster_idx].g += data[i].g*data_count[i];
			c_means[cluster_idx].b += data[i].b*data_count[i];
			mean_count[cluster_idx] += data_count[i];
		}

		for(i = 0; i < k; i++){
			c_means[i].r /= mean_count[i];
			c_means[i].g /= mean_count[i];
			c_means[i].b /= mean_count[i];
		}

		// Increase iteration counter
		it_count++;

		// If centroids didn't change, break the loop
		if(means_equal(c_means, old_means, k)) break;
	}

	printf("k-means itered %d times\n", it_count);

	mse = get_mse(n_datap, data, data_count, c_means, clusters, distance_f);
	free(old_means);
	free(mean_count);
	return mse;
}
