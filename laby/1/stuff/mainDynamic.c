#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <dlfcn.h>

// Jaakies ladne kolorki

// Bez koloru prosze zmienic ponizej
#define kolor

#ifdef kolor
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#else
#define ANSI_COLOR_RED     ""
#define ANSI_COLOR_GREEN   ""
#define ANSI_COLOR_YELLOW  ""
#define ANSI_COLOR_BLUE    ""
#define ANSI_COLOR_MAGENTA ""
#define ANSI_COLOR_CYAN    ""
#define ANSI_COLOR_RESET   ""
#endif

 
int main () {


	// Wypadaloby wyciszyc warningi od nieuzywanych wskaznikow funkcji

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-variable"

	// Otworzmy biblioteke
	void* lib = dlopen("../zad1/liblist.so", RTLD_NOW);

	printf(ANSI_COLOR_YELLOW "Otwieranie biblioteki dynamicznej"ANSI_COLOR_RESET"\n");
	if (lib == NULL) {
		fprintf(stderr,ANSI_COLOR_RED "Blednie otwarto biblioteke! Zamykamy program\n" ANSI_COLOR_RESET);
		return 1;	
	}
	printf(ANSI_COLOR_GREEN "Bilbioteka otwarta poprawnie. Importujemy funkcje. \n" ANSI_COLOR_RESET);

	// Zaimportujmy wszystko z biblioteki
	Data* (*Data_new)(char*, char*, char*, char*, char*, char*) = dlsym(lib, "Data_new");
	Data* (*Data_copy)(Data*) = dlsym(lib, "Data_copy");
	void (*Data_print)(Data*) = dlsym(lib, "Data_print");
	int (*Data_compare)(Data*, Data*) = dlsym(lib, "Data_compare");
	unsigned char (*Data_detect)(Data*, char*) = dlsym(lib, "Data_detect");
	void (*Data_del)(Data*) = dlsym(lib, "Data_del");

	void (*List_push_front)(List*, Data*) = dlsym(lib, "List_push_front");
	void (*List_push_back)(List*, Data*) = dlsym(lib, "List_push_back");

	Data* (*List_pop_front)(List*) = dlsym(lib, "List_pop_front");
	Data* (*List_pop_back)(List*) = dlsym(lib, "List_pop_back");

	Data* (*List_find)(List*, char*) = dlsym(lib, "List_find");
	void (*List_sort)(List*) = dlsym(lib, "List_sort");
	void (*List_print)(List*) = dlsym(lib, "List_print");

	List* (*List_new)() = dlsym(lib, "List_new");
	void (*List_del)(List* list) = dlsym(lib, "List_del");

	// Skonczylismy dodawac funckje z bilioteki
	printf(ANSI_COLOR_YELLOW "Skoczono import funkcji z biblioteki.\n" ANSI_COLOR_RESET);

	#pragma GCC diagnostic pop

	// Struktura dla times
	struct tms timesStruct;

	// Zapamietuje czas poczatkowy
	clock_t startTime = clock();

	printf(ANSI_COLOR_MAGENTA "----PROGRAM ZACZYNA SIE----\n" ANSI_COLOR_RESET);

	//Wypisuje czasy
	printf(ANSI_COLOR_CYAN);
	printf("Czas zegara scienego: %f\n", ((double)(clock() - startTime) / (double)(CLOCKS_PER_SEC)));
	times(&timesStruct);
	printf("Czas uzytkownika: %f\n", ((double)(timesStruct.tms_utime) / (double)sysconf(_SC_CLK_TCK)));
	printf("Czas systemu: %f\n" ANSI_COLOR_RESET, ((double)(timesStruct.tms_stime) / (double)sysconf(_SC_CLK_TCK)));


	printf(ANSI_COLOR_YELLOW "Alokujemy pamiec dla 6 generycznych kontaktow oraz listy.\n" ANSI_COLOR_RESET);
	List* list = List_new();

	Data* data1 = Data_new("Andrzej", "Wolbrom", "1994-05-03", "Warszawa, Konopnicka 3", "4566123-34", "Andrzej@Wolbrom.com");
	Data* data2 = Data_new("Jacek", "Placek", "1983-09-03", "Łodz, Rycerska 5", "9563213-34", "halastra@gumail.com");
	Data* data3 = Data_new("Anna", "Anninska", "1991-01-08", "Szczecin, Morska 6", "4544221-44", "anusia@xumail.com");
	Data* data4 = Data_new("Teodor", "Tadorski", "1989-05-03", "Koszalow", "11329217-34", "lodzie@podwodne.com");
	Data* data5 = Data_new("Konrad", "Bartus", "1964-11-03", "Tczyny, Wiejska 12", "465653-14", "tczyny@wujtix.com");
	Data* data6 = Data_new("Czeslaw", "Czeplinski", "1971-01-23", "Krakow, Czarnowiejska 24", "4612323-555", "czarnowies@krakow.com");

	if (list == NULL || data1 == NULL || data2 == NULL || data3 == NULL || data4 == NULL || data5 == NULL || data6 == NULL) {
		fprintf(stderr,ANSI_COLOR_RED "Bad allocation!\n" ANSI_COLOR_RESET);
		return 1;
	}
	printf(ANSI_COLOR_GREEN"Alokacja powiodla sie.\n"ANSI_COLOR_RESET);

	//Wypisuje czasy
	printf(ANSI_COLOR_CYAN);
	printf( "Czas zegara scienego: %f\n", ((double)(clock() - startTime) / (double)(CLOCKS_PER_SEC)));
	times(&timesStruct);
	printf("Czas uzytkownika: %f\n", ((double)(timesStruct.tms_utime) / (double)sysconf(_SC_CLK_TCK)));
	printf("Czas systemu: %f\n" ANSI_COLOR_RESET, ((double)(timesStruct.tms_stime) / (double)sysconf(_SC_CLK_TCK)));


	printf(ANSI_COLOR_YELLOW "Kazdy kontakt dodajemy do listy 3000 razy.\n" ANSI_COLOR_RESET);

	for (int i = 0; i < 3000; i++) {
		List_push_back(list, data1);
		List_push_back(list, data2);
		List_push_back(list, data3);
		List_push_back(list, data4);
		List_push_back(list, data5);
		List_push_back(list, data6);
	}

	//Wypisuje czasy
	printf(ANSI_COLOR_CYAN);
	printf( "Czas zegara scienego: %f\n", ((double)(clock() - startTime) / (double)(CLOCKS_PER_SEC)));
	times(&timesStruct);
	printf("Czas uzytkownika: %f\n", ((double)(timesStruct.tms_utime) / (double)sysconf(_SC_CLK_TCK)));
	printf("Czas systemu: %f\n" ANSI_COLOR_RESET, ((double)(timesStruct.tms_stime) / (double)sysconf(_SC_CLK_TCK)));


	printf(ANSI_COLOR_YELLOW "Sortujemy liste 18000 elementowa sortowaniem przez wstawianie.\n" ANSI_COLOR_RESET);
	List_sort(list);

	//Wypisuje czasy
	printf(ANSI_COLOR_CYAN);
	printf( "Czas zegara scienego: %f\n", ((double)(clock() - startTime) / (double)(CLOCKS_PER_SEC)));
	times(&timesStruct);
	printf("Czas uzytkownika: %f\n", ((double)(timesStruct.tms_utime) / (double)sysconf(_SC_CLK_TCK)));
	printf("Czas systemu: %f\n" ANSI_COLOR_RESET, ((double)(timesStruct.tms_stime) / (double)sysconf(_SC_CLK_TCK)));


	printf(ANSI_COLOR_YELLOW "Wykonujemy 2000 wyszukiwan liniowych w liscie.\n" ANSI_COLOR_RESET);
	for (int i = 0; i < 1000; i++) {
		List_find(list, "Koszalow");
		List_find(list, "czarnowies@krakow.com");
	}

	//Wypisuje czasy
	printf(ANSI_COLOR_CYAN);
	printf( "Czas zegara scienego: %f\n", ((double)(clock() - startTime) / (double)(CLOCKS_PER_SEC)));
	times(&timesStruct);
	printf("Czas uzytkownika: %f\n", ((double)(timesStruct.tms_utime) / (double)sysconf(_SC_CLK_TCK)));
	printf("Czas systemu: %f\n" ANSI_COLOR_RESET, ((double)(timesStruct.tms_stime) / (double)sysconf(_SC_CLK_TCK)));


	printf(ANSI_COLOR_YELLOW "Dealokujemy pamiec w tym liste. Kazdy node posiada osobna kopie generycznego kontaktu.\n" ANSI_COLOR_RESET);

	Data_del(data1);
	Data_del(data2);
	Data_del(data3);
	Data_del(data4);
	Data_del(data5);
	Data_del(data6);

	List_del(list);

	//Wypisuje czasy
	printf(ANSI_COLOR_CYAN);
	printf( "Czas zegara scienego: %f\n", ((double)(clock() - startTime) / (double)(CLOCKS_PER_SEC)));
	times(&timesStruct);
	printf("Czas uzytkownika: %f\n", ((double)(timesStruct.tms_utime) / (double)sysconf(_SC_CLK_TCK)));
	printf("Czas systemu: %f\n" ANSI_COLOR_RESET, ((double)(timesStruct.tms_stime) / (double)sysconf(_SC_CLK_TCK)));

	printf(ANSI_COLOR_MAGENTA "----PROGRAM KONCZY DZIALANIE----\n" ANSI_COLOR_RESET);

	printf(ANSI_COLOR_YELLOW"Zamykamy biblioteke.\n"ANSI_COLOR_RESET);
	if (dlclose(lib) != 0) {
		fprintf(stderr,ANSI_COLOR_RED "Blednie zamknieto biblioteke!\n" ANSI_COLOR_RESET);	
	}
	printf(ANSI_COLOR_GREEN"Biblioteka zamknieta poprawnie.\n"ANSI_COLOR_RESET);
	return 0;
}
