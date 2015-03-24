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
	char strtime[80];
	
	// Odtworzenie w odpowiedni sposob sciezki
	// cofamy sie az do znalezenia odpowiedniej sciezki
	char* c = dirPath+strlen(dirPath)-1;
	while(*c != '/') {
		c--;
	}
	
	timeStruct = *localtime(&time);
	strftime(strtime, sizeof(strtime), "%a %Y-%m-%d %H:%M:%S %Z", &timeStruct);

	printf("%s: %d, %s\n", c+1, (int) st->st_size, strtime);
	return 0;
}

// Funkcja rekurencyjna do przechodzenia po drzewie katalogow
int traverse(char* dirPath, char* mod) {

	struct dirent* dent;
	DIR* dir;
	struct stat st;
	char filename[FILENAME_MAX];
	int len = strlen(dirPath);
	
	if (len >= FILENAME_MAX - 1) {
		fprintf(stderr,"Za dluga nazwa jednego z pliku w drzewie !\n");
		return -1;
	}
	if (lstat(path, &st) < 0) {
		fprintf(stderr, "Blad funkcji lstat!\n");
		return -1;
	}
	if (S_ISREG(st.st_mode)) {
		if (check(mod, st)) {
			return get_statistics(dirPath, &st);
		}
		return 0;
	}
	
	// Sprawdzamy czy to nie przypadkiem cos innego niz katalog ani plik zwykly
	if (!(S_ISDIR(st.st_mode))) {
		 return 0; // jesli to nie jest katalog ani zwykly plik to nic nie robimy
	}
	
	// Tworzymy nowa sciezke do katalogu
	strcpy(filename, dirPath);
	filename[len++] = '/';
	
	if (!(dir = opendir(dirPath))) {
		fprintf(stderr, "Blad przy otwieraniu folderu: %s !\n", path);
		return 0;
	}
	
	while ((dent = readdir(dirPath))) {
		if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) continue;
		strcpy(filename+len, dent->d_name);
		if (traverse(filename, mod) != 0) break;
	}
	
	if (closedir(dirPath) < 0)	{
		fprintf(stderr, "Blad przy zamknieciu katalogu %s !\n", filename);
		return -1;
	}
	
	return 0;
	
}

