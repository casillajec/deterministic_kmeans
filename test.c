#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mapping.h"
#include "pixel.h"

int aux(pixel_uint8 p){
	printf("%d %d %d\n", p.r, p.g, p.b);
	return 1;
}

int main(){
	unsigned char ps[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	aux((pixel_uint8) {ps[0], ps[1], ps[2]});
	
	return 1;
}
