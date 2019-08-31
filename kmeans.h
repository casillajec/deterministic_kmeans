#include "pixel.h"


float k_means(int n_datap, pixel_uint8* data, int* data_count, int k, float (*distance_f)(pixel, pixel), void (*init_f)(int, pixel_uint8*, int, pixel*), int* clusters, pixel* c_means);
