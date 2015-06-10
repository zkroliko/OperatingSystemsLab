#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

// Zmienne globalne

char port[16];
char path[80];

int main(int argc, char** argv) {

	/*
		Najpierw sprawdzimy ilosc argumnetow
	*/

	if (argc != 3) {
		fprintf(stderr, "Zla liczba argumentow.\nProsze wpisac server <port> <sciezka>\n");
		exit(-1);
	}
	
	/*
		Kopiujemy argumenty	
	*/	
	
	strcpy(port, argv[1]);
	strcpy(path, argv[2]);

	// Lista obserwowanych deskyptorow
	fd_set mainSet;
	// Druga, przejsciowa lista
	fd_set read_fds;
	// Numer maksymalnego deskryptora
	int maxSocketDesc;

	// Deskryptor socketu internetowego
	int web_listener;
	// Deskryptor socketu Unix
	int unix_listener;
	// nowy deskryptor
	int newSocketDesc;

	// Buffor na wiadomosc
	char message[DATA_MAX_SIZE];
	// Liczba odczytanych bajtow
	int nBytesRead; 

	// Adres klienta
	struct sockaddr_storage clientaddr;
	// Dlugosc adresu
	socklen_t addrlen;

	int temporary = 1;  
	int rv;

	struct addrinfo hints;
	struct addrinfo* server_inet;

	// Czyszczenie zbioru deskryptorow
	FD_ZERO(&mainSet);
	FD_ZERO(&read_fds);

	/*
		Utworzenie socketa internetowego 
	*/

	memset(&hints, 0, sizeof hints);	
	hints.ai_flags = AI_PASSIVE;	
	// Automatyczny wybor protokolu
	hints.ai_family = AF_UNSPEC;		
	// Socket strumieniowy
	hints.ai_socktype = SOCK_STREAM;
	
	// Zapisz do server_inet informacje o adresie socketu zdalnego
	if ((rv = getaddrinfo(NULL, port, &hints, &server_inet)) != 0) {
		perror("Blad getaddrinfo");
		exit(-1);
	}
	
	// Utworzenie socketa i powiazanie do podanego portu
	for (struct addrinfo *p = server_inet; p != NULL; p = p->ai_next) {

		web_listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if (web_listener < 0) { 
			continue;
		}

		setsockopt(web_listener, SOL_SOCKET, SO_REUSEADDR, &temporary, sizeof(int));

		if (bind(web_listener, p->ai_addr, p->ai_addrlen) < 0) {
			close(web_listener);
			continue;
		}
		
		break;
	}

	/*
		Utworzenie socketa unixowego
	*/

	struct sockaddr_un local;
	int len;
	
	unix_listener = socket(AF_UNIX, SOCK_STREAM, 0);
	
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, path);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	
	if (bind(unix_listener, (struct sockaddr *)&local, len) < 0 ) {
		perror("Blad funkcji bind przy sockecie unixowym. Funkcjonalnosc unixowa nie bedzie dzialac prawidlowo.");	
	}

	freeaddrinfo(server_inet);

	
	// Czekamy na polaczenia dla socketa unixowego
	if (listen(unix_listener, 10) == -1) {
		perror("Blad funkcji listen");
		exit(-1);
	}


	// Czekamy na polaczenie dla socketa sieciowego
	if (listen(web_listener, 10) == -1) {
		perror("Blad funkcji listen");
		exit(-1);
	}

	// Dodanie listenera do mainSeta, na ktorym mamy deskryptory
	FD_SET(web_listener, &mainSet);
	FD_SET(unix_listener, &mainSet);

	// Inicjalizacja maxSocketDesc, na razie sa tylko dwa deskryptory
	maxSocketDesc = unix_listener;

	while(1) {
		read_fds = mainSet;
		// Czekamy na nadejscie wiadomosci
		if (select(maxSocketDesc+1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(-1);
		}

		/*
			Obsluga wiadomosci
		*/

		for (int i = 0; i <= maxSocketDesc; i++) {
			if (FD_ISSET(i, &read_fds)) {
				// Sprawdzamy czy to rejestracja i polaczenie sieciowe
				if (i == web_listener) {
					// Tworzymy nowe polaczenie
					addrlen = sizeof clientaddr;					
					// Akceptujemy polaczenie
					newSocketDesc = accept(web_listener, (struct sockaddr *) &clientaddr, &addrlen);

					if (newSocketDesc == -1) {
						perror("Blad funkcji accept");
					} else {
						// Dodanie do zbioru deksryptorow
						FD_SET(newSocketDesc, &mainSet); 
						if (newSocketDesc > maxSocketDesc) {
							maxSocketDesc = newSocketDesc;
						}

						printf("Z serwerem polaczyl sie nowy uzytkownik\n");

					}
					
				} else if (i == unix_listener) {					
					// Tworzymy nowe polaczenie
					addrlen = sizeof clientaddr;
					// Akceptujemy polaczenie
					newSocketDesc = accept(unix_listener, (struct sockaddr *) &clientaddr, &addrlen);

					if (newSocketDesc == -1) {
						perror("Blad przy funkcji accept");
					} 
					else {
						// Dodanie do zbioru deksryptorow
						FD_SET(newSocketDesc, &mainSet);
						if (newSocketDesc > maxSocketDesc) {
							maxSocketDesc = newSocketDesc;
						}

						printf("Z serwerem polaczyl sie nowy uzytkownik\n");

					}
			   	} else {		
					/*						
			   			Jezeli to zwykla wiadomosc
					*/	
				
					// Odczytujemy tresc wiadomosci
					if ((nBytesRead = recv(i, message, sizeof message, 0)) > 0) {
						/* 
							Rozsylamy wiadmosc do wszystkich
						*/
						for(int j = 0; j <= maxSocketDesc; j++) {
							if (FD_ISSET(j, &mainSet)) {
								if (j != web_listener && j != unix_listener && j != i) {
									if (send(j, message, nBytesRead, 0) == -1) {
										perror("Blad przy wysylaniu wiadomosci");
									}
								}
			
							}
						}
						// Koniec fora od rozsylania wiadmosci
					} else {

						if (nBytesRead == 0) {
							printf("Klient o numerze socketu %d odlaczyl sie\n", i);
						} else perror("Blad przy odczytywaniu wiadmosci przy pomocy funkcji recv");
						close(i);
						FD_CLR(i, &mainSet); // Usuniecie deskryptora ze zbioru

					}
					// Koniec else
				}
				// Koniec else, tego od tego czy to nie rejestracja
			}
		}
	}
	
	return 0;
}
