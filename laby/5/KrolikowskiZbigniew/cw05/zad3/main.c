#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int main(int argc, char* argv[]) {

	// Alokacja pamieci
	FILE* in ;
	FILE* out;

	if ((in = (FILE*) malloc(sizeof(FILE))) == NULL) {
		fprintf(stderr," Blad w alokacji pamieci dla struktury file");
		return (-1);
	}

	if ((out = (FILE*) malloc(sizeof(FILE))) == NULL) {
		fprintf(stderr," Blad w alokacji pamieci dla struktury file");
		return (-1);
	}

	char buffer[120];
	
	if ((in = popen("ls -l", "r")) == NULL) {
		fprintf(stderr, "Blad funkcji popen\n");
		return (-1);
	}
	
	if ((out = popen("grep ^d > out.txt", "w")) == NULL) {
		fprintf(stderr, "Blad funkcji popen\n");
		return (-1);
	}
	
	while (fgets(buffer, 120, in)) {
		 fputs(buffer, out);	
	}

	// Zamkniecie

	pclose(in);
	pclose(out);	

	return(0);
}
