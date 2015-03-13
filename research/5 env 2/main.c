#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* The ENVIRON variable contains the environment. */
extern char** environ;

int main (int argc, char* argv[])
{
	char** var;
	for (var = environ; *var != NULL; ++var)
	printf ("%s\n", *var);
	return 0;	

}
