#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int parseOption (char* option);
int setread (char* path , long unsigned position, struct flock* lock);
int setwrite (char* path , long unsigned position, struct flock* lock);
int printl (char* path , struct flock* lock);
int release (char* path , long unsigned position, struct flock* lock);
int readc (char* path , long unsigned position);
int writec (char* path , long unsigned position, char* character);

