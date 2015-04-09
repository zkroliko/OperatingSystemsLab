#ifndef HEADERS
	#define HEADERS
	#include "headers.h"
#endif

int test_threads(int (*threads)(int NThreads, long* child_time), int NThreads);
int increment(void* arg);
int threads_fork (int NThreads, long* child_time);
int threads_vfork (int NThreads, long* child_time);
int threads_clone (int NThreads, long* child_time);
int threads_vclone (int NThreads, long* child_time);
