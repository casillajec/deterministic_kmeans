#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pixel.h"
#include "pimap.h"

pimap pimap_init(){
	pimap pi_map = malloc(256*256*256*3);
	if(!pi_map){
		printf("Out of memmory\n");
		exit(0);
	}
	memset(pi_map, -1, 256*256*256*3);

	return pi_map;
}

void pimap_reset(pimap pi_map){
	memset(pi_map, -1, 256*256*256*3);
}

int pimap_get(pimap pi_map, pixel_uint8 p){
	int ret = 0;
	unsigned int offset = (p.b + p.g*256 + p.r*65536)*3;
	memcpy(&ret, pi_map + offset, 3);

	return ret;
}

void pimap_set(pimap pi_map, pixel_uint8 p, int idx){
	unsigned int offset = (p.b + p.g*256 + p.r*65536)*3;
	memcpy(pi_map + offset, &idx, 3);
}



