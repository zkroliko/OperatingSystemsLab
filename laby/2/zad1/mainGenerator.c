#include "generator.h"

//Ten program opakowuje tylko funkcje generate w argumenty linii polecen
int main(int argc, char* argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Zle argumenty. Prosze podac w formacie <<nazwa pliku> <wielkosc> <liczba>>\n");
		return -1;
	}
	if (atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0) {
		fprintf(stderr, "Zle argumenty. Prosze podac liczby dodatnie!\n");
		return -1;
	}
	generate(argv[1], atoi(argv[2]), atoi(argv[3]));
	return 0;
}
