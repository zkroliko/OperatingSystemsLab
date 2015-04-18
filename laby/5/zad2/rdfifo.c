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


    	if ((fd = open(myfifo, O_RDONLY)) == -1) {
		fprintf(stderr, "Blad przy otwieraniu kolejki fifo!\n");
		return -1;
	}

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

	printf("Potok otwarty. Serwer nasluchuje kolejki %s\n", myfifo);
	fflush(stdout);

	int readRet;
	while( access( myfifo, F_OK ) != -1 ) {			
		memset(line,0,strlen(line));	
		if ((readRet = (read(fd, line, MAXLENGHT - 1))) == -1) {
			fprintf(stderr, "Blad przy odczycie ze strumienia!\n");
			return -1;
		}
		// Godzina odczytu
		time_t current_time;
		current_time = time(NULL);	
   		buffer = ctime(&current_time);
		printf("Godzina odczytu: %s", buffer);	
		// Wypisanie komunikatu	
		printf("Wiadomosc: %s\n", line);
	}	


	return 0;
}
