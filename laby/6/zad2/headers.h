#define _GNU_SOURCE 
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/resource.h> 
#include <sched.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <mqueue.h>

#define MAX_CLIENTS_NUM 40
#define MAX_NAME_LENGHT 100

#define REGISTER SIGUSR1 
#define MESSAGE SIGUSR2

// Struktura od klienta

typedef struct {
	mqd_t queue;
	char name[MAX_NAME_LENGHT];
	struct sigevent sigev;
} Client;

// Naglowki funkcji

void messageHandler(int signo, siginfo_t *info, void* context);
void clientRegisterHandler(int signum);

void exitFunction();
void intHandler(int signo);

void exitFunctionClient();
void termHandlerClient(int sgn);

#define DEBUG
