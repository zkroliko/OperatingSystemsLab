#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int counter = 0;

int PID;

// Zmienna z linii polecen
int argument;

void r_usr1 () {
	counter++;
}

void r_usr2 () {
	printf("Koncowa wartosc licznika to: %d\n", counter);
	printf("Oczekiwana wartosc licznka to: %d\n", argument);
	exit(0);
}

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

	// Najpierw zablokujmy usr1 i usr2
	sigset_t set;
	sigemptyset (&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);	

	// Ustawienie obslugi sygnalow
	if (signal(SIGUSR1, r_usr1) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}
	if (signal(SIGUSR2, r_usr2) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");	
		exit(-1);		
	}

	// Bedziemy forkowac
	if((PID = fork()) < 0) {		
		fprintf(stderr, "Blad funkcji fork!\n");	
		exit(-1);
	} else if (PID == 0) {
		// Potomek
		execl("./child", "child", NULL);
	}

	// Rodzic teraz bedzie wysylal sygnaly

	// Odblokujmy wszystkie sygnaly z wyjatkiem usr2
	sigemptyset (&set);
	sigaddset(&set, SIGUSR2);

	for (int i = 0; i < argument; ++i) {
		if (kill(PID, SIGUSR1) < 0) {
			fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR1!\n");
		}
		
		// Czekanie na potwierdzenie z childa
		sigsuspend(&set);
		
	}

	// Odblokowujemy usr2
	sigdelset(&set, SIGUSR2);

	// Teraz wysylamy usr2
	if (kill(PID, SIGUSR2) < 0) {
		fprintf(stderr, "Blad przy wyslaniu syngalu SIGUSR2!\n");

	}
	// Teraz bedziemy odbierac usr2

	// Czekanie na sygnal usr2
	while (1) {
		 sigsuspend(&set);
	}
}
