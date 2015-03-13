#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Data {
	char* firstName;
	char* lastName;
	char* birthDate;
	char* address;
	char* phone;
} Data;

Data* Data_new(char* fName, char* lName, char* bDate, char* address, char* phone);
void Data_del(Data* data);

typedef struct Node {
	struct Node* next;
	struct Node* prev;
	Data* data;
} Node;

typedef struct List {
	Node* first;
	Node* last;
} List;

	void List_push_front(List*, Data*);
	void List_push_back(List*, Data*);

	Data* List_pop_front(List*);
	Data* List_pop_back(List*);

	List* List_new();
	void List_del(List* list);
