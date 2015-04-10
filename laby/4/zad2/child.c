#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int counter = 0;

int parent;

int usr2Rec = 0;

int received_conf = 0;

void r_conf () {
	printf("Child tutaj! Dostalem potwierdzenie, wysylam nastepny!\n");
	received_conf = 1;
}

void r_usr1 () {
	counter++;
	printf( "Child tutaj ! Odebralem, wysylam potwierdzenie!\n");
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

	// Najpierw odblokujmy usr1 i usr2
	sigset_t set;
	sigemptyset (&set);

	// Czekania na sygnaly
	while (usr2Rec != 1 ) {
		sigsuspend(&set);
	}

	// Ustawienie obslugi sygnalow na confirmation
	if (signal(SIGUSR1, r_conf) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}

	// Dziecko teraz bedzie wysylal sygnaly
	for (int i = 0; i < counter; ++i) {
		if (kill(parent, SIGUSR1) < 0) {
			fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR1!\n");
		}
		received_conf = 0;

		// Czekanie na potwierdzenie
		while (!received_conf) {
			sigsuspend(&set);
		}	
	}
	// Teraz ostatni
	if (kill(parent, SIGUSR2) < 0) {
		fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR2!\n");
	}

	return(0);
}
