#include <stdio.h>

struct Node {
	struct Node* next;
	struct Node* prev;
	struct Data* data;
};

struct Data {
	char* fistName;
	char* lastName;
	char* birthDate;
	char* adress;
};

struct List {
	struct Node* first;
	struct Node* last;
};

	void List_push(struct List*, struct Node*);
	struct Node* List_pop(struct List*);

	struct List* List_create();
	void List_delete();
