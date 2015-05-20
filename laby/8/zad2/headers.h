#define _POSIX_SOURCE
#define _DEFAULT_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/types.h>

#include<sys/mman.h>
#include<semaphore.h>

// Struktury

union semafore {
	int val;
	struct semid_ds* buf;
	ushort* array;
};

// Naglowki funkcji

void producent();
void consument();
int isPrime(unsigned int);
void wait(int base, int vary);

// Troche definicji

#define SHM_NAME "/tmp"

#define SM_TAKE "/take"
#define SM_PUT "/put"
#define SM_EMPTY "/empty"
#define SM_FULL "/full"
#define MAX_LENGTH 20


// Definicje od spania

#define WAIT_BASE 300
#define WAIT_VARY 2000
