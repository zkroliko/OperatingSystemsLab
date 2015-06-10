#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

// Funkcja wspolna takze umieszczona ja w osobnym pliku
// znaleziona w internecie

void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
