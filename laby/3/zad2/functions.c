#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif


// Funckja, ktora wykonuje cala prace w programie
int count(char* filePath, short flag)
{
	// Liczniki
	int counter = 0;
	// Struktura dirent
	struct dirent* dent;
	// Deskryptor pliku
	DIR* dir;
	// Struktury dla stat
	struct stat stat;
	
	int pid;
	// Dlugosc sciezki
	int len = strlen(filePath);
	char filename[FILENAME_MAX];

	
	// Sprawdzamy dlugosc nazwy pliku
	if ( len >= FILENAME_MAX) {
		fprintf(stderr, "Blad! Za dluga nazwa pliku!\n");
		exit(-1);
	}
	
	// Uzywamy funkcji lstat
	if ( lstat(filePath, &stat) < 0) {
		fprintf(stderr, "Blad w funkcji lstat!\n");
		exit(-1);
	}
	
	// Sprawdzamy czy jest katalogiem
	if ( !(S_ISDIR(stat.st_mode))) {
		exit(-1);
	}
	
	// Teraz kopiujemy nazwe folderu
	strcpy(filename, filePath);
	filename[len++] = '/';
	
	// Otwieramy katalog
	if (!(dir = opendir(filePath)))	{
		fprintf(stderr, "Blad przy otwieraniu katalogu: %s\n", filePath);
		exit(-1);
	}
	
	// Struktura dla lstata w katalogu
	struct stat stat_iteration;

	// Przebieg po katalogu
	while ((dent = readdir(dir))) {
		if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) {
			// Do pominiecia . oraz ..
			continue;
		}
		strcpy(filename+len, dent->d_name);
		
		// Wykonujemy lstat na kazdym pliku w katalogu
		if (lstat(filename, &stat_iteration) < 0) {
			fprintf(stderr, "Blad w funkcji lstat!\n");
			exit(-1);
		}
		
		// Sprawdzamy czy dany plik w katalogu nie jest katalogiem
		if (!(S_ISDIR(stat_iteration.st_mode))) {
			counter++;
		}
		else {
			// Jesli nie to wykonujemy funckje fork
			if ((pid = fork()) < 0)	{
				fprintf(stderr, "Blad w funkcji fork!\n");
				exit(-1);
			}
			else if (pid == 0) {
				// Proces potomny
				if (flag == 1) {
					// Wykonujemy z oczekiwaniem
					execl("./file_counter", "file_counter", "-w", filename, NULL);
				}
				else {
					execl("./file_counter", "file_counter", filename, NULL);
				}
			}
			
		}
		
	}

	// Jezeli to tryb -w to spimy
	if (flag == 1) sleep(20);
	
	// Teraz bedziemy zbierac liczniki od dzieci
	int status;
	int child_counter = 0;	

	// Zbieramy od dzieci
	while((pid = wait(&status)) > 0) {
		child_counter += WEXITSTATUS(status);
	}
	
	printf("%s: Liczba plikow w: %d  Liczba plikow w sumie: %d\n",
			filePath, counter, child_counter+counter);
	
	// Wychodzimy normalnie
	exit(child_counter+counter);
}
