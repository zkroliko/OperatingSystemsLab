#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

// Funkcja do obslugi sygnalu sigint

void sigint_handler () {
	fprintf(stderr, "Serwer konczy prace. Kolejka pozostaje w systemie!\n");
	exit(0);
}

// Tablice do przechowywania klientow

int clients[CLIENTS_MAX];
char* client_names[CLIENTS_MAX];
int count;

int main(int argc, char* argv[]) {

	// Ustwiamy obsluge sygnalow

	// Maska sygnalow
	sigset_t set;
	sigemptyset (&set);
	sigaddset(&set, SIGINT);

	struct sigaction action;
	action.sa_handler = sigint_handler;
	action.sa_mask = set;
	
	if (sigaction(SIGINT, &action, NULL) < 0) {
		fprintf(stderr, "Blad przy ustawianiu procedury obslugi sygnalu!\n");
		exit(-1);			
	}

	// Alokujemy pamiec dla nazw klientow
	for (int i = 0; i < CLIENTS_MAX; i++) {
		if ((client_names[i] = malloc(sizeof(char)*NAME_MAX_LENGTH)) == NULL)	{
			perror("Blad alokacji pamieci dla nazw uzytkownikow");
			return -1;
		}	
	}

	// Teraz zajmiemy sie kolejkami
	
	CMessage client_message;
	Message message;
	int msgq_id;
	count = 0;
	// Tworzymy kolejke
	if ((msgq_id = msgget(MAIN_KEY, IPC_CREAT | 0666)) < 0)	{
		perror("Blad przy tworzeniu kolejki po stronie serwera.\n");
		exit(-1);
	}
	
	// Bedziemy w kolko iterowac po klientach
	while (1) {
		if((msgrcv(msgq_id, &client_message, sizeof(CMessage)-sizeof(long), TO_SERVER_JOIN, IPC_NOWAIT)) > 0) {
			// Zapisujemy numer kolejki klienta oraz jego nazwe
			clients[count] = client_message.queue_id;
			memcpy(client_names[count], &(client_message.sender), NAME_MAX_LENGTH);
			printf("Uczestnik o nicku %s wszedl do czatu\n", client_names[count]);
			count++;
		}
		if((msgrcv(msgq_id, &client_message, sizeof(CMessage)-sizeof(long), TO_SERVER_LEAVE, IPC_NOWAIT)) > 0) {
			printf("Uzytkownik o nicku %s wyszedl z czatu\n", client_message.sender);
		}

		for (int i = 0; i < count; i++) {
			// Odbieramy wiadomosci i wysylamy je do klientow
			if((msgrcv(clients[i], &message, sizeof(Message)-sizeof(long), TO_SERVER, IPC_NOWAIT)) >= 0) {
				send(&message);
			}
		}
			
	}
	exit(0);
}

// To nam wysyla wiadomosc
int send(Message* msg) {
	msg->mtype = TO_CLIENT; // Zmieniamy typ wiadomosci na wyjsciowe
	// Iterujemy po wszystkich klientach i rozsylamy wiadomosci
	for (int i = 0; i < count; i++) {
		if ((msgsnd(clients[i], msg, sizeof(Message)-sizeof(long), 0) < 0)) {
			if (errno == EIDRM || errno == EINVAL) {
				// Ta kolejka juz zostala usunieta
			} else {
				perror("Blad przy wysylaniu wiadomosci.\n");
				exit(-1);
			}
		}
	}
	return 0;
}
