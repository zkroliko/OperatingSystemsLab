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
// SVR 5
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//Klucz
#include "key.h"

//Struktura

struct mymsg {
        long    mtype;
	time_t	time;
        char	mtext[100];
};


