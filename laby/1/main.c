#include <stdio.h>
#include "list.h"

int main () {
	printf("Ahoj!\n");
	Data* data = Data_new("Andrzej", "Tyczek", "1994-05-03", "Warszawa");
	if (data == NULL) {
		fprintf(stderr, "Bad allocation");
		return 1;
	}
	printf("Ahoj!\n");
	//printf("%s", data->firstName);
	return 0;
}
