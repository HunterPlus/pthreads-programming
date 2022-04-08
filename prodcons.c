#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define min(a,b) (((a)<(b))?(a):(b))
#define MAXNITEMS	1000000
#define MAXNTHREAD	50
void Thread_create(pthread_t *, pthread_attr_t *, void *(*)(void *), (void *));
void *produce(void *), *consume(void *);

int nitems;	/* read only by produce and consume */
struct {
	pthread_mutex	mutex;
	int		buff[MAXNITEMS];
	int		nput;
	int		nval;
} shared = {
	PTHREAD_MUTEX_INITIALIZER
}；

int main(int argc, char *argv[])
{
	int i, nthreads, count[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS], tid_consume;
	
	if (argc ! = 3) {
		fprintf(stderr, "arguments error.\n");
		exit(1);
	}
	nitems = min(atoi(argv[1]), MAXNITEMS);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);
	for (i = 0; i < nthreads; i++) {
		count[i] = 0;
		Pthread_create(&tid_produce[i], NULL, produce, &count[i]);		
	}
	for (i = 0; i < nthreads; i++) {
		pthreads_join(tid_produce[i], NULL);
		printf("count[%d] = %d\n", i, count[i]);
	}
	
	/* start, then wait for the consume thread */
	Pthread_create(&tid_consume, NULL, consume, NULL);
	pthread_join(tid_consume, NULL);
	return 0;	
}
void *produce(void *arg)
{
	pthread_mutex_lock(&shared.mutex);
	if (shared.nput >= nitems)  {
		pthread_mutex_unlock(&shared.mutex);
		return NULL;
	}
	shared.buff[shared.nput] = shared.nval;
	shared.nput++;
	shared.nval++;
	pthread_mutex_unlock(&shared.mutex);
	*((int *) arg) += 1;
}
void *consume(void *arg)
{
	
}
void Pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start)(void *), (void *)arg)
{
	if (pthread_create(thread, NULL, start, arg) != 0) {
		fprintf(stderr, "thread create error.\n");
		exit;
	}
}
