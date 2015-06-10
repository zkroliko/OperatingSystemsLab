#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <arpa/inet.h>

#define DATA_MAX_SIZE 256 
#define USERNAME_LENGTH_MAX 100
#define UNIX 0
#define WEB 1

// Funkcje

void *get_in_addr(struct sockaddr *sa);
