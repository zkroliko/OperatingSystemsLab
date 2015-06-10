#ifndef HEADERS
#define HEADERS
#include "headers.h"
#endif

// Zmienne globalne

pthread_t philTable[N_PHILO];

int main(int argc, char** argv) {

	// Tworzymy monitor dla watkow

	myMonitor* monitor = init_monitor();

	data_t* threadData;

	for (int i = 0; i < N_PHILO ; i++) {

		// Tworzymy dane dla watku
		// ciagle z tym samym monitorem
		threadData = init_data(monitor);

		threadData->left = (i);
		threadData->right = (i + 1) % N_PHILO;
		threadData->philo = i;
		if(pthread_create( &philTable[i], NULL, philosopher, (void*) threadData) != 0) {
			perror("Blad przy tworzeniu watku");
			exit(-1);
		}
	}


	// Laczymy watki
	for (int i = 0; i < N_PHILO ; i++) {
		if ( pthread_join( philTable[i], NULL) < 0)	{
			perror(" Blad funkcji join");
			exit(-1);
		}
	}

}

void* philosopher (void* data) {

	// Wyciaganie niezbednych informacji

	myMonitor* monitor = ((data_t*)data)->monitor;
	pthread_mutex_t* mutex = monitor->mutex;

	pthread_mutex_lock(mutex);

	int left = ((data_t*)data)->left;
	int right = ((data_t*)data)->right;

	// Najpierw wypisze jakies informacje po sobie 

	printf("Tu filozof %d\n",((data_t*)data)->philo);
 
	printf("Mam po lewej stronie widelec %d\n", left);
 
	printf("Mam po prawej stronie widelec %d\n", right);

	pthread_mutex_unlock(mutex);

	for (int i = 0; i < N_EAT_TIMES; i++) {
		
		// Miejsce przy stole

		do {
			pthread_mutex_lock(mutex);		
				// Wypisanie	
			printf("Filozof %d pyta kelnera.\n",((data_t*)data)->philo);
 
			if ((monitor->nEating) < N_PHILO - 1) {	
					
					printf(">>>> Kelner prosi filozofa %d do stolu.\n",((data_t*)data)->philo);	
					monitor->nEating += 1;					
					pthread_mutex_unlock(mutex);
					break;			
			} else {			
					printf("Kelner zbywa filozofa %d.\n",((data_t*)data)->philo);
					pthread_cond_wait(&(monitor->waiter), mutex);
			}	

			
			pthread_mutex_unlock(mutex);
		} while (1);
		
		// left widelec
	
		do {
			pthread_mutex_lock(mutex);
 
			printf("\tFilozof %d oglada sie za lewym widelcem.\n",((data_t*)data)->philo);
 
			if ((monitor->fTaken[left]) == 0) {		
				printf("\tFilozof %d lapie lewy widelec. --< \n",((data_t*)data)->philo);		
				monitor->fTaken[left] = 1;	
				pthread_mutex_unlock(mutex);
				break;				

			} else {			
				printf("\tFilozof %d czeka az ktos mu zwolni lewy widelec.\n",((data_t*)data)->philo);
				pthread_cond_wait(&(monitor->forks[left]), mutex);
			}				
			pthread_mutex_unlock(mutex);
		} while (1);

		// right widelec

		do {
			pthread_mutex_lock(mutex);
 
			printf("\tFilozof %d oglada sie za prawym widelcem.\n",((data_t*)data)->philo);
 
			if ((monitor->fTaken[right]) == 0) {			
				printf("\tFilozof %d lapie prawy widelec. >--\n",((data_t*)data)->philo);		
				monitor->fTaken[right]= 1;	
				pthread_mutex_unlock(mutex);
				break;											
			} else {		
				printf("\tFilozof %d czeka az ktos mu zwolni prawy widelec.\n",((data_t*)data)->philo);
				pthread_cond_wait(&(monitor->forks[right]), mutex);
			}				
			pthread_mutex_unlock(mutex);
		} while (1);

		// Teraz je
		
		printf("\tFilozof %d je. --< o >-- \n",((data_t*)data)->philo);
		eat();

		// Odkladanie widelcow

		pthread_mutex_lock(mutex);

		/* left widelec	*/

		printf("\tFilozof %d odklada lewy widelec. \n",((data_t*)data)->philo);	

		monitor->fTaken[left] = 0;
		pthread_cond_signal(&(monitor->forks[left]));
		/* right widelec */

		printf("\tFilozof %d odklada prawy widelec.\n",((data_t*)data)->philo);	

		monitor->fTaken[right] = 0;
		pthread_cond_signal(&(monitor->forks[right]));

		/* Odejscie od stolu */

		printf("<<<< Filozof %d pozdrawia wspoltowarzyszy i odchodzi od stolu.\n",((data_t*)data)->philo);	

		monitor->nEating -= 1;
		pthread_cond_signal(&(monitor->waiter));
						
		pthread_mutex_unlock(mutex);

		// Teraz mysli	
		printf("Filozof %d mysli. ??? \n",((data_t*)data)->philo);
		think();
	
	}

	pthread_exit(NULL);	
}

void eat() {
	srand(time(NULL));
	usleep((US_EAT_TIME + rand()%US_EAT_TIME_VAR)*1000);
}

void think() {
	srand(time(NULL));
	usleep((US_THINK_TIME + rand()%US_THINK_TIME_VAR)*1000);
}

// Funkcja sluzaca do alokacji danych watku
data_t* init_data (myMonitor* monitor) {

	data_t* data = malloc (sizeof(data_t));

	if (data == NULL) {	
		perror("Blad przy alokacji pamieci");
		exit(0);
	}

	data->monitor = monitor;

	return data;
}

// Funckja sluzaca do alokacji monitora
myMonitor* init_monitor () {

	// Tworzymy monitor
	
	myMonitor* monitor = malloc (sizeof(myMonitor));

	if (monitor == NULL) {	
		perror("Blad przy alokacji pamieci");
		exit(0);
	}

	// Tworzymy mutex	
	pthread_mutex_t* mutex = malloc (sizeof(pthread_mutex_t));

	if (mutex == NULL) {	
		perror("Blad przy alokacji pamieci");
		exit(0);
	}

	if ((pthread_mutex_init(mutex, NULL)) != 0) {
		perror("Blad przy inicjalizacji mutexu");
		exit(0);
	}

	// Ustawiamy pola struktury
	monitor->mutex = mutex;

	pthread_cond_init((monitor->forks), NULL);	
	pthread_cond_init(&(monitor->waiter), NULL);

	return monitor;
}


