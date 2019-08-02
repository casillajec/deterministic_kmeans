#ifndef __MAPPING
	#define __MAPPING
	#include "uthash.h"
	#include "pixel.h"

	// INT LIST
	typedef struct int_list{
		int el;
		struct int_list* next;
	} int_list;
	
	int int_list_add_front(int_list** list, int el);
	
	void int_list_print(int_list* list);
	
	int int_list_length(int_list* list);
	
	void int_list_free(int_list** list);
	
	// MAPPING
	typedef struct{
		//pixel p;
		pixel_uint8 p;
		int_list* indices;
		UT_hash_handle hh;
	} mapping;
	
	//int mapping_update(mapping** map, pixel p, int idx);
	int mapping_update(mapping** map, pixel_uint8 p, int idx);
	
	void mapping_print(mapping* map);
	
	void mapping_free(mapping** map);
#endif
