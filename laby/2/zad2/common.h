// Te pliki naglokowe, ktore wystepuja w obydwoch wariantach
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>
#include <string.h>

// Deklaracje tych funkcji, ktore sa doklanie w tej samej postaci w obu wariantach

int check(char* OurAccessMode, struct stat st);
int parse_access(char* accessMode);
