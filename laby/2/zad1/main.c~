#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/times.h>

#include "generator.h"

#define FILENAME "rekordy"

#define size1 1
#define count1 10

// Porownujemy dwa rekordy
unsigned char compare(char* first, char* second) {
	if ((unsigned int)first[0] > (unsigned int)second[0]) {
		return 1;	
	}
	return 0;
}

//Wypisuje klucze
void printKeys(int size, int count) {

	FILE* file = fopen(FILENAME,"rb");
	if (!file) {
		fprintf(stderr, "Blad przy otwarciu pliku!\n");
		exit(2);
	}

	char unsigned* key = malloc(sizeof(char));

	size_t read;

	for (int i = 0; i < count; i++) {
		read = fread(key,sizeof(char),1,file);
		if (read != 1) {
			fprintf(stderr, "Zly odczyt z pliku w wypisywaniu kluczy!\n");
			exit(2);
		}
		printf("%u ",*key);
		fseek(file,size-1,SEEK_CUR); // -1 poniewaz funkcja fread juz nas przesunela
	}
	printf("\n");
	fclose(file);
}

void testStdio(int size, int count) {

	FILE* file = fopen(FILENAME,"rw+");

	if (!file) {
		fprintf(stderr, "Blad przy otwarciu pliku.\n");
		exit(2);
	}

	char* first = malloc(size*sizeof(char));
	char* second = malloc(size*sizeof(char));

	if (first == NULL || second == NULL) {
		fprintf(stderr, "Zla alokacja w tescie.\n");
		exit(2);
	}

	size_t readFirst;
	size_t readSecond;
	size_t written;

	for (int i = 0; i < count -1; i++) {
		// Przesuwamy sie na odpowiednia pozycje w pliku
		fseek(file,0,SEEK_SET);
		for( int j = 0 ; j < count - i -1; j++) {
			readFirst  = fread(first,sizeof(char),size,file);
			readSecond = fread(second,sizeof(char),size,file);
			// Sprawdzamy poprawnosc odczytu
			if (readFirst != size || readSecond != size) {
				fprintf(stderr, "Zly odczyt z pliku w tescie z stdio.\n");
				exit(2);
			}
			if (compare(first,second) != 0) {
				// Jezeli sa w odwrotnej kolejnosci to zapisujemy je w pliku w prawidlowej
				fseek(file,-2*size,SEEK_CUR);
				written = fwrite(second,sizeof(char),size,file);
				written += fwrite(first,sizeof(char),size,file);
				if (written != size*2) {
					fprintf(stderr, "Blas zapisu stdio w sortowaniu!\n");
					exit(2);
				}
			}
			// Przesuwamy sie o jedna pozycje do tylu w pliku
			fseek(file,-size,SEEK_CUR);

			// DEBUGprintKeys(size,count);
		}
	}	

	fclose(file);
	free(first);
	free(second);
}

void testUnistd(int size, int count) {

	int file = open(FILENAME,1);

	if (file < 0) {
		fprintf(stderr, "Blad przy otwarciu pliku.\n");
		exit(2);
	}

	void* first = malloc(size*sizeof(char));
	void* second = malloc(size*sizeof(char));

	if (first == NULL || second == NULL) {
		fprintf(stderr, "Zla alokacja w tescie.\n");
		exit(2);
	}


	size_t readFirst;
	size_t readSecond;
	size_t written;

	for (int i = 0; i < count -1; i++) {
		// Przesuwamy sie na odpowiednia pozycje w pliku
		lseek(file,0,SEEK_SET);
		
		for( int j = 0 ; j < count - i -1; j++) {
			readFirst  = read(file, first,size);
			readSecond = read(file, second,size);
			
			if (readFirst != size || readSecond != size) {
				printf("%d %d\n", readFirst, readSecond);
				fprintf(stderr, "Zly odczyt z pliku w tescie z unistd!\n");
				exit(2);
			}
			/*
			if (compare(first,second) != 0) {
				// Jezeli sa w odwrotnej kolejnosci to zapisujemy je w pliku w prawidlowej
				fseek(file,-2*size,SEEK_CUR);
				written = fwrite(second,sizeof(char),size,file);
				written += fwrite(first,sizeof(char),size,file);
				if (written != size*2) {
					fprintf(stderr, "Blas zapisu stdio w sortowaniu!\n");
					exit(2);
				}
			}
			*/
			// Przesuwamy sie o jedna pozycje do tylu w pliku
			lseek(file,-size,SEEK_CUR);

			// DEBUGprintKeys(size,count);
			
		}


	}

	close(file);
	free(first);
	free(second);
}

int main() {
	if (generate(FILENAME,size1,count1) >= 0) {
		printKeys(size1, count1);
		testStdio(size1, count1);
		testUnistd(size1, count1);
		printKeys(size1, count1);
	}
}


