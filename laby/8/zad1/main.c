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

	// Deskryptor pliku
	int fd; 
	// Wskaznik na obszar pamieci
	int* ptr; 	
	// Klucze dla semafora
	key_t s_memory_key, semaphore_key; 
	// Dla czasu	
	struct timeval tv;
	
	// Zajmujemy sie semaforami
	int semset;
	
	// Tworzymy klucz
	if ((semaphore_key = ftok(SM_NAME, 0)) < 0) {
		perror("Blad tworzenia klucza przy pomocy funkcji ftok\n");
		exit(-1);
	}

	// Otwieramy zbior semaforow

	if ((semset = semget(semaphore_key, 4, 0)) < 0) {
		perror("Blad przy otwieraniu semaforow");
		exit(-1);
	}

	// Teraz pamiec wspoldzielona


	// Tworzymy klucz
	if ((s_memory_key = ftok(SHM_NAME, 0)) < 0) {
		perror("Blad przy tworzeniu klucza dla semaforow \n");
		exit(-1);
	}
		
	// Otwieramy pamiec wspoldzielona

	if ((fd = shmget(s_memory_key, (BUFFER_SIZE+2)*sizeof(int), IPC_CREAT)) < 0)	{
		perror("Blad przy otwieraniu wspoldzielonej pamieci");
		exit(-1);
	}
	
	// Mapujemy pamiec
	if ((ptr = (int*) shmat(fd, NULL, 0)) == -1) {
		perror("Blad przy mapowaniu pamieci");
		exit(-1);
	}
	
	// Struktura sembuf do funkcji semop
	struct sembuf op;

	// TO JEST BARDZO WAZNE, UBUNTU MA TO DOMYSLNIE USTAWIONE NA 1
	op.sem_flg= 0;


	
	while(1) {
		// Jestesmy w petli do odczytywania liczb
		
		// Odejmujemy 1 od semafora EMPTY
		op.sem_num = EMPTY;
		op.sem_op = -1;

		if (semop(semset, &op, 1) == -1) {
			perror("Blad funkcji semop");		
		}
	
		// Odczytujemy wartosc EMPTY
		int left;
		if ((left = semctl(semset, EMPTY, GETVAL, NULL)) == -1) {
			perror("Blad funkcji semctl");		
		} 
	
		// Ustawiamy semafor pobrania
		op.sem_num = TAKE;
		if (semop(semset, &op, 1) == -1) {
			perror("Blad funkcji semop");		
		} 
		
		// Pobieram numer z przedostatniego elementu tablicy
		// to indeks pobierania
		int num = ptr[ptr[BUFFER_SIZE]];
		// Zapisujemy czas momentu pobrania
		gettimeofday(&tv, NULL); 
		ptr[BUFFER_SIZE] = (ptr[BUFFER_SIZE]+1)%BUFFER_SIZE;

		// Wylaczamy semafor do pobrania
		op.sem_num = TAKE;
		op.sem_op = 1;
		if (semop(semset, &op, 1) == -1) {
			perror("Blad funkcji semop");		
		} 
		
		// Dodajemy do semafora empty 1
		op.sem_num = FULL;
		if (semop(semset, &op, 1) == -1) {
			perror("Blad funkcji semop");		
		} 
	
		// Wydruk wyniku
		if (isPrime(num) != 1) {
			printf("(%d %d.%d) Sprawdzilem liczbe %d - zlozona. Liczba zadan oczekujacych: %d\n", pid, (int)tv.tv_sec, 
									(int)tv.tv_usec, num,  left);
		} else {
			printf("(%d %d.%d) Sprawdzilem liczbe %d - pierwsza. Liczba zadan oczekujacych: %d\n", pid, (int)tv.tv_sec, 
										(int)tv.tv_usec, num,  left);
		}
		
		// Spimy	
		wait(WAIT_BASE, WAIT_VARY);			
	}
}

void producent() {

	// Deskryptor pliku
	int fd; 
	// Wskaznik na obszar pamieci
	int* ptr; 	
	// Klucze dla semafora
	key_t s_memory_key, semaphore_key; 
	// Dla czasu	
	struct timeval tv;
	
	// Zajmujemy sie semaforami
	int semset;
	
	// Tworzymy klucz
	if ((semaphore_key = ftok(SM_NAME, 0)) < 0) {
		perror("Blad tworzenia klucza przy pomocy funkcji ftok\n");
		exit(-1);
	}

	// Otwieramy zbior semaforow

	if ((semset = semget(semaphore_key, 4, 0)) < 0) {
		perror("Blad przy otwieraniu semaforow");
		exit(-1);
	}

	// Teraz pamiec wspoldzielona


	// Tworzymy klucz
	if ((s_memory_key = ftok(SHM_NAME, 0)) < 0) {
		perror("Blad przy tworzeniu klucza dla semaforow \n");
		exit(-1);
	}
		
	// Otwieramy pamiec wspoldzielona

	if ((fd = shmget(s_memory_key, (BUFFER_SIZE+2)*sizeof(int), IPC_CREAT)) < 0)	{
		perror("Blad przy otwieraniu wspoldzielonej pamieci");
		exit(-1);
	}
	
	// Mapujemy pamiec
	if ((ptr = (int*) shmat(fd, NULL, 0)) == -1) {
		perror("Blad przy mapowaniu pamieci");
		exit(-1);
	}
	
	// Do funkcji semop
	struct sembuf op;	

	// TO JEST BARDZO WAZNE, UBUNTU MA TO DOMYSLNIE USTAWIONE NA 1
	op.sem_flg= 0;

	// Iniciujemy generator losowy
	srand(time(NULL));
	

	// Petla w ktorej bedziemy dodawac liczby
	while(1) {		
		// Petla do wpisywania naszych liczb do tablicy
		int number = rand(); // wylosowana liczba
	
		// Odejmujemy od semafora FULL
		op.sem_num = FULL;
		op.sem_op = -1;

		if (semop(semset, &op, 1) == -1) {
			perror("Blad funkcji semop");		
		} 
		
		// Ustawiamy semafor dostawiania
		op.sem_num = PUT;
		if (semop(semset, &op, 1) == -1) {
			perror("Blad funkcji semop");		
		} 

		// Pobieramy element do wstawienia
		
		// W ostatniej komorce ptr[MAX_LENGTH+1] jest indeks
		// Ustawiam tam liczbe
		ptr[ptr[BUFFER_SIZE+1]] = number;
		// Dodaje do tego indeksu 1 zapetlam na poczatek tablicy jezeli jest ostatni
		ptr[BUFFER_SIZE+1] = (ptr[BUFFER_SIZE+1]+1)%BUFFER_SIZE;
	
		// Zwalniam semafor dostawiania
		op.sem_num = PUT;
		op.sem_op = 1;
		if (semop(semset, &op, 1) == -1) {
			perror("Blad funkcji semop");		
		} 
		
		// Dodaje do semafora EMPTY 1
		op.sem_num = EMPTY;
		if (semop(semset, &op, 1) == -1) {
			perror("Blad funkcji semop");		
		} 
	
		// Z semafora EMPTY pobieramy wartosc
		int left;
		if ((left = semctl(semset, EMPTY, GETVAL, NULL)) == -1) {
			perror("Blad funkcji semctl");		
		} 

		// Pobieram czas
		gettimeofday(&tv, NULL); 
	
		printf("(%d %d.%d) Dodalem liczbe: %d. Licba zadan oczekujacych: %d\n", pid, (int)tv.tv_sec, 
									(int)tv.tv_usec, number,  left);
							
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

