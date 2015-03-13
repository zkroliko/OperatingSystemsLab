#include "list.h"


	struct Data* Data_new(char* fName, char* lName, char* bDate, char* address) {
		// Allocating storage
		fprintf(stderr,"Dupa");
		Data* data = (Data*)malloc(sizeof(Data));
		// Checking if the allocation was correct		
		if (data == NULL) {
			return NULL; // Returning a null to indicate a bad allocation 
		}
		data->firstName = malloc(20 * sizeof(char));
		data->lastName = malloc(30 * sizeof(char));
		data->birthDate = malloc(30 * sizeof(char));
		data->address = malloc(50 * sizeof(char));
		// Checking if the allocation was correct
		if (data->firstName == NULL || data->lastName == NULL || data->birthDate == NULL || data->address == NULL) {
			return NULL; // Returning a null to indicate a bad allocation 
		}
		strcpy(fName, data->firstName);
		strcpy(lName, data->lastName);
		strcpy(bDate, data->birthDate);
		strcpy(address, data->address);
		return data;
	}


	void Data_del();	
	
	void List_push(struct List* list, struct Node* element) {
		list->last->next = element;
		list->last=element;
	}

/*

	struct Node* List_pop(struct List*){
		struct Node* temp list->last;
		return temp;
	}
*/
/*
	struct List* List_new() {
	}
	void List_del() {
	}
*/
