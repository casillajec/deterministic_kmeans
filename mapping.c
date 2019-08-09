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
	if( *list != NULL ){
		int_list *_free, *walk;
		_free = (*list);
		do{
			walk = _free->next;
			free(_free);
			_free = walk;
		}while(walk != NULL);
		*list = NULL;
	}
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
mapping* mapping_init(pixel_uint8* p_ptr, unsigned int unique_index){
	mapping* tmp = (mapping*) malloc(sizeof(mapping));

	if(!tmp){
		printf("Out of memmory while mallocing mapping\n");
		exit(0);
	}

	tmp->p = (pixel_uint8){p_ptr->r, p_ptr->g, p_ptr->b};
	tmp->unique_index = unique_index;
	tmp->data_count = 0;

	return tmp;
}

int mapping_update(mapping** map, pixel_uint8* p_ptr, int idx){

	mapping* aux = NULL;
	HASH_FIND(hh, *map, p_ptr, sizeof(pixel_uint8), aux);

	if(aux == NULL){
		aux = mapping_init(p_ptr, HASH_COUNT(*map));
		HASH_ADD(hh, *map, p, sizeof(pixel_uint8), aux);
	}
	(aux->data_count)++;
}


void mapping_print(mapping* map){
	mapping* walk = map;
	while(walk != NULL){
		printf("[%d %d %d] -> [", walk->p.r, walk->p.g, walk->p.b);
		walk = walk->hh.next;
	}
}

void mapping_free(mapping** map){
	mapping *_free, *walk;
	HASH_ITER(hh, *map, _free, walk){
		HASH_DEL(*map, _free);
		free(_free);
	}
}



