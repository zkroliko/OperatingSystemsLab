#ifndef COMMON 
	#define COMMON
	#include "common.h"
#endif

int get_statistics(char* dirPath, struct stat* st);
int traverse(char* path, char* access_mode);

