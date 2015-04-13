#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int counter = 0;

int confirmRec = 0;

int queueFull = 0;

void r_Data () {
	counter++;
}

void r_Confirm () {
	confirmRec = 1;
}

// Makra dla ulatwienia

#define Data SIGRTMIN+1 
#define Confirm SIGRTMIN+2

int main () {	

	// Ustawienie obslugi i odblokowanie sygnalow
	sigset_t set;
	sigemptyset (&set);
	sigaddset(&set, Data);
	sigaddset(&set, Confirm);
	// Poki co blokujemy syngaly
	sigprocmask(SIG_BLOCK, &set, NULL);


	// Ustawienie obslugi sygnalow
	struct sigaction DataAction, ConfirmAction;
	DataAction.sa_handler = r_Data;
	DataAction.sa_mask = set;
	ConfirmAction.sa_handler = r_Confirm;
	ConfirmAction.sa_mask = set;
	
	if (sigaction(Data, &DataAction, NULL) < 0) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}
	if (sigaction(Confirm, &ConfirmAction, NULL) < 0) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");	
		exit(-1);		
	}

	// Odblokujmy syngaly
	sigemptyset (&set);

	// Czekanie na sygnaly
	while (confirmRec != 1 ) {
		sigsuspend(&set);
	}

	int parent = getppid();

	// Dziecko teraz bedzie wysylal sygnaly
	union sigval sigVal;

	for (int i = 0; i < counter; ++i) {
		if (sigqueue(parent, Data, sigVal) < 0) {
			if (errno == EAGAIN ) {
				// Kolejka pelna! 
				queueFull = 1;
			} else {				
				fprintf(stderr, "Blad przy wysylaniu syngalu Data!\n");
			}
		}
	}
	// Teraz ostatni
	while (sigqueue(parent, Confirm, sigVal) < 0) {		
		if (errno == EAGAIN ) {					
			// Kolejka pelna!
			queueFull = 1;
		} else {
			fprintf(stderr, "Blad przy wysylaniu syngalu Confirm!\n");
		}
	}

	if (queueFull) {
		fprintf(stderr, "Przekroczono wymiary kolejki przy wysylaniu do dziecka! Wynik bedzie nieprawidlowy!\n");
	}

	return(0);
}
