#include "list.h"

	// For creating a data structure
	Data* Data_new(char* fName, char* lName, char* bDate, char* address, char* phone, char* email) {
		// Allocating storage
		Data* data = (Data*)malloc(sizeof(Data));
		// Checking if the allocation was correct		
		if (data == NULL) {
			return NULL; // Returning a null to indicate a bad allocation 
		}
		data->firstName = malloc(20 * sizeof(char));
		data->lastName = malloc(30 * sizeof(char));
		data->birthDate = malloc(30 * sizeof(char));
		data->address = malloc(50 * sizeof(char));
		data->phone = malloc(15 * sizeof(char));
		data->email = malloc(30 * sizeof(char));
		// Checking if the allocation was correct
		if (data->firstName == NULL || data->lastName == NULL || data->birthDate == NULL || data->address == NULL) {
			return NULL; // Returning a null to indicate a bad allocation 
		}
		strcpy(data->firstName, fName);
		strcpy(data->lastName, lName);
		strcpy(data->birthDate, bDate);
		strcpy(data->address, address);
		strcpy(data->phone, phone);
		strcpy(data->email, email);
		return data;
	}

	// For copying Data structure
	Data* Data_copy(Data* original) {
		Data * clone = (Data*)malloc(sizeof(Data));
		clone->firstName = malloc(20 * sizeof(char));
		clone->lastName = malloc(30 * sizeof(char));
		clone->birthDate = malloc(30 * sizeof(char));
		clone->address = malloc(50 * sizeof(char));
		clone->phone = malloc(15 * sizeof(char));
		clone->email = malloc(30 * sizeof(char));
		strcpy(clone->firstName,original->firstName);
		strcpy(clone->lastName,original->lastName);
		strcpy(clone->birthDate,original->birthDate);
		strcpy(clone->address,original->address);
		strcpy(clone->phone,original->phone);
		strcpy(clone->email,original->email);
		return clone;
	}

	// For wrting out a data structure
	void Data_print(Data* data) {
		printf("%s ", data->firstName);
		printf("%s ", data->lastName);
		printf("%s ", data->birthDate);
		printf("%s ", data->address);
		printf("%s ", data->phone);
		printf("%s\n", data->email);
	}

	// For detecting a string in the structure
	unsigned char Data_detect(Data* data, char* string) {
		// I any value is the same as string we will call it a success
		if (strcmp(data->firstName, string) == 0 ||
			strcmp(data->lastName, string) == 0 ||
			strcmp(data->birthDate, string) == 0 ||
			strcmp(data->address, string) == 0 ||
			strcmp(data->phone, string) == 0 ||
			strcmp(data->email, string) == 0
		) {
			return 1;		
		}
		return 0;
		
	}

	// For comparing, we will use last name for that
	int Data_compare(Data* first, Data* second) {
		return strcmp(first->lastName,second->lastName);
	}

	// For removing a data structure from memory
	void Data_del(Data* data) {
		free(data->firstName);
		free(data->lastName);
		free(data->birthDate);
		free(data->address);
	}	
	
	// For pushing a data element onto a list
	void List_push_back(List* list, Data* data) {
		Node* element = (Node*)malloc(sizeof(Node));
		element->data = Data_copy(data); // Copying data
		// Checking if the list isn't empty
		if (list->first == NULL) {
			list-> first = element;
			list-> last = element;
			return;		}
		// If we just initialized a list then this will work properly
		element->prev = list->last;
		element->next = list->first;
		// Putting the new element at the end of a list
		list->last->next = element;
		list->first->prev = element;
		list->last=element;
	}

	void List_push_front(List* list, Data* data) {
		Node* element = (Node*)malloc(sizeof(Node));
		element->data = Data_copy(data); // Copying data 
		// Checking if the list isn't empty
		if (list->first == NULL) {
			list-> first = element;
			list-> last = element;
		}
		// If we just initialized a list then this will work properly
		element->prev = list->last;
		element->next = list->first;
		// Putting the new element at the front of a list
		list->first->prev = element;
		list->last->next = element;
		list->first=element;
	}


	// For poping an element from the front of a list
	Data* List_pop_front(List* list){
		// Checking if the list isn't empty
		if (list->first == NULL) {
			return NULL; // The list is empty
		}
		// Making a temporaty node
		Node* temp = list->first;
		// Knitting the empty place in the chain
		if (list->first == list->last) {
			// If the list contains only one element
			list->first = NULL;
			list->last = NULL;
		} else {
			// If it contains many elements
			list->first = list->first->next;
			list->first->prev = list->last;
			list->last->next = list->first;
		}
		
		// Extracting data
		Data* data = temp->data;
		// Freeing the node from memory
		free(temp);
		return data;
	}

	// For poping an element from the back of a list
	Data* List_pop_back(List* list){
		// Checking if the list isn't empty
		if (list->first == NULL) {
			return NULL; // The list is empty
		}
		// Making a temporaty node
		Node* temp = list->last;
		// Knitting the empty place in the chain
		if (list->first == list->last) {
			// If the list contains only one element
			list->first = NULL;
			list->last = NULL;
		} else {
			// If it contains many elements
			list->last = list->last->prev;
			list->last->next = list->first;
			list->first->prev = list->last;	
		}
	
		// Extracting data
		Data* data = temp->data;
		// Freeing the node from memory
		free(temp);
		return data;
	}

	// For creating a new list
	List* List_new() {
		List* list = (List*)malloc(sizeof(List));
		return list;
	}

	// List length
	unsigned int List_length(List* list) {
		// Checking if the list isn't empty		
		if (list->first == NULL) {
			return 0;		
		}	
		unsigned int counter = 0;
		Node* temp = list->first;
		do {
			counter++;
			temp = temp->next;					
		} while (temp != list->first);
		return counter;
	}

	// Find element containing a given string 
	Data* List_find(List* list, char* string) {
		// Checking if the list isn't empty
		if (list->first == NULL) {
			return NULL;
		}
		Node* temp = list->first;
		while ( Data_detect(temp->data, string) != 1 && temp->next != list->first) {
			temp = temp->next;
		}
		// In case we haven't found anything
		if (temp->next == list->first) {
			Data* none = Data_new("Unknown","Unknown","Unknown","Unknown","Unknown","Unknown");	
			return none;	
		}		
		return temp->data;
	}

	// For sorting, we will use insert sort in this implementation
	void List_sort(List* list) {
		// If it contains 0 or 1 elements then it's already sorted
		if (list->first == list->last || list->first == NULL) {
			return;
		}
		Node temp;// This one is only temporary
		Node* cur_element = list->first->next;
		Node* consd_pos = cur_element->prev;
		// Now let iterate throught every element finding a place for it
		do {
			while (Data_compare(cur_element->data, consd_pos->data) > 0 && (consd_pos != list->first)) {
				fprintf(stderr, "Moving!");
				consd_pos = consd_pos->prev;				
			}
			if (consd_pos == cur_element) {
				cur_element = cur_element->next;
				consd_pos = cur_element;
				continue;	
			}
			fprintf(stderr, "Change!");
			// Now let's insert it
			// First let's check for some special situations
			if (cur_element == list->last) {
				list->last = list->last->prev;
			}		
			// We place any element in place of a first
			if (consd_pos == list->first) {
				list->first = cur_element;
			}	
			// The acctual exchange
			// The temporary next and prev for the element being inserted
			temp.prev = cur_element->prev;
			temp.next = cur_element->next;
			temp.next->prev = temp.prev;
			temp.prev->next = temp.next;
			// We dislocated this node
			// Now let's place it BEFORE our considered position
			// and fit the connections properly ---EC----- E is our element
			cur_element->prev = consd_pos->prev;
			cur_element->next = consd_pos;
			cur_element->prev->next = cur_element;	
			consd_pos->prev = cur_element;					
			fprintf(stderr, "Bump! \n");
			// Iterating to next element		
			cur_element = temp.next;
			consd_pos = cur_element->prev;
		} while(cur_element != list->first);
			fprintf(stderr, "Done");
	}

	// For printing the whole list
	void List_print(List* list) {
		// Checking whether the list contains any elements
		if (list->first == NULL) {
			return;
		}
		Node* temp = list->first;
		do {
			Data_print(temp->data);
			temp = temp->next;
		} while (temp != list->first);

	}

	// For removing a list, first all the element then the list itself
	void List_del(List* list) {
		// Checking if the list contains any elements
		if (list->first != NULL) {
			Node* temp;
			temp = list->first;
			// Now let's delete all the nodes and all the data structures in them
			while (temp->next != temp) {
				free(temp->data);
				temp = temp->next;
				free(temp->prev);
			}
		}
		// Now let's delete the list itself
		free(list);
	}

