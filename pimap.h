#ifndef __PIMAP
	#define __PIMAP
	#include "pixel.h"

	typedef void* pimap;

	pimap pimap_init();

	void pimap_reset(pimap pi_map);

	int pimap_get(pimap pi_map, pixel_uint8 p);

	void pimap_set(pimap pi_map, pixel_uint8 p, int idx);
#endif
