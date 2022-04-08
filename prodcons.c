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
	
}
void *produce(void *arg)
{
	
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
