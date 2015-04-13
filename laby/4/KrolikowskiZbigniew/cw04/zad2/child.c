#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int counter = 0;

int parent;

int usr2Rec = 0;

void r_usr1 () {
	counter++;
	if (kill(getppid(), SIGUSR1) < 0) {
		fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR1!\n");
	}
}

void r_usr2 () {
	usr2Rec = 1;
}

int main () {	
	parent = getppid();
	
	// Ustawienie obslugi sygnalow
	if (signal(SIGUSR1, r_usr1) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");	
		exit(-1);	
	}
	if (signal(SIGUSR2, r_usr2) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");	
		exit(-1);		
	}

	// Najpierw odblokujmy syngaly
	sigset_t set;
	sigemptyset (&set);

	// Czekania na sygnaly
	while (usr2Rec != 1 ) {
		sigsuspend(&set);
	}

	// Teraz oddajemy sygnal
	if (kill(parent, SIGUSR2) < 0) {
		fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR2!\n");
	}

	return(0);
}
