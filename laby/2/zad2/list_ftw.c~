#include "list_ftw.h"

#define MAX_FILES 512

// Zmienna globalna do zapamietywania uprawnien
// Niestety trzeba tak zrobic, bo funkcja ftw, nie przyjmie "dodatkowego pasazera"
char* accessMode;

int main(int argc, char* argv[]) {
	// Sprawdzamy ile podano argumentow
	if (argc != 3)
	{
		fprintf(stderr, "Podano zla liczbe argumentow!\n");
		fprintf(stderr, "Prosze podac w formacie: ftw <sciezka> <prawa dostepu>!\n");
		return -1;
	}
	
	// Takze czy podano uprawnienia w poprawnym foramcie
	if (parse_access(argv[2]) != 0)
	{
		fprintf(stderr, "Blad przy parsowaniu uprawnien!\n");
		fprintf(stderr, "Prosze podac uprawnienia w formacie <<***><***><***>> * := r|w|x|-, podobnie jak w polecaniach systemowych\n");
		return -1;
	}
	
	// Zapisujemy do odpowiednich zmiennych
	accessMode = argv[2];
	char* filePath = argv[1];
	
	// Deklarujemy wskaznik na zmienna
	int  (*fPointer)(const char*, const struct stat*, int);
	fPointer = (&get_statistics); // Adres funkcji
		
	if (ftw(filePath, fPointer, MAX_FILES) != 0) {
		fprintf(stderr, "Blad w funkcji ftw!\n");
		return -1;
	}

	return 0;
}

// Ta funkcje przekazujemy funkcji ftw, ktora przejdzie przez wszystkie katatalogi
int get_statistics(const char* dirPath, const struct stat* st, int flag) {
	
	// Sprawdzamy flage poprawnego odczytu katalogu
	// gdy np. nie mamy uprawnien
	if (flag == FTW_DNR) {
		fprintf(stderr, "Blad przy otwarciu katalogu %s !\n", dirPath);
		return -1;
	}
	
	// Sprawdzamy czy to plik zwykly
	if (flag == FTW_F) {
		// Wczytujemy czas modyfikacji ze struktury
		time_t t = st->st_mtime;
		struct tm time;
		char timeString[90];
		time = *localtime(&t);
		// Funkcja od formatowania czasu w poprwawnej formie
		strftime(timeString, sizeof(timeString), "%a %Y-%m-%d %H:%M:%S %Z", &time);
		// Sprawdzamy czy uprawnienia sa prawidlowe
		if (check(accessMode, *st)) {
			printf("%s %d %s\n", dirPath, (int) st->st_size, timeString);
		}
		
		return 0;
	}
	return 0;
}
