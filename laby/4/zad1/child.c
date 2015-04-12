#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int counter = 0;

int usr2Rec = 0;

void r_usr1 () {
	counter++;
}

void r_usr2 () {
	usr2Rec = 1;
}

int main () {	

	// Maska sygnalow
	sigset_t set;
	sigemptyset (&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);

	// Ustawienie obslugi sygnalow

	struct sigaction usr1Action, usr2Action;
	usr1Action.sa_handler = r_usr1;
	usr1Action.sa_mask = set;
	usr2Action.sa_handler = r_usr2;
	usr2Action.sa_mask = set;
	
	if (sigaction(SIGUSR1, &usr1Action, NULL) < 0) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}
	if (sigaction(SIGUSR2, &usr2Action, NULL) < 0 ) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");	
		exit(-1);		
	}

	// Odblokowanie	

	sigemptyset (&set);

	// Czekania na sygnaly
	while (usr2Rec != 1 ) {
		sigsuspend(&set);
	}

	int parent = getppid();

	// Dziecko teraz bedzie wysylal sygnaly
	for (int i = 0; i < counter; ++i) {
		if (kill(parent, SIGUSR1) < 0) {
			fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR1!\n");
		}
	}
	// Teraz ostatni
	if (kill(parent, SIGUSR2) < 0) {
		fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR2!\n");
	}

	return(0);
}
