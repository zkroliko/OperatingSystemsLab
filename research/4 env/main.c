#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// The name of this program.

int main (int argc, char* argv[])
{
	printf("Your username is: %s\n",getenv("USER"));
	printf("Your home is at: %s\n",getenv("HOME"));
	printf("Your PATH is: %s\n",getenv("PATH"));	
	return 0;
}
