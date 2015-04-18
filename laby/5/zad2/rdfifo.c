#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

#define MAXLENGHT 300

// Zmienne globalne
int fd;
char * myfifo = "/tmp/myfifo";

void handler () {
	close(fd);
	unlink(myfifo);	
	exit(0);
}

int main (int argc, char* argv[]) {

	if (mkfifo(myfifo, 0666) == -1) {
		if (errno == EEXIST) {
			// Jest juz
			// Usuwamy kolejke
			if (remove (myfifo) == -1) {
				fprintf(stderr, "Kolejka juz istnieje ale nie moge jej usunac!\n");
				return -1;	
			}
			// Tworzymy jeszcze raz
			if (mkfifo(myfifo, 0666) == -1) {
				fprintf(stderr, "Kolejka juz istniala i zostala usunieta. Blad przy tworzeniu kolejki!\n");
				return -1;	
			}
		} else {
			// Jakis inny blad
			fprintf(stderr, "Blad przy tworzeniu kolejki!\n");
			return -1;	
		}	
	}

	// Alokacja pamieci

	char * line, *buffer;
	if ((line = malloc(sizeof(char)*MAXLENGHT)) == NULL) {
		fprintf(stderr, "Blad przy alokacji pamieci!\n");
		return -1;
	}

	if ((buffer = malloc(sizeof(char)*MAXLENGHT)) == NULL) {
		fprintf(stderr, "Blad przy alokacji pamieci!\n");
		return -1;
	}

	// Ustawiamy obsluge syngalow

	if (signal(SIGINT, handler) == SIG_ERR) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}

	// Otwieranie

	printf("Serwer bedzie nasluchiwal kolejki %s\n", myfifo);

    	if ((fd = open(myfifo, O_RDONLY)) == -1) {
		fprintf(stderr, "Blad przy otwieraniu kolejki fifo!\n");
		return -1;
	}

	printf("Potok otwarty.\n");
	fflush(stdout);

	// Odczyt

	int readRet;
	while( access( myfifo, F_OK ) != -1 ) {			
		memset(line,0,MAXLENGHT);	
		if ((readRet = (read(fd, line, MAXLENGHT))) == -1) {
			fprintf(stderr, "Blad przy odczycie ze strumienia!\n");
			return -1;
		}
		if (strlen(line) == 0 ){		
			printf("Zaden proces nie wpisuje do kolejki. Czekam...\n");	
			fflush(stdin);
			// Ponowne otwarcie bedzie dzialac jak czekanie, w tym trybie (blokujacym)
    			if ((fd = open(myfifo, O_RDONLY)) == -1) {
				fprintf(stderr, "Blad przy otwieraniu kolejki fifo!\n");
				return -1;
			}
			printf("Proces zapisuj do kolejki. Wznawiam prace...\n");	
			fflush(stdin);
		} else {
			// Godzina odczytu
			time_t current_time;
			current_time = time(NULL);	
	   		buffer = ctime(&current_time);
			buffer[strlen(buffer)-1]='\0'; // Obcinamy \n

			// Wypisanie komunikatu			
			printf(" %s - %s", buffer , line);
		}
	}	


	return 0;
}
