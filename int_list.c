#include <stdlib.h>
#include <stdio.h>

#include "int_list.h"

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
