#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif
#ifndef FUN
#define FUN
#include "functions.h"
#endif

int main (int argc, char* argv[]) {

	if (argc != 4) {
		fprintf(stderr, "Prosze podac <lewy brzeg przedzialu> <prawy brzeg przedzialu> <ile testow w przedziale>\n");	
		return -1;
	}

	int low;
	int high;
	int howMany;
	
	if ((low = atoi(argv[1])) < 0) {
		fprintf(stderr, "Lewy brzeg przedzialu nie moze byc ujemny!\n");
		fprintf(stderr, "Prosze podac <lewy brzeg przedzialu> <prawy brzeg przedzialu> <ile testow w przedziale>\n");
		return -1;
	}
	if (((high = atoi(argv[2])) < 0) || high < low) {		
		fprintf(stderr, "Lewy brzeg przedzialu nie moze byc wiekszy niz prawy!\n");
		fprintf(stderr, "Prosze podac <lewy brzeg przedzialu> <prawy brzeg przedzialu> <ile testow w przedziale>\n");	
		return -1;
	}
	if (((howMany = atoi(argv[3])) < 0) || (high - low)+1 < howMany) {		
		fprintf(stderr, "Przedzial za maly do wykonania takiej liczby testow\n");
		fprintf(stderr, "Prosze podac <lewy brzeg przedzialu> <prawy brzeg przedzialu> <ile testow w przedziale>\n");	
		return -1;
	}

	printf("LIczba,czRMat,czRPot,czUz,czSys,czUzPot,czSysPot\n");
	for (int i = 0; i <= howMany; ++i) {	
		test_threads(threads_fork, ((low + (high)/howMany)*i));
	}
	printf("----------------------------------------------------------------NEXT\n");
	for (int i = 0; i < howMany; ++i) {	
		test_threads(threads_vfork, ((low + (high-low)/howMany)*i));
	}
	printf("----------------------------------------------------------------NEXT\n");
	for (int i = 0; i < howMany; ++i) {	
		test_threads(threads_clone, (low + ((high-low)/howMany)*i));
	}
	printf("----------------------------------------------------------------NEXT\n");
	for (int i = 0; i < howMany; ++i) {	
		test_threads(threads_vclone, (low + ((high-low)/howMany)*i));
	}

}
