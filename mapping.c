#include <stdlib.h>
#include <stdio.h>

#include "pixel.h"
#include "mapping.h"

// Aux functions for int_list
int int_list_add_front(int_list** list, int el){
	int_list* new_el = malloc(sizeof(int_list));
	
	new_el->el = el;
	new_el->next = *list;
	
	*list = new_el;
	
	return 1;
}

void int_list_print(int_list* list){
	printf("[");
	int_list* walk = list;
	while(walk != NULL){
		printf("%d, ", walk->el);
		walk = walk->next;
	}
	printf("\b\b]\n");
}

int int_list_length(int_list* list){
	int length = 0;
	int_list* walk = list;
	while(walk != NULL){
		length++;
		walk = walk->next;
	}
	
	return length;
}

void int_list_free(int_list** list){
	int_list *_free, *walk;
	_free = (*list);
	do{
		walk = _free->next;
		free(_free);
		_free = walk;
	}while(walk != NULL);
	*list = NULL;
}

int int_list_contains(int_list* list, int el){
	int_list* walk = list;
	
	while(walk != NULL){
		if(walk->el == el){ return 1; }
		walk = walk->next;
	}
	
	return 0;
}

// Mapping functions
//int mapping_update(mapping** map, pixel p, int idx){
int mapping_update(mapping** map, pixel_uint8 p, int idx){

	mapping* aux = NULL;
	//HASH_FIND(hh, *map, &p, sizeof(pixel), aux);
	HASH_FIND(hh, *map, &p, sizeof(pixel_uint8), aux);
	
	if(aux == NULL){
		aux = (mapping*) malloc(sizeof(mapping));
		aux->p = p;
		aux->indices = NULL;
		//HASH_ADD(hh, *map, p, sizeof(pixel), aux);
		HASH_ADD(hh, *map, p, sizeof(pixel_uint8), aux);
	}
	int_list_add_front(&(aux->indices), idx);
}


void mapping_print(mapping* map){
	mapping* walk = map;
	int_list* idx_walk;
	while(walk != NULL){
		//printf("[%f %f %f] -> [", walk->p.r, walk->p.g, walk->p.b);
		printf("[%d %d %d] -> [", walk->p.r, walk->p.g, walk->p.b);
		idx_walk = walk->indices;
		while(idx_walk != NULL){
			printf("%d, ", idx_walk->el);
			idx_walk = idx_walk->next;
		}
		printf("]\n");
		walk = walk->hh.next;
	}
}

void mapping_free(mapping** map){
	mapping *_free, *walk;
	HASH_ITER(hh, *map, _free, walk){
		HASH_DEL(*map, _free);
		int_list_free(&(_free->indices));
		free(_free);
	}
}



