#include "pixel.h"
#include "mapping.h"


typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} uint8_pixel;

typedef struct{
	int length;
	unsigned char* components;
} uint8_datap;

typedef struct{
	int length;
	float* comp;
} float_datap;


float k_means(int n_datap, pixel_uint8* data, int* data_count, int k, float (*distance_f)(pixel, pixel), void (*init_f)(int, pixel_uint8*, int, pixel*), int* clusters, pixel* c_means);

float euclidean_distance(pixel* p1, pixel* p2);

float rgb_distance(pixel p1, pixel p2);

void random_init(int n_datap, pixel_uint8* data, int k, pixel* c_means);

int pixel_cmp(pixel* p1, pixel* p2);
