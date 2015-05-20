#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int main (int argc, char* argv[]) {

	// Pierwszy pipe


	int pipe1[2];
	if (pipe(pipe1) == -1) {		
		fprintf(stderr, "Blad funkcji pipe!\n");
		return -1;
	}

	int child1, child2, child3;

	// Robimy trzy forki

	if((child1 = fork()) < 0) {		
		fprintf(stderr, "Blad funkcji fork!\n");	
		exit(-1);
	} else if (child1 == 0) {
		// Potomek
		// Ustawiamy wejscie pierwszego pipa jako wyjscie
		if (dup2(pipe1[1], STDOUT_FILENO) == -1) {		
			fprintf(stderr, "Blad funkcji dup2 z pierwszym pipem!\n");
			exit(-1);
		}
		// Zamykamy wyjscie
		if (close(pipe1[0]) == -1) {		
			fprintf(stderr, "Blad funkcji close z pierwszym pipem!\n");
			exit(-1);
		}
		execlp("ls", "ls", "-l", NULL);
	}

	// Drugi pipe	

	int pipe2[2];
	if (pipe(pipe2) == -1) {		
		fprintf(stderr, "Blad funkcji pipe!\n");
		return -1;
	}	

	// Drugi fork

	if((child2 = fork()) < 0) {		
		fprintf(stderr, "Blad funkcji fork!\n");	
		exit(-1);
	} else if (child2 == 0) {
		// Ustawiamy wyjscie pierwszego pipa jako wejscie
		if (dup2(pipe1[0], STDIN_FILENO) == -1) {		
			fprintf(stderr, "Blad funkcji dup2 z pierwszym pipem!\n");
			exit(-1);
		}	
		// Zamykamy wejscie pierwszego
		if (close(pipe1[1]) == -1) {		
			fprintf(stderr, "Blad funkcji close z pierwszym pipem!\n");
			exit(-1);
		}
		// Ustawiamy wejscie drugiego pipa jako jego wyjscie
		if (dup2(pipe2[1], STDOUT_FILENO) == -1) {		
			fprintf(stderr, "Blad funkcji dup2 z drugim pipem!\n");
			exit(-1);
		}	
		// Zamykamy wyjscie drugiego
		if (close(pipe2[0]) == -1) {		
			fprintf(stderr, "Blad funkcji close z drugim pipem!\n");
			exit(-1);
		}
		execlp("grep", "grep", "^d", NULL);
	}

	// Trzeci fork

	if((child3 = fork()) < 0) {		
		fprintf(stderr, "Blad funkcji fork!\n");	
		exit(-1);
	} else if (child3 == 0) {
		// Potomek
		// Zamykamy wejscie pierwszego
		if (close(pipe1[1]) == -1) {		
			fprintf(stderr, "Blad funkcji close z pierwszym pipem!\n");
			exit(-1);
		}
		// Zamykamy wyjscie pierwszego	
		if (close(pipe1[0]) == -1) {		
			fprintf(stderr, "Blad funkcji close z pierwszym pipem!\n");
			exit(-1);
		}
		// Ustawiamy wyjscie drugiego pipa jako jego wejscie
		if (dup2(pipe2[0], STDIN_FILENO) == -1) {		
			fprintf(stderr, "Blad funkcji dup2 z drugim pipem!\n");
			exit(-1);
		}	
		// Zamykamy wejscie drugiego
		if (close(pipe2[1]) == -1) {		
			fprintf(stderr, "Blad funkcji close z drugim pipem!\n");
			exit(-1);
		}
		execlp("wc", "wc", "-l", NULL);
	}

	// Domykanie pipow
	// Zamykamy wejscie pierwszego
	if (close(pipe1[0]) == -1) {		
		fprintf(stderr, "Blad funkcji close z pierwszym pipem!\n");
		exit(-1);
	}
	// Zamykamy wyjscie pierwszego
	if (close(pipe1[1]) == -1) {		
		fprintf(stderr, "Blad funkcji close z pierwszym pipem!\n");
		exit(-1);
	}
	// Zamykamy wejscie drugiego
	if (close(pipe2[0]) == -1) {		
		fprintf(stderr, "Blad funkcji close z drugim pipem!\n");
		exit(-1);
	}
	// Zamykamy wejscie drugiego
	if (close(pipe2[1]) == -1) {		
		fprintf(stderr, "Blad funkcji close z drugim pipem!\n");
		exit(-1);
	}

	return 0;
}
