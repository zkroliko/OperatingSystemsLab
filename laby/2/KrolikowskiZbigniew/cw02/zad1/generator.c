#include "generator.h"

int generate(char* filePath, int size, int count) {

	srand(time(NULL)); // Inicjalizacja generatora losowego
		
	FILE* file = fopen(filePath,"w");
	if(!file) {
		fprintf(stderr, "Blad przy otwarciu pliku!\n");
		return -1;
	}

	char* buffer = malloc(size*sizeof(char));

	if (buffer == NULL) {
		fprintf(stderr, "Zla alokacja w generatorze!\n");
		exit(2);
	}

	size_t written;

	for (int i = 0; i < count; i++) {	
		// Generowanie rekordu do bufora	
		for (int j =0; j < size; j++) {
			buffer[j]=(char)rand();		
		}
		written = fwrite(buffer,sizeof(char),size,file);
		if (written != size) {
			fprintf(stderr, "Blas zapisu w generatorze!\n");
			exit(2);
		}
	}
	
	free(buffer);
	fclose(file);

	return 0;
}
