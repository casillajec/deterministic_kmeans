#include "distances.h"
#include "pixel.h"
#include <math.h>

float euclidean_distance(pixel p1, pixel p2){
	return sqrt( pow(p2.r - p1.r, 2) + pow(p2.g - p1.g, 2) + pow(p2.b - p1.b, 2) );
}


float rgb_distance(pixel p1, pixel p2){
	float r, s[3], px[3];
	r = (p1.r + p2.r)/2;
	s[0] = (2+(r/256)); s[1] = 4; s[2] = ((2+(255-r))/256);
	px[0] = p1.r - p2.r; px[1] =  p1.g - p2.g; px[2] = p1.b - p2.b;
	return sqrt(s[0]*px[0]*px[0] + s[1]*px[1]*px[1] + s[2]*px[2]*px[2]);
}
