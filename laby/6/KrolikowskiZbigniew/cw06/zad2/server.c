#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

struct sigevent sigev;
mqd_t serverQ;

// Klienci
Client clients[MAX_CLIENTS_NUM];

int clientsCount = 0;
int activeClients=0;
int firstClient=1;

int main(){

	// Uzywamy funkcji atexit 
	// tak bedzie prosciej bo czynnosc ta
	// bedzie wykonana niezaleznie od 
	// sposobu zakonczenia procesu
	atexit(exitFunction);

	// Ustawiamy obsluge syngalow
	signal(SIGINT, intHandler);

	// Ustawiamy parametry wiadomosci
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 9;
	attr.mq_msgsize = 128;

	// Otwieramy kolejke od serwera
	if((serverQ = mq_open("/server_queue", O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &attr))==-1){
		perror("Blad przy otwieraniu kolejki od serwera\n");
		exit(1);
	}

	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;

	// Ustawienie procedury obslugi sygnalow
	
	sigset_t registerSet;
	sigemptyset(&registerSet);
	sigaddset(&registerSet,SIGUSR1);

	struct sigaction clientAction;
	clientAction.sa_handler = clientRegisterHandler;
	clientAction.sa_mask = registerSet;

	// Teraz odpadlamy sigaction na usr1
	if(sigaction(SIGUSR1, &clientAction, NULL) == -1){//obsluga sygnalu dodania klienta
		perror("Blad przy wykonaniu procedury sigaction\n");
		exit(1);
	}

	// Rejestrujemy sie na kolejke
	if(mq_notify(serverQ, &sigev) == -1){//rejestracja na kolejke
		perror("Blad przy rejestracji na kolejke\n");
		exit(1);
	}

	// Ustawienie procedury obslugi sygnalow

	sigset_t messageSet;
	sigemptyset(&messageSet);
	sigaddset(&messageSet, SIGUSR2);

	struct sigaction messageAction;
	messageAction.sa_sigaction = messageHandler;
	messageAction.sa_mask = messageSet;
	messageAction.sa_flags = SA_SIGINFO;

	// Sigaction na SIGUSR2
	if(sigaction(SIGUSR2, &messageAction, NULL) ==-1){
		perror("sigaction error");
		exit(1);
	}

	// Petla, ktora bedzie chodzic az nie wyczerpia sie nam klienci 
	while( activeClients > 0 || firstClient == 1) {
		pause();		
	}
	printf("Koniec dzialania serwera.\n");
	mq_unlink("/server_queue");

	return 0;
}


void clientRegisterHandler(int signum){
	if( signum == REGISTER){
		if(clientsCount >= MAX_CLIENTS_NUM){
			printf("\nPrzekroczono ilosc uzytkownikow\n");
			return;
		}

		// Odbieramy wiaomosc zawierajaca nazwe klienta
		if(mq_receive(serverQ, clients[clientsCount].name, 128, NULL) == -1){
			fprintf(stderr, "Blad przy odbieraniu nazwy kolejki");
			exit(1);
		}
		char clientQname[128 + 1];

		#ifdef DEBUG						
			fprintf(stderr, "Jest %d klientow. Klient o nazwie %s pragnie sie zarejestrowac\n", clientsCount, clients[clientsCount].name);
		#endif

		sprintf(clientQname, "/%s_queue", clients[clientsCount].name);

		// Otwieramy kolejke
		if((clients[clientsCount].queue = mq_open(clientQname, O_RDWR))==-1){
			fprintf(stderr,"Blad przy otwieraniu kolejki %s \n", clientQname);
			exit(1);
		}

		#ifdef DEBUG						
			fprintf(stderr, "Kolejka klienta o nazwie %s zostala otworzona \n", clients[clientsCount].name);
		#endif

		clients[clientsCount].sigev.sigev_notify = SIGEV_SIGNAL;
		clients[clientsCount].sigev.sigev_signo = SIGUSR2;

		union sigval val;
		val.sival_int = clientsCount;
		clients[clientsCount].sigev.sigev_value = val;

		// Rejestracja na kolejke klienta
		if(mq_notify(clients[clientsCount].queue, &clients[clientsCount].sigev)==-1){
			fprintf(stderr, "Blad przy rejestracji sie na kolejeke\n");
			exit(1);
		}

		// Sformatujemy sobie najpierw czas
		time_t receiveTime = time(NULL);
		char* timeString = ctime(&receiveTime);				
		strtok(timeString, "\n");

		printf("%s Uzytkownik od nicku %s wszedl do czatu \n", timeString,clients[clientsCount].name);

		// Zwiekszamy ilosc aktualnych klientow
		activeClients++;
		if(firstClient==1) {
			firstClient=0;
		}

		// Ponowna rejestracja na kolejke nazw
		if(mq_notify(serverQ, &sigev) == -1){
			perror("Blad przy notify");
			exit(1);
		}

		// Na wiadomosc
		char message[128+1];

		// To zapisuje wiadomosc w odpowiednim formacie
		sprintf(message, "%s Uzytkownik od nicku %s wszedl do czatu\n",timeString, clients[clientsCount].name);		

		// Teraz rozsylamy wiadomosci
		for(int i=0; i<= clientsCount; i++){
			#ifdef DEBUG						
				fprintf(stderr, "Wysylam do %s wiadomosc o wejsciu jakiegos klienta do czatu \n", clients[clientsCount].name);
			#endif

			if(mq_send(clients[i].queue, message, strlen(message) + 1, (unsigned int) 0) ==-1 && errno != EBADF){
			
					fprintf(stderr, "Blad przy wysylaniu wiadomosci do %s ", clients[i].name );
					exit(1);
			
			}
		}
		clientsCount++;
	}


}

void messageHandler(int signo, siginfo_t *info, void* context){
	// Bedziemy obslugiwac wiadomosci wysylane od klienta
	if(signo == MESSAGE){						
			int clientNo = info->si_value.sival_int; //Sprawdzamy ktorego klienta dotyczy wiadomosc
			int prio = -1, needToNotify = 1;
			struct mq_attr attr;
			#ifdef DEBUG						
			fprintf(stderr, "Odebralem wiadomosc od %s\n", clients[clientNo].name);
			#endif
			
			// Zbierzmy atrubuty tej wiadomosci
			if(mq_getattr(clients[clientNo].queue, &attr) ==-1){
				perror("Blad funkcji mq_getattr");
				exit(1);
			}
			char buff[attr.mq_msgsize];
			// Czyscimy pamiec
			memset(buff,0,strlen(buff));
			// Odbieranie wiadomosci
			if(mq_receive(clients[clientNo].queue, buff, attr.mq_msgsize, (unsigned int*) &prio) ==-1){
				fprintf(stderr, "Blad odbierania wiadmosci przez %s \n", clients[clientNo].name);
				exit(1);
			}
			// Sprawdzamy typ wiadomosci
			if(prio == 0){		
				// To jest od serwera do klienta, wyslemy ja z powrotem
				if(mq_send(clients[clientNo].queue, buff, attr.mq_msgsize, (unsigned int) 0) ==-1){
					fprintf(stderr, "Blad odsylania wiadomosci do %s \n", clients[clientNo].name);
					exit(1);
				}

			}
			else if(prio > 0){
				// To jest prawidlowa wiadomosc

				char message[128+1];

				// Sformatujemy sobie najpierw czas
				time_t receiveTime = time(NULL);
				char* timeString = ctime(&receiveTime);				
				strtok(timeString, "\n");

				// Sprawdzamy czy nie dostalismy exit od klienta
				if(strcmp(buff, "exit") == 0){
					// Zamykamy kolejke
					mq_close(clients[clientNo].queue);
					// Kolejka jest wolna
					clients[clientNo].queue = -1;
					// Wypisujemy
					sprintf(message, " %s Uzytkownik o nicku %s wyszedl z czatu \n",timeString,clients[clientNo].name);
					printf("\n%s\n",message);
					needToNotify = 0;
					// Mniej aktywnych klientow
					activeClients--;
				} else {
					sprintf(message, "%s %s: %s\n", timeString, clients[clientNo].name, buff);
				}
				// Porozsylamy wiadomosci
				for(int i=0; i < clientsCount; i++) {	
					#ifdef DEBUG		
					fprintf(stderr, "Wysylam wiadomosc do %s \n", clients[i].name);
					#endif		
					if(mq_send(clients[i].queue, message, strlen(message) + 1, (unsigned int) 0) ==-1 && errno != EBADF){
							fprintf(stderr, "Blad przy wysylaniu wiadomosci do %s \n", clients[i].name);
							exit(1);
					}					
				}

			}

			if(needToNotify){
				if(mq_notify(clients[clientNo].queue, &clients[clientNo].sigev) ==-1){
					fprintf(stderr, "Blad funkcji notyfikacji dla klienta %s \n", clients[clientNo].name);
					exit(1);
				}
			}

	}

}

// Obsluga zakonczenia
void exitFunction() {
	mq_unlink("/server_queue");
}

// Obsluga sygnalu wysylanego przez ctrl+c
void intHandler(int signo) {
	if(signo == SIGINT) {
		exit(0);
	}
}
