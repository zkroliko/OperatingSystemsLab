#include "list.h"


	void List_push(struct List* list, struct Node* element) {
		list->last->next = element;
		list->last=element;
	}

	struct Node* List_pop(struct List*){
		struct Node* temp list->last;
		return temp;
	}

	struct List* List_create();
	void List_delete();