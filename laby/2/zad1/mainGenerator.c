#include "generator.h"

//Ten program opakowuje tylko funkcje generate w argumenty linii polecen
int main(int argc, char* argv[]) {
	generate(argv[1], atoi(argv[2]), atoi(argv[3]));
	return 0;
}
