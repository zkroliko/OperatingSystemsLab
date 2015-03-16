#include <stdio.h>
#include "list.h"

int main () {
	Data* data1 = Data_new("Andrzej", "Wolbrom", "1994-05-03", "Warszawa, Konopnicka 3", "4566123-34", "Andrzej@tyczek.com");
	Data* data2 = Data_new("Jacek", "Placek", "1983-09-03", "Łodz, Rycerska 5", "9563213-34", "halastra@gumail.com");
	Data* data3 = Data_new("Anna", "Anusinska", "1991-01-08", "Szczecin, Morska 6", "4544221-44", "anusia@xumail.com");
	Data* data4 = Data_new("Teodor", "Tadorski", "1989-05-03", "Koszalow", "11329217-34", "lodzie@podwodne.com");
	Data* data5 = Data_new("Konrad", "Bartus", "1964-11-03", "Tczyny, Wiejska 12", "465653-14", "tczyny@wujtix.com");

	if (data1 == NULL || data2 == NULL || data3 == NULL || data4 == NULL || data5 == NULL) {
		fprintf(stderr, "Bad allocation");
		return 1;
	}
	//printf("%d", Data_detect(data, "Warszawa"));

	List* list = List_new();
	List_push_back(list, data1);
	List_push_back(list, data2);
	List_push_back(list, data3);
	List_push_back(list, data4);
	List_push_back(list, data5);

	List_print(list);
	List_sort(list);
	printf("%d\n", List_length(list));
	List_print(list);

	printf("%d\n", List_length(list));
	//Data_print(List_find(list, "Koszalow"));
	printf("%d\n", Data_compare(data2,data5));
	

	Data_del(data1);
	return 0;
}
