#include <stdio.h>

#include "pixel.h"

int pixel_cmp(pixel* p1, pixel* p2){
	return p1->r == p2->r && p1->g == p2->g && p1->b == p2->b;
}

void pixel_print(pixel* p){
	printf("[%f, %f, %f]\n", p->r, p->g, p->b);
}

short pixel_uint8_cmp(pixel_uint8 p1, pixel_uint8 p2){
	return p1.r == p2.r && p1.g == p2.g && p1.b == p2.b;
}
