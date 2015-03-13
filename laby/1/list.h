#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Data {
	char* firstName;
	char* lastName;
	char* birthDate;
	char* address;
} Data;

Data* Data_new(char* fName, char* lName, char* bDate, char* address);
void Data_del();

typedef struct Node {
	struct Node* next;
	struct Node* prev;
	Data* data;
} Node;

typedef struct List {
	Node* first;
	Node* last;
} List;

	void List_push(List*, Node*);
	Node* List_pop(List*);

	List* List_new();
	void List_del();
