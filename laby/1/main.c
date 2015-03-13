#include <stdio.h>
#include "list.h"

int main () {
	Data* data = Data_new("Andrzej", "Tyczek", "1994-05-03", "Warszawa", "4566123-34");
	if (data == NULL) {
		fprintf(stderr, "Bad allocation");
		return 1;
	}
	printf("%s\n", data->firstName);
	printf("%s\n", data->phone);

	List* list = List_new();
	List_push_front(list, data);
	List_push_front(list, data);
	List_push_front(list, data);
	Data* data2 = List_pop_front(list);
	Data* data3 = List_pop_front(list);
	Data* data4 = List_pop_back(list);

	printf("%s\n", data4->firstName);
	printf("%s\n", data4->phone);


	Data_del(data);
	return 0;
}
