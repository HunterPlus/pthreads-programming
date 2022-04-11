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
