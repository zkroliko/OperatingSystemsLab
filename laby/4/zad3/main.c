#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int main(int argc, char* argv[])
{
	// Sprawdzamy argumenty
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "Za duzo argumentow!\n");
		fprintf(stderr, "Prosze podac [-w] <sciezka>!\n");
	}
	if (argc == 2) {	
		// Normalne wywolanie funkcji
		count(argv[1], 0);
		exit(0);
	}
	else if (argc == 3) {
		// Drzewo katalogow
		if (strcmp(argv[1], "-w") == 0) {
			count(argv[2], 1);
			exit(0);
		}
		else {
			fprintf(stderr, "Blad argumentow!\n");
			fprintf(stderr, "Prosze podac [-w] <sciezka>!\n");
			exit(-1);
		}
	}
	
	exit(0);
	
}
