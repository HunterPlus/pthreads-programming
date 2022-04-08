#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define min(a,b) (((a)<(b))?(a):(b))
#define MAXNITEMS	1000000
#define MAXNTHREAD	50
void Thread_create(pthread_t *, pthread_attr_t *, void *(*)(void *), (void *));
void *produce(void *), *consume(void *);
