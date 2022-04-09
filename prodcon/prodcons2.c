#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define min(a,b) (((a)<(b))?(a):(b))
#define MAXNITEMS	1000000
#define MAXNTHREADS	50

void Pthread_create(pthread_t *, pthread_attr_t *, void *(*)(void *), void *);
void *produce(void *), *consume(void *);
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

int main(int argc, char *argv[])
{
	int i, nthreads, count[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS], tid_consume;
	
	if (argc != 3) {
		fprintf(stderr, "arguments error.\n");
		exit(1);
	}
	nitems = min(atoi(argv[1]), MAXNITEMS);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);
	for (i = 0; i < nthreads; i++) {
		count[i] = 0;
		Pthread_create(&tid_produce[i], NULL, produce, &count[i]);		
	}
	Pthread_create(&tid_consume, NULL, consume, NULL);
	
	/* wait for all produces and consume thread */
	for (i = 0; i < nthreads; i++) {
		pthread_join(tid_produce[i], NULL);
		printf("count[%d] = %d\n", i, count[i]);
	}	
	pthread_join(tid_consume, NULL);
	return 0;	
}


void Pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start)(void *), void *arg)
{
	if (pthread_create(thread, attr, start, arg) != 0) {
		fprintf(stderr, "thread create error.\n");
		exit(1);
	}
}
