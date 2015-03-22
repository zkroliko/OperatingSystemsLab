#include "functions.h"

//Ten program opakowuje tylko funkcje sortowania z uzyciem funkcji systemowych w argumenty linii polecen
int main(int argc, char* argv[]) {
	sortSys(argv[1], atoi(argv[2]));
	return 0;
}
