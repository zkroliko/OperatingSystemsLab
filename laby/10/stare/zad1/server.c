#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

fd_set readset;

char buff [512];
int sockets [100];
int curIndex = 0;
 
int main(void) {
	
	struct sockaddr_in stSockAddr;
	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
 
	if(-1 == SocketFD) {
		perror("Blad przy tworzeniu socketu-");
		exit(EXIT_FAILURE);
	}
 
	memset(&stSockAddr, 0, sizeof(stSockAddr));
 
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(1100);
	stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
	if(-1 == bind(SocketFD,(struct sockaddr *)&stSockAddr, sizeof(stSockAddr))) {
		perror("Blad przy przypisywaniu deskryptora socketowi");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}
 
	if(-1 == listen(SocketFD, 10)) {
		perror("Blad funkcji listen");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}
 
	for(;;) {

		FD_ZERO(&readset);
		FD_SET(SocketFD, &readset);

		for (int i =0; i < curIndex; i++) {
			FD_SET(sockets[i], &readset);		
		}	

		if (select(100, &readset, NULL, NULL, NULL) < 0) {
			perror("Blad funkcji select");
		}

		if (FD_ISSET(SocketFD, &readset)) {
			int fd = accept(SocketFD, NULL, NULL);
			if (fd < 0) {
				perror("Blad funkcji accept");
			} else if (fd > FD_SETSIZE) {
				close(fd);
			} else {
				sockets[curIndex++] = fd;
				printf("Pojawil sie klient.\n");
			}
		}

		for (int i =0; i < curIndex; i++) {
			if (FD_ISSET(sockets[i], &readset)) {
				int len = read(sockets[i], buff, 512);
				buff[len]='\0';
				printf("Odczytano %s\n", buff);

				for (int j =0; j < curIndex; i++) {
					write(sockets[j], buff, len);
				}	
			}		
		}	
	 
		if (-1 == shutdown(SocketFD, SHUT_RDWR)) {
			perror("Blad przy zamykaniu socketu");
			close(SocketFD);
			exit(EXIT_FAILURE);
		}
	}
 
	close(SocketFD);
	return EXIT_SUCCESS;  
}
