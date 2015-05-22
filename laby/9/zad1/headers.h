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

#include<sys/mman.h>
#include<semaphore.h>


// Jakies stale konfiguracyjne

#define N_PHILO 5
#define N_EAT_TIMES 100
#define US_EAT_TIME 1500
#define US_EAT_TIME_VAR 4000
#define US_THINK_TIME 3000
#define US_THINK_TIME_VAR 8000

// Struktury

// Struktura monitora
typedef struct myMonitor {	
	pthread_mutex_t* mutex;
	pthread_cond_t forks[N_PHILO];	
	pthread_cond_t waiter;	
	int fTaken[N_PHILO];
	// Ta zmienna okresla ilu filozofow chce teraz zjesc
	int nEating;
} myMonitor;

// Struktura danych
typedef struct data_t {
	int philo;
	int left;
	int right;
	myMonitor* monitor;
} data_t;

// Naglowki funkcji

void* philosopher (void* data);
data_t* init_data (myMonitor* monitor);
myMonitor* init_monitor ();
void eat();
void think();
