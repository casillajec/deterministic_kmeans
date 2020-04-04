#include <stdlib.h>

#include "initializers.h"
#include "int_list.h"


void random_init(int n_datap, pixel_uint8* data, int k, pixel* c_means){
	// Seed rng
	//srand(time(0));
	srand(0);

	// Initalize strucures and first random idx
	int_list* seen = NULL;
	int ridx;

	// Find different random indices
	for(int i = 0; i < k; i++){;
		ridx = rand()%n_datap;

		// Generate new idx until an unseen is generated
		while(int_list_contains(seen, ridx)){ ridx = rand()%n_datap; }

		c_means[i] = (pixel){data[ridx].r, data[ridx].g, data[ridx].b};
		int_list_add_front(&seen, ridx);
	}

	// Free the allocated structures
	int_list_free(&seen);
}

void mode_init(int n_datap, pixel_uint8* data, int k, pixel* c_means){

}
