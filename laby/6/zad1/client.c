#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

// Niestety to musi byc globalne

// Argument linii wywolan
char name[NAME_MAX_LENGTH];

// Zmienne przechowywujace identyfikatory
int u_queue_id;
int s_queue_id;

// Funkcja do obslugi sygnalu sigint

void sigint_handler () {
	koncz();
}

int main(int argc, char* argv[]) {

	// Sprawdzenie argumentow
	if (argc != 2 ) {
		if (argc > 2 ) {
			perror("Podano za duzo argumentow. Prosze podac tylko nazwe klienta");
		} else {
			perror("Nie podano arguemntu. Prosze podac nazwe klienta");
		}
		return -1;		
	}
	
	// Kopiujemy nazwe klienta do nowego miejsca w pamieci
	strncpy(name, argv[1], NAME_MAX_LENGTH);

	// Tworzymy indywidualna kolejke dla klienta
	if ((u_queue_id = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666)) < 0) 	{
		if (errno == EEXIST) {
			perror("Blad przy tworzeniu kolejki. Kolejka juz istnieje.\n");
			exit(-1);
		}
		else {
			perror("Blad przy tworzeniu kolejki.\n");
			exit(-1);
		}
	}
	
	// Otwieramy kolejke serwera
	if ((s_queue_id = msgget(MAIN_KEY, IPC_CREAT  | 0666)) < 0) {
		perror("Blad przy otwieraniu kolejki serwera.\n");
		exit(-1);
	}
	
	// Wysylamy wiadomosc do serwera informujaca o wejsciu nowego klienta do czatu
	CMessage cmessage;
	memcpy(cmessage.sender, name, sizeof(name));	
	cmessage.mtype = TO_SERVER_JOIN;
	cmessage.queue_id = u_queue_id;
	
	if (msgsnd(s_queue_id, &cmessage, sizeof(CMessage)-sizeof(long), 0) < 0) {
		perror("Blad wyslania wiadomosci.\n");
		exit(-1);
	}
	
	/* -----------------------------
	Potrzebujemy watku do obslugi wysylania wiadomosci,
	oraz do odbierania wiadomosci. Zrobimy forka.
	----------------------------- */ 

	int pid;
	
	if ((pid = fork()) < 0)	{
		perror("Blad w wywolaniu funkcji fork.\n");
		exit(-1);
	}
	else if (pid == 0) {

		// Jestesmy w watku w wysylania wiadomosci

		// Ustawiamy obsluge sygnalow, teraz juz mozemy gdyz wszystko powinno byc ustawione

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
	
		// Informujemy uzytkownika

		printf("Klient gotowy do wysylania wiadomosci\n");			
		printf("> ");			


		char message[MESSAGE_MAX_LENGTH];

		while(1) {		
			fgets(message, MESSAGE_MAX_LENGTH, stdin);	
			// Sprawdzamy czy uzytkownik nie wpisał exit
			if (strcmp(message, "exit\n") == 0) {
				// Zabijamy przodka
				kill(getppid(), SIGTERM);
				// Konczymy
				koncz();
			}
			// Ustalamy parametry wiadmosci
	
			Message msg;

			// Czas
			time_t t = time(NULL);

			msg.mtype = TO_SERVER;			
			memcpy(msg.sender, name, sizeof(name));
			memcpy(msg.mchar, message, sizeof(message));
			msg.time_send = *localtime(&t);
		
			// Wyslanie
			if (msgsnd(u_queue_id, &msg, sizeof(Message)-sizeof(long), 0) < 0)	{
				perror("Blad przy wysylaniu wiadomosci.\n");
				exit(-1);
			}
		}
	}
	else {
		// Jestesmy w watku odbierania wiadomosci
		while(1) {
			Message input_msg;
			if((msgrcv(u_queue_id, &input_msg, sizeof(Message)-sizeof(long), TO_CLIENT, 0)) < 0) {
				perror("Blad przy odbioraniu wiadomosci.\n");
				exit(-1);
			}		
			// Wypisywanie wiadomosci
			// -----------------------------------
			// Formatujemy czas
			time_t time = mktime(&(input_msg.time_send));
			char* time_string = ctime(&time);
			// Musze usunac newline z konca
			strtok(time_string, "\n");
			// Wypsisujemy tresc wiadomosci
			fprintf(stdout, "%s ", time_string);
			fprintf(stdout, "%s : ", input_msg.sender);
			fprintf(stdout, "%s", input_msg.mchar);
			// -----------------------------------
			// Znak zachety						
			printf("> ");	
			fflush(stdout);		
		}
	}		
}


void koncz () {
	// Konczymy prace oraz czyscimy po sobie
	// Usuwamy kolejke
	if (msgctl(u_queue_id, IPC_RMID, NULL) < 0) {
		perror("Blad przy usuwaniu kolejki.\n");
		exit(-1);
	}
	// Wysylamy wiadmosc o koncu transmisji
	CMessage msg;
	msg.mtype = TO_SERVER_LEAVE;
	memcpy(msg.sender, name, sizeof(name));
	msg.queue_id = u_queue_id;
	if (msgsnd(s_queue_id, &msg, sizeof(CMessage)-sizeof(long), 0) < 0) {
		perror("Blad przy wyslaniu wiadomosci.\n");
		exit(-1);
	}
	fprintf(stdout, "Opuszczam czat...\n");
	exit(0);				
}

