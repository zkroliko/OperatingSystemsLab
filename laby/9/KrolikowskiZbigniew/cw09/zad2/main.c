#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

// Zmienne globalne

pthread_t writerTable[MAX_N_WRITERS];
pthread_t readerTable[N_READERS];

int nWriters;

int main(int argc, char** argv) {

	// Sprawdzamy ilosc argumentow
	if (argc > 2) 	{
		printf("Podano za duzo argumentow. Prosze uruchomic program w formacie  main [-ilosc pisarzy]\n");
		exit(-1);
	} else {
		if (argc != 2) {
			printf("Podano za malo argumentow. Prosze uruchomic program w formacie  main [-ilosc pisarzy]\n");
			exit(-1);
		}
	}

	if ((nWriters = atoi(argv[1])) <= 0 || (nWriters > MAX_N_WRITERS)) {
		printf("Podano zly argument, prosze podac liczbe wieksza od 0 nie wieksza od %d \n", MAX_N_WRITERS);
		exit(-1);
	}	

	// Inicjalizacja generowania liczb losowych
	srand(time(NULL));

	// Tworzymy monitor dla watkow

	myMonitor* monitor = init_monitor();

	for (int i = 0; i < N_READERS ; i++) {
		monitor->number = rand()%MAX_VALUE;
		if(pthread_create( &readerTable[i], NULL, reader, (void*) monitor) != 0) {
			perror("Blad przy tworzeniu watku");
			exit(-1);
		}
	}

	for (int i = 0; i < nWriters ; i++) {
		if(pthread_create( &writerTable[i], NULL, writer, (void*) monitor) != 0) {
			perror("Blad przy tworzeniu watku");
			exit(-1);
		}
	}

	// joinujemy watki
	for (int i = 0; i < N_READERS ; i++) {
		if ( pthread_join( readerTable[i], NULL) < 0)	{
			perror(" Blad funkcji join");
			exit(-1);
		}
	}

	for (int i = 0; i < nWriters ; i++) {
		if ( pthread_join( writerTable[i], NULL) < 0)	{
			perror(" Blad funkcji join");
			exit(-1);
		}
	}

	exit(0);
}

void* reader (void* data) {

	// Niezbedne castowanie
	myMonitor* monitor = (myMonitor*)data;

	int number = monitor->number;
	int howManyFound;

	while(1) {

		// Poczatek czytania
		
		if (sem_wait(monitor->access) < 0) {
			perror("Blad funkcji sem_wait");
			exit(-1);
		} 

		if ((monitor->nReading > 0) || (monitor->nWriting > 0)) {
			// Zmniejszamy semafor
			if (sem_wait(monitor->readers) < 0) {
				perror("Blad funkcji sem_wait");
				exit(-1);
			}
		}		 
		monitor->nReading += 1;

		// Czytanie

		howManyFound = 0;
		for (int i = 0; i < N_NUMBERS; i++) {
			if (monitor->numbers[i] == number) {
				howManyFound++;
			}
		}
		if (howManyFound != 0) {			
			printf("Znalazlem %d liczb %d\n", howManyFound, number);
		}		

		// Koniec czytania
			
		if ((--(monitor->nReading)) == 0) {
			if (sem_post(monitor->writers) < 0) {
				perror("Blad funkcji sem_post");
				exit(-1);
			} 
		}

		if (sem_post(monitor->access) < 0) {
			perror("Blad funkcji sem_post");
			exit(-1);
		} 		
		
		usleep(SLEEP_TIME);
	}
		
	pthread_exit(NULL);	
}

void* writer (void* data) {

	// Niezbedne castowanie
	myMonitor* monitor = (myMonitor*)data;

	// Inicjalizacja generowania liczb losowych
	srand(time(NULL));

	int howMany;
	int number;

	while(1) {

		// Poczatek pisania

		if (sem_wait(monitor->access) < 0) {
			perror("Blad funkcji sem_wait");
			exit(-1);
		} 

		// Zmniejszamy semafor
		if (monitor->nWriting+monitor->nReading > 0) {
			if (sem_wait(monitor->writers) < 0) {
				perror("Blad funkcji sem_wait");
				exit(-1);
			} 
		}

		monitor->nWriting = 1;

		// Pisanie

		howMany = rand()%(MAX_CHANGED - 1) + 1;
	
		printf("Dodaje %d liczb:", howMany);

		for (int i = 0; i < howMany; i++) {
			number = rand()%MAX_VALUE;
			monitor->numbers[rand()%N_NUMBERS] = number;			
			printf(" %d", number);
		}
			
		printf(".\n");

		// Koniec pisania
		
		if (monitor->nReading == 0) {
			if (sem_post(monitor->writers) < 0) {
				perror("Blad funkcji sem_post");
				exit(-1);
			} else {
				do {
					if (sem_post(monitor->readers) < 0) {
						perror("Blad funkcji sem_post");
						exit(-1);
					} 
				} while (monitor->nReading > 0);
			}
		}

		// Predektrementujemy i sprawdzamy
		if (--(monitor->nWriting) == 0) {
			if (sem_post(monitor->readers) < 0) {
				perror("Blad funkcji sem_post");
				exit(-1);
			} 
		}

		if (sem_post(monitor->access) < 0) {
			perror("Blad funkcji sem_post");
			exit(-1);
		} 

		
		usleep(SLEEP_TIME);

	}
		
	pthread_exit(NULL);	
}

// Czyszczenie
void clean() {
	if(sem_unlink(SM_READERS) < 0) {
		perror("Mozliwy blad przy zamykaniu semafora pisarzy");
	}
	if(sem_unlink(SM_WRITERS) < 0) {
		perror("Mozliwy blad przy zamykaniu semafora czytelnikow");
	}
	if(sem_unlink(SM_MONITOR) < 0) {
		perror("Mozliwy blad przy zamykaniu semafora monitor");
	}

	exit(0);
}

// Sluzy do wypisywania liczb
void printNumbers (myMonitor* monitor) {

	printf("Liczby to: ");
	for (int i = 0 ; i < N_NUMBERS; i++) {
		printf("%d ", monitor->numbers[i] );
	}
	printf("\n");
						
}

// Funckja sluzaca do alokacji monitora
myMonitor* init_monitor () {

	// Tworzymy monitor
	
	myMonitor* monitor = malloc (sizeof(myMonitor));

	if (monitor == NULL) {	
		perror("Blad przy alokacji pamieci");
		exit(0);
	}

	// Tworzymy semafory	

	if ((monitor->writers = sem_open(SM_READERS, O_CREAT, 0666, 1)) == SEM_FAILED) {
		perror("Nie moge otworzyc semafora czytelnikow ");
		exit(-1);
	}
	if ((monitor->readers= sem_open(SM_WRITERS, O_CREAT, 0666, 1)) == SEM_FAILED) {
		perror("Nie moge otworzyc semafora pisarzy ");
		exit(-1);
	}
	if ((monitor->access= sem_open(SM_MONITOR, O_CREAT, 0666, 1)) == SEM_FAILED) {
		perror("Nie moge otworzyc semafora monitora ");
		exit(-1);
	}

	// Teraz wypelnimy czyms tablice liczb

	memset(monitor->numbers, 0 ,(sizeof(int)*N_NUMBERS));

	// Teraz nalezy ustawic obsluge polecenia czyszczenia semaforow po wyjsciu z programu

		atexit(clean);

		// Sygnaly tez

		if ((signal(SIGTERM, clean)) == SIG_ERR) {			
			perror("Blad przy ustawianiu procedury obslugi sygnalu ");
		};
		if ((signal(SIGINT, clean)) == SIG_ERR) {			
			perror("Blad przy ustawianiu procedury obslugi sygnalu ");
		};

	return monitor;
}


