#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

// Zmienne globalne

struct sockaddr_un unix_addr;
struct sockaddr_in net_addr;

time_t timestamp;
int pid;

char msgbuf[MAX_MESSAGE_SIZE];
char *username;
int sockettype;
int socketFD;

fd_set mainSet;

int main(int argc, char* argv[]) {

	/* 
		Sprawdzenie i odczytanie argumentow
	*/

	if (argc != 5 && argc != 4) {
		if (argc == 2) {
			if (strcmp(argv[1], "--h") == 0) {				
				fprintf(stderr, "Prosze wpisac: client <username> < -l <sciezka> | -r <adres> <port> >\n");
				exit(0);
			}
		}	
		fprintf(stderr, "Zla liczba argumentow. Prosze wpisac: client <username> < -l <sciezka> | -r <adres> <port> >\n");
		exit(-1);
	}
	

	// Odczytujemy nazwe klienta	

	username = argv[1];

	// Teraz sprawdzamy czy chodzi o klienta unixowego czy sieciowego
	
	if (strcmp(argv[2], "-l") == 0) {
		sockettype = UNIX_CLIENT;
	} else if (strcmp(argv[2], "-r") == 0) {
		sockettype = WEB_CLIENT;
	} else {				
		fprintf(stderr, "Zla argumenty.Prosze wpisac: client <username> < -l <sciezka> | -r <adres> <port> >\n");
		exit(-1);
	}
		
	int temporary = 1;

	// Tworzymy socket

	// w zaleznosci od rodzaju klienta stworzymy inne sockety

	if (sockettype == WEB_CLIENT && argc == 5) {	
		// Klient sieciowy
		socketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		net_addr.sin_family = AF_INET;
		net_addr.sin_port = htons(atoi(argv[4]));
		net_addr.sin_addr.s_addr = inet_addr(argv[3]);
		
		temporary = 1;

		setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &temporary, sizeof(int));

		if (connect(socketFD, (struct sockaddr*) &net_addr, sizeof(struct sockaddr_in)) < 0) 
		{ 
			perror("Blad funkcji bind");
			exit(-1);
		}
	}
	
	if (sockettype == UNIX_CLIENT && argc == 4) {
		// Klient unixowy
		socketFD = socket(AF_UNIX, SOCK_DGRAM, 0); 

		if ((socketFD = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
			perror("Blad funkcji socket");
			exit(-1);
		} 
	
		unix_addr.sun_family = AF_UNIX;
		strcpy(unix_addr.sun_path, argv[3]);
		
		setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &temporary, sizeof(int));

		// Wiazemy socket z deskryptorem

		if (connect(socketFD, (struct sockaddr*) &unix_addr, sizeof(struct sockaddr_un)) < 0) 
		{ 
			perror("Blad funkcji connect");
			exit(-1); 
		}

	}

	// Ustawiamy handlery syngalow

	signal(SIGUSR1, &message_send);
	signal(SIGUSR2, &client_register);
	
	int pid;

	/*

	Uzyjemy funkcji fork zamiast korzystania z biblioteki pthreads	

	*/
	
	if ((pid = fork()) < 0)	{
		perror("Blad funkcji fork");
		exit(-1);
	}
	else if (pid > 0) {

		// Odbieranie wiadomosci

		FD_ZERO(&mainSet);
		FD_SET(socketFD, &mainSet);
	
		message data;
		struct sockaddr socket_addr;
		socklen_t size_addr = sizeof(struct sockaddr);
	
		while(1) {
			// Czekamy przy pomocy funkcji select
			select(socketFD+1, &mainSet, NULL, NULL, NULL);
		
			if (FD_ISSET(socketFD, &mainSet)) {
				// Odczytanie wiadomosci
				if (recvfrom(socketFD, (void*) &data, sizeof(struct message), 0, (struct sockaddr*) &socket_addr, &size_addr) < 0) { 
					perror("Blad funkcji recvfrom"); 
					exit(-1);
				}
			}
			//if (strcmp(username, data.username) != 0) {					
				if (data.type == MESSAGE) {			
					printf("%s: %s", data.username, data.message);
				}
				if (data.type == REGISTER) {			
					printf("Uzytkownik o nicku %s wszedl do czatu\n", data.username);
				}
			//}
			// Dodajemy do zbioru
			FD_SET(socketFD, &mainSet);
		}
	} else if (pid == 0) {
		// Wysylanie wiadomosci
		while(1) {
			fgets(msgbuf, MAX_MESSAGE_SIZE, stdin);
			msgbuf[strlen(msgbuf)] = '\0';
			
			// Sprawdzamy czas

			if(time(NULL) - timestamp > 30) {
				timestamp = time(NULL);
				// Rejestrujemy sie ponownie
				kill(pid, SIGUSR2);
			}
			
			kill(pid, SIGUSR1);
		}
	}
	
	exit(0);
}

void client_register(int signo) {
	if (signo == SIGUSR2) {

		message data;
		sprintf(data.username, "%s", username);

		data.type = REGISTER;
		data.comtype = sockettype;
		
		if(sockettype == WEB_CLIENT) {
			if(sendto(socketFD, (void*) &data, sizeof(struct message), 0, (struct sockaddr*) &net_addr, sizeof(net_addr)) < 0) { 
				perror("Blad funkcji sendto"); 
				exit(-1); 
			}
		} else if(sockettype == UNIX_CLIENT) {
			if(sendto(socketFD, (void*) &data, sizeof(struct message), 0, (struct sockaddr*) &unix_addr, sizeof(unix_addr)) < 0) { 
				perror("Blad funkcji sendto"); 
				exit(-1); 
			}
		}
	}
}


void message_send(int signo) {
	if (signo == SIGUSR1) {
		message data;
		sprintf(data.username,"%s",username);
		sprintf(data.message,"%s", msgbuf);
		data.type = MESSAGE;
		data.comtype = sockettype;

		if (sockettype == WEB_CLIENT) {
			if(sendto(socketFD, (void*) &data, sizeof(struct message), 0, (struct sockaddr*) &net_addr, sizeof(net_addr)) < 0) { 
			  	perror("Blad funkcji sendto"); 
			  	exit(-1); 
			}
		} else if (sockettype == UNIX_CLIENT) {
			if(sendto(socketFD, (void*) &data, sizeof(struct message), 0, (struct sockaddr*) &unix_addr, sizeof(unix_addr)) < 0) {
				perror("Blad funkcji sendto");
				exit(-1);
			}
		}
	}
}

