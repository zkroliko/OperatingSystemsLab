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

#include<pthread.h>

#define MAXLENGTH 200
#define MAX_THREAD_COUNT 100
#define RECORDSIZE 1024

// Funkcja, ktora bedzie wykonywac watek
void* search(void*);
// Destruktor dla watku
static void cleanUp(void*);
// Funkcja inicjalizujaca klucz dla watkow
static void make_keys(void);

typedef struct node_data {
	// Poczatek fragmentu
	int start;	
	// Liczba rekordow do przeszukania na raz	
	int record_count;
	// Miejsce, ktore pozniej ma przeszukac watek
	int next;		
} node_data;
