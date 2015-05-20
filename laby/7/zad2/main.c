#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

/*
Main sluzy do wyboru pomiedzy konsumentem a producentem
segment pamieci oraz semafory juz powinny byc zainicjalizowane
*/

// Zmienne globalne
int pid;

int main(int argc, char* argv[]) {

	// Sprawdzamy ilosc argumentow
	if (argc > 2) 	{
		printf("Podano za duzo argumentow. Prosze uruchomic program w formacie prokons [-prod|-kons]\n");
		exit(-1);
	} else {
		if (argc != 2) {
			printf("Podano za malo argumentow. Prosze uruchomic program w formacie prokons [-prod|-kons]\n");
			exit(-1);
		}
	}
	
	// Zapisujemy nasze pid
	pid = getpid();
	
	// Sprawdzamy argumenty
	if (strcmp(argv[1], "-kons") == 0) {
		consument();
	} else	{
		if (strcmp(argv[1], "-prod") == 0) {
			producent();
		} else 	{
			printf("Podano zly argument. Prosze uruchomic program w formacie prokons [-prod|-kons]\n");
			exit(-1);
		}
	}
	
	exit(0);	
}

void consument() {
	// Semafory
	sem_t* empty, *full, *take, *put;
	// Zmienna pomocnicza
	int left;
	// Deskryptor pliku dla pamieci wspoldzielonej
	int fd; 
	// Wskaznik na obszar pamieci wspoldzieonej	
	int* ptr; 
	// Struktura stat
	struct stat stat;
	// Do pomiaru czasu
	struct timeval tv;

	// Otwieramy semafory
	if ((empty = sem_open(SM_EMPTY, 0)) == SEM_FAILED) {
		perror("Blad przy otwieraniu semafora SM_EMPTY ");
		exit(-1);
	}
	if ((full = sem_open(SM_FULL, 0)) == SEM_FAILED) {
		perror("Blad przy otwieraniu semafora SM_FULL ");
		exit(-1);
	}
	if ((take = sem_open(SM_TAKE, 0)) == SEM_FAILED) {
		perror("Blad przy otwieraniu semafora SM_POBIERZ ");
		exit(-1);
	}
	if ((put = sem_open(SM_PUT, 0)) == SEM_FAILED) {
		perror("Blad przy otwieraniu semafora SM_WSTAW ");
		exit(-1);
	}
	
	// Otwiaramy pamiec wspoldzielona
	if ((fd = shm_open(SHM_NAME, O_RDWR, 0666)) < 0) {
		perror("Blad przy otwieraniu pamieci wspoldzielonej");
		exit(-1);
	}
	
	// Pobieramy strukture stat z fd
	if (fstat(fd, &stat) < 0) {
		perror("Blad funkcji fstat");
		exit(-1);
	}
	
	// Mapujemy pamiec
	if ((ptr = (int*) mmap(0, stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("Blad funkcji mmap");
		exit(-1);
	}
	
	// Zamykamy deskryptor
	if (close(fd) < 0) {
		perror("Blad przy zamykaniu deskryptora");
		exit(-1);
	}
	
	// Petla w ktorej bedziemy pobierac liczby
	while(1) {
		
		// Zmniejszamy semafor empty
		if (sem_wait(empty) < 0) {
			perror("Blad funkcji sem_wait podczas blokowania semafora empty");
			exit(-1);
		} 
		// Sprawdzamy wartosc empty
		if (sem_getvalue(empty, &left) < 0) {
			perror("Blad funkcji sem_getvalue");
			exit(-1);
		}
		// Czekamy na take
		if (sem_wait(take) < 0) {
			perror("Blad funkcji sem_wait podczas blokowania semafora take");
		}
		
		// Przedostatni element tablicy to nastepny element do zabrania
		int num = ptr[ptr[MAX_LENGTH]];
		// Pobieram czas
		gettimeofday(&tv, NULL);
		// Zmieniam indeks na nastepny
		ptr[MAX_LENGTH] = (ptr[MAX_LENGTH]+1)%MAX_LENGTH;
		
		// Zawalniamy semafor take
		if (sem_post(take) < 0)	{
			perror("Blad funkcji sem_post podczas zwalniania semafora take");
			exit(-1);
		}
		// Zwiekszamy semafor full
		if (sem_post(full) < 0)	{
			perror("Blad funkcji sem_post podczas zwalniania semafora full");
			exit(-1);	
		}
		
		// Wypisywanie
		if (isPrime(num) == 1) {			
			printf("(%d %d.%d) Sprawdzilem liczbe %d - pierwsza. Pozostalo zadan oczekujacych: %d\n", pid, (int)tv.tv_sec, (int)tv.tv_usec, num,  left);
		} else {
			printf("(%d %d.%d) Sprawdzilem liczbe %d - zlozona. Pozostalo zadan oczekujacych: %d\n", pid, (int)tv.tv_sec, (int)tv.tv_usec, num,  left);
		}
									
		// Spimy
		sleep(1);			
	}
}

void producent() {

	// Semafory
	sem_t* empty, *full, *take, *put;
	// Zmienna pomocnicza
	int left;
	// Deskryptor pliku dla pamieci wspoldzielonej
	int fd; 
	// Wskaznik na obszar pamieci wspoldzieonej	
	int* ptr; 
	// Do pomiaru czasu
	struct timeval tv;

	// Otwieramy semafory
	if ((empty = sem_open(SM_EMPTY, 0)) == SEM_FAILED) {
		perror("Blad przy otwieraniu semafora SM_EMPTY ");
		exit(-1);
	}
	if ((full = sem_open(SM_FULL, 0)) == SEM_FAILED) {
		perror("Blad przy otwieraniu semafora SM_FULL ");
		exit(-1);
	}
	if ((take = sem_open(SM_TAKE, 0)) == SEM_FAILED) {
		perror("Blad przy otwieraniu semafora SM_POBIERZ ");
		exit(-1);
	}
	if ((put = sem_open(SM_PUT, 0)) == SEM_FAILED) {
		perror("Blad przy otwieraniu semafora SM_WSTAW ");
		exit(-1);
	}	
	
	// Otwiaramy pamiec wspoldzielona
	if ((fd = shm_open(SHM_NAME, O_RDWR, 0666)) < 0) {
		perror("Blad przy otwieraniu pamieci wspoldzielonej");
		exit(-1);
	}

	// Mapujemy pamiec
	if ((ptr = (int*) mmap(0, (MAX_LENGTH+2)*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("Blad funkcji mmap");
		exit(-1);
	}

	// Zamykamy deskryptor
	if (close(fd) < 0) {
		perror("Blad przy zamykaniu deskryptora");
		exit(-1);
	}
	
	// Inicjuemy generator
	srand(time(NULL));
	
	while(1) {
		// Dekrementujemy pelny
		if (sem_wait(full) < 0)	{
			perror("Blad funkcji sem_wait podczas dekrementacji pelny");
			exit(-1);
		}
		// Ustawiamy put
		if (sem_wait(put) < 0) {
			perror("Blad funkcji sem_wait podczas blokowania semafora put");
		}
		
		// Ostatni element tablicy to indeks gdzie mozna wstawic
		ptr[ptr[MAX_LENGTH+1]] = rand(); // wlasciwy zapis
		ptr[MAX_LENGTH+1] = (ptr[MAX_LENGTH+1]+1)%MAX_LENGTH; // zmiana indeksu do wstawienia
		// koniec monitora
		
		// Odblokowujemy put
		if (sem_post(put) < 0)	{
			perror("Blad funkcji sem_post podczas zwalniania semafora put");
			exit(-1);
		}
		// Dekrementujemy empty
		if (sem_post(empty) < 0) {
			perror("Blad funkcji sem_post podczas dekrementacji semafora empty");
			exit(-1);
		}
	
		// Sprawdzamy ile zostalo
		if (sem_getvalue(empty, &left) < 0) {
			perror("Blad funkcji sem_getvalue");
			exit(-1);
		}
		
		// Pobranie czasu
		gettimeofday(&tv, NULL); 	

		// Wypisywanie	
		printf("(%d %d.%d) Dodalem liczbe: %d. Liczba zadan oczekujacych: %d\n", pid, (int)tv.tv_sec, (int)tv.tv_usec, rand(),  left);
		
		// Spimy
		wait(WAIT_BASE, WAIT_VARY);
	}	
}

int isPrime(unsigned int number) {
	if (number <= 3) {
		// Pierwsza
		return 1;
	}		
	if (number % 2 == 0) {
		return 0;
	}		
	for (unsigned int i = 3; i*i <= number; i+=2) {
		if (number % i == 0) {
			// Zlozona
			return 0; 
		}
	}
	// Pierwsza
	return 1;
}

void wait(int base, int vary) {
	// Inicjuemy generator
	srand(time(NULL));

	usleep((base+rand()%vary)*1000);
} 

