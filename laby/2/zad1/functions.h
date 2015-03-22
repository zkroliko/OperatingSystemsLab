#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#ifndef std
#include <stdio.h>
#include <stdlib.h>
#endif

#include <time.h>
#include <sys/times.h>

// Deklaracje
unsigned char compare(char* first, char* second);
void printKeys(char* filename, int size);
int copy_file(char *old_filename, char  *new_filename);
void testStdio(char* filename, int size);
void testUnistd(char* filename, int size);
