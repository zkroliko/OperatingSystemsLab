#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

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

	// Struktura dla times
	struct tms timesStruct;

	// Zapamietuje czas poczatkowy
	clock_t startTime = clock();

	printf(ANSI_COLOR_MAGENTA "----PROGRAM ZACZYNA SIE----\n" ANSI_COLOR_RESET);

	//Wypisuje czasy
	printf(ANSI_COLOR_CYAN);
	printf( "Czas zegara scienego: %f\n", ((double)(clock() - startTime) / (double)(CLOCKS_PER_SEC)));
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

	return 0;
}
