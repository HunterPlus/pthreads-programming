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
	PTHREAD_MUTEX_INITIALIZER
};

struct {
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	int	nready;
} nready = {
	PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER
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

void *produce(void *arg)
{
	while (1) {
		pthread_mutex_lock(&put.mutex);
		if (put.nput >= nitems) {
			pthread_mutex_unlock(&put.mutex);
			return NULL;
		}
		buff[put.nput] = put.nval;
		put.nput++;
		put.nval++;
		pthread_mutex_unlock(&put.mutex);
		
		pthread_mutex_lock(&nready.mutex);
		if (nready.nready == 0)
			pthread_cond_signal(&nready.cond);
		nready.nready++;
		pthread_mutex_unlock(&nready.mutex);
		
		*((int *) arg) += 1;
	}
}

void *consume(void *arg)
{
	int	i;
	
	for (i = 0; i < nitems; i++) {
		pthread_mutex_lock(&nready.mutex);
		while (nready.nready == 0)
			pthread_cond_wait(&nready.cond, &nready.mutex);
		nready.nready--;
		pthread_mutex_unlock(&nready.mutex);
		
		if (buff[i] != i)
			printf("buff[%d] = %d\n", i, buff[i]);
	}
	return NULL;
}

void Pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start)(void *), void *arg)
{
	if (pthread_create(thread, attr, start, arg) != 0) {
		fprintf(stderr, "thread create error.\n");
		exit(1);
	}
}
