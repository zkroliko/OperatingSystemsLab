#include "functions.h"

//Ten program opakowuje tylko funkcje sortowania z uzyciem funkcji bibliotecznych w argumenty linii polecen
int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Zle argumenty. Prosze podac w formacie <<nazwa pliku> <wielkosc> <liczba>>\n");
		return -1;
	}
	if (atoi(argv[2]) <= 0) {
		fprintf(stderr, "Zle argumenty. Prosze podac liczbe dodatnia!\n");
		return -1;
	}
	sortLib(argv[1], atoi(argv[2]));
	return 0;
}
