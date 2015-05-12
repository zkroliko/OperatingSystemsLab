#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int main (int argc, char* argv[]) {
	key_t   mykey;
        mykey = ftok("/tmp/myapp", KEY);
	return 0;
}
