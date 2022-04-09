#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXNITEMS	1000000
#define MAXNTHREADS	50

int nitems;
int buff[MAXNITEMS];
struct {
	pthread_mutex_t		mutex;
	int 	nput;
	int	nval;
} put = {
	PTHREAD_MUTEX_INITIALIZAER
};

struct {
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	int	nready;
} nready = {
	PTHREAD_MUTEX_INITIALIZAER, PTHREAD_COND_INITIALIZER
};

