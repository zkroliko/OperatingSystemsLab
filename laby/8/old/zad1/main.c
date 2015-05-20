#include "headers.h"


/* define a variable to hold the key, once created.         */
	pthread_key_t list_key;
/* it is specific to our program, not to TSD                */
void* cleanup_list(void* mem) {
	printf("Czyszczenie pamieci");
	free(mem);
}

	/* function to be executed by the new thread */
void* do_loop(void* data) {

	    int i;
	    int me = *((int*)data);     /* thread identifying number */

	    for (i=0; i<1000; i++) {
		usleep(1000*20);
		printf("'%u' - Got '%d'\n", pthread_self(), i);
	    }

	    /* terminate the thread */

	pthread_exit(NULL);
}

/* like any C program, program's execution begins in main */
int main(int argc, char* argv[]) {
	int        thr_id;         /* thread ID for the newly created thread */
	pthread_t  p_thread;       /* thread's structure                     */
	int        a         = 1;  /* thread 1 identifying number            */
	int        b         = 2;  /* thread 2 identifying number            */


	/* create a new thread that will execute 'do_loop()' */
	thr_id = pthread_create(&p_thread, NULL, do_loop, (void*)&a);

	/* rc is used to contain return values of pthread functions */
	int rc;

	/* cleanup_list is a function that can clean up some data   */
	/* it is specific to our program, not to TSD                */
	extern void* cleanup_list(void*);

	/* create the key, supplying a function that'll be invoked when it's deleted. */
	rc = pthread_key_create(&list_key, cleanup_list);


		pthread_detach((&p_thread));


	

	//pthread_join(p_thread, NULL);
    
    /* NOT REACHED */
return 0;
}

