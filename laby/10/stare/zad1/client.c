#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

// Buffor do wpisywania z konsoli
char buff [512];

int SocketFD;

int main(int argc, char** argv) {

	struct sockaddr_in stSockAddr;
 	int Res;
	SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
 
	if (-1 == SocketFD) {
		perror("Blad przy tworzeniu socketu");
		exit(EXIT_FAILURE);
	}
 
	memset(&stSockAddr, 0, sizeof(stSockAddr));
 
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(1100);
	Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);
 
	if (0 > Res) {
		 perror("Bledna rodzina adresow");
		 close(SocketFD);
		 exit(EXIT_FAILURE);
	}
	else if (0 == Res) {
		 perror("Bledny adres IP");
		 close(SocketFD);
		 exit(EXIT_FAILURE);
	}
 
	if (-1 == connect(SocketFD, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr))) {
		 perror("Blad przy polaczeniu");
		 close(SocketFD);
		 exit(EXIT_FAILURE);
	}

	// Teraz bedziemy w koncu pisac
 
	pthread_t writeThread;

	if (pthread_create( &writeThread, NULL, consoleInput, NULL) != 0) {
		perror("Blad przy tworzeniu watku");
		exit(-1);
	}

	while (1) {
		int len = read(SocketFD, buff, sizeof(buff));
		buff[len] = '\0';
		printf("Odczytano: %s\n",  buff);
	}

	if (pthread_join(writeThread, NULL) < 0) {
		perror("Blad w funkcji pthread_detach");
		exit(-1);
	}

	(void) shutdown(SocketFD, SHUT_RDWR);
 
	close(SocketFD);

	return EXIT_SUCCESS;
}


void* consoleInput (void* data) {

	char buffRead [512];

	while (1) {	
		scanf("%s", buffRead);
		write(SocketFD, buffRead, sizeof(buff));
	}

	pthread_exit(NULL);
}
