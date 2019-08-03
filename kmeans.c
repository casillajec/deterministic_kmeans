#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "kmeans.h"
#include "pixel.h"
#include "mapping.h"

#define EPS 1.0E-3

void random_init(int n_datap, pixel_uint8* data, int k, pixel* c_means){
	// Seed rng
	srand(time(0));
	
	// Initalize strucures and first random idx
	int_list* seen = NULL;
	int ridx = rand()%n_datap;
	c_means[0].r = data[ridx].r;
	c_means[0].g = data[ridx].g;
	c_means[0].b = data[ridx].b;
	int_list_add_front(&seen, ridx);
	
	// Find different random indices
	for(int i = 1; i < k; i++){;
		ridx = rand()%n_datap;
		
		// Generate new idx until an unseen is generated
		while(int_list_contains(seen, ridx)){ ridx = rand()%n_datap; }
		
		//c_means[i] = (pixel){data[ridx].r, data[ridx].g, data[ridx].b};
		c_means[i].r = data[ridx].r;
		c_means[i].g = data[ridx].g;
		c_means[i].b = data[ridx].b;
		int_list_add_front(&seen, ridx);
	}
	
	// Free the allocated structures
	int_list_free(&seen);
}

float euclidean_distance(pixel* p1, pixel* p2){
	return sqrt( pow(p2->r - p1->r, 2) + pow(p2->g - p1->g, 2) + pow(p2->b - p1->b, 2) );
}


float rgb_distance(pixel* p1, pixel* p2){
	float r, s[3], px[3];
	r = (p1->r + p2->r)/2;
	s[0] = (2+(r/256)); s[1] = 4; s[2] = ((2+(255-r))/256);
	px[0] = p1->r - p2->r; px[1] =  p1->g - p2->g; px[2] = p1->b - p2->b;
	return sqrt(s[0]*px[0]*px[0] + s[1]*px[1]*px[1] + s[2]*px[2]*px[2]);
}

void clusterize(int n_datap, pixel_uint8* data, int k, pixel* c_means, int* clusters, float (*distance_f)(pixel*, pixel*)){
	float dis, min_dis;
	int min_idx, i, j;
	pixel aux;
	
	for(i = 0; i < n_datap; i++){
		min_dis = 1000000;
		min_idx = -1;
		for(j = 0; j < k; j++){
			aux.r = data[i].r; aux.g = data[i].g; aux.b = data[i].b;
			dis = distance_f(&aux, &c_means[j]);
			if( dis < min_dis){
				min_dis = dis;
				min_idx = j;
			}
		}
		clusters[i] = min_idx;
	}
}

void update_means(int n_datap, pixel_uint8* data, int k, pixel* c_means, int* clusters, int* data_count, int* mean_count) {
	int i, cluster_idx;
	
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
}

float get_mse(int n_datap, pixel_uint8* data, pixel* c_means, int* clusters, int* data_count, float (*distance_f)(pixel*, pixel*)){
	float mse = 0;
	int acc = 0;
	pixel aux;
	for(int i = 0; i < n_datap; i++){
		aux = (pixel){data[i].r, data[i].g, data[i].b};
		mse += pow(distance_f(&aux, &c_means[clusters[i]]), 2)*data_count[i];
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

float k_means(int n_datap, pixel_uint8* data, int* data_count, int k, float (*distance_f)(pixel*, pixel*), void (*init_f)(int, pixel_uint8*, int, pixel*), int* clusters, pixel* c_means){
	
	// Structure initialization
	pixel* old_means = malloc(sizeof(pixel)*k);
	int* mean_count = malloc(sizeof(int)*k);
	int i, it_count = 0;
	float mse = 0;
	
	// Select starting cluster centers
	init_f(n_datap, data, k, c_means);
	
	// Initial clusterization
	clusterize(n_datap, data, k, c_means, clusters, distance_f);
	
	while(1){
		// Copy c_means to old_means
		for(i = 0; i < k; i++){
			old_means[i].r = c_means[i].r;
			old_means[i].g = c_means[i].g;
			old_means[i].b = c_means[i].b;
		}
		
		// Update centroids
		update_means(n_datap, data, k, c_means, clusters, data_count, mean_count);
		
		// Increase iteration counter
		it_count++;
		
		// If centroids didn't change, break the loop
		if(means_equal(c_means, old_means, k)) break;
		
		// Reclusterize
		clusterize(n_datap, data, k, c_means, clusters, distance_f);
	}
	
	printf("k-means itered %d times\n", it_count);
	
	mse = get_mse(n_datap, data, c_means, clusters, data_count, distance_f);
	free(old_means);
	free(mean_count);
	return mse;
}
