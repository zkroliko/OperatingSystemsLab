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
#include<sys/ipc.h>

#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>

#define SHM_NAME "/tmp"
#define SM_NAME "/"
#define BUFFER_SIZE 120

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


	/* Rozpiska semaforow
	0 - empty - ile jest wolny
	1 - full - ile jest pelnych
	2 - take 
	3 - put
	*/ 

// Teraz troche makrodefinicji

#define FULL 0
#define EMPTY 1
#define TAKE 2
#define PUT 3


// Definicje od spania

#define WAIT_BASE 300
#define WAIT_VARY 2000
