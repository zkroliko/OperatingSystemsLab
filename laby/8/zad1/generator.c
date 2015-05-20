#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

#define FILENAME "records"


// Program zapisuje podana liczbe rekordow postaci <<int> <[a-z]*>>
int main(int argc, char** argv) {

	// Sprawdzamy argumenty linii polecen
	if (argc != 3)	{
		perror("Blednie wywolano program. Prosze wpisac generator <liczba rekordow> <rozmiar>");
		exit(-1);
	}

	if (atoi(argv[1]) == 0 || atoi(argv[2]) == 0) {
		perror("Liczba rekordow oraz rozmiar musza byc liczbami naturalnym wiekszymi od zera.");
		exit(-1);
	}

	int fd;
	int count;
	int size;
	
	// Kopiujemy argumemty programu
	count = atoi(argv[1]);
	size = atoi(argv[2]);
	
	// Otwieramy plik
	if ((fd = open(FILENAME, O_WRONLY | O_TRUNC | O_CREAT , S_IWUSR| S_IRUSR )) < 0)	{
		perror("Blad otwarcia pliku");
		exit(-1);
	}

	// Inicjzalizujemy generator liczb losowych
	srand(time(NULL));
	
	// Tworzymy bufor
	char buf[size];

	for (int j = 0; j < count ; j++) {
		// Liczba
		if(write(fd, &j, 4) < 0) {
			perror("Blad przy funkcji write");
			exit(-1);
		}
		
		// Ciag znakow
		for(int i = 0; i < (size - sizeof(int)); i++) {
			buf[i] = (rand()%26)+97;
		}
		
		if(write(fd, buf, size - sizeof(int)) < 0) {
			perror("Blad przy funkcji write");
			exit(-1);
		}		
	}
	
	return 0;	
}
