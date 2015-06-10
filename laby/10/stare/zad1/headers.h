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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 

#define MAX_SIZE 512;

void* consoleInput (void* data);
