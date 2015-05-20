#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int main() {
	if(sem_unlink(SM_EMPTY) < 0) {
		perror("Blad przy zamykaniu semafora empty");
	}
	if(sem_unlink(SM_FULL) < 0) {
		perror("Blad przy zamykaniu semafora full");
	}
	if(sem_unlink(SM_TAKE) < 0) {
		perror("Blad przy zamykaniu semafora take");
	}
	if(sem_unlink(SM_PUT) < 0) {
		perror("Blad przy zamykaniu semafora put");
	}
	exit(0);
}
