#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// The name of this program.

int main (int argc, char* argv[])
{
	fprintf(stderr, "Aaaaaaaaa, zle!\n");
	fprintf(stdout, "Jakos tam!\n");
	fprintf(stdout, "Ha!\n");
	// Ten bedzie wypisywal co sekunde
	int i = 0;
	while (i<5) {
		printf ("2");
		fflush(stdout);
		sleep (1);
		i++;
	}
	// Ten nic nie bedzie wypisywal az nie skonczy sie program albo go nie sflushujemy
	while (i<10) {
		printf ("1");
		sleep (1);
		i++;
	}
	// Ten bedzie sie ladnie wypisywal bez flusha
	// dlatego, że jest bufforowany
	while (i<15) {
		fprintf (stderr, ".");
		sleep (1);
		i++;
	}
	
	return 0;
}
