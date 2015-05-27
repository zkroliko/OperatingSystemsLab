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
#include <signal.h>

#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>

#include<pthread.h>

#include<sys/mman.h>
#include<semaphore.h>

// To okresla szybkosc dzialania programu

#define SLEEP_TIME 1000*1000*2


// Jakies stale konfiguracyjne

#define N_READERS 20
#define MAX_N_WRITERS 50

#define N_NUMBERS 15000
#define MAX_VALUE 1000

#define MAX_CHANGED 150

// Semafory

#define SM_READERS "/readers"
#define SM_WRITERS "/writers"
#define SM_MONITOR "/monitor"

// Struktura monitora

typedef struct myMonitor {	
	sem_t* readers;	
	sem_t* writers;		
	sem_t* access;	
	// Semafory
	int nWriting, nReading;
	// Tablica z liczbami
	int numbers[N_NUMBERS];
	// Poszukiwana liczba, tylko dla czytelnika
	int number;
} myMonitor;

// Naglowki funkcji

void* writer (void* monitor);
void* reader (void* monitor);
void printNumbers (myMonitor* monitor);
myMonitor* init_monitor ();
void clean();
