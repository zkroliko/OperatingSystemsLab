#include "ryglowanie.h"

enum Options {SETREAD, SETWRITE, PRINTL, RELEASE, READC, WRITEC};
const char* option_strings[] = {"setread", "setwrite", "printl", "release", "readc", "writec"};
const char* options_string = {"setread setwrite printl release readc writec"};
	
int main(int argc, char* argv[]) {

	// Przechowywanie opcji
	char* option;
	if ((option = malloc(sizeof(char)*10)) == NULL) {
		fprintf(stderr, "Blad alokacji pamieci!\n");
	}

	// Sciezka do pliku
	char* path;
	if ((path = malloc(sizeof(char)*80)) == NULL) {
		fprintf(stderr, "Blad alokacji pamieci!\n");
	}

	// Struktura flock
	struct flock* lock;
	if ((lock = malloc(sizeof(struct flock))) == NULL ) {
		fprintf(stderr, "Blad alokacji pamieci!\n");
	}

	// Miejsce w pliku
	long unsigned* position;
	if ((position = malloc(sizeof(long unsigned))) == NULL) {
		fprintf(stderr, "Blad alokacji pamieci!\n");
	}
	
	// Pobranie opcji
	// Pierwsze przejscie
	printf("Prosze podac jedna z opcji: %s \n", options_string);
	if (scanf("%s", option) < 0) {
		fprintf(stderr, "\nBlad odczytu!\n");
	} // Powtarzamy az nie bedzie prawidlowe
	while ( !(parseOption(option) >= 0 && parseOption(option) < 6 ) ) {
		printf("Nieprawdilowa wartosc. \n");
		printf("Prosze podac jedna z opcji: %s \n", options_string);
		if (scanf("%s", option) < 0) {
			fprintf(stderr, "\nBlad odczytu!\n");
		}
	}

	// Pobranie sciezki
	// Pierwsze przejscie
	printf("Prosze podac sciezke do pliku. \n");
	if (scanf("%s", path) < 0) {
			fprintf(stderr, "\nBlad odczytu!\n");
	} 
	// Powtarzamy az nie bedzie prawidlowe
	while ( access( path, F_OK ) == -1 ) {
		printf("Ten plik nie daje sie otworzyc! Prawdopodobnie nie istnieje. \n");
		printf("Prosze podac inna sciezke do pliku. \n");
		if (scanf("%s", path) < 0) {
			fprintf(stderr, "\nBlad odczytu!\n");
		} 
	}

	// Zamieniamy sobie od razi na integer zeby bylo wygodniej
	int option_int = parseOption(option);

	// Teraz sprawdzamy polecenia
	if (option_int == PRINTL) {
		// Jezeli mamy po prostu go wypisac
		printl(path, lock);
		return 0;
	} 
	// Jezeli wykonanie doszlo do tego miesjca to znaczy, ze musimy pobrac pozycje w pliku
	// Pobranie miejsca w pliku
	// Najpierw sprawdzmy jego dlugosc
	struct stat st;
	stat(path, &st);
	// Teraz pobieramy pozycje
	printf("Prosze podac pozycje w pliku. \n");
	if (scanf("%lu", position) < 0) {
		fprintf(stderr, "\nBlad odczytu!\n");
	}
	while (*position > st.st_size || *position < 0) {
		printf("Podano miejsce znajdujace sie poza plikiem.\n");
		printf("Prosze podac pozycje w pliku.\n");
		if (scanf("%lu", position) < 0) {
			fprintf(stderr, "\n!\n");
		}
	}
	// Teraz skorzystamy z odpowiednich funkcji
	if (option_int == SETREAD) {
		if (setread (path, *position, lock)  == 0) {
			sleep(40);
			printf("Poprawnie zalozone rygiel do odczytu w pozycji: %lu.\n", *position);
			return 0;
		} else {
			fprintf(stderr, "Blad przy zakladaniu rygla!\n");
			return -1;
		}
	}
	if (option_int == SETWRITE) {
		if (setwrite (path, *position, lock)  == 0) {
			sleep(40);
			printf("Poprawnie zalozone rygiel do zapisu w pozycji: %lu\n", *position);
			return 0;
		} else {
			fprintf(stderr, "Blad przy zakladaniu rygla!\n");
			return -1;
		}
	}
	if (option_int == RELEASE) {
		if (release (path, *position, lock)  == 0) {
			printf("Poprawnie zwolniono rygiel w pozycji: %lu\n", *position);
			return 0;
		} else {
			fprintf(stderr, "Blad przy zwolnianiu rygla!\n");
			return -1;
		}
	}
	if (option_int == READC) {
		if (readc (path, *position)  == 0) {
			return 0;
		} else {
			fprintf(stderr, "Blad przy odczytywaniu znaku!\n");
			return -1;
		}		
	}
	if (option_int == WRITEC) {
		printf("Prosze podac znak: \t");
		char* c = malloc(sizeof(char));
		scanf(" %c", c);
		if (writec (path, *position, c)  == 0) {				
			printf("Znak na miejscu %lu w pliku zamieniony na: %c \n", *position, *c);
			free(c);
			return 0;
		} else {
			fprintf(stderr, "Blad przy zapisywaniu znaku!\n");
			free(c);
			return -1;
		}
	}
			
	// Dealokacja pamieci
	free(option);
	free(path);
	free(position);
	free(lock);

	return -1;
}

// Do zamieniania opcji w postaci lancuch do liczby
int parseOption (char* option) {
	for (int i = 0; i < 6; ++i)	{
		if (strcmp(option, option_strings[i]) == 0) {
			return i;		
		}
	}
	return -1;
}

// Do wypisania rygli
int printl(char* path, struct flock* lock) {
	// Deskryptor pliku
	int fileDesc;
	char* character = (char*) malloc(sizeof(char));
	// Otwieramy plik
	if((fileDesc = open(path , O_RDWR)) < 0) {
		fprintf(stderr, "Blad przy otwarciu pliku\n");
		return -1;
	}
	// Ustawiamy parametry ryglowania
	lock->l_type = F_WRLCK;
	lock->l_start = -1;
	lock->l_whence = SEEK_CUR;
	lock->l_len = 1;
	
	for(int i = 0; read(fileDesc, character, 1) > 0; ++i) {
		// Te ustawienia ulegaja zmienie w kazdym przejsciu petli
		lock->l_start = -1;		
		lock->l_type = F_WRLCK;
		lock->l_whence = SEEK_CUR;
		
		if (fcntl(fileDesc, F_GETLK, lock)) {
			fprintf(stderr, "Blad przy odczycie rygla!\n");
			return -1;
		}
		if (lock->l_type == F_WRLCK) {
			// Do zapisu
			printf("%d\t zapis \tNumer PID procesu: %d\n", i, lock->l_pid);
		}
		if (lock->l_type == F_RDLCK) {
			// Do odczytu
			printf("%d\t odczyt \tNumer PID procesu: %d\n", i, lock->l_pid);
		}
	}
	free(character);

	return 0;
}

// Do zalozenia rygla do odczytu
int setread (char* path, long unsigned position, struct flock* lock) {
	// Deskryptor pliku
	int fileDesc;
	// Otwieramy plik
	if((fileDesc = open(path , O_RDONLY)) < 0) {
		fprintf(stderr, "Blad przy otwieraniu pliku!\n");
		return -1;
	}
	// Ustawiamy parametry ryglowania
	lock->l_type = F_RDLCK;
	lock->l_start = (off_t)position;
	lock->l_whence = SEEK_SET;
	lock->l_len = 1; //jeden znak
	if (fcntl(fileDesc, F_SETLK, lock) < 0) {
		fprintf(stderr, "Blad w zalozeniu locka!\n");
		return -1;
	}
	return 0;
}

// Do zalozenia rygla do zapisu
int setwrite (char* path, long unsigned position, struct flock* lock) {
	// Deskryptor pliku
	int fileDesc;
	// Otwieramy plik
	if((fileDesc = open(path , O_WRONLY)) < 0) {
		fprintf(stderr, "Blad przy otwieraniu pliku!\n");
		return -1;
	}
	// Ustawiamy parametry ryglowania
	lock->l_type = F_WRLCK; // write lock
	lock->l_start = (off_t)position; // gdzie
	lock->l_whence = SEEK_SET;
	lock->l_len = 1; // na jeden znak
	if (fcntl(fileDesc, F_SETLK, lock)) {
		fprintf(stderr, "Blad w zalozeniu rygla!\n");
		return -1;
	}
	return 0;
}

// Do zwolnienia rygla
int release (char* path, long unsigned position, struct flock* lock) {
	// Deskryptor pliku
	int fileDesc;
	if((fileDesc = open(path , O_WRONLY)) < 0) {
		fprintf(stderr, "Blad przy otwarciu pliku!\n");
		return -1;
	}
	// Ustawiamy parametry ryglowania
	lock->l_type = F_UNLCK; // unlock
	lock->l_start = (off_t)position; // gdzie
	lock->l_whence = SEEK_SET;
	lock->l_len = 1; // jeden znak
	if ((fcntl(fileDesc, F_SETLK, lock)) < 0) {
		fprintf(stderr, "Blad przy zdejmowaniu rygla!\n");
		return -1;
	}
	return 0;
}

// Do odczytu znaku
int readc (char* path, long unsigned position) {
	// Deskryptor pliku
	int fileDesc;
	// Nie chcialem robic tego wczesnie to musze to zrobic niestety teraz
	char* characterCopy = malloc(sizeof(char));
	// Otwieramy plik
	if((fileDesc = open(path , O_RDONLY)) < 0) {
		fprintf(stderr, "Blad otwarcia pliku\n");
		return -1;
	}
	// Przesuwamy sie na odpowiednie miejsce
	lseek(fileDesc, position, SEEK_SET);
	// Odczytujemy znak
	if ((read(fileDesc, characterCopy, 1)) < 0) {
		fprintf(stderr, "Nie mozna odczytac danego znaku.\n");
		return -1;
	}

	printf("Znak na miejscu %lu w pliku to: %c. \n", position, *characterCopy);

	free(characterCopy);

	return 0;	
}

// Do nadpisania znaku
int writec (char* path, long unsigned position, char* character) {
	int fileDesc; 
	// Otwieramy plik
	if((fileDesc = open(path , O_WRONLY)) < 0) {
		fprintf(stderr, "Blad przy otwarciu pliku!\n");
		return -1;
	}
	// Przesuwamy sie na odpowiednia pozycje
	lseek(fileDesc, position, SEEK_SET);
	// Zapisujemy znak
	if ((write(fileDesc, character, 1)) < 0) {
		fprintf(stderr, "Blad zapisu do pliku!\n");
		return -1;
	}

	return 0;
}
