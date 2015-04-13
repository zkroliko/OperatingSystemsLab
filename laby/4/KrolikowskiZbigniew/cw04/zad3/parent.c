#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int counter = 0;

int queueFull = 0;

// Zmienna z linii polecen
int argument;

void r_Data () {
	counter++;
}

void r_Confirm () {
	printf("Koncowa wartosc licznika to: %d\n", counter);
	printf("Oczekiwana wartosc licznka to: %d\n", argument);

	if (queueFull) {
		fprintf(stderr, "Przekroczono wymiary kolejki przy wysylaniu do rodzica! Wynik jest nieprawidlowy!\n");
		exit(-1);
	}

	exit(0);
}


// Makra dla ulatwienia

#define Data SIGRTMIN+1 
#define Confirm SIGRTMIN+2

int main (int argc, char* argv[]) {

	// Sprawdzamy argumenty
	if (argc != 2) {
		fprintf(stderr, "Podano bledna liczbe arguentow! Prosze podac <liczba sygnalow>!\n");	
		return (-1);
	}

	// Ustawiamy zmienna argument z linii polecen w postaci inta
	argument = atoi(argv[1]);

	if (argument <= 0 ) {
		fprintf(stderr, "Podano bledny argument. Prosze podac liczbe naturalna!\n");
		return (-1);		
	}
	// Wszystko w porzadku
	

	// Zablokujmy syngaly
	sigset_t set;
	sigemptyset (&set);
	sigaddset(&set, Data);
	sigaddset(&set, Confirm);
	sigprocmask(SIG_BLOCK, &set, NULL);	

	struct sigaction DataAction, ConfirmAction;
	DataAction.sa_handler = r_Data;
	DataAction.sa_mask = set;
	ConfirmAction.sa_handler = r_Confirm;
	ConfirmAction.sa_mask = set;


	// Ustawienie obslugi sygnalow
	if (sigaction(Data, &DataAction, NULL) < 0) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}
	if (sigaction(Confirm, &ConfirmAction, NULL) < 0 ) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");	
		exit(-1);		
	}

	// Bedziemy forkowac
	
	int PID;
	if((PID = fork()) < 0) {		
		fprintf(stderr, "Blad funkcji fork!\n");
		exit(-1);	
	} else if (PID == 0) {
		// Potomek
		execl("./child", "child", NULL);
	}
	// Rodzic teraz bedzie wysylal sygnaly
	union sigval sigVal;

	for (int i = 0; i < atoi(argv[1]); ++i) {
		if (sigqueue(PID, Data, sigVal) < 0) {
			if (errno == EAGAIN ) {
				// Kolejka pelna!
				queueFull = 1;
			} else {				
				fprintf(stderr, "Blad przy wysylaniu syngalu Data!\n");
			}
		}

	}

	// Bedziemy odblokowywac usr2
	sigemptyset (&set);

	// Teraz usr2
	while (sigqueue(PID, Confirm, sigVal) < 0) {
		if (errno == EAGAIN ) {					
			// Kolejka pelna!
			queueFull = 1;
		} else {
			fprintf(stderr, "Blad przy wysylaniu syngalu Confirm!\n");
		}
	}

	// Teraz bedziemy odbierac

	// Czekanie na usr2
	while (1) {
		 sigsuspend(&set);
	}

	return -1;
}
