#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

mqd_t serverQ;
mqd_t clientQ;
char* queueName;

int receving=1;

int main(int argc, char *argv[]){
	// Sprawdzenie argumentow
	if (argc != 2 ) {
		if (argc > 2 ) {
			perror("Podano za duzo argumentow. Prosze podac tylko nazwe klienta");
		} else {
			perror("Nie podano arguemntu. Prosze podac nazwe klienta");
		}
		return -1;		
	}

	// Ustawianie funkcji atexit
	atexit(exitFunctionClient);
	queueName=malloc(sizeof(char)*128);
	sprintf(queueName, "/%s_queue", argv[1]);

	// Ustawianie parametrow wiadomosci
	struct mq_attr attr;
	attr.mq_flags = O_NONBLOCK;
	attr.mq_maxmsg = 9;
	attr.mq_msgsize = 128;

	// Otwieracie kolejki dla tego klienta
	if((clientQ = mq_open(queueName, O_RDWR | O_CREAT | O_NONBLOCK, S_IRUSR | S_IWUSR, &attr))==-1) {
		perror("Blad przy otwieraniu kolejki od klienta\n");
		exit(1);
	}
	
	// Otwarcie kolejki serwera
	if((serverQ = mq_open("/server_queue", O_RDWR))==-1) {
		perror("Blad przy otwieraniu kolejki od serwera nie powiodlo sie\n");
		exit(1);
	}

	// Wyslanie nazwy do serwera
	if(mq_send(serverQ, argv[1], strlen(argv[1]) + 1, 0)==-1) {
		perror("Wysylanie danych identyfikacyjnych do serwera nie powiodlo sie\n");
		exit(1);
	}

	// Zamykamy kolejke od serwera
	if(mq_close(serverQ) ==-1){
		perror("Blad przy zamykaniu kolejki od serwera\n");
		exit(1);
	}

	
	printf("Wszedles do czatu.\n");

	// Teraz uruchomimy osobny proces do obslugi odczytu

	pid_t pid;
	if( (pid=fork()) == -1) {
		perror("Blad przy funkcji fork\n");
		exit(-1);
	}

	if(pid == 0) {
		// Jestesmy w procesie od odczytu
		int prio = -1;
		char buff[attr.mq_msgsize];

		// Ustawiamy obsluge sygnalu
		if(signal(SIGTERM,&termHandlerClient)==SIG_ERR){
			perror("Blad przy ustawianiu procedury obslugi sygnalu\n");
			exit(1);
		}
	
		// Bedziemy odbierac wiadomosci w kolejce
		while(receving) {
				// Czyscimy pamiec
				memset(buff,0,strlen(buff));
				// Odbieramy wiadomosc
				if(mq_receive(clientQ, buff, attr.mq_msgsize, (unsigned int*) &prio) == -1) {
					if(errno == EAGAIN){
						// Kolejka jest pusta						
						continue;
					} else {
						perror("Blad przy odczycie z kolejki\n");
						exit(1);
					}
				}
				if(prio == 0){		
					// To jest wiadomosc z serwera 	
					// Wypisujemy takze znak zachety
					printf("%s> ", buff);
					fflush(stdout);
				}
				else if(prio > 0){			
					// !=0 klient serwer wraca do kolejki		
					if(mq_send(clientQ, buff, strlen(buff), prio) ==-1) {
						perror("Blad przy wysylaniu wiadomosci do serwera\n");
						exit(1);
					}

				}
		}
		return 0;
	} else {
		// Proces do wysylania 
		// Tworzymy sobie buforek
		char buff[attr.mq_msgsize];

		do {
			// Czyscimy pamiec
			memset(buff,0,strlen(buff));
			// Pobieramy napis z klawiatury
			fgets (buff, attr.mq_msgsize, stdin);
			printf("> ");
			// Tak na wszelki wypadek
			buff[strlen(buff) - 1] = '\0';
			// Wysylamy wiadomosc
			if (mq_send(clientQ, buff, strlen(buff) + 1, 1)==-1) {
				perror("Blad wysylaniu wiadomsci do serwera\n");
				exit(1);
			}
		} while(strcmp(buff, "exit") != 0);

		// Koniec wysylania, sprzatamy

		// Zabijam potomka
		if(kill(pid, SIGTERM) == -1) {
			perror("Blad przy zabijaniu potomka");
			exit(1);
		}
		mq_unlink(queueName);
	}

	return 0;
}

void exitFunctionClient(){
	
	char* buff = "exit";

	if (mq_send(clientQ, buff, strlen(buff) + 1, 1)==-1) {
		perror("Blad wysylaniu wiadomsci wyjscia do serwera\n");
		exit(1);
	}

	mq_unlink(queueName);
	free(queueName);
}
void termHandlerClient(int sgn){
	if(sgn==SIGTERM) {
		receving = 0;
	} 
}
