#ifndef HEADERS
	#define HEADERS
	#include "headers.h"
#endif
#ifndef FUN
	#define FUN
	#include "functions.h"
#endif

#define STACK_SIZE 64 // Starczy na nasz ministosik	

static int counter;

// Do testowania, przyjmuje wskaznik na funkcje
int test_threads(int (*threads)(int NThreads, long* child_time), int NThreads) {

	// Struktury dla time
	clock_t timeStart;
	clock_t timeEnd;

	// Cos dla czasu dzieci
	long* child_time;
	if((child_time = malloc(sizeof(long))) == NULL) {
		fprintf(stderr, "Bledna alokacja pamieci!\n");
		return -1;		
	}

	// Iniciujemy struktury dla funkcji rusage
	struct rusage* usage_before;
	struct rusage* usage_child_before;
	struct rusage* usage_after;
	struct rusage* usage_child_after;
	if ( (usage_before = malloc(sizeof (struct rusage))) == NULL ||
		(usage_child_before = malloc(sizeof (struct rusage))) == NULL ||
		(usage_after = malloc(sizeof (struct rusage))) == NULL ||
		(usage_child_after = malloc(sizeof (struct rusage))) == NULL ) {
		fprintf(stderr, "Bledna alokacja pamieci!\n");
		return -1;
	}
	
	if (getrusage(RUSAGE_SELF, usage_before) < 0)
	{
		fprintf(stderr,"Blad w funkcji getrusage.\n");
		return -1;
	}
	if (getrusage(RUSAGE_CHILDREN, usage_child_before) < 0)
	{
		fprintf(stderr,"Blad w funkcji getrusage.\n");
		return -1;
	}
		
	// Generujemy czasy poczatkowe
	timeStart = clock();


	//------------------- Test dla funkcji
	int val;
	val = threads(NThreads, child_time);

	//Wypisuje czasy
	timeEnd = clock();


	if (getrusage(RUSAGE_SELF, usage_after) < 0)
	{
		fprintf(stderr, "Blad w funkcji getrusage.\n");
		return -1;
	}
	if (getrusage(RUSAGE_CHILDREN, usage_child_after) < 0)
	{
		fprintf(stderr, "Blad w funkcji getrusage.\n");
		return -1;
	}

	// Sprawdzmy czy zadeklarowano wypisywanie w formacie CSV
#ifdef CSV
	printf( "%d, %d, %f,", NThreads , val,((double)(timeEnd - timeStart) / (double)(CLOCKS_PER_SEC)));
	printf("%f,", (double) *child_time/CLOCKS_PER_SEC);
	printf("%f,", (float) (usage_after->ru_utime.tv_sec-usage_before->ru_utime.tv_sec) 
						+ (usage_after->ru_utime.tv_usec-usage_before->ru_utime.tv_usec)/1E6);
	printf("%f,",	(float) (usage_after->ru_stime.tv_sec-usage_before->ru_stime.tv_sec)
						+ ((usage_after->ru_stime.tv_usec-usage_before->ru_stime.tv_usec)/1E6));
	printf("%f,", (float) (usage_child_after->ru_utime.tv_sec-usage_child_before->ru_utime.tv_sec) 
						+ (usage_child_after->ru_utime.tv_usec-usage_child_before->ru_utime.tv_usec)/1E6);
	printf("%f\n",	(float) (usage_child_after->ru_stime.tv_sec-usage_child_before->ru_stime.tv_sec)
						+ ((usage_child_after->ru_stime.tv_usec-usage_child_before->ru_stime.tv_usec)/1E6));
	// Jak nie to wypisujemy ladnie
#else
	printf("Koncowa wartosc zmiennej counter:\t%d\n", val);

	printf( "Czas rzeczywisty procesu macierzystego: %f\n", ((double)(timeEnd - timeStart) / (double)(CLOCKS_PER_SEC)));
	printf("Czas rzeczywisty procesow potomnych: %f\n", (double) *child_time/CLOCKS_PER_SEC);
	printf("----Czasy procesu macierzystego:----\n");
	printf("Uzytkownika: %f\n", (float) (usage_after->ru_utime.tv_sec-usage_before->ru_utime.tv_sec) 
						+ (usage_after->ru_utime.tv_usec-usage_before->ru_utime.tv_usec)/1E6);
	printf("Systemu: %f\n",	(float) (usage_after->ru_stime.tv_sec-usage_before->ru_stime.tv_sec)
						+ ((usage_after->ru_stime.tv_usec-usage_before->ru_stime.tv_usec)/1E6));
	printf("----Czasy procesow-dzieci:----\n");
	printf("Uzytkownika: %f\n", (float) (usage_child_after->ru_utime.tv_sec-usage_child_before->ru_utime.tv_sec) 
						+ (usage_child_after->ru_utime.tv_usec-usage_child_before->ru_utime.tv_usec)/1E6);
	printf("Systemu: %f\n",	(float) (usage_child_after->ru_stime.tv_sec-usage_child_before->ru_stime.tv_sec)
						+ ((usage_child_after->ru_stime.tv_usec-usage_child_before->ru_stime.tv_usec)/1E6));
#endif

	// Zwalnianie pamieci
	free(child_time);
	free(usage_before);
	free(usage_after);
	free(usage_child_before);
	free(usage_child_after);

	return 0;
}

int threads_fork (int NThreads, long* child_time) {
	counter = 0;
	*child_time = 0;
	clock_t start, end;
	int PID;

	int* returnValue = malloc(sizeof(int));

	for (int i = 0; i < NThreads; ++i) {
		start = clock();
		if ((PID = fork()) == 0 ) {			
			counter = counter + 1;
			//printf("Proces potomny ustawia counter na: %d\n", counter);
			_exit(0);
		} else if (PID > 0) {
			// W trakcie trwania waitpid mierzymy czas trwania procesu dziecka
			waitpid(PID, returnValue, 0);
			end = clock();
			*child_time += end - start;
			if (!WIFEXITED(*returnValue)) {
				fprintf(stderr,"Proces potomny wyszedl z bledem!");
				return -1;
			}				
		} else {
			fprintf(stderr,"Blad funkcji fork!");
			return -1;		
		}
	}
	free(returnValue);
	return counter;
}

int threads_vfork (int NThreads, long* child_time) {
	counter = 0;
	*child_time = 0;
	clock_t start, end;
	int PID;

	int* returnValue = malloc(sizeof(int));

	for (int i = 0; i < NThreads; ++i) {
		start = clock();
		if ((PID = vfork()) == 0 ) {
			counter = counter + 1;
			//printf("Proces potomny ustawia counter na: %d\n", counter);
			_exit(0);
		} else if (PID > 0) {
			// W trakcie trwania waitpid mierzymy czas trwania procesu dziecka
			waitpid(PID, returnValue, 0);
			end = clock();
			*child_time += end - start;
			if (!WIFEXITED(*returnValue) ) {
				fprintf(stderr,"Proces potomny wyszedl z bledem!");	
				return -1;
			}				
		} else {
			fprintf(stderr,"Blad funkcji fork!");	
			return -1;	
		}
	}
	free(returnValue);
	return counter;
}

// To dla funkcji clone
int increment(void* arg)
{
	counter++;
	return 0;
}

int threads_clone (int NThreads, long* child_time) {
	counter = 0;
	int PID;
	clock_t start, end;
	*child_time = 0;

	int* returnValue = malloc(sizeof(int));

	for (int i = 0; i < NThreads; ++i) {
		start = clock();
		void* child_stack = malloc(STACK_SIZE);	
		PID = clone(&increment, (char*)child_stack+STACK_SIZE, CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD , NULL);
		if (PID < 0) { 
			fprintf(stderr, "Blad przy wowolaniu funkcji clone!\n");
			return -1;
		}
		waitpid(PID, returnValue, 0);
		end = clock();
		if(!(WIFEXITED(*returnValue)))
		{
			fprintf(stderr,"Proces potomny wyszedl z bledem!");	
			return -1;
		}
		
		*child_time += end-start;
		free(child_stack);
	}
	free(returnValue);
	return counter;
}

int threads_vclone (int NThreads, long* child_time) {
	counter = 0;
	int PID;
	clock_t start, end;
	*child_time = 0;

	int* returnValue = malloc(sizeof(int));
	char* child_stack = malloc(STACK_SIZE);

	for (int i = 0; i < NThreads; ++i) {
		start = clock();
		PID = clone(&increment, (char*)(child_stack+STACK_SIZE), SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_VM|CLONE_VFORK, NULL);
		if (PID < 0) { 
			fprintf(stderr, "Blad przy wowolaniu funkcji clone!\n");
			return -1;
		}
		waitpid(PID, returnValue, 0);
		end = clock();
		if(!(WIFEXITED(*returnValue)))
		{
			fprintf(stderr,"Proces potomny wyszedl z bledem!");	
			return -1;
		}		
		*child_time += end-start;
		
	}
	free(child_stack);
	free(returnValue);
	return counter;
}
