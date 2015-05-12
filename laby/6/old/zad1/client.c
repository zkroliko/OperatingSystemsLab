#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

int main (int argc, char* argv[]) {

	// Sprawdzenie argumentow
	if (argc != 2) {
		if (argc > 2) {
			perror("Podano za duzo argumentow. Prosze podac tylko nazwe klienta");
		} else {
			perror("Nie podano arguemntu. Prosze podac nazwe klienta");
		}
		return -1;		
	}

	key_t   mykey;
        mykey = ftok(".", KEY);

	int msqid;

	// Otwieram kolejke
	if ((msqid = msgget(mykey, 777)) < 0 )	{
		perror("Blad przy otwieraniu kolejki");
		//return -1;			
	}

	struct msg = message;

	if (msgsnd(msqid, msg, sizeof(msg), 0) == -1) {
		perror("Blad wysylania wiadomosci");
	}

	// Zamykam kolejke

	struct msqid_ds;

	if ((msgctl(msqid, IPC_RMID, NULL)) < 0 )	{
		perror("Blad przy zamykaniu kolejki");
		return -1;			
	}

	return 0;
}
