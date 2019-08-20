#ifndef __INT_LIST
	#define __INT_LIST

	typedef struct int_list{
		int el;
		struct int_list* next;
	} int_list;

	int int_list_add_front(int_list** list, int el);

	void int_list_print(int_list* list);

	int int_list_length(int_list* list);

	void int_list_free(int_list** list);

	int int_list_contains(int_list* list, int el);
#endif
