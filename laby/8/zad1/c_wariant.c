#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

// Zmienne globalne

// Klucz dla danych watku
static pthread_key_t key_d;
// Klucz dla buforow watkow
static pthread_key_t key_buffer; 	
// Klucz dla pliku
static pthread_key_t key_file;

static pthread_once_t once = PTHREAD_ONCE_INIT; 

// Mutex do znalezienia, taki "jednorazowy"
static pthread_mutex_t found_mutex; 

// Slowo do szukania
char word[MAXLENGTH];
// Nazwa pliku
char path[MAXLENGTH];
// Liczba watkow do uruchomienia
int thread_count; 		
// Liczba rekordow dla jednego watku
int record_count; 	
// Tablica id wlasciwosci watkow
pthread_t threads[MAX_THREAD_COUNT]; 

int main(int argc, char** argv) {
	// Sprawdzenie argumentow linii polecen
	if (argc != 5)
	{
		fprintf(stderr, "Bledne wywolanie programu\n");
		fprintf(stderr, "Prosze wpisac a_wariant <liczba watkow> <plik> <ilosc rekordow dla watku> <slowo>\n");
		exit(0);
	}

	// Sprawdzamy czy podano prawidlowe argumenty
	if (atoi(argv[1]) <= 0 || atoi(argv[3]) <= 0) {
		fprintf(stderr, "Podano zle argumenty. Musisz podac liczby naturalne\n");
		exit(-1);
	}

	// Sprawdzamy czy podano prawidlowe argumenty
	if (atoi(argv[1]) > MAX_THREAD_COUNT) {
		fprintf(stderr, "Podano za duzo watkow. Prosze podac maksymalnie: %d\n", MAX_THREAD_COUNT);
		exit(-1);
	}
		
	// Zapiswanie argumentow linii polecen
	thread_count = atoi(argv[1]);
	record_count = atoi(argv[3]);
	strncpy(path, argv[2], MAXLENGTH);
	strncpy(word, argv[4], MAXLENGTH);
	
	// Tworzymy mutex dla jednokrotnego wypisywania
	pthread_mutex_init(&found_mutex, NULL);

	// Teraz bedziemy tworzyc watki
	
	for (int i = 0; i < thread_count; i++) {
		node_data* data_ptr = (node_data*) malloc(sizeof(node_data));
		
		// Tworzymy dane dla watku		
		data_ptr->record_count = record_count;
		data_ptr->start = i*record_count*RECORDSIZE;
		data_ptr->next = thread_count*record_count*RECORDSIZE+data_ptr->start;
	
		// Stworzenie watku i przekazanie danych
		if(pthread_create(&threads[i], NULL, search, (void*) data_ptr) != 0) {
			perror("Blad funkcji pthread_create");
			exit(-1);
		}
	}
	
	// Detachujemy nasze watki
	for (int i = 0; i < thread_count; i++) {
		if (pthread_detach(threads[i]) < 0) {
			perror("Blad w funkcji pthread_detach");
			exit(-1);
		}
	}
	return 0;
}

void* search(void* inputData) {
	
	char* buffer;
	FILE* file;
	node_data* data;
	
	// Korzystamy z funkcji pthread_once aby tylko jeden watek zaalokowal klucz
	if (pthread_once(&once, make_keys) != 0) {
		perror("Blad funkcji pthread_once");
		exit(-1);
	}
	// Alokujemy pamiec	
	file = (FILE*) malloc(sizeof(FILE));
	buffer = (char*) calloc(RECORDSIZE, 1);
	data= (node_data*) inputData;
	
	if (file == NULL ||  buffer == NULL || data== NULL) {
		perror("Blad w funkcji calloc");
		exit(-1);
	}
	
	// Asocjujemy zmienne danego watku
	if (pthread_setspecific(key_buffer, buffer) != 0) {
		perror("Blad funkcji pthread_setspecific");
		exit(-1);
	}
	if (pthread_setspecific(key_file, file) != 0) {
		perror("Blad funkcji pthread_setspecific");
		exit(-1);
	}
	if (pthread_setspecific(key_d, data) != 0) {
		perror("Blad funkcji pthread_setspecific");
		exit(-1);
	}
		
	// Otwieramy plik do odczytu
	
	file = pthread_getspecific(key_file);
	
	if ((file = fopen(path, "r")) == NULL) {
		perror("Blad przy otwieraniu pliku z rekordami");
		exit(-1);
	}	

	// Pobieramy nasz  fragment pliku ze struktury
	data = pthread_getspecific(key_d);
	
	// Korzystamy z funkcji fseek do znalezienia miejsca w pliku
	if (fseek(file, data->start, SEEK_SET) < 0) {
		perror("Blad funkcji fseek");
		exit(-1);
	}

	// To wskaznik na nasz lancuch znaleziony w rekordzie
	char* word_found;
	
	// Pobieramy ilosc rekordow do pobrania z node'a
	int currentRead = 0;
	int targetRead = data->record_count;
	
	// Pobieramy swoj konkretny bufor
	buffer = pthread_getspecific(key_buffer);
	
	/*
		Petla, w ktorej bedziemy wczytac dane w pliku dopoki da sie cokolwiek odczytac
	*/

	int ret;
	while((ret = fread(buffer, sizeof(*buffer), RECORDSIZE, file)) != 0) {
		if (ret == -1) {
			// Blad czytania z pliku
			perror("Blad przy odczycie z pliku");
			exit(-1);
		}
		
		/*
			Tutaj znajdujemy nasze slowo
			sizeof(int) poniewaz chcemy pominac numer		
		*/
		word_found = strstr(buffer+sizeof(int), word);
		
		if (word_found != NULL) {
			// Numer rekordu
			int recordnumber;
			// Kopiujemy numer
			memcpy(&recordnumber, buffer, 4);
			
			// Mutex do wypisywania
			pthread_mutex_lock(&found_mutex);

			printf("Znaleziono w watku o numerze: %u w rekordzie o numerze: %d\n", (int) pthread_self(), recordnumber);

			// Zwalniamy mutex
			pthread_mutex_unlock(&found_mutex);
			
		}
		
		// Zmniejszamy liczbe rekordow do przeszukania w tej sesji
		currentRead++;
		// Sprawdzamy czy nie trzeba juz przeskoczyc
		if (currentRead == targetRead) {
			// Przechodzimy do nastepnych rekordow
			if (fseek(file, data->next, SEEK_SET) < 0) {
				perror("Blad w funkcji fseek");
				exit(-1);
			}
			data->next += data->next-data->start;
			currentRead = data->record_count;
		}
	}
	
	// Konczymy
	pthread_exit(NULL);
}

static void make_keys() {

	if(pthread_key_create(&key_buffer, cleanUp) != 0) {
		perror("Blad funkcji pthread_key_create");
		exit(-1);
	}
	if(pthread_key_create(&key_file, cleanUp) != 0) {
		perror("Blad funkcji pthread_key_create");
		exit(-1);
	}
	if(pthread_key_create(&key_d, cleanUp) != 0) {
		perror("Blad funkcji pthread_key_create");
		exit(-1);
	}
}

static void cleanUp(void* ptr) {
	if(ptr != NULL) {
		free(ptr);
	}
}
