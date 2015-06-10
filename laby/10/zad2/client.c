#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

char thisClientUsername[USERNAME_LENGTH_MAX];
int clientType;
char address[20];
char socketPath[80];
char port[8];

int main(int argc, char *argv[]) {	

	/*
		Sprawdzamy i kopiujemy argumenty linii poleceń
	*/
	
	// Najpierw skopiujmy nazwe klienta
	if (argc >= 1) {		
		strcpy(thisClientUsername, argv[1]);
		if (strlen(thisClientUsername) > USERNAME_LENGTH_MAX) {		
			fprintf(stderr, "Podano za dluga nazwe uzytkownika. Maksymalny rozmiar to: %d", USERNAME_LENGTH_MAX);
			exit(-1);		
		}
	}

	// To przypadek kiedy mamy klienta sieciowego. Potrzebne jest wtedy 5 argumentow
	if (argc == 5) {
		if (strcmp(argv[2], "-r") == 0) {
			clientType = WEB;
		} else {
			printf("Podano zle argumenty. Prosze wpisac: client <thisClientUsername> < -l <path> | -r <adres> <port> >\n");
			exit(-1);
		}
		strcpy(port, argv[4]);
		strcpy(address, argv[3]);
	}
	// To przypadek kiedy mamy klienta unixowego. Potrzebne sa wtedy 4 argumenty
	else if (argc == 4) {
		if (strcmp(argv[2], "-l") == 0) {
			clientType = UNIX;
		} else {
			printf("Podano zle argumenty. Prosze wpisac: client <name> < -l <path> | -r <adres> <port> >\n");
			exit(-1);
		}
		strcpy(socketPath, argv[3]);
	} else {
		fprintf(stderr, "Zla liczba argumentow. Prosze wpisac: client <name> < -l <path> | -r <adres> <port> >\n");
		exit(-1);  
	}

	// Deklaracja zmiennych

	int howManySent, server;  
	char message[DATA_MAX_SIZE];
	int rv;
	struct addrinfo hints, *servinfo, *p;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	/*
		Teraz w zaleznosci od typu klienta bedziemy robic rozne rzeczy
	*/
    
	if (clientType == WEB) {
		// Klient sieciowy
		if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
			fprintf(stderr, "Blad funkcji getaddrinfo: %s\n", gai_strerror(rv));
			return 1;
		}
		for (p = servinfo; p != NULL; p = p->ai_next) {

			if ((server = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
				perror("Blad funkcji socket");
				continue;// get sockaddr, IPv4 or IPv6:
			}

			if (connect(server, p->ai_addr, p->ai_addrlen) == -1) {
				close(server);
				perror("Blad funkcji connect");
				continue;
			}
				
			break;
	    	}

			inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
	}
    
	if (clientType == UNIX) {
		// Klient unixowy
		int len;
		struct sockaddr_un remote;

		if ((server = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
			perror("Blad funkcji socket");
			exit(1);
		}

		remote.sun_family = AF_UNIX;
		strcpy(remote.sun_path, socketPath);
		len = strlen(remote.sun_path) + sizeof(remote.sun_family);
		if (connect(server, (struct sockaddr *)&remote, len) == -1) {
			perror("Blad funkcji connect");
			exit(1);
		}
	}

	/*

	Teraz uzyjemy funkcji fork aby klient mial czesci odpowiedzialne za odbieranie i wysylanie wiadomosci

	*/
	    
	int pid;
    
	if ((pid = fork()) == -1) {
		perror("Blad funkcji fork");
		exit(-1);
	} else if (pid == 0) {
		// Odbieranie sygnalow
		int howMany;
    		char msgrcvd[DATA_MAX_SIZE];
    		while(1) {
			if ((howMany = recv(server, msgrcvd, DATA_MAX_SIZE-1, 0)) == -1) {
				perror("Blad przy odbieraniu sygnalu");
				exit(-1);
			}
		
			printf("%s", msgrcvd);
		}
    	} else {
		// Wysylanie sygnalow

		// Ten length jest nam potrzebny po to zeby bylo lvalue. Ograniczenie jezykowe.
    		size_t lenght = (size_t) sizeof message;
		char buffer[USERNAME_LENGTH_MAX+DATA_MAX_SIZE+2];
    		int read;
    		char* lineptr = message;
    		while ((read = getline(&lineptr, &lenght, stdin)) != -1) {
			memset(buffer, 0, strlen(buffer));
			strcat(buffer, thisClientUsername);
			strcat(buffer, ": ");		
			strcat(buffer, lineptr);
			if ((howManySent = send(server, buffer, DATA_MAX_SIZE-1, 0)) == -1) {
			    	perror("Blad funkcji przy wysylaniu sygnalow przy pomocy funkcji send");
			    	exit(1);
			}
    		}	
    	
    		close(server);
    	}
    
    return 0;
}
