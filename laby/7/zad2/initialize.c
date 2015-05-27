#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int main() {

	// Sprawdzamy ilosc argumentow
	if (argc > 2) 	{
		printf("Podano za duzo argumentow. Prosze uruchomic program w formacie  main [-ilosc pisarzy]\n");
		exit(-1);
	} else {
		if (argc != 2) {
			printf("Podano za duzo argumentow. Prosze uruchomic program w formacie  main [-ilosc pisarzy]\n");
			exit(-1);
		}
	}



	// Tworzymy semafory
	sem_t* empty, *full, *take, *put;

	if ((empty = sem_open(SM_EMPTY, O_CREAT, 0666, 0)) == SEM_FAILED) {
		perror("Nie moge otworzyc semafora SM_EMPTY ");
		exit(-1);
	}
	if ((full = sem_open(SM_FULL, O_CREAT, 0666, MAX_LENGTH)) == SEM_FAILED) {
		perror("Nie moge otworzyc semafora SM_FULL ");
		exit(-1);
	}
	if ((take = sem_open(SM_TAKE, O_CREAT, 0666, 1)) == SEM_FAILED) {
		perror("Nie moge otworzyc semafora do pobierania ");
		exit(-1);
	}
	if ((put = sem_open(SM_PUT, O_CREAT, 0666, 1)) == SEM_FAILED) {
		perror("Nie moge otworzyc semafora do wstawiania ");
		exit(-1);
	}
	
	// Tworzymy pamiec wspoldzielona

	// Deskryptor pliku oraz wskaznik
	int fd, *ptr;

	if ((fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666)) < 0) {
		perror("Blad przy tworzeniu wspoldzielonej pamieci.");
		exit(-1);
	}
	
	// Zmieniamy rozmiar pamieci
	if (ftruncate(fd, (MAX_LENGTH+2)*sizeof(int)) < 0) {
		perror("Blad funkcji ftruncate ");
		exit(-1);
	}
	
	// Mapujemy nasza pamiec na wskaznik
	if ((ptr = (int*) mmap(NULL, (MAX_LENGTH+2)*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("Blad funkcji mmap");
		exit(-1);
	}
	
	// Zamykamy deskryptor
	if (close(fd) < 0) {
		perror("Blad przy zamykaniu pliku ");
		exit(-1);
	}
	
	// Wyczyszczenie pamieci wspoldzielonej

	memset(ptr, 0, MAX_LENGTH+1);
	
	exit(0);
}
