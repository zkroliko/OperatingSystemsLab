#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int main() {

	int semset;
	
	// Deskryptor pliku
	int fd;
	// Wskaznik na obszar pamieci
	int* ptr; 	
	
	// Klucz
	key_t key;
	
	// Tworzymy klucz
	if ((key = ftok(SHM_NAME, 0)) < 0) {
		perror("Blad tworzenia klucza przy pomocy funkcji ftok\n");
		exit(-1);
	}
	
	// Tworzymy pamiec wspoldzielona
	if ((fd = shmget(key, (BUFFER_SIZE+2)*sizeof(int), 0666 | IPC_CREAT)) < 0)	{
		perror("Blad tworzenia pamieci wspoldzielonej.\n");
		exit(-1);
	}
	
	// Mapujemy
	if ((ptr = (int*) shmat(fd, NULL, 0)) < 0) {
		perror("Blad przy mapowaniu pamieci wspoldzielonej \n");
		exit(-1);
	}
	
	// Przygotowujemy bufor
	for (int i = 0; i <= (BUFFER_SIZE + 1); i++) {
		ptr[i] = 0;
	}

	// Tworzenie semaforow

	if ((key = ftok(SM_NAME, 0)) < 0) {
		perror("Blad przy tworzeniu klucza dla semaforow \n");
		exit(-1);
	}
	
	if ((semset = semget(key, 4, 0666|IPC_CREAT)) < 0) {
		perror("Blad przy tworzeniu semaforow\n");
		exit(-1);
	}

	// Ustawienie semaforow
	
	union semafore arg;
	
	arg.val = BUFFER_SIZE;
	if (semctl(semset, FULL, SETVAL, arg) < 0)	{
		perror("Blad przy funkcji semctl \n");
		exit(-1);
	}
	arg.val = 0;
	if (semctl(semset, EMPTY, SETVAL, arg) < 0) {
		perror("Blad przy funkcji semctl \n");
		exit(-1);
	}
	arg.val = 1;
	if (semctl(semset, PUT, SETVAL, arg) < 0)	{
		perror("Blad przy funkcji semctl \n");
		exit(-1);
	}
	arg.val = 1;
	if (semctl(semset, TAKE, SETVAL, arg) < 0)	{
		perror("Blad przy funkcji semctl \n");
		exit(-1);
	}
	
	
	exit(0);
}
