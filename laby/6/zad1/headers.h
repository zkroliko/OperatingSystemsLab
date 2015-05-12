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
#include <string.h>
// SVR 5
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//Klucz
#define MAIN_KEY 1001L 

//Struktura od wiadomosci

#define MESSAGE_MAX_LENGTH 500
#define NAME_MAX_LENGTH 50
#define CLIENTS_MAX 80

typedef struct{
	long mtype;
	struct tm time_send;
	char sender[NAME_MAX_LENGTH];
	char mchar[MESSAGE_MAX_LENGTH];
} Message;

typedef struct{
	long mtype;
	char sender[NAME_MAX_LENGTH];
	int queue_id;
} CMessage;

//Inne naglowki
int send(Message* msg);
void koncz ();

// Takie tam do oznaczania typow wiadomosci
#define TO_SERVER 1
#define TO_CLIENT 2
#define TO_SERVER_JOIN 3 
#define TO_SERVER_LEAVE 4
