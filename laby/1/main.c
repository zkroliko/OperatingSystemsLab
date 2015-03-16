#include <stdio.h>
#include "list.h"

int main () {
	Data* data = Data_new("Andrzej", "Tyczek", "1994-05-03", "Warszawa", "4566123-34", "Andrzej@tyczek.com");
	if (data == NULL) {
		fprintf(stderr, "Bad allocation");
		return 1;
	}
	Data_print(data);
	//printf("%d", Data_detect(data, "Warszawa"));

	List* list = List_new();
	List_push_front(list, data);
	List_push_front(list, data);
	List_push_front(list, data);
	List_push_front(list, data);
	List_push_front(list, data);
	printf("%d\n", List_length(list));
	Data_print(List_find(list, "Andrzej"));
	Data* data2 = List_pop_front(list);
	Data* data3 = List_pop_front(list);
	Data* data4 = List_pop_back(list);

	Data_print(data4);

	Data_del(data);
	return 0;
}
