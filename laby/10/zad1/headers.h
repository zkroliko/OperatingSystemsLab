#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define USERSIZE 80
#define MAX_MESSAGE_SIZE 256
#define MAXCLIENTS 20

#define UNIX_CLIENT 0
#define WEB_CLIENT 1

#define REGISTER 0
#define MESSAGE 1


typedef struct client {
	char username[USERSIZE];
	struct sockaddr address;
	int active;
	int comtype;
    	time_t timestamp;
} client;

typedef struct message {
	char username[USERSIZE];
	char message[MAX_MESSAGE_SIZE];
	int type;
	int comtype;
    	int clientType;
} message;


void client_register(int signo);
void message_send(int signo);
