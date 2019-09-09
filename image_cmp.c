#include <math.h>

#include "distances.h"
#include "pixel.h"


float linear_cmp(unsigned char* img1, unsigned char* img2, unsigned int n_pixels){

	float dif = 0;
	pixel p1, p2;

	for(int i = 0; i < n_pixels; i++){
		p1 = (pixel){img1[(i*3) + 0], img1[(i*3) + 1], img1[(i*3) + 2]};
		p2 = (pixel){img2[(i*3) + 0], img2[(i*3) + 1], img2[(i*3) + 2]};

		dif += rgb_distance(p1, p2);
	}

	dif /= n_pixels;

	return dif;
}

float avg_distance(unsigned int n_datap, pixel_uint8 *unique_data, pixel *c_means, unsigned int *clusters, unsigned int *data_count, unsigned int n_pixels){
	double dif = 0;
	pixel p1_tmp, p2_tmp;
	//unsigned int count = 0;

	for(int i = 0; i < n_datap; i++){
		p1_tmp = (pixel){unique_data[i].r, unique_data[i].g, unique_data[i].b};
		p2_tmp = (pixel){round(c_means[clusters[i]].r), round(c_means[clusters[i]].g), round(c_means[clusters[i]].b)};
		dif += rgb_distance(p1_tmp, p2_tmp)*data_count[i];
		//count += data_count[i];
	}
	//dif /= count;
	dif /= n_pixels;

	return dif;
}
