#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Data {
	char* firstName;
	char* lastName;
	char* birthDate;
	char* address;
	char* phone;
	char* email;
} Data;

// For allocating a new data structure
Data* Data_new(char* fName, char* lName, char* bDate, char* address, char* phone, char* email);
// For copying to a new place in memory, old structure is left intact
Data* Data_copy(Data* data);
// For printing the contents
void Data_print(Data* data);
// For comparing, we will use last name for that
int Data_compare(Data* first, Data* second);
// For detecting a string in the structure
unsigned char Data_detect(Data* data, char* string);
// For deallocating
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

	// List length
	unsigned int List_length(List* list);
	// Find element containing a given string 
	Data* List_find(List* list, char* string);
	// For sorting, we will use insert sort in this implementation
	void List_sort(List* list);
	// For printing the whole list
	void List_print(List* list);

	List* List_new();
	void List_del(List* list);

