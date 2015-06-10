#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int main(int argc, char** argv) {

	// Zmienne
	
	client clients[MAXCLIENTS];
	int clientCount = 0;

	// Sciezka
	char* path;
	int port;
	int fdmax;
	int unixDescriptor;
	int webDescriptor;

	// Sprawdzenie argumentow
	
	if (argc != 3)	{
		perror("Zla liczba argumentow. Prosze wpisac server <port> <sciezka>\n");
		exit(-1);
	}

	// Pobranie argumentow

	path = argv[2];

	if ((port = atoi(argv[1])) <= 0) {
		perror("Podano ujemny numer portu. Musi on byc wiekszy od zera.\n");
		exit(-1);
	}
	
	/*
		Utworzenie socketa UNIX i powiazanie
	*/
	struct sockaddr_un adress_unix;
	
	adress_unix.sun_family = AF_UNIX;
	strcpy(adress_unix.sun_path, path);
	
	int temporary = 1;

	// Otwieramy socket
	
	if ((unixDescriptor = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
		perror("Blad funkcji socket przy sockecie unixowym");
		exit(-1);
	} 

	setsockopt(unixDescriptor, SOL_SOCKET, SO_REUSEADDR, &temporary, sizeof(int));

	// Wiazemy

	if (bind(unixDescriptor, (struct sockaddr*) &adress_unix, sizeof(adress_unix)) < 0) {
		perror("Blad funkcji bind przy sockecie unixowym");
		exit(-1);
	}
	
	/*
		Utworzenie socketa sieciowego i powiazanie
	*/
	
	struct sockaddr_in adress_net;
	
	adress_net.sin_family = AF_INET;
	adress_net.sin_port = htons(port); 
	adress_net.sin_addr.s_addr = htonl(INADDR_ANY);
	
	temporary = 1;
	
	if ((webDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("Blad w funkcji socket przy sockecie internetowym");
		exit(-1);
	}

	setsockopt(webDescriptor, SOL_SOCKET, SO_REUSEADDR, &temporary, sizeof(int));

	// Wiazemy

	if (bind(webDescriptor, (struct sockaddr*)&adress_net, sizeof(adress_net)) < 0) {
		perror("Blad w funkcji bind przy sockecie internetowym");
		exit(-1);
	}

	/*
	
	Zajmijmy sie zbiorem deskryptorow

	*/
	
	fd_set mainSet;
	
	FD_ZERO(&mainSet);
	
	fdmax = unixDescriptor;

	FD_SET(unixDescriptor, &mainSet);
	
	if (webDescriptor > fdmax) {
		fdmax = webDescriptor;
	}

	FD_SET(webDescriptor, &mainSet);

	// Wszyscy kliecni sa nieaktywni

	for (int i = 0; i < MAXCLIENTS; i++) {
		clients[i].active = 0;
	}
	
	struct message datagram;
	struct sockaddr socketAddress;
	socklen_t sizeAddress = sizeof(struct sockaddr);
	
	/*
		Petla do odczytywania wiadomosci a nastepnie do ich rozsylania
	*/

	while(1) {

		fprintf(stderr, "bump1");
		
		// Uzycie funkcji select do oczekiwania na wiadomoscs
		if (select(fdmax+1, &mainSet, NULL, NULL, NULL) <= 0) { 
			perror("Blad funkcji select");
			exit(-1); 
		}

		fprintf(stderr, "bump2");
		
		/*

			Teraz uzyjemy takiej dziwnej konstrukcji, nie musimy dzieki temu duplikowac kodu

		*/
		
		for (int i = 0; i < 2; i++) {

			int socketDesc;

			if (i == WEB_CLIENT) {
				socketDesc = webDescriptor;
			} else {
				socketDesc = unixDescriptor;
			}

			if (FD_ISSET(socketDesc, &mainSet)) {
				// Odczytanie wiadomosci
				if (recvfrom(socketDesc, (void*) &datagram, sizeof(struct message), 0, (struct sockaddr*) &socketAddress, &sizeAddress) < 0) { 
					perror("Blad przy odczytywaniu wiadomosci"); 
					exit(-1); 
				}
				
				// Sprawdzamy aktywnosci klientow
				for (int j = 0; j <= clientCount; j++)	{
					if (clients[j].active == 1) 
					{
						if (time(NULL) - clients[j].timestamp > 30) {
							 clients[j].active = 0;
						}
					}
				}
			
				// Czy typ danych to rejestracja
				if (datagram.type == REGISTER) {	
					for (int i = 0; i <= clientCount; i++) {
						if (clients[i].active == 0) {
							clients[i].active = 1;
							clients[i].timestamp = time(NULL);
							clients[i].comtype = datagram.comtype;
							clients[i].address = socketAddress;
							sprintf(clients[i].username, "%s", datagram.username);
							break;
						}
					}
					if (clientCount < MAXCLIENTS) {
						// Wiecej klientow
						clientCount++;
					}
			}
				// Czy typ danych to wiadomosc
				else if (datagram.type == MESSAGE) {
					for (int i = 0; i < clientCount; i++) {
						// Rozsylanie wiadomosci do podlaczonych klientow
						if(clients[i].active == 1 && strcmp(datagram.username, clients[i].username) != 0) {
							// Ten oto jest podlaczony
							// Najpierw sprawdzmy 
							int clientSocketDesc;
							if (clients[i].comtype == WEB_CLIENT) {
								clientSocketDesc = webDescriptor;
							} else {
								clientSocketDesc = unixDescriptor;
							}

							if (sendto(clientSocketDesc, (void*) &datagram, sizeof(struct message), 0, 
										&clients[i].address, sizeof(clients[i].address)) < 0) { 
								// Blad przy wysylaniu
								perror("Blad przy rozsylaniu wiadomosci");
								exit(-1); 
							}
						}
					}
				}

				break;
			}
		}
		
	}
	exit(0);
}
