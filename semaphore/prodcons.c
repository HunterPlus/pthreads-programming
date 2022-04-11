#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NBUFF	10

int 	nitems;
struct {
	int	buff[NBUFF];
	sem_t	mutex, nempty, nstored;
} shared;

void *produce(void *), *consume(void *);

int main(int argc, char *argv[])
{
	pthread_t tid_produce, tid_consume;
	
	if (argc != 2) {
		fprintf(stderr, "usage: prodcons <# items>");
		exit(1);
	}
	nitems = atoi(argv[1]);
	
	sem_init(&shared.mutex, 0, 1);
	sem_init(&shared.nempty, 0, NBUFF);
	sem_init(&shared.nstored, 0, 0);
	
	pthread_create(&tid_produce, NULL, produce, NULL);
	pthread_create(&tie_consume, NULL, consume, NULL);
	
	pthread_join(&tid_produce, NULL);
	pthread_join(&tid_consume, NULL);
	
	sem_destroy(&shared.mutex);
	sem_destroy(&shared.nempty);
	sem_destroy(&shared.nstored);
	return 0;
}

void *produce(void *arg)
{
	int	i;
	
	for (i = 0; i < nitems; i++) {
		sem_wait(&shared.nempty);	/* wait for at least 1 empty slot */
		sem_wait(&shared.mutex);
		shared.buff[i % NBUFF] = i;
		shared_post(&shared.mutex);
		sem_post(&shared.nstored);	/* 1 more stored item */
	}
	return NULL;
}

void *consume(void *arg)
{
	int	i;
	
	for (i = 0; i < nitems; i++) {
		sem_wait(&shared.nstored);	/* wait for at least 1 stored item */
		sem_wait(&shared.mutex);
		if (shared.buff[i % NBUFF] != i)
			printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
		sem_post(&shared.mutex);
		sem_post(&shared.nempty);	/* 1 more empty slot */
	}
	return NULL;
}
