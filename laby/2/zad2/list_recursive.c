#include "list_recursive.h"

int main(int argc, char* argv[])
{
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
	
	// Zapisujemy argumenty linii polecen
	char* file_path = argv[1];
	char* access_mode = argv[2];
	
	traverse(file_path, access_mode);
	return 0;
}

// Ta funkcja wypisuje nam statystyki danego pliku
int get_statistics(char* dirPath, struct stat* st) {

	// Czas 
	time_t time = st->st_mtime;
	struct tm timeStruct;
	char timeString[90];
	
	// Odtworzenie w odpowiedni sposob sciezki
	// cofamy sie az do znalezenia odpowiedniej sciezki
	char* c = dirPath+strlen(dirPath)-1;
	while(*c != '/') {
		c--;
	}
	
	// Ladne wypisanie czasu
	timeStruct = *localtime(&time);
	strftime(timeString, sizeof(timeString), "%a %d-%m-%Y %H:%M:%S %Z", &timeStruct);

	printf("%s %d %s\n", c+1, (int) st->st_size, timeString);
	return 0;
}

// Funkcja rekurencyjna do przechodzenia po drzewie katalogow
int traverse(char* dirPath, char* mod) {

	// Dlugosc sciezki, ktora otrzymalismy w argumencie
	int pathLenght = strlen(dirPath);	

	// Struktura dla lstat
	struct stat statStruct;

	// Bufor na nazwe pliku
	char filename[FILENAME_MAX];
	
	// Strumien do katalogu
	DIR* directory;
	// Potrzebne do czytania katalogu
	struct dirent* dent;
	
	// Sprawdzamy dlugosc nazwy pliku
	if (pathLenght >= FILENAME_MAX - 1) {
		fprintf(stderr,"Za dluga nazwa jednego z pliku w drzewie !\n");
		return -1;
	}
	// Uruchamiamy lstat, zapisze to nam strukture
	if (lstat(dirPath, &statStruct) < 0) {
		fprintf(stderr, "Blad funkcji lstat!\n");
		return -1;
	}
	// Spradzamy czy to plik zwykly
	if (S_ISREG(statStruct.st_mode)) {	
		// Jezeli tak to sprawdzamy uprawnienia
		if (check(mod, statStruct)) {
			return get_statistics(dirPath, &statStruct);
		}
		return 0;
	} else if (!(S_ISDIR(statStruct.st_mode))) {
		// Jezeli to nie jest plik regularny ani katalog to wychodzimy
		 return 0;
	}
	
	// Kopiujemy argument funckji do naszej sciezki
	strcpy(filename, dirPath);
	// Dodajemy na koniec / 
	filename[pathLenght++] = '/';
	
	// Otwieramy nasz strumien
	if (!(directory = opendir(dirPath))) {
		fprintf(stderr, "Blad przy otwieraniu folderu: %s !\n", dirPath);
		return 0;
	}
	
	while ((dent = readdir(directory))) {
		// Sprawdzamy czy to nie przypadkiem "." lub ".."
		if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) {
			continue;
		}
		// Dopisujemy na koniec sciezki biezacego katalogu, nazwe podkatalogu
		strcpy(filename+pathLenght, dent->d_name);
		if (traverse(filename, mod) != 0) {
			break;
		}
	}
	
	// Teraz zamykamy strumien
	if (closedir(directory) < 0)	{
		fprintf(stderr, "Blad przy zamknieciu katalogu %s !\n", filename);
		return -1;
	}
	
	return 0;
	
}

