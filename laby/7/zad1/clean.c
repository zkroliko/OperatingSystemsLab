#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int main() {
	// Klucz 
	key_t key;
	// 
	int semset;
	int shm;

	// Tworzymy klucz
	if ((key = ftok(SM_NAME, 0)) < 0) {
		perror("Blad przy tworzeniu klucza dla semaforow \n");
		exit(-1);
	}
	
	// Otwieramy semafory
	if ((semset = semget(key, 4, 0666 | IPC_CREAT)) < 0) {
		perror("Blad przy otwieraniu semaforow \n");
		exit(-1);
	}

	// Usuwamy semafory
	
	if (semctl(semset, 0, IPC_RMID) < 0) {
		perror("Blad funcji semctl przy usuwaniu semaforow\n");
		exit(-1);
	}

	// Tworzymy klucz wspolnej pamieci
	
	if ((key = ftok(SHM_NAME, 0)) < 0) {
		perror("Blad tworzenia klucza przy pomocy funkcji ftok\n");
		exit(-1);
	}

	// Otwieramy pamiec wspolna	

	if ((shm = shmget( key, ( BUFFER_SIZE + 2)*sizeof(int), 0666)) < 0) {
		perror("Blad przy otwieraniu wspolnej pamieci\n");
		exit(-1);
	}
	
	// Usuwamy wspolna pamiec

	if (shmctl(shm, IPC_RMID, NULL) < 0) {
		perror("Blad przy usuwaniu wspolnej pamieci");
		exit(-1);
	}
	exit(0);
}
